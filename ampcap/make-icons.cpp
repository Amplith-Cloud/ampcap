/*
    AmpCap
    make-icons.cpp: renders the app icon (the Amplithian infinity on an
    Amplithian Blue tile, drawn by ampcap_brand.h) and writes icon1.ico
    (Windows) and ampcap.icns (macOS) as PNG-compressed icon sets.

    Build and run with make-icons.sh.
*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "../WDL/lice/lice.h"
#include "../WDL/zlib/zlib.h"
#include "ampcap_brand.h"

static void put32be(std::string &s, unsigned int v)
{
  s += (char)(v>>24); s += (char)(v>>16); s += (char)(v>>8); s += (char)v;
}
static void put16le(std::string &s, unsigned int v) { s += (char)v; s += (char)(v>>8); }
static void put32le(std::string &s, unsigned int v) { put16le(s, v & 0xffff); put16le(s, v >> 16); }

static void png_chunk(std::string &out, const char *type, const std::string &data)
{
  put32be(out, (unsigned int)data.size());
  std::string td = std::string(type, 4) + data;
  out += td;
  put32be(out, (unsigned int)crc32(0, (const Bytef *)td.data(), (uInt)td.size()));
}

// 8-bit RGBA PNG, not premultiplied
static std::string encode_png(const unsigned char *rgba, int w, int h)
{
  std::string raw;
  for (int y = 0; y < h; y++)
  {
    raw += (char)0;
    raw.append((const char *)rgba + y*w*4, w*4);
  }
  uLongf zl = compressBound((uLong)raw.size());
  std::string z(zl, 0);
  compress2((Bytef *)&z[0], &zl, (const Bytef *)raw.data(), (uLong)raw.size(), 9);
  z.resize(zl);

  std::string ihdr;
  put32be(ihdr, w); put32be(ihdr, h);
  ihdr += (char)8; ihdr += (char)6; ihdr += (char)0; ihdr += (char)0; ihdr += (char)0;

  std::string out("\x89PNG\r\n\x1a\n", 8);
  png_chunk(out, "IHDR", ihdr);
  png_chunk(out, "IDAT", z);
  png_chunk(out, "IEND", std::string());
  return out;
}

// Rounded-square coverage of pixel (x,y), 4x4 supersampled
static double tile_coverage(int x, int y, double x0, double y0, double x1, double y1, double rad)
{
  int hits = 0;
  for (int sy = 0; sy < 4; sy++)
    for (int sx = 0; sx < 4; sx++)
    {
      const double px = x + (sx + 0.5)/4.0, py = y + (sy + 0.5)/4.0;
      if (px < x0 || px > x1 || py < y0 || py > y1) continue;
      const double cx = px < x0+rad ? x0+rad : px > x1-rad ? x1-rad : px;
      const double cy = py < y0+rad ? y0+rad : py > y1-rad ? y1-rad : py;
      if ((px-cx)*(px-cx) + (py-cy)*(py-cy) <= rad*rad) hits++;
    }
  return hits / 16.0;
}

static std::string render_icon(int size)
{
  LICE_MemBitmap bm(size, size);
  LICE_Clear(&bm, AMPCAP_BLUE);

  const double margin = size >= 64 ? size * 0.06 : 0.0;
  const double inner = size - margin*2;
  const int lw = size < 32 ? size : (int)(inner * 0.84 + 0.5);
  const int lh = size < 32 ? (size+1)/2 : (int)(lw / 2.6 + 0.5);
  ampcap_draw_infinity(&bm, (size - lw)/2, (size - lh)/2, lw, lh);

  std::string rgba(size*size*4, 0);
  const LICE_pixel *bits = bm.getBits();
  for (int y = 0; y < size; y++)
    for (int x = 0; x < size; x++)
    {
      const LICE_pixel p = bits[y*bm.getRowSpan() + x];
      const double a = tile_coverage(x, y, margin, margin, size - margin, size - margin, inner * 0.22);
      unsigned char *o = (unsigned char *)&rgba[(y*size + x)*4];
      o[0] = (unsigned char)LICE_GETR(p);
      o[1] = (unsigned char)LICE_GETG(p);
      o[2] = (unsigned char)LICE_GETB(p);
      o[3] = (unsigned char)(a*255.0 + 0.5);
    }
  return encode_png((const unsigned char *)rgba.data(), size, size);
}

static bool write_file(const char *fn, const std::string &data)
{
  FILE *f = fopen(fn, "wb");
  if (!f) { fprintf(stderr, "can't write %s\n", fn); return false; }
  fwrite(data.data(), 1, data.size(), f);
  fclose(f);
  printf("wrote %s (%d bytes)\n", fn, (int)data.size());
  return true;
}

int main(int argc, char **argv)
{
  const char *dir = argc > 1 ? argv[1] : ".";
  std::string fn;

  // Windows: PNG-compressed entries, read by Windows Vista and later
  static const int ico_sizes[] = { 16, 20, 24, 32, 40, 48, 64, 256 };
  const int ico_n = sizeof(ico_sizes)/sizeof(ico_sizes[0]);
  std::string ico, ico_data;
  put16le(ico, 0); put16le(ico, 1); put16le(ico, ico_n);
  for (int i = 0; i < ico_n; i++)
  {
    const int s = ico_sizes[i];
    const std::string png = render_icon(s);
    ico += (char)(s >= 256 ? 0 : s); ico += (char)(s >= 256 ? 0 : s);
    ico += (char)0; ico += (char)0;
    put16le(ico, 1); put16le(ico, 32);
    put32le(ico, (unsigned int)png.size());
    put32le(ico, 6 + 16*ico_n + (unsigned int)ico_data.size());
    ico_data += png;

  }
  if (!write_file((fn = std::string(dir) + "/icon1.ico").c_str(), ico + ico_data)) return 1;

  // macOS: PNG icon families (OS X 10.7 and later)
  static const struct { const char *type; int size; } icns_types[] = {
    { "icp4", 16 }, { "icp5", 32 }, { "icp6", 64 }, { "ic07", 128 }, { "ic08", 256 },
    { "ic09", 512 }, { "ic10", 1024 }, { "ic11", 32 }, { "ic12", 64 }, { "ic13", 256 }, { "ic14", 512 },
  };
  std::string icns_body;
  for (size_t i = 0; i < sizeof(icns_types)/sizeof(icns_types[0]); i++)
  {
    const std::string png = render_icon(icns_types[i].size);
    icns_body += std::string(icns_types[i].type, 4);
    put32be(icns_body, 8 + (unsigned int)png.size());
    icns_body += png;
  }
  std::string icns("icns");
  put32be(icns, 8 + (unsigned int)icns_body.size());
  return write_file((fn = std::string(dir) + "/ampcap.icns").c_str(), icns + icns_body) ? 0 : 1;
}

/*
    AmpCap
    ampcap_brand.h: Amplithian header band and infinity mark, drawn with LICE

    Copyright (C) 2026 Amplithian Holdings LLC

    AmpCap is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/

#ifndef _AMPCAP_BRAND_H_
#define _AMPCAP_BRAND_H_

// needs <math.h>, included before lice.h (it breaks after SWELL's min/max macros)

// Amplithian brand tokens (AF-2026-0318)
#define AMPCAP_BLUE        LICE_RGBA(0x25,0x63,0xFF,255) // Amplithian Blue
#define AMPCAP_VIOLET      LICE_RGBA(0x7C,0x5C,0xFF,255) // Resonance Violet
#define AMPCAP_LIGHT_CORE  LICE_RGBA(0xF4,0xF8,0xFF,255)
#define AMPCAP_BLUE_30     LICE_RGBA(0xB5,0xCC,0xFF,255) // Blue tonal 30
#define AMPCAP_BLUE_170    LICE_RGBA(0x0A,0x1A,0x4D,255) // Blue tonal 170

// Antialiased stroke along a sampled curve: each pixel's coverage comes from
// its distance to the nearest segment. The curves are tiny (header-sized),
// so brute force is fine and avoids LICE's thin-line-only AA.
struct ampcap_curve
{
  enum { N = 192 };
  double x[N+1], y[N+1];

  double dist(double px, double py) const
  {
    double best = 1e9;
    for (int i = 0; i < N; i++)
    {
      const double ax = x[i], ay = y[i], dx = x[i+1]-ax, dy = y[i+1]-ay;
      const double l2 = dx*dx + dy*dy;
      double t = l2 > 0.0 ? ((px-ax)*dx + (py-ay)*dy) / l2 : 0.0;
      if (t < 0.0) t = 0.0; else if (t > 1.0) t = 1.0;
      const double ex = ax + t*dx - px, ey = ay + t*dy - py;
      const double d = ex*ex + ey*ey;
      if (d < best) best = d;
    }
    return sqrt(best);
  }
};

static LICE_pixel ampcap_lerp_color(LICE_pixel a, LICE_pixel b, double t)
{
  if (t < 0.0) t = 0.0; else if (t > 1.0) t = 1.0;
  const int ar = (int)LICE_GETR(a), ag = (int)LICE_GETG(a), ab = (int)LICE_GETB(a);
  return LICE_RGBA((int)(ar + ((int)LICE_GETR(b)-ar)*t + 0.5),
                   (int)(ag + ((int)LICE_GETG(b)-ag)*t + 0.5),
                   (int)(ab + ((int)LICE_GETB(b)-ab)*t + 0.5), 255);
}

// The Amplithian infinity: a lemniscate whose stroke runs cyan to violet,
// a thinner inner ring in each loop, and a bright point at the crossing.
// Drawn over whatever is in bm, inside the w x h box at (x0,y0).
static void ampcap_draw_infinity(LICE_IBitmap *bm, int x0, int y0, int w, int h)
{
  if (!bm || w < 8 || h < 4) return;

  const double outer_w = h >= 24 ? h/10.0 : 1.6;
  const double inner_w = h >= 24 ? h/22.0 : 0.9;
  const double margin = outer_w + 1.0;
  const double cx = x0 + w*0.5, cy = y0 + h*0.5;
  const double sx = w*0.5 - margin, sy = (h*0.5 - margin) / 0.3536;

  ampcap_curve outer, inner_l, inner_r;
  for (int i = 0; i <= ampcap_curve::N; i++)
  {
    const double t = (6.283185307179586*i) / ampcap_curve::N, s = sin(t), c = cos(t);
    // lemniscate of Bernoulli, |y| <= 0.3536
    outer.x[i] = cx + sx * c / (1.0 + s*s);
    outer.y[i] = cy + sy * s * c / (1.0 + s*s);
    // inner rings sit inside each loop without reaching the crossing
    inner_l.x[i] = cx + sx * (-0.56 + 0.34*c);
    inner_l.y[i] = cy + sy * 0.17 * s;
    inner_r.x[i] = cx + sx * ( 0.56 + 0.34*c);
    inner_r.y[i] = cy + sy * 0.17 * s;
  }

  const LICE_pixel cyan = LICE_RGBA(0x5F,0xE0,0xFF,255);
  const LICE_pixel lilac = LICE_RGBA(0xC4,0xA8,0xFF,255);
  const double dot_r = outer_w * 0.9 + 0.3;

  for (int py = y0; py < y0+h; py++)
  {
    for (int px = x0; px < x0+w; px++)
    {
      const double fx = px + 0.5, fy = py + 0.5;
      const double gx = (fx - x0) / (double)w; // gradient position, left to right

      // soft dark halo first, so the light stroke reads on Amplithian Blue
      const double d_o = outer.dist(fx, fy);
      const double halo = exp(-(d_o*d_o) / (2.0 * outer_w*outer_w * 2.2)) * 0.35;
      if (halo > 0.01) LICE_PutPixel(bm, px, py, AMPCAP_BLUE_170, (float)halo, LICE_BLIT_MODE_COPY);

      const double d_i = d_o < outer_w*0.5 + 1.0 ? 1e9 :
                         (fx < cx ? inner_l.dist(fx, fy) : inner_r.dist(fx, fy));
      double a = inner_w*0.5 + 0.5 - d_i;
      if (a > 0.0)
        LICE_PutPixel(bm, px, py, ampcap_lerp_color(lilac, cyan, gx), (float)(a > 1.0 ? 0.6 : a*0.6), LICE_BLIT_MODE_COPY);

      a = outer_w*0.5 + 0.5 - d_o;
      if (a > 0.0)
      {
        // cyan through Light Core to violet, brightest near the crossing
        LICE_pixel col = gx < 0.5 ? ampcap_lerp_color(cyan, AMPCAP_LIGHT_CORE, gx*2.0)
                                  : ampcap_lerp_color(AMPCAP_LIGHT_CORE, lilac, gx*2.0 - 1.0);
        LICE_PutPixel(bm, px, py, col, (float)(a > 1.0 ? 1.0 : a), LICE_BLIT_MODE_COPY);
      }

      const double dd = sqrt((fx-cx)*(fx-cx) + (fy-cy)*(fy-cy));
      a = dot_r + 0.5 - dd;
      if (a > 0.0) LICE_PutPixel(bm, px, py, LICE_RGBA(255,255,255,255), (float)(a > 1.0 ? 1.0 : a), LICE_BLIT_MODE_COPY);
    }
  }
}

// Header band: Amplithian Blue, with the infinity mark at the left and the
// brand gradient (#2563FF -> #7C5CFF) as a 2px rule along the bottom edge.
// Returns the x where text should start.
static int ampcap_draw_header(LICE_IBitmap *bm, int w, int h)
{
  if (!bm || w < 1 || h < 1) return 0;
  LICE_FillRect(bm, 0, 0, w, h, AMPCAP_BLUE, 1.0f, LICE_BLIT_MODE_COPY);

  const int rule = h >= 16 ? 2 : 1;
  const float d = 255.0f * (float)w;
  LICE_GradRect(bm, 0, h-rule, w, rule,
                0x25/255.0f, 0x63/255.0f, 1.0f, 1.0f,
                (0x7C-0x25)/d, (0x5C-0x63)/d, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                LICE_BLIT_MODE_COPY);

  const int pad = h / 6 + 1;
  const int lh = h - rule - pad*2;
  const int lw = (int)(lh * 2.9);
  ampcap_draw_infinity(bm, pad + 2, pad, lw, lh);
  return pad + 2 + lw + pad + 2;
}

#endif

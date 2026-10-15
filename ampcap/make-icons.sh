#!/bin/sh
# Regenerates icon1.ico and ampcap.icns. Needs a C and a C++ compiler; uses
# WDL's LICE, libpng and zlib.
#
#   ./make-icons.sh [output dir]
#     the Amplithian infinity drawn in ampcap_brand.h (default: this directory)
#   ./make-icons.sh --from art-512.png [--from favicon-32.png ...] output dir
#     resampled from square brand artwork, e.g. for brands/<name>/
set -e
here=$(cd "$(dirname "$0")" && pwd)
wdl="$here/../WDL"
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

for f in adler32 compress crc32 deflate trees zutil inflate inffast inftrees uncompr; do
  ${CC:-cc} -O2 -w -c "$wdl/zlib/$f.c" -o "$tmp/$f.o"
done
for f in "$wdl"/libpng/*.c; do
  case "$f" in */pngtest.c) continue ;; esac
  ${CC:-cc} -O2 -w -I"$wdl/zlib" -c "$f" -o "$tmp/$(basename "$f" .c).o"
done
# LICE_SysBitmap references SWELL's GDI layer; the generator only uses
# LICE_MemBitmap, so those symbols just need to exist.
echo 'void *SWELL_DeleteGfxContext, *SWELL_GetCtxFrameBuffer, *SWELL_CreateMemContext;' > "$tmp/stubs.c"
${CC:-cc} -c "$tmp/stubs.c" -o "$tmp/stubs.o"
${CXX:-c++} -O2 -w -DSWELL_PROVIDED_BY_APP -I"$wdl" -o "$tmp/make-icons" \
  "$here/make-icons.cpp" "$wdl/lice/lice.cpp" "$wdl/lice/lice_line.cpp" "$wdl/lice/lice_arc.cpp" \
  "$wdl/lice/lice_text.cpp" "$wdl/lice/lice_png.cpp" "$tmp"/*.o
if [ $# -eq 0 ]; then set -- "$here"; fi
"$tmp/make-icons" "$@"

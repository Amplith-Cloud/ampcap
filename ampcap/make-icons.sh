#!/bin/sh
# Regenerates icon1.ico and ampcap.icns from the Amplithian infinity drawn in
# ampcap_brand.h. Needs a C and a C++ compiler; uses WDL's LICE and zlib.
# Usage: ./make-icons.sh [output dir]   (default: this directory)
set -e
here=$(cd "$(dirname "$0")" && pwd)
wdl="$here/../WDL"
out=${1:-$here}
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

for f in adler32 compress crc32 deflate trees zutil; do
  ${CC:-cc} -O2 -w -c "$wdl/zlib/$f.c" -o "$tmp/$f.o"
done
# LICE_SysBitmap references SWELL's GDI layer; the generator only uses
# LICE_MemBitmap, so those symbols just need to exist.
echo 'void *SWELL_DeleteGfxContext, *SWELL_GetCtxFrameBuffer, *SWELL_CreateMemContext;' > "$tmp/stubs.c"
${CC:-cc} -c "$tmp/stubs.c" -o "$tmp/stubs.o"
${CXX:-c++} -O2 -w -DSWELL_PROVIDED_BY_APP -I"$wdl" -o "$tmp/make-icons" \
  "$here/make-icons.cpp" "$wdl/lice/lice.cpp" "$wdl/lice/lice_line.cpp" "$wdl/lice/lice_arc.cpp" \
  "$wdl/lice/lice_text.cpp" "$tmp"/*.o
"$tmp/make-icons" "$out"

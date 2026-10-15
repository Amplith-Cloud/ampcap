/*
    AmpCap
    ampcap_brand_config.h: build branding (Amplithian, the default)

    A branded build replaces this file, icon1.ico and ampcap.icns with the
    ones in brands/<name>/ before compiling (see .github/workflows/build.yml).
*/

#ifndef _AMPCAP_BRAND_CONFIG_H_
#define _AMPCAP_BRAND_CONFIG_H_

#define AMPCAP_BRAND_ID           "amplithian"

// Copyright watermark: default text (after the (C) sign) and colors, 0xRRGGBB
#define AMPCAP_WATERMARK_TEXT     "Amplithian Polyforge"
#define AMPCAP_WATERMARK_PLATE    0x0B1020 // Dark Ink
#define AMPCAP_WATERMARK_MARK_A   0x2563FF // Amplithian Blue, top left of the mark
#define AMPCAP_WATERMARK_MARK_B   0x7C5CFF // Resonance Violet, bottom right
#define AMPCAP_WATERMARK_INK      0xF4F8FF // Light Core

#endif

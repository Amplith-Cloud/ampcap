/*
    AmpCap
    ampcap_brand_config.h: build branding for FM+ (Foundational Music, Inc.)

    Colors from the FM+ web app's dark theme, its original identity
    (fm-prod src/index.css .dark), and the gold FM+ wordmark.
*/

#ifndef _AMPCAP_BRAND_CONFIG_H_
#define _AMPCAP_BRAND_CONFIG_H_

#define AMPCAP_BRAND_ID           "fmplus"

// Copyright watermark: default text (after the (C) sign) and colors, 0xRRGGBB
#define AMPCAP_WATERMARK_TEXT     "Foundational Music, Inc."
#define AMPCAP_WATERMARK_PLATE    0x170326 // --music-darker
#define AMPCAP_WATERMARK_MARK_A   0xFFD24D // --music-gold, top left of the mark
#define AMPCAP_WATERMARK_MARK_B   0x915D28 // wordmark bronze, bottom right
#define AMPCAP_WATERMARK_INK      0xF8FAFC // --foreground

#endif

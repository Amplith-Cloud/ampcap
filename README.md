# AmpCap

AmpCap is the Amplithian fork of [LICEcap](https://www.cockos.com/licecap/), the
lightweight screen-capture-to-GIF/LCF tool by Cockos Incorporated. It tracks the
upstream at https://github.com/justinfrankel/licecap and adds
Amplithian-specific features.

Layout:

- `ampcap/` – the application (Windows: `ampcap/ampcap.sln`; macOS: `ampcap/ampcap.xcodeproj`;
  legacy VC6 workspace `ampcap/ampcap.dsw`)
- `.github/workflows/build.yml` – CI build: 64-bit Windows exe + installer, universal macOS app
- `WDL/` – Cockos WDL library (LICE, SWELL, EEL2, zlib, libpng), unmodified except for a
  one-line zlib fix (`WDL/zlib/zutil.h`) so it compiles with current macOS SDKs

AmpCap is released under the GNU General Public License v2 or later, the same
license as LICEcap. See `ampcap/license.txt`.

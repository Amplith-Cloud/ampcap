# AmpCap

AmpCap is the Amplithian fork of [LICEcap](https://www.cockos.com/licecap/), the
lightweight screen-capture-to-GIF/LCF tool by Cockos Incorporated. It tracks the
upstream mirror at https://github.com/Amplith-Cloud/licecap and adds
Amplithian-specific features.

Layout:

- `ampcap/` – the application (Windows: `ampcap/ampcap.sln`; macOS: `ampcap/ampcap.xcodeproj`;
  legacy VC6 workspace `ampcap/ampcap.dsw`)
- `WDL/` – Cockos WDL library (LICE, SWELL, EEL2, zlib, libpng), used unmodified

AmpCap is released under the GNU General Public License v2 or later, the same
license as LICEcap. See `ampcap/license.txt`.

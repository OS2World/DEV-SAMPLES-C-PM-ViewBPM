# DEV-SAMPLES-C-PM-ViewBPM
OS/2 Presentation Manager bitmap viewer: opens, displays, and inspects OS/2 1.x, OS/2 2.x, and Windows 3.x .BMP files.

## Description

Written by Mark Benge and Matt Smith of Prominare Inc. and published in the
*OS/2 Developer* magazine (November/December 1995), this program is a
fully-functional bitmap viewer demonstrating a range of PM programming
techniques.

The application can:
- Open OS/2 1.x (BITMAPFILEHEADER), OS/2 2.x (BITMAPFILEHEADER2), and
  Windows 3.x BMP files
- Display bitmap array files (multiple bitmaps chained via `offNext`) with
  all images shown side by side
- Scale the current bitmap from 25% to 800% or "show all" mode
- Flip the colour table of palettised bitmaps
- Inspect every field of the binary bitmap headers in separate dialogs
- Show display device capabilities (DevQueryCaps)

## PM Concepts Demonstrated

### Multi-format bitmap loading
`GetBmp.C` reads the entire file into a `malloc`'d buffer and detects format
by inspecting the `cbSize` / `usType` fields.  Windows 3.x bitmaps are
converted to OS/2 2.x format in memory via `pbfh2GetWindowsBmp`.

### GPI bitmap creation from raw data
```c
hbm = GpiCreateBitmap(hps, &pbfh2->bmp2, CBM_INIT,
                       pb + pbfh2->offBits,
                       (PBITMAPINFO2)&pbfh2->bmp2);
```
`CBM_INIT` initialises the GPI bitmap directly from the raw pixel data buffer.
`WinGetPS(HWND_DESKTOP)` provides a compatible presentation space.

### Bitmap array files
Files with `usType == BFT_BITMAPARRAY` contain multiple bitmaps chained via
`BITMAPARRAYFILEHEADER.offNext`.  Each entry typically targets a different
screen resolution (`cxDisplay` / `cyDisplay`).  The viewer loads all entries
and picks the best match for the current display.

### Scaled drawing with WinDrawBitmap
```c
WinDrawBitmap(hps, hbm, NULL, (PPOINTL)&rclDest, 0L, 0L, DBM_STRETCH);
```
`DBM_STRETCH` stretches the bitmap to fill `rclDest`.  For 2-colour (1-bit)
bitmaps the foreground and background colours must be supplied explicitly.

### 3D drop-shadow borders with GpiPolyLine
Each bitmap is surrounded by a two-layer border drawn with four `GpiPolyLine`
calls.  `aptlArea[8]` stores 4 pairs of corner points (outer ±2px, inner ±1px).

### Dialog-in-window technique (PDSGetTemplate / PDSKeyProc)
`PDSGetTemplate` (Support.C) loads a `RT_DIALOG` resource and creates its
controls as children of a regular client window, giving them the appearance
and input behaviour of dialog controls without using `WinCreateDlg`.
`PDSKeyProc` intercepts Tab/Shift-Tab/arrow keys and routes them via
`WinEnumDlgItem` to give Tab-stop and group navigation.

### Custom file open dialog
`OpenDlg.C` implements a file browser from scratch using `DosFindFirst`,
`DosFindNext`, `DosQueryFSAttach`, and `DosQueryFSInfo` — covering drive
enumeration, directory navigation, volume label queries, and removable-media
handling.

### Display-resolution-independent window positioning
`CreateStdWindow` (Support.C) wraps `WinCreateStdWindow` and uses
`WinMapDlgPoints` to convert dialog-unit coordinates to pixels, making
initial window placement independent of screen resolution.

## Project Structure

```
src/
  appdefs.h    Global variable declarations and function prototypes
  appdefs.c    Global variable definitions
  viewbmp.h    Resource / menu / dialog ID constants
  viewbmp.c    main() — PM init, window class registration, message loop
  bmpwnd.c     Client window procedure, bitmap layout, WM_PAINT, menus
  effects.c    Colour table flip, bitmap refresh (GpiCreateBitmap)
  getbmp.c     File loading, format detection, bitmap array walking
  opendlg.c    Custom file open dialog (directories / files / drives)
  support.c    InitApp, CreateStdWindow, PDSGetTemplate, PDSKeyProc
  viewdlg.c    Header inspection dialogs and colour table viewer
  winbmp.h     Windows 3.x bitmap structure definitions
  viewbmp.rc   Resources: icon, menus, dialog templates
  viewbmp.def  Module definition (NAME, DESCRIPTION/bldlevel, STACKSIZE)
  bmpview.ico  Application icon
bin-gcc/       GCC build output (not in source control)
bin-ow/        OpenWatcom build output (not in source control)
```

## Building

### With GCC 9.2 for OS/2

```
compile-gcc.cmd
```

or

```
make -f Makefile.gcc
```

Requires GCC 9.2 (OS/2 port), `wl.exe` (OpenWatcom linker), and `wrc`.

### With OpenWatcom

```
compile-ow.cmd
```

or

```
wmake -f Makefile.ow
```

Requires OpenWatcom 1.9 or 2.0.

## Version History

* 2026-07-24 — v1.01: source reorganised into `src/`, build scripts added,
  documentation improved for learners.
* 1995-10-29 — errata for the published article (colour table bit-count table).
* 1995-11 — Original version published in *OS/2 Developer*, Nov/Dec 1995.

## License

Not specified by original authors.

## Authors

* Mark Benge, Matt Smith — Prominare Inc. (1995)
* Martin Iturbide (2026)

## Links

* https://github.com/OS2World/DEV-SAMPLES-C-PM-ViewBPM
* Original article: *OS/2 Developer* magazine, November/December 1995
  (archived at https://archive.org/details/os2devmag/OS2DevMag-V7N6/page/n13/mode/2up)

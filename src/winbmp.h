/*
 * winbmp.h -- Windows 3.x bitmap structure definitions for cross-format support
 *
 * OS/2 2.x bitmaps are the native format for this viewer, but Windows 3.x
 * BMP files are also widely encountered.  This header defines the Windows
 * equivalents of the OS/2 bitmap header structures so the viewer can detect,
 * read, and convert them.
 *
 * Structure sizes that distinguish formats:
 *   OS/2 1.x BITMAPINFOHEADER  : cbFix == 12  (3 USHORT fields for cx/cy)
 *   OS/2 2.x BITMAPINFOHEADER2 : cbFix == 64  (ULONG fields, many extras)
 *   Windows  winBITMAPINFOHEADER: biSize == 40 (DWORD fields, no cbFix)
 *   Windows  winBITMAPCOREHEADER: bcSize == 12 (Win 2.x / OS/2 compat)
 *
 * All structures use #pragma pack(1) to suppress compiler padding — bitmap
 * files must be read byte-for-byte as the format specifies.
 *
 * Conversion note: GetBmp.C contains pbfh2GetWindowsBmp() which allocates a
 * new OS/2 BITMAPFILEHEADER2 block and copies/transforms Windows header fields
 * into it.  Only the header is converted; pixel data is compatible as-is.
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#ifndef WINBMP_H
#define WINBMP_H

#pragma pack(1)

/*
 * winBITMAPINFOHEADER - Windows 3.x 40-byte info header (DIB format).
 *
 * Key differences from OS/2 BITMAPINFOHEADER2:
 *   - biSize is a DWORD at offset 0 (OS/2 uses ULONG cbFix at offset 0 of
 *     the info-header, but cbFix==12 for 1.x or 64 for 2.x vs biSize==40)
 *   - Dimensions are signed LONGs (OS/2 uses ULONGs in v2)
 *   - biClrUsed / biClrImportant track actual palette usage
 *   - biCompression: BI_RGB=0, BI_RLE8=1, BI_RLE4=2
 */
typedef struct _winBITMAPINFOHEADER {
    ULONG  biSize;           /* Header size in bytes == 40                  */
    LONG   biWidth;          /* Bitmap width in pixels                      */
    LONG   biHeight;         /* Bitmap height in pixels (positive = bottom-up) */
    USHORT biPlanes;         /* Number of colour planes (must be 1)         */
    USHORT biBitCount;       /* Bits per pixel: 1, 4, 8, 16, 24, 32        */
    ULONG  biCompression;    /* Compression: 0=BI_RGB, 1=BI_RLE8, 2=BI_RLE4 */
    ULONG  biSizeImage;      /* Size of compressed image data (0 if BI_RGB) */
    LONG   biXPelsPerMeter;  /* Horizontal resolution (pixels per metre)    */
    LONG   biYPelsPerMeter;  /* Vertical resolution (pixels per metre)      */
    ULONG  biClrUsed;        /* Number of palette entries actually used      */
    ULONG  biClrImportant;   /* Number of important colours (0 = all)       */
} winBITMAPINFOHEADER, *PwinBITMAPINFOHEADER;

/*
 * winRGBQUAD - Windows palette entry (4 bytes, blue/green/red + reserved).
 * OS/2 RGB2 has the same layout (bBlue, bGreen, bRed, fcOptions).
 */
typedef struct _winRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} winRGBQUAD, *PwinRGBQUAD;

/*
 * winRGBTRIPLE - OS/2 1.x / Windows CORE format palette entry (3 bytes).
 * Used in BITMAPINFO structs with winBITMAPCOREHEADER.
 */
typedef struct _winRGBTRIPLE {
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} winRGBTRIPLE, *PwinRGBTRIPLE;

/*
 * winBITMAPFILEHEADER - 14-byte header at the start of a .BMP file.
 *
 * Every Windows .BMP begins with "BM" (0x42, 0x4D).  The bfOffBits field
 * points past the header and colour table to the first pixel byte.
 */
typedef struct _winBITMAPFILEHEADER {
    USHORT bfType;           /* File type signature: 'BM' (0x4D42)         */
    ULONG  bfSize;           /* Total file size in bytes                   */
    USHORT bfReserved1;      /* Reserved; must be 0                        */
    USHORT bfReserved2;      /* Reserved; must be 0                        */
    ULONG  bfOffBits;        /* Byte offset from start of file to pixels   */
    winBITMAPINFOHEADER bmiHeader; /* Embedded info header (40 bytes)      */
} winBITMAPFILEHEADER, *PwinBITMAPFILEHEADER;

/*
 * winBITMAPCOREHEADER - 12-byte legacy header (Windows 2.x / OS/2 compat).
 * When bcSize == 12 the palette uses winRGBTRIPLE (3-byte) entries.
 */
typedef struct _winBITMAPCOREHEADER {
    ULONG  bcSize;           /* Header size: 12                            */
    USHORT bcWidth;          /* Bitmap width in pixels                     */
    USHORT bcHeight;         /* Bitmap height in pixels                    */
    USHORT bcPlanes;         /* Colour planes (must be 1)                  */
    USHORT bcBitCount;       /* Bits per pixel                             */
} winBITMAPCOREHEADER, *PwinBITMAPCOREHEADER;

/* winBITMAPCOREINFO - core header plus variable-length palette. */
typedef struct _winBITMAPCOREINFO {
    winBITMAPCOREHEADER bmciHeader;
    winRGBTRIPLE        bmciColors[1]; /* Palette entries (variable count) */
} winBITMAPCOREINFO, *PwinBITMAPCOREINFO;

/*
 * WIDTHBYTES(i) - number of bytes per scanline for a bitmap of width i bits.
 * Windows DIBs pad each scanline to a 4-byte (DWORD) boundary.
 */
#define WIDTHBYTES(i) ((i+31)/32*4)

#pragma pack()

#endif /* WINBMP_H */

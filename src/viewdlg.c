/*
 * viewdlg.c -- Bitmap header inspection dialog procedures
 *
 * Provides dialog procedures that display the binary contents of bitmap
 * file headers in a readable form.  Each dialog opens when the user right-
 * clicks on a bitmap and selects a header item from the popup menu.
 *
 * Dialog procedures in this file:
 *
 *   ViewBITMAPARRAYFILEHEADER2DlgProc  - OS/2 2.x array file header
 *   ViewBITMAPFILEHEADER2DlgProc       - OS/2 2.x file header
 *   ViewBITMAPINFOHEADER2DlgProc       - OS/2 2.x info header (all fields)
 *   ViewBITMAPARRAYFILEHEADERDlgProc   - OS/2 1.x array file header
 *   ViewBITMAPFILEHEADERDlgProc        - OS/2 1.x file header
 *   ViewBITMAPINFOHEADERDlgProc        - OS/2 1.x info header
 *   ViewColourTableDlgProc             - palette colour table (scrollable)
 *   ViewSystemBitmapInfoDlgProc        - display capabilities (DevQueryCaps)
 *
 * All dialog procedures follow the same pattern:
 *   WM_INITDLG: cast the application parameter (mp2) to the appropriate
 *               bitmap header pointer, then call WinSetDlgItemText() for
 *               each field.  The bitmap index is passed as the integer app
 *               parameter, used to select the right entry from abm[].
 *
 * OS/2 2.x BITMAPINFOHEADER2 field notes (for learners):
 *   cbFix          : size of this structure in bytes (64 for OS/2 2.x)
 *   cx / cy        : bitmap width and height in pixels (ULONG)
 *   cPlanes        : always 1 for OS/2 bitmaps
 *   cBitCount      : 1, 4, 8, 16, 24, or 32
 *   ulCompression  : BCA_UNCOMP=0, BCA_RLE8=1, BCA_RLE4=2, BCA_HUFFMAN1D=3
 *   cbImage        : size of pixel data (0 = uncompressed; can be computed)
 *   cxResolution   : horizontal pixels per metre
 *   cyResolution   : vertical pixels per metre
 *   cclrUsed       : entries in colour table (0 = 2^cBitCount)
 *   cclrImportant  : important colour count (0 = all)
 *   usUnits        : resolution units (0 = pixels per metre)
 *   usRecording    : recording algorithm (0 = bottom-up)
 *   usRendering    : halftoning algorithm (BRH_NOTHALFTONED=0, etc.)
 *   ulColorEncoding: colour space (BCE_RGB=0)
 *   ulIdentifier   : application-defined ID (0 = not used)
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#define INCL_BITMAPFILEFORMAT
#include "appdefs.h"

/* min/max are not standard C; define them if the headers don't provide them */
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

/* CAPS_DISTINCT_COLORS and CAPS_RASTER_SCALE are absent from older kLIBC headers */
#ifndef CAPS_DISTINCT_COLORS
#define CAPS_DISTINCT_COLORS  CAPS_COLORS   /* CAPS_COLORS = 14, count of colors */
#endif
#ifndef CAPS_RASTER_SCALE
#define CAPS_RASTER_SCALE  0x0004UL         /* raster scaling capability bit     */
#endif

/* -----------------------------------------------------------------------
 * Helper: format a usType field value as a symbolic string.
 * ----------------------------------------------------------------------- */
static const char *pszUsType(USHORT usType)
{
    switch (usType) {
        case BFT_BMAP:         return "BFT_BMAP";
        case BFT_BITMAPARRAY:  return "BFT_BITMAPARRAY";
        default:               return "?";
    }
}

/* -----------------------------------------------------------------------
 * Helper: format ulCompression as a symbolic string.
 * ----------------------------------------------------------------------- */
static const char *pszCompression(ULONG ul)
{
    switch (ul) {
        case BCA_UNCOMP:      return "BCA_UNCOMP";
        case BCA_RLE8:        return "BCA_RLE8";
        case BCA_RLE4:        return "BCA_RLE4";
        case BCA_HUFFMAN1D:   return "BCA_HUFFMAN1D";
        case BCA_RLE24:       return "BCA_RLE24";
        default:              return "?";
    }
}

/* -----------------------------------------------------------------------
 * Helper: format usRendering as a symbolic string.
 * ----------------------------------------------------------------------- */
static const char *pszRendering(USHORT us)
{
    switch (us) {
        case BRH_NOTHALFTONED:   return "BRH_NOTHALFTONED";
        case BRH_ERRORDIFFUSION: return "BRH_ERRORDIFFUSION";
        case BRH_PANDA:          return "BRH_PANDA";
        case BRH_SUPERCIRCLE:    return "BRH_SUPERCIRCLE";
        default:                 return "?";
    }
}

/* -----------------------------------------------------------------------
 * Helper: format ulColorEncoding as a symbolic string.
 * ----------------------------------------------------------------------- */
static const char *pszColorEncoding(ULONG ul)
{
    switch (ul) {
        case BCE_RGB:     return "BCE_RGB";
        case BCE_PALETTE: return "BCE_PALETTE";
        default:          return "?";
    }
}

/* =======================================================================
 * ViewBITMAPARRAYFILEHEADER2DlgProc
 * Shows the BITMAPARRAYFILEHEADER2 fields for an OS/2 2.x bitmap array entry.
 * ======================================================================= */
MRESULT EXPENTRY ViewBITMAPARRAYFILEHEADER2DlgProc(HWND hwnd, ULONG msg,
                                                     MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            INT   iBmp = (INT)(LONG)mp2;
            PBITMAPARRAYFILEHEADER2 pbfah2;
            CHAR  sz[64];

            if (iBmp < 0 || iBmp >= cBitmaps) break;
            pbfah2 = (PBITMAPARRAYFILEHEADER2)abm[iBmp].pbArray;
            if (!pbfah2) break;

            sprintf(sz, "%d", iBmp);
            WinSetDlgItemText(hwnd, IT_BITMAPINDEX, sz);

            WinSetDlgItemText(hwnd, IT_USTYPE,
                              pszUsType(pbfah2->usType));

            sprintf(sz, "%lu", pbfah2->cbSize);
            WinSetDlgItemText(hwnd, IT_CBSIZE, sz);

            sprintf(sz, "%lu", pbfah2->offNext);
            WinSetDlgItemText(hwnd, IT_OFFNEXT, sz);

            sprintf(sz, "%u", pbfah2->cxDisplay);
            WinSetDlgItemText(hwnd, IT_CXDISPLAY, sz);

            sprintf(sz, "%u", pbfah2->cyDisplay);
            WinSetDlgItemText(hwnd, IT_CYDISPLAY, sz);
            return 0;
        }

        case WM_COMMAND:
            if (SHORT1FROMMP(mp1) == DID_OK)
                WinDismissDlg(hwnd, DID_OK);
            return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* =======================================================================
 * ViewBITMAPFILEHEADER2DlgProc
 * Shows the BITMAPFILEHEADER2 fields (usType, cbSize, hotspot, offBits).
 * ======================================================================= */
MRESULT EXPENTRY ViewBITMAPFILEHEADER2DlgProc(HWND hwnd, ULONG msg,
                                               MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            INT   iBmp = (INT)(LONG)mp2;
            PBITMAPFILEHEADER2 pbfh2;
            CHAR  sz[64];

            if (iBmp < 0 || iBmp >= cBitmaps) break;
            pbfh2 = (PBITMAPFILEHEADER2)abm[iBmp].pb;

            sprintf(sz, "%d", iBmp);
            WinSetDlgItemText(hwnd, IT_BITMAPINDEX, sz);

            WinSetDlgItemText(hwnd, IT_USTYPE, pszUsType(pbfh2->usType));

            sprintf(sz, "%lu", pbfh2->cbSize);
            WinSetDlgItemText(hwnd, IT_CBSIZE, sz);

            sprintf(sz, "%d", pbfh2->xHotspot);
            WinSetDlgItemText(hwnd, IT_XHOTSPOT, sz);

            sprintf(sz, "%d", pbfh2->yHotspot);
            WinSetDlgItemText(hwnd, IT_YHOTSPOT, sz);

            sprintf(sz, "%lu", pbfh2->offBits);
            WinSetDlgItemText(hwnd, IT_OFFBITS, sz);
            return 0;
        }

        case WM_COMMAND:
            if (SHORT1FROMMP(mp1) == DID_OK)
                WinDismissDlg(hwnd, DID_OK);
            return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* =======================================================================
 * ViewBITMAPINFOHEADER2DlgProc
 * Shows all 14 fields of the OS/2 2.x BITMAPINFOHEADER2.
 * ======================================================================= */
MRESULT EXPENTRY ViewBITMAPINFOHEADER2DlgProc(HWND hwnd, ULONG msg,
                                               MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            INT   iBmp = (INT)(LONG)mp2;
            PBITMAPFILEHEADER2  pbfh2;
            PBITMAPINFOHEADER2  pbmp2;
            CHAR  sz[64];

            if (iBmp < 0 || iBmp >= cBitmaps) break;
            pbfh2 = (PBITMAPFILEHEADER2)abm[iBmp].pb;
            pbmp2 = &pbfh2->bmp2;

            sprintf(sz, "%d", iBmp);
            WinSetDlgItemText(hwnd, IT_BITMAPINDEX, sz);

            sprintf(sz, "%lu", pbmp2->cbFix);
            WinSetDlgItemText(hwnd, IT_CBFIX, sz);

            sprintf(sz, "%lu", pbmp2->cx);
            WinSetDlgItemText(hwnd, IT_CX, sz);

            sprintf(sz, "%lu", pbmp2->cy);
            WinSetDlgItemText(hwnd, IT_CY, sz);

            sprintf(sz, "%u", pbmp2->cPlanes);
            WinSetDlgItemText(hwnd, IT_CPLANES, sz);

            sprintf(sz, "%u", pbmp2->cBitCount);
            WinSetDlgItemText(hwnd, IT_CBITCOUNT, sz);

            WinSetDlgItemText(hwnd, IT_ULCOMPRESSION,
                              pszCompression(pbmp2->ulCompression));

            sprintf(sz, "%lu", pbmp2->cbImage);
            WinSetDlgItemText(hwnd, IT_CBIMAGE, sz);

            sprintf(sz, "%lu", pbmp2->cxResolution);
            WinSetDlgItemText(hwnd, IT_CXRESOLUTION, sz);

            sprintf(sz, "%lu", pbmp2->cyResolution);
            WinSetDlgItemText(hwnd, IT_CYRESOLUTION, sz);

            sprintf(sz, "%lu", pbmp2->cclrUsed);
            WinSetDlgItemText(hwnd, IT_CCLRUSED, sz);

            sprintf(sz, "%lu", pbmp2->cclrImportant);
            WinSetDlgItemText(hwnd, IT_CCLRIMPORTANT, sz);

            sprintf(sz, "%u", pbmp2->usUnits);
            WinSetDlgItemText(hwnd, IT_USUNITS, sz);

            sprintf(sz, "%u", pbmp2->usReserved);
            WinSetDlgItemText(hwnd, IT_USRESERVED, sz);

            sprintf(sz, "%u", pbmp2->usRecording);
            WinSetDlgItemText(hwnd, IT_USRECORDING, sz);

            WinSetDlgItemText(hwnd, IT_USRENDERING,
                              pszRendering(pbmp2->usRendering));

            sprintf(sz, "%lu", pbmp2->cSize1);
            WinSetDlgItemText(hwnd, IT_CSIZE1, sz);

            sprintf(sz, "%lu", pbmp2->cSize2);
            WinSetDlgItemText(hwnd, IT_CSIZE2, sz);

            WinSetDlgItemText(hwnd, IT_ULCOLORENCODING,
                              pszColorEncoding(pbmp2->ulColorEncoding));

            sprintf(sz, "%lu", pbmp2->ulIdentifier);
            WinSetDlgItemText(hwnd, IT_ULIDENTIFIER, sz);
            return 0;
        }

        case WM_COMMAND:
            if (SHORT1FROMMP(mp1) == DID_OK)
                WinDismissDlg(hwnd, DID_OK);
            return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* =======================================================================
 * ViewBITMAPARRAYFILEHEADERDlgProc
 * Shows the OS/2 1.x BITMAPARRAYFILEHEADER fields.
 * ======================================================================= */
MRESULT EXPENTRY ViewBITMAPARRAYFILEHEADERDlgProc(HWND hwnd, ULONG msg,
                                                    MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            INT  iBmp = (INT)(LONG)mp2;
            PBITMAPARRAYFILEHEADER pbfah;
            CHAR sz[64];

            if (iBmp < 0 || iBmp >= cBitmaps) break;
            pbfah = (PBITMAPARRAYFILEHEADER)abm[iBmp].pbArray;
            if (!pbfah) break;

            sprintf(sz, "%d", iBmp);
            WinSetDlgItemText(hwnd, IT_BITMAPINDEX, sz);

            WinSetDlgItemText(hwnd, IT_USTYPE, pszUsType(pbfah->usType));

            sprintf(sz, "%lu", pbfah->cbSize);
            WinSetDlgItemText(hwnd, IT_CBSIZE, sz);

            sprintf(sz, "%lu", pbfah->offNext);
            WinSetDlgItemText(hwnd, IT_OFFNEXT, sz);

            sprintf(sz, "%u", pbfah->cxDisplay);
            WinSetDlgItemText(hwnd, IT_CXDISPLAY, sz);

            sprintf(sz, "%u", pbfah->cyDisplay);
            WinSetDlgItemText(hwnd, IT_CYDISPLAY, sz);
            return 0;
        }

        case WM_COMMAND:
            if (SHORT1FROMMP(mp1) == DID_OK)
                WinDismissDlg(hwnd, DID_OK);
            return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* =======================================================================
 * ViewBITMAPFILEHEADERDlgProc
 * Shows the OS/2 1.x BITMAPFILEHEADER fields.
 * ======================================================================= */
MRESULT EXPENTRY ViewBITMAPFILEHEADERDlgProc(HWND hwnd, ULONG msg,
                                              MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            INT  iBmp = (INT)(LONG)mp2;
            PBITMAPFILEHEADER pbfh;
            CHAR sz[64];

            if (iBmp < 0 || iBmp >= cBitmaps) break;
            pbfh = (PBITMAPFILEHEADER)abm[iBmp].pb;

            sprintf(sz, "%d", iBmp);
            WinSetDlgItemText(hwnd, IT_BITMAPINDEX, sz);

            WinSetDlgItemText(hwnd, IT_USTYPE, pszUsType(pbfh->usType));

            sprintf(sz, "%lu", pbfh->cbSize);
            WinSetDlgItemText(hwnd, IT_CBSIZE, sz);

            sprintf(sz, "%d", pbfh->xHotspot);
            WinSetDlgItemText(hwnd, IT_XHOTSPOT, sz);

            sprintf(sz, "%d", pbfh->yHotspot);
            WinSetDlgItemText(hwnd, IT_YHOTSPOT, sz);

            sprintf(sz, "%lu", pbfh->offBits);
            WinSetDlgItemText(hwnd, IT_OFFBITS, sz);
            return 0;
        }

        case WM_COMMAND:
            if (SHORT1FROMMP(mp1) == DID_OK)
                WinDismissDlg(hwnd, DID_OK);
            return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* =======================================================================
 * ViewBITMAPINFOHEADERDlgProc
 * Shows the OS/2 1.x BITMAPINFOHEADER fields (cbFix, cx, cy, cPlanes,
 * cBitCount).  The 1.x header has far fewer fields than the 2.x version.
 * ======================================================================= */
MRESULT EXPENTRY ViewBITMAPINFOHEADERDlgProc(HWND hwnd, ULONG msg,
                                              MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            INT  iBmp = (INT)(LONG)mp2;
            PBITMAPFILEHEADER pbfh;
            CHAR sz[64];

            if (iBmp < 0 || iBmp >= cBitmaps) break;
            pbfh = (PBITMAPFILEHEADER)abm[iBmp].pb;

            sprintf(sz, "%d", iBmp);
            WinSetDlgItemText(hwnd, IT_BITMAPINDEX, sz);

            sprintf(sz, "%lu", pbfh->bmp.cbFix);
            WinSetDlgItemText(hwnd, IT_CBFIX, sz);

            sprintf(sz, "%u", pbfh->bmp.cx);
            WinSetDlgItemText(hwnd, IT_CX, sz);

            sprintf(sz, "%u", pbfh->bmp.cy);
            WinSetDlgItemText(hwnd, IT_CY, sz);

            sprintf(sz, "%u", pbfh->bmp.cPlanes);
            WinSetDlgItemText(hwnd, IT_CPLANES, sz);

            sprintf(sz, "%u", pbfh->bmp.cBitCount);
            WinSetDlgItemText(hwnd, IT_CBITCOUNT, sz);
            return 0;
        }

        case WM_COMMAND:
            if (SHORT1FROMMP(mp1) == DID_OK)
                WinDismissDlg(hwnd, DID_OK);
            return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* =======================================================================
 * ViewColourTableDlgProc
 * Displays the bitmap colour table with a vertical scrollbar.
 *
 * The dialog shows 16 rows at a time (EF_INDEX00 .. EF_INDEX15).
 * WM_VSCROLL scrolls through the complete table (up to 256 entries for
 * 8-bit bitmaps, or 2 for 1-bit bitmaps which use DLG_VIEW2COLOURTABLE).
 *
 * lVScrollPos tracks the current top row (0 = first entry visible).
 * SetTableValues / QueryTableValues update the 16 entry fields and labels.
 * ======================================================================= */

/* Module-level state for the colour table dialog */
static PRGB2  prgb2Table = NULL;   /* Pointer to colour table in raw data  */
static LONG   cTableColours = 0;   /* Number of entries in the table       */
static LONG   lVScrollPos = 0;     /* Current scroll position (top row)    */

/* Write the 16 visible colour table entries to the dialog controls. */
static VOID SetTableValues(HWND hwnd)
{
    INT  i;
    CHAR szIndex[24];
    CHAR szRGB[24];

    for (i = 0; i < 16; i++) {
        LONG iEntry = lVScrollPos + i;

        /* Update the row label (shows absolute index). */
        if (iEntry < cTableColours)
            sprintf(szIndex, "RGB[%03ld] =", iEntry);
        else
            sprintf(szIndex, "         ");
        WinSetDlgItemText(hwnd, (ULONG)(IT_INDEX00 + i), szIndex);

        /* Update the hex RGB value. */
        if (iEntry < cTableColours)
            sprintf(szRGB, "0x%02X%02X%02X",
                    (UINT)prgb2Table[iEntry].bRed,
                    (UINT)prgb2Table[iEntry].bGreen,
                    (UINT)prgb2Table[iEntry].bBlue);
        else
            szRGB[0] = '\0';
        WinSetDlgItemText(hwnd, (ULONG)(EF_INDEX00 + i), szRGB);
    }
}

MRESULT EXPENTRY ViewColourTableDlgProc(HWND hwnd, ULONG msg,
                                        MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            INT  iBmp = (INT)(LONG)mp2;
            PBITMAPFILEHEADER2 pbfh2;

            lVScrollPos = 0;
            if (iBmp < 0 || iBmp >= cBitmaps) break;

            pbfh2      = (PBITMAPFILEHEADER2)abm[iBmp].pb;
            cTableColours = abm[iBmp].cColours;
            prgb2Table = (PRGB2)((PBYTE)&pbfh2->bmp2 + pbfh2->bmp2.cbFix);

            /* Configure the scrollbar range.  Max = max scroll position
             * (total entries minus the 16 visible rows). */
            WinSendDlgItemMsg(hwnd, SB_INDEX, SBM_SETSCROLLBAR,
                              MPFROMLONG(0L),
                              MPFROM2SHORT(0,
                                  (SHORT)max(0L, cTableColours - 16L)));

            SetTableValues(hwnd);
            return 0;
        }

        case WM_VSCROLL:
        {
            USHORT usCmd = SHORT2FROMMP(mp2);
            LONG   lMax  = max(0L, cTableColours - 16L);

            switch (usCmd)
            {
                case SB_LINEUP:
                    lVScrollPos = max(0L, lVScrollPos - 1L);
                    break;
                case SB_LINEDOWN:
                    lVScrollPos = min(lMax, lVScrollPos + 1L);
                    break;
                case SB_PAGEUP:
                    lVScrollPos = max(0L, lVScrollPos - 16L);
                    break;
                case SB_PAGEDOWN:
                    lVScrollPos = min(lMax, lVScrollPos + 16L);
                    break;
                case SB_SLIDERPOSITION:
                case SB_SLIDERTRACK:
                    lVScrollPos = (LONG)SHORT1FROMMP(mp2);
                    lVScrollPos = max(0L, min(lMax, lVScrollPos));
                    break;
                default:
                    return 0;
            }

            WinSendDlgItemMsg(hwnd, SB_INDEX, SBM_SETPOS,
                              MPFROMLONG(lVScrollPos), 0);
            SetTableValues(hwnd);
            return 0;
        }

        case WM_COMMAND:
            if (SHORT1FROMMP(mp1) == DID_OK)
                WinDismissDlg(hwnd, DID_OK);
            return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* =======================================================================
 * ViewSystemBitmapInfoDlgProc
 * Shows display device capabilities queried via DevQueryCaps.
 *
 * DevQueryCaps(hdc, CAPS_FAMILY, count, array) fills an array of LONG
 * values with device-capability constants.  The first argument is the
 * CAPS_FAMILY starting index; the second is how many to query.
 *
 * Capabilities displayed:
 *   CAPS_WIDTH           - screen width in pixels
 *   CAPS_HEIGHT          - screen height in pixels
 *   CAPS_HORIZONTAL_RESOLUTION - horizontal pixels per metre
 *   CAPS_VERTICAL_RESOLUTION   - vertical pixels per metre
 *   CAPS_COLOR_PLANES    - number of colour planes
 *   CAPS_DISTINCT_COLORS - number of distinct colours
 *   CAPS_COLOR_BITCOUNT  - bits per pixel
 *   CAPS_BITMAP_FORMATS  - number of supported bitmap formats
 *   CAPS_RASTER_CAPS     - bitmask of raster capabilities
 * ======================================================================= */
MRESULT EXPENTRY ViewSystemBitmapInfoDlgProc(HWND hwnd, ULONG msg,
                                             MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            HPS   hps;
            HDC   hdc;
            LONG  alCaps[CAPS_RASTER_CAPS + 1];
            CHAR  sz[64];
            ULONG ulCaps;

            hps = WinGetPS(HWND_DESKTOP);
            hdc = GpiQueryDevice(hps);

            /* Query from index 0 (CAPS_FAMILY) through CAPS_RASTER_CAPS. */
            DevQueryCaps(hdc, CAPS_FAMILY,
                         (LONG)(CAPS_RASTER_CAPS + 1L),
                         alCaps);

            WinReleasePS(hps);

            sprintf(sz, "%ld", alCaps[CAPS_WIDTH]);
            WinSetDlgItemText(hwnd, IT_CXSCREEN, sz);

            sprintf(sz, "%ld", alCaps[CAPS_HEIGHT]);
            WinSetDlgItemText(hwnd, IT_CYSCREEN, sz);

            sprintf(sz, "%ld dpi (horiz)",
                    alCaps[CAPS_HORIZONTAL_RESOLUTION]);
            WinSetDlgItemText(hwnd, IT_HORZRESOLUTION, sz);

            sprintf(sz, "%ld dpi (vert)",
                    alCaps[CAPS_VERTICAL_RESOLUTION]);
            WinSetDlgItemText(hwnd, IT_VERTRESOLUTION, sz);

            sprintf(sz, "%ld", alCaps[CAPS_COLOR_PLANES]);
            WinSetDlgItemText(hwnd, IT_PLANES, sz);

            sprintf(sz, "%ld", alCaps[CAPS_DISTINCT_COLORS]);
            WinSetDlgItemText(hwnd, IT_DISTINCTCOLOURS, sz);

            sprintf(sz, "%ld", alCaps[CAPS_COLOR_BITCOUNT]);
            WinSetDlgItemText(hwnd, IT_BITCOUNT, sz);

            sprintf(sz, "%ld", alCaps[CAPS_BITMAP_FORMATS]);
            WinSetDlgItemText(hwnd, IT_BITMAPFORMATS, sz);

            /* CAPS_RASTER_CAPS is a bitmask: extract individual bits. */
            ulCaps = (ULONG)alCaps[CAPS_RASTER_CAPS];

            WinSetDlgItemText(hwnd, IT_BITBLT,
                (ulCaps & CAPS_RASTER_BITBLT) ? "Yes" : "No");
            WinSetDlgItemText(hwnd, IT_BANDING,
                (ulCaps & CAPS_RASTER_BANDING) ? "Yes" : "No");
            WinSetDlgItemText(hwnd, IT_BITBLTSCALING,
                (ulCaps & CAPS_RASTER_SCALE) ? "Yes" : "No");
            return 0;
        }

        case WM_COMMAND:
            if (SHORT1FROMMP(mp1) == DID_OK)
                WinDismissDlg(hwnd, DID_OK);
            return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

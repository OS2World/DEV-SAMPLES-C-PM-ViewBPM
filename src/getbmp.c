/*
 * getbmp.c -- Bitmap file loading and format detection
 *
 * This module reads a .BMP file from disk, determines its format (OS/2 1.x,
 * OS/2 2.x, or Windows 3.x), converts Windows format to OS/2 2.x in memory,
 * and creates GPI bitmap handles for all images in the file.
 *
 * Supported bitmap formats:
 *
 *   OS/2 1.x single:   BITMAPFILEHEADER  (usType=BFT_BMAP,  cbSize=14)
 *   OS/2 1.x array:    BITMAPARRAYFILEHEADER (usType=BFT_BITMAPARRAY)
 *   OS/2 2.x single:   BITMAPFILEHEADER2 (usType=BFT_BMAP,  cbSize=26)
 *   OS/2 2.x array:    BITMAPARRAYFILEHEADER2 (usType=BFT_BITMAPARRAY)
 *   Windows 3.x:       winBITMAPFILEHEADER (bfType='BM', biSize=40)
 *
 * Format detection strategy:
 *   Read the first few bytes and inspect cbSize / biSize:
 *     sizeof(BITMAPARRAYFILEHEADER2) > sizeof(BITMAPFILEHEADER2) -> 2.x array
 *     sizeof(BITMAPFILEHEADER2)      = 26  -> OS/2 2.x single
 *     sizeof(BITMAPARRAYFILEHEADER)  > sizeof(BITMAPFILEHEADER) -> 1.x array
 *     sizeof(BITMAPFILEHEADER)       = 14  -> OS/2 1.x single
 *     anything else                        -> attempt Windows conversion
 *
 * Struct layout for array formats:
 *   BITMAPARRAYFILEHEADER2 { usType, cbSize, offNext, cxDisplay, cyDisplay,
 *                            bfh2 (embedded BITMAPFILEHEADER2) }
 *   BITMAPARRAYFILEHEADER  { usType, cbSize, offNext, cxDisplay, cyDisplay,
 *                            bfh  (embedded BITMAPFILEHEADER)  }
 *
 * abm[i].pb     always points to the inner BITMAPFILEHEADER2/BITMAPFILEHEADER,
 *               which is what effects.c and the colour-table dialogs need.
 * abm[i].pbArray points to the outer BITMAPARRAYFILEHEADER2/BITMAPARRAYFILEHEADER
 *               (NULL for single-bitmap files), used by the array-header dialogs.
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#define INCL_BITMAPFILEFORMAT
#include "appdefs.h"

/* Forward declarations for internal helpers */
static BOOL    BuildBitmapStack(PBITMAPARRAYFILEHEADER2 pbfah2);
static BOOL    BuildBitmapStack1(PBITMAPARRAYFILEHEADER pbfah);
static LONG    lSelectBfh(PBITMAPARRAYFILEHEADER2 pbfah2);
static LONG    lSelectBfh1(PBITMAPARRAYFILEHEADER pbfah);
static HBITMAP hbmGetSingleBitmap(PBITMAPFILEHEADER2 pbfh2);
static HBITMAP hbmGetSingleBitmap1(PBITMAPFILEHEADER pbfh);
static PBITMAPFILEHEADER2 pbfh2GetWindowsBmp(PwinBITMAPFILEHEADER pwbfh);

/*
 * hbmGetBitmap - load a bitmap file and populate the abm[] array.
 *
 * hwnd          : client window (used only to pass to layout helpers)
 * pszBitmapFile : full path to the .BMP file
 *
 * Returns TRUE on success, FALSE on any error.
 * On success, cBitmaps > 0 and abm[0..cBitmaps-1] are valid.
 */
BOOL hbmGetBitmap(HWND hwnd, PSZ pszBitmapFile)
{
    HFILE       hf;
    FILESTATUS3 fs;
    ULONG       cbRead;
    ULONG       ulAction;
    PBITMAPFILEHEADER2  pbfh2;
    PBITMAPFILEHEADER   pbfh;
    PwinBITMAPFILEHEADER pwbfh;
    INT  i;

    (VOID)hwnd;

    /* Free any previously loaded bitmap data. */
    for (i = 0; i < cBitmaps; i++)
        if (abm[i].hbm)
            GpiDeleteBitmap(abm[i].hbm);
    if (pb)
        free(pb);
    pb       = NULL;
    cBitmaps = 0;
    f20Bitmap      = FALSE;
    fWindowsBitmap = FALSE;
    fBitmapArray   = FALSE;

    /* Open the file and query its size. */
    if (DosOpen(pszBitmapFile, &hf, &ulAction, 0L, FILE_NORMAL,
                OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_FLAGS_NOINHERIT | OPEN_SHARE_DENYWRITE | OPEN_ACCESS_READONLY,
                NULL))
        return FALSE;

    DosQueryFileInfo(hf, FIL_STANDARD, &fs, sizeof(fs));

    /* Read the entire file into a heap buffer. */
    pb = (PBYTE)malloc(fs.cbFile);
    if (!pb) {
        DosClose(hf);
        return FALSE;
    }

    DosRead(hf, pb, fs.cbFile, &cbRead);
    DosClose(hf);

    if (cbRead != fs.cbFile) {
        free(pb);
        pb = NULL;
        return FALSE;
    }

    strcpy(szBitmapFile, pszBitmapFile);

    /* -------------------------------------------------------------------
     * Format detection by inspecting the header size field.
     * Both BITMAPFILEHEADER2 and BITMAPARRAYFILEHEADER2 start with the
     * same usType + cbSize layout, so reading cbSize via a PBITMAPFILEHEADER2
     * cast is safe for detection purposes.
     * ------------------------------------------------------------------- */
    pbfh2  = (PBITMAPFILEHEADER2)pb;
    pbfh   = (PBITMAPFILEHEADER)pb;
    pwbfh  = (PwinBITMAPFILEHEADER)pb;

    if (pbfh2->cbSize == sizeof(BITMAPARRAYFILEHEADER2)) {
        /* OS/2 2.x bitmap array */
        f20Bitmap    = TRUE;
        fBitmapArray = TRUE;
        return BuildBitmapStack((PBITMAPARRAYFILEHEADER2)pb);
    }

    if (pbfh2->cbSize == sizeof(BITMAPFILEHEADER2)) {
        /* OS/2 2.x single bitmap */
        f20Bitmap      = TRUE;
        abm[0].pb      = pb;
        abm[0].pbArray = NULL;
        abm[0].hbm     = hbmGetSingleBitmap(pbfh2);
        if (!abm[0].hbm) return FALSE;
        abm[0].cColours = (pbfh2->bmp2.cBitCount <= 8)
                          ? (1L << pbfh2->bmp2.cBitCount)
                          : 0L;
        cBitmaps = 1;
        return TRUE;
    }

    if (pbfh->cbSize == sizeof(BITMAPARRAYFILEHEADER)) {
        /* OS/2 1.x bitmap array */
        fBitmapArray = TRUE;
        return BuildBitmapStack1((PBITMAPARRAYFILEHEADER)pb);
    }

    if (pbfh->cbSize == sizeof(BITMAPFILEHEADER)) {
        /* OS/2 1.x single bitmap */
        abm[0].pb      = pb;
        abm[0].pbArray = NULL;
        abm[0].hbm     = hbmGetSingleBitmap1(pbfh);
        if (!abm[0].hbm) return FALSE;
        abm[0].cColours = (pbfh->bmp.cBitCount <= 8)
                          ? (1L << pbfh->bmp.cBitCount)
                          : 0L;
        cBitmaps = 1;
        return TRUE;
    }

    /* Attempt Windows format conversion. */
    {
        PBITMAPFILEHEADER2 pbfh2New;
        pbfh2New = pbfh2GetWindowsBmp(pwbfh);
        if (!pbfh2New) {
            free(pb);
            pb = NULL;
            return FALSE;
        }
        /* Replace the raw buffer with the converted OS/2 2.x data. */
        free(pb);
        pb = (PBYTE)pbfh2New;
        f20Bitmap      = TRUE;
        fWindowsBitmap = TRUE;
        pbfh2          = (PBITMAPFILEHEADER2)pb;
        abm[0].pb      = pb;
        abm[0].pbArray = NULL;
        abm[0].hbm     = hbmGetSingleBitmap(pbfh2);
        if (!abm[0].hbm) return FALSE;
        abm[0].cColours = (pbfh2->bmp2.cBitCount <= 8)
                          ? (1L << pbfh2->bmp2.cBitCount)
                          : 0L;
        cBitmaps = 1;
        return TRUE;
    }
}

/*
 * hbmGetSingleBitmap - create a GPI HBITMAP from an OS/2 2.x file header.
 *
 * offBits in BITMAPFILEHEADER2 is relative to the start of that header
 * struct, so the pixel data is at (PBYTE)pbfh2 + pbfh2->offBits.
 */
static HBITMAP hbmGetSingleBitmap(PBITMAPFILEHEADER2 pbfh2)
{
    HPS     hps;
    HBITMAP hbm;

    hps = WinGetPS(HWND_DESKTOP);
    hbm = GpiCreateBitmap(hps,
                          &pbfh2->bmp2,
                          CBM_INIT,
                          (PBYTE)pbfh2 + pbfh2->offBits,
                          (PBITMAPINFO2)&pbfh2->bmp2);
    WinReleasePS(hps);
    return hbm;
}

/*
 * hbmGetSingleBitmap1 - create a GPI HBITMAP from an OS/2 1.x file header.
 *
 * The 1.x BITMAPINFOHEADER is 12 bytes (ULONG cbFix + 3 USHORTs for cx/cy/
 * cPlanes/cBitCount).  GpiCreateBitmap accepts it via the PBITMAPINFO2 cast
 * because it inspects the cbFix field to determine which variant is present.
 */
static HBITMAP hbmGetSingleBitmap1(PBITMAPFILEHEADER pbfh)
{
    HPS     hps;
    HBITMAP hbm;

    hps = WinGetPS(HWND_DESKTOP);
    hbm = GpiCreateBitmap(hps,
                          (PBITMAPINFOHEADER2)&pbfh->bmp,
                          CBM_INIT,
                          (PBYTE)pbfh + pbfh->offBits,
                          (PBITMAPINFO2)&pbfh->bmp);
    WinReleasePS(hps);
    return hbm;
}

/*
 * BuildBitmapStack - walk an OS/2 2.x BITMAPARRAYFILEHEADER2 chain and
 *                   create an HBITMAP for each image.
 *
 * Each array entry is a BITMAPARRAYFILEHEADER2 which contains an embedded
 * BITMAPFILEHEADER2 (bfh2).  We store:
 *   abm[i].pb      = pointer to the inner bfh2 (used by effects, colour table)
 *   abm[i].pbArray = pointer to the outer array header (used by header dialogs)
 *
 * The best-fit image for the current screen is stored in abm[iBitmap].
 */
static BOOL BuildBitmapStack(PBITMAPARRAYFILEHEADER2 pbfah2)
{
    LONG    iBest;
    PBITMAPARRAYFILEHEADER2 pbfahCur;
    ULONG   offNext;
    INT     i;

    iBest = lSelectBfh(pbfah2);

    pbfahCur = pbfah2;
    i = 0;
    do {
        abm[i].pb      = (PBYTE)&pbfahCur->bfh2;
        abm[i].pbArray = (PBYTE)pbfahCur;
        abm[i].hbm     = hbmGetSingleBitmap(&pbfahCur->bfh2);
        abm[i].cColours = (pbfahCur->bfh2.bmp2.cBitCount <= 8)
                          ? (1L << pbfahCur->bfh2.bmp2.cBitCount)
                          : 0L;
        offNext = pbfahCur->offNext;
        i++;
        if (offNext)
            pbfahCur = (PBITMAPARRAYFILEHEADER2)(pb + offNext);
    } while (offNext && i < 32);

    cBitmaps = i;
    iBitmap  = (iBest >= 0 && iBest < i) ? (INT)iBest : 0;
    iBitmapSelected = iBitmap;
    return (cBitmaps > 0);
}

/*
 * BuildBitmapStack1 - same as BuildBitmapStack but for OS/2 1.x arrays.
 *
 * Uses BITMAPARRAYFILEHEADER (1.x) which has USHORT offNext/cxDisplay/cyDisplay
 * and an embedded BITMAPFILEHEADER (bfh).
 */
static BOOL BuildBitmapStack1(PBITMAPARRAYFILEHEADER pbfah)
{
    LONG   iBest;
    PBITMAPARRAYFILEHEADER pbfahCur;
    ULONG  offNext;
    INT    i;

    iBest = lSelectBfh1(pbfah);

    pbfahCur = pbfah;
    i = 0;
    do {
        abm[i].pb      = (PBYTE)&pbfahCur->bfh;
        abm[i].pbArray = (PBYTE)pbfahCur;
        abm[i].hbm     = hbmGetSingleBitmap1(&pbfahCur->bfh);
        abm[i].cColours = (pbfahCur->bfh.bmp.cBitCount <= 8)
                          ? (1L << pbfahCur->bfh.bmp.cBitCount)
                          : 0L;
        offNext = pbfahCur->offNext;
        i++;
        if (offNext)
            pbfahCur = (PBITMAPARRAYFILEHEADER)(pb + offNext);
    } while (offNext && i < 32);

    cBitmaps = i;
    iBitmap  = (iBest >= 0 && iBest < i) ? (INT)iBest : 0;
    iBitmapSelected = iBitmap;
    return (cBitmaps > 0);
}

/*
 * lSelectBfh - choose the best bitmap from an OS/2 2.x array for the display.
 *
 * Uses a simple heuristic: pick the entry whose cxDisplay/cyDisplay best
 * matches the actual screen resolution (prefers 1024x768 on wide screens,
 * 640x480 otherwise).  Returns the 0-based index of the best entry.
 */
static LONG lSelectBfh(PBITMAPARRAYFILEHEADER2 pbfah2)
{
    PBITMAPARRAYFILEHEADER2 pbfahCur;
    ULONG  offNext;
    INT    i, iBest;
    LONG   cxTarget, cyTarget;
    LONG   cxScr;

    cxScr    = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
    cxTarget = (cxScr > 800L) ? 1024L : 640L;
    cyTarget = (cxScr > 800L) ?  768L : 480L;

    pbfahCur = pbfah2;
    i     = 0;
    iBest = 0;
    do {
        if ((LONG)pbfahCur->cxDisplay == cxTarget &&
            (LONG)pbfahCur->cyDisplay == cyTarget)
            iBest = i;
        offNext = pbfahCur->offNext;
        i++;
        if (offNext)
            pbfahCur = (PBITMAPARRAYFILEHEADER2)(pb + offNext);
    } while (offNext && i < 32);

    return (LONG)iBest;
}

/*
 * lSelectBfh1 - same as lSelectBfh but for OS/2 1.x BITMAPARRAYFILEHEADER.
 */
static LONG lSelectBfh1(PBITMAPARRAYFILEHEADER pbfah)
{
    PBITMAPARRAYFILEHEADER pbfahCur;
    ULONG  offNext;
    INT    i, iBest;
    LONG   cxTarget, cyTarget;
    LONG   cxScr;

    cxScr    = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
    cxTarget = (cxScr > 800L) ? 1024L : 640L;
    cyTarget = (cxScr > 800L) ?  768L : 480L;

    pbfahCur = pbfah;
    i     = 0;
    iBest = 0;
    do {
        if ((LONG)pbfahCur->cxDisplay == cxTarget &&
            (LONG)pbfahCur->cyDisplay == cyTarget)
            iBest = i;
        offNext = pbfahCur->offNext;
        i++;
        if (offNext)
            pbfahCur = (PBITMAPARRAYFILEHEADER)(pb + offNext);
    } while (offNext && i < 32);

    return (LONG)iBest;
}

/*
 * pbfh2GetWindowsBmp - convert a Windows 3.x BMP header to OS/2 2.x format.
 *
 * Allocates a new buffer large enough for BITMAPFILEHEADER2 + colour table +
 * pixel data.  Copies and transforms the Windows fields into OS/2 equivalents.
 * The pixel data is bit-compatible and is copied verbatim.
 *
 * Returns a pointer to the new OS/2 2.x buffer (caller must free it), or NULL.
 */
static PBITMAPFILEHEADER2 pbfh2GetWindowsBmp(PwinBITMAPFILEHEADER pwbfh)
{
    PwinBITMAPINFOHEADER pwbmp;
    PBITMAPFILEHEADER2   pbfh2New;
    PBITMAPINFOHEADER2   pbmp2;
    ULONG                cbTotal;
    ULONG                cbPixels;
    LONG                 cColours;
    ULONG                cbColourTable;
    PBYTE                pbPixelsSrc;
    PBYTE                pbPixelsDst;
    PwinRGBQUAD          pwrgb;
    PRGB2                prgb2;
    LONG                 i;

    pwbmp = &pwbfh->bmiHeader;

    /* Compute colour count (biClrUsed == 0 means use all entries). */
    if (pwbmp->biBitCount < 16)
        cColours = (pwbmp->biClrUsed > 0)
                   ? (LONG)pwbmp->biClrUsed
                   : (1L << pwbmp->biBitCount);
    else
        cColours = 0;

    cbColourTable = (ULONG)(cColours * sizeof(RGB2));
    cbPixels      = pwbfh->bfSize - pwbfh->bfOffBits;
    cbTotal       = sizeof(BITMAPFILEHEADER2) + cbColourTable + cbPixels;

    pbfh2New = (PBITMAPFILEHEADER2)malloc(cbTotal);
    if (!pbfh2New)
        return NULL;

    /* Fill BITMAPFILEHEADER2 */
    pbfh2New->usType   = BFT_BMAP;
    pbfh2New->cbSize   = sizeof(BITMAPFILEHEADER2);
    pbfh2New->xHotspot = 0;
    pbfh2New->yHotspot = 0;
    pbfh2New->offBits  = sizeof(BITMAPFILEHEADER2) + cbColourTable;

    /* Fill BITMAPINFOHEADER2 from winBITMAPINFOHEADER */
    pbmp2 = &pbfh2New->bmp2;
    memset(pbmp2, 0, sizeof(BITMAPINFOHEADER2));
    pbmp2->cbFix         = sizeof(BITMAPINFOHEADER2);
    pbmp2->cx            = (ULONG)pwbmp->biWidth;
    pbmp2->cy            = (ULONG)pwbmp->biHeight;
    pbmp2->cPlanes       = pwbmp->biPlanes;
    pbmp2->cBitCount     = pwbmp->biBitCount;
    pbmp2->ulCompression = pwbmp->biCompression;  /* BI_RGB == BCA_UNCOMP == 0 */
    pbmp2->cbImage       = pwbmp->biSizeImage;
    pbmp2->cxResolution  = (ULONG)pwbmp->biXPelsPerMeter;
    pbmp2->cyResolution  = (ULONG)pwbmp->biYPelsPerMeter;
    pbmp2->cclrUsed      = (ULONG)cColours;
    pbmp2->cclrImportant = pwbmp->biClrImportant;

    /* Convert Windows RGBQUAD (B/G/R/reserved) to OS/2 RGB2 (same layout). */
    pwrgb = (PwinRGBQUAD)((PBYTE)pwbmp + sizeof(winBITMAPINFOHEADER));
    prgb2 = (PRGB2)((PBYTE)pbfh2New + sizeof(BITMAPFILEHEADER2));
    for (i = 0; i < cColours; i++) {
        prgb2[i].bBlue     = pwrgb[i].rgbBlue;
        prgb2[i].bGreen    = pwrgb[i].rgbGreen;
        prgb2[i].bRed      = pwrgb[i].rgbRed;
        prgb2[i].fcOptions = 0;
    }

    /* Copy pixel data verbatim. */
    pbPixelsSrc = (PBYTE)pwbfh + pwbfh->bfOffBits;
    pbPixelsDst = (PBYTE)pbfh2New + pbfh2New->offBits;
    memcpy(pbPixelsDst, pbPixelsSrc, cbPixels);

    return pbfh2New;
}

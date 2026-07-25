/*
 * effects.c -- In-memory bitmap colour-table manipulation effects
 *
 * This module provides two operations that modify the colour table of a
 * palettised bitmap that has already been loaded into memory:
 *
 *   hbmFlipColourTable  - reverses the order of all palette entries in the
 *                         raw file data, then rebuilds the GPI bitmap.
 *                         Only meaningful for 1-, 4-, or 8-bit bitmaps that
 *                         have an explicit colour table (cColours > 0).
 *
 *   hbmRefreshBitmap    - deletes the current GPI bitmap handle and creates
 *                         a new one from the same raw data buffer.  Used
 *                         after any in-place modification of the raw data.
 *
 * How it works:
 *   The raw file data (pb) is a malloc'd buffer containing the complete
 *   bitmap file exactly as read from disk.  For OS/2 2.x files the colour
 *   table starts immediately after the BITMAPINFOHEADER2 (offset cbFix into
 *   the BITMAPFILEHEADER2).  Flipping the table reverses the palette entries
 *   in that buffer, then hbmRefreshBitmap recreates the GPI bitmap so the
 *   screen reflects the change.
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#include "appdefs.h"

/*
 * hbmFlipColourTable - reverse the colour table of a palettised bitmap.
 *
 * pb        : pointer to the BITMAPFILEHEADER2 (start of raw file data)
 * hbm       : existing GPI bitmap handle (will be deleted and replaced)
 * cColours  : number of palette entries (must be > 0; returns hbm unchanged
 *             if the bitmap has no colour table, e.g. 24-bit truecolour)
 *
 * Returns the new HBITMAP, or the original hbm if no flip was performed.
 *
 * Implementation notes:
 *   The colour table immediately follows the BITMAPINFOHEADER2 inside the
 *   BITMAPFILEHEADER2.  Each entry is an RGB2 struct (4 bytes).
 *   A simple swap loop reverses entries 0..cColours/2.
 *   After the in-place swap, hbmRefreshBitmap rebuilds the GPI handle.
 */
HBITMAP hbmFlipColourTable(PBYTE pb, HBITMAP hbm, LONG cColours)
{
    PBITMAPFILEHEADER2  pbfh2;
    PBITMAPINFOHEADER2  pbmp2;
    RGB2               *prgb2;
    RGB2                rgb2Tmp;
    INT                 i;

    /* Only palettised bitmaps have a colour table to flip. */
    if (!cColours)
        return hbm;

    pbfh2 = (PBITMAPFILEHEADER2)pb;
    pbmp2 = &pbfh2->bmp2;

    /* Colour table starts immediately after the fixed-size info header. */
    prgb2 = (RGB2 *)((PBYTE)pbmp2 + pbmp2->cbFix);

    /* Reverse the palette entries in-place. */
    for (i = 0; i < cColours / 2; i++) {
        rgb2Tmp             = prgb2[i];
        prgb2[i]            = prgb2[cColours - 1 - i];
        prgb2[cColours - 1 - i] = rgb2Tmp;
    }

    /* Rebuild the GPI bitmap from the modified raw data. */
    return hbmRefreshBitmap(pb, hbm);
}

/*
 * hbmRefreshBitmap - delete an existing GPI bitmap and recreate it from
 *                    the raw file data pointed to by pb.
 *
 * pb  : pointer to the BITMAPFILEHEADER2 in the raw data buffer
 * hbm : the GPI bitmap handle to delete
 *
 * Returns the new HBITMAP created by GpiCreateBitmap(), or NULLHANDLE on
 * failure.
 *
 * GpiCreateBitmap parameters:
 *   hpsMem  - any presentation space works; HWND_DESKTOP gives a screen PS
 *   pbmp2   - pointer to BITMAPINFOHEADER2 (describes dimensions/depth)
 *   CBM_INIT - tells GPI to initialise the bitmap from the pbData/pInfo args
 *   pbData  - pointer to pixel data (at pbfh2->offBits)
 *   pInfo   - pointer to BITMAPINFO2 (header + colour table) for the init
 */
HBITMAP hbmRefreshBitmap(PBYTE pb, HBITMAP hbm)
{
    PBITMAPFILEHEADER2 pbfh2;
    HPS                hpsMem;
    HBITMAP            hbmNew;

    pbfh2 = (PBITMAPFILEHEADER2)pb;

    /* Release the old GPI bitmap handle. */
    GpiDeleteBitmap(hbm);

    /* A screen PS is sufficient; GpiCreateBitmap only needs it for the
     * device context to determine colour-depth compatibility. */
    hpsMem = WinGetPS(HWND_DESKTOP);

    hbmNew = GpiCreateBitmap(hpsMem,
                             &pbfh2->bmp2,
                             CBM_INIT,
                             pb + pbfh2->offBits,
                             (PBITMAPINFO2)&pbfh2->bmp2);

    WinReleasePS(hpsMem);

    return hbmNew;
}

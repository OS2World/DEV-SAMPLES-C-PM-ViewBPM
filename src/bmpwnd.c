/*
 * bmpwnd.c -- Bitmap viewer client window procedure
 *
 * This is the main drawing and interaction module.  It implements
 * BitmapViewerWndProc, the window procedure for the "ViewBmp" client class.
 *
 * Scaling modes (controlled by lScale):
 *   lScale == 0       "Show All" mode: all bitmaps in an array are displayed
 *                     side by side across the client width.
 *   lScale > 0        Single-bitmap mode: the selected bitmap is stretched to
 *                     lScale percent of its original size.
 *
 * Layout helpers:
 *   SizeBitmap()      Computes rclDest and aptlArea for a single bitmap,
 *                     scaling by lScale percent and centering in the client.
 *   SizeBitmapStack() Computes the same for all bitmaps in an array,
 *                     distributing them evenly across the client width.
 *
 * 3D border rendering (aptlArea):
 *   Each bitmap is surrounded by a two-layer drop-shadow border drawn with
 *   GpiPolyLine.  aptlArea[8] stores 4 pairs of POINTL values (8 points
 *   total):
 *     aptlArea[0..1] - outer highlight (top-left), drawn 2 pixels out
 *     aptlArea[2..3] - outer shadow (bottom-right), drawn 2 pixels out
 *     aptlArea[4..5] - inner highlight, drawn 1 pixel out
 *     aptlArea[6..7] - inner shadow, drawn 1 pixel out
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#include "appdefs.h"

/* RGB color constants not defined in kLIBC headers */
#define RGBCLR_PALEGRAY  0x00C0C0C0L
#define RGBCLR_WHITE     0x00FFFFFFL
#define RGBCLR_DARKGRAY  0x00808080L
#define RGBCLR_BLACK     0x00000000L

/* Current scale factor: 0=show-all, else percentage (25/50/75/100/200/400/800) */
static LONG lScale = 0;

/* -----------------------------------------------------------------------
 * SizeBitmap - compute layout rectangles for a single bitmap.
 *
 * i     : index into abm[] for the bitmap to lay out
 * rclClient : current client area rectangle (from WinQueryWindowRect)
 *
 * Sets abm[i].rclImage (full-size logical rect) and abm[i].rclDest
 * (scaled destination on screen), both bottom-left-origin.
 * Also computes abm[i].aptlArea for the two-layer border.
 * ----------------------------------------------------------------------- */
static VOID SizeBitmap(INT i, RECTL *rclClient)
{
    BITMAPINFOHEADER bmp2;
    LONG             cxBitmap, cyBitmap;
    LONG              cxDest, cyDest;
    LONG              xOrg, yOrg;

    /* Query the actual pixel dimensions of the GPI bitmap. */
    GpiQueryBitmapParameters(abm[i].hbm, &bmp2);
    cxBitmap = bmp2.cx;
    cyBitmap = bmp2.cy;

    if (lScale == 0) {
        /* No explicit scale: fit the bitmap in the client area. */
        cxDest = cxBitmap;
        cyDest = cyBitmap;
    } else {
        cxDest = (cxBitmap * lScale) / 100L;
        cyDest = (cyBitmap * lScale) / 100L;
    }

    /* Centre the scaled bitmap in the client area. */
    xOrg = (rclClient->xRight - cxDest) / 2L;
    yOrg = (rclClient->yTop   - cyDest) / 2L;

    abm[i].rclDest.xLeft   = xOrg;
    abm[i].rclDest.yBottom = yOrg;
    abm[i].rclDest.xRight  = xOrg + cxDest;
    abm[i].rclDest.yTop    = yOrg + cyDest;

    abm[i].rclImage.xLeft   = 0;
    abm[i].rclImage.yBottom = 0;
    abm[i].rclImage.xRight  = cxBitmap;
    abm[i].rclImage.yTop    = cyBitmap;

    /* Compute the 3D border points around rclDest.
     * Four L-shaped pairs: each pair draws two sides of the border.
     * The order is: top-right corner going left then down (highlight),
     * then bottom-left corner going right then up (shadow).            */

    /* Outer highlight (top and left, 2px outside dest) */
    abm[i].aptlArea[0].x = abm[i].rclDest.xRight  + 2L;
    abm[i].aptlArea[0].y = abm[i].rclDest.yTop     + 2L;
    abm[i].aptlArea[1].x = abm[i].rclDest.xLeft    - 2L;
    abm[i].aptlArea[1].y = abm[i].rclDest.yTop     + 2L;

    /* Outer shadow (bottom and right, 2px outside dest) */
    abm[i].aptlArea[2].x = abm[i].rclDest.xLeft    - 2L;
    abm[i].aptlArea[2].y = abm[i].rclDest.yBottom  - 2L;
    abm[i].aptlArea[3].x = abm[i].rclDest.xRight   + 2L;
    abm[i].aptlArea[3].y = abm[i].rclDest.yBottom  - 2L;

    /* Inner highlight (1px outside) */
    abm[i].aptlArea[4].x = abm[i].rclDest.xRight  + 1L;
    abm[i].aptlArea[4].y = abm[i].rclDest.yTop     + 1L;
    abm[i].aptlArea[5].x = abm[i].rclDest.xLeft    - 1L;
    abm[i].aptlArea[5].y = abm[i].rclDest.yTop     + 1L;

    /* Inner shadow (1px outside) */
    abm[i].aptlArea[6].x = abm[i].rclDest.xLeft    - 1L;
    abm[i].aptlArea[6].y = abm[i].rclDest.yBottom  - 1L;
    abm[i].aptlArea[7].x = abm[i].rclDest.xRight   + 1L;
    abm[i].aptlArea[7].y = abm[i].rclDest.yBottom  - 1L;
}

/* -----------------------------------------------------------------------
 * SizeBitmapStack - lay out all bitmaps in the array side by side.
 *
 * Distributes all cBitmaps across the client width.  Each bitmap gets an
 * equal share of the horizontal space, centred vertically.
 * ----------------------------------------------------------------------- */
static VOID SizeBitmapStack(RECTL *rclClient)
{
    INT  i;
    LONG cxSlot;    /* Width of each bitmap's slot in the client area */

    if (cBitmaps <= 0)
        return;

    cxSlot = (rclClient->xRight - rclClient->xLeft) / cBitmaps;

    for (i = 0; i < cBitmaps; i++) {
        BITMAPINFOHEADER bmp2;
        LONG cxBitmap, cyBitmap;
        LONG cxDest, cyDest;
        LONG xOrg, yOrg;

        GpiQueryBitmapParameters(abm[i].hbm, &bmp2);
        cxBitmap = bmp2.cx;
        cyBitmap = bmp2.cy;

        /* Scale to fit the slot, preserving aspect ratio. */
        cxDest = cxSlot - 8L;  /* 4px margin each side */
        if (cxDest < 1L) cxDest = 1L;
        cyDest = (cxBitmap > 0)
                 ? (cyBitmap * cxDest) / cxBitmap
                 : cyBitmap;

        /* Centre within the slot horizontally; centre vertically in client. */
        xOrg = rclClient->xLeft + i * cxSlot
               + (cxSlot - cxDest) / 2L;
        yOrg = (rclClient->yTop - cyDest) / 2L;

        abm[i].rclDest.xLeft   = xOrg;
        abm[i].rclDest.yBottom = yOrg;
        abm[i].rclDest.xRight  = xOrg + cxDest;
        abm[i].rclDest.yTop    = yOrg + cyDest;

        abm[i].rclImage.xLeft   = 0;
        abm[i].rclImage.yBottom = 0;
        abm[i].rclImage.xRight  = cxBitmap;
        abm[i].rclImage.yTop    = cyBitmap;

        /* 3D border points */
        abm[i].aptlArea[0].x = abm[i].rclDest.xRight  + 2L;
        abm[i].aptlArea[0].y = abm[i].rclDest.yTop     + 2L;
        abm[i].aptlArea[1].x = abm[i].rclDest.xLeft    - 2L;
        abm[i].aptlArea[1].y = abm[i].rclDest.yTop     + 2L;

        abm[i].aptlArea[2].x = abm[i].rclDest.xLeft    - 2L;
        abm[i].aptlArea[2].y = abm[i].rclDest.yBottom  - 2L;
        abm[i].aptlArea[3].x = abm[i].rclDest.xRight   + 2L;
        abm[i].aptlArea[3].y = abm[i].rclDest.yBottom  - 2L;

        abm[i].aptlArea[4].x = abm[i].rclDest.xRight  + 1L;
        abm[i].aptlArea[4].y = abm[i].rclDest.yTop     + 1L;
        abm[i].aptlArea[5].x = abm[i].rclDest.xLeft    - 1L;
        abm[i].aptlArea[5].y = abm[i].rclDest.yTop     + 1L;

        abm[i].aptlArea[6].x = abm[i].rclDest.xLeft    - 1L;
        abm[i].aptlArea[6].y = abm[i].rclDest.yBottom  - 1L;
        abm[i].aptlArea[7].x = abm[i].rclDest.xRight   + 1L;
        abm[i].aptlArea[7].y = abm[i].rclDest.yBottom  - 1L;
    }
}

/*
 * BitmapViewerWndProc - client window procedure for the bitmap viewer.
 *
 * Handles:
 *   WM_SIZE       : recompute layout when window is resized
 *   WM_PAINT      : draw background, bitmaps, and 3D borders
 *   WM_COMMAND    : menu commands (File/Scale/Effects)
 *   WM_BUTTON2CLICK: right-click shows appropriate popup menu
 *   WM_INITMENU   : update Scale checkmarks and Effects enable/disable state
 */
MRESULT EXPENTRY BitmapViewerWndProc(HWND hwnd, ULONG msg,
                                     MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        /* ---------------------------------------------------------------
         * WM_SIZE - window size has changed.
         * Recompute all bitmap layout rectangles to fill the new client area.
         * --------------------------------------------------------------- */
        case WM_SIZE:
        {
            RECTL rclClient;
            WinQueryWindowRect(hwnd, &rclClient);

            if (cBitmaps > 0) {
                if (lScale == 0 && cBitmaps > 1)
                    SizeBitmapStack(&rclClient);
                else
                    SizeBitmap(iBitmap, &rclClient);
            }
            WinInvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }

        /* ---------------------------------------------------------------
         * WM_PAINT - render the window contents.
         *
         * Drawing order:
         *   1. Set RGB colour table mode for WinFillRect
         *   2. Fill background with pale grey
         *   3. For each bitmap: draw 3D border then WinDrawBitmap
         * --------------------------------------------------------------- */
        case WM_PAINT:
        {
            HPS    hps;
            RECTL  rclClient;
            INT    i;
            INT    iFirst, iLast;

            hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);

            /* Switch to direct RGB colour table so RGBCLR_ constants work. */
            GpiCreateLogColorTable(hps, 0L, LCOLF_RGB, 0L, 0L, NULL);

            WinQueryWindowRect(hwnd, &rclClient);
            WinFillRect(hps, &rclClient, RGBCLR_PALEGRAY);

            if (cBitmaps <= 0) {
                WinEndPaint(hps);
                return 0;
            }

            /* In show-all mode, draw every bitmap; otherwise just iBitmap. */
            if (lScale == 0 && cBitmaps > 1) {
                iFirst = 0;
                iLast  = cBitmaps - 1;
            } else {
                iFirst = iLast = iBitmap;
            }

            for (i = iFirst; i <= iLast; i++) {
                if (!abm[i].hbm)
                    continue;

                /* Draw the outer 3D border (2 px highlight + 2 px shadow). */
                GpiSetColor(hps, RGBCLR_WHITE);
                GpiMove(hps, &abm[i].aptlArea[0]);
                GpiPolyLine(hps, 2L, &abm[i].aptlArea[1]);

                GpiSetColor(hps, RGBCLR_DARKGRAY);
                GpiMove(hps, &abm[i].aptlArea[2]);
                GpiPolyLine(hps, 2L, &abm[i].aptlArea[3]);

                /* Draw the inner 3D border (1 px). */
                GpiSetColor(hps, RGBCLR_PALEGRAY);
                GpiMove(hps, &abm[i].aptlArea[4]);
                GpiPolyLine(hps, 2L, &abm[i].aptlArea[5]);

                GpiSetColor(hps, RGBCLR_BLACK);
                GpiMove(hps, &abm[i].aptlArea[6]);
                GpiPolyLine(hps, 2L, &abm[i].aptlArea[7]);

                /* Draw the bitmap, stretched to fill rclDest.
                 * For 2-colour (1-bit) bitmaps DBM_STRETCH alone is not
                 * enough — we must supply explicit foreground and background
                 * colours.  For all other depths DBM_STRETCH suffices.    */
                if (abm[i].cColours == 2) {
                    PBITMAPFILEHEADER2  pbfh2;
                    RGB2               *prgb2;
                    LONG                clrFore, clrBack;

                    pbfh2 = (PBITMAPFILEHEADER2)abm[i].pb;
                    prgb2 = (RGB2 *)((PBYTE)&pbfh2->bmp2
                                     + pbfh2->bmp2.cbFix);

                    /* Extract the two palette entries as LONG RGB values. */
                    clrBack = ((LONG)prgb2[0].bRed   << 16)
                            | ((LONG)prgb2[0].bGreen  <<  8)
                            |  (LONG)prgb2[0].bBlue;
                    clrFore = ((LONG)prgb2[1].bRed   << 16)
                            | ((LONG)prgb2[1].bGreen  <<  8)
                            |  (LONG)prgb2[1].bBlue;

                    WinDrawBitmap(hps, abm[i].hbm, NULL,
                                  (PPOINTL)&abm[i].rclDest,
                                  clrFore, clrBack,
                                  DBM_STRETCH);
                } else {
                    WinDrawBitmap(hps, abm[i].hbm, NULL,
                                  (PPOINTL)&abm[i].rclDest,
                                  0L, 0L,
                                  DBM_STRETCH);
                }
            }

            WinEndPaint(hps);
            return 0;
        }

        /* ---------------------------------------------------------------
         * WM_COMMAND - handle menu selections.
         * --------------------------------------------------------------- */
        case WM_COMMAND:
        {
            RECTL rclClient;

            switch (SHORT1FROMMP(mp1))
            {
                case IDM_OPEN:
                {
                    /* Show the custom file open dialog. */
                    CHAR szFile[CCHMAXPATH];
                    szFile[0] = '\0';
                    if (WinDlgBox(HWND_DESKTOP, hwnd,
                                  OpenBitmapDlgProc,
                                  (HMODULE)NULL,
                                  DLG_OPENBITMAP, szFile) == DID_OK
                        && szFile[0])
                    {
                        WinSetPointer(HWND_DESKTOP, hptrWait);
                        hbmGetBitmap(hwnd, szFile);
                        WinSetPointer(HWND_DESKTOP, hptrArrow);
                        WinQueryWindowRect(hwnd, &rclClient);
                        if (lScale == 0 && cBitmaps > 1)
                            SizeBitmapStack(&rclClient);
                        else
                            SizeBitmap(iBitmap, &rclClient);
                        WinInvalidateRect(hwnd, NULL, FALSE);
                    }
                    return 0;
                }

                case IDM_VIEWSYSTEMINFO:
                    WinDlgBox(HWND_DESKTOP, hwnd,
                              ViewSystemBitmapInfoDlgProc,
                              (HMODULE)NULL,
                              DLG_VIEWBITMAPSYSINO, NULL);
                    return 0;

                /* Scale commands: update lScale and trigger repaint. */
                case IDM_25PERCENT:  lScale = 25L;  goto recalc;
                case IDM_50PERCENT:  lScale = 50L;  goto recalc;
                case IDM_75PERCENT:  lScale = 75L;  goto recalc;
                case IDM_100PERCENT: lScale = 100L; goto recalc;
                case IDM_200PERCENT: lScale = 200L; goto recalc;
                case IDM_400PERCENT: lScale = 400L; goto recalc;
                case IDM_800PERCENT: lScale = 800L; goto recalc;
                case IDM_SHOWALL:    lScale = 0L;   goto recalc;

                recalc:
                    iBitmap = iBitmapSelected;
                    WinQueryWindowRect(hwnd, &rclClient);
                    if (lScale == 0 && cBitmaps > 1)
                        SizeBitmapStack(&rclClient);
                    else
                        SizeBitmap(iBitmap, &rclClient);
                    WinInvalidateRect(hwnd, NULL, FALSE);
                    return 0;

                /* Effects */
                case IDM_FLIPCOLOURTABLE:
                    if (cBitmaps > 0 && abm[iBitmap].cColours > 0) {
                        abm[iBitmap].hbm = hbmFlipColourTable(
                            abm[iBitmap].pb,
                            abm[iBitmap].hbm,
                            abm[iBitmap].cColours);
                        WinInvalidateRect(hwnd, NULL, FALSE);
                    }
                    return 0;

                /* Popup menu: open header inspection dialogs for the
                 * currently selected bitmap (iBitmapSelected). */
                case IDM_BITMAPARRAYFILEHEADER:
                    if (f20Bitmap)
                        WinDlgBox(HWND_DESKTOP, hwnd,
                                  ViewBITMAPARRAYFILEHEADER2DlgProc,
                                  (HMODULE)NULL,
                                  DLG_VIEWBITMAPARRAYFILEHEADER2,
                                  (PVOID)(LONG)iBitmapSelected);
                    else
                        WinDlgBox(HWND_DESKTOP, hwnd,
                                  ViewBITMAPARRAYFILEHEADERDlgProc,
                                  (HMODULE)NULL,
                                  DLG_VIEWBITMAPARRAYFILEHEADER,
                                  (PVOID)(LONG)iBitmapSelected);
                    return 0;

                case IDM_BITMAPFILEHEADER:
                    if (f20Bitmap)
                        WinDlgBox(HWND_DESKTOP, hwnd,
                                  ViewBITMAPFILEHEADER2DlgProc,
                                  (HMODULE)NULL,
                                  DLG_VIEWBITMAPFILEHEADER2,
                                  (PVOID)(LONG)iBitmapSelected);
                    else
                        WinDlgBox(HWND_DESKTOP, hwnd,
                                  ViewBITMAPFILEHEADERDlgProc,
                                  (HMODULE)NULL,
                                  DLG_VIEWBITMAPFILEHEADER,
                                  (PVOID)(LONG)iBitmapSelected);
                    return 0;

                case IDM_BITMAPINFOHEADER:
                    if (f20Bitmap)
                        WinDlgBox(HWND_DESKTOP, hwnd,
                                  ViewBITMAPINFOHEADER2DlgProc,
                                  (HMODULE)NULL,
                                  DLG_VIEWBITMAPINFOHEADER2,
                                  (PVOID)(LONG)iBitmapSelected);
                    else
                        WinDlgBox(HWND_DESKTOP, hwnd,
                                  ViewBITMAPINFOHEADERDlgProc,
                                  (HMODULE)NULL,
                                  DLG_VIEWBITMAPINFOHEADER,
                                  (PVOID)(LONG)iBitmapSelected);
                    return 0;

                case IDM_COLOURTABLE:
                {
                    ULONG idDlg = (abm[iBitmapSelected].cColours == 2)
                                  ? DLG_VIEW2COLOURTABLE
                                  : DLG_VIEWCOLOURTABLE;
                    WinDlgBox(HWND_DESKTOP, hwnd,
                              ViewColourTableDlgProc,
                              (HMODULE)NULL,
                              idDlg,
                              (PVOID)(LONG)iBitmapSelected);
                    return 0;
                }
            }
            break;
        }

        /* ---------------------------------------------------------------
         * WM_BUTTON2CLICK - right mouse button: show context menu.
         * Use the array popup when multiple bitmaps are present,
         * the single popup otherwise.
         * --------------------------------------------------------------- */
        case WM_BUTTON2CLICK:
        {
            POINTL ptl;
            INT    i;
            ULONG  idPopup;

            /* Find which bitmap was right-clicked (hit test). */
            ptl.x = SHORT1FROMMP(mp1);
            ptl.y = SHORT2FROMMP(mp1);
            iBitmapSelected = iBitmap;
            for (i = 0; i < cBitmaps; i++) {
                if (ptl.x >= abm[i].rclDest.xLeft  &&
                    ptl.x <= abm[i].rclDest.xRight  &&
                    ptl.y >= abm[i].rclDest.yBottom &&
                    ptl.y <= abm[i].rclDest.yTop) {
                    iBitmapSelected = i;
                    break;
                }
            }

            idPopup = (cBitmaps > 1) ? IDM_POPUP : IDM_POPUPSINGLE;
            WinPopupMenu(hwnd, hwnd,
                         WinLoadMenu(HWND_OBJECT, (HMODULE)NULL, idPopup),
                         ptl.x, ptl.y,
                         0,
                         PU_HCONSTRAIN | PU_VCONSTRAIN |
                         PU_MOUSEBUTTON2 | PU_KEYBOARD);
            return 0;
        }

        /* ---------------------------------------------------------------
         * WM_INITMENU - update menu item states before the menu is shown.
         * --------------------------------------------------------------- */
        case WM_INITMENU:
        {
            HWND  hmenu = HWNDFROMMP(mp2);

            /* Check the current scale item; uncheck all others. */
            static const ULONG aScaleIds[] = {
                IDM_25PERCENT, IDM_50PERCENT, IDM_75PERCENT, IDM_100PERCENT,
                IDM_200PERCENT, IDM_400PERCENT, IDM_800PERCENT, IDM_SHOWALL
            };
            static const LONG aScaleVals[] = {
                25L, 50L, 75L, 100L, 200L, 400L, 800L, 0L
            };
            ULONG n;

            for (n = 0; n < 8; n++) {
                WinCheckMenuItem(hmenu, aScaleIds[n],
                                 (lScale == aScaleVals[n]));
            }

            /* Enable flip-colour only for palettised bitmaps. */
            WinEnableMenuItem(hmenu, IDM_FLIPCOLOURTABLE,
                              (cBitmaps > 0 && abm[iBitmap].cColours > 0));
            return 0;
        }

        case WM_DESTROY:
        {
            INT i;
            for (i = 0; i < cBitmaps; i++)
                if (abm[i].hbm)
                    GpiDeleteBitmap(abm[i].hbm);
            if (pb)
                free(pb);
            return 0;
        }
    }

    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

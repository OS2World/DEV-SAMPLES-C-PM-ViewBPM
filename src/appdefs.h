/*
 * appdefs.h -- Global application state declarations and prototypes
 *
 * This header declares all global variables used across the Bitmap Viewer
 * modules and forward-declares every public function.  Exactly one source
 * file (appdefs.c) defines these variables; all other modules include
 * this header and see them as extern.
 *
 * Global variable strategy: the application is single-instance (one window,
 * one bitmap load at a time), so a flat set of globals is the right fit for
 * a sample of this era.  A production design would package these into a
 * per-window or per-document data structure.
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#ifndef APPDEFS_H
#define APPDEFS_H

/* Pull in every OS/2 API subsystem used by the viewer. */
#define INCL_WIN
#define INCL_GPI
#define INCL_DOS
#define INCL_BITMAPFILEFORMAT   /* BFT_BMAP, BFT_BITMAPARRAY, and all       */
                                /* BITMAPARRAYFILEHEADER / BITMAPFILEHEADER2 */
                                /* structures and constants                  */
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "viewbmp.h"
#include "winbmp.h"

/* -----------------------------------------------------------------------
 * BITMAPSTACK - per-bitmap state for one entry in a bitmap array file
 *
 * A single .BMP file may contain multiple bitmap images chained via
 * BITMAPARRAYFILEHEADER.offNext.  For each such image the viewer creates
 * one BITMAPSTACK entry that holds:
 *   hbm       - the GPI bitmap handle created by GpiCreateBitmap()
 *   rclDest   - destination rectangle in client-window coordinates
 *   rclImage  - full-resolution rectangle (before scaling)
 *   aptlArea  - 8 points for the two-layer 3D drop-shadow border
 *   pb        - pointer into the raw file-data buffer for this image's header
 *   cColours  - number of palette entries (0 for 24-bit, >0 for palettised)
 * ----------------------------------------------------------------------- */
typedef struct _BITMAPSTACK {
    HBITMAP hbm;            /* GPI bitmap handle                            */
    RECTL   rclDest;        /* Scaled destination rectangle on screen       */
    RECTL   rclImage;       /* Full-size image rectangle                    */
    POINTL  aptlArea[8];    /* 4 pairs: outer ±2px and inner ±1px borders   */
    PBYTE   pb;             /* Pointer to BITMAPFILEHEADER2 in raw data     */
    PBYTE   pbArray;        /* Pointer to BITMAPARRAYFILEHEADER2 (or NULL)  */
    LONG    cColours;       /* Palette colour count (0 = truecolour)        */
} BITMAPSTACK;

/* -----------------------------------------------------------------------
 * Presentation space and font metrics
 * fm  - FONTMETRICS from GpiQueryFontMetrics; used to get dialog unit sizes
 * ----------------------------------------------------------------------- */
extern FONTMETRICS fm;

/* -----------------------------------------------------------------------
 * PM session handles
 * ----------------------------------------------------------------------- */
extern HAB    hAB;              /* Anchor block from WinInitialize()         */
extern HSWITCH hSwitch;         /* Task switcher entry handle                */
extern HMQ    hmqViewBmp;       /* Application message queue                 */
extern HWND   hwndViewBmp;      /* Client window handle                      */
extern HWND   hwndViewBmpFrame; /* Frame window handle                       */
extern HWND   hmenuViewBmp;     /* Main menu bar handle (HWND in OS/2 PM)    */
extern HWND   hmenuPopup;       /* Right-click popup menu handle             */
extern HPOINTER hptrArrow;      /* Standard arrow cursor                     */
extern HPOINTER hptrWait;       /* Hourglass / wait cursor                   */

/* -----------------------------------------------------------------------
 * Display metrics
 * ----------------------------------------------------------------------- */
extern LONG   cxScreen;         /* Screen width in pixels (CAPS_WIDTH)       */

/* -----------------------------------------------------------------------
 * Current bitmap state
 * ----------------------------------------------------------------------- */
extern CHAR   szBitmapFile[CCHMAXPATH]; /* Full path of the loaded .BMP file */

/* Up to 32 bitmaps from a bitmap-array file.  Single .BMP files use abm[0].*/
extern BITMAPSTACK abm[32];
extern INT    cBitmaps;         /* Number of valid entries in abm[]          */

/* Raw file data: the entire .BMP file is read into a malloc'd block.
 * pb points into that block.  pbfh2 / pbfh are typed views of the same data
 * and are used according to which format the file uses.                      */
extern PBYTE  pb;

/* Format flags set by hbmGetBitmap() after format detection: */
extern BOOL   f20Bitmap;        /* TRUE = OS/2 2.x format                   */
extern BOOL   fWindowsBitmap;   /* TRUE = Windows 3.x format                */

/* Index of the currently selected (highlighted) bitmap in the array. */
extern INT    iBitmap;
extern INT    iBitmapSelected;

/* TRUE when a bitmap array has been loaded (multiple images in one file). */
extern BOOL   fBitmapArray;

/* -----------------------------------------------------------------------
 * Function prototypes - BmpWnd.C
 * ----------------------------------------------------------------------- */
MRESULT EXPENTRY BitmapViewerWndProc(HWND hwnd, ULONG msg,
                                     MPARAM mp1, MPARAM mp2);

/* -----------------------------------------------------------------------
 * Function prototypes - Effects.C
 * ----------------------------------------------------------------------- */
HBITMAP hbmFlipColourTable(PBYTE pb, HBITMAP hbm, LONG cColours);
HBITMAP hbmRefreshBitmap(PBYTE pb, HBITMAP hbm);

/* -----------------------------------------------------------------------
 * Function prototypes - GetBmp.C
 * ----------------------------------------------------------------------- */
BOOL hbmGetBitmap(HWND hwnd, PSZ pszBitmapFile);

/* -----------------------------------------------------------------------
 * Function prototypes - OpenDlg.C
 * ----------------------------------------------------------------------- */
MRESULT EXPENTRY OpenBitmapDlgProc(HWND hwnd, ULONG msg,
                                   MPARAM mp1, MPARAM mp2);

/* -----------------------------------------------------------------------
 * Function prototypes - Support.C
 * ----------------------------------------------------------------------- */
VOID   InitApp(HWND hwndFrame, HWND hwnd, PSZ pszFilename);
HWND   CreateStdWindow(HWND hwndParent, ULONG flStyle,
                       ULONG flCreateFlags, PSZ pszClientClass,
                       PSZ pszTitle, ULONG styleClient,
                       HMODULE hmod, ULONG idResources,
                       PHWND phwndClient,
                       SHORT sX, SHORT sY, SHORT sCX, SHORT sCY);
MRESULT EXPENTRY PDSKeyProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID   PDSGetTemplate(HWND hwnd, ULONG idDlg);

/* -----------------------------------------------------------------------
 * Function prototypes - ViewDlg.C
 * ----------------------------------------------------------------------- */
MRESULT EXPENTRY ViewBITMAPARRAYFILEHEADER2DlgProc(HWND hwnd, ULONG msg,
                                                     MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ViewBITMAPFILEHEADER2DlgProc(HWND hwnd, ULONG msg,
                                               MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ViewBITMAPINFOHEADER2DlgProc(HWND hwnd, ULONG msg,
                                               MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ViewBITMAPARRAYFILEHEADERDlgProc(HWND hwnd, ULONG msg,
                                                    MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ViewBITMAPFILEHEADERDlgProc(HWND hwnd, ULONG msg,
                                              MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ViewBITMAPINFOHEADERDlgProc(HWND hwnd, ULONG msg,
                                              MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ViewColourTableDlgProc(HWND hwnd, ULONG msg,
                                        MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ViewSystemBitmapInfoDlgProc(HWND hwnd, ULONG msg,
                                             MPARAM mp1, MPARAM mp2);

#endif /* APPDEFS_H */

/*
 * appdefs.c -- Global application state definitions
 *
 * This file defines (allocates storage for) every global variable declared
 * as extern in appdefs.h.  It is the only file that must be linked when
 * the globals change; all other modules include appdefs.h and reference
 * them by name via the extern linkage.
 *
 * See appdefs.h for descriptions of each variable.
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#include "appdefs.h"

/* Font metrics used to compute dialog-unit conversions in Support.C */
FONTMETRICS fm;

/* PM session handles */
HAB     hAB;
HSWITCH hSwitch;
HMQ     hmqViewBmp;
HWND    hwndViewBmp;
HWND    hwndViewBmpFrame;
HWND    hmenuViewBmp;
HWND    hmenuPopup;
HPOINTER hptrArrow;
HPOINTER hptrWait;

/* Screen width — queried once at startup, used to pick target resolution
 * when selecting the best-fit bitmap from an array. */
LONG    cxScreen;

/* Full path of the currently loaded bitmap file */
CHAR    szBitmapFile[CCHMAXPATH];

/* Array of per-bitmap state.  Index 0 is always the first (or only) bitmap.
 * Entries 1..cBitmaps-1 are valid when a bitmap array was loaded. */
BITMAPSTACK abm[32];

/* Number of bitmaps currently loaded into abm[] */
INT     cBitmaps = 0;

/* Pointer to the raw malloc'd file buffer.  Individual abm[i].pb fields
 * point into this same buffer at the start of each bitmap's header. */
PBYTE   pb;

/* Format flags — set in GetBmp.C after inspecting the file header. */
BOOL    f20Bitmap      = FALSE;   /* TRUE = OS/2 2.x BITMAPFILEHEADER2     */
BOOL    fWindowsBitmap = FALSE;   /* TRUE = Windows 3.x winBITMAPFILEHEADER */

/* Index of the bitmap currently displayed / selected */
INT     iBitmap         = 0;
INT     iBitmapSelected = 0;

/* TRUE when the loaded file is a BITMAPARRAYFILEHEADER chain */
BOOL    fBitmapArray = FALSE;

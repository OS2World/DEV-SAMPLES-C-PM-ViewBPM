/*
 * viewbmp.c -- Application entry point and window class registration
 *
 * This module contains main() which:
 *   1. Initialises PM (WinInitialize / WinCreateMsgQueue)
 *   2. Registers the "ViewBmp" client window class with CS_SYNCPAINT and
 *      CS_SIZEREDRAW styles and 8 bytes of per-window data
 *   3. Creates the standard PM frame+client window pair via CreateStdWindow()
 *      (a helper in Support.C that converts dialog-unit coordinates to pixels)
 *   4. Calls InitApp() to set up font metrics and the task-switcher entry
 *   5. Runs the standard PM message loop
 *   6. Cleans up on exit
 *
 * Window class notes:
 *   CS_SYNCPAINT  - WM_PAINT is sent synchronously, preventing flicker when
 *                   the window is resized or uncovered.
 *   CS_SIZEREDRAW - WM_SIZE is sent automatically whenever the client area
 *                   changes size, triggering BitmapViewerWndProc to recompute
 *                   bitmap layout rectangles.
 *   8UL extra bytes - reserved for future per-window storage (QWL_USER etc.).
 *
 * Version: 1.01
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - reorganised into src/, build scripts added,
 *                 documentation improved.
 */

#pragma subsystem(PM)

#include "appdefs.h"

/* The name used for WinRegisterClass and WinCreateStdWindow must match. */
PSZ pszViewBmpClassName = (PSZ)"ViewBmp";

/*
 * main() - application entry point.
 */
INT main(INT argc, CHAR *argv[])
{
    QMSG qmsg;    /* Message retrieved from the queue by WinGetMsg() */

    /* --- 1. Connect to PM -------------------------------------------- */
    hmqViewBmp = WinCreateMsgQueue(hAB = WinInitialize(0UL), 0L);

    if (!hAB) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      (PSZ)"WinInitialize failed", (PSZ)"ViewBmp Debug", 0,
                      MB_OK | MB_ERROR);
        return 0;
    }
    if (!hmqViewBmp) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      (PSZ)"WinCreateMsgQueue failed", (PSZ)"ViewBmp Debug", 0,
                      MB_OK | MB_ERROR);
        return 0;
    }

    /* --- 2. Register the client window class -------------------------- */
    if (!WinRegisterClass(hAB,
                          pszViewBmpClassName,
                          (PFNWP)BitmapViewerWndProc,
                          CS_SYNCPAINT | CS_SIZEREDRAW,
                          8UL)) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      (PSZ)"WinRegisterClass failed", (PSZ)"ViewBmp Debug", 0,
                      MB_OK | MB_ERROR);
        return 0;
    }

    /* --- 3. Create the standard PM window ----------------------------- */
    if (!(hwndViewBmpFrame = CreateStdWindow(
            HWND_DESKTOP,
            WS_VISIBLE,
            FCF_NOBYTEALIGN | FCF_TASKLIST | FCF_TITLEBAR | FCF_ICON |
                FCF_SYSMENU | FCF_MENU | FCF_MINMAX | FCF_SIZEBORDER,
            pszViewBmpClassName,
            (PSZ)"Bitmap Viewer",
            0L,
            (HMODULE)NULL,
            WIN_BITMAPVIEWER,
            &hwndViewBmp,
            10, 10, 150, 75))) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      (PSZ)"CreateStdWindow failed", (PSZ)"ViewBmp Debug", 0,
                      MB_OK | MB_ERROR);
        return 0;
    }

    /* --- 4. Post-creation initialisation ------------------------------ */
    /*
     * InitApp (Support.C) queries font metrics (for dialog unit conversions),
     * loads menu and pointer handles, and adds the app to the task switcher.
     * The third argument is an optional filename to open at startup (NULL here).
     */
    InitApp(hwndViewBmpFrame, hwndViewBmp, NULL);

    /* --- 5. Standard PM message loop ---------------------------------- */
    while (WinGetMsg(hAB, &qmsg, (HWND)NULL, 0UL, 0UL))
        WinDispatchMsg(hAB, &qmsg);

    /* --- 6. Cleanup --------------------------------------------------- */
    WinDestroyWindow(hwndViewBmpFrame);
    WinDestroyMsgQueue(hmqViewBmp);
    WinTerminate(hAB);

    return 0;
}

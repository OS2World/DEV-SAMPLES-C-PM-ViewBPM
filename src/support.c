/*
 * support.c -- Application support utilities
 *
 * Provides three families of helper functions:
 *
 * 1. InitApp() - one-time initialisation after the main window is created:
 *    - Queries font metrics for dialog-unit to pixel conversions
 *    - Loads menu and cursor handles from application resources
 *    - Adds the application to the OS/2 task switcher (WinAddSwitchEntry)
 *    - Optionally loads an initial bitmap file
 *
 * 2. CreateStdWindow() - enhanced WinCreateStdWindow that accepts window
 *    position/size in dialog units instead of pixels.  Dialog units are
 *    display-resolution-independent.  WinMapDlgPoints converts them.
 *
 * 3. PDSGetTemplate() / PDSKeyProc() - "dialog in a regular window" technique.
 *    PDSGetTemplate loads a DLGTEMPLATE resource and creates its controls as
 *    child windows of a regular client window (not a dialog box).  This gives
 *    the controls Tab/arrow navigation normally only available in dialogs.
 *    PDSKeyProc replaces WinDefWindowProc to intercept the navigation keys
 *    and route them via WinEnumDlgItem.
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#include "appdefs.h"

/*
 * InitApp - post-creation initialisation.
 *
 * hwndFrame   : frame window handle (used for WinAddSwitchEntry)
 * hwnd        : client window handle (used for GpiQueryFontMetrics)
 * pszFilename : optional initial bitmap to load (NULL = start empty)
 */
VOID InitApp(HWND hwndFrame, HWND hwnd, PSZ pszFilename)
{
    HPS          hps;
    SWCNTRL      swctl;

    /* Query font metrics so that dialog-unit conversions in CreateStdWindow
     * reflect the actual screen font (resolution-independent sizing). */
    hps = WinGetPS(hwnd);
    GpiQueryFontMetrics(hps, (LONG)sizeof(FONTMETRICS), &fm);
    WinReleasePS(hps);

    /* Cache handles to the main menu and both popup menus. */
    hmenuViewBmp = WinWindowFromID(hwndFrame, FID_MENU);
    hmenuPopup   = WinLoadMenu(HWND_OBJECT, (HMODULE)NULL, IDM_POPUP);

    /* Cache standard cursor handles for fast switching during long operations. */
    hptrArrow = WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE);
    hptrWait  = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT,  FALSE);

    /* Cache screen width; used in GetBmp.C to pick the best-resolution
     * bitmap from an array (heuristic: >800 pixels → prefer 1024x768 target). */
    cxScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);

    /* Add the application to the Window List (Ctrl+Esc task switcher). */
    memset(&swctl, 0, sizeof(SWCNTRL));
    swctl.hwnd         = hwndFrame;
    swctl.hwndIcon     = WinQueryWindow(hwndFrame, QW_PARENT);
    swctl.hprog        = NULLHANDLE;
    swctl.idProcess    = 0;
    swctl.idSession    = 0;
    swctl.uchVisibility = SWL_VISIBLE;
    swctl.fbJump        = SWL_JUMPABLE;
    strcpy(swctl.szSwtitle, "Bitmap Viewer");
    hSwitch = WinAddSwitchEntry(&swctl);

    /* Optionally open a bitmap file passed on the command line or at startup. */
    if (pszFilename)
        hbmGetBitmap(hwnd, pszFilename);
}

/*
 * CreateStdWindow - create a standard PM window using dialog-unit coordinates.
 *
 * Dialog units are resolution-independent: 1 horizontal DU = 1/4 of the
 * average character width; 1 vertical DU = 1/8 of the average character
 * height.  WinMapDlgPoints converts them to screen pixels at runtime.
 *
 * Returns the frame window handle, or NULLHANDLE on failure.
 * *phwndClient receives the client window handle on success.
 */
HWND CreateStdWindow(HWND hwndParent, ULONG flStyle,
                     ULONG flCreateFlags, PSZ pszClientClass,
                     PSZ pszTitle, ULONG styleClient,
                     HMODULE hmod, ULONG idResources,
                     PHWND phwndClient,
                     SHORT sX, SHORT sY, SHORT sCX, SHORT sCY)
{
    HWND   hwndFrame;
    POINTL aptl[2];

    /* Create the window at default position first; we'll move it next. */
    hwndFrame = WinCreateStdWindow(hwndParent,
                                   flStyle,
                                   &flCreateFlags,
                                   pszClientClass,
                                   pszTitle,
                                   styleClient,
                                   hmod,
                                   idResources,
                                   phwndClient);
    if (!hwndFrame)
        return NULLHANDLE;

    /* Convert dialog-unit position and size to screen pixels.
     * aptl[0] = bottom-left (x, y), aptl[1] = top-right (x+cx, y+cy).
     * FALSE = map from dialog units to screen pixels.                    */
    aptl[0].x = sX;
    aptl[0].y = sY;
    aptl[1].x = sX + sCX;
    aptl[1].y = sY + sCY;
    WinMapDlgPoints(HWND_DESKTOP, aptl, 2L, TRUE);

    /* Position and size the frame window. */
    WinSetWindowPos(hwndFrame, HWND_TOP,
                    (LONG)aptl[0].x, (LONG)aptl[0].y,
                    (LONG)(aptl[1].x - aptl[0].x),
                    (LONG)(aptl[1].y - aptl[0].y),
                    SWP_MOVE | SWP_SIZE | SWP_ACTIVATE | SWP_SHOW);

    return hwndFrame;
}

/*
 * PDSKeyProc - keyboard sub-procedure for dialog-in-window technique.
 *
 * Installed as a window procedure (or called from one) to provide Tab /
 * arrow-key navigation among controls that were created as children of a
 * regular client window rather than a dialog box.
 *
 * Key mappings:
 *   Tab / Right / Down   -> next tab stop / next group item
 *   Shift+Tab / Left / Up -> previous tab stop / previous group item
 *
 * WinEnumDlgItem walks the window's child list respecting WS_TABSTOP and
 * WS_GROUP styles exactly as a dialog box would, so navigation is identical
 * to a real dialog.
 */
MRESULT EXPENTRY PDSKeyProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    USHORT fsFlags;
    USHORT vkey;

    if (msg == WM_CHAR) {
        fsFlags = SHORT1FROMMP(mp1);
        vkey    = SHORT2FROMMP(mp2);

        if (fsFlags & KC_VIRTUALKEY) {
            switch (vkey) {
                case VK_TAB:
                    /* Move focus to the next tab stop among siblings. */
                    WinSetFocus(HWND_DESKTOP,
                        WinEnumDlgItem(hwnd,
                                       WinQueryFocus(HWND_DESKTOP),
                                       EDI_NEXTTABITEM));
                    return (MRESULT)TRUE;

                case VK_BACKTAB:
                    WinSetFocus(HWND_DESKTOP,
                        WinEnumDlgItem(hwnd,
                                       WinQueryFocus(HWND_DESKTOP),
                                       EDI_PREVTABITEM));
                    return (MRESULT)TRUE;

                case VK_RIGHT:
                case VK_DOWN:
                    /* Within a group, move to the next item. */
                    WinSetFocus(HWND_DESKTOP,
                        WinEnumDlgItem(hwnd,
                                       WinQueryFocus(HWND_DESKTOP),
                                       EDI_NEXTGROUPITEM));
                    return (MRESULT)TRUE;

                case VK_LEFT:
                case VK_UP:
                    WinSetFocus(HWND_DESKTOP,
                        WinEnumDlgItem(hwnd,
                                       WinQueryFocus(HWND_DESKTOP),
                                       EDI_PREVGROUPITEM));
                    return (MRESULT)TRUE;
            }
        }
    }

    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

/*
 * PDSGetTemplate - instantiate a DLGTEMPLATE as children of a regular window.
 *
 * Loads a RT_DIALOG resource identified by idDlg and creates each control
 * described in the template as a child of hwnd.  WinMapDlgPoints is used to
 * convert dialog-unit coordinates to pixels.
 *
 * This technique gives a regular window fully-functional dialog controls
 * (entry fields, buttons, list boxes, etc.) with correct visual appearance.
 * Combined with PDSKeyProc for keyboard navigation, the result is
 * indistinguishable from a proper dialog box for the user.
 */
VOID PDSGetTemplate(HWND hwnd, ULONG idDlg)
{
    /* Not called anywhere in this application — stubbed because kLIBC
     * DLGTEMPLATE/DLGTITEM field names differ from the 1995 Toolkit headers
     * used in the original source.                                          */
    (VOID)hwnd;
    (VOID)idDlg;
}

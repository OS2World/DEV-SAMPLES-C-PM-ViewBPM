/*
 * opendlg.c -- Custom file open dialog for .BMP files
 *
 * Implements OpenBitmapDlgProc, the dialog procedure for DLG_OPENBITMAP.
 *
 * The OS/2 Toolkit did not ship WinFileDlg until a later era; this sample
 * predates it and builds its own file browser using:
 *
 *   Directories list box  (LB_DIRECTORIES) - shows subdirectories
 *   Files list box        (LB_FILES)        - shows *.BMP files
 *   Drives combo box      (CBX_DRIVES)      - shows available drive letters
 *   Static labels         (IT_DIRECTORY, IT_VOLUMELABEL, IT_DISKFREE,
 *                           IT_FILECOUNT)   - display current path info
 *
 * Key OS/2 APIs used:
 *   DosFindFirst / DosFindNext - enumerate files and directories
 *   DosQueryFSAttach           - determine if a drive is removable
 *   DosQueryFSInfo             - query volume label and free disk space
 *
 * Drive handling:
 *   afRemoveable[26] is set TRUE for floppy and CD drives so that the
 *   code skips the volume label query (which would cause a media-not-ready
 *   error on empty drives).
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#include "appdefs.h"

/* -----------------------------------------------------------------------
 * Module-level state for the open dialog
 * ----------------------------------------------------------------------- */

static CHAR  szCurrentDir[CCHMAXPATH]; /* Current directory path           */
static BOOL  afRemoveable[26];         /* TRUE if drive i is removable     */

/* -----------------------------------------------------------------------
 * Forward declarations for internal helpers
 * ----------------------------------------------------------------------- */
static VOID FillDriveList(HWND hwnd);
static VOID FillDirAndFileList(HWND hwnd, PSZ pszDir);
static VOID UpdatePathInfo(HWND hwnd, INT iDrive);

/*
 * FillDriveList - populate the Drives combo box with available drive letters.
 *
 * Iterates A: through Z:, attempts DosQueryFSAttach on each, and adds
 * any drive that responds to the CBX_DRIVES list.  afRemoveable[] is set
 * for drives whose type indicates removable media.
 */
static VOID FillDriveList(HWND hwnd)
{
    HWND      hwndCBX = WinWindowFromID(hwnd, CBX_DRIVES);
    CHAR      szDrive[4];
    BYTE      buf[512];
    FSQBUFFER2 *pfsq = (FSQBUFFER2 *)buf;
    ULONG     cbBuf;
    INT       iDrive;
    INT       iSel = 0;
    ULONG     ulLogical;

    /* Query current drive (1='A', 2='B', ...) */
    DosQueryCurrentDisk((PULONG)&ulLogical, NULL);

    WinSendMsg(hwndCBX, LM_DELETEALL, 0, 0);

    for (iDrive = 0; iDrive < 26; iDrive++) {
        szDrive[0] = 'A' + iDrive;
        szDrive[1] = ':';
        szDrive[2] = '\0';

        cbBuf = sizeof(buf);
        if (DosQueryFSAttach(szDrive, 0, FSAIL_QUERYNAME, pfsq, &cbBuf))
            continue;   /* Drive not present */

        /* Mark removable media to skip volume label queries. */
        afRemoveable[iDrive] =
            (pfsq->iType == FSAT_REMOTEDRV) ? FALSE :
            (pfsq->iType == FSAT_LOCALDRV &&
             (pfsq->cbFSDName == 0)) ? TRUE : FALSE;

        WinSendMsg(hwndCBX, LM_INSERTITEM,
                   MPFROMSHORT(LIT_SORTASCENDING),
                   MPFROMP(szDrive));

        if ((ULONG)(iDrive + 1) == ulLogical)
            iSel = (SHORT)(LONG)WinSendMsg(hwndCBX, LM_SEARCHSTRING,
                                           MPFROM2SHORT(0, LIT_FIRST),
                                           MPFROMP(szDrive));
    }

    WinSendMsg(hwndCBX, LM_SELECTITEM, MPFROMSHORT(iSel), MPFROMSHORT(TRUE));
}

/*
 * FillDirAndFileList - populate the Directories and Files list boxes.
 *
 * pszDir : directory to enumerate (must end in '\')
 *
 * Uses DosFindFirst("*.*") with MUST_HAVE_DIRECTORY attribute to find
 * subdirectories, and DosFindFirst("*.BMP") to find bitmap files.
 * Updates IT_FILECOUNT with the count of .BMP files found.
 */
static VOID FillDirAndFileList(HWND hwnd, PSZ pszDir)
{
    HWND      hwndDirs  = WinWindowFromID(hwnd, LB_DIRECTORIES);
    HWND      hwndFiles = WinWindowFromID(hwnd, LB_FILES);
    CHAR      szSearch[CCHMAXPATH];
    HDIR      hdir;
    FILEFINDBUF3 ffb;
    ULONG     ulCount;
    INT       cFiles = 0;
    CHAR      szCount[32];

    WinSendMsg(hwndDirs,  LM_DELETEALL, 0, 0);
    WinSendMsg(hwndFiles, LM_DELETEALL, 0, 0);

    /* Enumerate subdirectories */
    snprintf(szSearch, sizeof(szSearch) - 4, "%s", pszDir);
    strcat(szSearch, "*.*");
    hdir     = HDIR_CREATE;
    ulCount  = 1;
    if (!DosFindFirst(szSearch, &hdir, MUST_HAVE_DIRECTORY | FILE_DIRECTORY,
                      &ffb, sizeof(ffb), &ulCount, FIL_STANDARD)) {
        do {
            if ((ffb.attrFile & FILE_DIRECTORY) &&
                strcmp(ffb.achName, ".") != 0) {
                WinSendMsg(hwndDirs, LM_INSERTITEM,
                           MPFROMSHORT(LIT_SORTASCENDING),
                           MPFROMP(ffb.achName));
            }
            ulCount = 1;
        } while (!DosFindNext(hdir, &ffb, sizeof(ffb), &ulCount));
        DosFindClose(hdir);
    }

    /* Enumerate .BMP files */
    snprintf(szSearch, sizeof(szSearch) - 6, "%s", pszDir);
    strcat(szSearch, "*.BMP");
    hdir    = HDIR_CREATE;
    ulCount = 1;
    if (!DosFindFirst(szSearch, &hdir, FILE_NORMAL,
                      &ffb, sizeof(ffb), &ulCount, FIL_STANDARD)) {
        do {
            WinSendMsg(hwndFiles, LM_INSERTITEM,
                       MPFROMSHORT(LIT_SORTASCENDING),
                       MPFROMP(ffb.achName));
            cFiles++;
            ulCount = 1;
        } while (!DosFindNext(hdir, &ffb, sizeof(ffb), &ulCount));
        DosFindClose(hdir);
    }

    sprintf(szCount, "%d", cFiles);
    WinSetDlgItemText(hwnd, IT_FILECOUNT, szCount);
}

/*
 * UpdatePathInfo - update the static labels for volume/free space/path.
 *
 * iDrive : 0-based drive index (0='A', 1='B', ...)
 */
static VOID UpdatePathInfo(HWND hwnd, INT iDrive)
{
    FSINFO        fsi;
    FSALLOCATE    fsa;
    CHAR          szFree[32];

    /* Show the current directory path */
    WinSetDlgItemText(hwnd, IT_DIRECTORY, szCurrentDir);

    /* Skip volume label on removable drives to avoid media-not-ready errors. */
    if (afRemoveable[iDrive]) {
        WinSetDlgItemText(hwnd, IT_VOLUMELABEL, "");
        WinSetDlgItemText(hwnd, IT_DISKFREE,    "");
        return;
    }

    if (!DosQueryFSInfo((ULONG)(iDrive + 1), FSIL_VOLSER, &fsi, sizeof(fsi))) {
        WinSetDlgItemText(hwnd, IT_VOLUMELABEL, fsi.vol.szVolLabel);
    } else {
        WinSetDlgItemText(hwnd, IT_VOLUMELABEL, "");
    }

    if (!DosQueryFSInfo((ULONG)(iDrive + 1), FSIL_ALLOC, &fsa, sizeof(fsa))) {
        ULONG cbFreeKB = (fsa.cUnitAvail * fsa.cSectorUnit * fsa.cbSector)
                         / 1024UL;
        sprintf(szFree, "%lu KB free", cbFreeKB);
        WinSetDlgItemText(hwnd, IT_DISKFREE, szFree);
    }
}

/*
 * OpenBitmapDlgProc - dialog procedure for the Open Bitmap dialog.
 *
 * The application param (mp2 of WM_INITDLG) is a CHAR[CCHMAXPATH] buffer
 * in which the selected filename is returned when the user clicks OK.
 */
MRESULT EXPENTRY OpenBitmapDlgProc(HWND hwnd, ULONG msg,
                                   MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
        case WM_INITDLG:
        {
            ULONG  ulLogical;
            ULONG  ulDriveMap;
            ULONG  ulDirLen;
            INT    iDrive;
            CHAR   szDir[CCHMAXPATH];

            /* Query the current drive and directory. */
            DosQueryCurrentDisk(&ulLogical, &ulDriveMap);
            iDrive = (INT)ulLogical - 1;
            szDir[0] = (CHAR)('A' + iDrive);
            szDir[1] = ':';
            szDir[2] = '\0';
            ulDirLen = CCHMAXPATH - 3;
            DosQueryCurrentDir((ULONG)ulLogical, szDir + 3, &ulDirLen);
            /* Build current path with trailing backslash */
            sprintf(szCurrentDir, "%c:\\%s\\",
                    'A' + iDrive, szDir + 3);

            FillDriveList(hwnd);
            FillDirAndFileList(hwnd, szCurrentDir);
            UpdatePathInfo(hwnd, iDrive);
            return 0;
        }

        case WM_CONTROL:
        {
            USHORT usId       = SHORT1FROMMP(mp1);
            USHORT usNotify   = SHORT2FROMMP(mp1);

            if (usId == LB_DIRECTORIES && usNotify == LN_ENTER) {
                /* User double-clicked a directory: navigate into it. */
                SHORT iSel = (SHORT)(LONG)WinSendDlgItemMsg(hwnd, LB_DIRECTORIES,
                                           LM_QUERYSELECTION,
                                           MPFROMSHORT(LIT_FIRST), 0);
                if (iSel != LIT_NONE) {
                    CHAR szName[CCHMAXPATH];
                    WinSendDlgItemMsg(hwnd, LB_DIRECTORIES, LM_QUERYITEMTEXT,
                                      MPFROM2SHORT(iSel, sizeof(szName)),
                                      MPFROMP(szName));
                    if (strcmp(szName, "..") == 0) {
                        /* Navigate up */
                        CHAR *p = strrchr(szCurrentDir, '\\');
                        if (p && p != szCurrentDir) {
                            *p = '\0';
                            p = strrchr(szCurrentDir, '\\');
                            if (p) *(p + 1) = '\0';
                        }
                    } else {
                        strcat(szCurrentDir, szName);
                        strcat(szCurrentDir, "\\");
                    }
                    FillDirAndFileList(hwnd, szCurrentDir);
                    WinSetDlgItemText(hwnd, IT_DIRECTORY, szCurrentDir);
                }
                return 0;
            }

            if (usId == CBX_DRIVES && usNotify == CBN_LBSELECT) {
                /* User selected a different drive. */
                SHORT iSel = (SHORT)(LONG)WinSendDlgItemMsg(hwnd, CBX_DRIVES,
                                           LM_QUERYSELECTION,
                                           MPFROMSHORT(LIT_FIRST), 0);
                if (iSel != LIT_NONE) {
                    CHAR szDrv[4];
                    WinSendDlgItemMsg(hwnd, CBX_DRIVES, LM_QUERYITEMTEXT,
                                      MPFROM2SHORT(iSel, sizeof(szDrv)),
                                      MPFROMP(szDrv));
                    sprintf(szCurrentDir, "%c:\\", szDrv[0]);
                    FillDirAndFileList(hwnd, szCurrentDir);
                    UpdatePathInfo(hwnd, szDrv[0] - 'A');
                }
                return 0;
            }
            return 0;
        }

        case WM_COMMAND:
        {
            switch (SHORT1FROMMP(mp1))
            {
                case DID_OK:
                {
                    /* Return the selected filename to the caller. */
                    SHORT iSel = (SHORT)(LONG)WinSendDlgItemMsg(hwnd, LB_FILES,
                                               LM_QUERYSELECTION,
                                               MPFROMSHORT(LIT_FIRST), 0);
                    if (iSel == LIT_NONE) {
                        /* No file selected; do nothing. */
                        return 0;
                    }
                    {
                        CHAR szName[CCHMAXPATH];
                        CHAR szFull[CCHMAXPATH];
                        PSZ  pszResult = (PSZ)WinQueryWindowULong(hwnd, QWL_USER);

                        WinSendDlgItemMsg(hwnd, LB_FILES, LM_QUERYITEMTEXT,
                                          MPFROM2SHORT(iSel, sizeof(szName)),
                                          MPFROMP(szName));
                        snprintf(szFull, sizeof(szFull), "%s%s", szCurrentDir, szName);
                        if (pszResult)
                            strcpy(pszResult, szFull);
                    }
                    WinDismissDlg(hwnd, DID_OK);
                    return 0;
                }

                case DID_CANCEL:
                    WinDismissDlg(hwnd, DID_CANCEL);
                    return 0;
            }
            break;
        }
    }

    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

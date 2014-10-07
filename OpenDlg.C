#pragma	title("View Bitmap  --  Version 1  --  (OpenDlg.C)")
#pragma	subtitle("   File Open - Interface Definitions")

#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_DOSERRORS		   /* Include OS/2 DOS Errors		*/
#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#pragma	info(noext)

#include <malloc.h>
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "appdefs.h"
#include "viewbmp.h"

/* This	module contains	the routines that allow	the user to select a	*/
/* bitmap file to open and view.					*/

/* Filename:   OpenDlg.C						*/

/*  Version:   1							*/
/*  Created:   1995-07-05						*/
/*  Revised:   1995-07-22						*/

/* Routines:   MRESULT EXPENTRY	OpenBitmapDlgProc(HWND hWnd,		*/
/*						  ULONG	msg,		*/
/*						  MPARAM mp1,		*/
/*						  MPARAM mp2);		*/
/*	       static VOID ShowDirInfo(HWND hWnd, ULONG	idPath,		*/
/*				       ULONG idVol, ULONG idRoom);	*/
/*	       static VOID DiskVolDrive(ULONG ulDrive,			*/
/*					CHAR *pszDiskName);		*/
/*	       static VOID QueryDriveList( );				*/
/*	       static ULONG ulGetDirList(HWND hwndDrives, HWND hwndDirs,*/
/*					 HWND hwndFiles);		*/

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* DISCLAIMER OF WARRANTIES:						*/
/* -------------------------						*/
/* The following [enclosed] code is sample code	created	by IBM		*/
/* Corporation and Prominare Inc.  This	sample code is not part	of any	*/
/* standard IBM	product	and is provided	to you solely for the purpose	*/
/* of assisting	you in the development of your applications.  The code	*/
/* is provided "AS IS",	without	warranty of any	kind.  Neither IBM nor	*/
/* Prominare shall be liable for any damages arising out of your	*/
/* use of the sample code, even	if they	have been advised of the	*/
/* possibility of such damages.						*/
/************************************************************************/
/************************************************************************/
/************************************************************************/
/*		       D I S C L A I M E R				*/
/* This	code is	provided on an as is basis with	no implied support.	*/
/* It should be	considered freeware that cannot	be rebundled as		*/
/* part	of a larger "*ware" offering without our consent.		*/
/************************************************************************/
/************************************************************************/
/************************************************************************/

/* Copyright ¸ International Business Machines Corp., 1995.		*/
/* Copyright ¸ 1995  Prominare Inc.  All Rights	Reserved.		*/

/* --------------------------------------------------------------------	*/

/************************************************************************/
/*									*/
/*     Module Prototype	Definitions					*/
/*									*/
/************************************************************************/

static VOID  QueryDriveList(VOID);
static ULONG ulGetDirList(HWND hwndDrives, HWND	hwndDirs, HWND hwndFiles);
static VOID  DiskVolDrive(ULONG	ulDrive, CHAR *pszDiskName);
static VOID  ShowDirInfo(HWND hWnd, ULONG idPath, ULONG	idVol, ULONG idRoom);

/************************************************************************/
/*									*/
/*     Module Data Definitions						*/
/*									*/
/************************************************************************/

BOOL  fLineNumbers = FALSE;	   /* Line Numbers Flag			*/
INT   iFileSel = LIT_NONE;	   /* File Selected			*/
BOOL  fDrivesFilled;		   /* Drives Filled Flag		*/
ULONG ulDriveMap;		   /* Drive Bit	Map Holder		*/
LONG  cDrives;
CHAR  aszDrives[26][20];
BOOL  afRemoveable[26] = { FALSE, FALSE, FALSE,	FALSE, FALSE, FALSE, FALSE, FALSE,
			   FALSE, FALSE, FALSE,	FALSE, FALSE, FALSE, FALSE, FALSE,
			   FALSE, FALSE, FALSE,	FALSE, FALSE, FALSE, FALSE, FALSE,
			   FALSE, FALSE	};

#pragma	subtitle("   Open Bitmap -  Open Bitmap Dialogue Procedure")
#pragma	page( )

/* --- OpenBitmapDlgProc ------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	retrieve a bitmap file for viewing	*/
/*     print dialogue procedure.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Dialog Window Handle				*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     OpenBitmapDlgProc = Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY OpenBitmapDlgProc(HWND	hWnd, ULONG msg, MPARAM	mp1, MPARAM mp2)

{
CHAR	 szFileName[CCHMAXPATH];   /* Filename Holder			*/
CHAR	 szStrBuf[CCHMAXPATH];	   /* String Buffer			*/
register INT i;			   /* Loop Counters			*/

switch ( msg )
   {
		       /* Perform dialog initialization			*/
   case	WM_INITDLG :
       WinSetPointer(HWND_DESKTOP, hptrWait);

		       /* Clear	the drives filled flag			*/

       fDrivesFilled = FALSE;

		       /* Get a	list of	the drives, directories	and	*/
		       /* files	for the	list boxes and show the	number	*/
		       /* of files within the directory			*/

       sprintf(szStrBuf, "%d", ulGetDirList(WinWindowFromID(hWnd, CBX_DRIVES),
					    WinWindowFromID(hWnd, LB_DIRECTORIES),
					    WinWindowFromID(hWnd, LB_FILES)));
       WinSetDlgItemText(hWnd, IT_FILECOUNT, szStrBuf);

		       /* Get directory/file list and display in list	*/
		       /* boxes						*/

       ShowDirInfo(hWnd, IT_DIRECTORY, IT_VOLUMELABEL, IT_DISKFREE);

		       /* Select the last Open Bitmaped			 */

       if ( iFileSel !=	LIT_NONE )
	   WinSendDlgItemMsg(hWnd, LB_DIRECTORIES, LM_SELECTITEM, MPFROMSHORT(iFileSel),
			     MPFROMSHORT(TRUE));
       WinSetPointer(HWND_DESKTOP, hptrArrow);
       break;
			/* Process control selections			*/
   case	WM_CONTROL :
       switch (	SHORT2FROMMP(mp1) )
	   {
		       /* Enter	or selection made within error combo	*/
		       /* box						*/
	   case	CBN_ENTER :
	       switch (	SHORT1FROMMP(mp1) )
		   {
		   case	CBX_DRIVES :

		       /* Get item selected from the list box		*/

		       WinSendDlgItemMsg(hWnd, CBX_DRIVES, LM_QUERYITEMTEXT,
				  MPFROM2SHORT(LONGFROMMR(WinSendDlgItemMsg(hWnd, CBX_DRIVES,
									    LM_QUERYSELECTION,
									    0L,	0L)),
					       CCHMAXPATH),
				  MPFROMP(szFileName));

		       /* Change drives					*/

		       DosSetDefaultDisk((ULONG)((szFileName[0]	& 223) - '@'));

		       /* Refresh the directory	files list and show the	*/
		       /* number of files within the directory		*/

		       sprintf(szStrBuf, "%d", ulGetDirList(WinWindowFromID(hWnd, CBX_DRIVES),
				    WinWindowFromID(hWnd, LB_DIRECTORIES),
				    WinWindowFromID(hWnd, LB_FILES)));
		       WinSetDlgItemText(hWnd, IT_FILECOUNT, szStrBuf);

		       /* Display the directory	path, volume label and	*/
		       /* total	disk space free				*/

		       ShowDirInfo(hWnd, IT_DIRECTORY, IT_VOLUMELABEL, IT_DISKFREE);
		       break;

		   default :
		       break;
		   }
	       break;
		       /* Drive/directory selected, get	entry selected	*/
		       /* and change to	drive or directory selected	*/

	   case	LN_ENTER :
	       switch (	SHORT1FROMMP(mp1) )
		   {
		   case	LB_DIRECTORIES :

		       /* Get item selected from the directories list	*/
		       /* box						*/

		       WinSendDlgItemMsg(hWnd, LB_DIRECTORIES, LM_QUERYITEMTEXT,
				  MPFROM2SHORT(LONGFROMMR(WinSendDlgItemMsg(hWnd, LB_DIRECTORIES,
									    LM_QUERYSELECTION, 0L, 0L)),
					       sizeof(szFileName)),
				  MPFROMP(szFileName));

		       /* Change to the	selected directory		*/

		       DosSetCurrentDir(szFileName);

		       /* Refresh the directory	files list and show the	*/
		       /* number of files within the directory		*/

		       sprintf(szStrBuf, "%d", ulGetDirList(WinWindowFromID(hWnd, CBX_DRIVES),
				    WinWindowFromID(hWnd, LB_DIRECTORIES),
				    WinWindowFromID(hWnd, LB_FILES)));
		       WinSetDlgItemText(hWnd, IT_FILECOUNT, szStrBuf);

		       /* Display the directory	path, volume label and	*/
		       /* total	disk space free				*/

		       ShowDirInfo(hWnd, IT_DIRECTORY, IT_VOLUMELABEL, IT_DISKFREE);
		       break;

		   case	LB_FILES :

		       /* Item within the files	list box double	clicked	*/
		       /* on, user indicating that the file selected	*/
		       /* should be printed, therefore,	force the	*/
		       /* simulation of	the Print push button		*/

		       WinSendMsg(hWnd,	WM_COMMAND, MPFROMSHORT(DID_OK), 0L);
		       break;
		   }
	       break;
	   }
       break;
			/* Process push	button selections		*/
   case	WM_COMMAND :
	switch ( SHORT1FROMMP(mp1) )
	    {
		       /* Print	push button clicked			*/
	   case	DID_OK :
		       /* Get the file selected				*/

	       if ( (iFileSel =	(i = (INT)LONGFROMMR(WinSendDlgItemMsg(hWnd, LB_FILES,
								       LM_QUERYSELECTION,
								       0L, 0L)))) == LIT_NONE )
		   break;
	       else
		       /* Get item selected from the list box and	*/
		       /* determine which file selected			*/

		   WinSendDlgItemMsg(hWnd, LB_FILES, LM_QUERYITEMTEXT,
				     MPFROM2SHORT(i, CCHMAXPATH),
				     MPFROMP(szBitmapFile));

	       WinDismissDlg(hWnd, TRUE);
	       break;
		       /* Cancel push button selected			*/
	   case	DID_CANCEL :
	       WinDismissDlg(hWnd, FALSE);
	       break;
	    }
	break;
			/* Close requested, exit dialogue		*/
   case	WM_CLOSE :
	WinDismissDlg(hWnd, FALSE);
	break;
			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   Open Bitmap - Directory Information Display Function")
#pragma	page( )

/* --- ShowDirInfo ------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	display	information regarding the	*/
/*     path, volume label and room remaining.				*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND  hWnd;   = Dialog Dialogue Window Handle			*/
/*     ULONG idPath; = Path ID						*/
/*     ULONG idVol;  = Volume Label ID					*/
/*     ULONG idRoom; = Room Remaining ID				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID ShowDirInfo(HWND hWnd, ULONG idPath, ULONG idVol, ULONG idRoom)

{
CHAR	   szStrBuf[CCHMAXPATH];   /* String Buffer			*/
FSALLOCATE diskinfo;		   /* Disk Information Structure	*/
FSINFO	   fsinf;		   /* Disk Volume Information Structure	*/
ULONG	   cbBuf;		   /* Directory	Buffer Size Count	*/
ULONG	   ulCurDrive;		   /* Current Drive Designation		*/
ULONG	   ulDrives;		   /* Drive Map				*/

		       /* Form the current drive/path and display	*/
		       /* within the Path display field			*/

DosQueryCurrentDisk(&ulCurDrive, &ulDriveMap);

		       /* Get the current drive	and path being used and	*/
		       /* display with Path display field		*/

DosQueryCurrentDisk(&ulCurDrive, &ulDrives);
memcpy(&szStrBuf[1], ":\\", 3);
szStrBuf[0] = (CHAR)(ulCurDrive	+ '@');
cbBuf =	CCHMAXPATH - 3;
		       /* Get information on the current directory	*/

if ( !DosQueryCurrentDir(ulCurDrive, &szStrBuf[3], &cbBuf) )
   {
		       /* Show the full	path information		*/

   WinSetDlgItemText(hWnd, idPath, szStrBuf);

		       /* Get the volume information and display it	*/

   if (	!DosQueryFSInfo(0UL, FSIL_VOLSER, (PBYTE)(&fsinf), sizeof(FSINFO)) )
       if ( fsinf.vol.szVolLabel[0] )
	   WinSetDlgItemText(hWnd, idVol, fsinf.vol.szVolLabel);
       else
	   WinSetDlgItemText(hWnd, idVol, "");

		       /* Get the remaining space of the drive and	*/
		       /* display					*/

   if (	!DosQueryFSInfo(ulCurDrive, FSIL_ALLOC,	(PVOID)&diskinfo,
			sizeof(FSALLOCATE)) )
       WinSetDlgItemText(hWnd, idRoom,
			 strcat(_ltoa((LONG)(diskinfo.cbSector * diskinfo.cSectorUnit *	diskinfo.cUnitAvail),
				szStrBuf, 10), " Bytes"));
   }
}
#pragma	subtitle("   Open Bitmap - Get Disk Volume Label Function")
#pragma	page( )

/* --- DiskVolDrive -----------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	retrieve the volume label of the	*/
/*     default disk drive.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     ULONG ulDrive;	   = Drive Number				*/
/*     CHAR  *pszDiskName; = Holder for	Volume Label			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID DiskVolDrive(ULONG ulDrive,	CHAR *pszDiskName)

{
FSINFO fsinf;			  /* Disk Volume Information Structure */

		       /* Get the disk volume label			*/

if ( !DosQueryFSInfo(ulDrive, FSIL_VOLSER, (PBYTE)(&fsinf), sizeof(FSINFO)) )
   if (	fsinf.vol.szVolLabel[0]	)
       strcat(strcat(memcpy(pszDiskName, "[ ", 3), fsinf.vol.szVolLabel), " ]");
   else
       pszDiskName[0] =	(CHAR)0;
}
#pragma	subtitle("   Open Bitmap - Drive List Query Procedure")
#pragma	page( )

/* --- QueryDriveList ---------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	build a	list of	valid drives.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     Nothing								*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID QueryDriveList(VOID)

{
CHAR	    szDrive[4];		   /* Drive Designation	Holder		*/
PFSQBUFFER2 psqbf;		   /* File System Query	Buffer Pointer	*/
ULONG	    cbDrvi;		   /* Buffer Count			*/
ULONG	    ulError;		   /* OS/2 Error			*/
ULONG	    ulCurDrive;		   /* Current Drive Designation		*/
ULONG	    ulDriveMask	= 1;	   /* Drive Bit	Map Mask		*/
register INT i;			   /* Loop Counter			*/

		       /* Perform general initialization		*/

DosQueryCurrentDisk(&ulCurDrive, &ulDriveMap);

		       /* Allocate space for the file system query	*/
		       /* buffer					*/

psqbf =	(PFSQBUFFER2)malloc(1024UL);
memcpy(&szDrive[1], ":", 2);

for ( i	= cDrives = 0; i < 26; i++ )
   {
		       /* Determine if a disk drive present by masking	*/
		       /* out the drive	present	bit			*/

   if (	ulDriveMap & ulDriveMask )
       {
		       /* Save the drive designation for the drive	*/
		       /* that is present				*/

       aszDrives[cDrives][0] = szDrive[0] = (CHAR)(i + 'A');
       memcpy(&aszDrives[cDrives][1], ":\\ ", 4);

		       /* Start	with hard disk drives, check to	see if	*/
		       /* the drive is local or	remote.	 Remote	drives	*/
		       /* are generally	networked drives.  Prominare	*/
		       /* Builder denotes diskette, local and remote	*/
		       /* drives within	the Drives/Directories window.	*/
       if ( i >	1 )
	   {
		       /* Initialize the buffer	count before the file	*/
		       /* system is queried otherwise the call may fail	*/

	   cbDrvi = 1024UL;

	   if (	(ulError = DosQueryFSAttach(szDrive, 1UL, FSAIL_QUERYNAME,
					    (PFSQBUFFER2)psqbf,
					    &cbDrvi)) == ERROR_NOT_READY )
	       afRemoveable[cDrives++] = TRUE;
	   else
	       if ( !ulError )
		   if (	!memcmp(&psqbf->rgFSAData[1], "CDFS", 5) )
		       afRemoveable[cDrives++] = TRUE;
		   else
		       ++cDrives;
	   }
       else
	   afRemoveable[cDrives++] = TRUE;
       }
		       /* Shift	the drive designation bitmap mask	*/
   ulDriveMask <<= 1;
   }
		       /* Release the memory allocated for the query	*/
		       /* buffer					*/
free(psqbf);
}
#pragma	subtitle("   Open Bitmap - Directory Retrieval/Display Function")
#pragma	page( )

/* --- ulGetDirList -----------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	search for sub-directories and files	*/
/*     within the current sub-directory/drive and place	the entries	*/
/*     within two list boxes.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hwndDrives;	= Drives Combo Box Handle			*/
/*     HWND hwndDirs;	= Directories List Box Handle			*/
/*     HWND hwndFiles;	= Files	List Box Handle				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     ulGetDirList = Number of	Files in Files List Box			*/
/*									*/
/* --------------------------------------------------------------------	*/

static ULONG ulGetDirList(HWND hwndDrives, HWND	hwndDirs, HWND hwndFiles)

{
HDIR	      hDir = (HDIR)HDIR_CREATE;/* Directory Handle		*/
HPOINTER      hptr;		   /* Mouse Pointer Handle		*/
PFILEFINDBUF3 pfindbuf3;	   /* File Find	Buffer			*/
PFILEFINDBUF3 pfindbuf;		   /* File Find	Buffer			*/
ULONG	      cFiles = 0UL;	   /* Files Count			*/
ULONG	      ulCurDrive;	   /* Current Drive Designation		*/
ULONG	      ulDrives;		   /* Drive Bit	Map Holder		*/
ULONG	      ulErr;		   /* OS/2 Error Value			*/
ULONG	      ulFileCnt	= 71UL;	   /* File Counter Holder		*/
register INT i,	n;		   /* Loop Counters			*/

hptr = WinQueryPointer(HWND_DESKTOP);

		       /* Set the mouse	pointer	to hourglass shape	*/
		       /* while	directory search taking	place		*/

WinSetPointer(HWND_DESKTOP,
	      WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));

		       /* Allocate memory for a	large directory	search	*/
		       /* buffer					*/

pfindbuf3 = (PFILEFINDBUF3)malloc(71UL * sizeof(FILEFINDBUF3));

		       /* Get the handles of the list boxes and	disable	*/
		       /* updating of them while inserting the data	*/
		       /* into them and	delete the contents within them	*/

WinEnableWindowUpdate(hwndDirs,	 FALSE);
WinEnableWindowUpdate(hwndFiles, FALSE);
WinSendMsg(hwndDirs,  LM_DELETEALL, 0L,	0L);
WinSendMsg(hwndFiles, LM_DELETEALL, 0L,	0L);

		       /* Start	directory search			*/

if ( !(ulErr = DosFindFirst("*.*", &hDir, MUST_HAVE_DIRECTORY |	FILE_DIRECTORY,
			    (PVOID)pfindbuf3, 71UL * sizeof(FILEFINDBUF3),
			    &ulFileCnt,	FIL_STANDARD)) )
   {
   do
       for ( i = 0, pfindbuf = pfindbuf3; i < ulFileCnt; i++ )
	   {
		       /* If file found	a sub-directory, place into the	*/
		       /* Directories list box				*/

	   if (	(pfindbuf->attrFile & FILE_DIRECTORY) && memcmp(pfindbuf->achName, ".",	2) )
	       WinSendMsg(hwndDirs, LM_INSERTITEM, MPFROMSHORT(LIT_SORTASCENDING),
			  MPFROMP(pfindbuf->achName));
	   pfindbuf = (PFILEFINDBUF3)((PBYTE)pfindbuf +	pfindbuf->oNextEntryOffset);
	   }
       while ( (ulFileCnt == 71UL) &&
	       !DosFindNext(hDir, pfindbuf3, 71UL * sizeof(FILEFINDBUF3), &ulFileCnt) );

		       /* Close	directory search handle			*/
   DosFindClose(hDir);
   }
else
   if (	ulErr != ERROR_NO_MORE_FILES )
       {
		       /* Release the memory allocated for the search	*/
		       /* buffer					*/

       free((PVOID)pfindbuf3);
       return(0);
       }

if ( !fDrivesFilled )
   {
		       /* Form the current drive/path and display	*/
		       /* within the Path display field			*/

   DosQueryCurrentDisk(&ulCurDrive, &ulDrives);
   if (	ulDriveMap != ulDrives )
       QueryDriveList( );
   for ( i = 0;	i < cDrives; i++ )
       {
       if ( !afRemoveable[i] )
	   DiskVolDrive((ULONG)(i + 1),	&aszDrives[i][4]);
       else
	   aszDrives[i][4] = (CHAR)0;
       n = (INT)LONGFROMMR(WinSendMsg(hwndDrives, LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP(aszDrives[i])));
       if ( i == (ulCurDrive - 1) )
	   WinSendMsg(hwndDrives, LM_SELECTITEM, MPFROMLONG(n),	MPFROMLONG(TRUE));
       }
   fDrivesFilled = TRUE;
   }
		       /* Reset	directory handle and file counter	*/
hDir = (HDIR)HDIR_CREATE;
ulFileCnt = 71UL;
		       /* Start	file search				*/

if ( !(ulErr = DosFindFirst("*.bmp", &hDir, FILE_NORMAL,
			    (PVOID)pfindbuf3, 71UL * sizeof(FILEFINDBUF3),
			    &ulFileCnt,	FIL_STANDARD)) )
   {
   do
       for ( i = 0, pfindbuf = pfindbuf3; i < ulFileCnt; i++ )
	   {
		       /* If file found	not a sub-directory, place	*/
		       /* filename in list box				*/

	   if (	(pfindbuf->attrFile & FILE_DIRECTORY) != FILE_DIRECTORY	)
	       {
	       ++cFiles;
	       WinSendMsg(hwndFiles, LM_INSERTITEM, MPFROMSHORT(LIT_SORTASCENDING),
			  MPFROMP(pfindbuf->achName));
	       }
	   pfindbuf = (PFILEFINDBUF3)((PBYTE)pfindbuf +	pfindbuf->oNextEntryOffset);
	   }
		       /* Search for remaining entries and place valid	*/
		       /* entries in list box				*/

       while ( (ulFileCnt == 71UL) &&
		!DosFindNext(hDir, pfindbuf3, 71UL * sizeof(FILEFINDBUF3), &ulFileCnt) );

		       /* Close	directory search handle			*/
   DosFindClose(hDir);
   }
else
   if (	ulErr != ERROR_NO_MORE_FILES )
       {
		       /* Release the memory allocated for the search	*/
		       /* buffer					*/

       free((PVOID)pfindbuf3);
       return(0);
       }
		       /* Release the memory allocated for the search	*/
		       /* buffer					*/
free((PVOID)pfindbuf3);
		       /* Re-enable updating to	the list boxes and	*/
		       /* force	the displaying of the new contents	*/

WinShowWindow(hwndDirs,	TRUE);
WinShowWindow(hwndFiles, TRUE);

		       /* Reset	edit field text	in case	invalid	search	*/
		       /* spec entered and reset mouse pointer back to	*/
		       /* arrow	shape					*/

WinSetPointer(HWND_DESKTOP, hptr);
return(cFiles);
}

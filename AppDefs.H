
/* appdefs.h	    Created:	1995-07-05  Revised:  1995-07-15	*/

/* Module Definition Header						*/

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


#define	RGBCLR_PALEGRAY		0x00c9c9c9L

#define	RGBCLR_SHADOW		0x00808080L
#define	RGBCLR_DIALOGBACKGROUND	0x00ccccccL

typedef	struct _BITMAPSTACK
   {
   HBITMAP hbm;			   /* Bitmap Handle			*/
   RECTL   rclDest;		   /* Destination Rectangle		*/
   RECTL   rclImage;		   /* Image Rectangle			*/
   POINTL  aptlArea[8];		   /* Shadow Points			*/
   PBYTE   pb;			   /* Bitmap Pointer			*/
   LONG	   cColours;		   /* Colours Count			*/
   } BITMAPSTACK, *PBITMAPSTACK;

/************************************************************************/
/************************************************************************/
/*	Global Variable	Definitions					*/
/************************************************************************/
/************************************************************************/

extern FONTMETRICS fm;		   /* Font Metrics Info			*/
extern HSWITCH	   hSwitch;	   /* Task List	Entry Handle		*/
extern HAB	   hAB;		   /* Program Anchor Block Handle	*/

extern HMQ  hmqViewBmp;		   /* Program Message Queue Handle	*/
extern HWND hwndViewBmp;	   /* Client Window Handle		*/
extern HWND hwndViewBmpFrame;	   /* Frame Window Handle		*/
extern HWND hmenuViewBmp;	   /* Menu Handle			*/
extern HWND hmenuPopup;		   /* Popup Menu Handle			*/

extern HPOINTER	hptrArrow;	   /* Arrow Pointer Handle		*/
extern HPOINTER	hptrWait;	   /* Wait Pointer Handle		*/

extern LONG	cxScreen;	   /* Screen Width			*/

extern CHAR	szBitmapFile[CCHMAXPATH]; /* Bitmap Filename Holder	*/

extern BITMAPSTACK abm[32];	   /* Bitmap Stack			*/
extern LONG	cBitmaps;	   /* Bitmap Stack Count		*/

extern PBYTE	pb;		   /* Bitmap File Data Pointer		*/
extern BOOL	f20Bitmap;	   /* 2.x Bitmap Type			*/
extern BOOL	fWindowsBitmap;	   /* Windows Bitmap Type		*/
extern LONG	iBitmap;	   /* Single Bitmap Index		*/
extern LONG	iBitmapSelected;   /* Selected Bitmap Index		*/
extern BOOL	fBitmapArray;	   /* Bitmap Array Flag			*/

/************************************************************************/
/************************************************************************/
/*	Function Prototype Definitions					*/
/************************************************************************/
/************************************************************************/


/* --- BmpWnd.C	Prototype Definitions ---------------------------------	*/

MRESULT	EXPENTRY BitmapViewerWndProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2);

/* --- Effects.C Prototype Definitions --------------------------------	*/

HBITMAP	hbmFlipColourTable(LONG	iImage);
HBITMAP	hbmRefreshBitmap(LONG iImage);

/* --- GetBmp.C	Prototype Definitions ---------------------------------	*/

HBITMAP	hbmGetBitmap(PSZ pszFileName);

/* --- OpenDlg.C Prototype Definitions --------------------------------	*/

MRESULT	EXPENTRY OpenBitmapDlgProc(HWND	hWnd, ULONG msg, MPARAM	mp1, MPARAM mp2);

/* --- Support.C Prototype Definitions --------------------------------	*/

VOID	InitApp(HWND hwndFrame,	HWND hwndClient, PSZ pszWindowListTitle);
HWND	CreateStdWindow(HWND hwndParent, ULONG flStyle,	ULONG flCreateFlags,
			PSZ pszClientClass, PSZ	pszTitle, ULONG	styleClient,
			HMODULE	hmod, ULONG idResources, PHWND phwndClient,
			LONG x,	LONG y,	LONG cx, LONG cy);
MRESULT	PDSKeyProc(HWND	hWnd, ULONG msg, MPARAM	mp1, MPARAM mp2);
VOID	PDSGetTemplate(HWND hWnd, ULONG	id);

/* --- ViewDlg.C Prototype Definitions --------------------------------	*/

MRESULT	EXPENTRY ViewBITMAPARRAYFILEHEADER2DlgProc(HWND	hWnd, ULONG msg, MPARAM	mp1, MPARAM mp2);
MRESULT	EXPENTRY ViewBITMAPFILEHEADER2DlgProc(HWND hWnd, ULONG msg, MPARAM mp1,	MPARAM mp2);
MRESULT	EXPENTRY ViewBITMAPINFOHEADER2DlgProc(HWND hWnd, ULONG msg, MPARAM mp1,	MPARAM mp2);
MRESULT	EXPENTRY ViewBITMAPARRAYFILEHEADERDlgProc(HWND hWnd, ULONG msg,	MPARAM mp1, MPARAM mp2);
MRESULT	EXPENTRY ViewBITMAPFILEHEADERDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT	EXPENTRY ViewBITMAPINFOHEADERDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT	EXPENTRY ViewColourTableDlgProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT	EXPENTRY ViewSystemBitmapInfoDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2);

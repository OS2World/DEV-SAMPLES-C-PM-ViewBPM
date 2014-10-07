#pragma	title("View Bitmap  --  Version 1  --  (BmpWnd.C)")
#pragma	subtitle("   Main Client Window - Interface Definitions")

#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <malloc.h>
#include <os2.h>
#include <string.h>

#include "appdefs.h"
#include "viewbmp.h"

/* This	module contains	routine	used to	handle the main	application	*/
/* window.								*/

/* Filename:   BmpWnd.C							*/

/*  Version:   1							*/
/*  Created:   1995-07-05						*/
/*  Revised:   1995-07-22						*/

/* Routines:   static VOID SizeBitmap(HWND hWnd);			*/
/*	       static VOID SizeBitmapStack(HWND	hWnd);			*/
/*	       MRESULT EXPENTRY	BitmapViewerWndProc(HWND hWnd,		*/
/*						    ULONG msg,		*/
/*						    MPARAM mp1,		*/
/*						    MPARAM mp2);	*/


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

HBITMAP	hbmView	= 0;		   /* Bitmap Handle			*/
LONG	lScale = 100L;		   /* Scale Factor			*/
RECTL	rclDest;		   /* Destination Rectangle		*/
RECTL	rclImage;		   /* Image Rectangle			*/
POINTL	aptlArea[8];		   /* Shadow Points			*/

LONG	iImage;			   /* Bitmap Image Index		*/

static VOID SizeBitmap(HWND hWnd);
static VOID SizeBitmapStack(HWND hWnd);

#pragma	subtitle("   Client Window - Bitmap Sizing Function")
#pragma	page( )

/* --- SizeBitmap -------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	size the scaled	bitmap for the window.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hWnd; = Window Handle					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID SizeBitmap(HWND hWnd)

{
BITMAPINFOHEADER bmp;		   /* Bitmap Information Header		*/
RECTL		 rcl;		   /* Window Rectangle			*/
LONG		 cx;		   /* Bitmap Width			*/
LONG		 cy;		   /* Bitmap Height			*/

WinQueryWindowRect(hWnd, &rcl);

GpiQueryBitmapParameters(hbmView, &bmp);

cx = (bmp.cx * lScale) / 100L;
cy = (bmp.cy * lScale) / 100L;

rclDest.xRight = (rclDest.xLeft	  = ((rcl.xRight - rcl.xLeft) /	2L) - (cx / 2L)) + cx;
rclDest.yTop   = (rclDest.yBottom = ((rcl.yTop - rcl.yBottom) /	2L) - (cy / 2L)) + cy;

rclImage.xRight	= (rclImage.xLeft   = ((rcl.xRight - rcl.xLeft)	/ 2L) -	(bmp.cx	* 4L)) + bmp.cx	* 8L;
rclImage.yTop	= (rclImage.yBottom = ((rcl.yTop - rcl.yBottom)	/ 2L) -	(bmp.cy	* 4L)) + bmp.cy	* 8L;

rclImage.xLeft	 -= 3L;
rclImage.yBottom -= 3L;
rclImage.xRight	 += 3L;
rclImage.yTop	 += 3L;

		       /* Form the final boundary points		*/

aptlArea[0].x =	rclDest.xLeft  - 2L;
aptlArea[0].y =	rclDest.yTop   + 2L;
aptlArea[1].x =	rclDest.xRight + 2L;
aptlArea[1].y =	rclDest.yTop   + 2L;

aptlArea[2].x =	rclDest.xRight	+ 2L;
aptlArea[2].y =	rclDest.yBottom	- 2L;
aptlArea[3].x =	rclDest.xLeft	- 2L;
aptlArea[3].y =	rclDest.yBottom	- 2L;

aptlArea[4].x =	rclDest.xLeft  - 1L;
aptlArea[4].y =	rclDest.yTop   + 1L;
aptlArea[5].x =	rclDest.xRight + 1L;
aptlArea[5].y =	rclDest.yTop   + 1L;

aptlArea[6].x =	rclDest.xRight	+ 1L;
aptlArea[6].y =	rclDest.yBottom	- 1L;
aptlArea[7].x =	rclDest.xLeft	- 1L;
aptlArea[7].y =	rclDest.yBottom	- 1L;
}
#pragma	subtitle("   Client Window - Bitmap Stack Sizing Function")
#pragma	page( )

/* --- SizeBitmapStack --------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	calculate the size and position	of	*/
/*     the bitmaps from	the bitmap array for the window.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hWnd; = Window Handle					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID SizeBitmapStack(HWND hWnd)

{
BITMAPINFOHEADER bmp;		   /* Bitmap Information Header		*/
RECTL		 rcl;		   /* Window Rectangle			*/
LONG		 cx;		   /* Bitmap Width			*/
LONG		 cy;		   /* Bitmap Height			*/
LONG		 xImage;	   /* Image Starting Point		*/
register INT i;			   /* Loop Counter			*/


WinQueryWindowRect(hWnd, &rcl);
	
for ( i	= 0, cx	= 10 * (cBitmaps - 1), cy = 0L;	i < cBitmaps; i++ )
   {
   GpiQueryBitmapParameters(abm[i].hbm,	&bmp);

   cx += bmp.cx;
   if (	bmp.cy > cy )
       cy = bmp.cy;
   }

rclImage.xRight	= (rclImage.xLeft   = ((rcl.xRight - rcl.xLeft)	/ 2L) -	(cx / 2L)) + cx;
rclImage.yTop	= (rclImage.yBottom = ((rcl.yTop - rcl.yBottom)	/ 2L) -	(cy / 2L)) + cy;

xImage = rclImage.xLeft;

rclImage.xLeft	 -= 3L;
rclImage.yBottom -= 3L;
rclImage.xRight	 += 3L;
rclImage.yTop	 += 3L;

for ( i	= 0; i < cBitmaps; i++ )
   {
   GpiQueryBitmapParameters(abm[i].hbm,	&bmp);

   abm[i].rclDest.xRight = (abm[i].rclDest.xLeft   = xImage) + bmp.cx;
   abm[i].rclDest.yTop	 = (abm[i].rclDest.yBottom = ((rcl.yTop	- rcl.yBottom) / 2L) - (bmp.cy / 2L)) +	bmp.cy;

		       /* Form the final boundary points		*/

   abm[i].aptlArea[0].x	= abm[i].rclDest.xLeft	- 2L;
   abm[i].aptlArea[0].y	= abm[i].rclDest.yTop	+ 2L;
   abm[i].aptlArea[1].x	= abm[i].rclDest.xRight	+ 2L;
   abm[i].aptlArea[1].y	= abm[i].rclDest.yTop	+ 2L;

   abm[i].aptlArea[2].x	= abm[i].rclDest.xRight	 + 2L;
   abm[i].aptlArea[2].y	= abm[i].rclDest.yBottom - 2L;
   abm[i].aptlArea[3].x	= abm[i].rclDest.xLeft	 - 2L;
   abm[i].aptlArea[3].y	= abm[i].rclDest.yBottom - 2L;

   abm[i].aptlArea[4].x	= abm[i].rclDest.xLeft	- 1L;
   abm[i].aptlArea[4].y	= abm[i].rclDest.yTop	+ 1L;
   abm[i].aptlArea[5].x	= abm[i].rclDest.xRight	+ 1L;
   abm[i].aptlArea[5].y	= abm[i].rclDest.yTop	+ 1L;

   abm[i].aptlArea[6].x	= abm[i].rclDest.xRight	 + 1L;
   abm[i].aptlArea[6].y	= abm[i].rclDest.yBottom - 1L;
   abm[i].aptlArea[7].x	= abm[i].rclDest.xLeft	 - 1L;
   abm[i].aptlArea[7].y	= abm[i].rclDest.yBottom - 1L;

   xImage += (bmp.cx + 10L);
   }
}
#pragma	subtitle("   Client Window - Bitmap Viewer Window Procedure")
#pragma	page( )

/* --- BitmapViewerWndProc ----------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages sent to the	*/
/*     applications client window.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Window Handle					*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     BitmapViewerWndProc = Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY BitmapViewerWndProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2)

{
CHAR			szTitle[300];  /* Title	Bar Text		*/
HPS			hPS;	   /* Presentation Space Handle		*/
LONG			lClrBack;  /* Colour Holder			*/
LONG			lClrFore;  /* Colour Holder			*/
PBITMAPARRAYFILEHEADER	pbafh;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPARRAYFILEHEADER2	pbafh2;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPFILEHEADER	pbfh;	   /* Bitmap File Header Pointer	*/
PBITMAPFILEHEADER2	pbfh2;	   /* Bitmap File Header Pointer	*/
PBITMAPINFO		pbmi;	   /* Bitmap Info Pointer		*/
PBITMAPINFO2		pbmi2;	   /* Bitmap Info Pointer		*/
PBYTE			pbData;	   /* Byte Pointer			*/
POINTL			ptl;	   /* Pointer Position			*/
RECTL			rcl;	   /* Window Rectangle			*/
register INT i,	n;		   /* Index				*/

switch ( msg )
   {
/************************************************************************/
/* Window being	created, perform window	initialization			*/
/************************************************************************/

   case	WM_CREATE :

       hptrWait	 = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT,	FALSE);
       hptrArrow = WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW,	FALSE);

       cxScreen	= WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);

       hmenuViewBmp = WinWindowFromID(WinQueryWindow(hWnd, QW_PARENT), FID_MENU);

		       /************************************************/
		       /* PDSGetTemplate is used to allow controls in  */
		       /* windows.  Do not remove this function	if you */
		       /* intend to include controls within the	window.*/
		       /************************************************/

       PDSGetTemplate(hWnd, WIN_BITMAPVIEWER);
       break;

/************************************************************************/
/* Window being	sized							*/
/************************************************************************/

   case	WM_SIZE	:
       if ( lScale )
	   {
	   if (	hbmView	)
	       SizeBitmap(hWnd);
	   }
       else
	   if (	cBitmaps )
	       SizeBitmapStack(hWnd);
       break;

/************************************************************************/
/* Perform menu	initialization						*/
/************************************************************************/

   case	WM_INITMENU :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	IDM_FILE :
	       break;

	   case	IDM_SCALE :
	       switch (	lScale )
		   {
		   case	25 :
		       i = IDM_25PERCENT;
		       break;
		   case	50 :
		       i = IDM_50PERCENT;
		       break;
		   case	75 :
		       i = IDM_75PERCENT;
		       break;
		   case	100 :
		       i = IDM_100PERCENT;
		       break;
		   case	200 :
		       i = IDM_200PERCENT;
		       break;
		   case	400 :
		       i = IDM_400PERCENT;
		       break;
		   case	800 :
		       i = IDM_800PERCENT;
		       break;
		   default :
		       i = IDM_SHOWALL;
		       break;
		   }

	       for ( n = IDM_25PERCENT;	n <= IDM_SHOWALL; n++ )
		   WinSendMsg(hmenuViewBmp, MM_SETITEMATTR, MPFROM2SHORT(n, TRUE),
			      MPFROM2SHORT(MIA_CHECKED,	(i == n) ? MIA_CHECKED : 0));
	       break;

	   case	IDM_EFFECTS :
	       WinSendMsg(hmenuViewBmp,	MM_SETITEMATTR,	MPFROM2SHORT(IDM_FLIPCOLOURTABLE, TRUE),
			  MPFROM2SHORT(MIA_DISABLED, cBitmaps && lScale	? 0 : MIA_DISABLED));
	       break;
	   }
       break;

/************************************************************************/
/* Process key press from keyboard					*/
/************************************************************************/

   case	WM_CHAR	:
		       /************************************************/
		       /* PDSKeyProc is	used to	allow controls in      */
		       /* windows.  Do not remove this function	if you */
		       /* intend to include controls within the	window.*/
		       /************************************************/

       return(PDSKeyProc(hWnd, msg, mp1, mp2));

   case	WM_BUTTON2CLICK	:

/************************************************************************/
/* Button 2 clicked							*/
/************************************************************************/

       if ( !lScale && cBitmaps	)
	   {
	   ptl.x = (LONG)(SHORT)SHORT1FROMMP(mp1);
	   ptl.y = (LONG)(SHORT)SHORT2FROMMP(mp1);
	   for ( i = 0;	i < cBitmaps; i++ )
	       if ( WinPtInRect(hAB, &abm[i].rclDest, &ptl) )
		   {
		   iImage = i;
		   if (	hmenuPopup )
		       WinDestroyWindow(hmenuPopup);
		   hmenuPopup =	WinLoadMenu(hWnd, (HMODULE)NULL, (cBitmaps > 1)	? IDM_POPUP : IDM_POPUPSINGLE);
		   WinPopupMenu(hWnd, hWnd, hmenuPopup,	SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), IDM_BITMAPARRAYFILEHEADER,
				PU_KEYBOARD | PU_MOUSEBUTTON1 |	PU_VCONSTRAIN |	PU_HCONSTRAIN);
		   }
	   }
       else
	   if (	hbmView	)
	       {
	       ptl.x = (LONG)(SHORT)SHORT1FROMMP(mp1);
	       ptl.y = (LONG)(SHORT)SHORT2FROMMP(mp1);
	       if ( WinPtInRect(hAB, &rclDest, &ptl) )
		   {
		   iImage = iBitmap;
		   if (	hmenuPopup )
		       WinDestroyWindow(hmenuPopup);
		   hmenuPopup =	WinLoadMenu(hWnd, (HMODULE)NULL, (cBitmaps > 1)	? IDM_POPUP : IDM_POPUPSINGLE);
		   WinPopupMenu(hWnd, hWnd, hmenuPopup,	SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), IDM_BITMAPARRAYFILEHEADER,
				PU_KEYBOARD | PU_MOUSEBUTTON1 |	PU_VCONSTRAIN |	PU_HCONSTRAIN);
		   }
	       }

       break;

/************************************************************************/
/* Process control selections						*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	IDM_OPEN :
	       if ( WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)OpenBitmapDlgProc,
			      (HMODULE)NULL, DLG_OPENBITMAP, NULL) )
		   {
		   hbmView = hbmGetBitmap(szBitmapFile);
		   SizeBitmap(hWnd);
		   WinInvalidateRect(hWnd, NULL, FALSE);
		   strcat(memcpy(szTitle, "Bitmap Viewer - ", 17), szBitmapFile);
		   if (	fWindowsBitmap )
		       strcat(szTitle, " [Windows 3.x format]");
		   else
		       if ( f20Bitmap )
			   strcat(szTitle, " [OS/2 2.x format]");
		       else
			   strcat(szTitle, " [OS/2 1.x format]");
		   WinSetWindowText(hwndViewBmpFrame, szTitle);
		   }
	       break;

	   case	IDM_VIEWSYSTEMINFO :
	       WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)ViewSystemBitmapInfoDlgProc,
			 (HMODULE)NULL,	DLG_VIEWBITMAPSYSINO, NULL);
	       break;

	   case	IDM_25PERCENT :
	       lScale =	25L;
	       SizeBitmap(hWnd);
	       WinInvalidateRect(hWnd, &rclImage, FALSE);
	       break;

	   case	IDM_50PERCENT :
	       lScale =	50L;
	       SizeBitmap(hWnd);
	       WinInvalidateRect(hWnd, &rclImage, FALSE);
	       break;

	   case	IDM_75PERCENT :
	       lScale =	75L;
	       SizeBitmap(hWnd);
	       WinInvalidateRect(hWnd, &rclImage, FALSE);
	       break;

	   case	IDM_100PERCENT :
	       lScale =	100L;
	       SizeBitmap(hWnd);
	       WinInvalidateRect(hWnd, &rclImage, FALSE);
	       break;

	   case	IDM_200PERCENT :
	       lScale =	200L;
	       SizeBitmap(hWnd);
	       WinInvalidateRect(hWnd, &rclImage, FALSE);
	       break;

	   case	IDM_400PERCENT :
	       lScale =	400L;
	       SizeBitmap(hWnd);
	       WinInvalidateRect(hWnd, &rclImage, FALSE);
	       break;

	   case	IDM_800PERCENT :
	       lScale =	800L;
	       SizeBitmap(hWnd);
	       WinInvalidateRect(hWnd, &rclImage, FALSE);
	       break;

	   case	IDM_SHOWALL :
	       lScale =	0L;
	       SizeBitmapStack(hWnd);
	       WinInvalidateRect(hWnd, NULL, FALSE);
	       break;

	   case	IDM_BITMAPARRAYFILEHEADER :
	       if ( f20Bitmap )
		   WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)ViewBITMAPARRAYFILEHEADER2DlgProc,
			     (HMODULE)NULL, DLG_VIEWBITMAPARRAYFILEHEADER2, (PVOID)iImage);
	       else
		   WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)ViewBITMAPARRAYFILEHEADERDlgProc,
			     (HMODULE)NULL, DLG_VIEWBITMAPARRAYFILEHEADER, (PVOID)iImage);
	       break;

	   case	IDM_BITMAPFILEHEADER :
	       if ( f20Bitmap )
		   {
		   if (	(cBitmaps == 1)	&& !fBitmapArray )
		       pbData =	abm[iBitmapSelected = iImage].pb;
		   else
		       {
		       pbafh2 =	(PBITMAPARRAYFILEHEADER2)abm[iBitmapSelected = iImage].pb;
		       pbData =	(PBYTE)&pbafh2->bfh2;
		       }
		   WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)ViewBITMAPFILEHEADER2DlgProc,
			     (HMODULE)NULL, DLG_VIEWBITMAPFILEHEADER2, (PVOID)pbData);
		   }
	       else
		   {
		   if (	(cBitmaps == 1)	&& !fBitmapArray )
		       pbData =	abm[iBitmapSelected = iImage].pb;
		   else
		       {
		       pbafh = (PBITMAPARRAYFILEHEADER)abm[iBitmapSelected = iImage].pb;
		       pbData =	(PBYTE)&pbafh->bfh;
		       }
		   WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)ViewBITMAPFILEHEADERDlgProc,
			     (HMODULE)NULL, DLG_VIEWBITMAPFILEHEADER, (PVOID)pbData);
		   }
	       break;

	   case	IDM_BITMAPINFOHEADER :
	       if ( f20Bitmap )
		   {
		   if (	(cBitmaps == 1)	&& !fBitmapArray )
		       {
		       pbfh2 = (PBITMAPFILEHEADER2)abm[iBitmapSelected = iImage].pb;
		       pbData =	(PBYTE)&pbfh2->bmp2;
		       }
		   else
		       {
		       pbafh2 =	(PBITMAPARRAYFILEHEADER2)abm[iBitmapSelected = iImage].pb;
		       pbData =	(PBYTE)&pbafh2->bfh2.bmp2;
		       }
		   WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)ViewBITMAPINFOHEADER2DlgProc,
			     (HMODULE)NULL, DLG_VIEWBITMAPINFOHEADER2, (PVOID)pbData);
		   }
	       else
		   {
		   if (	(cBitmaps == 1)	&& !fBitmapArray )
		       {
		       pbfh = (PBITMAPFILEHEADER)abm[iBitmapSelected = iImage].pb;
		       pbData =	(PBYTE)&pbfh->bmp;
		       }
		   else
		       {
		       pbafh = (PBITMAPARRAYFILEHEADER)abm[iBitmapSelected = iImage].pb;
		       pbData =	(PBYTE)&pbafh->bfh.bmp;
		       }
		   WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)ViewBITMAPINFOHEADERDlgProc,
			     (HMODULE)NULL, DLG_VIEWBITMAPINFOHEADER, (PVOID)pbData);
		   }
	       break;

	   case	IDM_COLOURTABLE	:
	       if ( f20Bitmap )
		   {
		   if (	(cBitmaps == 1)	&& !fBitmapArray )
		       {
		       pbfh2 = (PBITMAPFILEHEADER2)abm[iBitmapSelected = iImage].pb;
		       pbmi2 = (PBITMAPINFO2)&pbfh2->bmp2;
		       }
		   else
		       {
		       pbafh2 =	(PBITMAPARRAYFILEHEADER2)abm[iBitmapSelected = iImage].pb;
		       pbmi2 = (PBITMAPINFO2)&pbafh2->bfh2.bmp2;
		       }
		   pbData = (PBYTE)&pbmi2->argbColor;
		   }
	       else
		   {
		   if (	(cBitmaps == 1)	&& !fBitmapArray )
		       {
		       pbfh = (PBITMAPFILEHEADER)abm[iBitmapSelected = iImage].pb;
		       pbmi = (PBITMAPINFO)&pbfh->bmp;
		       }
		   else
		       {
		       pbafh = (PBITMAPARRAYFILEHEADER)abm[iBitmapSelected = iImage].pb;
		       pbmi = (PBITMAPINFO)&pbafh->bfh.bmp;
		       }
		   pbData = (PBYTE)&pbmi->argbColor;
		   }
	       if ( WinDlgBox(HWND_DESKTOP, hwndViewBmpFrame, (PFNWP)ViewColourTableDlgProc,
			      (HMODULE)NULL, abm[iImage].cColours == 2L	? DLG_VIEW2COLOURTABLE : DLG_VIEWCOLOURTABLE, (PVOID)pbData) )
		   {
		   hbmView = hbmRefreshBitmap(iImage);
		   WinInvalidateRect(hWnd, &rclImage, FALSE);
		   }
	       break;

	   case	IDM_FLIPCOLOURTABLE :
	       hbmView = hbmFlipColourTable(iImage);
	       WinInvalidateRect(hWnd, &rclImage, FALSE);
	       break;
	   }
       break;

/************************************************************************/
/* Erase window	background						*/
/************************************************************************/

   case	WM_ERASEBACKGROUND :
       WinQueryWindowRect(hWnd,	&rcl);
       WinFillRect((HPS)LONGFROMMP(mp1), &rcl, CLR_PALEGRAY);
       break;

/************************************************************************/
/* Paint client	window							*/
/************************************************************************/

   case	WM_PAINT :
       GpiCreateLogColorTable(hPS = WinBeginPaint(hWnd,	(HPS)NULL, &rcl), 0UL, LCOLF_RGB, 0L, 0L, (PLONG)NULL);
       WinFillRect(hPS,	&rcl, RGBCLR_PALEGRAY);

		       /* Check	to see if image	being scaled otherwise	*/
		       /* are showing complete bitmap array		*/
       if ( lScale )
	   {
		       /* Check	to see if the image is a 2-colour which	*/
		       /* menas	that the colours have to be set		*/
		       /* explicitely					*/

	   if (	abm[iImage].cColours ==	2L )
	       {
	       lClrBack	= lClrFore = 0L;

		       /* Check	to see if a 2.x	format bitmap since the	*/
		       /* the structures are slightly different		*/

	       if ( f20Bitmap )
		   {
		       /* Locate the bitmap information	structure	*/

		   if (	(cBitmaps == 1)	&& !fBitmapArray )
		       {
		       pbfh2 = (PBITMAPFILEHEADER2)abm[iImage].pb;
		       pbmi2 = (PBITMAPINFO2)&pbfh2->bmp2;
		       }
		   else
		       {
		       pbafh2 =	(PBITMAPARRAYFILEHEADER2)abm[iImage].pb;
		       pbmi2 = (PBITMAPINFO2)&pbafh2->bfh2.bmp2;
		       }
			   /* Transfer the RGB info to the colour	*/
			   /* holders					*/

		   memcpy(&lClrBack, &pbmi2->argbColor[0], 3);
		   memcpy(&lClrFore, &pbmi2->argbColor[1], 3);
		   }
	       else
		   {
		       /* Locate the bitmap information	structure	*/

		   if (	(cBitmaps == 1)	&& !fBitmapArray )
		       {
		       pbfh = (PBITMAPFILEHEADER)abm[iImage].pb;
		       pbmi = (PBITMAPINFO)&pbfh->bmp;
		       }
		   else
		       {
		       pbafh = (PBITMAPARRAYFILEHEADER)abm[iImage].pb;
		       pbmi = (PBITMAPINFO)&pbafh->bfh.bmp;
		       }
			   /* Transfer the RGB info to the colour	*/
			   /* holders					*/

		   memcpy(&lClrBack, &pbmi->argbColor[0], 3);
		   memcpy(&lClrFore, &pbmi->argbColor[1], 3);
		   }
			   /* Draw the 2-colour	bitmap using the	*/
			   /* provided colours from the	bitmap		*/

	       WinDrawBitmap(hPS, hbmView, (PRECTL)NULL, (PPOINTL)(PVOID)&rclDest,
			     lClrFore, lClrBack, DBM_NORMAL | DBM_STRETCH);
	       }
	   else
			   /* Normal bitmap, draw stretched		*/
	       WinDrawBitmap(hPS, hbmView, (PRECTL)NULL, (PPOINTL)(PVOID)&rclDest,
			     0L, 0L, DBM_NORMAL	| DBM_STRETCH);

			   /* Draw the 3-D frame around	the image	*/

	   GpiSetColor(hPS, RGBCLR_SHADOW);
	   GpiMove(hPS,	&aptlArea[7]);
	   GpiPolyLine(hPS, 2L,	&aptlArea[4]);

	   GpiSetColor(hPS, RGB_WHITE);
	   GpiPolyLine(hPS, 2L,	&aptlArea[6]);

	   GpiSetColor(hPS, RGB_BLACK);
	   GpiMove(hPS,	&aptlArea[3]);
	   GpiPolyLine(hPS, 2L,	&aptlArea[0]);

	   GpiSetColor(hPS, RGBCLR_PALEGRAY);
	   GpiPolyLine(hPS, 2L,	&aptlArea[2]);
	   }
       else
		       /* Display the bitmap array images side-by-side	*/

	   for ( i = 0;	i < cBitmaps; i++ )
	       {
		       /* Check	to see if the image is a 2-colour which	*/
		       /* menas	that the colours have to be set		*/
		       /* explicitely					*/

	       if ( abm[i].cColours == 2L )
		   {
		   lClrBack = lClrFore = 0L;

		       /* Check	to see if a 2.x	format bitmap since the	*/
		       /* the structures are slightly different		*/

		   if (	f20Bitmap )
		       {
		       /* Locate the bitmap information	structure	*/

		       if ( (cBitmaps == 1) && !fBitmapArray )
			   {
			   pbfh2 = (PBITMAPFILEHEADER2)abm[i].pb;
			   pbmi2 = (PBITMAPINFO2)&pbfh2->bmp2;
			   }
		       else
			   {
			   pbafh2 = (PBITMAPARRAYFILEHEADER2)abm[i].pb;
			   pbmi2 = (PBITMAPINFO2)&pbafh2->bfh2.bmp2;
			   }
		       /* Transfer the RGB info	to the colour holders	*/

		       memcpy(&lClrBack, &pbmi2->argbColor[0], 3);
		       memcpy(&lClrFore, &pbmi2->argbColor[1], 3);
		       }
		   else
		       {
		       /* Locate the bitmap information	structure	*/

		       if ( (cBitmaps == 1) && !fBitmapArray )
			   {
			   pbfh	= (PBITMAPFILEHEADER)abm[i].pb;
			   pbmi	= (PBITMAPINFO)&pbfh->bmp;
			   }
		       else
			   {
			   pbafh = (PBITMAPARRAYFILEHEADER)abm[i].pb;
			   pbmi	= (PBITMAPINFO)&pbafh->bfh.bmp;
			   }
		       /* Transfer the RGB info	to the colour holders	*/

		       memcpy(&lClrBack, &pbmi->argbColor[0], 3);
		       memcpy(&lClrFore, &pbmi->argbColor[1], 3);
		       }
		       /* Draw the 2-colour bitmap using the		*/
		       /* provided colours from	the bitmap		*/

		   WinDrawBitmap(hPS, abm[i].hbm, (PRECTL)NULL,	(PPOINTL)(PVOID)&abm[i].rclDest,
				 lClrFore, lClrBack, DBM_NORMAL);
		   }
	       else
		       /* Draw the bitmap from the array		*/

		   WinDrawBitmap(hPS, abm[i].hbm, (PRECTL)NULL,	(PPOINTL)(PVOID)&abm[i].rclDest,
				 RGB_WHITE, RGB_BLACK, DBM_NORMAL);

		       /* Draw the 3-D frame around the	image		*/

	       GpiSetColor(hPS,	RGBCLR_SHADOW);
	       GpiMove(hPS, &abm[i].aptlArea[7]);
	       GpiPolyLine(hPS,	2L, &abm[i].aptlArea[4]);

	       GpiSetColor(hPS,	RGB_WHITE);
	       GpiPolyLine(hPS,	2L, &abm[i].aptlArea[6]);

	       GpiSetColor(hPS,	RGB_BLACK);
	       GpiMove(hPS, &abm[i].aptlArea[3]);
	       GpiPolyLine(hPS,	2L, &abm[i].aptlArea[0]);

	       GpiSetColor(hPS,	RGBCLR_PALEGRAY);
	       GpiPolyLine(hPS,	2L, &abm[i].aptlArea[2]);
	       }
       WinEndPaint(hPS);
       break;

/************************************************************************/
/* Window being	destroyed, perform clean-up operations			*/
/************************************************************************/

   case	WM_DESTROY :
       for ( i = 0; i <	cBitmaps; i++ )
	   GpiDeleteBitmap(abm[i].hbm);

       if ( hmenuPopup )
	   WinDestroyWindow(hmenuPopup);

       if ( pb )
	   free(pb);
       break;

		       /* Default message processing			*/
   default :
       return(WinDefWindowProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}

#pragma	title("View Bitmap  --  Version 1  --  (ViewDlg.C)")
#pragma	subtitle("   View Dialogues - Interface Definitions")

#define	INCL_DEV		   /* Include OS/2 Device Interface	*/
#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <malloc.h>
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "appdefs.h"
#include "viewbmp.h"

/* This	module contains	routines used to display the bitmap information	*/
/* contained within the	bitmap structures.				*/

/* Filename:   ViewDlg.C						*/

/*  Version:   1							*/
/*  Created:   1995-07-08						*/
/*  Revised:   1995-10-29						*/

/* Routines:   MRESULT EXPENTRY	ViewBITMAPARRAYFILEHEADER2DlgProc(	*/
/*		       HWND hWnd, ULONG	msg, MPARAM mp1, MPARAM	mp2);	*/
/*	       MRESULT EXPENTRY	ViewBITMAPFILEHEADER2DlgProc(		*/
/*		       HWND hWnd, ULONG	msg, MPARAM mp1, MPARAM	mp2);	*/
/*	       MRESULT EXPENTRY	ViewBITMAPINFOHEADER2DlgProc(		*/
/*		       HWND hWnd, ULONG	msg, MPARAM mp1, MPARAM	mp2);	*/
/*	       MRESULT EXPENTRY	ViewBITMAPARRAYFILEHEADER2DlgProc(	*/
/*		       HWND hWnd, ULONG	msg, MPARAM mp1, MPARAM	mp2);	*/
/*	       MRESULT EXPENTRY	ViewBITMAPFILEHEADER2DlgProc(		*/
/*		       HWND hWnd, ULONG	msg, MPARAM mp1, MPARAM	mp2);	*/
/*	       MRESULT EXPENTRY	ViewBITMAPINFOHEADER2DlgProc(		*/
/*		       HWND hWnd, ULONG	msg, MPARAM mp1, MPARAM	mp2);	*/
/*	       static VOID SetTableValues(HWND hWnd);			*/
/*	       static VOID QueryTableValues(HWND hWnd);			*/
/*	       MRESULT EXPENTRY	ViewColourTableDlgProc(			*/
/*		       HWND hWnd, ULONG	msg, MPARAM mp1, MPARAM	mp2);	*/
/*	       MRESULT EXPENTRY	ViewSystemBitmapInfoDlgProc(		*/
/*		       HWND hWnd, ULONG	msg, MPARAM mp1, MPARAM	mp2);	*/


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

RGB   *prgb;			   /* RGB Pointer			*/
RGB2  *prgb2;			   /* RGB Pointer			*/
PBYTE pbTable;			   /* RGB Table	Pointer			*/
LONG  lVScrollPos;		   /* Vertical Scroll Bar Position	*/

#pragma	subtitle("   Module Purpose - Dialog Procedure")
#pragma	page( )

/* --- ViewBITMAPARRAYFILEHEADER2DlgProc --------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
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
/*     ViewBITMAPARRAYFILEHEADER2DlgProc = Message Handling Result	*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewBITMAPARRAYFILEHEADER2DlgProc(HWND	hWnd, ULONG msg, MPARAM	mp1, MPARAM mp2)

{
CHAR			szNumber[32];  /* Numeric Buffer		*/
PBITMAPARRAYFILEHEADER2	pbafh;	   /* Bitmap Array File	Header Pointer	*/
PSZ			pszType;   /* Type String Pointer		*/
register INT i;			   /* Index				*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       pbafh = (PBITMAPARRAYFILEHEADER2)abm[i =	LONGFROMMP(mp2)].pb;
       switch (	pbafh->usType )
	   {
	   case	BFT_ICON :
	       pszType = "BFT_ICON";
	       break;

	  case BFT_BMAP	:
	       pszType = "BFT_BMAP";
	       break;

	   case	BFT_POINTER :
	       pszType = "BFT_POINTER";
	       break;

	   case	BFT_COLORICON :
	       pszType = "BFT_COLORICON";
	       break;

	   case	BFT_COLORPOINTER :
	       pszType = "BFT_COLORPOINTER";
	       break;

	   case	BFT_BITMAPARRAY	:
	       pszType = "BFT_BITMAPARRAY";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_BITMAPINDEX, _ltoa((LONG)i, szNumber,	10));
       WinSetDlgItemText(hWnd, IT_USTYPE,    pszType);
       WinSetDlgItemText(hWnd, IT_CBSIZE,    _ltoa((LONG)pbafh->cbSize,	szNumber, 10));
       WinSetDlgItemText(hWnd, IT_OFFNEXT,   _ltoa((LONG)pbafh->offNext, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CXDISPLAY, _ltoa((LONG)pbafh->cxDisplay, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CYDISPLAY, _ltoa((LONG)pbafh->cyDisplay, szNumber, 10));
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       WinDismissDlg(hWnd, TRUE);
	       break;
	   }
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   Module Purpose - Dialog Procedure")
#pragma	page( )

/* --- ViewBITMAPFILEHEADER2DlgProc -------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
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
/*     ViewBITMAPFILEHEADER2DlgProc = Message Handling Result		*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewBITMAPFILEHEADER2DlgProc(HWND hWnd, ULONG msg, MPARAM mp1,	MPARAM mp2)

{
CHAR		   szNumber[32];   /* Numeric Buffer			*/
PBITMAPFILEHEADER2 pbfh2;	   /* Bitmap Array File	Header Pointer	*/
PSZ		   pszType;	   /* Type String Pointer		*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       pbfh2 = (PBITMAPFILEHEADER2)PVOIDFROMMP(mp2);
       switch (	pbfh2->usType )
	   {
	   case	BFT_ICON :
	       pszType = "BFT_ICON";
	       break;

	  case BFT_BMAP	:
	       pszType = "BFT_BMAP";
	       break;

	   case	BFT_POINTER :
	       pszType = "BFT_POINTER";
	       break;

	   case	BFT_COLORICON :
	       pszType = "BFT_COLORICON";
	       break;

	   case	BFT_COLORPOINTER :
	       pszType = "BFT_COLORPOINTER";
	       break;

	   case	BFT_BITMAPARRAY	:
	       pszType = "BFT_BITMAPARRAY";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_BITMAPINDEX, _ltoa((LONG)iBitmapSelected, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_USTYPE,   pszType);
       WinSetDlgItemText(hWnd, IT_CBSIZE,   _ltoa((LONG)pbfh2->cbSize, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_XHOTSPOT, _ltoa((LONG)pbfh2->xHotspot, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_YHOTSPOT, _ltoa((LONG)pbfh2->yHotspot, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_OFFBITS,  _ltoa((LONG)pbfh2->offBits,	szNumber, 10));
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       WinDismissDlg(hWnd, TRUE);
	       break;
	   }
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   Module Purpose - Dialog Procedure")
#pragma	page( )

/* --- ViewBITMAPINFOHEADER2DlgProc -------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
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
/*     ViewBITMAPINFOHEADER2DlgProc = Message Handling Result		*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewBITMAPINFOHEADER2DlgProc(HWND hWnd, ULONG msg, MPARAM mp1,	 MPARAM	mp2)

{
CHAR		   szNumber[32];   /* Numeric Buffer			*/
PBITMAPINFOHEADER2 pbmp2;	   /* Bitmap Info Pointer		*/
PSZ		   pszType;	   /* Type String Pointer		*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       pbmp2 = (PBITMAPINFOHEADER2)PVOIDFROMMP(mp2);
       WinSetDlgItemText(hWnd, IT_BITMAPINDEX, _ltoa((LONG)iBitmapSelected, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CBFIX,     _ltoa((LONG)pbmp2->cbFix, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CX,	     _ltoa((LONG)pbmp2->cx, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CY,	     _ltoa((LONG)pbmp2->cy, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CPLANES,   _ltoa((LONG)pbmp2->cPlanes, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CBITCOUNT, _ltoa((LONG)pbmp2->cBitCount, szNumber, 10));

       switch (	pbmp2->ulCompression )
	   {
	   case	BCA_UNCOMP :
	       pszType = "BCA_UNCOMP";
	       break;

	   case	BCA_HUFFMAN1D :
	       pszType = "BCA_HUFFMAN1D";
	       break;

	   case	BCA_RLE4 :
	       pszType = "BCA_RLE4";
	       break;

	   case	BCA_RLE8 :
	       pszType = "BCA_RLE8";
	       break;

	   case	BCA_RLE24 :
	       pszType = "BCA_RLE24";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_ULCOMPRESSION, pszType);
       WinSetDlgItemText(hWnd, IT_CBIMAGE,	   _ltoa((LONG)pbmp2->cbImage, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CXRESOLUTION,	   _ltoa((LONG)pbmp2->cxResolution, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CYRESOLUTION,	   _ltoa((LONG)pbmp2->cyResolution, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CCLRUSED,	   _ltoa((LONG)pbmp2->cclrUsed,	szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CCLRIMPORTANT,   _ltoa((LONG)pbmp2->cclrImportant, szNumber, 10));

       switch (	pbmp2->usUnits )
	   {
	   case	BRU_METRIC :
	       pszType = "BRU_METRIC";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_USUNITS,	   pszType);
       WinSetDlgItemText(hWnd, IT_USRESERVED,	   _ltoa((LONG)pbmp2->usReserved, szNumber, 10));

       switch (	pbmp2->usRecording )
	   {
	   case	BRA_BOTTOMUP :
	       pszType = "BRA_BOTTOMUP";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_USRECORDING,	   pszType);
       switch (	pbmp2->usRendering )
	   {
	   case	BRH_NOTHALFTONED :
	       pszType = "BRH_NOTHALFTONED";
	       break;

	   case	BRH_ERRORDIFFUSION :
	       pszType = "BRH_ERRORDIFFUSION";
	       break;

	   case	BRH_PANDA :
	       pszType = "BRH_PANDA";
	       break;

	   case	BRH_SUPERCIRCLE	:
	       pszType = "BRH_SUPERCIRCLE";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_USRENDERING,	   pszType);
       WinSetDlgItemText(hWnd, IT_CSIZE1,	   _ltoa((LONG)pbmp2->cSize1, szNumber,	10));
       WinSetDlgItemText(hWnd, IT_CSIZE2,	   _ltoa((LONG)pbmp2->cSize2, szNumber,	10));
       switch (	pbmp2->ulColorEncoding )
	   {
	   case	BCE_PALETTE :
	       pszType = "BCE_PALETTE";
	       break;

	   case	BCE_RGB	:
	       pszType = "BCE_RGB";
	       break;

	   case	BRH_PANDA :
	       pszType = "BRH_PANDA";
	       break;

	   case	BRH_SUPERCIRCLE	:
	       pszType = "BRH_SUPERCIRCLE";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_ULCOLORENCODING, pszType);
       WinSetDlgItemText(hWnd, IT_ULIDENTIFIER,	   _ltoa((LONG)pbmp2->ulIdentifier, szNumber, 10));
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       WinDismissDlg(hWnd, TRUE);
	       break;
	   }
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   Module Purpose - Dialog Procedure")
#pragma	page( )

/* --- ViewBITMAPARRAYFILEHEADERDlgProc	---------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
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
/*     ViewBITMAPARRAYFILEHEADERDlgProc	= Message Handling Result	*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewBITMAPARRAYFILEHEADERDlgProc(HWND hWnd, ULONG msg,	MPARAM mp1, MPARAM mp2)

{
CHAR		       szNumber[32];   /* Numeric Buffer		*/
PBITMAPARRAYFILEHEADER pbafh;	  /* Bitmap Array File Header Pointer  */
PSZ		       pszType;	   /* Type String Pointer		*/
register INT i;			   /* Index				*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       pbafh = (PBITMAPARRAYFILEHEADER)abm[i = LONGFROMMP(mp2)].pb;
       switch (	pbafh->usType )
	   {
	   case	BFT_ICON :
	       pszType = "BFT_ICON";
	       break;

	  case BFT_BMAP	:
	       pszType = "BFT_BMAP";
	       break;

	   case	BFT_POINTER :
	       pszType = "BFT_POINTER";
	       break;

	   case	BFT_COLORICON :
	       pszType = "BFT_COLORICON";
	       break;

	   case	BFT_COLORPOINTER :
	       pszType = "BFT_COLORPOINTER";
	       break;

	   case	BFT_BITMAPARRAY	:
	       pszType = "BFT_BITMAPARRAY";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_BITMAPINDEX, _ltoa((LONG)i, szNumber,	10));
       WinSetDlgItemText(hWnd, IT_USTYPE,    pszType);
       WinSetDlgItemText(hWnd, IT_CBSIZE,    _ltoa((LONG)pbafh->cbSize,	szNumber, 10));
       WinSetDlgItemText(hWnd, IT_OFFNEXT,   _ltoa((LONG)pbafh->offNext, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CXDISPLAY, _ltoa((LONG)pbafh->cxDisplay, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CYDISPLAY, _ltoa((LONG)pbafh->cyDisplay, szNumber, 10));
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       WinDismissDlg(hWnd, TRUE);
	       break;
	   }
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   Module Purpose - Dialog Procedure")
#pragma	page( )

/* --- ViewBITMAPFILEHEADERDlgProc --------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
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
/*     ViewBITMAPFILEHEADERDlgProc = Message Handling Result		*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewBITMAPFILEHEADERDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2)

{
CHAR		  szNumber[32];	   /* Numeric Buffer			*/
PBITMAPFILEHEADER pbfh;		   /* Bitmap Array File	Header Pointer	*/
PSZ		  pszType;	   /* Type String Pointer		*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       pbfh = (PBITMAPFILEHEADER)PVOIDFROMMP(mp2);
       switch (	pbfh->usType )
	   {
	   case	BFT_ICON :
	       pszType = "BFT_ICON";
	       break;

	  case BFT_BMAP	:
	       pszType = "BFT_BMAP";
	       break;

	   case	BFT_POINTER :
	       pszType = "BFT_POINTER";
	       break;

	   case	BFT_COLORICON :
	       pszType = "BFT_COLORICON";
	       break;

	   case	BFT_COLORPOINTER :
	       pszType = "BFT_COLORPOINTER";
	       break;

	   case	BFT_BITMAPARRAY	:
	       pszType = "BFT_BITMAPARRAY";
	       break;

	   default :
	       pszType = "Unknown";
	       break;
	   }
       WinSetDlgItemText(hWnd, IT_BITMAPINDEX, _ltoa((LONG)iBitmapSelected, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_USTYPE,   pszType);
       WinSetDlgItemText(hWnd, IT_CBSIZE,   _ltoa((LONG)pbfh->cbSize, szNumber,	10));
       WinSetDlgItemText(hWnd, IT_XHOTSPOT, _ltoa((LONG)pbfh->xHotspot,	szNumber, 10));
       WinSetDlgItemText(hWnd, IT_YHOTSPOT, _ltoa((LONG)pbfh->yHotspot,	szNumber, 10));
       WinSetDlgItemText(hWnd, IT_OFFBITS,  _ltoa((LONG)pbfh->offBits, szNumber, 10));
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       WinDismissDlg(hWnd, TRUE);
	       break;
	   }
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   Module Purpose - Dialog Procedure")
#pragma	page( )

/* --- ViewBITMAPINFOHEADERDlgProc --------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
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
/*     ViewBITMAPINFOHEADERDlgProc = Message Handling Result		*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewBITMAPINFOHEADERDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1,	MPARAM mp2)

{
CHAR		  szNumber[32];	   /* Numeric Buffer			*/
PBITMAPINFOHEADER pbmp;		   /* Bitmap Info Pointer		*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       pbmp = (PBITMAPINFOHEADER)PVOIDFROMMP(mp2);
       WinSetDlgItemText(hWnd, IT_BITMAPINDEX, _ltoa((LONG)iBitmapSelected, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CBFIX,     _ltoa((LONG)pbmp->cbFix, szNumber,	10));
       WinSetDlgItemText(hWnd, IT_CX,	     _ltoa((LONG)pbmp->cx, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CY,	     _ltoa((LONG)pbmp->cy, szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CPLANES,   _ltoa((LONG)pbmp->cPlanes,	szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CBITCOUNT, _ltoa((LONG)pbmp->cBitCount, szNumber, 10));
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       WinDismissDlg(hWnd, TRUE);
	       break;
	   }
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   View Dialogues - RGB Table Value Set Function")
#pragma	page( )

/* --- SetTableValues ---------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	set the	values for the RGB table	*/
/*     within the RGB dialogue.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Dialog Window Handle				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID SetTableValues(HWND	hWnd)

{
CHAR   szColour[32];		   /* Numeric Value			*/
CHAR   szIndex[32];		   /* Numeric Value			*/
LONG   cShown;			   /* Number of	Colours	Shown		*/
PULONG pul;			   /* Long Pointer			*/
register INT i,	n;		   /* Loop Counter			*/

		       /* Check	to see if the table only contains 2	*/
		       /* colours in which case	only need to update	*/
		       /* 2 colours					*/

if ( abm[iBitmapSelected].cColours == 2	)
   cShown = 2;
else
   cShown = 16;

for ( i	= lVScrollPos, n = 0; n	< cShown; i++, n++ )
   {
   sprintf(szIndex, "RGB[%3d] =", i);
   if (	f20Bitmap )
       {
       pul = (PULONG)(PVOID)&prgb2[i];
       sprintf(szColour, "0x%08X", *pul);
       }
   else
       sprintf(szColour, "0x00%02X%02X%02X", prgb[i].bRed, prgb[i].bGreen, prgb[i].bBlue);
   WinSetDlgItemText(hWnd, (ULONG)(IT_INDEX00 +	n), szIndex);
   WinSetDlgItemText(hWnd, (ULONG)(EF_INDEX00 +	n), szColour);
   }
}
#pragma	subtitle("   View Dialogues - RGB Table Query Function")
#pragma	page( )

/* --- QueryTableValues	-------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	get the	values from the	RGB table	*/
/*     within the RGB dialogue.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Dialog Window Handle				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID QueryTableValues(HWND hWnd)

{
CHAR   szColour[32];		   /* Numeric Value			*/
LONG   cShown;			   /* Number of	Colours	Shown		*/
PULONG pul;			   /* Long Pointer			*/
ULONG  ulColour;		   /* Colour Value			*/
register INT i,	n;		   /* Loop Counter			*/

		       /* Check	to see if the table only contains 2	*/
		       /* colours in which case	only need to query	*/
		       /* 2 colours					*/

if ( abm[iBitmapSelected].cColours == 2	)
   cShown = 2;
else
   cShown = 16;

for ( i	= lVScrollPos, n = 0; n	< cShown; i++, n++ )
   {
   WinQueryDlgItemText(hWnd, (ULONG)(EF_INDEX00	+ n), 11L, szColour);
   ulColour = (ULONG)strtoul(szColour, NULL, 0);
   if (	f20Bitmap )
       {
       pul = (PULONG)(PVOID)&prgb2[i];
       *pul = ulColour;
       }
   else
       {
       prgb[i].bRed   =	(BYTE)((ulColour & 0x00ff0000UL) >> 16);
       prgb[i].bGreen =	(BYTE)((ulColour & 0x0000ff00UL) >> 8);
       prgb[i].bBlue  =	(BYTE)(ulColour	& 0x000000ffUL);
       }
   }
}
#pragma	subtitle("   View Dialogues - View Colour RGB Colour Table Procedure")
#pragma	page( )

/* --- ViewColourTableDlgProc -------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the View	*/
/*     RGB Colour Table	dialogue.					*/
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
/*     ViewColourTableDlgProc =	Message	Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewColourTableDlgProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)

{
BOOL   fUpdate;			   /* Table Updated Flag		*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       lVScrollPos = 0L;
       if ( f20Bitmap )
	   memcpy(prgb2	= (RGB2	*)malloc(abm[iBitmapSelected].cColours * sizeof(RGB2)),
		  pbTable = (PBYTE)PVOIDFROMMP(mp2), abm[iBitmapSelected].cColours * sizeof(RGB2));
       else
	   memcpy(prgb = (RGB *)malloc(abm[iBitmapSelected].cColours * sizeof(RGB)),
		  pbTable = (PBYTE)PVOIDFROMMP(mp2), abm[iBitmapSelected].cColours * sizeof(RGB));

       if ( abm[iBitmapSelected].cColours == 16L )
	   WinDestroyWindow(WinWindowFromID(hWnd, SB_INDEX));

       SetTableValues(hWnd);
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       QueryTableValues(hWnd);
	       if ( f20Bitmap )
		   {
		   if (	memcmp(prgb2, pbTable, abm[iBitmapSelected].cColours * sizeof(RGB2)) )
		       {
		       memcpy(pbTable, prgb2, abm[iBitmapSelected].cColours * sizeof(RGB2));
		       fUpdate = TRUE;
		       }
		   else
		       fUpdate = FALSE;
		   free(prgb2);
		   }
	       else
		   {
		   if (	memcmp(prgb, pbTable, abm[iBitmapSelected].cColours * sizeof(RGB)) )
		       {
		       memcpy(pbTable, prgb, abm[iBitmapSelected].cColours * sizeof(RGB));
		       fUpdate = TRUE;
		       }
		   else
		       fUpdate = FALSE;
		   free(prgb);
		   }
	       WinDismissDlg(hWnd, fUpdate);
	       break;
	   }
       break;

/************************************************************************/
/* Vertical Scroll bar selected						*/
/************************************************************************/

   case	WM_VSCROLL :
       QueryTableValues(hWnd);
       switch (	HIUSHORT(mp2) )
	   {
	   case	SB_LINEUP :
	       lVScrollPos--;
	       break;

	   case	SB_LINEDOWN :
	       lVScrollPos++;
	       break;

	   case	SB_PAGEUP :
	       lVScrollPos -= 16L;
	       break;

	   case	SB_PAGEDOWN :
	       lVScrollPos += 16L;
	       break;

	   case	SB_SLIDERPOSITION :
	       lVScrollPos = SHORT1FROMMP(mp2);
	       break;

	   default :
	       return(0L);
	   }
		       /* Reposition slider in scroll bar		*/

       WinSendDlgItemMsg(hWnd, SB_INDEX, SBM_SETPOS, MPFROM2SHORT(lVScrollPos =	max(0, min(lVScrollPos,	240)), 0), 0L);
       SetTableValues(hWnd);
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   View Dialogues  - View System Info Procedure")
#pragma	page( )

/* --- ViewSystemBitmapInfoDlgProc --------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
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
/*     ViewSystemBitmapInfoDlgProc = Message Handling Result		*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewSystemBitmapInfoDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1,	MPARAM mp2)

{
CHAR szNumber[32];		   /* Numeric Buffer			*/
LONG alCaps[25];		   /* Caps Info	Array			*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       DevQueryCaps(WinOpenWindowDC(hWnd), CAPS_FAMILY,	25L, alCaps);
       WinSetDlgItemText(hWnd, IT_CXSCREEN,	   _ltoa(alCaps[CAPS_WIDTH], szNumber, 10));
       WinSetDlgItemText(hWnd, IT_CYSCREEN,	   _ltoa(alCaps[CAPS_HEIGHT], szNumber,	10));
       WinSetDlgItemText(hWnd, IT_HORZRESOLUTION,  _ltoa(alCaps[CAPS_HORIZONTAL_RESOLUTION], szNumber, 10));
       WinSetDlgItemText(hWnd, IT_VERTRESOLUTION,  _ltoa(alCaps[CAPS_VERTICAL_RESOLUTION], szNumber, 10));
       WinSetDlgItemText(hWnd, IT_PLANES,	   _ltoa(alCaps[CAPS_COLOR_PLANES], szNumber, 10));
       WinSetDlgItemText(hWnd, IT_DISTINCTCOLOURS, _ltoa(alCaps[CAPS_COLORS], szNumber,	10));
       WinSetDlgItemText(hWnd, IT_BITCOUNT,	   _ltoa(alCaps[CAPS_COLOR_BITCOUNT], szNumber,	10));
       WinSetDlgItemText(hWnd, IT_BITMAPFORMATS,   _ltoa(alCaps[CAPS_BITMAP_FORMATS], szNumber,	10));

       if ( alCaps[CAPS_RASTER_CAPS] & CAPS_RASTER_BITBLT )
	   WinSetDlgItemText(hWnd, IT_BITBLT, "Yes");
       if ( alCaps[CAPS_RASTER_CAPS] & CAPS_RASTER_BANDING )
	   WinSetDlgItemText(hWnd, IT_BANDING, "Yes");
       if ( alCaps[CAPS_RASTER_CAPS] & CAPS_RASTER_BITBLT_SCALING )
	   WinSetDlgItemText(hWnd, IT_BITBLTSCALING, "Yes");
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       WinDismissDlg(hWnd, TRUE);
	       break;
	   }
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}

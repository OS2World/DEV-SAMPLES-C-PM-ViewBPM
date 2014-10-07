#pragma	title("View Bitmap  --  Version 1  --  (AppDefs.C)")
#pragma	subtitle("   Application Definitions - Interface Definitions")

#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <os2.h>

#include "appdefs.h"
#include "viewbmp.h"

/* This	module contains	the global data	definitions for	the		*/
/* application.								*/

/* Filename:   AppDefs.C						*/

/*  Version:   1							*/
/*  Created:   1995-07-05						*/
/*  Revised:   1995-07-15						*/

/* Routines:   None							*/


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

FONTMETRICS fm;			   /* Font Metrics Info			*/
HAB	    hAB;		   /* Program Anchor Block Handle	*/
HSWITCH	    hSwitch;		   /* Task List	Entry Handle		*/

HMQ  hmqViewBmp;		   /* Program Message Queue Handle	*/
HWND hwndViewBmp;		   /* Client Window Handle		*/
HWND hwndViewBmpFrame;		   /* Frame Window Handle		*/
HWND hmenuViewBmp;		   /* Menu Handle			*/
HWND hmenuPopup;		   /* Popup Menu Handle			*/

HPOINTER hptrArrow;		   /* Arrow Pointer Handle		*/
HPOINTER hptrWait;		   /* Wait Pointer Handle		*/

LONG	 cxScreen;		   /* Screen Width			*/

CHAR	 szBitmapFile[CCHMAXPATH]; /* Bitmap Filename Holder		*/

BITMAPSTACK abm[32];		   /* Bitmap Stack Handles		*/
LONG	 cBitmaps = 0L;		   /* Bitmap Stack Count		*/

PBYTE	 pb;			   /* Bitmap Data File Pointer		*/
BOOL	 f20Bitmap;		   /* 2.x Bitmap Type			*/
BOOL	 fWindowsBitmap;	   /* Windows Bitmap Type		*/
LONG	 iBitmap;		   /* Single Bitmap Index		*/
LONG	 iBitmapSelected;	   /* Selected Bitmap Index		*/
BOOL	 fBitmapArray;		   /* Bitmap Array Flag			*/

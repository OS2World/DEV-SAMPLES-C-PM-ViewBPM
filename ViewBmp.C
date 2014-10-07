#pragma	title("View Bitmap  --  Version 1  --  (ViewBmp.C)")
#pragma	subtitle("   main( ) - Interface Definitions")

#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#if defined(__IBMC__)

#pragma	info(nopar)

#endif

#include <os2.h>

#include "appdefs.h"
#include "viewbmp.h"

/* This	module contains	routine	used as	the main entry point to	the	*/
/* Bitmap View application.						*/

/* Filename:   ViewBmp.C						*/

/*  Version:   1							*/
/*  Created:   1995-07-06						*/
/*  Revised:   1995-07-22						*/

/* Routines:   INT main(INT argc, CHAR *argv[ ]);			*/


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

PSZ pszViewBmpClassName	= (PSZ)"ViewBmp";


INT main(INT argc, CHAR	*argv[ ])

{
QMSG  qmsg;			   /* PM Message Queue Holder		*/

		       /* Initialize the program for PM	and create the	*/
		       /* message queue					*/

hmqViewBmp = WinCreateMsgQueue(hAB = WinInitialize(0UL), 0L);

		       /* Register the window class			*/

if ( !WinRegisterClass(hAB, pszViewBmpClassName, (PFNWP)BitmapViewerWndProc,
		       CS_SYNCPAINT | CS_SIZEREDRAW, 8UL) )
   return(0);
		       /* Create the main program window		*/

if ( !(hwndViewBmpFrame	= CreateStdWindow(HWND_DESKTOP,	WS_VISIBLE, FCF_NOBYTEALIGN | FCF_TASKLIST |
					  FCF_TITLEBAR | FCF_ICON | FCF_SYSMENU	|
					  FCF_MENU | FCF_MINMAX	| FCF_SIZEBORDER,
					  pszViewBmpClassName, (PSZ)"Bitmap Viewer", 0L,
					  (HMODULE)NULL, WIN_BITMAPVIEWER, &hwndViewBmp, 10, 10, 150, 75)) )
   return(0);
		       /* Perform application initialization		*/

InitApp(hwndViewBmpFrame, hwndViewBmp, NULL);

		       /* Get and dispatch the message to program	*/
		       /* windows					*/

while (	WinGetMsg(hAB, &qmsg, (HWND)NULL, 0UL, 0UL) )
   WinDispatchMsg(hAB, &qmsg);

		       /* Have received	a WM_QUIT, start the program	*/
		       /* shutdown by destroying the program windows	*/
		       /* and destroying the message queue		*/

WinDestroyWindow(hwndViewBmpFrame);

WinDestroyMsgQueue(hmqViewBmp);

		       /* Notify PM that main program thread not needed	*/
		       /* any longer					*/
WinTerminate(hAB);
		       /* Exit back to OS/2 cleanly			*/
return(0);
}

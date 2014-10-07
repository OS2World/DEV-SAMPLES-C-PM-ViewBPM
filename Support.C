#pragma	title("View Bitmap  --  Version 1  --  (Support.C)")
#pragma	subtitle("   Support Module - Interface Definitions")

#define	INCL_DOS		   /* Include OS/2 PM DOS Interface	*/
#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <os2.h>

#include <string.h>
#include "appdefs.h"
#include "viewbmp.h"

/* This	module contains	routines used to provide miscellanious support.	*/

/* Filename:   Support.C						*/

/*  Version:   1							*/
/*  Created:   1995-07-05						*/
/*  Revised:   1995-07-05						*/

/* Routines:   VOID PDSGetTemplate(HWND	hWnd, ULONG idDlg);		*/
/*	       MRESULT PDSKeyProc(HWND hWnd, ULONG msg,	MPARAM mp1,	*/
/*				  MPARAM mp2);				*/
/*	       VOID InitApp(HWND hwndFrame, HWND hwndClient,		*/
/*			    PSZ	pszWindowListTitle);			*/
/*	       HWND CreateStdWindow(HWND hwndParent, ULONG flStyle,	*/
/*				    ULONG flCreateFlags,		*/
/*				    PSZ	pszClientClass,	PSZ pszTitle,	*/
/*				    ULONG styleClient, HMODULE hmod,	*/
/*				    ULONG idResources,			*/
/*				    PHWND phwndClient, LONG x, LONG y,	*/
/*				    LONG cx, LONG cy);			*/


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


/* -- Standard Window Classes -----------------------------------------	*/

#define	WINCLASS_FRAME		   0x00000001L
#define	WINCLASS_COMBOBOX	   0x00000002L
#define	WINCLASS_BUTTON		   0x00000003L
#define	WINCLASS_MENU		   0x00000004L
#define	WINCLASS_STATIC		   0x00000005L
#define	WINCLASS_ENTRYFIELD	   0x00000006L
#define	WINCLASS_LISTBOX	   0x00000007L
#define	WINCLASS_SCROLLBAR	   0x00000008L
#define	WINCLASS_TITLEBAR	   0x00000009L
#define	WINCLASS_MLE		   0x0000000AL
/* 000B	to 000F	reserved */
#define	WINCLASS_APPSTAT	   0x00000010L
#define	WINCLASS_KBDSTAT	   0x00000011L
#define	WINCLASS_PECIC		   0x00000012L
#define	WINCLASS_DBE_KKPOPUP	   0x00000013L
/* 0014	to 001F	reserved */
#define	WINCLASS_SPINBUTTON	   0x00000020L
/* 0021	to 0024	reserved */
#define	WINCLASS_CONTAINER	   0x00000025L
#define	WINCLASS_SLIDER		   0x00000026L
#define	WINCLASS_VALUESET	   0x00000027L
#define	WINCLASS_NOTEBOOK	   0x00000028L
#define	WINCLASS_HWXENTRY	   0x00000029L
#define	WINCLASS_SKETCH		   0x0000002aL
/* 002b	to 0030	reserved */
/* 0030	to 003F	reserved */
#define	WINCLASS_GRAPHICBUTTON	   0x00000040L
#define	WINCLASS_CIRCULARSLIDER	   0x00000041L

#if !defined(WC_HWXENTRY)
#define	WC_HWXENTRY	     ((PSZ)0xffff0029L)
#endif

#if !defined(WC_SKETCH)
#define	WC_SKETCH	     ((PSZ)0xffff002aL)
#endif

#if !defined(WC_GRAPHICBUTTON)
#define	WC_GRAPHICBUTTON     ((PSZ)0xffff0040L)
#endif

#if !defined(WC_CIRCULARSLIDER)
#define	WC_CIRCULARSLIDER    ((PSZ)0xffff0041L)
#endif

#pragma	subtitle("      Window Controls Support - Template Load Function")
#pragma	page( )

/* --- PDSGetTemplate ---------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	read in	the dialog information from	*/
/*     the opened resource file.  The information read in for each	*/
/*     control is then analyzed	and converted into suitable format	*/
/*     to allow	the control to be created.				*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND  hWnd;  = Window Handle					*/
/*     ULONG idDlg; = Dialog ID						*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID PDSGetTemplate(HWND hWnd, ULONG idDlg)

{
PDLGTEMPLATE pdlgt;		   /* Dialog Template Pointer		*/
POINTL	     aptl[2];		   /* Dialog Points Array		*/
PVOID	     pvClassName;	   /* Control Data Pointer		*/
PVOID	     pvCtlData;		   /* Control Data Pointer		*/
PVOID	     pvPresParams;	   /* Presentation Parameters Pointer	*/
PVOID	     pvStart;		   /* Dialog Template Item Pointer	*/
PVOID	     pvText;		   /* Control Data Pointer		*/
INT	     cbparam;		   /* Presentation Parameter Size	*/
PPARAM	     pparam;		   /* Presentation Parameters Pointer	*/
PPRESPARAMS  ppres;		   /* Presentation Parameters Pointer	*/
register INT cItems;		   /* Dialog Items Counter		*/
register INT i;			   /* Loop Counter			*/

		       /* Try reading in the dialog template for the	*/
		       /* dialog ID given				*/

if ( DosGetResource((HMODULE)NULL, RT_DIALOG, idDlg, (PPVOID)(PVOID)&pdlgt) )

		       /* Dialog template not found, exit without	*/
		       /* creating any controls				*/
   return;
		       /* Convert the memory selector returned into an	*/
		       /* addressable pointer to allow the controls to	*/
		       /* be decoded					*/

pvStart	= (PVOID)pdlgt;
		       /* Check	to see if any presentation parameters	*/
		       /* associated with the control.	A -1 indicates	*/
		       /* that no presentation parameters are		*/
		       /* associated.					*/

if ( pdlgt->adlgti[0].offPresParams != 0xffff )
   {
   ppres = (PPRESPARAMS)((PBYTE)pvStart	+ pdlgt->adlgti[0].offPresParams);
   cbparam = (INT)ppres->cb;

   i = 0;
   pparam = ppres->aparam;
   while ( cbparam )
       {
       pparam =	(PPARAM)((BYTE *)pparam	+ i);
       WinSetPresParam(hWnd, pparam->id, pparam->cb, pparam->ab);
       cbparam -= (i = (INT)pparam->cb + (INT)sizeof(ULONG) * 2);
       }
   }
		       /* Save the number of controls found within the	*/
		       /* dialog template				*/

cItems = pdlgt->adlgti[0].cChildren + 1;

		       /* Read in and translate	each of	the controls	*/

for ( i	= 1; i < cItems; i++ )
   {
		       /* Get the position and size of the control and	*/
		       /* convert from dialog units to actual window	*/
		       /* co-ordinates					*/

   aptl[0].x = (LONG)pdlgt->adlgti[i].x;
   aptl[0].y = (LONG)pdlgt->adlgti[i].y;
   aptl[1].x = (LONG)pdlgt->adlgti[i].cx;
   aptl[1].y = (LONG)pdlgt->adlgti[i].cy;

   WinMapDlgPoints(hWnd, aptl, 2UL, TRUE);

		       /* Check	to see if a custom class is specified	*/
		       /* or if	a standard PM control class is being	*/
		       /* used						*/

   if (	pdlgt->adlgti[i].cchClassName )

		       /* Since	a length for the class name present,	*/
		       /* a custom class name is being used for	the	*/
		       /* control.  Point to the memory	location where	*/
		       /* the class name is found within the dialog	*/
		       /* template information.				*/

       pvClassName = (PVOID)((PBYTE)pvStart + pdlgt->adlgti[i].offClassName);
   else
		       /* No class name	length given indicating	that a	*/
		       /* standard PM class is being used.  The	class	*/
		       /* name is stored as an index value.  For	*/
		       /* example, the class for static's is defined as */
		       /*						*/
		       /* #define WC_STATIC ((PSZ)0xffff0005L)		*/
		       /*						*/
		       /* The values within the	dialog template	for	*/
		       /* the static class would be			*/
		       /*						*/
		       /* adlgti[i].cchClassName = 0			*/
		       /* adlgti[i].offClassName = 5			*/
		       /*						*/
		       /* Therefore, the value of offClassName field	*/
		       /* must be used as an index that	is used	to	*/
		       /* actually select the class name.		*/

       switch (	pdlgt->adlgti[i].offClassName )
	   {
		       /* Control Type:	 Button				*/

	   case	WINCLASS_BUTTON	:
	       pvClassName = WC_BUTTON;	
	       break;
		       /* Control Type:	 Frame				*/

	   case	WINCLASS_FRAME :
	       pvClassName = WC_FRAME;
	       break;
		       /* Control Type:	 Scroll	Bar			*/

	   case	WINCLASS_SCROLLBAR :
	       pvClassName = WC_SCROLLBAR;
	       break;
		       /* Control Type:	 List Box			*/

	   case	WINCLASS_LISTBOX :
	       pvClassName = WC_LISTBOX;
	       break;
		       /* Control Type:	 Edit				*/

	   case	WINCLASS_ENTRYFIELD :
	       pvClassName = WC_ENTRYFIELD;
	       break;
		       /* Control Type:	 Static				*/

	   case	WINCLASS_STATIC	:
	       pvClassName = WC_STATIC;	
	       break;
		       /* Control Type:	 Combo Box			*/

	   case	WINCLASS_COMBOBOX :
	       pvClassName = WC_COMBOBOX;
	       break;
		       /* Control Type:	 Multi-Line Edit		*/

	   case	WINCLASS_MLE :
	       pvClassName = WC_MLE;
	       break;
		       /* Control Type:	 Spin Button		  [1.3]	*/

	   case	WINCLASS_SPINBUTTON :
	       pvClassName = WC_SPINBUTTON;
	       break;
		       /* Control Type:	 Container		  [2.0]	*/

	   case	WINCLASS_CONTAINER :
	       pvClassName = WC_CONTAINER;
	       break;
		       /* Control Type:	 Slider			  [2.0]	*/

	   case	WINCLASS_SLIDER	:
	       pvClassName = WC_SLIDER;
	       break;
		       /* Control Type:	 Value Set		  [2.0]	*/

	   case	WINCLASS_VALUESET :
	       pvClassName = WC_VALUESET;
	       break;
		       /* Control Type:	 Notebook		  [2.0]	*/

	   case	WINCLASS_NOTEBOOK :
	       pvClassName = WC_NOTEBOOK;
	       break;
		       /* Control Type:	 Handwriting	 [Pen for OS/2]	*/

	   case	WINCLASS_HWXENTRY :
	       pvClassName = WC_HWXENTRY;
	       break;
		       /* Control Type:	 Sketch		 [Pen for OS/2]	*/

	   case	WINCLASS_SKETCH	:
	       pvClassName = WC_SKETCH;
	       break;
		       /* Control Type:	 Graphic Button	       [MMPM/2]	*/

	   case	WINCLASS_GRAPHICBUTTON :
	       pvClassName = WC_GRAPHICBUTTON;
	       break;
		       /* Control Type:	 Circular Slider       [MMPM/2]	*/

	   case	WINCLASS_CIRCULARSLIDER	:
	       pvClassName = WC_CIRCULARSLIDER;
	       break;
	   }
		       /* Check	to see if any control data associated	*/
		       /* with the control.  A -1 indicates that no	*/
		       /* control data is associated.			*/

   if (	pdlgt->adlgti[i].offCtlData != 0xffff )
       pvCtlData = (PVOID)((PBYTE)pvStart + pdlgt->adlgti[i].offCtlData);
   else
       pvCtlData = NULL;
		       /* Check	to see if any presentation parameters	*/
		       /* associated with the control.	A -1 indicates	*/
		       /* that no presentation parameters are		*/
		       /* associated.					*/

   if (	pdlgt->adlgti[i].offPresParams != 0xffff )
       pvPresParams = (PVOID)((PBYTE)pvStart + pdlgt->adlgti[i].offPresParams);
   else
       pvPresParams = NULL;

		       /* Check	to see if any text specified for the	*/
		       /* control					*/

   if (	pdlgt->adlgti[i].cchText )
       pvText =	(PVOID)((PBYTE)pvStart + pdlgt->adlgti[i].offText);
   else
       pvText =	NULL;
		       /* Create the control				*/

   WinCreateWindow(hWnd, pvClassName, pvText,
		   pdlgt->adlgti[i].flStyle,
		   aptl[0].x, aptl[0].y,
		   aptl[1].x, aptl[1].y,
		   hWnd, HWND_BOTTOM, (ULONG)(pdlgt->adlgti[i].id & 0xffff),
		   pvCtlData, pvPresParams);
   }
		       /* Release the memory allocated for the dialog	*/
		       /* template before returning			*/
DosFreeResource(pvStart);
}
#pragma	subtitle("      Window Controls Support - Default Window Procedure")
#pragma	page( )

/* --- PDSKeyProc -------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the window messages for	a	*/
/*     window that has controls	within it where	TAB key	selection	*/
/*     of controls should be possible.	The function is	used to		*/
/*     replace the WinDefWindowProc in such a window.			*/
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
/*     PDSKeyProc = Message Handling Result				*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	PDSKeyProc(HWND	hWnd, ULONG msg, MPARAM	mp1, MPARAM mp2)

{

switch ( msg )
   {
		       /* Check	for key	strokes				*/
   case	WM_CHAR	:
		       /* Check	for the	key up flag in which case the	*/
		       /* condition should be ignored			*/

       if ( SHORT1FROMMP(mp1) &	KC_KEYUP )
	   break;
		       /* Check	for virtual keys			*/

       if ( SHORT1FROMMP(mp1) &	KC_VIRTUALKEY )
	   switch ( SHORT2FROMMP(mp2) )
	       {
	       case VK_TAB :

		       /* TAB key pressed, determine which control is	*/
		       /* the next tab stop and	set the	focus on that	*/
		       /* control					*/

		   WinSetFocus(HWND_DESKTOP,
			       WinEnumDlgItem(hWnd,
					      WinQueryFocus(HWND_DESKTOP),
					      EDI_NEXTTABITEM));
		   break;

	       case VK_RIGHT :
	       case VK_DOWN :

		       /* Right	or down	arrow key pressed, determine	*/
		       /* which	control	is the next entry and set the	*/
		       /* focus	on that	control				*/

		   WinSetFocus(HWND_DESKTOP,
			       WinEnumDlgItem(hWnd,
					      WinQueryFocus(HWND_DESKTOP),
					      EDI_NEXTGROUPITEM));
		   break;

	       case VK_BACKTAB :

		       /* Shift+TAB key	pressed, determine which	*/
		       /* control is the previous tab stop and set the	*/
		       /* focus	on that	control				*/

		   WinSetFocus(HWND_DESKTOP,
			       WinEnumDlgItem(hWnd,
					      WinQueryFocus(HWND_DESKTOP),
					      EDI_PREVTABITEM));
		   break;

	       case VK_LEFT :
	       case VK_UP :

		       /* Left or up arrow key pressed,	determine	*/
		       /* which	control	is the previous	entry and set	*/
		       /* the focus on that control			*/

		   WinSetFocus(HWND_DESKTOP,
			       WinEnumDlgItem(hWnd,
					      WinQueryFocus(HWND_DESKTOP),
					      EDI_PREVGROUPITEM));
		   break;
	       }
       break;
		       /* Virtual key not TAB or Shift+TAB, fall	*/
		       /* through to default window procedure		*/

		       /* Default message processing			*/
   default :
       return(WinDefWindowProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   Program Initialization -   Program Initialization Function")
#pragma	page( )

/* --- InitApp ----------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	perform	basic application		*/
/*     initialization to support the owner draw	list boxes and to set	*/
/*     the window list title if	provided.				*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hwndFrame;	  = Application	Frame Window Handle		*/
/*     HWND hwndClient;	  = Application	Client Window Handle		*/
/*     PSZ  pszTaskTitle; = Window List	Title				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID InitApp(HWND hwndFrame, HWND hwndClient, PSZ pszWindowListTitle)

{
HPS	hPS;			   /* Presentation Space Handle		*/
SWCNTRL	swCtl;			   /* Task Switch Control Structure	*/

		       /* Get a	temporary presentation space so	that	*/
		       /* the system's font metrics can be found and    */
		       /* the proper sizing of owner draw list boxes	*/
		       /* can be performed properly			*/

if ( (hPS = WinGetPS(hwndClient)) != (HPS)NULL )
   {
   GpiQueryFontMetrics(hPS, sizeof(FONTMETRICS), &fm);

		       /* Release the temporary	presentation space	*/
   WinReleasePS(hPS);
   }

if ( pszWindowListTitle	)
   {
		       /* Fill Switch Entry structure with required	*/
		       /* values before	adding program name to Task	*/
		       /* Manager switch list				*/

   swCtl.hwnd	       = hwndFrame;
   swCtl.hwndIcon      = (HWND)NULL;
   swCtl.hprog	       = (HPROGRAM)NULL;
   swCtl.idProcess     =
   swCtl.idSession     = 0;
   swCtl.uchVisibility = SWL_VISIBLE;
   swCtl.fbJump	       = SWL_JUMPABLE;
   strcpy(swCtl.szSwtitle, pszWindowListTitle);

   hSwitch = WinAddSwitchEntry(&swCtl);
   }
}
#pragma	subtitle("      Program Initialization - Window Creation Function")
#pragma	page( )

/* --- CreateStdWindow --------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	create a standard window using		*/
/*     WinCreateStdWindow and to place the window in the location	*/
/*     and size	specified if the shell position	style has not been	*/
/*     specified.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND    hwndParent;     = Parent	Window Handle			*/
/*     ULONG   flStyle;	       = Window	Style				*/
/*     ULONG   flCreateFlags;  = Frame Creation	Flags			*/
/*     PSZ     pszClientClass; = Client	Area Class			*/
/*     PSZ     pszTitle;       = Window	Title				*/
/*     HMODULE hmod;	       = Resources Module Handle		*/
/*     ULONG   idResources;    = Resource ID				*/
/*     PHWND   phwndClient;    = Client	Window Handle Pointer		*/
/*     LONG    x;	       = x Co-ordinate				*/
/*     LONG    y;	       = y Co-ordinate				*/
/*     LONG    cx;	       = Window	Width				*/
/*     LONG    cy;	       = Window	Height				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     CreateStdWindow = NULL :	Error Occurred,	No Window Created	*/
/*		       = > 0  :	Frame Window Handle			*/
/*									*/
/* --------------------------------------------------------------------	*/

HWND CreateStdWindow(HWND hwndParent, ULONG flStyle, ULONG flCreateFlags,
		     PSZ pszClientClass, PSZ pszTitle, ULONG styleClient,
		     HMODULE hmod, ULONG idResources, PHWND phwndClient,
		     LONG x, LONG y, LONG cx, LONG cy)

{
HWND   hwndFrame;		   /* Frame Window Handle		*/
POINTL aptl[2];			   /* Point Translation	Array		*/

if ( !(hwndFrame = WinCreateStdWindow(hwndParent, flStyle, &flCreateFlags,
				      pszClientClass, pszTitle,	styleClient,
				      hmod, idResources, phwndClient)) )
   return((HWND)NULL);

if ( !(flCreateFlags & FCF_SHELLPOSITION) )
   {
   aptl[0].x = x;
   aptl[0].y = y;
   aptl[1].x = cx;
   aptl[1].y = cy;
   WinMapDlgPoints(HWND_DESKTOP, aptl, 2UL, TRUE);
   WinSetWindowPos(hwndFrame, HWND_TOP,	aptl[0].x, aptl[0].y,
		   aptl[1].x, aptl[1].y, SWP_ACTIVATE |	SWP_SIZE | SWP_MOVE |
		   (ULONG)((flStyle & WS_VISIBLE) ? SWP_SHOW : 0UL));
   }
		       /* Return back the window handle			*/
return(hwndFrame);
}

#pragma	title("View Bitmap  --  Version 1  --  (Effects.C)")
#pragma	subtitle("   Effects Functions - Interface Definitions")

#define	INCL_BITMAPFILEFORMAT	   /* Include OS/2 Bitmap Information	*/
#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_GPI		   /* Include OS/2 GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <malloc.h>
#include <os2.h>
#include <string.h>

#include "appdefs.h"


/* This	module contains	the functions to apply the desired effects to	*/
/* to a	bitmap.								*/

/* Filename:   Effects.C						*/

/*  Version:   1							*/
/*  Created:   1995-07-08						*/
/*  Revised:   1995-07-22						*/

/* Routines:   HBITMAP hbmFlipColourTable(LONG iImage);			*/
/*	       HBITMAP hbmRefreshBitmap(LONG iImage);			*/


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

/* --- Module Prototype	Definitions -----------------------------------	*/

#pragma	subtitle("   Bitmap Effects - Flip colour Table Function")
#pragma	page( )

/* --- hbmFlipColourTable ------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	flip the colour	table and recreate	*/
/*     the bitmap.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     LONG iImage; = Image Index					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

HBITMAP	hbmFlipColourTable(LONG	iImage)

{
PBITMAPARRAYFILEHEADER	pbafh;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPARRAYFILEHEADER2	pbafh2;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPFILEHEADER	pbfh;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPFILEHEADER2	pbfh2;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPINFO		pbmi;	   /* Bitmap Info Pointer		*/
PBITMAPINFO2		pbmi2;	   /* Bitmap Info Pointer		*/
PRGB2			prgb2;	   /* RGB Array				*/
RGB			*prgb;	   /* RGB Array				*/
register INT i,	n;		   /* Loop Counter			*/

		       /* Check	to make	sure that the bitmap is	using	*/
		       /* a colour table since bitmaps with more than	*/
		       /* 256 colours use the actual bits of the image	*/
		       /* to describe the colour			*/

if ( abm[iImage].cColours <= 256L )
   {
		       /* Check	to see if the image is a OS/2 2.x	*/
		       /* format					*/
   if (	f20Bitmap )
       {
		       /* Check	to see if the bitmap is	an array and	*/
		       /* depending on type, use appropriate		*/
		       /* structures to	locate the RGB table		*/

       if ( (cBitmaps == 1) && !fBitmapArray )
	   {
	   pbfh2 = (PBITMAPFILEHEADER2)abm[iImage].pb;
	   pbmi2 = (PBITMAPINFO2)&pbfh2->bmp2;
	   }
       else
	   {
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)abm[iImage].pb;
	   pbmi2 = (PBITMAPINFO2)&pbafh2->bfh2.bmp2;
	   }
		       /* Allocate memory for the RGB table		*/

       prgb2 = (PRGB2)malloc(abm[iImage].cColours * sizeof(RGB2));

		       /* Transfer the RGB info	to the new table	*/
		       /* starting from	the top	of the table moving to	*/
		       /* the bottom					*/

       for ( i = (INT)abm[iImage].cColours - 1,	n = 0; i >= 0; i--, n++	)
	   prgb2[n] = pbmi2->argbColor[i];

		       /* Copy the flipped RGB table back into the	*/
		       /* bitmap data and release the temporary	table	*/
		       /* memory					*/

       memcpy(pbmi2->argbColor,	prgb2, abm[iImage].cColours * sizeof(RGB2));
       free(prgb2);
       }
   else
		       /* Image	a OS/2 1.x format, use the appropriate	*/
		       /* structures					*/
       {
		       /* Check	to see if the bitmap is	an array and	*/
		       /* depending on type, use appropriate		*/
		       /* structures to	locate the RGB table		*/

       if ( (cBitmaps == 1) && !fBitmapArray )
	   {
	   pbfh	= (PBITMAPFILEHEADER)abm[iImage].pb;
	   pbmi	= (PBITMAPINFO)&pbfh->bmp;
	   }
       else
	   {
	   pbafh = (PBITMAPARRAYFILEHEADER)abm[iImage].pb;
	   pbmi	= (PBITMAPINFO)&pbafh->bfh.bmp;
	   }
		       /* Allocate memory for the RGB table		*/

       prgb = (RGB *)malloc(abm[iImage].cColours * sizeof(RGB));

		       /* Transfer the RGB info	to the new table	*/
		       /* starting from	the top	of the table moving to	*/
		       /* the bottom					*/

       for ( i = (INT)abm[iImage].cColours - 1,	n = 0; i >= 0; i--, n++	)
	   prgb[n] = pbmi->argbColor[i];

		       /* Copy the flipped RGB table back into the	*/
		       /* bitmap data and release the temporary	table	*/
		       /* memory					*/

       memcpy(pbmi->argbColor, prgb, abm[iImage].cColours * sizeof(RGB));
       free(prgb);
       }
		       /* Refresh the bitmap inmage to use the new	*/
		       /* RGB colour table				*/

   return(hbmRefreshBitmap(iImage));
   }
else
   return(abm[iImage].hbm);
}
#pragma	subtitle("   Bitmap Effects - Bitmap Refresh Function")
#pragma	page( )

/* --- hbmRefreshBitmap	-------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	refresh	the bitmap after the colour	*/
/*     table has been revised.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     LONG iImage; = Image Index					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     hbmRefreshBitmap	= Bitmap Handle					*/
/*									*/
/* --------------------------------------------------------------------	*/

HBITMAP	hbmRefreshBitmap(LONG iImage)

{
HPS			hpsMem;	   /* Memory Presentation Space	Handle	*/
PBITMAPARRAYFILEHEADER	pbafh;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPARRAYFILEHEADER2	pbafh2;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPFILEHEADER	pbfh;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPFILEHEADER2	pbfh2;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPINFO2		pbmi2;	   /* Bitmap Info Pointer		*/
ULONG			offBits;   /* Bitmap Offset			*/

if ( abm[iImage].cColours <= 256L )
   {
		       /* Depending on the version of the bitmap	*/
		       /* structure, locate the	appropriate bitmap	*/
		       /* data for the revised image			*/
   if (	f20Bitmap )
       if ( (cBitmaps == 1) && !fBitmapArray )
	   {
	   pbfh2 = (PBITMAPFILEHEADER2)abm[iImage].pb;
	   pbmi2 = (PBITMAPINFO2)&pbfh2->bmp2;
	   offBits = pbfh2->offBits;
	   }
       else
	   {
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)abm[iImage].pb;
	   pbmi2 = (PBITMAPINFO2)&pbafh2->bfh2.bmp2;
	   offBits = pbafh2->bfh2.offBits;
	   }
   else
       if ( (cBitmaps == 1) && !fBitmapArray )
	   {
	   pbfh	= (PBITMAPFILEHEADER)abm[iImage].pb;
	   pbmi2 = (PBITMAPINFO2)&pbfh->bmp;
	   offBits = pbfh->offBits;
	   }
       else
	   {
	   pbafh = (PBITMAPARRAYFILEHEADER)abm[iImage].pb;
	   pbmi2 = (PBITMAPINFO2)&pbafh->bfh.bmp;
	   offBits = pbafh->bfh.offBits;
	   }
		       /* Destroy previous version of the bitmap	*/

   GpiDeleteBitmap(abm[iImage].hbm);

		       /* Point	to the independent bitmap file header	*/
		       /* since	it is the default			*/

   abm[iImage].hbm = GpiCreateBitmap(hpsMem = WinGetPS(HWND_DESKTOP), (PBITMAPINFOHEADER2)pbmi2, CBM_INIT,
				     (PBYTE)(abm[0].pb + offBits), pbmi2);

		       /* Release the memory presentation space		*/
   WinReleasePS(hpsMem);

   }
return(abm[iImage].hbm);
}

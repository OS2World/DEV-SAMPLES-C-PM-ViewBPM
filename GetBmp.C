#pragma	title("View Bitmap  --  Version 1  --  (GetBmp.C)")
#pragma	subtitle("   Main Client Window - Interface Definitions")

#define	INCL_BITMAPFILEFORMAT	   /* Include OS/2 Bitmap Information	*/
#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_GPI		   /* Include OS/2 GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <malloc.h>
#include <os2.h>
#include <string.h>

#include "appdefs.h"
#include "winbmp.h"


/* This	module contains	the functions to load a	requested .BMP file.	*/

/* Filename:   GetBmp.C							*/

/*  Version:   1							*/
/*  Created:   1995-07-05						*/
/*  Revised:   1995-07-22						*/

/* Routines:   static VOID BuildBitmapStack(				*/
/*				   PBITMAPARRAYFILEHEADER2 pbafh2);	*/
/*	       static VOID BuildBitmapStack1(				*/
/*				   PBITMAPARRAYFILEHEADER pbafh);	*/
/*	       static PBITMAPFILEHEADER2 pbfhSelectBfh(			*/
/*				       PBITMAPARRAYFILEHEADER2 pbafh2,	*/
/*				       ULONG cxDefault,			*/
/*				       ULONG cyDefault);		*/
/*	       static PBITMAPFILEHEADER	pbfhSelectBfh1(			*/
/*				       PBITMAPARRAYFILEHEADER pbafh,	*/
/*				       ULONG cxDefault,			*/
/*				       ULONG cyDefault);		*/
/*	       static HBITMAP hbmGetSingleBitmap(			*/
/*					     PBITMAPFILEHEADER2	pbfh2);	*/
/*	       static HBITMAP hbmGetSingleBitmap1(			*/
/*					       PBITMAPFILEHEADER pbfh);	*/
/*	       HBITMAP hbmGetBitmap(PSZ	pszFileName);			*/
/*	       static PBITMAPFILEHEADER2 pbfh2GetWindowsBmp(		*/
/*				   PBYTE pbData, ULONG cbData);		*/


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

static VOID BuildBitmapStack(PBITMAPARRAYFILEHEADER2 pbafh2);
static VOID BuildBitmapStack1(PBITMAPARRAYFILEHEADER pbafh);

static LONG lSelectBfh(PBITMAPARRAYFILEHEADER2 pbafh2);
static HBITMAP		  hbmGetSingleBitmap(PBITMAPFILEHEADER2	pbfh2);

static LONG lSelectBfh1(PBITMAPARRAYFILEHEADER pbafh);
static HBITMAP		  hbmGetSingleBitmap1(PBITMAPFILEHEADER	pbfh);

static PBITMAPFILEHEADER2 pbfh2GetWindowsBmp(PBYTE pbData, ULONG cbData);

#pragma	subtitle("   Bitmap Retrieve - Select Bitmap File Header Function")
#pragma	page( )

/* --- BuildBitmapStack	-------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	build the bitmap images	for a bitmap	*/
/*     array and place the bitmaps within the bitmap stack.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPARRAYFILEHEADER2 pbafh2; = Bitmap	Array Header Pointer	*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID BuildBitmapStack(PBITMAPARRAYFILEHEADER2 pbafh2)

{
PBITMAPARRAYFILEHEADER2	pbafhOrg;  /* Bitmap Array File	Header Origin	*/
PBITMAPFILEHEADER2	pbfh2;	   /* Bitmap Array File	Header Pointer	*/
HPS		   hpsMem;	   /* Memory Presentation Space	Handle	*/
register INT i;			   /* Loop Counter			*/

		       /* Delete any previously	loaded bitmaps		*/

for ( i	= 0; i < cBitmaps; i++ )
   GpiDeleteBitmap(abm[i].hbm);

cBitmaps = 0L;
hpsMem = WinGetPS(HWND_DESKTOP);

		       /* Save the start of the	bitmap file header	*/
		       /* since	all offsets are	from the beginning of	*/
		       /* the header					*/

abm[cBitmaps].pb = (PBYTE)(pbafhOrg = pbafh2);

		       /* Point	to the independent bitmap file header	*/
		       /* since	it is the default			*/
pbfh2 =	&pbafh2->bfh2;
abm[cBitmaps].cColours = (ULONG)(1 << pbfh2->bmp2.cBitCount);
abm[cBitmaps++].hbm = GpiCreateBitmap(hpsMem, &pbfh2->bmp2, CBM_INIT,
				      (PBYTE)((PBYTE)pbafh2 + pbafh2->bfh2.offBits),
				      (PBITMAPINFO2)(PVOID)&pbfh2->bmp2);

		       /* Scan the bitmap array	for the	desired	icon	*/
		       /* type based on	the current screen display	*/
while (	pbafh2->offNext	)
   {
		       /* Point	to the next array header		*/

   abm[cBitmaps].pb = (PBYTE)(pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbafhOrg + pbafh2->offNext));
   abm[cBitmaps].cColours = (ULONG)(1 << pbafh2->bfh2.bmp2.cBitCount);

		       /* Create a memory presentation space using the	*/
		       /* desktop window handle	and create the colour	*/
		       /* bitmap					*/

   abm[cBitmaps].hbm = GpiCreateBitmap(hpsMem, &pbafh2->bfh2.bmp2, CBM_INIT,
				       (PBYTE)((PBYTE)pbafhOrg + pbafh2->bfh2.offBits),
				       (PBITMAPINFO2)(PVOID)&pbafh2->bfh2.bmp2);
   if (	++cBitmaps == 32L )
       break;
   }
		       /* Release the memory presentation space		*/
WinReleasePS(hpsMem);
}
#pragma	subtitle("   Bitmap Retrieve - Select Bitmap File Header Function")
#pragma	page( )

/* --- BuildBitmapStack1 ------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	build the bitmap images	for a bitmap	*/
/*     array and place the bitmaps within the bitmap stack.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPARRAYFILEHEADER2 pbafh2; = Bitmap	Array Header Pointer	*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID BuildBitmapStack1(PBITMAPARRAYFILEHEADER pbafh)

{
PBITMAPARRAYFILEHEADER pbafhOrg;   /* Bitmap Array File	Header Origin	*/
PBITMAPFILEHEADER      pbfh;	   /* Bitmap Array File	Header Pointer	*/
HPS		       hpsMem;	   /* Memory Presentation Space	Handle	*/
register INT i;			   /* Loop Counter			*/

		       /* Delete any previously	loaded bitmaps		*/

for ( i	= 0; i < cBitmaps; i++ )
   GpiDeleteBitmap(abm[i].hbm);

cBitmaps = 0L;
hpsMem = WinGetPS(HWND_DESKTOP);

		       /* Save the start of the	bitmap file header	*/
		       /* since	all offsets are	from the beginning of	*/
		       /* the header					*/

abm[cBitmaps].pb    = (PBYTE)(pbafhOrg = pbafh);

		       /* Point	to the independent bitmap file header	*/
		       /* since	it is the default			*/
pbfh = &pbafh->bfh;
abm[cBitmaps].cColours = (ULONG)(1 << pbfh->bmp.cBitCount);
abm[cBitmaps++].hbm = GpiCreateBitmap(hpsMem, (PBITMAPINFOHEADER2)&pbfh->bmp, CBM_INIT,
				      (PBYTE)((PBYTE)pbafh + pbafh->bfh.offBits),
				      (PBITMAPINFO2)(PVOID)&pbfh->bmp);

		       /* Scan the bitmap array	for the	desired	icon	*/
		       /* type based on	the current screen display	*/
while (	pbafh->offNext )
   {
		       /* Point	to the next array header		*/

   abm[cBitmaps].pb = (PBYTE)(pbafh = (PBITMAPARRAYFILEHEADER)((PBYTE)pbafhOrg + pbafh->offNext));
   abm[cBitmaps].cColours = (ULONG)(1 << pbafh->bfh.bmp.cBitCount);

		       /* Create a memory presentation space using the	*/
		       /* desktop window handle	and create the colour	*/
		       /* bitmap					*/

   abm[cBitmaps].hbm = GpiCreateBitmap(hpsMem, (PBITMAPINFOHEADER2)&pbafh->bfh.bmp, CBM_INIT,
				       (PBYTE)((PBYTE)pbafhOrg + pbafh->bfh.offBits),
				       (PBITMAPINFO2)(PVOID)&pbafh->bfh.bmp);
   if (	++cBitmaps == 32L )
       break;
   }
		       /* Release the memory presentation space		*/
WinReleasePS(hpsMem);
}
#pragma	subtitle("   Bitmap Retrieve - Select Bitmap File Header Function")
#pragma	page( )

/* --- lSelectBfh -------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	move through the bitmap	array file	*/
/*     header passed to	it to locate the appropriate bitmap file	*/
/*     header.	Two methods can	be used	to locate the bitmap:  use	*/
/*     the cxDisplay and cyDisplay to match the	screen size; or	to	*/
/*     use the independent form	which located first in the file.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPARRAYFILEHEADER2 pbafh2; = Bitmap	Array Header Pointer	*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     lSelectBfh = Selected Bitmap Index				*/
/*									*/
/* --------------------------------------------------------------------	*/

static LONG lSelectBfh(PBITMAPARRAYFILEHEADER2 pbafh2)

{
PBITMAPARRAYFILEHEADER2	pbafhOrg;  /* Bitmap Array File	Header Origin	*/
USHORT			cxDisplay; /* Bitmap Screen Design Width	*/
USHORT			cyDisplay; /* Bitmap Screen Design Height	*/
USHORT			cxTarget;  /* Bitmap Screen Design Width	*/
USHORT			cyTarget;  /* Bitmap Screen Design Height	*/

		       /* Check	to make	sure that a bitmap array has	*/
		       /* been read in					*/

if ( pbafh2->usType != BFT_BITMAPARRAY )

		       /* Not a	bitmap array, return the beginning of	*/
		       /* header to ensure proper decoding		*/
   return(iBitmap);
else
   {
   cxDisplay = cyDisplay = (USHORT)0;
   if (	(cxScreen >= 1024L) && (cxScreen >= 768L) )
       {
       cxTarget	= (USHORT)1024;
       cyTarget	= (USHORT)768;
       }
   else
       {
       cxTarget	= (USHORT)640;
       cyTarget	= (USHORT)480;
       }
   }
		       /* Save the start of the	bitmap file header	*/
		       /* since	all offsets are	from the beginning of	*/
		       /* the header					*/
pbafhOrg = pbafh2;
		       /* Check	to see if the current entry's size      */
		       /* matches that of the current screen		*/

if ( (pbafh2->cxDisplay	== cxTarget) &&	(pbafh2->cyDisplay == cyTarget)	)
   return(iBitmap);
else
   if (	(pbafh2->cxDisplay > cxDisplay)	&& (pbafh2->cyDisplay >	cyDisplay) )
       {
       cxDisplay = pbafh2->cxDisplay;
       cyDisplay = pbafh2->cyDisplay;
       }
		       /* Scan the bitmap array	for the	desired	icon	*/
		       /* type based on	the current screen display	*/
while (	pbafh2->offNext	)
   {
		       /* Point	to the next array header		*/

   pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbafhOrg +	pbafh2->offNext);
   ++iBitmap;
		       /* Check	to see if the current entry's size      */
		       /* matches that of the current screen		*/

   if (	(pbafh2->cxDisplay == cxTarget)	&& (pbafh2->cyDisplay == cyTarget) )
       return(iBitmap);
   else
       if ( (pbafh2->cxDisplay > cxDisplay) && (pbafh2->cyDisplay > cyDisplay) )
	   {
	   cxDisplay = pbafh2->cxDisplay;
	   cyDisplay = pbafh2->cyDisplay;
	   }
   }
		       /* Return the address to	the desired bitmap	*/
		       /* array	file header				*/
return(iBitmap);
}
#pragma	subtitle("   Bitmap Retrieve - Select Bitmap File Header Function")
#pragma	page( )

/* --- lSelectBfh1 ------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	move through the bitmap	array file	*/
/*     header passed to	it to locate the appropriate bitmap file	*/
/*     header.	Two methods can	be used	to locate the bitmap:  use	*/
/*     the cxDisplay and cyDisplay to match the	screen size; or	to	*/
/*     use the independent form	which located first in the file.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPARRAYFILEHEADER pbafh; = Bitmap Array File Pointer	*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     lSelectBfh1 = Selected Bitmap Index				*/
/*									*/
/* --------------------------------------------------------------------	*/

static LONG lSelectBfh1(PBITMAPARRAYFILEHEADER pbafh)

{
PBITMAPARRAYFILEHEADER pbafhOrg;   /* Bitmap Array File	Header Origin	*/
USHORT		       cxDisplay;  /* Bitmap Screen Design Width	*/
USHORT		       cyDisplay;  /* Bitmap Screen Design Height	*/
USHORT		       cxTarget;   /* Bitmap Screen Design Width	*/
USHORT		       cyTarget;   /* Bitmap Screen Design Height	*/

		       /* Check	to make	sure that a bitmap array has	*/
		       /* been read in					*/

if ( pbafh->usType != BFT_BITMAPARRAY )

		       /* Not a	bitmap array, return the beginning of	*/
		       /* header to ensure proper decoding		*/
   return(iBitmap);
else
   {
   cxDisplay = cyDisplay = (USHORT)0;
   if (	(cxScreen >= 1024L) && (cxScreen >= 768L) )
       {
       cxTarget	= (USHORT)1024;
       cyTarget	= (USHORT)768;
       }
   else
       {
       cxTarget	= (USHORT)640;
       cyTarget	= (USHORT)480;
       }
   }
		       /* Save the start of the	bitmap file header	*/
		       /* since	all offsets are	from the beginning of	*/
		       /* the header					*/
pbafhOrg = pbafh;
		       /* Check	to see if the current entry's size      */
		       /* matches that of the current screen		*/

if ( (pbafh->cxDisplay == cxTarget) && (pbafh->cyDisplay == cyTarget) )
   return(iBitmap);
else
   if (	(pbafh->cxDisplay > cxDisplay) && (pbafh->cyDisplay > cyDisplay) )
       {
       cxDisplay = pbafh->cxDisplay;
       cyDisplay = pbafh->cyDisplay;
       }
		       /* Scan the bitmap array	for the	desired	icon	*/
		       /* type based on	the current screen display	*/
while (	pbafh->offNext )
   {
		       /* Point	to the next array header		*/

   pbafh = (PBITMAPARRAYFILEHEADER)((PBYTE)pbafhOrg + pbafh->offNext);
   ++iBitmap;
		       /* Check	to see if the current entry's size      */
		       /* matches that of the current screen		*/

   if (	(pbafh->cxDisplay == cxTarget) && (pbafh->cyDisplay == cyTarget) )
       return(iBitmap);
   else
       if ( (pbafh->cxDisplay >	cxDisplay) && (pbafh->cyDisplay	> cyDisplay) )
	   {
	   cxDisplay = pbafh->cxDisplay;
	   cyDisplay = pbafh->cyDisplay;
	   }
   }
		       /* Return the address to	the desired bitmap	*/
		       /* array	file header				*/
return(iBitmap);
}
#pragma	subtitle("   Bitmap Retrieve - Retrieve Single Bitmap Function")
#pragma	page( )

/* --- hbmGetSingleBitmap -----------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	load a single bitmap from a bitmap	*/
/*     file.								*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPFILEHEADER pbafh;	= Bitmap File Header Pointer		*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     hbmGetSingleBitmap = Bitmap Handle				*/
/*									*/
/* --------------------------------------------------------------------	*/

static HBITMAP hbmGetSingleBitmap(PBITMAPFILEHEADER2 pbfh2)

{
HPS	hPS;			   /* Presentation Space		*/
register INT i;			   /* Loop Counter			*/

		       /* Check	to see that the	size of	the bitmap	*/
		       /* info header is correct otherwise the bitmap	*/
		       /* cannot be created				*/

if ( pbfh2->bmp2.cbFix != sizeof(BITMAPINFOHEADER2) )

		       /* Invalid header size, return NULL bitmap	*/
		       /* handle					*/
   return((HBITMAP)NULL);
		       /* Delete any previously	loaded bitmaps		*/

for ( i	= 0; i < cBitmaps; i++ )
   GpiDeleteBitmap(abm[i].hbm);

		       /* Save the start of the	bitmap file header	*/
		       /* since	all offsets are	from the beginning of	*/
		       /* the header					*/
cBitmaps = 1L;
abm[0].pb = (PBYTE)pbfh2;
abm[0].cColours	= (ULONG)(1 << pbfh2->bmp2.cBitCount);

		       /* Point	to the independent bitmap file header	*/
		       /* since	it is the default			*/

abm[0].hbm = GpiCreateBitmap(hPS = WinGetPS(HWND_DESKTOP),
			     &(pbfh2->bmp2), CBM_INIT, (PBYTE)pbfh2 + pbfh2->offBits,
			     (PBITMAPINFO2)(PVOID)&pbfh2->bmp2);

		       /* Release the desktop presentation space	*/
WinReleasePS(hPS);
		       /* Return the bitmap handle			*/
return(abm[0].hbm);

}
#pragma	subtitle("   Bitmap Retrieve - Retrieve Single Bitmap Function")
#pragma	page( )

/* --- hbmGetSingleBitmap1 ----------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	load a single bitmap from a bitmap	*/
/*     file.								*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPFILEHEADER pbafh;	= Bitmap File Header Pointer		*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     hbmGetSingleBitmap1 = Bitmap Handle				*/
/*									*/
/* --------------------------------------------------------------------	*/

static HBITMAP hbmGetSingleBitmap1(PBITMAPFILEHEADER pbfh)

{
HPS	hPS;			   /* Presentation Space		*/
register INT i;			   /* Loop Counter			*/

		       /* Check	to see that the	size of	the bitmap	*/
		       /* info header is correct otherwise the bitmap	*/
		       /* cannot be created				*/

if ( pbfh->bmp.cbFix !=	sizeof(BITMAPINFOHEADER) )

		       /* Invalid header size, return NULL bitmap	*/
		       /* handle					*/
   return((HBITMAP)NULL);
		       /* Delete any previously	loaded bitmaps		*/

for ( i	= 0; i < cBitmaps; i++ )
   GpiDeleteBitmap(abm[i].hbm);

		       /* Save the start of the	bitmap file header	*/
		       /* since	all offsets are	from the beginning of	*/
		       /* the header					*/
cBitmaps = 1L;
abm[0].pb = (PBYTE)pbfh;
abm[0].cColours	= (ULONG)(1 << pbfh->bmp.cBitCount);

		       /* Get the desktop presentation space to	create	*/
		       /* the bitmap					*/

abm[0].hbm = GpiCreateBitmap(hPS = WinGetPS(HWND_DESKTOP),
			     (PBITMAPINFOHEADER2)(PVOID)&pbfh->bmp,
			     CBM_INIT, (BYTE *)pbfh + pbfh->offBits, (PBITMAPINFO2)(PVOID)&pbfh->bmp);

		       /* Release the desktop presentation space	*/
WinReleasePS(hPS);
		       /* Return the bitmap handle			*/
return(abm[0].hbm);
}
#pragma	subtitle("   Bitmap Retrieve - Bitmap Retrieve Function")
#pragma	page( )

/* --- hbmGetBitmap ------------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	load the requested bitmap file and to	*/
/*     create a	bitmap that can	be displayed through Prominare		*/
/*     Designer.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PSZ pszFileName;	= Bitmap Filename				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     hbmGetBitmap = Bitmap Handle					*/
/*									*/
/* --------------------------------------------------------------------	*/

HBITMAP	hbmGetBitmap(PSZ pszFileName)

{
FILESTATUS3	       fs;	   /* File Status Information		*/
HFILE		       hFile;	   /* File Handle			*/
HBITMAP		       hbm   = 0;  /* Bitmap Handle			*/
PBITMAPARRAYFILEHEADER pbafh = 0;  /* Bitmap File Header Pointer	*/
ULONG		       cbRead;	   /* Bytes Read			*/
ULONG		       ulResult;   /* File Open	Fail Result		*/

		       /* Open the requested bitmap file		*/

if ( DosOpen(pszFileName, &hFile, &ulResult, 0UL, FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
	     OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE, NULL) )

		       /* Could	not open the file, return NULL bitmap	*/
		       /* pointer handle				*/
   return((HBITMAP)NULL);
		       /* Get the file information to allow the	proper	*/
		       /* amount of memory to be allocated for the	*/
		       /* bitmap information				*/

DosQueryFileInfo(hFile,	FIL_STANDARD, (PVOID)&fs, sizeof(FILESTATUS3));

		       /* Allocate memory for the bitmap information	*/

if ( (pbafh = (PBITMAPARRAYFILEHEADER)malloc(fs.cbFile)) == NULL )

		       /* Memory allocation failed, return NULL	bitmap	*/
		       /* pointer handle				*/
   return((HBITMAP)NULL);
		       /* Read into memory the bitmap file		*/

if ( DosRead(hFile, (PVOID)pbafh, fs.cbFile, (PULONG)&cbRead) || (cbRead != fs.cbFile) )
   {
		       /* Release the memory allocated for the bitmap	*/
		       /* information					*/
   free(pbafh);
		       /* Close	the bitmap file				*/
   DosClose(hFile);
		       /* Error	occurred in reading the	bitmap file,	*/
		       /* return NULL bitmap handle			*/
   return((HBITMAP)NULL);
   }
else
		       /* Close	the bitmap file				*/
   DosClose(hFile);

iBitmap	= 0L;

if ( (pbafh->cbSize == sizeof(BITMAPARRAYFILEHEADER)) || (pbafh->cbSize	== sizeof(BITMAPFILEHEADER)) )
   {
   fWindowsBitmap = f20Bitmap =	fBitmapArray = FALSE;

		       /* Check	to see if the bitmap is	monochrome	*/

   if (	pbafh->usType == BFT_BMAP )

		       /* Monochrome bitmap, convert into a memory	*/
		       /* bitmap and a valid bitmap handle		*/

       hbm = hbmGetSingleBitmap1((PBITMAPFILEHEADER)(PVOID)pbafh);
   else
		       /* Check	to see if the icon is a	bitmap array	*/
		       /* thereby indicating a coloured	bitmap		*/

       if ( pbafh->usType == BFT_BITMAPARRAY )
	   {
		       /* Coloured bitmap, convert into	a memory bitmap	*/
		       /* bitmap and a valid bitmap handle		*/

	   BuildBitmapStack1((PBITMAPARRAYFILEHEADER)pbafh);
	   hbm = abm[lSelectBfh1((PBITMAPARRAYFILEHEADER)pbafh)].hbm;
	   fBitmapArray	= TRUE;
	   }
   }
else
   if (	(pbafh->cbSize == sizeof(BITMAPARRAYFILEHEADER2)) || (pbafh->cbSize == sizeof(BITMAPFILEHEADER2)) )
       {
       f20Bitmap = TRUE;
       fWindowsBitmap =	fBitmapArray = FALSE;

		       /* Check	to see if the bitmap is	monochrome	*/

       if ( pbafh->usType == BFT_BMAP )

		       /* Monochrome bitmap, convert into a memory	*/
		       /* bitmap and a valid bitmap handle		*/

	   hbm = hbmGetSingleBitmap((PBITMAPFILEHEADER2)(PVOID)pbafh);
       else
		       /* Check	to see if the icon is a	bitmap array	*/
		       /* thereby indicating a coloured	bitmap		*/

	   if (	pbafh->usType == BFT_BITMAPARRAY )
	       {
		       /* Coloured bitmap, convert into	a memory bitmap	*/
		       /* bitmap and a valid bitmap handle		*/

	       BuildBitmapStack((PBITMAPARRAYFILEHEADER2)(PVOID)pbafh);
	       hbm = abm[lSelectBfh((PBITMAPARRAYFILEHEADER2)(PVOID)pbafh)].hbm;
	       fBitmapArray = TRUE;
	       }
       }
   else
		       /* Appears to be	a Windows bitmap, try to	*/
		       /* convert it to	an OS/2	2.x format bitmap	*/
       {
       fBitmapArray = FALSE;
       hbm = hbmGetSingleBitmap((PBITMAPFILEHEADER2)(pbafh = (PBITMAPARRAYFILEHEADER)pbfh2GetWindowsBmp((PBYTE)pbafh, fs.cbFile)));
       }

if ( pb	)
   free(pb);
pb = (PBYTE)pbafh;
		       /* Return the bitmap handle			*/
return(hbm);

}
#pragma	subtitle("   Bitmap Retrieve - Windows Bitmap Image Retrieve Function")
#pragma	page( )

/* --- pbfh2GetWindowsBmp ------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	convert	a Windows bitmap to an OS/2	*/
/*     2.x type	of bitmap.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBYTE pbBitmap; = Pointer to Windows Bitmap Data			*/
/*     ULONG cbData;   = Windows Bitmap	Data Size			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     pbfh2GetWindowsBmp = Convert Bitmap Data	Pointer			*/
/*									*/
/* --------------------------------------------------------------------	*/

static PBITMAPFILEHEADER2 pbfh2GetWindowsBmp(PBYTE pbBitmap, ULONG cbData)

{
PBITMAPFILEHEADER2   pbfh2;	   /* Bitmap File Header Pointer	*/
PULONG		     pul;	   /* Value Pointer			*/
PwinRGBQUAD	     pargbq;	   /* RGB Quad Array Pointer		*/
PwinRGBTRIPLE	     pargbt;	   /* RGB Triple Array Pointer		*/
ULONG		     cClrs;	   /* Colours Table Count		*/
ULONG		     cb;	   /* Image Size			*/
winBITMAPCOREHEADER  bmpc;	   /* Bitmap Information Header		*/
winBITMAPINFOHEADER  bmpi;	   /* Bitmap Information Header		*/
PwinBITMAPFILEHEADER pwbfh;	   /* Bitmap File Header		*/
PBYTE		     pbImage;	   /* Image Pointer			*/
PBYTE		     pbClrTable;   /* Colour Table Pointer		*/
register INT i;			   /* Loop Counter			*/

pwbfh =	(PwinBITMAPFILEHEADER)pbBitmap;

if ( (pwbfh->bfType == BFT_BMAP) || (pwbfh->bfSize == cbData) )
   {
		       /* Check	to see if the bitmap is	a variation of	*/
		       /* the bitmap layouts in	which case, transfer	*/
		       /* the info and point to	the RGB	colour table	*/

   if (	pwbfh->bmpi.biSize == sizeof(winBITMAPCOREHEADER) )
       {
       memset(&bmpi, 0,	sizeof(winBITMAPINFOHEADER));
       memcpy(&bmpc, &pwbfh->bmpi, sizeof(winBITMAPCOREHEADER));
       bmpi.biWidth    = bmpc.bcWidth;
       bmpi.biHeight   = bmpc.bcHeight;
       bmpi.biPlanes   = bmpc.bcPlanes;
       bmpi.biBitCount = bmpc.bcBitCount;
       pbClrTable = pbBitmap + (sizeof(winBITMAPFILEHEADER) - sizeof(winBITMAPINFOHEADER)) + sizeof(winBITMAPCOREHEADER);
       }
   else
		       /* Standard bitmap format, transfer the info and	*/
		       /* point	to the colour table			*/
       {
       memcpy(&bmpi, &pwbfh->bmpi, sizeof(winBITMAPINFOHEADER));
       pbClrTable = pbBitmap + sizeof(winBITMAPFILEHEADER);
       }
		       /* Point	to the image data of the bitmap		*/

   pbImage = pbBitmap +	pwbfh->bfOffBits;

		       /* Get the colours used count and when zero	*/
		       /* the colours are defined to be	the maximum	*/
		       /* values for the bit count			*/

   if (	(cClrs = bmpi.biClrUsed) == 0 )
       switch (	bmpi.biBitCount	)
	   {
		       /* 2 colour bitmap				*/
	   case	1 :
	       cClrs = 2UL;
	       break;
		       /* 16 colour bitmap				*/
	   case	4 :
	       cClrs = 16UL;
	       break;
		       /* 256 colour bitmap				*/
	   case	8 :
	       cClrs = 256UL;
	       break;
		       /* 16 MB	colour bitmap, no colour table defined	*/
	   default :
	       cClrs = 0UL;
	       break;
	   }
		       /* When colour table defined for	bitmap,		*/
		       /* allocated enough memory for it and then read	*/
		       /* it in	from disk				*/
   if (	cClrs )
		       /* Check	to see if the bitmap is	a variant in	*/
		       /* which	case the RGB colours are like the OS/2	*/
		       /* 1.x colours, 3 bytes in size,	therefore need	*/
		       /* to be	transferred individually		*/

       if ( pwbfh->bmpi.biSize == sizeof(winBITMAPCOREHEADER) )
	   {
	   memset(pargbq = (winRGBQUAD *)malloc(sizeof(winRGBQUAD) * cClrs),
		  0, sizeof(winRGBQUAD)	* cClrs);
	   pargbt = (PwinRGBTRIPLE)pbClrTable;
	   for ( i = 0;	i < cClrs; i++ )
	       {
	       pargbq[i].rgbBlue  = pargbt[i].rgbBlue;
	       pargbq[i].rgbGreen = pargbt[i].rgbGreen;
	       pargbq[i].rgbRed	  = pargbt[i].rgbRed;
	       }
	   }
       else
		       /* Standard colour table, transfer the info	*/

	   memcpy(pargbq = (winRGBQUAD *)malloc(sizeof(winRGBQUAD) * cClrs),
		  pbClrTable, sizeof(winRGBQUAD) * cClrs);
   else
		       /* No colour table				*/

       pargbq =	(PwinRGBQUAD)NULL;

   if (	bmpi.biSizeImage )
       cb = bmpi.biSizeImage;
   else
       if ( pwbfh->bmpi.biSize == sizeof(winBITMAPCOREHEADER) )
	   cb =	(ULONG)(cbData - (sizeof(winBITMAPFILEHEADER) -	sizeof(winBITMAPINFOHEADER)) - (sizeof(winRGBTRIPLE) * cClrs));
       else
	   cb =	(ULONG)(cbData - sizeof(winBITMAPFILEHEADER) - (sizeof(winRGBQUAD) * cClrs));
   }
else
   {

   pul = (PULONG)pbBitmap;

   if (	*pul ==	sizeof(winBITMAPCOREHEADER) )
       {
		       /* Read the bitmap file info header from	the	*/
		       /* bitmap					*/

       memcpy(&bmpc, pbBitmap, sizeof(winBITMAPCOREHEADER));
       pbImage = pbBitmap + sizeof(winBITMAPCOREHEADER);
       memset(&bmpi, 0,	sizeof(winBITMAPINFOHEADER));
       bmpi.biWidth    = bmpc.bcWidth;
       bmpi.biHeight   = bmpc.bcHeight;
       bmpi.biPlanes   = bmpc.bcPlanes;
       bmpi.biBitCount = bmpc.bcBitCount;
       }
   else
		       /* Read the bitmap file info header from	the	*/
		       /* bitmap					*/
       {
       memcpy(&bmpi, pbBitmap, sizeof(winBITMAPINFOHEADER));
       pbImage = pbBitmap + sizeof(winBITMAPINFOHEADER);
       }
		       /* Get the colours used count and when zero	*/
		       /* the colours are defined to be	the maximum	*/
		       /* values for the bit count			*/

   if (	(cClrs = bmpi.biClrUsed) == 0 )
       switch (	bmpi.biBitCount	)
	   {
		       /* 2 colour bitmap				*/
	   case	1 :
	       cClrs = 2UL;
	       break;
		       /* 16 colour bitmap				*/
	   case	4 :
	       cClrs = 16UL;
	       break;
		       /* 256 colour bitmap				*/
	   case	8 :
	       cClrs = 256UL;
	       break;
		       /* 16 MB	colour bitmap, no colour table defined	*/
	   default :
	       cClrs = 0UL;
	       break;
	   }
		       /* When colour table defined for	bitmap,		*/
		       /* allocated enough memory for it and then read	*/
		       /* it in	from disk				*/
   if (	cClrs )
       {
       memcpy(pargbq = (winRGBQUAD *)malloc(sizeof(winRGBQUAD) * cClrs),
	      pbBitmap,	sizeof(winRGBQUAD) * cClrs);
       pbImage = pbImage + (sizeof(winRGBQUAD) * cClrs);
       }
   else
       pargbq =	(PwinRGBQUAD)NULL;

   cb =	(ULONG)(cbData - sizeof(winBITMAPINFOHEADER) - (sizeof(winRGBQUAD) * cClrs));
   }
		       /* Initialize the bitmap	array header		*/

memset(pbfh2 = (PBITMAPFILEHEADER2)malloc(sizeof(BITMAPFILEHEADER2) + cClrs * sizeof(RGB2) + cb),
       0, sizeof(BITMAPFILEHEADER2));

		       /* Initialize the first bitmap header for the	*/
		       /* bitmap					*/

pbfh2->usType	= (USHORT)BFT_BMAP;
pbfh2->cbSize	= sizeof(BITMAPFILEHEADER2);
pbfh2->xHotspot	= (SHORT)(bmpi.biWidth	/ 2);
pbfh2->yHotspot	= (SHORT)(bmpi.biHeight	/ 2);
pbfh2->offBits	= sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * cClrs;

		       /* Initialize the bitmap	information for	the	*/
		       /* bitmap based on the values from the Windows	*/
		       /* 3.1 bitmap					*/

pbfh2->bmp2.cbFix	  = sizeof(BITMAPINFOHEADER2);
pbfh2->bmp2.cx		  = (ULONG)bmpi.biWidth;
pbfh2->bmp2.cy		  = (ULONG)bmpi.biHeight;
pbfh2->bmp2.cPlanes	  =	   bmpi.biPlanes;
pbfh2->bmp2.cBitCount	  =	   bmpi.biBitCount;
pbfh2->bmp2.cbImage	  =	   cb;
pbfh2->bmp2.cclrUsed	  = cClrs;
pbfh2->bmp2.cclrImportant = cClrs;

		       /* When a colour	table present, write out the	*/
		       /* colour table					*/
if ( cClrs )
   {
   memcpy((PBYTE)pbfh2 + sizeof(BITMAPFILEHEADER2), pargbq, sizeof(winRGBQUAD) * cClrs);
   memcpy((PBYTE)pbfh2 + sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * cClrs, pbImage, cb);
   }
else
   memcpy((PBYTE)pbfh2 + sizeof(BITMAPFILEHEADER2), pbImage, cb);

		       /* If a colour table was	present, release the	*/
		       /* memory allocated for it			*/
if ( cClrs )
   free(pargbq);

fWindowsBitmap = f20Bitmap = TRUE;

free(pbBitmap);

return(pbfh2);
}

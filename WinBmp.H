
/* winbmp.h	Created:    1995-07-09 Revised:				*/

/* Windows Bitmaps Definition Header					*/

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

/* This	WIDTHBYTES macro determines the	number of BYTES	per scan line	*/
#define	 WIDTHBYTES(i)	 ((i+31)/32*4)

#pragma	pack(1)

/************************************************************************/
/*** Note ***************************************************************/
/************************************************************************/
/*									*/
/* The following structures are	defined	for Microsoft Windows 3.1	*/
/* bitmaps, icons and pointers (known as cursors in Windows).  The	*/
/* structures are derived from Microsoft Windows headers when		*/
/* available otherwise,	they are derived from the Microsoft Windows 3.1	*/
/* Programmers Reference Volume	4, Section 1.  The description of the	*/
/* different types is defined within this section.  It should be noted	*/
/* that	there is very little difference	in the actual bitmap data	*/
/* between Windows 3.1 and OS/2	2.x.  The major	differences lie	within	*/
/* the structure headers and some masks.				*/
/*									*/
/************************************************************************/
/************************************************************************/


/* --- Bitmap Definitions ---------------------------------------------	*/

typedef	struct _winBITMAPINFOHEADER/* Bitmap Information Header		*/
   {				   /* Size:    40 Bytes			*/
   ULONG       biSize;		   /* Structure	Size			*/
   LONG	       biWidth;		   /* Bitmap Width in Pixels		*/
   LONG	       biHeight;	   /* Bitmap Height in Pixels		*/
   USHORT      biPlanes;	   /* Device Planes (Must be 1)		*/
   USHORT      biBitCount;	   /* Bits per Pixel (1, 4, 8 or 24)	*/
   ULONG       biCompression;	   /* Compression Type			*/
   ULONG       biSizeImage;	   /* Image Size in Bytes		*/
   LONG	       biXPelsPerMeter;	   /* Horizontal Resolution		*/
   LONG	       biYPelsPerMeter;	   /* Vertical Resolution		*/
   ULONG       biClrUsed;	   /* Colours Used in Bitmap		*/
   ULONG       biClrImportant;	   /* Colours Important	for Display	*/
   } winBITMAPINFOHEADER;

typedef	winBITMAPINFOHEADER *PwinBITMAPINFOHEADER;

/* constants for the biCompression field */
#define	BI_RGB	    0L
#define	BI_RLE8	    1L
#define	BI_RLE4	    2L

typedef	struct _winRGBQUAD	   /* RGB Colour Definition		*/
   {				   /* Size:	4 Bytes			*/
   BYTE	       rgbBlue;		   /* Blue Element			*/
   BYTE	       rgbGreen;	   /* Green Element			*/
   BYTE	       rgbRed;		   /* Reg Element			*/
   BYTE	       rgbReserved;	   /* Reserved				*/
   } winRGBQUAD;

typedef	winRGBQUAD *PwinRGBQUAD;

typedef	struct _winRGBTRIPLE	   /* RGB Colour Definition		*/
   {				   /* Size:	3 Bytes			*/
   BYTE	       rgbBlue;		   /* Blue Element			*/
   BYTE	       rgbGreen;	   /* Green Element			*/
   BYTE	       rgbRed;		   /* Reg Element			*/
   } winRGBTRIPLE;

typedef	winRGBTRIPLE *PwinRGBTRIPLE;

typedef	struct _winBITMAPFILEHEADER/* Bitamp File Header		*/
   {				   /* Size:    54 Bytes			*/
   USHORT	       bfType;	   /* Specifies	Type of	File		*/
   ULONG	       bfSize;	   /* File Size	in Bytes		*/
   USHORT	       bfReserved1;/* Reserved Must Be Zero		*/
   USHORT	       bfReserved2;/* Reserved Must Be Zero		*/
   ULONG	       bfOffBits;  /* Offset to	Data from Start	of File	*/
   winBITMAPINFOHEADER bmpi;	   /* Bitmap Information		*/
   } winBITMAPFILEHEADER;

typedef	winBITMAPFILEHEADER *PwinBITMAPFILEHEADER;

typedef	struct _winBITMAPCOREHEADER/* Bitmap Information Header		*/
   {				   /* Size:    12 Bytes			*/
   ULONG       bcSize;		   /* Structure	Size			*/
   SHORT       bcWidth;		   /* Bitmap Width in Pixels		*/
   SHORT       bcHeight;	   /* Bitmap Height in Pixels		*/
   USHORT      bcPlanes;	   /* Device Planes (Must be 1)		*/
   USHORT      bcBitCount;	   /* Bits per Pixel (1, 4, 8 or 24)	*/
   } winBITMAPCOREHEADER;

typedef	winBITMAPCOREHEADER *PwinBITMAPCOREHEADER;

typedef	struct _winBITMAPCOREINFO  /* Bitmap Information Header		*/
   {				   /* Size:    15 Bytes			*/
   ULONG	bcSize;		   /* Structure	Size			*/
   SHORT	bcWidth;	   /* Bitmap Width in Pixels		*/
   SHORT	bcHeight;	   /* Bitmap Height in Pixels		*/
   USHORT	bcPlanes;	   /* Device Planes (Must be 1)		*/
   USHORT	bcBitCount;	   /* Bits per Pixel (1, 4, 8 or 24)	*/
   winRGBTRIPLE	bmciColours[1];	   /* Bitmap Core Info Colour Table	*/
   } winBITMAPCOREINO;

typedef	winBITMAPCOREINO *PwinBITMAPCOREINO;

#pragma	pack( )

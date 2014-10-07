
/* viewbmp.h	    Created:	1995-07-05  Revised:  1995-07-06	*/

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

#define	WIN_BITMAPVIEWER 1

#define	IDM_FILE 10
#define	IDM_OPEN 11
#define	IDM_SCALE 20
#define	IDM_25PERCENT 21
#define	IDM_50PERCENT 22
#define	IDM_75PERCENT 23
#define	IDM_100PERCENT 24
#define	IDM_200PERCENT 25
#define	IDM_400PERCENT 26
#define	IDM_800PERCENT 27
#define	IDM_SHOWALL    28
#define	IDM_EFFECTS 30
#define	IDM_FLIPCOLOURTABLE 31

#define	DLG_OPENBITMAP	   1150
#define	IT_DIRECTORY	   1151
#define	IT_VOLUMELABEL	   1152
#define	IT_DISKFREE	   1153
#define	IT_FILECOUNT	   1154
#define	CBX_DRIVES	   1155
#define	LB_DIRECTORIES	   1156
#define	LB_FILES	   1157

#define	IDM_POPUP		  512
#define	IDM_POPUPSINGLE		  513
#define	IDM_BITMAPARRAYFILEHEADER 514
#define	IDM_BITMAPFILEHEADER	  515
#define	IDM_BITMAPINFOHEADER	  516
#define	IDM_COLOURTABLE		  517

#define	DLG_VIEWBITMAPARRAYFILEHEADER  2048
#define	DLG_VIEWBITMAPFILEHEADER       2049
#define	DLG_VIEWBITMAPINFOHEADER       2050
#define	DLG_VIEWBITMAPARRAYFILEHEADER2 2051
#define	DLG_VIEWBITMAPFILEHEADER2      2052
#define	DLG_VIEWBITMAPINFOHEADER2      2053

#define	IT_BITMAPINDEX		   1024
#define	IT_USTYPE		   1025
#define	IT_CBSIZE		   1026
#define	IT_OFFNEXT		   1027
#define	IT_CXDISPLAY		   1028
#define	IT_CYDISPLAY		   1029
#define	IT_XHOTSPOT		   1030
#define	IT_YHOTSPOT		   1031
#define	IT_OFFBITS		   1032
#define	IT_CBFIX		   1033
#define	IT_CX			   1034
#define	IT_CY			   1035
#define	IT_CPLANES		   1036
#define	IT_CBITCOUNT		   1037
#define	IT_ULCOMPRESSION	   1038
#define	IT_CBIMAGE		   1039
#define	IT_CXRESOLUTION		   1040
#define	IT_CYRESOLUTION		   1041
#define	IT_CCLRUSED		   1042
#define	IT_CCLRIMPORTANT	   1043
#define	IT_USUNITS		   1044
#define	IT_USRESERVED		   1045
#define	IT_USRECORDING		   1046
#define	IT_USRENDERING		   1047
#define	IT_CSIZE1		   1048
#define	IT_CSIZE2		   1049
#define	IT_ULCOLORENCODING	   1050
#define	IT_ULIDENTIFIER		   1051

#define	DLG_VIEWBITMAPSYSINO	   2060
#define	IT_CXSCREEN		   2061
#define	IT_CYSCREEN		   2062
#define	IT_HORZRESOLUTION	   2063
#define	IT_VERTRESOLUTION	   2064
#define	IT_PLANES		   2065
#define	IT_DISTINCTCOLOURS	   2066
#define	IT_BITCOUNT		   2067
#define	IT_BITMAPFORMATS	   2068
#define	IT_BITBLT		   2069
#define	IT_BANDING		   2070
#define	IT_BITBLTSCALING	   2071

#define	DLG_VIEWCOLOURTABLE 700
#define	SB_INDEX	    701
#define	IT_INDEX00	    702
#define	IT_INDEX01	    703
#define	IT_INDEX02	    704
#define	IT_INDEX03	    705
#define	IT_INDEX04	    706
#define	IT_INDEX05	    707
#define	IT_INDEX06	    708
#define	IT_INDEX07	    709
#define	IT_INDEX08	    710
#define	IT_INDEX09	    711
#define	IT_INDEX10	    712
#define	IT_INDEX11	    713
#define	IT_INDEX12	    714
#define	IT_INDEX13	    715
#define	IT_INDEX14	    716
#define	IT_INDEX15	    717
#define	EF_INDEX00	    718
#define	EF_INDEX01	    719
#define	EF_INDEX02	    720
#define	EF_INDEX03	    721
#define	EF_INDEX04	    722
#define	EF_INDEX05	    723
#define	EF_INDEX06	    724
#define	EF_INDEX07	    725
#define	EF_INDEX08	    726
#define	EF_INDEX09	    727
#define	EF_INDEX10	    728
#define	EF_INDEX11	    729
#define	EF_INDEX12	    730
#define	EF_INDEX13	    731
#define	EF_INDEX14	    732
#define	EF_INDEX15	    733
#define	DLG_VIEW2COLOURTABLE 734
#define	IDM_VIEWSYSTEMINFO 12

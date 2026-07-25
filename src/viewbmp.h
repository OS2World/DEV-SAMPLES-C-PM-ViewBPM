/*
 * viewbmp.h -- Resource and control IDs for the Bitmap Viewer application
 *
 * This header defines every numeric constant referenced in ViewBmp.Rc, the
 * menu command handlers, dialog procedures, and the custom file open dialog.
 * It is shared by all source modules in the project.
 *
 * ID naming conventions used in this sample:
 *   WIN_   - Main window resource ID (icon, menu, dialog templates)
 *   IDM_   - Menu item command IDs
 *   DLG_   - Dialog template IDs
 *   IT_    - Static-text item IDs inside dialogs (display only)
 *   EF_    - Entry field IDs (editable text)
 *   SB_    - Scroll bar IDs
 *   CBX_   - Combo box IDs
 *   LB_    - List box IDs
 *
 * Authors: Mark Benge, Matt Smith (Prominare Inc.) 1995
 * Updated: 2026 - documentation improved
 */

#ifndef VIEWBMP_H
#define VIEWBMP_H

/* -----------------------------------------------------------------------
 * Main window resource ID
 * Used for the application icon, main menu, and as the module ID passed
 * to WinCreateStdWindow (resource module handle = NULL means .EXE).
 * ----------------------------------------------------------------------- */
#define WIN_BITMAPVIEWER        1

/* -----------------------------------------------------------------------
 * Main menu command IDs
 * ----------------------------------------------------------------------- */

/* File menu */
#define IDM_FILE               10
#define IDM_OPEN               11   /* Open a .BMP file via custom dialog   */
#define IDM_VIEWSYSTEMINFO     12   /* Show display capabilities dialog     */

/* Scale menu — set zoom level for the current bitmap */
#define IDM_SCALE              20
#define IDM_25PERCENT          21
#define IDM_50PERCENT          22
#define IDM_75PERCENT          23
#define IDM_100PERCENT         24
#define IDM_200PERCENT         25
#define IDM_400PERCENT         26
#define IDM_800PERCENT         27
#define IDM_SHOWALL            28   /* Show all bitmaps in array side-by-side */

/* Effects menu — colour manipulations on the loaded bitmap */
#define IDM_EFFECTS            30
#define IDM_FLIPCOLOURTABLE    31   /* Reverse the colour table in memory   */

/* -----------------------------------------------------------------------
 * Context (right-click popup) menu IDs
 * IDM_POPUP      : shown when a bitmap array is loaded (shows all headers)
 * IDM_POPUPSINGLE: shown when a single bitmap is loaded
 * ----------------------------------------------------------------------- */
#define IDM_POPUP              850
#define IDM_POPUPSINGLE        851

/* Popup menu items — open header-inspection dialogs */
#define IDM_BITMAPARRAYFILEHEADER  40
#define IDM_BITMAPFILEHEADER       41
#define IDM_BITMAPINFOHEADER       42
#define IDM_COLOURTABLE            43

/* -----------------------------------------------------------------------
 * Dialog template IDs
 * ----------------------------------------------------------------------- */

/* Custom file-open dialog (predates WinFileDlg) */
#define DLG_OPENBITMAP                  1150

/* OS/2 1.x bitmap header inspection dialogs */
#define DLG_VIEWBITMAPARRAYFILEHEADER   2048  /* BITMAPARRAYFILEHEADER    */
#define DLG_VIEWBITMAPFILEHEADER        2049  /* BITMAPFILEHEADER (1.x)   */
#define DLG_VIEWBITMAPINFOHEADER        2050  /* BITMAPINFOHEADER (1.x)   */

/* OS/2 2.x bitmap header inspection dialogs */
#define DLG_VIEWBITMAPARRAYFILEHEADER2  2051  /* BITMAPARRAYFILEHEADER2   */
#define DLG_VIEWBITMAPFILEHEADER2       2052  /* BITMAPFILEHEADER2        */
#define DLG_VIEWBITMAPINFOHEADER2       2053  /* BITMAPINFOHEADER2        */

/* Colour table dialogs */
#define DLG_VIEWCOLOURTABLE     700   /* Up to 256 entries with scrollbar  */
#define DLG_VIEW2COLOURTABLE    701   /* 2-entry table for 1-bit bitmaps   */

/* System bitmap info dialog */
#define DLG_VIEWBITMAPSYSINO    800   /* Shows DevQueryCaps results        */

/* -----------------------------------------------------------------------
 * Static-text item IDs in header-inspection dialogs
 * ----------------------------------------------------------------------- */

/* Shared across multiple header dialogs */
#define IT_BITMAPINDEX      100   /* "Bitmap image index" label             */
#define IT_USTYPE           101   /* usType / bfType field value            */
#define IT_CBSIZE           102   /* cbSize / biSize field value            */
#define IT_OFFNEXT          103   /* offNext field value                    */
#define IT_CXDISPLAY        104   /* cxDisplay field value                  */
#define IT_CYDISPLAY        105   /* cyDisplay field value                  */
#define IT_XHOTSPOT         106   /* xHotspot field value                   */
#define IT_YHOTSPOT         107   /* yHotspot field value                   */
#define IT_OFFBITS          108   /* offBits field value                    */
#define IT_CBFIX            109   /* cbFix field value                      */
#define IT_CX               110   /* cx / biWidth field value               */
#define IT_CY               111   /* cy / biHeight field value              */
#define IT_CPLANES          112   /* cPlanes field value                    */
#define IT_CBITCOUNT        113   /* cBitCount field value                  */

/* OS/2 2.x BITMAPINFOHEADER2 additional fields */
#define IT_ULCOMPRESSION    114
#define IT_CBIMAGE          115
#define IT_CXRESOLUTION     116
#define IT_CYRESOLUTION     117
#define IT_CCLRUSED         118
#define IT_CCLRIMPORTANT    119
#define IT_USUNITS          120
#define IT_USRESERVED       121
#define IT_USRECORDING      122
#define IT_USRENDERING      123
#define IT_CSIZE1           124
#define IT_CSIZE2           125
#define IT_ULCOLORENCODING  126
#define IT_ULIDENTIFIER     127

/* -----------------------------------------------------------------------
 * Colour table dialog controls (DLG_VIEWCOLOURTABLE)
 * 16 visible rows; user scrolls to see all 256 entries.
 * ----------------------------------------------------------------------- */

/* Static index labels (IT_INDEX00 .. IT_INDEX15) */
#define IT_INDEX00          200
#define IT_INDEX01          201
#define IT_INDEX02          202
#define IT_INDEX03          203
#define IT_INDEX04          204
#define IT_INDEX05          205
#define IT_INDEX06          206
#define IT_INDEX07          207
#define IT_INDEX08          208
#define IT_INDEX09          209
#define IT_INDEX10          210
#define IT_INDEX11          211
#define IT_INDEX12          212
#define IT_INDEX13          213
#define IT_INDEX14          214
#define IT_INDEX15          215

/* Entry fields showing the hex RGB value (EF_INDEX00 .. EF_INDEX15) */
#define EF_INDEX00          300
#define EF_INDEX01          301
#define EF_INDEX02          302
#define EF_INDEX03          303
#define EF_INDEX04          304
#define EF_INDEX05          305
#define EF_INDEX06          306
#define EF_INDEX07          307
#define EF_INDEX08          308
#define EF_INDEX09          309
#define EF_INDEX10          310
#define EF_INDEX11          311
#define EF_INDEX12          312
#define EF_INDEX13          313
#define EF_INDEX14          314
#define EF_INDEX15          315

/* Vertical scrollbar for the colour table */
#define SB_INDEX            400

/* -----------------------------------------------------------------------
 * System bitmap info dialog item IDs (DLG_VIEWBITMAPSYSINO)
 * ----------------------------------------------------------------------- */
#define IT_CXSCREEN         500   /* Screen width in pixels                 */
#define IT_CYSCREEN         501   /* Screen height in pixels                */
#define IT_HORZRESOLUTION   502   /* Horizontal DPI / resolution            */
#define IT_VERTRESOLUTION   503   /* Vertical DPI / resolution              */
#define IT_PLANES           504   /* Number of colour planes                */
#define IT_DISTINCTCOLOURS  505   /* Number of distinct colours             */
#define IT_BITCOUNT         506   /* Bits per pixel on display              */
#define IT_BITMAPFORMATS    507   /* Supported bitmap formats count         */
#define IT_BITBLT           508   /* BitBlt capability flag                 */
#define IT_BANDING          509   /* Banding capability flag                */
#define IT_BITBLTSCALING    510   /* Bit-blt scaling capability flag        */

/* -----------------------------------------------------------------------
 * File open dialog (DLG_OPENBITMAP) control IDs
 * ----------------------------------------------------------------------- */
#define IT_DIRECTORY        600   /* Current directory path label           */
#define IT_VOLUMELABEL      601   /* Volume label of current drive          */
#define IT_DISKFREE         602   /* Free disk space string                 */
#define IT_FILECOUNT        603   /* Number of .BMP files in current dir    */
#define CBX_DRIVES          604   /* Drop-down combobox: available drives   */
#define LB_DIRECTORIES      605   /* List box: subdirectory names           */
#define LB_FILES            606   /* List box: .BMP file names              */

#endif /* VIEWBMP_H */

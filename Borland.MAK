
# MAKE file for Borland.MAK

# Revised:  1995-10-05

# Macro definitions

C_SW=-Ox -v- -w -c 
RC_SW=-r 


ViewBmp.Exe: AppDefs.Obj BmpWnd.Obj Effects.Obj\
        GetBmp.Obj OpenDlg.Obj Support.Obj\
        ViewBmp.Obj ViewBmp.Res ViewDlg.Obj
 TLink @Borland.Lnk
 rc ViewBmp.Res ViewBmp.Exe

AppDefs.Obj: AppDefs.C
 Bcc $(C_SW) -o$*.Obj AppDefs.C

BmpWnd.Obj: BmpWnd.C
 Bcc $(C_SW) -o$*.Obj BmpWnd.C

Effects.Obj: Effects.C
 Bcc $(C_SW) -o$*.Obj Effects.C

GetBmp.Obj: GetBmp.C
 Bcc $(C_SW) -o$*.Obj GetBmp.C

OpenDlg.Obj: OpenDlg.C
 Bcc $(C_SW) -o$*.Obj OpenDlg.C

Support.Obj: Support.C
 Bcc $(C_SW) -o$*.Obj Support.C

ViewBmp.Obj: ViewBmp.C
 Bcc $(C_SW) -o$*.Obj ViewBmp.C

ViewBmp.Res: ViewBmp.Rc
 Rc $(RC_SW) ViewBmp.Rc $*.Res

ViewDlg.Obj: ViewDlg.C
 Bcc $(C_SW) -o$*.Obj ViewDlg.C


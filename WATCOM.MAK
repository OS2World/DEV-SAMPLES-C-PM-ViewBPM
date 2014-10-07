
# MAKE file for WATCOM.MAK

# Revised:  1995-10-05

# Macro definitions

C_SW=-c -ms -4s -s -ox -zp4 -w3 
RC_SW=-r 


ViewBmp.Exe: AppDefs.Obj BmpWnd.Obj Effects.Obj\
        GetBmp.Obj OpenDlg.Obj Support.Obj\
        ViewBmp.Obj ViewBmp.Res ViewDlg.Obj
 WLink @WATCOM.Lnk
 Rc ViewBmp.Res ViewBmp.Exe

AppDefs.Obj: AppDefs.C
 Wcl386 $(C_SW) -fo=$*.Obj AppDefs.C

BmpWnd.Obj: BmpWnd.C
 Wcl386 $(C_SW) -fo=$*.Obj BmpWnd.C

Effects.Obj: Effects.C
 Wcl386 $(C_SW) -fo=$*.Obj Effects.C

GetBmp.Obj: GetBmp.C
 Wcl386 $(C_SW) -fo=$*.Obj GetBmp.C

OpenDlg.Obj: OpenDlg.C
 Wcl386 $(C_SW) -fo=$*.Obj OpenDlg.C

Support.Obj: Support.C
 Wcl386 $(C_SW) -fo=$*.Obj Support.C

ViewBmp.Obj: ViewBmp.C
 Wcl386 $(C_SW) -fo=$*.Obj ViewBmp.C

ViewBmp.Res: ViewBmp.Rc
 Rc $(RC_SW) ViewBmp.Rc $*.Res

ViewDlg.Obj: ViewDlg.C
 Wcl386 $(C_SW) -fo=$*.Obj ViewDlg.C


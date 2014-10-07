
# MAKE file for IBM.MAK

# Revised:  1995-10-05

# Macro definitions

C_SW=-G4 -O+ -W3 -C 
RC_SW=-r 


ViewBmp.Exe: AppDefs.Obj BmpWnd.Obj Effects.Obj\
        GetBmp.Obj OpenDlg.Obj Support.Obj\
        ViewBmp.Obj ViewBmp.Res ViewDlg.Obj
 ILink /EXEPACK /NOFREE /ALIGN:4 /BASE:0x10000 /IGNORECASE @IBM.Lnk
 Rc ViewBmp.Res ViewBmp.Exe

AppDefs.Obj: AppDefs.C
 Icc $(C_SW) -Fo$*.Obj AppDefs.C

BmpWnd.Obj: BmpWnd.C
 Icc $(C_SW) -Fo$*.Obj BmpWnd.C

Effects.Obj: Effects.C
 Icc $(C_SW) -Fo$*.Obj Effects.C

GetBmp.Obj: GetBmp.C
 Icc $(C_SW) -Fo$*.Obj GetBmp.C

OpenDlg.Obj: OpenDlg.C
 Icc $(C_SW) -Fo$*.Obj OpenDlg.C

Support.Obj: Support.C
 Icc $(C_SW) -Fo$*.Obj Support.C

ViewBmp.Obj: ViewBmp.C
 Icc $(C_SW) -Fo$*.Obj ViewBmp.C

ViewBmp.Res: ViewBmp.Rc
 Rc $(RC_SW) ViewBmp.Rc $*.Res

ViewDlg.Obj: ViewDlg.C
 Icc $(C_SW) -Fo$*.Obj ViewDlg.C


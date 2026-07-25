@echo off
rem compile-ow.cmd -- Build ViewBmp with OpenWatcom C for OS/2
rem
rem Requires:
rem   OpenWatcom 1.9 or 2.0 on the PATH (wcc386, wlink, wrc)
rem   OS/2 Toolkit headers and import libraries
rem
rem wlink reads the "option description" string and embeds it in the EXE
rem as the bldlevel DESCRIPTION field, visible via the bldlevel command.
rem
rem Build log is written to compile-ow.log

set SRC=src
set OUT=bin-ow
set CFLAGS=-bt=os2 -mf -3 -zq -I%SRC%
set LOG=compile-ow.log

echo Build started > %LOG%
echo. >> %LOG%

echo Compiling...
echo === Compiling === >> %LOG%

echo   appdefs.c
wcc386 %CFLAGS% -fo=%OUT%\appdefs.obj  %SRC%\appdefs.c  >> %LOG% 2>&1

echo   bmpwnd.c
wcc386 %CFLAGS% -fo=%OUT%\bmpwnd.obj   %SRC%\bmpwnd.c   >> %LOG% 2>&1

echo   effects.c
wcc386 %CFLAGS% -fo=%OUT%\effects.obj  %SRC%\effects.c  >> %LOG% 2>&1

echo   getbmp.c
wcc386 %CFLAGS% -fo=%OUT%\getbmp.obj   %SRC%\getbmp.c   >> %LOG% 2>&1

echo   opendlg.c
wcc386 %CFLAGS% -fo=%OUT%\opendlg.obj  %SRC%\opendlg.c  >> %LOG% 2>&1

echo   support.c
wcc386 %CFLAGS% -fo=%OUT%\support.obj  %SRC%\support.c  >> %LOG% 2>&1

echo   viewbmp.c
wcc386 %CFLAGS% -fo=%OUT%\viewbmp.obj  %SRC%\viewbmp.c  >> %LOG% 2>&1

echo   viewdlg.c
wcc386 %CFLAGS% -fo=%OUT%\viewdlg.obj  %SRC%\viewdlg.c  >> %LOG% 2>&1

echo Compiling resource...
echo. >> %LOG%
echo === Resource === >> %LOG%
wrc -r -I%SRC% -fo=%OUT%\viewbmp.res %SRC%\viewbmp.rc >> %LOG% 2>&1

echo Linking...
echo. >> %LOG%
echo === Linking === >> %LOG%
wlink ^
    name %OUT%\ViewBmp.exe ^
    system os2v2_pm ^
    option modname=ViewBmp ^
    option stack=32768 ^
    option description '@#OS2World:1.01#@##1## 24 Jul 2026 00:00:00     ARCAOS-507::::::v1.01@@Bitmap Viewer - OS/2 PM sample for viewing and analyzing bitmap files.' ^
    file %OUT%\appdefs.obj ^
    file %OUT%\bmpwnd.obj ^
    file %OUT%\effects.obj ^
    file %OUT%\getbmp.obj ^
    file %OUT%\opendlg.obj ^
    file %OUT%\support.obj ^
    file %OUT%\viewbmp.obj ^
    file %OUT%\viewdlg.obj ^
    library pm >> %LOG% 2>&1

echo Adding resources...
echo. >> %LOG%
echo === Adding resources === >> %LOG%
wrc %OUT%\viewbmp.res %OUT%\ViewBmp.exe >> %LOG% 2>&1

echo. >> %LOG%
echo Build finished >> %LOG%

echo Done. See %LOG% for full output.

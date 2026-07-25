@echo off
rem compile-gcc.cmd -- Build ViewBmp with GCC 9.2 for OS/2
rem
rem Compiles each .c file with GCC (-Zomf produces OMF-format objects),
rem then uses GCC as the linker (-Zcrtdll links against kLIBC DLL).
rem GCC handles libgcc and kLIBC automatically; only PM lazy libs are explicit.
rem
rem Requires:
rem   GCC 9.2 (OS/2 port) on the PATH   -- compile and link
rem   wrc (OpenWatcom resource compiler) -- for resources
rem   OS/2 Toolkit headers in the compiler include path
rem
rem Build log is written to compile-gcc.log

SET EMXOMFLD_TYPE=WLINK
SET EMXOMFLD_LINKER=wl.exe

set SRC=src
set OUT=bin-gcc
set CFLAGS=-c -Zomf -O2 -Wall -Wno-pointer-sign -I%SRC%
set LOG=compile-gcc.log

echo Build started > %LOG%
echo. >> %LOG%

if not exist %OUT% mkdir %OUT%

echo Compiling...
echo === Compiling === >> %LOG%

echo   appdefs.c
gcc %CFLAGS% -o %OUT%\appdefs.obj  %SRC%\appdefs.c  >> %LOG% 2>&1

echo   bmpwnd.c
gcc %CFLAGS% -o %OUT%\bmpwnd.obj   %SRC%\bmpwnd.c   >> %LOG% 2>&1

echo   effects.c
gcc %CFLAGS% -o %OUT%\effects.obj  %SRC%\effects.c  >> %LOG% 2>&1

echo   getbmp.c
gcc %CFLAGS% -o %OUT%\getbmp.obj   %SRC%\getbmp.c   >> %LOG% 2>&1

echo   opendlg.c
gcc %CFLAGS% -o %OUT%\opendlg.obj  %SRC%\opendlg.c  >> %LOG% 2>&1

echo   support.c
gcc %CFLAGS% -o %OUT%\support.obj  %SRC%\support.c  >> %LOG% 2>&1

echo   viewbmp.c
gcc %CFLAGS% -o %OUT%\viewbmp.obj  %SRC%\viewbmp.c  >> %LOG% 2>&1

echo   viewdlg.c
gcc %CFLAGS% -o %OUT%\viewdlg.obj  %SRC%\viewdlg.c  >> %LOG% 2>&1

echo Compiling resource...
echo. >> %LOG%
echo === Resource === >> %LOG%
wrc -r -I%SRC% -fo=%OUT%\viewbmp.res %SRC%\viewbmp.rc >> %LOG% 2>&1

echo Linking...
echo. >> %LOG%
echo === Linking === >> %LOG%
set EMXOMFLD_TYPE=wlink
set EMXOMFLD_PRELINK=0
gcc -Zomf -Zcrtdll -Zmap -o %OUT%\ViewBmp.exe %OUT%\appdefs.obj %OUT%\bmpwnd.obj %OUT%\effects.obj %OUT%\getbmp.obj %OUT%\opendlg.obj %OUT%\support.obj %OUT%\viewbmp.obj %OUT%\viewdlg.obj -L C:\usr\lib -los2_pm_lazy -llazyimp >> %LOG% 2>&1

echo Setting PM subsystem flag...
echo. >> %LOG%
echo === PM flag === >> %LOG%
rexx setpm.cmd %OUT%\ViewBmp.exe >> %LOG% 2>&1

echo Adding resources...
echo. >> %LOG%
echo === Adding resources === >> %LOG%
wrc %OUT%\viewbmp.res %OUT%\ViewBmp.exe >> %LOG% 2>&1

echo. >> %LOG%
echo Build finished >> %LOG%

echo Done. See %LOG% for full output.

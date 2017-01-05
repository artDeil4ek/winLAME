@echo off
REM
REM winLAME - a frontend for the LAME encoding engine
REM Copyright (c) 2000-2017 Michael Fink
REM
REM Copies libraries to bin folder
REM

REM used when double-clicking
if "%1" == "" call CopyLibraries.cmd Debug
if "%1" == "" call CopyLibraries.cmd Release
if "%1" == "" exit 0

mkdir ..\..\bin 2> nul

set TARGET=..\..\bin\%1

mkdir %TARGET% 2> nul

copy bass.dll %TARGET%
copy basscd.dll %TARGET%
copy basswma.dll %TARGET%
copy libFLAC_dynamic.dll %TARGET%
copy libmmd.dll %TARGET%
copy libmp3lame.dll %TARGET%
copy libsndfile-1.dll %TARGET%
copy libvorbis.dll %TARGET%
copy libspeex.dll %TARGET%
copy libopus-0.dll %TARGET%
copy libopusfile-0.dll %TARGET%
copy libgcc_s_sjlj-1.dll %TARGET%
copy libogg-0.dll %TARGET%
copy libwinpthread-1.dll %TARGET%
copy libfaac.dll %TARGET%
copy libfaad2.dll %TARGET%
copy MACDll.dll %TARGET%
copy MrCrash.exe %TARGET%

mkdir %ProgramData%\winLAME 2> nul
copy ..\presets.xml %ProgramData%\winLAME\presets.xml

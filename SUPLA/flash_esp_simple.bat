@echo off
setlocal EnableExtensions EnableDelayedExpansion
title ESP Simple Flash

set "SCRIPT_DIR=%~dp0"
set "ESPTOOL=%SCRIPT_DIR%esptool.exe"
set "PORT="
set "BAUD=460800"

:menu
cls
echo ====================================
echo        ESP SIMPLE FLASH TOOL
echo ====================================
echo.
echo Folder : %SCRIPT_DIR%
echo Port   : %PORT%
echo Baud   : %BAUD%
echo.
echo 1. List COM ports
echo 2. Select COM port
echo 3. Flash from current folder
echo 0. Exit
echo.
set /p "OPT=> "

if "%OPT%"=="1" goto list_ports
if "%OPT%"=="2" goto select_port
if "%OPT%"=="3" goto flash
if "%OPT%"=="0" goto end
goto menu

:list_ports
cls
echo Available COM ports:
echo.
for /f "skip=2 tokens=3" %%A in ('reg query "HKLM\HARDWARE\DEVICEMAP\SERIALCOMM" 2^>nul') do echo %%A
echo.
pause
goto menu

:select_port
cls
echo Available COM ports:
echo.
for /f "skip=2 tokens=3" %%A in ('reg query "HKLM\HARDWARE\DEVICEMAP\SERIALCOMM" 2^>nul') do echo %%A
echo.
set /p "P=Enter COM number or COM name: "
if "%P%"=="" goto menu

if /i "%P:~0,3%"=="COM" (
    set "PORT=%P%"
) else (
    set "PORT=COM%P%"
)
goto menu

:flash
if "%PORT%"=="" (
    echo.
    echo Select COM port first.
    pause
    goto menu
)

if not exist "%ESPTOOL%" (
    echo.
    echo Missing file:
    echo %ESPTOOL%
    echo.
    echo Put esptool.exe in the same folder as this script.
    pause
    goto menu
)

call :find_files

if defined FLASH_CANCELLED goto menu
if defined MERGED goto flash_merged
if defined BOOT if defined PART if defined APP0 if defined APP goto flash_multi

echo.
echo No supported firmware files found in:
echo %SCRIPT_DIR%
echo.
echo Expected one of:
echo - *merged.bin
echo - *bootloader*.bin + *partitions*.bin + boot_app0.bin + *.ino.bin
echo.
pause
goto menu

:find_files
set "FLASH_CANCELLED="
set "MERGED="
set "BOOT="
set "PART="
set "APP0="
set "APP="
set /a MCOUNT=0
set /a APPCOUNT=0

for %%F in ("%SCRIPT_DIR%*merged.bin") do (
    set /a MCOUNT+=1
    set "MERGED[!MCOUNT!]=%%~fF"
    set "MERGEDNAME[!MCOUNT!]=%%~nxF"
)

if %MCOUNT%==1 set "MERGED=!MERGED[1]!"
if %MCOUNT% gtr 1 call :choose_merged

for %%F in ("%SCRIPT_DIR%*.bin") do (
    set "FN=%%~nxF"
    echo !FN! | findstr /i "bootloader" >nul && set "BOOT=%%~fF"
    echo !FN! | findstr /i "partitions" >nul && set "PART=%%~fF"
    if /i "%%~nxF"=="boot_app0.bin" set "APP0=%%~fF"
    echo !FN! | findstr /i /c:".ino.bin" >nul && (
        set /a APPCOUNT+=1
        set "APP[!APPCOUNT!]=%%~fF"
        set "APPNAME[!APPCOUNT!]=%%~nxF"
    )
)

if %APPCOUNT%==1 set "APP=!APP[1]!"
if %APPCOUNT% gtr 1 call :choose_app
exit /b

:choose_merged
cls
echo Multiple merged files found:
echo.
for /L %%I in (1,1,%MCOUNT%) do echo %%I. !MERGEDNAME[%%I]!
echo.
echo Q = cancel
echo.
set /p "MSEL=Select merged file number: "
if "%MSEL%"=="" (
    set "FLASH_CANCELLED=1"
    set "MERGED="
    exit /b
)
if /i "%MSEL%"=="Q" (
    set "FLASH_CANCELLED=1"
    set "MERGED="
    exit /b
)
if not defined MERGED[%MSEL%] (
    set "MERGED="
    exit /b
)
call set "MERGED=%%MERGED[%MSEL%]%%"
exit /b

:choose_app
cls
echo Multiple app files found:
echo.
for /L %%I in (1,1,%APPCOUNT%) do echo %%I. !APPNAME[%%I]!
echo.
echo Q = cancel
echo.
set /p "ASEL=Select app file number: "
if "%ASEL%"=="" (
    set "FLASH_CANCELLED=1"
    set "APP="
    exit /b
)
if /i "%ASEL%"=="Q" (
    set "FLASH_CANCELLED=1"
    set "APP="
    exit /b
)
if not defined APP[%ASEL%] (
    set "APP="
    exit /b
)
call set "APP=%%APP[%ASEL%]%%"
exit /b

:flash_merged
cls
echo Flashing merged image:
echo %MERGED%
echo.
echo Port : %PORT%
echo Baud : %BAUD%
echo.
choice /M "Start flashing"
if errorlevel 2 goto menu

"%ESPTOOL%" --chip esp32 --port %PORT% --baud %BAUD% --before default-reset --after hard-reset ^
write-flash -z --flash-mode keep --flash-freq keep --flash-size keep ^
0x0 "%MERGED%"

echo.
pause
goto menu

:flash_multi
cls
echo Flashing from 4 files:
echo.
echo Bootloader : %BOOT%
echo Partitions : %PART%
echo boot_app0  : %APP0%
echo App        : %APP%
echo.
echo Port : %PORT%
echo Baud : %BAUD%
echo.
choice /M "Start flashing"
if errorlevel 2 goto menu

"%ESPTOOL%" --chip esp32 --port %PORT% --baud %BAUD% --before default-reset --after hard-reset ^
write-flash -z --flash-mode keep --flash-freq keep --flash-size keep ^
0x1000 "%BOOT%" ^
0x8000 "%PART%" ^
0xe000 "%APP0%" ^
0x10000 "%APP%"

echo.
pause
goto menu

:end
endlocal
exit /b

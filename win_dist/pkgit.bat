@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "CYGWIN_ROOT=%SCRIPT_DIR%\cygwin"
set "HOME=%CYGWIN_ROOT%\home"
set "PATH=%ROOT%;%PATH%"

"%SCRIPT_DIR%\pkgit_bin.exe" %*

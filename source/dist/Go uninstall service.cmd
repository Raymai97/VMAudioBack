@echo off
cd %~dp0
VMAudioBack.exe --uninstall-service && echo OK || echo Error
pause
exit/b

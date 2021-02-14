@echo off
cd %~dp0
VMAudioBack.exe --install-service && echo OK || echo Error
pause
exit/b

@echo off

for /r %%a in (.) do (
set list=%%a
setlocal enabledelayedexpansion
if "!list:~-8,6!"=="-Debug" rd /q /s "!list:~0,-2!"
if "!list:~-10,8!"=="-Release" rd /q /s "!list:~0,-2!"
endlocal
)

rd /q /s "test\debug"
rd /q /s "test\release"

del /f /q /s /a *.user *.ncb *.suo *.sdf *.i
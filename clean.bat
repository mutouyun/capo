@echo off

rd /q /s "build*"
rd /q /s "test\Debug"
rd /q /s "test\Release"

del /f /q /s /a *.user *.ncb *.suo *.sdf *.i
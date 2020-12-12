@echo off
pushd ..\
call vendor\bin\premake\premake5.exe codeblocks
popd
PAUSE
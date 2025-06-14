@setlocal enableextensions
@cd /d "%~dp0"
@echo off
COLOR 1F
Title Killing running process...



C:
cd C:\Windows\Microsoft.Net\Framework\v2.0.50727\
set msbuildemitsolution=1


MSBuild "Z:\cplusplus\eddalauncher\[Client]Launcher\Launcher.sln" /nologo /verbosity:quiet /consoleloggerparameters:NoItemAndPropertyList /target:Launcher /p:Configuration=Release


@pause
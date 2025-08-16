@echo off

REM Set colors
color

REM Delete specific file types
del /q /s *.ncb
del /q /s *.sbr
del /q /s *.obj
del /q /s *.pdb
del /q /s *.idb
del /q /s *.ib_pdb_index
del /q /s *.pch
del /q /s *.ib_tag
del /q /s *.embed.manifest
del /q /s *.user
del /q /s *.exp
del /q /s *.ilk
del /q /s *.map
del /q /s *.intermediate.manifest
del /q /s *.plg
del /q /s *.dep
del /q /s *.htm
del /q /s *.xml
del /q /s *.cod

REM Pause the script
pause

REM Delete empty directories recursively
call :delete_empty_dirs
goto :eof

:delete_empty_dirs
for /f "delims=" %%i in ('dir /ad /b /s ^| sort /r') do (
  rd "%%i" 2>nul
)
goto :eof

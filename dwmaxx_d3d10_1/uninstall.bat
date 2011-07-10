@echo off

echo About to uninstall the D3D10_1 proxy....
pause

echo Stopping DWM
sc stop uxsms > nul
timeout /t 5 /nobreak > nul

echo Restoring d3d10_1.dll
copy /y "C:\Windows\System32\d3d10_1_original.dll" "C:\Windows\System32\d3d10_1.dll" > nul
del "C:\Windows\System32\d3d10_1_original.dll" > nul

echo Restarting DWM
sc start uxsms > nul

echo Done!
pause

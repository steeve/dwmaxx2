@echo off

echo About to install the D3D10_1 proxy....
pause

echo Stopping DWM...
sc stop uxsms > nul
timeout /t 5 /nobreak > nul

echo Setting permissions on d3d10_.dll
takeown /F "C:\Windows\System32\d3d10_1.dll" > nul
icacls "C:\Windows\System32\d3d10_1.dll" /grant %UserName%:F > nul

echo Backuping d3d10_1.dll
copy /y "C:\Windows\System32\d3d10_1.dll" "C:\Windows\System32\d3d10_1_original.dll" > nul

echo Replacing d3d10_1.dll
copy /y dwmaxx_d3d10_1.dll "C:\Windows\System32\d3d10_1.dll" > nul

echo Restarting DWM
sc start uxsms > nul

echo Done!
pause

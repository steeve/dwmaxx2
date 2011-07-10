@echo off
echo Stopping 32 bits watermarker...
taskkill /F /IM dwmaxx_watermarker32.exe

echo Stopping 64 bits watermarker...
taskkill /F /IM dwmaxx_watermarker64.exe

echo Done!
pause

ECHO OFF
if not "%1"=="am_admin" (
    powershell -Command "Start-Process -Verb RunAs -FilePath '%0' -ArgumentList 'am_admin'"
    exit /b
)

for %%i in (blackeagle.exe cmd.exe) do taskkill /f /im "%%i" /t
PAUSE
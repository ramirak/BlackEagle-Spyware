ECHO OFF
for %%i in (blackeagle.exe cmd.exe) do taskkill /f /im "%%i" /t
PAUSE
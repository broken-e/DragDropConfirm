cd /d "%~dp0"
if %PROCESSOR_ARCHITECTURE%==x86 (
  RegSvr32.exe /U DragDropInterceptor_32.dll
) else (
  RegSvr32.exe /U DragDropInterceptor_64.dll
)
msg "%username%" You need to reboot after unregistering DragDropInterceptor.
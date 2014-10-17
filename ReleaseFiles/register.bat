cd /d "%~dp0"
if %PROCESSOR_ARCHITECTURE%==x86 (
  RegSvr32.exe DragDropInterceptor_32.dll
) else (
  RegSvr32.exe DragDropInterceptor_64.dll
)
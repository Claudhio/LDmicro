:DEL_LDMICRO
@del ldmicro.exe
@if exist ldmicro.exe echo   NOT DELETED ldmicro.exe !!!
@if exist ldmicro.exe PAUSE
@if exist ldmicro.exe goto DEL_LDMICRO

:DEL_LDMICRO_DEBUG
@del build\Debug\ldmicro.exe
@if exist build\Debug\ldmicro.exe echo   NOT DELETED build\Debug\ldmicro.exe !!!
@if exist build\Debug\ldmicro.exe PAUSE
@if exist build\Debug\ldmicro.exe goto DEL_LDMICRO_DEBUG

@SET PROGECT=D:\LDs\SVARKA2\svarka1.ld
@SET PROGECT=D:\LDs\SVARKA2\svarka2.ld
@SET PROGECT=D:\LDs\SVARKA2\svarka3.ld
:SVARKA1 - ����
@SET PROGECT=D:\LDs\SVARKA1\svarka.ld
:SVARKA2 - ��ன
@SET PROGECT=D:\LDs\SVARKA2\svarka.ld

@SET PROGECT=d:\lds\tmp\TMP.ld
@SET PROGECT=D:\LDs\uart\UART_M_ATmega2560_5.ld

if "%1" == "" goto DO_BUILD
@SET PROGECT="%1"
:DO_BUILD

md build
cd build
cmake.exe ..
cmake.exe --build .
cd ..

@if not exist build\bin\Debug\ldmicro.exe goto EXIT
start build\bin\Debug\ldmicro.exe %PROGECT%

:EXIT

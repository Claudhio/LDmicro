@rem COLOR [background][foreground]
@rem 0 = Black   8 = Gray
@rem 1 = Blue    9 = Light Blue
@rem 2 = Green   A = Light Green
@rem 3 = Aqua    B = Light Aqua
@rem 4 = Red     C = Light Red
@rem 5 = Purple  D = Light Purple
@rem 6 = Yellow  E = Light Yellow
@rem 7 = White   F = Bright White
@COLOR F0

@echo OFF
@rem This file is part of LDmicro project and must be located in same directory where LDmicro.exe located.
cls
if "%1" == "" goto AVR
if "%1" == "AVR" goto AVR
if "%1" == "PIC16" goto PIC16
if "%1" == "" goto pauses
goto NOT_SUPPOTED

:AVR
@rem Set up avrdude.exe path. It may be:
::SET AVRDUDE_PATH=D:\WinAVR\bin\
  SET AVRDUDE_PATH=D:\Arduino\hardware\tools\avr\bin\

@rem Set up your hardware avrdude programmer.
@rem   See avrdude.conf programmer id.
  SET AVRDUDE_PROGRAMMER_ID=dapa
::SET AVRDUDE_PROGRAMMER_ID=stk200s5

@rem Set up your avrdude Atmel Microcontroller.
@rem   See avrdude.conf part id.
@rem ATmega8=M8
::SET AVRDUDE_PART_ID=m8
@rem ATmega328P=m328p
::SET AVRDUDE_PART_ID=m328p
@rem ATmega32U4=m32u4
  SET AVRDUDE_PART_ID=m32u4

%AVRDUDE_PATH%avrdude.exe -y -c %AVRDUDE_PROGRAMMER_ID% -p %AVRDUDE_PART_ID% -U lfuse:w:0xff:m -U hfuse:w:0xda:m -U efuse:w:0xff:m
@echo ERRORLEVEL=%ERRORLEVEL%
if ERRORLEVEL==1 goto pauses
goto exit

:PIC16
@echo You can write own command for read PIC.
pause
goto exit

:NOT_SUPPOTED
@echo You can write own command for '%1'.

:pauses
@echo USE:
@echo "fuseMcu.bat AVR|PIC16|ANSIC|INTERPRETED|NETZER|PASCAL|ARDUINO|CAVR"
pause

:exit
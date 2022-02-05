::██▀▀█▀▀██▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█
::██  ▀  █▄  ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █ 
::█  ▄ ▄  ▀▀  ▀█▀  ▄  ▀█ ▀▄█ █▄ █
::█▄▄█▄█▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█
:: by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
@echo off

rem ───────────────────────────────────────────────────────────────────────────
rem  Build Tool configuration
rem ───────────────────────────────────────────────────────────────────────────
call ..\default_config.cmd %0

rem ***************************************************************************
rem  PROJECT SELECTION
rem ***************************************************************************
set DoPause=0
set Input=%~n1

:CheckInput
if not [%Input%]==[] goto :FoundInput
cls
set DoPause=1
echo No valide target selected...
echo Available targets:
set Formats=BIN,DOS1,DOS2,ROM_8K,ROM_8K_P2,ROM_16K,ROM_16K_P2,ROM_32K,ROM_48K,ROM_48K_ISR,ROM_64K,ROM_64K_ISR,ROM_ASCII8,ROM_ASCII16,ROM_KONAMI,ROM_KONAMI_SCC
for %%G in (%Formats%) do echo - %%G
set /p Name=Enter a target: 
for %%I in ("%Name%") do (set Input=%%~nI)
goto :CheckInput

:FoundInput
rem ***************************************************************************
rem * PROJECT SETTINGS                                                        *
rem ***************************************************************************

rem  Project name (will be use for output filename)
set ProjName=s_target

rem  Project modules to build (use ProjName if not defined)
set ProjModules=s_target

rem  List of library modules to build
set LibModules=system,bios,vdp,print,input,memory

rem  MSX version:
rem  - 1		MSX 1
rem  - 2		MSX 2
rem  - 2P		MSX 2+
rem  - TR		MSX TurboR
rem  - 12		MSX 1/2
set Machine=1

rem Target:
rem - BIN				.bin	BASIC binary program (8000h~)
rem - ROM_8K			.rom	8KB ROM in page 1 (4000h ~ 5FFFh)
rem - ROM_8K_P2			.rom	8KB ROM in page 2 (8000h ~ 9FFFh)
rem - ROM_16K			.rom	16KB ROM in page 1 (4000h ~ 7FFFh)
rem - ROM_16K_P2		.rom	16KB ROM in page 2 (8000h ~ BFFFh)
rem - ROM_32K			.rom	32KB ROM in page 1-2 (4000h ~ BFFFh)
rem - ROM_48K			.rom	48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 1-2 visible at start
rem - ROM_48K_ISR		.rom	48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 0-2 visible at start
rem - ROM_64K			.rom	64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 1-2 visible at start
rem - ROM_64K_ISR		.rom	64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 0-2 visible at start
rem - ROM_ASCII8		.rom	128KB ROM using ASCII-8 mapper
rem - ROM_ASCII16		.rom	128KB ROM using ASCII-16 mapper
rem - ROM_KONAMI		.rom	128KB ROM using Konami mapper (8KB segments)
rem - ROM_KONAMI_SCC	.rom	128KB ROM using Konami SCC mapper (8KB segments)
rem - DOS1				.com	MSX-DOS 1 program (0100h~) No direct acces to Main-ROM
rem - DOS2				.com	MSX-DOS 2 program (0100h~) No direct acces to Main-ROM
rem - DOS2_ARG			.com	[WIP] MSX-DOS 2 program (using command line arguments ; 0100h~) No direct acces to Main-ROM. 
set Target=%Input%
rem  Mapper size
REM set ROMSize=%2
set ROMSize=

rem  Optim:
rem  - Default
rem  - Speed
rem  - Size
set Optim=Speed

rem  Overwrite RAM starting address
set ForceRamAddr=

rem  Additionnal compilation flag
set CompileOpt=

rem  Verbose mode: 0 or 1
set Verbose=0

rem ***************************************************************************
rem * EMULATOR SETING                                                         *
rem ***************************************************************************

rem  Emulator options: 0 or 1
set EmulMachine=1
set Emul60Hz=0
set EmulFullScreen=0
set EmulMute=0
set EmulDebug=0
rem  Emulator extra parameters to be add to command-line
set EmulExtraParam=

rem ***************************************************************************
rem * BUILD STEPS                                                             *
rem ***************************************************************************
set DoClean=0
set DoCompile=1
set DoMake=1
set DoPackage=1
set DoDeploy=1
set DoRun=0

rem ***************************************************************************
rem * START BUILD                                                             *
rem ***************************************************************************
call %LibDir%\script\build.cmd

if %DoPause%==1 pause

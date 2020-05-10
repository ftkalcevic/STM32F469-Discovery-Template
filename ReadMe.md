# Template project for STM32F469 Discovery
TouchGFX, STM32CubeIDE, Visual GDB

It took me almost 3 days to get a these products to work together.  This repository contains...

| Directory | Description |
| ------ | ----- |
|TouchGFX|The TouchGFX project.  This was generated using TouchGFX for the STM32F469 Discovery board, then modified to include some additional peripherals.|
|STM32F469_Disco_DCC|The VisualGDB project|
|QSPIDriver|The VisualGDB QSPI OpenOCD external memory driver.  Built as described on the visualgdb web site.|

## Tips...

Edit the TouchGFX files using the standalone application, TouchGFX, by openning the file "\TouchGFX\TouchGFX\TouchGFX.touchgfx" directly.  Don't open it in CubeIDE.

Edit the CubeMX files directly in the standalone application, STM32CubeMX, by openning the file "\TouchGFX\STM32F469I-DISCO.ioc" directly.  Don't open it in CubeIDE.

Generate the TouchGFX code in TouchGFX.  Use CubeMX to generate code for peripheral initialisation.

Additional peripherals and GPIOs were added to the CubeMX file, except for FatFS or USB.  FatFS requires FreeRTOS/CMSIS_V1, but TouchGFX requires V2.  I hadn't decided what USB flavour I wanted.

This is based on TouchGFX 4.13.0, CubeIDE 1.3.0, CubeMX 5.5, Visual GDB 5.5.



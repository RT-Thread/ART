ART is an Arduino like board with STM32F407VGT6 (ARM Cortex-M4) chip. 
RT-Thread RTOS is running as a platform in this board.
- Arduino hardware/software compatible.
- 32bit ARM Cortex-M4 with FPU.
- Running multi-Arduino Program in parallel.

Hardware Information
--------------------
- ARM 32-bit Cortex-M4 CPU with FPU
- 168 MHz maximum frequency, 210 DMIPS/1.25 DMIPS/MHz
- 1 Mbyte of Flash memory
- 192+4 Kbytes of SRAM
- Serial wire debug (SWD)
- Up to 3 UARTs
- Up to 2 SPIs
- 1 CAN interfaces
- 2 USB OTG.

Software Information
--------------------
The software of ART uses [RT-Thread] RTOS as a platform, in which this platform, the application module feature is used to execute Arduino program in parallel. 

  [RT-Thread]: http://en.rt-thread.org

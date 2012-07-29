import os

# bsp name
BSP = 'stm32f40x'

# toolchains
EXEC_PATH = 'C:/Program Files/CodeSourcery/Sourcery G++ Lite/bin'
if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

PREFIX = 'arm-none-eabi-'
CC = PREFIX + 'g++'
CXX = PREFIX + 'g++'
AS = PREFIX + 'gcc'
AR = PREFIX + 'ar'
LINK = PREFIX + 'gcc'
TARGET_EXT = 'mo'
SIZE = PREFIX + 'size'
OBJDUMP = PREFIX + 'objdump'
OBJCPY = PREFIX + 'objcopy'

DEVICE = ' -mcpu=cortex-m4'
CFLAGS = DEVICE + ' -mthumb -mlong-calls -O2 -fPIC -fno-exceptions -fno-rtti'
AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp'
LFLAGS = DEVICE + ' -mthumb -Wl,-z,max-page-size=0x4 -shared -fPIC -nostdlib'

CPATH = ''
LPATH = ''

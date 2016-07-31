import os

cppdefines = ['F_CPU=8000000UL']
cppflags = ['-Wall', '-Wextra', '-mmcu=atmega328p', '-g', '-Os', '-ffunction-sections', '-fdata-sections']
linker_flags = ['-Wl,--gc-sections', '-Os', '-mmcu=atmega328p']

env = Environment(ENV=os.environ, CC="avr-gcc", CPPFLAGS=cppflags, CPPDEFINES=cppdefines)

sources = [
    'x27.c',
    'x27-adc.c',
    'x27-pwm.c',
    'x27-serial.c',
    'x27-spi.c',
    'x27-168-breakout.c'
]

objects = [ env.Object(src) for src in sources ]

elf_builder = env.Program('x27-168-breakout.elf', objects, LINKFLAGS=linker_flags)
hex_copier = env.Command("x27-168-breakout.hex", "x27-168-breakout.elf", "avr-objcopy -R .eeprom -O ihex $SOURCE $TARGET")

env.AlwaysBuild(elf_builder)
env.AlwaysBuild(hex_copier)

if 'upload-arduino' in COMMAND_LINE_TARGETS:
    port = ARGUMENTS["port"]
    upload_command = env.Command(None, "x27-168-breakout.hex", "avrdude -pm328 -carduino -P%s -Uflash:w:$SOURCE:a" % port)
    env.Alias("upload-arduino", upload_command)

elif 'upload-tiny' in COMMAND_LINE_TARGETS:
    upload_command = env.Command(None, "x27-168-breakout.hex", "avrdude -pm328 -cusbtiny -Uflash:w:$SOURCE:a")
    env.Alias("upload-tiny", upload_command)

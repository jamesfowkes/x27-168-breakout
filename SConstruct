import os

cppdefines = ['F_CPU=8000000UL']
cppflags = ['-Wall', '-Wextra', '-mmcu=atmega328p', '-g', '-Os']
linker_flags = ['-Wl,-gc-sections', '-Os']

env = Environment(ENV=os.environ, CC="avr-gcc", CPPFLAGS=cppflags, CPPDEFINES=cppdefines)

objects = [
	env.Object('x27-168-breakout.c'),
	env.Object('x27.c'),
	env.Object('x27-adc.c'),
	env.Object('x27-pwm.c'),
	env.Object('x27-serial.c'),
	env.Object('x27-spi.c')
]

elf_builder = env.Program('x27-168-breakout.elf', objects, LINKFLAGS=linker_flags)
hex_copier = env.Command("x27-168-breakout.hex", "x27-168-breakout.elf", "avr-objcopy -R .eeprom -O ihex $SOURCE $TARGET")

env.AlwaysBuild(elf_builder)
env.AlwaysBuild(hex_copier)

if 'upload' in COMMAND_LINE_TARGETS:
	upload_command = env.Command(None, "x27-168-breakout.hex", "avrdude -pm328p -carduino -Uflash:w:$SOURCE:a")
	env.Alias("upload", upload_command)

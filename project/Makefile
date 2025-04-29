# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h 
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c



all:snake.elf

#additional rules for files
snake.elf: ${COMMON_OBJECTS} main.o snake.o wdt_handler.o game.o lcdutils.o lcddraw.o buzzer.o led.o switches.o assembly_helper.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 
lcdutils.o: ../lcdLib/lcdutils.c

	${CC} ${CFLAGS} -c -o $@ $<



lcddraw.o: ../lcdLib/lcddraw.c

	${CC} ${CFLAGS} -c -o $@ $<

load: snake.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf *~

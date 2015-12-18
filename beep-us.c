/*
beep.c for DOS: Programmed by Masanori HATA <http://www.mihr.net> 2009-11-04~2012-11-24
Source Code from http://www.mihr.net/comp/dos/programming.html#sound

beep-us.c for DOS: Fork for support microsecond by Denir Li (2015-12-18)
Reference: http://www.delorie.com/djgpp/doc/libc/libc_844.html

compile: gcc beep-us.c -o beep-us.exe

usage:
	beep-us.exe <frequency> <microseconds>
	If you want to generate 262Hz (C4) sound for 0.5 seconds <500000 microseconds>:
	C:> beep-us.exe 262 500000
*/
#include <unistd.h>

#define SPEAKER_PORT  0x61
#define PIT_CONTROL   0x43
#define PIT_CHANNEL_2 0x42
#define PIT_FREQUENCY 0x1234DD

typedef unsigned char  BYTE;
typedef unsigned short WORD;

int main(int argc, char *argv[])
{
	WORD frequency = atoi(argv[1]);
	WORD counter   = PIT_FREQUENCY / frequency;
	BYTE spk_on, spk_off;

	outp(PIT_CONTROL, 0xB6); /* 10110110 means:  <cf. pit.txt of PC-GPE>
					Counter 0, Read/Load LSB then MSB, Mode 3 (square wave rate)
					and binary 16bit. */
	outp(PIT_CHANNEL_2, (BYTE)  counter);
	outp(PIT_CHANNEL_2, (BYTE) (counter >> 8));

	spk_on  = inp(SPEAKER_PORT) | 0x03; /* OR  00000011 */
	spk_off = spk_on            & 0xFC; /* AND 11111100 */
	outp(SPEAKER_PORT, spk_on);
	usleep(atoi(argv[2]));
	outp(SPEAKER_PORT, spk_off);

	return(0);
}

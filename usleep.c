/*
usleep.c for DOS: Programmed by Denir Li (2015-12-18)
Reference: http://www.delorie.com/djgpp/doc/libc/libc_844.html

compile: gcc usleep.c -o usleep.exe

usage:
	usleep.exe <microseconds>
	If you want to sleep for 0.5 seconds <500000 microseconds>:
	C:> usleep.exe 500000
*/
#include <unistd.h>

int main(int argc, char *argv[])
{
	usleep(atoi(argv[1]));

	return(0);
}

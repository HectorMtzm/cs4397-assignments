#include <windows.h>
#include <stdio.h>

void setCommand(char* p, char s) {
	// three-byte format: 11111110 - 00AAAAAA - ACCDDDDD
	// if the engine is addressed with 23 (in binary: 10111), then AAAAAAA will be 0010111

	// the first byte is OFE
	*p |= 1 << 1;
	*p |= 1 << 2;
	*p |= 1 << 3;
	*p |= 1 << 4;
	*p |= 1 << 5;
	*p |= 1 << 6;
	*p |= 1 << 7;

	// the second byte: 00AAAAAA
	//engine addressing code:   000010111
	*(p + 1) |= 1 ;
	*(p + 1) |= 1 << 1;
	*(p + 1) |= 1 << 3;

	// the third byte: 1CCDDDDD
	*(p + 2) |= 1 << 7;

	switch (s) {
		// remain part for the third byte
	case 'h': // h - horn 1:   0011100
		*(p + 2) |= 1 << 2;
		*(p + 2) |= 1 << 3;
		*(p + 2) |= 1 << 4;
		break;

	case 'b':		//brake:	0000111
		*(p + 2) |= 1;
		*(p + 2) |= 1 << 1;
		*(p + 2) |= 1 << 2;
		break;

	case 'e':		//bell:		0011101
		*(p + 2) |= 1;
		*(p + 2) |= 1 << 2;
		*(p + 2) |= 1 << 3;
		*(p + 2) |= 1 << 4;
        break;

	case 'a':	//accelerate	1003000	
		*(p + 2) |= 1 << 3;
		*(p + 2) |= 1 << 6;
		*(p + 2) |= 1 << 8;
		break;

	case 'd':   //Decelerate    1000010
		*(p + 2) |= 1 << 1;
		*(p + 2) |= 1 << 6;
		*(p + 2) |= 1 << 8;

        break;

	case 's':		//stop		1101000	
		*(p + 2) |= 1 << 6;
		*(p + 2) |= 1 << 3;
		*(p + 2) |= 1 << 7;
		break;

	case 't':		//start		1100111	
		*(p + 2) |= 1;
		*(p + 2) |= 1 << 1;
		*(p + 2) |= 1 << 2;
		*(p + 2) |= 1 << 5;
		*(p + 2) |= 1 << 6;
		*(p + 2) |= 1 << 7;
		*(p + 2) |= 1 << 8;
		break;

	default: break;
	}

}
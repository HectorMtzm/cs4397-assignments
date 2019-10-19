#include <windows.h>
#include <stdio.h>
#include "commands.c"

char menu(char);
int intAction(char);
char charAction(int);
void clearScreen();
void renderMenu();
int sendCommand();

int main()
{
    // Each command is in a three-byte command
	char bytes_to_send[3];
	bytes_to_send[0] = 0;
	bytes_to_send[1] = 0;
	bytes_to_send[2] = 0;

	char action = '-';
	renderMenu();
	while(1){
		action = menu(action);

		if(action == 'x')
			break;

		setCommand(bytes_to_send, action);

		int result = sendCommand(bytes_to_send);

		bytes_to_send[0] = 0;
		bytes_to_send[1] = 0;
		bytes_to_send[2] = 0;

		if(result > 0)
			return result;
	}

	return 0;
}

int sendCommand(char* bytes_to_send){
	
	// Declare variables and structures
	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };

	// Open the right serial port number, normally on this computer should be COM1
	fprintf(stderr, "Opening serial port...");

	hSerial = CreateFile(
		"\\\\.\\COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSerial == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	else fprintf(stderr, "OK\n");


	// Set device parameters (9600 baud, 1 start bit,
	// 1 stop bit, no parity)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error getting device state\n");
		CloseHandle(hSerial);
		return 1;
	}

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (SetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error setting device parameters\n");
		CloseHandle(hSerial);
		return 1;
	}

	// Set COM port timeout settings
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (SetCommTimeouts(hSerial, &timeouts) == 0)
	{
		fprintf(stderr, "Error setting timeouts\n");
		CloseHandle(hSerial);
		return 1;
	}

	// Send specified text (remaining command line arguments)
	DWORD bytes_written, total_bytes_written = 0;
	fprintf(stderr, "Sending bytes...");
	if (!WriteFile(hSerial, bytes_to_send, 3, &bytes_written, NULL))
	{
		fprintf(stderr, "Error\n");
		CloseHandle(hSerial);
		return 1;
	}
	fprintf(stderr, "%d bytes written\n", bytes_written);

    // Close serial port
	fprintf(stderr, "Closing serial port...");
	if (CloseHandle(hSerial) == 0)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	fprintf(stderr, "OK\n");

	// exit normally

	return 0;
}

void renderMenu(){
	char* mainMenu = "\n========================================\n|                                      |\n|       Aperature Research Labs:       |\n|             Train Control            |\n|               Mechanism              |\n|                                      |\n========================================\n";
	printf("%s", mainMenu);
	printf("\nWELCOME!\n");
}

char menu(char action){
	

	char* menuTitle = "AVAILABLE ACTIONS\n";
	char* delim = "---------------------------------------------------------\n";
	char* menuOptions = "    1. Horn (h)\n    2. Bell (e)\n    3. Acclerate (a)\n    4. Decelerate (d)\n    5. Break (b)\n    6. Start (t)\n    7. Stop (s)\n    8. Exit (x)\n";

	int valid = 1;

	char buffer[64];
	int choice;
	do{
		printf("%s", menuTitle);
		printf("%s", delim);
		printf("%s", menuOptions);
		printf("%s", delim);

		if(!valid)
			printf("\nERROR: Invalid menu option. Please choose from the given options.\n");

		printf("\n[Last Action]: %d", intAction(action));
		printf("\n[New Action]: "); 
		fgets(buffer, sizeof(buffer), stdin);
		choice = buffer[0] - 48;

		if(choice < 1 || choice > 8){
			valid = 0;
		} else {
			valid = 1;
		}

		clearScreen();

	} while(valid != 1);

	return charAction(choice);
}

char charAction(int intAction){
		switch(intAction){
		case 1: 
			return 'h';
		case 2:
			return 'e';
		case 3:
			return 'a';
		case 4:
			return 'd';
		case 5:
			return 'b';
		case 6:
			return 't';
		case 7:
			return 's';
		case 8:
			return 'x';
		default:
			return '-';
	}
}

int intAction(char charAction){
		switch(charAction){
		case 'h': 
			return 1;
		case 'e':
			return 2;
		case 'a':
			return 3;
		case 'd':
			return 4;
		case 'b':
			return 5;
		case 't':
			return 6;
		case 's':
			return 7;
		case 'x':
			return 8;
		default:
			return 0;
	}
}

void clearScreen(){
	int i;
	for(i = 0; i < 50; i++){
		printf("\n");
	}
}

// reference: https://batchloaf.wordpress.com/2013/02/13/writing-bytes-to-a-serial-port-in-c/

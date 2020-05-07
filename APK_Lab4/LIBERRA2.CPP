#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define COUNT 47

void soundGeneration() {
	int frequency[COUNT] = { 0, 208, 277, 311, 330, 370, 415, 370, 330, 311, 330,
		      		 0, 208, 277, 311, 330, 370, 415, 370, 330, 311, 466,
		      		 0, 208, 277, 311, 330, 370, 415, 370, 330, 311, 330,
		      		 0, 220, 440, 220, 415, 220, 370,
		      		 0, 208, 415, 208, 370, 208, 311 };
	int durability[COUNT] = { 250, 125, 125, 125, 125, 125, 125, 250, 250, 250, 250,
		     		  250, 125, 125, 125, 125, 125, 125, 250, 250, 250, 250,
		     		  250, 125, 125, 125, 125, 125, 125, 250, 250, 250, 250,
		    		  125, 125, 125, 125, 125, 125, 250,
		    		  125, 125, 125, 125, 125, 125, 250 };
	long unsigned base = 1193180;
	int frequencyCounter, kd;

	for (frequencyCounter = 0; frequencyCounter < COUNT; frequencyCounter++) {
		if(frequency[frequencyCounter] == 0){
			outp(0x43, 0xB6);
			delay(durability[frequencyCounter]);
			outp(0x61, inp(0x61) & 0xFC);
		}
		if(frequency[frequencyCounter] != 0){
			outp(0x43, 0xB6);
			kd = base / frequency[frequencyCounter];
			outp(0x42, kd % 256); 
			kd /= 256;
			outp(0x42, kd);

			outp(0x61, inp(0x61) | 3);
			delay(durability[frequencyCounter]);
			outp(0x61, inp(0x61) & 0xFC);
		}
	}
}

void statusWord() {
	printf("\n\nStatus word: \n");
	char stateWord[8];
	int Channel, state;
	int ports[] = { 0x40, 0x41, 0x42 };
	int controlWords[] = { 226, 228, 232 };   //11 10 001 0, 11 10 010 0, 11 10 100 0

	for (Channel = 0; Channel < 3; Channel++) {
		outp(0x43, controlWords[Channel]);
		state = inp(ports[Channel]);

		for (int i = 7; i >= 0; i--) {
			stateWord[i] = state % 2 + '0';
			state /= 2;
		}
		printf("\nChannel %d: ", Channel);
		for (i = 0; i < 8; i++) {
			printf("%c", stateWord[i]);
		}
		printf("\n");
	}
}

void divisionCoefficient() {
	unsigned long j;
	int Channel;
	int ports[] = { 0x40, 0x41, 0x42 };
	int controlWords[] = { 0, 64, 128 };
	unsigned kdLow, kdHigh, kd, max;

	printf("\n\nCoefficient of division: \n");

	for (Channel = 0; Channel < 3; Channel++) {
		kd = 0;
		max = 0;

		if (Channel == 2) {
			outp(0x61, inp(0x61) | 3);
		}

		for (j = 0; j < 65536; j++) {
			outp(0x43, controlWords[Channel]);
			kdLow = inp(ports[Channel]);
			kdHigh = inp(ports[Channel]);
			kd = kdHigh * 256 + kdLow;

			if (kd > max) {
				max = kd;
			}
		}

		if (Channel == 2) {
			outp(0x61, inp(0x61) & 0xFC);
		}
		printf("\nChannel %d: %X\n", Channel, max);
	}
}

void main() {
	char choice;
	do {
		printf("\n1. Sound generation\n2. Status word\n3. Coefficient of division\n0. Exit\n");
		printf("\nYour choise: ");
		scanf("%s", &choice);

		switch (choice) {
		case '1': soundGeneration(); break;
		case '2': statusWord(); break;
		case '3': divisionCoefficient(); break;
		}
	} while (choice != '0');
}
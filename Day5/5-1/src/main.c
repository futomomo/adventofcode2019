#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 1024

typedef enum opcodes {
	NUL = 0,
	ADD = 1,
	MUL = 2,
	IN = 3,
	OUT = 4,
	BRK = 99
} OPcode;

typedef enum modes {
	POS = 0,
	IMM = 1
} OPmode;

int prgMem[BUFSIZE] = {NUL};
// int prgArgCache[4] = {NUL};
int prgIP = 0; // instruction pointer
int prgEnd = 0; // program end pointer

int loadPrg(const char* fileName);
void runPrg();
int opAdd();
int opMul();
int opInput();
int opOutput();
int inline getMem(OPmode mode, int address);

int main(int argc, char* argv[])
{
	char* fileName = "input.txt";
	if(argc > 1)
		fileName = argv[1];

	if(!loadPrg(fileName))
		return -1;

	printf("Prg size %d units.\n", prgEnd);

	runPrg();

	return 0;
}


int loadPrg(const char* fileName)
{
	int memUnit = 0;
	FILE* storedMem = NULL;
	storedMem = fopen(fileName, "r");
	if(!storedMem)
	{
		printf("Unable to open program %s\n", fileName);
		return 0;
	}

	printf("Reading memory stored in file %s\n", fileName);
	for(int memPtr = 0; memPtr < BUFSIZE; memPtr++)
	{
		fscanf(storedMem, "%d", &memUnit);
		if(feof(storedMem))
		{
			printf("EOF on file.\n");
			prgEnd = (memPtr);
			break;
		}
		if(ferror(storedMem))
		{
			printf("ERROR: Error on reading file\n");
			return 0;
		}
		prgMem[memPtr] = memUnit;
		fgetc(storedMem);
	}
	if(prgEnd == 0)
	{
		printf("ERROR: prgEnd == 0\n");
		return 0;
	}
	return 1;
}


void runPrg()
{
	printf("Running program in memory\n");
	int operation = 0, panic = 0;
	while((prgIP < prgEnd) && !panic)
	{
		operation = prgMem[prgIP];
		printf("IP: $%-8d", prgIP);
		prgIP++;
		switch((operation%100))
		{
			case ADD:
				panic = opAdd();
				break;
			case MUL:
				panic = opMul();
				break;
			case IN:
				panic = opInput();
				break;
			case OUT:
				panic = opOutput();
				break;
			case BRK:
				printf("BRK: HALTING\n");
				return;
				break;
			default:
				printf("PANIC\n");
				panic = prgIP;
				break;
		}
		//getchar();
	}
	if(panic)
		printf("Panic: %d\n", panic);
}


int opAdd()
{
	int augend, addend, sum, address;
	int op = prgMem[prgIP-1];
	OPmode modeArg1 = (OPmode)((op/100)%10), modeArg2 = (OPmode)((op/1000)%10);
	augend = getMem(modeArg1, prgIP);
	addend = getMem(modeArg2, prgIP+1);
	address = prgMem[prgIP+2];
	sum = augend + addend;
	printf("ADD #%d #%d -> #%d $%d\n",augend, addend, sum, address);
	printf("RAW: %05d,%d,%d,%d\n", prgMem[prgIP-1], prgMem[prgIP], prgMem[prgIP+1], prgMem[prgIP+2]);
	prgMem[address] = sum;
	prgIP += 3;
	return 0;
}

int opMul()
{
	int multiplicand, multiplier, product, address;
	int op = prgMem[prgIP-1];
	OPmode modeArg1 = (OPmode)((op/100)%10), modeArg2 = (OPmode)((op/1000)%10);
	multiplicand = getMem(modeArg1, prgIP);
	multiplier = getMem(modeArg2, prgIP+1);
	address = prgMem[prgIP+2];
	product = multiplicand * multiplier;
	printf("MUL #%d #%d -> #%d $%d\n", multiplicand, multiplier, product, address);
	printf("RAW: %05d,%d,%d,%d\n", prgMem[prgIP-1], prgMem[prgIP], prgMem[prgIP+1], prgMem[prgIP+2]);
	prgMem[address] = product;
	prgIP += 3;
	return 0;
}

int opInput()
{
	int inVal = 0, address = 0;
	printf("IN: ?");
	while(scanf("%d", &inVal) != 1);
	address = prgMem[prgIP];
	printf("IN -> #%d $%d\n", inVal, address);
	printf("RAW: %05d,%d\n", prgMem[prgIP-1], prgMem[prgIP]);
	prgMem[address] = inVal;
	prgIP += 1;
	return 0;
}

int opOutput()
{
	OPmode mode = (OPmode)((prgMem[prgIP-1]/100)%10);
	int value = getMem(mode, prgIP);
	printf("OUT: #%d\n", value);
	printf("RAW: %05d,%d\n", prgMem[prgIP-1], prgMem[prgIP]);
	if(prgMem[prgIP+1] != 99 && value)
		return -4;
	prgIP += 1;
	return 0;
}


int inline getMem(OPmode mode, int address)
{
	switch(mode)
	{
		case POS:
			return prgMem[prgMem[address]];
			break;
		case IMM:
			return prgMem[address];
			break;
		default:
			return 0;
	}
}

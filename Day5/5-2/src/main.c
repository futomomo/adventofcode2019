#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 1024

typedef int opFunc(void);

typedef enum opcodes {
	NUL = 0,
	ADD = 1,
	MUL = 2,
	IN = 3,
	OUT = 4,
	JOT = 5, // jump on true
	JOF = 6, // jump on false
	SLT = 7, // set on less than
	SEQ = 8, // set on equal
	BRK = 99
} OPcode;

typedef enum modes {
	POS = 0,
	IMM = 1
} OPmode;

int loadPrg(const char* fileName);
void runPrg();
int inline getMem(OPmode mode, int address);
void getArgs(int count);

// ---- operation functions ----
int opAdd();
int opMul();
int opInput();
int opOutput();
int opJOT();
int opJOF();
int opSLT();
int opSEQ();
opFunc* opFunctions[8] = {&opAdd, &opMul, &opInput, &opOutput, &opJOT, &opJOF, &opSLT, &opSEQ};


int prgMem[BUFSIZE] = {NUL};
int argCache[2] = {0};
int prgIP = 0; // instruction pointer
int prgEnd = 0; // program end pointer


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
	printf("Running program in memory.\n");
	int panic = 0, instruction = 0;
	while((prgIP < prgEnd) && !panic)
	{
		instruction = (prgMem[prgIP]%100)-1;
		if(instruction == 98 || instruction < 0 || 7 < instruction)
		{
			printf("Breaking due to instruction: %d\n", instruction+1);
			break;
		}

		printf("IP: $%-8d", prgIP);
		++prgIP;
		panic = (*(opFunctions[instruction]))();

		//getchar();
	}
	if(panic)
		printf("Panic: %d\n", panic);
}


int opAdd()
{
	int augend, addend, sum, address;
	getArgs(2);
	augend = argCache[0];
	addend = argCache[1];
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
	getArgs(2);
	multiplicand = argCache[0];
	multiplier = argCache[1];
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

int opJOT()
{
	getArgs(2);
	int IP = prgIP;
	int boolean = argCache[0];
	printf("JOT: #%d:%s IP $%d -> ", boolean, (boolean ? "JUMP":"NO JUMP"), prgIP);
	if(boolean)
	{
		prgIP = argCache[1];
		printf("$%d\n", prgIP);
	}
	else
	{
		printf("$%d\n", prgIP);
		prgIP += 2;
	}
	printf("RAW: %05d,%d,%d\n", prgMem[IP-1], prgMem[IP], prgMem[IP+1]);
	return 0;
}

int opJOF()
{
	getArgs(2);
	int IP = prgIP;
	int boolean = argCache[0];
	printf("JOF: #%d:%s IP $%d -> ", boolean, (!boolean ? "JUMP":"NO JUMP"), prgIP);
	if(!boolean)
	{
		prgIP = argCache[1];
		printf("$%d\n", prgIP);
	}
	else
	{
		printf("$%d\n", prgIP);
		prgIP += 2;
	}
	printf("RAW: %05d,%d,%d\n", prgMem[IP-1], prgMem[IP], prgMem[IP+1]);
	return 0;
}


int opSLT()
{
	getArgs(2);
	printf("SEQ: #%d==#%d:%s $%d\n", argCache[0], argCache[1], (argCache[0] == argCache[1]?"TRUE":"FALSE"), prgMem[prgIP+2]);
	if(argCache[0] < argCache[1])
		prgMem[prgMem[prgIP+2]] = 1;
	else
		prgMem[prgMem[prgIP+2]] = 0;
	printf("RAW: %05d,%d,%d,%d\n", prgMem[prgIP-1], prgMem[prgIP], prgMem[prgIP+1], prgMem[prgIP+2]);
	prgIP += 3;
	return 0;
}

int opSEQ()
{
	getArgs(2);
	printf("SEQ: #%d==#%d:%s $%d\n", argCache[0], argCache[1], (argCache[0] == argCache[1]?"TRUE":"FALSE"), prgMem[prgIP+2]);
	if(argCache[0] == argCache[1])
		prgMem[prgMem[prgIP+2]] = 1;
	else
		prgMem[prgMem[prgIP+2]] = 0;
	printf("RAW: %05d,%d,%d,%d\n", prgMem[prgIP-1], prgMem[prgIP], prgMem[prgIP+1], prgMem[prgIP+2]);
	prgIP += 3;
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

// assumes prgIP points at the first argument
// resolves modes and puts the resolved values in argCache
// only handles up to 2 arguments obviously
void inline getArgs(int count)
{
	OPmode modes[2] = {(OPmode)((prgMem[prgIP-1]/100)%10), (OPmode)((prgMem[prgIP-1]/1000)%10)};
	for(int i = 0; i < count; i++)
		argCache[i] = getMem(modes[i], prgIP+i);
}

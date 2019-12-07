#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prg.h"

enum opcodes {
	ADD = 1,
	MUL = 2,
	BRK = 99
};

void prgopAddMem(program* p);
void prgopMulMem(program* p);

program* prgLoad(const char* filename, const char* mode)
{
	program* prog = malloc(sizeof(program));
	if(!prog)
	{
		printf("Failed to allocate program struct memory.\n");
		return NULL;
	}
	prog->instPtr = 0;
	prog->prgSize = 0;
	prog->prg = NULL;
	long prgFileLen = 0L;
	size_t readCount = 0;
	char* programString = NULL;
	FILE* prgfile = fopen(filename, mode);
	if(prgfile == NULL)
		return NULL;

	fseek(prgfile, 0L, SEEK_END);
	prgFileLen = ftell(prgfile);
	rewind(prgfile);

	if(prgFileLen == 0L)
		return NULL;

	programString = malloc(sizeof(char)*(prgFileLen+1L));
	if(!programString)
	{
		printf("Failed to allocate program string memory.\n");
		return NULL;
	}

	readCount = fread(programString, sizeof(char), prgFileLen+1L, prgfile);
	if(ferror(prgfile))
	{
		printf("Error reading from file.\n");
		return NULL;
	}
	if(!feof(prgfile))
	{
		// TODO: Handle what is essentially ftell() giving too short of a length
		printf("Did not reach end of file while reading.\n");
		return NULL;
	}

	fclose(prgfile);

	size_t stringLength = strlen(programString);
	if(stringLength == 0L)
	{
		printf("programString length is zero.\n");
		return NULL;
	}

	int delimCount = 1;

	for(size_t i = 0L; i < stringLength; i++)
	{
		if(programString[i] == ',')
			++delimCount;
	}

	prog->prg = malloc(sizeof(int)*(delimCount));
	// printf("Allocating memory for prog->prg of size %d\n", sizeof(int)*(delimCount+1));

	char* token = strtok(programString, ",");
	int prgByte, prgSize;
	prgByte = prgSize = 0;
	while(token != NULL && prgSize < delimCount+1)
	{
		sscanf(token, "%d", &prgByte);
		prog->prg[prgSize] = prgByte;
		//printf("%d,", prog->prg[prgSize]);
		token = strtok(NULL, ",");
		++prgSize;
	}
	prog->prgSize = prgSize;

	/*printf("prgSize: %d\ndelimCount+1: %d\n", prgSize, delimCount+1);

		printf("--IN prgLoad--\nprg: 0x%x\nprg->instPtr: 0x%x\nprg->prgSize: 0x%x\nprg->prg: 0x%x\n",
		prog, (prog->instPtr), (prog->prgSize), (prog->prg));
		*/
	return prog;
}

void prgFree(program* p)
{
	//printf("Freeing p->prg\n");
	free(p->prg);
	//printf("Freeing p\n");
	free(p);
}

void prgRun(program* p)
{
	int operation, panic;
	operation = panic = 0;
	while((p->instPtr < p->prgSize) && panic == 0)
	{
		operation = p->prg[p->instPtr];
		printf("IP: %d      ", p->instPtr);


		switch(operation)
		{
			case ADD:
				prgopAddMem(p);
				break;
			case MUL:
				prgopMulMem(p);
				break;
			case BRK:
				printf("BRK: HALTING\n");
				panic = 1;
				break;
			default:
				printf("PANIC\n");
				panic = -99;
				break;
		}
	}
	if(panic != 0)
		printf("Panic: %d\n", panic);
}

void prgopAddMem(program* p)
{
	// TODO: Validate prg access
	int result, augend, addend, outmem;
	augend = p->prg[p->instPtr+1];
	addend = p->prg[p->instPtr+2];
	outmem = p->prg[p->instPtr+3];
	result = p->prg[augend] + p->prg[addend];
	printf("ADD $%d + $%d, STORE IN $%d\n", augend, addend, outmem);
	p->prg[outmem] = result;
	p->instPtr += 4;
}

void prgopMulMem(program* p)
{
	// TODO: Validate prg access
	int product, multiplicand, multiplier, outmem;
	multiplicand = p->prg[p->instPtr+1];
	multiplier   = p->prg[p->instPtr+2];
	outmem       = p->prg[p->instPtr+3];
	product = p->prg[multiplicand] * p->prg[multiplier];
	printf("MULTIPLY $%d * $%d, STORE IN $%d\n", multiplicand, multiplier, outmem);
	p->prg[outmem] = product;
	p->instPtr += 4;
}


#include <stdio.h>
#include <string.h>
#include "prg.h"

#define BUFSIZE 1024

int main(int argc, char* argv[])
{
  program* prog = prgLoad("input.txt", "r");

  if(!prog)
  {
    printf("Failed to get program struct pointer.\n");
    return -1;
  }
  program* bakprog = copyPrg(prog);

  int inNoun, inVerb, inputFound;
  inNoun = inVerb = inputFound = 0;
  for(int noun = 0; noun < 100; noun++)
  {
    for(int verb = 0; verb < 100; verb++)
    {
      printf("Running prog with noun: %d and verb: %d\n", noun, verb);

      prog->prg[1] = noun;
      prog->prg[2] = verb;
      prgRun(prog);
      printf("Program final position 0: %d\n", prog->prg[0]);
      if(prog->prg[0] == 19690720)
      {
	inNoun = noun;
	inVerb = verb;
	inputFound = 1;
	break;
      }
      prgFree(prog);
      prog = copyPrg(bakprog);
      if(!prog)
      {
	printf("Copy prg unsuccessfull.\n");
	return -1;
      }
    }
    if(inputFound)
      break;
  }
  
  prgFree(prog);
  prgFree(bakprog);
  printf("Noun: %d\nVerb: %d\n", inNoun, inVerb);

  return 0;
}

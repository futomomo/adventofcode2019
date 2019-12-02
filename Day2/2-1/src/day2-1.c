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
/*printf("--OUT prgLoad--\nprog: 0x%x\nprog->instPtr: 0x%x\nprog->prgSize: 0x%x\nprog->prg: 0x%x\n",
      prog, (prog->instPtr), (prog->prgSize), prog->prg);
  printf("Freeing pointer.\n");
  printf("0x%x\n", (prog->prg)[0]);
  for(int i = 0; i < prog->prgSize; i++)
  {
    printf("%d,", prog->prg[i]);
  }
  */
  printf("Running program...\n");
  prgRun(prog);
  printf("Position 0: %d\n", prog->prg[0]);
  printf("Freeing program...\n");
  prgFree(prog);
  return 0;
}

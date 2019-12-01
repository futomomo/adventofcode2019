#include <stdio.h>

int main(int arc, char* argv[])
{
  FILE* input = fopen("input.txt", "r");
  if (input == NULL)
  {
    printf("fopen returned null pointer");
    return -1;
  }

  unsigned int mass = 0;
  unsigned int fuel = 0;
  unsigned int totalFuel = 0;

  while(fscanf(input, "%u", &mass) != EOF)
  {
    fuel = (mass/3)-2;
    totalFuel += fuel;
    printf("Mass: %u, Fuel: %u\n", mass, fuel);
  }

  if(feof(input))
    printf("End of file.\n");

  if(ferror(input))
    printf("Error indicator set.\n");

  printf("Total fuel: %u\n", totalFuel);

  fclose(input);
  return 0;
}

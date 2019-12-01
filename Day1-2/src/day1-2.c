#include <stdio.h>

int moduleTotalFuel(int fuel);

int main(int argc, char* argv[])
{
  FILE* input = fopen("input.txt", "r");
  if (input == NULL)
  {
    printf("fopen returned null pointer");
    return -1;
  }

  int mass = 0;
  int fuel = 0;
  int totalFuel = 0;

  while(fscanf(input, "%d", &mass) != EOF)
  {
    fuel = moduleTotalFuel((mass/3)-2);
    totalFuel += fuel; 
    printf("Mass: %d, Fuel: %d\n", mass, fuel);
  }

  if(feof(input))
    printf("End of file.\n");

  if(ferror(input))
    printf("Error indicator set.\n");

  printf("Total fuel for real this time: %d\n", totalFuel);

  fclose(input);
  return 0;
}


int moduleTotalFuel(int fuel)
{
  if(fuel <= 0)
    return 0;
  else
    return fuel+moduleTotalFuel((fuel/3)-2);
}


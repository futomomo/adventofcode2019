#include <stdio.h>


int hasRepeatDigit();
int hasDecreasingDigit();
int failCritera();

int rangeEnd = 784956;
char number[64] = "";

int main(int argc, char* argv[])
{
	int matchingPassw = 0;
	for(int rangeCount = 240298; rangeCount <= rangeEnd; rangeCount++)
	{
		sprintf(number, "%d", rangeCount); // turn number into string
		if(failCritera())
			continue;
		matchingPassw++;
	}

	printf("Number of passwords matching the criteria: %d\n", matchingPassw);
	return 0;
}

// returns 1 on criteria fail
int failCritera()
{
	int c, last = 0;
	int repeat = 1; // 1 no repeating, 0 has repeating
	for(int i = 0; i < 6; i++)
	{
		c = (int)number[i];
		if(c < last) // check for decreasing digits
			return 1;
		if(c == last)
			repeat = 0;
		last = c;
	}
	return repeat;	
}

int hasDecreasingDigit()
{
	int c, last = '0';
	for(int i = 0; i < 6; i++)
	{
		c = (int)number[i];
		if(c < last)
			return 1;
		last = c;
	}
	return 0;
}

int hasRepeatDigit()
{
	char last = ' ', c;
	for(int i = 0; i < 6; i++)
	{
		c = number[i];
		if(c == last)
			return 1;
		last = c;
	}
	return 0;
}

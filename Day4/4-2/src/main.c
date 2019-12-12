#include <stdio.h>

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
// criteria: both points assume left to right parsing of the number
// 		no decreasing digits
// 		repeating digits but ONLY if it's twice in a row, more doesn't count
int failCritera()
{
	int c, last = number[0];
	int repeat = 1; // 1 no repeating, 0 has repeating
	int repeatCount = 0; // if this is higher than 1 repeat is 1
	for(int i = 1; i < 6; i++)
	{
		c = (int)number[i];
		if(c < last) // check for decreasing digits
			return 1;
		if(c != last)
		{
			if(repeatCount == 1)
				repeat = 0;
			repeatCount = 0;
		}
		if(c == last)
			repeatCount++;
		last = c;
	}
	if(repeatCount == 1)
		repeat = 0;
	return repeat;
}

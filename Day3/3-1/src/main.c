#include <stdio.h>
#include <limits.h>

typedef struct vec2
{
	int x, y;
} Vec2;

typedef struct line
{
	Vec2 pos1, pos2;
} Line;

int lineIntersect(Line line1, Line line2);

Vec2 wires[2][302] = {0,0};
Vec2 lastPos;
int shortestDist = INT_MAX;

int main(void)
{
	char direction;
	int length, delim, wireIndex;

	FILE* input = fopen("input.txt", "r");
	if(!input)
	{
		printf("Failed to get input.\n");
		return -1;
	}
	for(int i = 0; i < 2; i++)
	{
		lastPos = (struct vec2){0,0};
		wires[i][0] = lastPos;
		wireIndex = 1;
		do
		{
			fscanf(input, "%c%d", &direction, &length);
			/* update last position */
			switch(direction)
			{
				case 'U':
					lastPos.y += length;
						break;
				case 'R':
					lastPos.x += length;
						break;
				case 'D':
					lastPos.y -= length;
						break;
				case 'L':
					lastPos.x -= length;
						break;
			}
			wires[i][wireIndex] = lastPos;
			// printf("x: %d, y: %d\n", wires[i][wireIndex].x, wires[i][wireIndex].y);
			// it only counts if a wire crosses another
			if(i > 0 && wireIndex > 0)
			{
				Line currentLine = {wires[i][wireIndex-1], wires[i][wireIndex]};
				// both wires are 302 long so just hardcode it
				for(int j = 1; j < 302; j++)
				{
					Line testLine = {wires[i-1][j-1], wires[i-1][1]};
					int dist = lineIntersect(currentLine, testLine);
					if(dist < shortestDist)
						shortestDist = dist;
				}
			}
			++wireIndex;
		} while((delim = fgetc(input)) == ',' && !ferror(input));

		printf("Wire done, size: %d\n", wireIndex);
		if(feof(input))
		{
			printf("Reached end of file");
			break;
		}
		if(ferror(input))
		{
			printf("File error");
			break;
		}
	}


	fclose(input);
	return 0;
}

int lineIntersect(Line line1, Line line2)
{
	int manhatDist = 0;
	int noCollide = (line1.pos1.x -line1.pos2.x)*(line2.pos1.y - line2.pos2.y) - (line1.pos1.y - line1.pos2.y)*(line2.pos1.x-line2.pos2.x);
	if(noCollide == 0)
		return INT_MAX;
}

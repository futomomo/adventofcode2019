#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct _point
{
	double x, y;
} Point;

int ptIntersect(Point a1, Point a2, Point b1, Point b2, Point* c);

Point wires[2][302] = {{{0.0l,0.0l}}};
Point lastPos = {0.0l, 0.0l};
double shortestDist = 0.0l;

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
		lastPos = (Point){0,0};
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
			printf("x: %f, y: %f\n", wires[i][wireIndex].x, wires[i][wireIndex].y);
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
			return -1;
		}
	}

	fclose(input);

	Point intersect = {0.0l, 0.0l};
	int manhatDist = 0, valid = 0;
	for(int i = 1; i < wireIndex; i++)
	{
		for(int j = 1; j < wireIndex; j++)
		{
			if(i == 1 && j == 1)
				continue;
			valid = ptIntersect(wires[0][i-1], wires[0][i], wires[1][j-1], wires[1][j], &intersect);
			if(valid)
			{
			}
		}
	}


	return 0;
}

// returns 1 on intersection found and 0 on not found
int ptIntersect(Point a1, Point a2, Point b1, Point b2, Point* c)
{
	(*c) = (Point){0.0l, 0.0l};
	double denom = (a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x);
	if(denom == 0)
		return 0;

	double t = ((a1.x - b1.x) * (b1.y - b2.y) - (a1.y - b1.y) * (b1.x - b2.x))/denom;
	double u = ((a1.x - a2.x) * (a1.y - b1.y) - (a1.y - a2.y) * (a1.x - b1.x))/denom;
	if(t < 0.0l || 1.0l < t || u < 0.0l || 1.0l < u)
		return 0;

	c->x = (a1.x + (t * (a2.x - a1.x)));
	c->y = (a1.y + (t * (a2.y - a1.y)));
	return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct
{
	double x, y;
} Point;


double lineIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
Point intersectPoint(Point a1, Point a2, Point b1, Point b2);

Point wires[2][302] = {0,0};
Point lastPos;
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

	printf("%f, %f, %f, %f\n", wires[0][0].x, wires[0][0].y, wires[1][0].x, wires[1][0].y);
	double intersect = 0.0l;
	for(int i = 1; i < wireIndex; i++)
	{
		for(int j = 1; j < wireIndex; j++)
		{
			intersect = lineIntersect(wires[0][i-1].x, wires[0][i-1].y, wires[0][i].x, wires[0][i].y, wires[1][j-1].x, wires[1][j-1].y, wires[1][j].x, wires[1][j].y);
			if(intersect != 0.0l)
				printf("Manhattan length: %f\n", intersect);
		}
	}


	return 0;
}

// returns 0.0l when there's no intersection
double lineIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	double denominator = (double)((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
	//printf("denominator: %f\n", denominator);
	if(denominator == 0.0l)
		return 0.0l;

	double t = (double)((x1-x3)*(y3-y4)-(y1-y3)*(x3-x4));
	t = t/denominator;
	double u = (double)((x1-x2)*(y1-y3)-(y1-y2)*(x1-x3));
	u = -(u/denominator);

	if(0.0l >= t || t >= 1.0l || 0.0l >= u || u >= 1.0l)
		return 0.0l;

	double x, y;
	x = ((double)x1+(t*(double)(x2-x1)));
	y = ((double)y1+(t*(double)(y2-y1)));

	return 0.0l;
}


Point intersectPoint(Point a1, Point a2, Point b1, Point b2)
{
	Point c;

}

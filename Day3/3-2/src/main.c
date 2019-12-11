#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct _point
{
	double x, y;
} Point;

typedef enum _direction
{
	LEFT = 'L',
	UP = 'U',
	RIGHT = 'R',
	DOWN = 'D'
} Direction;

typedef struct _vec2
{
	Point pos;
	Direction direction;
	int size;
} Vec2;


int ptIntersect(Point a1, Point a2, Point b1, Point b2, Point* c);
int vecIntersect(Vec2 vector1, Vec2 vector2, Point* c);

Vec2 vectors[2][400];
int vectorIndex = 0;
Point wires[2][302] = {{{0.0,0.0}}};
Point lastPos = {0.0, 0.0};
Point collisionList[1000] = {{0.0,0.0}};
int collisions = 0;
double shortestDist = 0.0;

void printPoint(Point p)
{
	printf("x: %f, y: %f\n", p.x, p.y);
}

void printVec2(Vec2 vector)
{
	printPoint(vector.pos);
	printf("Raw: %c%d\n", (char)vector.direction, vector.size);
}

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
		lastPos = (Point){0.0,0.0};
		vectorIndex = 0;
		do
		{
			fscanf(input, "%c%d", &direction, &length);
			// add direction and size
			vectors[i][vectorIndex].direction = (Direction)direction;
			vectors[i][vectorIndex].size = length;
			vectors[i][vectorIndex].pos = lastPos;
			switch(direction)
			{
				case UP:
					lastPos.y += (double)length;
					break;
				case RIGHT:
					lastPos.x += (double)length;
					break;
				case DOWN:
					lastPos.y -= (double)length;
					break;
				case LEFT:
					lastPos.x -= (double)length;
					break;
				default:
						printf("ERROR: Unexpected direction.\n");
						exit(-1);
						break;

			}
			printf("wire: %d, index: %d\n", i, vectorIndex);
			printVec2(vectors[i][vectorIndex]);
			printPoint(lastPos);
			printf("\n");
			vectorIndex++;
		} while((delim = fgetc(input)) == ',' && !ferror(input));

		printf("Wire done, size: %d\n", vectorIndex);
		getchar();
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
	input = NULL;

	int valid;
	Point result;
	double manhatDist = 0.0, shortestDist = 0.0;
	for(int i = 0; i < vectorIndex; i++)
	{
		for(int cmpIndex = 0; cmpIndex < vectorIndex; cmpIndex++)
		{
			if(i == 0 && cmpIndex == 0)
				continue;
			valid = vecIntersect(vectors[0][i], vectors[1][cmpIndex], &result);
			if(valid == 1)
			{
				manhatDist = fabs(result.x) + fabs(result.y);
				if(manhatDist  < shortestDist || shortestDist == 0.0)
					shortestDist = manhatDist;
				//printf("Vectors collided in [0][%d] -> [1][%d]:\n", i, cmpIndex);
				//printVec2(vectors[0][i]);
				//printVec2(vectors[1][cmpIndex]);
				//printPoint(result);
				//getchar();
			}
		}
	}

	printf("Shortest distance found: %f\n", shortestDist);

	return 0;
}


int vecIntersect(Vec2 vector1, Vec2 vector2, Point* c)
{
	Point a1, a2, b1, b2;
	a1 = vector1.pos; a2 = vector1.pos;
	b1 = vector2.pos; b2 = vector2.pos;
	switch(vector1.direction)
	{
		case UP:
			a2.y += (double)vector1.size;
			break;
		case RIGHT:
			a2.x += (double)vector1.size;
			break;
		case DOWN:
			a2.y -= (double)vector1.size;
			break;
		case LEFT:
			a2.x -= (double)vector1.size;
			break;
		default:
			printf("ERROR: Unexpected direction.\n");
			exit(-1);
			break;
	}
	switch(vector2.direction)
	{
		case UP:
			b2.y += (double)vector2.size;
			break;
		case RIGHT:
			b2.x += (double)vector2.size;
			break;
		case DOWN:
			b2.y -= (double)vector2.size;
			break;
		case LEFT:
			b2.x -= (double)vector2.size;
			break;
		default:
			printf("ERROR: Unexpected direction.\n");
			exit(-1);
			break;
	}
	//printf("Testing intersection...\n");
	//printPoint(a1);
	//printPoint(a2);
	//printPoint(b1);
	//printPoint(b2);

	return ptIntersect(a1, a2, b1, b2, c);
}

// returns 1 on intersection found and 0 on not found
int ptIntersect(Point a1, Point a2, Point b1, Point b2, Point* c)
{
	(*c) = (Point){0.0, 0.0};
	double denom = ((a1.x - a2.x) * (b1.y - b2.y)) - ((a1.y - a2.y) * (b1.x - b2.x));
	if(denom == 0)
		return 0;

	double t = ((a1.x - b1.x) * (b1.y - b2.y) - (a1.y - b1.y) * (b1.x - b2.x))/denom;
	double u = -1.0*(((a1.x - a2.x) * (a1.y - b1.y) - (a1.y - a2.y) * (a1.x - b1.x))/denom);
	if(t < 0.0 || 1.0 < t || u < 0.0 || 1.0 < u)
		return 0;

	c->x = (a1.x + (t * (a2.x - a1.x)));
	c->y = (a1.y + (t * (a2.y - a1.y)));
	return 1;
}

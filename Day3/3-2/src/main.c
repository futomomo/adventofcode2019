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


// steps is the total amount of steps taken until this node is reached
typedef struct _vec2
{
	Point pos;
	Direction direction;
	int size;
	int steps;
} Vec2;


int ptIntersect(Point a1, Point a2, Point b1, Point b2, Point* c);
int vecIntersect(Vec2 vector1, Vec2 vector2, Point* c);

void printPoint(Point p)
{
	printf("x: %f, y: %f\n", p.x, p.y);
}

void printVec2(Vec2 vector)
{
	printPoint(vector.pos);
	printf("Raw: %c%d, steps: %d\n", (char)vector.direction, vector.size, vector.steps);
}

Vec2 vectors[2][400];
int vectorIndex = 0;
int stepCount = 0;
Point lastPos = {0.0, 0.0};
Point collisionList[1000] = {{0.0,0.0}};
int collisions = 0;
double shortestDist = 0.0;

int main(int argc, char* argv[])
{
	char direction;
	char* fileName = "input.txt";
	int length, delim;
	if(argc > 1)
		fileName = argv[1];

	printf("Opening %s\n", fileName);
	FILE* input = fopen(fileName, "r");
	if(!input)
	{
		printf("Failed to open input file.\n");
		return -1;
	}
	for(int i = 0; i < 2; i++)
	{
		lastPos = (Point){0.0,0.0};
		vectorIndex = 0;
		stepCount = 0;
		do
		{
			fscanf(input, "%c%d", &direction, &length);
			// add direction and size
			vectors[i][vectorIndex].direction = (Direction)direction;
			vectors[i][vectorIndex].size = length;
			vectors[i][vectorIndex].pos = lastPos;
			vectors[i][vectorIndex].steps = stepCount;
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
			//printf("wire: %d, index: %d\n", i, vectorIndex);
			//printVec2(vectors[i][vectorIndex]);
			//printPoint(lastPos);
			//printf("\n");
			stepCount += length;
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
	int minSteps = 0, steps = 0;
	for(int i = 0; i < vectorIndex; i++)
	{
		for(int cmpIndex = 0; cmpIndex < vectorIndex; cmpIndex++)
		{
			// skip first inital wire vectors since they collide at 0,0
			// (although the collide check function doesn't allow 2 parrallel lines to collide anyway)
			if(i == 0 && cmpIndex == 0)
				continue;
			valid = vecIntersect(vectors[0][i], vectors[1][cmpIndex], &result);
			if(valid == 1)
			{
				printf("Collision detected at [0][%d], [1][%d]:\n", i, cmpIndex);
				printVec2(vectors[0][i]);
				printVec2(vectors[1][cmpIndex]);
				printPoint(result);
				steps = 0;
				// calculate sum of steps taken for both wires to get here
				// total amount of steps taken up until the vector is in each vec2 struct
				// need to calculate steps taken from this point until collision for both and add to get
				// total steps taken until collision
				steps = vectors[0][i].steps + vectors[1][cmpIndex].steps;
				steps += abs((int)vectors[0][i].pos.x - (int)result.x) + abs((int)vectors[0][i].pos.y - (int)result.y);
				steps += abs((int)vectors[1][cmpIndex].pos.x - (int)result.x) + abs((int)vectors[1][cmpIndex].pos.y - (int)result.y);
				printf("Total steps taken until collision: %d\n\n", steps);
				if(steps < minSteps || minSteps == 0)
					minSteps = steps;
				//getchar();
			}
		}
	}

	printf("Shortest step count found: %d\n", minSteps);

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
	if(t < 0.0 || 1.0 < t)
		return 0;
	double u = -1.0*(((a1.x - a2.x) * (a1.y - b1.y) - (a1.y - a2.y) * (a1.x - b1.x))/denom);
	if(u < 0.0 || 1.0 < u)
		return 0;

	c->x = (a1.x + (t * (a2.x - a1.x)));
	c->y = (a1.y + (t * (a2.y - a1.y)));
	return 1;
}

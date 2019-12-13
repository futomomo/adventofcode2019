#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define LISTMAX 3000

struct planet_;

typedef struct planet_
{
	char name[4]; // all planets have 3 letter names
	int steps;
	int parentCount;
	struct planet_* child; // pointer to planet this planet is orbiting
	struct planet_* parent[16]; // pointer to planets orbiting this one
} Planet;

// hash table would probably be better
// in practice the 'list' is treated like a global stack to store newly found
// planets on
Planet list[LISTMAX];
Planet* edges[LISTMAX/2];
int listSize = 0;
int edgeSize = 0;
int totalOrbits = 0;

int findPlanet(char* name);
int createPlanet(char* name);
void printPlanet(Planet p);
void traverseTree(Planet* pQueued[], int len, int step);

int main(int argc, char* argv[])
{
	char fileName[64] = "input.txt";
	if(argc > 1)
		strcpy(fileName, argv[1]);
	FILE* input = fopen(fileName, "r");
	if(!input)
		return -1;

	int index = 0, orbitIndex = 0;
	char planet[4] = "", orbiter[4] = "";
	while(fscanf(input, "%3s)%3s\n", planet, orbiter) != EOF && !ferror(input))
	{
		printf("%d: %3s:%3s\n",listSize, planet, orbiter);
		index = findPlanet(planet);
		if(index == -1)
			index = createPlanet(planet);
		orbitIndex = findPlanet(orbiter);
		if(orbitIndex == -1)
			orbitIndex = createPlanet(orbiter);

		list[index].parent[list[index].parentCount] = &(list[orbitIndex]);
		list[index].parentCount += 1;
		list[orbitIndex].child = &(list[index]);
	}
	fclose(input);

	printf("Unique planets: %d\n", listSize);
	int totalParents = 0;
	Planet* root;
	// populate all the egdes and root
	for(int i = 0; i < listSize; i++)
	{
		totalParents += list[i].parentCount;
		if(list[i].parentCount == 0)
		{
			printPlanet(list[i]);
			printf("ADDING TO EDGE LIST\n--------\n");
			edges[edgeSize] = &(list[i]);
			++edgeSize;
		}
		if(list[i].child == NULL)
			root = &(list[i]);
	}
	// if a planet has a parent, that is a direct orbit which means in total_orbits = direct + indirect,
	// direct can be substituted with total parents
	totalOrbits += totalParents;
	printf("Total parents: %d\n", totalParents);
	printf("Total edges: %d\n", edgeSize);
	assert(root);
	printf("Root planet: %s\n", root->name);

	getchar();

	// indirect orbits can be calculated by stepping through the tree, adding 1 to an accumulator and then
	// associating it with the current planet node with each step. Each planet node will then contain the amount of
	// steps (orbits) encountered until that node is reached. By subtracting 1 you get the total indirect orbits
	// for the node
	printf("Traversing the tree.\n");
	traverseTree(&root, 1, 0);
/*int steps = 1, trvSize = 1;
	trvQueue[0] = (root->child);
	do
	{
		for(int i = 0; i < trvSize; i++)
		{
		}
	} while(0);*/
	printf("Total orbits: %d\n", totalOrbits);
	return 0;
}

void traverseTree(Planet* pQueued[], int len, int step)
{
	Planet* pNext[32];
	int pNextSize = 0;
	//getchar();
	// iterate over the queued nodes
	for(int i = 0; i < len; i++)
	{
		printf("In node %s depth %d\n", pQueued[i]->name, step);
		// add all parents to queue
		for(int j = 0; j < pQueued[i]->parentCount; j++)
		{
			printf("Adding parent %s to next queue.\n", pQueued[i]->parent[j]->name);
			assert(pNextSize < 32);
			pNext[pNextSize++] = pQueued[i]->parent[j];
		}
		// add steps taken 
		if(step > 1)
			totalOrbits += (step-1);
	}
	if(!pNextSize)
	{
		printf("Reached bottom at depth %d, exting traversal.\n", step);
		return;
	}
	printf("Finished at depth %d, continuing onto next queue.\n", step);
	return traverseTree(pNext, pNextSize, ++step);
}

int findPlanet(char* name)
{
	for(int i = 0; i < listSize; i++)
		if(!strcmp(list[i].name, name))
			return i;

	return -1;
}

int createPlanet(char* name)
{
	Planet p = {"", -1, 0, NULL, {NULL}};
	strcpy(p.name, name);
	list[listSize] = p;
	return listSize++;
}

void printPlanet(Planet p)
{
	printf("Planet: %s\n", p.name);
	printf("Orbiting planet %s\n", p.child->name);
	printf("Planets in orbit: %d\n", p.parentCount);
	for(int i = 0; i < p.parentCount; i++)
		printf("\t%s\n", p.parent[i]->name);
}

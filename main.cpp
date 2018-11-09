#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "token.h"
#include "node.h"
#include "parser.h"
#include "testTree.h"

FILE *fp = NULL;

int main(int argc, char *argv[])
{
	node_t *root = NULL;

	// Choose source based on number arguments
	if (argc == 1)
	{
		// Call test scanner with standard input
		fp = stdin;
		root = parser();
	}
	else if (argc == 2)
	{
		// Build filename from arguments
		int fileNameLen = strlen(argv[1]) + 6;
		char fileName[fileNameLen];
		strcpy(fileName, argv[1]);
		strcat(fileName, ".fs18");
		
		// Open file and call test scanner with file pointer
		fp = fopen(fileName, "r");
		if (fp == NULL)
		{
			// Error if file cannot be opened
			printf("Error: Could not open file \"%s\"\n", fileName);
			return 1;
		}
		root = parser();
		fclose(fp);
	}
	else
	{
		// Error message for more than two arguments
		printf("Error: invalid number of arguments\n");
		return 1;
	}

	testTree(root, 0);

	return 0;
}

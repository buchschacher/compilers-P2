#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include "token.h"
#include "node.h"
#include "parser.h"
#include "testTree.h"

FILE *fp = NULL;

int main(const int argc, char *argv[])
{
	node_t *root = NULL;

	// Choose file pointer based on number of arguments
	if (argc == 1)
		fp = stdin;
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
			printf("Main error: Could not open file \"%s\"\n", fileName);
			return 1;
		}
	}
	else
	{
		// Error message for more than two arguments
		printf("Main error: invalid number of arguments\n");
		return 1;
	}

	// Create tree from input
	root = parser();

	// Ouput preorder tree
	treePrint(root, 0);

	// close file
	if (argc == 2)
		fclose(fp);
	

	return 0;
}

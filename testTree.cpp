#include <cstdlib>
#include <cstdio>
#include "token.h"
#include "node.h"

void treePrint(node_t* node, int depth)
{
	// Indent node based on level
	for (int i = 0; i < depth; i++)
	{
		printf("  ");
	}

	// Print node label and tokens
	printf("%s:", node->label);
	for (int i = 0; i < 2; i++)
	{
		if (node->token[i].type != 0)
			printf(" %s", node->token[i].inst);	
		//else
		//	break;
	}
	printf("\n");

	// Traverse child nodes
	for (int i = 0; i < 4; i++)
	{
		if (node->child[i] != NULL)
			treePrint(node->child[i], depth + 1);
		//else
		//	break;
	}

	return;
}

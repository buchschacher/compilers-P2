#include <cstdlib>
#include <cstdio>
#include "token.h"
#include "node.h"

void testTree(node_t* node, int depth)
{
	for (int i = 0; i < depth; i++)
	{
		printf("  ");
	}

	printf("%s", node->label);

	for (int i = 0; i < 2; i++)
	{
		if (node->token[i].type != 0)
			printf(" %s:%s", tokenNames[node->token[i].type], node->token[i].inst);
		else
			break;
	}
	printf("\n");

	for (int i = 0; i < 4; i++)
	{
		if (node->child[i] != NULL)
			testTree(node->child[i], depth + 1);
		else
			break;
	}

	return;
}

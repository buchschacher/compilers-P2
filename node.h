#ifndef NODE_H
#define NODE_H

#include "token.h"

// Node structure 
typedef struct node
{
	char label[8];
	token_t token[2];
	struct node *child[4];
} node_t;

#endif

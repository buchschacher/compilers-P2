#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstring>
#include "token.h"
#include "scanner.h"
#include "node.h"

node_t* program();
node_t* block();
node_t* vars();
node_t* expr();
node_t* a();
node_t* m();
node_t* r();
node_t* stats();
node_t* mstat();
node_t* stat();
node_t* in();
node_t* out();
node_t* iff();
node_t* loop();
node_t* assign();
node_t* ro();
void error();
token_t token;

node_t* parser()
{
	node_t *root = NULL;
	token = scanner();
	root = program();
	if (token.type == EOFtk)
	{
		printf("Parse was OK\n");
		return root;
	}
	else
		exit(1);
}

/* <program> -> void <vars> <block> */
node_t* program()
{
	node_t *p = new node_t;
	strcpy(p->label, "program");
	if (token.type == VOIDtk)
	{
		token = scanner();
		p->child[0] = vars();
		p->child[1] = block();
		return p;
	}
	error();
}

/* <block> -> begin <vars> <stats> end */
node_t* block()
{
	node_t *p = new node_t;
        strcpy(p->label, "block");
	if (token.type == BGNtk)
	{
		token = scanner();
		p->child[0] = vars();
		p->child[1] = stats();
		if (token.type == ENDtk)
		{
			token = scanner();
			return p;
		}
	}
	error();
}

/* <vars> -> empty | let id = int <vars> */
node_t* vars()
{
	node_t *p = new node_t;
        strcpy(p->label, "vars");
	if (token.type == LETtk)
	{
		token = scanner();
		if (token.type == IDtk)
		{
			p->token[0] = token;
			token = scanner();
			if (token.type == EQUtk)
			{
				token = scanner();
				if (token.type == INTtk)
				{
					p->token[1] = token;
					token = scanner();
					p->child[0] = vars();
					return p;
				}
			}
		}
		error();
	}
	return p;
}

/* <expr> -> <a> / <expr> | <a> * <expr> | <a> */
node_t* expr()
{
	node_t *p = new node_t;
        strcpy(p->label, "expr");
	p->child[0] = a();						
	if ((token.type == DIVtk) || (token.type == MULTtk))
	{
		p->token[0] = token;
		token = scanner();
		expr();
		return p;
	}
	return p;
}

/* <a> -> <m> + <a> | <m> - <a> | <m> */
node_t* a()
{
	node_t *p = new node_t;	
        strcpy(p->label, "A");
	p->child[0] = m();
	if ((token.type == PLUStk) || (token.type == MINtk))
	{
		p->token[0] = token;
		token = scanner();
		p->child[1] = a();
		return p;
	}
	return p;
}

/* <m> -> - <m> | <r> */
node_t* m()
{
	node_t *p = new node_t;
        strcpy(p->label, "M");
	if (token.type == MINtk)	// <m> -> - <m>
	{
		p->token[0] = token;
		token = scanner();
		p->child[0] = m();
		return p;
	}
	else				// <m> -> <r> 
	{
		p->child[0] = r();
		return p;
	}
}

/* <r> -> ( <expr> ) | id | int */
node_t* r()
{
	node_t *p = new node_t;
        strcpy(p->label, "R");
	if (token.type == LPtk)
	{
		token = scanner();
		p->child[0] = expr();
		if (token.type == RPtk)
		{
			token = scanner();
			return p;
		}
		error();
	}
	if (token.type == IDtk)
	{
		p->token[0] = token;
		token = scanner();
		return p;
	}
	if (token.type == INTtk)
	{
		p->token[0] = token;
		token = scanner();
		return p;
	}
	error();
}

/* <stats> -> <stat> <mstat> */
node_t* stats()
{
	node_t *p = new node_t;
        strcpy(p->label, "stats");
	p->child[0] = stat();
	p->child[1] = mstat();
	return p;
}

/* <mstat> -> empty | <stat> <mstat> */
node_t* mstat()
{
	node_t *p = new node_t;
        strcpy(p->label, "mStat");
	if ((token.type == READtk) || (token.type == BGNtk) || (token.type == ITERtk) || (token.type == PRNTtk) || (token.type == IDtk) || (token.type == CONDtk))
	{
		p->child[0] = stat();
		p->child[1] = mstat();
	}
	return p;
}

/* <stat> -> <in> | <out> | <block> | <if> | <loop> | <assign> */
node_t* stat()
{
	node_t *p = new node_t;
        strcpy(p->label, "stat");
	if (token.type == READtk)
	{
		p->child[0] = in();
		return p;
	}
	if (token.type == PRNTtk)
	{
		p->child[0] = out();
		return p;
	}
	if (token.type == BGNtk)
	{
		p->child[0] = block();
		return p;
	}
	if (token.type == CONDtk)
	{
		p->child[0] = iff();
		return p;
	}
	if (token.type == ITERtk)
	{
		p->child[0] = loop();
		return p;
	}
	if (token.type == IDtk)
	{
		p->child[0] = assign();
		return p;
	}
	error();
}

/* <in> -> read ( id ) : */
node_t* in()
{
	node_t *p = new node_t;
        strcpy(p->label, "in");
	if (token.type == READtk)
	{
		token = scanner();
		if (token.type == LPtk)
		{
			token = scanner();
			if (token.type == IDtk)
			{
				p->token[0] = token;
				token = scanner();
				if (token.type == RPtk)
				{
					token = scanner();
					if (token.type == COLtk)
					{
						token = scanner();
						return p;
					}
				}
			}
		}
	}
	error();
}

/* <out> -> print ( <expr> ) : */
node_t* out()
{
	node_t *p = new node_t;
        strcpy(p->label, "out");
	if (token.type == PRNTtk)
	{
		token = scanner();
		if (token.type == LPtk)
		{
			token = scanner();
			p->child[0] = expr();
			if (token.type == RPtk)
			{
				token = scanner();
				if (token.type == COLtk)
				{
					token = scanner();
					return p;
				}
			}
		}
	}
}

/* <if> -> cond ( <expr> <ro> <expr> ) <stat> */
node_t* iff()
{
	node_t *p = new node_t;
        strcpy(p->label, "if");
	if (token.type == CONDtk)
	{
		token = scanner();
		if (token.type == LPtk)
		{
			token = scanner();
			p->child[0] = expr();
			p->child[1] = ro();
			p->child[2] = expr();
			if (token.type == RPtk)
			{
				token = scanner();
				p->child[3] = stat();
				return p;
			}
		}
	}
}

/* <loop> -> iter ( <expr> <ro> <expr> ) <stat> */
node_t* loop()
{
	node_t *p = new node_t;
        strcpy(p->label, "loop");
	if (token.type == ITERtk)
	{
		token = scanner();
		if (token.type == LPtk)
		{
			token = scanner();
			p->child[0] = expr();
			p->child[1] = ro();
			p->child[2] = expr();
			if (token.type == RPtk)
			{
				token = scanner();
				p->child[3] = stat();
				return p;
			}
		}
	}
}

/* <assign> -> id = <expr> : */
node_t* assign()
{
	node_t *p = new node_t;
        strcpy(p->label, "assign");
	if (token.type == IDtk)
	{
		p->token[0] = token;
		token = scanner();
		if (token.type == EQUtk)
		{
			token = scanner();
			p->child[0] = expr();
			if (token.type == COLtk)
			{
				token = scanner();
				return p;
			}
		}
	}
}

/* <ro> -> < | < = | > | > = | = = | = */
node_t* ro()
{
	node_t *p = new node_t;
        strcpy(p->label, "RO");
	if (token.type == LSTtk)
	{
		p->token[0] = token;
		token = scanner();
		if (token.type == EQUtk)
		{
			p->token[1] = token;
			token = scanner();
		}
		return p;
	}
	if (token.type == GRTtk)
	{
		p->token[0] = token;
		token = scanner();
		if (token.type == EQUtk)
		{
			p->token[1] = token;
			token = scanner();
		}
		return p;
	}
	if (token.type == EQUtk)
	{
		p->token[0] = token;
		token = scanner();
		if (token.type == EQUtk)
		{
			p->token[1] = token;
			token = scanner();
		}
		return p;
	}
}

void error()
{
	printf("Parser Error: Unexpected %s token \"%s\" on line %d\n", tokenNames[token.type], token.inst, token.line);
	exit(1);
}

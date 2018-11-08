#include <cstdlib>
#include <cstdio>
#include "token.h"
#include "scanner.h"

void program();
void block();
void vars();
void expr();
void a();
void m();
void r();
void stats();
void mstat();
void stat();
void in();
void out();
void iff();
void loop();
void assign();
void ro();
void error();

token_t token;

void parser()
{
	token = scanner();
	program();
	if (token.type == EOFtk)
	{
		printf("Parse was OK\n");
		return;
	}
	else
		error();
}

/* <program> -> void <vars> <block> */
void program()
{
	if (token.type == VOIDtk)
	{
		token = scanner();
		vars();
		block();
		return;
	}
	error();
}

/* <block> -> begin <vars> <stats> end */
void block()
{
	if (token.type == BGNtk)
	{
		token = scanner();
		vars();
		stats();
		if (token.type == ENDtk)
		{
			token = scanner();
			return;
		}
	}
	error();
}

/* <vars> -> empty | let id = int <vars> */
void vars()
{
	if (token.type == LETtk)
	{
		token = scanner();
		if (token.type == IDtk)
		{
			token = scanner();
			if (token.type == EQUtk)
			{
				token = scanner();
				if (token.type == INTtk)
				{
					token = scanner();
					vars();
					return;
				}
			}
		}
		error();
	}
	return;
}

/* <expr> -> <a> / <expr> | <a> * <expr> | <a> */
void expr()
{
	a();						
	if (token.type == DIVtk)
	{
		token = scanner();
		expr();
		return;
	}
	else if (token.type == MULTtk)
	{
		token = scanner();
		expr();
		return;
	}
	return;
}

/* <a> -> <m> + <a> | <m> - <a> | <m> */
void a()
{
	m();
	if (token.type == PLUStk)	// <a> -> <m> + <a>
	{
		token = scanner();
		a();
		return;
	}
	if (token.type == PLUStk)	// <a> -> <m> - <a>
	{
		token = scanner();
		a();
		return;
	}
	return;						// <a> -> <m>
}

/* <m> -> - <m> | <r> */
void m()
{
	if (token.type == MINtk)	// <m> -> - <m>
	{
		token = scanner();
		m();
		return;
	}
	else						// <m> -> <r> 
	{
		r();
		return;
	}
}

/* <r> -> ( <expr> ) | id | int */
void r()
{
	if (token.type == LPtk)
	{
		token = scanner();
		expr();
		if (token.type == RPtk)
		{
			token = scanner();
			return;
		}
		error();
	}
	if (token.type == IDtk)
	{
		token = scanner();
		return;
	}
	if (token.type == INTtk)
	{
		token = scanner();
		return;
	}
	error();
}

/* <stats> -> <stat> <mstat> */
void stats()
{
	stat();
	mstat();
	return;
}

/* <mstat> -> empty | <stat> <mstat> */
void mstat()
{
	if ((token.type == READtk) || (token.type == BGNtk) || (token.type == ITERtk) || (token.type == PRNTtk) || (token.type == IDtk) || (token.type == CONDtk))
	{
		stat();
		mstat();
	}
	return;
}

/* <stat> -> <in> | <out> | <block> | <if> | <loop> | <assign> */
void stat()
{
	if (token.type == READtk)
	{
		in();
		return;
	}
	if (token.type == PRNTtk)
	{
		out();
		return;
	}
	if (token.type == BGNtk)
	{
		block();
		return;
	}
	if (token.type == CONDtk)
	{
		iff();
		return;
	}
	if (token.type == ITERtk)
	{
		loop();
		return;
	}
	if (token.type == IDtk)
	{
		assign();
		return;
	}
	error();
}

/* <in> -> read ( id ) : */
void in()
{
	if (token.type == READtk)
	{
		token = scanner();
		if (token.type == LPtk)
		{
			token = scanner();
			if (token.type == IDtk)
			{
				token = scanner();
				if (token.type == RPtk)
				{
					token = scanner();
					if (token.type == COLtk)
					{
						token = scanner();
						return;
					}
				}
			}
		}
	}
	error();
}

/* <out> -> print ( <expr> ) : */
void out()
{
	if (token.type == PRNTtk)
	{
		token = scanner();
		if (token.type == LPtk)
		{
			token = scanner();
			expr();
			if (token.type == RPtk)
			{
				token = scanner();
				if (token.type == COLtk)
				{
					token = scanner();
					return;
				}
			}
		}
	}
}

/* <if> -> cond ( <expr> <ro> <expr> ) <stat> */
void iff()
{
	if (token.type == CONDtk)
	{
		token = scanner();
		if (token.type == LPtk)
		{
			token = scanner();
			expr();
			ro();
			expr();
			if (token.type == RPtk)
			{
				token = scanner();
				stat();
				return;
			}
		}
	}
}

/* <loop> -> iter ( <expr> <ro> <expr> ) <stat> */
void loop()
{
	if (token.type == ITERtk)
	{
		token = scanner();
		if (token.type == LPtk)
		{
			token = scanner();
			expr();
			ro();
			expr();
			if (token.type == RPtk)
			{
				token = scanner();
				stat();
				return;
			}
		}
	}
}

/* <assign> -> id = <expr> : */
void assign()
{
	if (token.type == IDtk)
	{
		token = scanner();
		if (token.type == EQUtk)
		{
			token = scanner();
			expr();
			if (token.type == COLtk)
			{
				token = scanner();
				return;
			}
		}
	}
}

/* <ro> -> < | < = | > | > = | = = | = */
void ro()
{
	if (token.type == LSTtk)
	{
		token = scanner();
		if (token.type == EQUtk)
		{
			token = scanner();
		}
		return;
	}
	if (token.type == GRTtk)
	{
		token = scanner();
		if (token.type == EQUtk)
		{
			token = scanner();
		}
		return;
	}
	if (token.type == EQUtk)
	{
		token = scanner();
		if (token.type == EQUtk)
		{
			token = scanner();
		}
		return;
	}
}

void error()
{
	printf("Parser Error: Unexpected %s token \"%s\" on line %d\n", tokenNames[token.type], token.inst, token.line);
	exit(1);
}

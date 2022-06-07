#ifndef GOSLING_PARSER_H
#define GOSLING_PARSER_H

#include <list>
#include <regex>
#include <iterator>
#include "token.h"
#include <iostream>

class Parser {
private:
	bool lang(std::list<Token>::iterator&);
	bool expr(std::list<Token>::iterator&);
	bool EXPR_IF_ELSE(std::list<Token>::iterator&);
	bool EXPR_WHILE(std::list<Token>::iterator&);
	bool EXPR_FOR(std::list<Token>::iterator&);
	bool EXPR_ASSIGN(std::list<Token>::iterator&);
	bool DEF_CALL(std::list<Token>::iterator&);
	bool OBJ_CALL(std::list<Token>::iterator&);
	bool EXPR_ELIF(std::list<Token>::iterator&);
	bool EXPR_ELSE(std::list<Token>::iterator&);
	bool VALUE(std::list<Token>::iterator&);
	bool DESCRIBE(std::list<Token>::iterator&);
	bool EXEC_BLOCK(std::list<Token>::iterator&);
	bool EXPR_PREDICATE(std::list<Token>::iterator&);

	//bool DESCR_DEF(std::list<Token>::iterator&);
	//bool OBJ_ASSIGN(std::list<Token>::iterator&);


	std::list<Token>* p_tokens;
public:
	Parser(std::list<Token>&);
	bool check();
};

#endif //GOSLING_PARSER_H

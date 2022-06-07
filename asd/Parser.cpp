#include "Parser.h"

Parser::Parser(std::list<Token> &tokens) {
	p_tokens = &tokens;
}

bool Parser::check() {
	// lang -> expr* | DESCRIBE*
	// expr -> EXPR_IF_ELSE | EXPR_WHILE | EXPR_FOR | EXPR_ASSIGN | DEF_CALL | OBJ_CALL | OBJ_ASSIGN
	// EXPR_ASSIGN -> VAR ASSIGN_OP VALUE (OP VALUE)*
	// VALUE -> VAR | DIGIT | STRING | DEF_CALL | OBJ_CALL
	// DESCRIBE -> DESCR_DEF | DESCR_CLASS
	// OBJ_ASSIGN ->


	// EXPR_WHILE -> WHILE L_BRACKET EXPR_PREDICATE R_BRACKET EXEC_BLOCK END
	// EXPR_PREDICATE -> BOOL | VALUE | (VALUE CONDITIONAL_OP VALUE)
	// VALUE -> VAR | DIGIT | STRING | DEF_CALL
	// EXEC_BLOCK -> expr ((CONTINUE | BRAKE) expr)*

	// while (a < b)
	//     a = a + 1
	// end


	// EXPR_IF_ELSE -> if L_BRACKET EXPR_PREDICATE R_BRACKET EXEC_BLOCK EXPR_ELIF* | EXPR_ELSE{1,0} END
	// EXPR_PREDICATE -> BOOL | EXPR_DEF | (VALUE CONDITIONAL_OP VALUE)
	// EXEC_BLOCK -> expr

	// EXPR_ELIF -> L_BRACKET EXPR_PREDICATE R_BRACKET EXEC_BLOCK EXPR_ELIF* EXPR_ELSE+ END
	// EXEC_BLOCK -> expr

	// EXPR_ELSE -> ELSE EXEC_BLOCK END

	// if (a == b)
	//     a = 0
	//	elif (a < b)
	//	    a = b
	//	else
	//		b = 0
	//	end



	// EXPR_FOR -> FOR FOR_BLOCK EXEC_BLOCK END
	// FOR_BLOCK -> L_BRACKET FOR_B_P1 FOR_B_P2 FOR_B_P3 R_BRACKET
	// FOR_B_P1 -> EXPR_ASSIGN (ENUM_SEPARATE EXPR_ASSIGN)* SEMICOLON
	// FOR_B_P2 -> EXPR_PREDICATE SEMICOLON
	// FOR_B_P3 -> EXPR_ASSIGN (ENUM_SEPARATE EXPR_ASSIGN)* SEMICOLON


	// DESCR_DEF -> DEF VAR L_BRACKET (VAR (ENUM_SEPARATE VAR)*)? R_BRACKET EXEC_BLOCK END
	// EXEC_BLOCK -> expr


	// DEF_CALL -> VAR L_BRACKET (VAR (ENUM_SEPARATE VAR)*)? R_BRACKET EXEC_BLOCK END
	// EXEC_BLOCK -> expr


	// (gos_)(NAME_STRUCT)<TYPE> VAR
	// NAME_STRUCT -> *список реализованных структур*
	// TYPE -> *список описанных/существующих данных*

	// gos_list<int> mylist
	// mylist.add(1)
	// mylist.get(0)

	auto it = p_tokens->begin();

	bool result = true;

	while (it != p_tokens->end() && result) {
		result = lang(it);
	}

	if (result) {
		std::cout << "успех\n";
	} else {
		std::cout << "ошибка в грамматике\n";
	}

	return result;
}

bool Parser::lang(std::list<Token>::iterator& it) {
	// lang -> (expr | DESCRIBE)*
	// expr -> EXPR_IF_ELSE | EXPR_WHILE | EXPR_FOR | EXPR_ASSIGN | DEF_CALL | OBJ_CALL | OBJ_ASSIGN

	bool result;

	while (it != p_tokens->end()) {

		result = expr(it);

		if (!result) {
			result = DESCRIBE(it);
		}

		if (!result) {
			return false;
		}

	}

	return true;
}

bool Parser::expr(std::list<Token>::iterator &it) {
	// expr -> EXPR_IF_ELSE | EXPR_WHILE | EXPR_FOR | EXPR_ASSIGN | DEF_CALL | OBJ_CALL

	while (it != p_tokens->end()) {

		if (!EXPR_IF_ELSE(it)) {
			if (!EXPR_WHILE(it)) {
				if (!EXPR_FOR(it)) {
					if (!EXPR_ASSIGN(it)) {
						if (!DEF_CALL(it)) {
							if (!OBJ_CALL(it)) {
								return false;
							}
							break;
						}
						break;
					}
					break;
				}
				break;
			}
			break;
		}
		break;
	}

	return true;
}

bool Parser::EXPR_IF_ELSE(std::list<Token>::iterator &it) {
	// EXPR_IF_ELSE -> if L_BRACKET EXPR_PREDICATE R_BRACKET EXEC_BLOCK EXPR_ELIF* | EXPR_ELSE{1,0} END
	// EXPR_PREDICATE -> BOOL | EXPR_DEF | (VALUE CONDITIONAL_OP VALUE)
	// EXEC_BLOCK -> expr

	if (it->get_type() != "IF") {
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (it->get_type() != "L_BRACKET") {
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (!EXPR_PREDICATE(it)) {
		return false;
	}

	if (it->get_type() != "R_BRACKET" || it == p_tokens->end()) {
		throw std::string("отсутствует закрывающая скобка: ") + std::string("строка ") + std::to_string((it)->get_num_line());
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (!EXEC_BLOCK(it)) {
		return false;
	}

	while (EXPR_ELIF(it));

	EXPR_ELSE(it);

	if (it->get_type() != "END" || it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
	}

	++it;

	return true;
}

bool Parser::EXPR_WHILE(std::list<Token>::iterator& it) {
	// EXPR_WHILE -> WHILE L_BRACKET EXPR_PREDICATE R_BRACKET EXEC_BLOCK END
	// EXPR_PREDICATE -> BOOL | VALUE | (VALUE CONDITIONAL_OP VALUE)
	// VALUE -> VAR | DIGIT | STRING | DEF_CALL

	if (it->get_type() != "WHILE") {
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (it->get_type() != "L_BRACKET") {
		throw std::string("отсутсвует открывающая скобка: ") + std::string("строка ") + std::to_string((it)->get_num_line());
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (!EXPR_PREDICATE(it)) {
		return false;
	}

	if (it->get_type() != "R_BRACKET") {
		throw std::string("отсутствует закрывающая скобка: ") + std::string("строка ") + std::to_string((it)->get_num_line());
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	EXEC_BLOCK(it);

	if (it->get_type() != "END" || it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	++it;

	return true;
}

bool Parser::EXPR_FOR(std::list<Token>::iterator& it) {
	// EXPR_FOR -> FOR FOR_BLOCK EXEC_BLOCK END

	if (it->get_type() != "FOR") {
		return false;
	}


	return false;
}

bool Parser::VALUE(std::list<Token>::iterator &it) {
	// VALUE -> VAR | DIGIT | STRING | DEF_CALL | OBJ_CALL

	if (it->get_type() != "VAR") {
		if (it->get_type() != "DIGIT") {
			if (it->get_type() != "STRING") {
				if (!DEF_CALL(it)) {
					if (!OBJ_CALL(it)) {
						throw std::string("отсутсвует переменная/значение: ") + std::string("строка ") + std::to_string((it)->get_num_line());
						return false; // exception
					}
				}
			}
		}
	}

	return true;
}

bool Parser::EXPR_ASSIGN(std::list<Token>::iterator &it) {
	// EXPR_ASSIGN -> VALUE ASSIGN_OP VALUE (OP VALUE)*

	if (it->get_type() != "VAR") {
		return false; // exception
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (it->get_type() != "ASSIGN_OP") {
		throw std::string("отсутсвует оператор присваивания: ") + std::string("строка ") + std::to_string((it)->get_num_line());
		return false; // exception
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (!VALUE(it)) {
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	while (it->get_type() == "OP") {
		++it;
		if (it == p_tokens->end()) {
			throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
			return false; // exception
		}

		if (!VALUE(it)) {
			return false;
		}

		++it;
		if (it == p_tokens->end()) {
			break;
		}
	}

	return true;
}

bool Parser::DEF_CALL(std::list<Token>::iterator &) {
	return false;
}

bool Parser::OBJ_CALL(std::list<Token>::iterator &) {
	return false;
}

bool Parser::EXPR_ELIF(std::list<Token>::iterator& it) {
	// EXPR_ELIF -> ELIF L_BRACKET EXPR_PREDICATE R_BRACKET EXEC_BLOCK
	// EXEC_BLOCK -> expr

	if (it->get_type() != "ELIF") {
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (it->get_type() != "L_BRACKET") {
		throw std::string("отсутствует открывающая скобка: ") + std::string("строка ") + std::to_string((it)->get_num_line());
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (!EXPR_PREDICATE(it)) {
		return false;
	}

	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	if (it->get_type() != "R_BRACKET") {
		throw std::string("отсутствует закрывающая скобка: ") + std::string("строка ") + std::to_string((it)->get_num_line());
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	EXEC_BLOCK(it);

	return true;
}

bool Parser::EXPR_ELSE(std::list<Token>::iterator& it) {
	// EXPR_ELSE -> ELSE EXEC_BLOCK

	if (it->get_type() != "ELSE") {
		return false;
	}

	++it;
	if (it == p_tokens->end()) {
		throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
		return false; // exception
	}

	EXEC_BLOCK(it);

	return true;
}

bool Parser::DESCRIBE(std::list<Token>::iterator &) {
	return false;
}

bool Parser::EXEC_BLOCK(std::list<Token>::iterator& it) {
	while (expr(it) && it != p_tokens->end());
	return true;
}

bool Parser::EXPR_PREDICATE(std::list<Token>::iterator &it) {
	// EXPR_PREDICATE -> BOOL | DEF_CALL | (VALUE CONDITIONAL_OP VALUE)

	if (it->get_type() != "BOOL") {
		if (!DEF_CALL(it)) {
			if (!VALUE(it)) {
				throw std::string("некорректный предикат: ") + std::string("строка ") + std::to_string((it)->get_num_line());
			}

			++it;
			if (it == p_tokens->end()) {
				throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
				return false; // exception
			}

			if (it->get_type() != "CONDITIONAL_OP") {
				throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
			}

			++it;
			if (it == p_tokens->end()) {
				throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
				return false; // exception
			}

			if (!VALUE(it)) {
				throw std::string("некорректный предикат: ") + std::string("строка ") + std::to_string((it)->get_num_line());
			}

			++it;
			if (it == p_tokens->end()) {
				throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
				return false; // exception
			}
		}
		return true;
	} else {
		++it;
		if (it == p_tokens->end()) {
			throw std::string("незаконченная конструкция: ") + std::string("строка ") + std::to_string((--it)->get_num_line());
			return false; // exception
		}

		return true;
	}
}



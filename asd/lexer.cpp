#include "lexer.h"

const std::pair<std::string, std::regex> Lexer::DESCRIPTION_LECHEMS[] = {
		std::make_pair("WHILE", std::regex(("(while)"))),
		std::make_pair("FOR", std::regex((R"(for)"))),
		std::make_pair("IN", std::regex((R"(in)"))),
		std::make_pair("IF", std::regex((R"(if)"))),
		std::make_pair("ELIF", std::regex((R"(elif)"))),
		std::make_pair("ELSE", std::regex((R"(else)"))),
		std::make_pair("DEF", std::regex((R"(def)"))),
		std::make_pair("END", std::regex((R"(end)"))),
		std::make_pair("RETURN", std::regex((R"(return)"))),
		std::make_pair("BOOL", std::regex((R"(true|false)"))),
		std::make_pair("L_BRACKET", std::regex((R"([\(])"))),
		std::make_pair("R_BRACKET", std::regex((R"([\)])"))),
		std::make_pair("ENUM_SEPARATE", std::regex((R"(,)"))),
		std::make_pair("SEMICOLON", std::regex((R"(;)"))),
		std::make_pair("VAR", std::regex((R"([a-zA-Z_][A-Za-z0-9_]*)"))),
		std::make_pair("DIGIT", std::regex((R"(([0-9])|([1-9][0-9]*))"))),
		std::make_pair("ASSIGN_OP", std::regex((R"(=)"))),
		std::make_pair("STRING", std::regex((R"((^'[^']*))"))),
		std::make_pair("OP", std::regex((R"([\+\-\*\/])"))),
		std::make_pair("CONDITIONAL_OP", std::regex((R"(([<>=][=]*)|([!][=]{0,1}))"))),
		std::make_pair("STRING", std::regex((R"((^'[^']*))"))),
};

const size_t Lexer::DESC_LECHEMS_SIZE = sizeof(DESCRIPTION_LECHEMS) / sizeof(std::pair<std::string, std::regex>);

std::list<Token> Lexer::tokens{};


std::string Lexer::check_lexems(std::string lechema) {
	for (int j = 0; j < DESC_LECHEMS_SIZE; j++) {
		if (std::regex_match(lechema.data(), DESCRIPTION_LECHEMS[j].second)) {
			return DESCRIPTION_LECHEMS[j].first;
		}
	}
	return "";
}

int Lexer::num_line = 1;

void Lexer::run(std::string expression) {
	expression += " ";
	std::string lechema = "";
	std::string type_of_current_lechema;
	std::string type_last_of_lechema = "";
	std::string check_lex = "";

	for (int i = 0; i < expression.size(); i++) {
		lechema += expression[i];
		type_of_current_lechema = "";

		type_of_current_lechema = check_lexems(lechema);

		if (type_of_current_lechema.empty()) {
			if (!type_last_of_lechema.empty()) {
				if (type_last_of_lechema != "STRING") {

					check_lex = check_lexems(lechema.substr(0, lechema.size() - 1));

					if (type_last_of_lechema != check_lex && !check_lex.empty()) {
						tokens.emplace_back(Token(check_lex, lechema.substr(0, lechema.size() - 1), num_line));
					} else {
						tokens.emplace_back(Token(type_last_of_lechema, lechema.substr(0, lechema.size() - 1), num_line));
					}

					i--;
				} else {
					tokens.emplace_back(Token(type_last_of_lechema, lechema.substr(0, lechema.size()), num_line));
				}
			} else {
				if (lechema != " ") {
					tokens.emplace_back(Token("error", lechema, num_line));
					i--;
				}
			}

			lechema = "";
		}

		type_last_of_lechema = type_of_current_lechema;
	}

	if (type_of_current_lechema == "STRING") {
		tokens.emplace_back(Token("error", lechema, num_line));
	}

	num_line++;
}

std::list<Token>& Lexer::get_list_token() {
	return tokens;
}

#include "token.h"

Token::Token(std::string type, std::string value, int num_line) {
	this->num_line = num_line;
	this->type = type;
	this->value = value;
}

Token::Token(Token const &other_token) {
	this->type = other_token.type;
	this->value = other_token.value;
	this->num_line = other_token.num_line;
}

std::string Token::get_type() {
	return type;
}

std::string Token::get_value() {
	return value;
}

int Token::get_num_line() {
	return num_line;
}


std::ostream& operator<<(std::ostream &os, Token &token) {
	os << "{Token: type = " << token.get_type() << ", lechema = " << token.get_value() << ", number of line = " << token.get_num_line() << "}";
	return os;
}

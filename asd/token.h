#ifndef GOSLING_TOKEN_H
#define GOSLING_TOKEN_H
#include <string>
#include <ostream>

class Token {
private:
	std::string type;
	std::string value;
	int num_line;
public:
	Token() = delete;
	Token(std::string, std::string, int);
	Token(Token const &other_token);
	std::string get_type();
	std::string get_value();
	int get_num_line();
};

std::ostream& operator<<(std::ostream&, Token&);

#endif //GOSLING_TOKEN_H

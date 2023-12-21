#include <sstream>

#include "Token.h"

// Definizione operatore di output (utilizzare per stampare in console tutti i Token)
std::ostream& operator<<(std::ostream& os, const Token& t) {
	std::stringstream tmp;
	// Stampo su console (ID : parola)
	tmp << t.tag << " : " << t.word;
	os << tmp.str();
	return os;
}
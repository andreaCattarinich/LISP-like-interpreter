#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// STRUTTURA TOKEN
// La struttura Token è utile alla Tokenizzazione del file.
// Per eseguire l'analisi sintattica useremo questa struttura.
struct Token {

	// Definisco gli identificativi dei Token
	static constexpr int LP = 0;		// (
	static constexpr int RP = 1;		// )
	static constexpr int NUMERO = 2;	// es: '123'
	static constexpr int BLOCK = 3;		// ( BLOCK
	static constexpr int INPUT = 4;		// ( INPUT
	static constexpr int PRINT = 5;		// ( PRINT
	static constexpr int SET = 6;		// ( SET
	static constexpr int WHILE = 7;		// ( WHILE
	static constexpr int IF = 8;		// ( IF
	static constexpr int GT = 9;		// ( GT
	static constexpr int LT = 10;		// ( LT
	static constexpr int EQ = 11;		// ( EQ
	static constexpr int AND = 12;		// ( AND
	static constexpr int OR = 13;		// ( OR
	static constexpr int NOT = 14;		// ( NOT
	static constexpr int TRUE = 15;		// TRUE
	static constexpr int FALSE = 16;	// FALSE
	static constexpr int ADD = 17;		// ( ADD
	static constexpr int SUB = 18;		// ( SUB
	static constexpr int MUL = 19;		// ( MUL
	static constexpr int DIV = 20;		// ( DIV
	static constexpr int VARIABILE = 21;// es: 'var'

	// Conversione ID to word
	static constexpr const char* id2word[]{
		"(", ")", "NUMERO", "BLOCK", "INPUT", "PRINT", "SET", "WHILE",
		"IF", "GT", "LT", "EQ", "AND", "OR", "NOT", "TRUE", "FALSE",
		"ADD", "SUB", "MUL", "DIV"
	};

	Token(int t, const char* w) : tag{ t }, word{ w } { }
	Token(int t, std::string w) : tag{ t }, word{ w } { }

	// Il token è costituito dalla coppia (ID, parola)
	int tag;
	std::string word;
};

// Dichiarazione operatore di output (utilizzare per stampare in console tutti i Token)
std::ostream& operator<<(std::ostream& os, const Token& t);

#endif
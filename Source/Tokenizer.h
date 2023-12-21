#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <fstream>

#include "Token.h"

// CLASSE TOKENIZER
class Tokenizer {
public:
	// Function Object, nonché l'overloading dell'operatore ().
	// Il valore di ritorno è proprio un vettore di Token.
	std::vector<Token> operator()(std::ifstream& inputFile) {
		std::vector<Token> inputTokens;
		tokenizeInputFile(inputFile, inputTokens);
		return inputTokens;
	}
private:
	// Definizione del metodo privato che tokenizza il file in input.
	// Prende come parametri il File in Input ed il vettore di Token.
	void tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens);
};

#endif
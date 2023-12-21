#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "Tokenizer.h"
#include "Exceptions.h"

// Dichiarazione del metodo privato che tokenizza il file in input. 
void Tokenizer::tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens) {
	// Leggo il file carattere per carattere fino a quando non termina
	char ch;
	ch = inputFile.get();

	while (!inputFile.eof()) {
		// Salto lo spazio bianco
		if (std::isspace(ch)) {
			ch = inputFile.get();
			continue;
		}

		if (ch == '(') { // Parentesi aperta
			inputTokens.push_back(Token{ Token::LP, Token::id2word[Token::LP] });
		} else if (ch == ')') { // Parentesi chiusa
			inputTokens.push_back(Token{ Token::RP, Token::id2word[Token::RP] });
		} else if (ch == '-') { // Segno meno
			std::stringstream tmp{};
			tmp << ch;
			// Prelevo un numero positivo
			do {
				ch = inputFile.get();
				if (std::isdigit(ch)) {
					tmp << ch;	// TODO: controllare questo errore (PRINT -)
				} else if (ch == ' ' || ch == '(' || ch == ')') {
					continue;
				} else {
					std::stringstream tmp{};
					tmp << "A negative number (-) cannot be followed by a '" << ch << "' character.";
					throw LexicalError(tmp.str());
				}
			} while (std::isdigit(ch));
			// TODO: eventualmente controllare se tmp è un numero (negativo)
			if (tmp.str() == "-")
				throw LexicalError("The single symbol '-' is not recognized by the grammar.");
			inputTokens.push_back(Token{ Token::NUMERO, tmp.str() });
			continue;
		} else if (std::isdigit(ch)) { // Prelevo un numero positivo
			std::stringstream tmp{};
			tmp << ch;
			do {
				ch = inputFile.get();
				if (std::isdigit(ch)) {
					tmp << ch;
				} else if (ch != ' ' && ch != '\n' && ch != ')') {
					std::stringstream tmp{};
					tmp << "A number cannot be followed by the character: '" << ch << "'.";
					throw LexicalError(tmp.str());
				}
			} while (std::isdigit(ch));
			// TODO: eventualmente controllare se tmp è un numero (positivo)
			inputTokens.push_back(Token{ Token::NUMERO, tmp.str() });
			continue;
		} else if (std::isalpha(ch)) { // Variabile
			std::stringstream tmp{};
			tmp << ch;
			// Prelevo parola composta da caratteri alfabetici
			do {
				ch = inputFile.get();
				if (std::isalpha(ch)) {
					tmp << ch;
				} else if (ch != ' ' && ch != '\n' && ch != ')' && ch != '\t') {
					std::stringstream tmp{};
					tmp << "A variable cannot contains the character: '" << ch << "'.";
					throw LexicalError(tmp.str());
				}
			} while (std::isalpha(ch));

			std::string to_compare = "" + tmp.str();

			// Cerco la parola all'interno del dizionario presente in Token.h riga 36
			// ovvero: static constexpr const char* id2word[] { "ADD", "BLOCK", ... } 
			bool trovata = false;
			for (const std::string& elemento : Token::id2word) {
				// Il controllo to_compare != "NUMERO" è necessario perché nelle id2word[]
				// è presente la seguente stringa "NUMERO" e ciò darebbe luogo ad un errore
				// per quanto riguarda l'identificatore.
				if (elemento == to_compare && to_compare != "NUMERO") {
					trovata = true;
					break;
				}
			}
			// Se la parola è presente nel dizionare, effettuo la push nell'array di tokens:
			// nel caso fosse una PAROLA CHIAVE, eseguo la push della PAROLA CHIAVE,
			// nel caso fosse diversa, eseguo la push della VARIABILE
			if (trovata) {
				if (to_compare == Token::id2word[Token::BLOCK])
					inputTokens.push_back(Token{ Token::BLOCK, to_compare });
				else if (to_compare == Token::id2word[Token::INPUT])
					inputTokens.push_back(Token{ Token::INPUT, to_compare });
				else if (to_compare == Token::id2word[Token::PRINT])
					inputTokens.push_back(Token{ Token::PRINT, to_compare });
				else if (to_compare == Token::id2word[Token::SET])
					inputTokens.push_back(Token{ Token::SET, to_compare });
				else if (to_compare == Token::id2word[Token::WHILE])
					inputTokens.push_back(Token{ Token::WHILE, to_compare });
				else if (to_compare == Token::id2word[Token::IF])
					inputTokens.push_back(Token{ Token::IF, to_compare });
				else if (to_compare == Token::id2word[Token::GT])
					inputTokens.push_back(Token{ Token::GT, to_compare });
				else if (to_compare == Token::id2word[Token::LT])
					inputTokens.push_back(Token{ Token::LT, to_compare });
				else if (to_compare == Token::id2word[Token::EQ])
					inputTokens.push_back(Token{ Token::EQ, to_compare });
				else if (to_compare == Token::id2word[Token::AND])
					inputTokens.push_back(Token{ Token::AND, to_compare });
				else if (to_compare == Token::id2word[Token::OR])
					inputTokens.push_back(Token{ Token::OR, to_compare });
				else if (to_compare == Token::id2word[Token::NOT])
					inputTokens.push_back(Token{ Token::NOT, to_compare });
				else if (to_compare == Token::id2word[Token::TRUE])
					inputTokens.push_back(Token{ Token::TRUE, to_compare });
				else if (to_compare == Token::id2word[Token::FALSE])
					inputTokens.push_back(Token{ Token::FALSE, to_compare });
				else if (to_compare == Token::id2word[Token::ADD])
					inputTokens.push_back(Token{ Token::ADD, to_compare });
				else if (to_compare == Token::id2word[Token::SUB])
					inputTokens.push_back(Token{ Token::SUB, to_compare });
				else if (to_compare == Token::id2word[Token::MUL])
					inputTokens.push_back(Token{ Token::MUL, to_compare });
				else if (to_compare == Token::id2word[Token::DIV])
					inputTokens.push_back(Token{ Token::DIV, to_compare });
			} else {
				inputTokens.push_back(Token{ Token::VARIABILE, tmp.str() });
			}
			continue;
		} else {
			// Ci sono dei simboli non riconosciuti dal tokenizzatore nel file,
			// ad esempio i seguenti simboli: | ! " £ $ % & / = ? ^
			std::stringstream tmp{};
			tmp << "The symbol '" << ch << "'" << " is not recognized by the grammar.";
			throw LexicalError(tmp.str());
		}
		ch = inputFile.get();
	}

	// Controllo se il file è vuoto
	if (!inputTokens.size())
		throw SintaxError("Empty File");
}
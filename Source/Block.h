#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include "Statement.h"
//#include "Visitor.h" // TODO: occhio all'include

class Visitor;

class Statement; // TODO: occhio, possibile problematica di inclusione

// CLASSE BLOCK
class Block {
public:
	// Per instanziare un oggetto di classe Block è necessario un array di puntatori a Statement
	Block(std::vector<Statement*> s) : stmt{ s } {};

	// Distruttore di default
	~Block() = default;
	
	// Costruttore di copia e Operatore di assegnamento sono di default
	Block(const Block& other) = default;
	Block& operator=(const Block& other) = default;
	
	// Metodi utili allo sviluppo
	std::vector<Statement*> getStatement() const { return stmt; }
	Statement* getOnlyAStatement(int index) const { return stmt[index]; }
	int getDimension() const { return static_cast<int>(stmt.size()); }

	// Metodo per aggiungere uno statement alla lista
	void AddStatement(Statement* s) { stmt.push_back(s); }

	void accept(Visitor* v);
private:
	// Vettore di statement
	std::vector<Statement*> stmt;
};

#endif
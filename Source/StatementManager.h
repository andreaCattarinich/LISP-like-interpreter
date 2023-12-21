#ifndef STATEMENTMANAGER_H
#define STATEMENTMANAGER_H

#include <iostream>
#include <vector>

#include "Block.h"
#include "Statement.h"

//#define DEBUG

// CLASSE STATEMENTMANAGER
// Utile per la gestione dei nodi dell'albero sintattico -> semantico
class StatementManager {
public:
	// Costruttore di default
	StatementManager() = default;

	// Distruttore manuale che de-alloca iterativamente ciascun blocco
	// La funzione è definita in questo .h
	~StatementManager() {
		clearMemory();
	};

	// Costruttore di copia e Operatore di assegnamento sono inaccessibili
	// per evitare problemi e conflitti con la memoria
	StatementManager(const StatementManager& other) = delete;
	StatementManager& operator=(const StatementManager& other) = delete;

	// Metodo make PrintStatement che esegue la push nello stack
	Statement* makePrintStatement(NumExpr* ne) {
		#ifdef DEBUG
		std::cout << "Creo PrintStatement" << std::endl;
		#endif // DEBUG
		
		Statement* print_stmt = new PrintStmt(ne);
		allocated.push_back(print_stmt);
		return print_stmt;
	}

	// Metodo make SetStatement che esegue la push nello stack
	Statement* makeSetStatement(Variable* var, NumExpr* ne) {
		#ifdef DEBUG
		std::cout << "Creo SetStatement" << std::endl;
		#endif // DEBUG
		Statement* set_stmt = new SetStmt(var, ne);

		allocated.push_back(set_stmt);
		return set_stmt;
	}

	// Metodo make InputStatement che esegue la push nello stack
	Statement* makeInputStatement(Variable* var) {
		#ifdef DEBUG
		std::cout << "Creo InputStatement" << std::endl;
		#endif // DEBUG	
		Statement* input_stmt = new InputStmt(var);
		allocated.push_back(input_stmt);
		return input_stmt;
	}

	// Metodo make WhileStatement che esegue la push nello stack
	Statement* makeWhileStatement(BoolExpr* be, Block* stmt_b) {
		#ifdef DEBUG
		std::cout << "Creo WhileStatement" << std::endl;
		#endif // DEBUG			

		Statement* while_stmt = new WhileStmt(be, stmt_b);
		allocated.push_back(while_stmt);
		return while_stmt;
	}

	// Metodo make IfStatement che esegue la push nello stack
	Statement* makeIfStatement(BoolExpr* be, Block* lb, Block* rb) {
		#ifdef DEBUG
		std::cout << "Creo IfStatement" << std::endl;
		#endif // DEBUG		
		Statement* if_stmt = new IfStmt(be, lb, rb);
		allocated.push_back(if_stmt);
		return if_stmt;
	}

	// Definizione funzione clearMemory
	void clearMemory() {
		auto i = allocated.begin();
		for (; i != allocated.end(); ++i) {
			delete(*i);
		}
		allocated.resize(0);
	}

private:
	std::vector<Statement*> allocated;
};

#endif


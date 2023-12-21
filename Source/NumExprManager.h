#ifndef NUMEXPRMANAGER_H
#define NUMEXPRMANAGER_H

#include <vector>

#include "NumExpr.h"

//#define DEBUG

// CLASSE NUMEXPRMANAGER
// Utile per la gestione dei nodi dell'albero sintattico -> semantico
class NumExprManager {
public:
	// Costruttore di default
	NumExprManager() = default;

	// Distruttore manuale che de-alloca iterativamente ciascun blocco
	// La funzione è definita in questo .h
	~NumExprManager() {
		clearMemory();
	};

	// Costruttore di copia e Operatore di assegnamento sono inaccessibili
	// per evitare problemi e conflitti con la memoria
	NumExprManager(const NumExprManager& other) = delete;
	NumExprManager& operator=(const NumExprManager& other) = delete;

	// Metodo make Operator che esegue la push nello stack
	NumExpr* makeOperator(Operator::OpCode_Operator op, NumExpr* l_op, NumExpr* r_op) {
		#ifdef DEBUG
		std::cout << "Creo Operator" << std::endl;
		#endif // DEBUG

		NumExpr* oper = new Operator(op, l_op, r_op);
		allocated.push_back(oper);
		return oper;
	}

	// Metodo make Number che esegue la push nello stack
	NumExpr* makeNumber(int64_t v) {
		#ifdef DEBUG
		std::cout << "Creo Number" << std::endl;
		#endif // DEBUG

		NumExpr* n = new Number(v);
		allocated.push_back(n);
		return n;
	}

	// Due metodi uguali con due valori di ritorno differenti

	// Metodo make Variable che esegue la push nello stack
	// di una variabile in una num_expr
	NumExpr* makeVariable(std::string variable_id, int64_t value) {
		#ifdef DEBUG
		std::cout << "Creo Variable" << std::endl;
		#endif // DEBUG

		NumExpr* var = new Variable(variable_id, value);
		allocated.push_back(var);
		return var;
	}

	// Metodo make Variable che esegue la push nello stack
	// di una variabile in una set ?
	Variable* makeVariableV(std::string variable_id, int64_t value) {
		Variable* var = new Variable(variable_id, value);
		variable_allocated.push_back(var);
		return var;
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
	std::vector<NumExpr*> allocated;
	std::vector<Variable*> variable_allocated;
};

#endif
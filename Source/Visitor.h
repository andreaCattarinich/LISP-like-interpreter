#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <iostream>

#include "Block.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Statement.h"
#include "Exceptions.h"

#include "SymbolTable.h"

// Visitor astratto per la visita di tutti gli oggetti 
class Visitor {
public:
	// BLOCK
	virtual void visitBlock(Block* blockNode) = 0;

	// STATEMENT
	virtual void visitPrintStmt(PrintStmt* printNode) = 0;
	virtual void visitSetStmt(SetStmt* setNode) = 0;
	virtual void visitInputStmt(InputStmt* inputNode) = 0;

	virtual void visitWhileStmt(WhileStmt* whileNode) = 0;
	virtual void visitIfStmt(IfStmt* ifNode) = 0;

	// NUM EXPR
	virtual void visitOperator(Operator* opNode) = 0;
	virtual void visitNumber(Number* numNode) = 0;
	virtual void visitVariable(Variable* varNode) = 0;

	// BOOL EXPR
	virtual void visitRelOp(RelOp* relopNode) = 0;
	virtual void visitBoolConst(BoolConst* bcNode) = 0;
	virtual void visitBoolOp(BoolOp* bopNode) = 0;

	void setTabs(int value);
	void increaseTab(void);
	void decreaseTab(void);
};

// Visitor concreto per la valutazione delle espressioni
class EvaluationVisitor : public Visitor {
public:
	EvaluationVisitor() : accumulator{ } { }
	// TODO: distruttore, costruttori di copia, ecc.

	// BLOCK
	void visitBlock(Block* blockNode) override {
		// Prendo ciascun statement contenuto nel blocco e
		// consecutivamente ed eseguo una visita
		for (int i = 0; i < blockNode->getDimension(); i++) {
			blockNode->getOnlyAStatement(i)->accept(this);
		}
	}

	// STATEMENT
	void visitPrintStmt(PrintStmt* printNode) override {
		NumExpr* print_num_expr = printNode->getPrint();
		print_num_expr->accept(this);
		// std::cout << "Output console: " << this->getValue() << std::endl;
		std::cout << this->getValue() << std::endl;
		// TODO: effettuare la scrittura su file
	}

	void visitSetStmt(SetStmt* setNode) override {
		Variable* var_set = setNode->getVar();
		NumExpr* var_num_expr = setNode->getNumExpr();
		var_num_expr->accept(this);

		var_set->setValue(this->getValue());
		
		// Controllo se la variabile è già presente nella simbol table
		bool trovato = false;
		for (int i = 0; i < symbTable.size(); i++) {
			// Se è già presente ne cambio il valore
			// altrimenti ...
			if (symbTable[i].getVariableID() == var_set->getVariableID()) {
				trovato = true;
				symbTable[i].setValue(var_set->getValue());
				break;
			}
		}
		// ... altrimenti la pusho nella symbTable
		if (!trovato)
			symbTable.push_back(*var_set);
		// TODO: controllare perché vengono allocati due volte nell'accumulator
		// accumulator.push_back(var_set->getValue());
	}

	void visitInputStmt(InputStmt* inputNode) override {
		Variable* var_input = inputNode->getVar();
	
		std::string input;
		std::cout << "Inserire un numero >> ";
		std::cin >> input;

		// Controllo se la stringa inserita in input è un numero intero
		bool is_a_integer = true;
		if (input.empty())
			is_a_integer = false;

		size_t start = 0;
		if (input[0] == '+' || input[0] == '-') {
			start = 1;  // Ignoriamo il segno se presente
		}

		for (size_t i = start; i < input.size(); ++i) {
			if (input[i] < '0' || input[i] > '9') {
				is_a_integer = false;
				break;
			}
		}

		// Se è un intero procedo ad inserire la variabile nella symbolTable,
		// altrimenti genero un'errore di input.
		if (is_a_integer) {
			int64_t value = std::stoi(input);
			var_input->setValue(value);

			// Controllo se la variabile è già presente nella simbol table
			bool trovato = false;
			for (int i = 0; i < symbTable.size(); i++) {
				if (symbTable[i].getVariableID() == var_input->getVariableID()) {
					trovato = true;
					symbTable[i].setValue(value);
					break;
				}
			}
			if (!trovato)
				symbTable.push_back(*var_input);
			accumulator.push_back(var_input->getValue());
		} else {
			throw InputError("The entered value is not an integer");
		}
	}

	void visitWhileStmt(WhileStmt* whileNode) override {
		// Prelevo l'espressione booleana e la valuto
		BoolExpr* boolexpr = whileNode->getBoolExpr();
		boolexpr->accept(this);
		// Entro nel costrutto while se la boolexpr accumula valore positivo
		while (accumulator.back()) {
			accumulator.pop_back();
			// Prelevo il lo stmt_block 
			whileNode->getStmtBlock()->accept(this);
			boolexpr->accept(this);
		}
	}

	void visitIfStmt(IfStmt* ifNode) override {
		// Prelevo l'espressione booleana e la valuto
		BoolExpr* boolexpr = ifNode->getBoolExpr();
		boolexpr->accept(this);
		// Entro nel costrutto if se la boolexpr accumula valore positivo,
		// altrimenti vado nell'else
		if (accumulator.back()) {
			accumulator.pop_back();
			ifNode->getLeftBlock()->accept(this);
		} else {
			accumulator.pop_back();
			ifNode->getRightBlock()->accept(this);
		}
	}

	// NUM EXPR
	void visitOperator(Operator* opNode) override {
		// Prelevo i puntatori agli operandi e gli faccio accettare 
		// questo oggetto come visitatore (propago la visita)
		NumExpr* left = opNode->getLeftOp();
		left->accept(this);
		NumExpr* right = opNode->getRightOp();
		right->accept(this);
		// Prelevo i valori dall'accumulatore
		int64_t rval = accumulator.back(); accumulator.pop_back();
		int64_t lval = accumulator.back(); accumulator.pop_back();
		switch (opNode->getOperand()) {
		case Operator::ADD:
			accumulator.push_back(lval + rval); return;
		case Operator::SUB:
			accumulator.push_back(lval - rval); return;
		case Operator::MUL:
			accumulator.push_back(lval * rval); return;
		case Operator::DIV:
			if (rval != 0) {
				accumulator.push_back(lval / rval);
				return;
			} else {
				throw SemanticError("Division by 0 not allowed");
			}
		default:
			return;
		}
	}

	void visitNumber(Number* numNode) override {
		// Il metodo push_back inserisce il valore come ultimo elemento del vector
		// (equivale ad una PUSH in uno stack)
		accumulator.push_back(numNode->getNumber());
	}

	int64_t getValue() const {
		// Il metodo back() restituisce l'ultimo valore del vector (equivale a una TOP in uno stack)
		return accumulator.back();
	}

	void visitVariable(Variable* varNode) override {
		// Cerco la variabile nella symbTable
		bool trovata = false;
		for (int i = 0; i < symbTable.size(); i++) {
			if (symbTable[i].getVariableID() == varNode->getVariableID()) {
				trovata = true;
				varNode->setValue(symbTable[i].getValue());
				break;
			}
		}
		// Se non è stata trovata stampo errore semantico,
		// altrimenti la pusho nell'accumulatore
		if (!trovata) {
			std::string tmp = "Undefined variable '" + varNode->getVariableID() + "'.";
			throw SemanticError(tmp);
		} else
			accumulator.push_back(varNode->getValue());
	}

	int64_t getVariable() const {
		return accumulator.back();
	}

	// BOOL EXPR
	void visitRelOp(RelOp* relopNode) override {
		// Prelevo gli operandi
		NumExpr* left = relopNode->getLeftOp();
		left->accept(this);
		NumExpr* right = relopNode->getRightOp();
		right->accept(this);
		// Prelevo i valori dall'accumulatore
		int64_t rval = accumulator.back();
		accumulator.pop_back();
		int64_t lval = accumulator.back();
		accumulator.pop_back();
		switch (relopNode->getOperand()) {
		case RelOp::LT:
			accumulator.push_back( lval < rval ); return;
		case RelOp::GT:
			accumulator.push_back(lval > rval); return;
		case RelOp::EQ:
			accumulator.push_back(lval == rval); return;
		default:
			return;
		}
	}

	int64_t getBoolExprValue() const {
		return accumulator.back();
	}

	void visitBoolConst(BoolConst* bcNode) override {
		// Prelevo TRUE o FALSE
		BoolConst::OpCode_BoolConst control = bcNode->getBoolConst();
		if (control == BoolConst::OpCode_BoolConst::TRUE) {
			accumulator.push_back(true);
			return;
		} else if (control == BoolConst::OpCode_BoolConst::FALSE) {
			accumulator.push_back(false);
			return;
		}
	}

	void visitBoolOp(BoolOp* bopNode) override {
		switch (bopNode->getOperand()) {
		case BoolOp::AND: {
			BoolExpr* left = bopNode->getLeftOp();
			left->accept(this);
			int64_t lval = accumulator.back();
			accumulator.pop_back();
			// Cortocircuito l'operatore AND se il primo operando risulta negativo
			if (lval == static_cast<int>(false)) {
				accumulator.push_back(false);
				return;
			}			
			BoolExpr* right = bopNode->getRightOp();
			right->accept(this);
			int64_t rval = accumulator.back();
			accumulator.pop_back();
			
			if (lval == static_cast<int>(true) && rval == static_cast<int>(true)) {
				accumulator.push_back(true);
				return;
			} else {
				accumulator.push_back(false);
				return;
			}
			break;
		}
		case BoolOp::OR: {
			BoolExpr* left = bopNode->getLeftOp();
			left->accept(this);
			int64_t lval = accumulator.back();
			accumulator.pop_back();
			// Cortocircuito l'operatore OR se il primo operando risulta positivo
			if (lval == static_cast<int>(true)) {
				accumulator.push_back(static_cast<int>(true));
				return;
			}
			BoolExpr* right = bopNode->getRightOp();
			right->accept(this);
			int64_t rval = accumulator.back();
			accumulator.pop_back();
			
			if (lval == static_cast<int>(true) || rval == static_cast<int>(true)) {
				accumulator.push_back(static_cast<int>(true));
				return;
			} else {
				accumulator.push_back(static_cast<int>(false));
				return;
			}
			break;
		}
		case BoolOp::NOT: {
			BoolExpr* left = bopNode->getLeftOp();
			left->accept(this);
			int64_t val = accumulator.back();
			accumulator.pop_back();
			if (val == static_cast<int>(true)) {
				accumulator.push_back(static_cast<int>(false));
				return;
			} else if (val == static_cast<int>(false)) {
				accumulator.push_back(static_cast<int>(true));
				return;
			}
			break;
		}
		default:
			return;
		}
	}

private:
	std::vector<int64_t> accumulator;
};

// Visitor concreto per la stampa
class PrintVisitor : public Visitor {
public:
	// BLOCK
	void visitBlock(Block* blockNode) override {
		std::cout << "(BLOCK \n";
		increaseTab();
		// Stampa gli statement propagando la visita
		for (int i = 0; i < blockNode->getDimension(); i++) {
			//std::cout << "   ";
			for (int j = 0; j < tabs; j++)
				std::cout << "   ";
			blockNode->getOnlyAStatement(i)->accept(this);
			std::cout << std::endl;
		}
		decreaseTab();
		for (int j = 0; j < tabs; j++)
			std::cout << "   ";
		std::cout << ")" << std::endl;
		
	}

	// STATEMENT
	void visitPrintStmt(PrintStmt* printNode) override {
		std::cout << "(PRINT ";
		printNode->getPrint()->accept(this);
		std::cout << ")";
	}

	void visitSetStmt(SetStmt* setNode) override {
		std::cout << "(SET ";
		setNode->getVar()->accept(this);
		std::cout << " ";
		setNode->getNumExpr()->accept(this);
		std::cout << ")";
	}

	void visitInputStmt(InputStmt* inputNode) override {
		std::cout << "(INPUT ";
		inputNode->getVar()->accept(this);
		std::cout << ")";
	}

	void visitWhileStmt(WhileStmt* whileNode) override {
		std::cout << "(WHILE ";
		whileNode->getBoolExpr()->accept(this);

		increaseTab();
		std::cout << "\n";

		for (int j = 0; j < tabs; j++)
			std::cout << "   ";
		whileNode->getStmtBlock()->accept(this);

		decreaseTab();
		for (int j = 0; j < tabs; j++)
			std::cout << "   ";
		std::cout << ")";
		//decreaseTab();
	}

	void visitIfStmt(IfStmt* ifNode) override {
		std::cout << "(IF ";
		
		ifNode->getBoolExpr()->accept(this);
		increaseTab();

		std::cout << "\n";
		for (int j = 0; j < tabs; j++)
			std::cout << "   ";
		
		ifNode->getLeftBlock()->accept(this);
		//std::cout << "\n";

		for (int j = 0; j < tabs; j++)
			std::cout << "   ";
		ifNode->getRightBlock()->accept(this);

		decreaseTab();
		for (int j = 0; j < tabs; j++)
			std::cout << "   ";
		std::cout << ")";
		//std::cout << "\n";

		//decreaseTab();
	}

	// NUM EXPR
	void visitOperator(Operator* opNode) override {
		std::cout << "(";
		std::cout << Operator::OpCode_OperatorToString(opNode->getOperand());
		std::cout << " ";
		opNode->getLeftOp()->accept(this);
		std::cout << " ";
		opNode->getRightOp()->accept(this);
		std::cout << ")";
	}

	void visitNumber(Number* numNode) override {
		std::cout << numNode->getNumber();
	}

	void visitVariable(Variable* numNode) override {
		std::cout << numNode->getVariableID();
	}

	// BOOL EXPR
	void visitRelOp(RelOp* relopNode) override {
		std::cout << "(";
		std::cout << RelOp::OpCode_RelOpToString(relopNode->getOperand());
		std::cout << " ";
		relopNode->getLeftOp()->accept(this);
		std::cout << " ";
		relopNode->getRightOp()->accept(this);
		std::cout << ")";
	}

	void visitBoolConst(BoolConst* bcNode) override {
		std::cout << BoolConst::OpCode_BoolConstToString(bcNode->getBoolConst());
	}

	void visitBoolOp(BoolOp* bopNode) override {
		std::cout << "(";
		std::cout << BoolOp::OpCode_BoolOpToString(bopNode->getOperand());
		std::cout << " ";
		if (bopNode->getOperand() == BoolOp::NOT) {
			bopNode->getLeftOp()->accept(this);
			std::cout << ")";
		} else {
			bopNode->getLeftOp()->accept(this);
			std::cout << " ";
			bopNode->getRightOp()->accept(this);
			std::cout << ")";
		}
	}
	
	void setTabs(int value) { tabs = value; }
	void increaseTab(void) { tabs++; }
	void decreaseTab(void) { tabs--; }
private:
	int tabs{}; // TODO: inizializzarlo?
};

class CVisitor : public Visitor {

	void visitBlock(Block* blockNode) override {
		increaseTab();
		for (int i = 0; i < blockNode->getDimension(); i++) {
			for (int i = 0; i < tabs; i++)
				std::cout << "\t";
			blockNode->getOnlyAStatement(i)->accept(this);
		}
		decreaseTab();
	}

	void visitPrintStmt(PrintStmt* printNode) override {
		std::cout << "std::cout << ";
		printNode->getPrint()->accept(this);
		std::cout << " << std::endl;" << std::endl;
	}

	void visitSetStmt(SetStmt* setNode) override {
		// Prelevo nome della variabile
		std::string var_id = setNode->getVar()->getVariableID();

		// Controllo se è già stata inizializzata
		bool trovato = false;
		int index;
		for (int i = 0; i < symbTableforC.size(); i++) {
			// Se è già presente metto trovato = true;
			// altrimenti ...
			if (symbTableforC[i].getVariableID() == var_id) {
				trovato = true;
				index = i;
				break;
			}
		}

		if (trovato) {
			std::cout << var_id << " = ";
			setNode->getNumExpr()->accept(this);
			std::cout << ";" << std::endl;
			symbTableforC[index] = *(setNode->getVar());
		} else {
			std::cout << "int64_t " << var_id << " = ";
			setNode->getNumExpr()->accept(this);
			std::cout << ";" << std::endl;
			symbTableforC.push_back(*(setNode->getVar()));
		}
		
	}

	void visitInputStmt(InputStmt* inputNode) override {
		std::cout << "std::cout >> " << inputNode->getVar()->getVariableID() << ";" << std::endl;
	}

	void visitWhileStmt(WhileStmt* whileNode) override {
		std::cout << "while(";
		whileNode->getBoolExpr()->accept(this);
		std::cout << ") {" << std::endl;

		whileNode->getStmtBlock()->accept(this);
		//decreaseTab();

		for (int i = 0; i < tabs; i++)
			std::cout << "\t";
		std::cout << "}" << std::endl;

	}

	void visitIfStmt(IfStmt* ifNode) override {
		std::cout << "if(";
		ifNode->getBoolExpr()->accept(this);
		std::cout << ") {" << std::endl;

		ifNode->getLeftBlock()->accept(this);

		for (int i = 0; i < tabs; i++)
			std::cout << "\t";
		std::cout << "} else {" << std::endl;

		ifNode->getRightBlock()->accept(this);

		for (int i = 0; i < tabs; i++)
			std::cout << "\t";
		std::cout << "}" << std::endl;

	}

	void visitOperator(Operator* opNode) override {
		NumExpr* left = opNode->getLeftOp();
		left->accept(this);

		switch (opNode->getOperand()) {
		case Operator::ADD: std::cout << " + "; break;
		case Operator::SUB: std::cout << " - "; break;
		case Operator::MUL: std::cout << " * "; break;
		case Operator::DIV: std::cout << " / "; break;
		default: throw SintaxError("Operand not recognized"); break;
		}

		NumExpr* right = opNode->getRightOp();
		right->accept(this);
	}

	void visitNumber(Number* numNode) override {
		int64_t value = numNode->getNumber();
		std::cout << value;
	}

	void visitVariable(Variable* varNode) override {
		
		std::string var_id = varNode->getVariableID();
		std::cout << var_id;		
		/*
		int64_t value = varNode->getValue();
		std::cout << value;
		*/
	}

	void visitRelOp(RelOp* relopNode) override {
		NumExpr* left = relopNode->getLeftOp();
		left->accept(this);

		switch (relopNode->getOperand()) {
		case RelOp::GT: std::cout << " > "; break;
		case RelOp::LT: std::cout << " < "; break;
		case RelOp::EQ: std::cout << " == "; break;
		default: throw SintaxError("Operand not recognized"); break;
		}

		NumExpr* right = relopNode->getRightOp();
		right->accept(this);
	}

	void visitBoolConst(BoolConst* bcNode) override {

	}

	void visitBoolOp(BoolOp* bopNode) override {
		switch (bopNode->getOperand()) {
		case BoolOp::AND: {
			BoolExpr* left = bopNode->getLeftOp();
			left->accept(this);

			std::cout << " && ";

			BoolExpr* right = bopNode->getRightOp();
			right->accept(this);
			break;
		}			
		case BoolOp::OR: {
			BoolExpr* left = bopNode->getLeftOp();
			left->accept(this);

			std::cout << " || ";

			BoolExpr* right = bopNode->getRightOp();
			right->accept(this);
			break;
		}
		case BoolOp::NOT: {
			std::cout << "!(";
			BoolExpr* left = bopNode->getLeftOp();
			left->accept(this);
			std::cout << ")";
			break;
		}
		default: throw SintaxError("Operand not recognized"); break;
		}
	}

	void setTabs(int value) { tabs = value; }
	void increaseTab(void) { tabs++; }
	void decreaseTab(void) { tabs--; }
private:
	int tabs{};
};


#endif
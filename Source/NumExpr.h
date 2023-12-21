#ifndef NUMEXPR_H
#define NUMEXPR_H

#include <string>

#include "Exceptions.h"

class Visitor;

// CLASSE NUMEXPR
// Classe Virtuale. Di conseguenza, dovro istanziare
// solamente OPERATOR, NUMBER, VARIABLE
class NumExpr {
public:
	virtual ~NumExpr() { };
	virtual void accept(Visitor* v) = 0;
};

// CLASSE OPERATOR 
// Classe Derivata
class Operator : public NumExpr {
public:
	enum OpCode_Operator { ADD, SUB, MUL, DIV };

	Operator(OpCode_Operator o, NumExpr* lop, NumExpr* rop)
		: op{ o }, l_op{ lop }, r_op{ rop } { }

	Operator(const Operator& other) = default;
	Operator& operator=(const Operator& other) = default;

	~Operator() = default;

	OpCode_Operator getOperand() const { return op; }
	NumExpr* getLeftOp() const { return l_op; }
	NumExpr* getRightOp() const { return r_op; }

	void accept(Visitor* v) override;

	// Metodo "a livello di classe": non opera su un particolare oggetto, 
	// ma è generale e serve a fornire la conversione
	static OpCode_Operator stringToOpCode_Operator(std::string s) {
		if (s == "ADD")		 return ADD;
		else if (s == "SUB") return SUB;
		else if (s == "MUL") return MUL;
		else if (s == "DIV") return DIV;
		else throw SintaxError("Operator not recognized");
	}
	static std::string OpCode_OperatorToString(OpCode_Operator op) {
		switch (op) {
		case ADD: return "ADD";
		case SUB: return "SUB";
		case MUL: return "MUL";
		case DIV: return "DIV";
		default: throw SintaxError("Operator not recognized");
		}
	}

private:
	OpCode_Operator op; // Operator ADD, SUB, MUL, DIV
	NumExpr* l_op;		// left operand
	NumExpr* r_op;		// right operand
};

// CLASSE NUMBER
// Classe Derivata
class Number : public NumExpr {
public:
	Number(int64_t v) : value{ v } { }
	~Number() = default;

	Number(const Number& other) = default;
	Number& operator=(const Number& other) = default;

	int64_t getNumber() const { return value; }

	void accept(Visitor* v) override;
private:
	int64_t value;
};

// CLASSE VARIABLE
// Classe Derivata
class Variable : public NumExpr {
public:
	Variable(std::string var_id, int64_t v)
		: variable_id{ var_id }, value{ v } { }

	Variable(const Variable& other) = default;
	Variable& operator=(const Variable& other) = default;

	~Variable() = default;

	std::string getVariableID() const { return variable_id; }
	int64_t getValue() const { return value; }

	void setValue(int64_t v) { value = v; }

	void accept(Visitor* v) override;

private:
	std::string variable_id;
	int64_t value;
};

#endif
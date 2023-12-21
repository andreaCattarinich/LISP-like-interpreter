#ifndef BOOLEXPR_H
#define BOOLEXPR_H

#include "Exceptions.h"
#include "NumExpr.h"

// CLASSE STATEMENT
// Classe Virtuale. Di conseguenza, dovro istanziare
// solamente RELOP, BOOLCONST, BOOLOP
class BoolExpr {
public:
	virtual ~BoolExpr() { };
	virtual void accept(Visitor* v) = 0;
};

// CLASSE RELOP 
// Classe Derivata
class RelOp : public BoolExpr {
public:
	enum OpCode_RelOp { LT, GT, EQ };

	RelOp(OpCode_RelOp o, NumExpr* lop, NumExpr* rop)
		: op{ o }, l_op{ lop }, r_op{ rop } { }

	RelOp(const RelOp& other) = default;
	RelOp& operator=(const RelOp& other) = default;

	~RelOp() = default;

	OpCode_RelOp getOperand() const { return op; }
	NumExpr* getRightOp() const { return r_op; }
	NumExpr* getLeftOp() const { return l_op; }

	void accept(Visitor* v) override;

	// Metodo "a livello di classe": non opera su un particolare oggetto, 
	// ma è generale e serve a fornire la conversione
	static OpCode_RelOp stringToOpCode_Operator(std::string s) {
		if (s == "LT")		return LT;
		else if (s == "GT") return GT;
		else if (s == "EQ") return EQ;
		else throw SintaxError("Rel Operand not recognized");
	}

	static std::string OpCode_RelOpToString(OpCode_RelOp op) {
		switch (op) {
		case LT: return "LT";
		case GT: return "GT";
		case EQ: return "EQ";
		default: throw SintaxError("Rel Operand not recognized");
		}
	}
private:
	OpCode_RelOp op; // Operando LT, GT, EQ
	NumExpr* l_op;   // right operand
	NumExpr* r_op;   // left operand
};

// CLASSE BOOLCONST 
// Classe Derivata
class BoolConst : public BoolExpr {
public:
	enum OpCode_BoolConst { TRUE, FALSE };
	BoolConst(OpCode_BoolConst o) : _const{ o } { }

	BoolConst(const BoolConst& other) = default;
	BoolConst& operator=(const BoolConst& other) = default;

	OpCode_BoolConst getBoolConst() const { return _const; }

	void accept(Visitor* v) override;

	static OpCode_BoolConst stringToOpCode_Operator(std::string s) {
		if (s == "TRUE")	return TRUE;
		else if (s == "FALSE") return FALSE;
		else throw SintaxError("Bool Const not recognized");
	}

	static std::string OpCode_BoolConstToString(OpCode_BoolConst op) {
		switch (op) {
		case TRUE: return "TRUE";
		case FALSE: return "FALSE";
		default: throw SintaxError("Bool Const not recognized");
		}
	}

private:
	OpCode_BoolConst _const; // Costante TRUE o FALSE
};

// CLASSE BOOLOP 
// Classe Derivata
class BoolOp : public BoolExpr {
public:
	enum OpCode_BoolOp { AND, OR, NOT };

	BoolOp(OpCode_BoolOp o, BoolExpr* lop, BoolExpr* rop)
		: op{ o }, l_op{ lop }, r_op{ rop } { }

	BoolOp(OpCode_BoolOp o, BoolExpr* not_be)
		: op{ o }, l_op{ not_be }, r_op{ nullptr } { }

	BoolOp(const BoolOp& other) = default;
	BoolOp& operator=(const BoolOp& other) = default;

	~BoolOp() = default;

	OpCode_BoolOp getOperand() const { return op; }
	BoolExpr* getLeftOp() const { return l_op; }
	BoolExpr* getRightOp() const { return r_op; }

	void accept(Visitor* v) override;

	static OpCode_BoolOp stringToOpCode_Operator(std::string s) {
		if (s == "AND")	return AND;
		else if (s == "OR") return OR;
		else if (s == "NOT") return NOT;
		else throw SintaxError("Bool Operator not recognized");
	}

	static std::string OpCode_BoolOpToString(OpCode_BoolOp op) {
		switch (op) {
		case AND: return "AND";
		case OR: return "OR";
		case NOT: return "NOT";
		default: throw SintaxError("Bool Operator not recognized");
		}
	}

private:
	OpCode_BoolOp op; // Operando AND, OR, NOT
	BoolExpr* l_op;	  // right operand
	BoolExpr* r_op;   // left operand
};

#endif
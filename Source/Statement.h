#ifndef STATEMENT_H
#define STATEMENT_H

#include "NumExpr.h"
#include "BoolExpr.h"
//#include "Block.h"

class Block;

// CLASSE STATEMENT
// Classe Virtuale. Di conseguenza, dovro istanziare
// solamente PRINT, SET, INPUT, WHILE, IF
class Statement {
public:
	virtual ~Statement() { }
	virtual void accept(Visitor* v) = 0;
};

// CLASSE PRINT 
// Classe Derivata
class PrintStmt : public Statement {
public:
	PrintStmt(NumExpr* ne) : num_expr{ ne } {}

	PrintStmt(const PrintStmt& other) = default;
	PrintStmt& operator=(const PrintStmt& other) = default;

	~PrintStmt() = default;

	NumExpr* getPrint() { return num_expr; }

	void accept(Visitor* v) override;

private:
	NumExpr* num_expr;
};

// CLASSE SETSTMT 
// Classe Derivata
class SetStmt : public Statement {
public:
	SetStmt(Variable* var, NumExpr* ne)
		: var_id{ var }, num_expr{ ne } { }

	SetStmt(const SetStmt& other) = default;
	SetStmt& operator=(const SetStmt& other) = default;

	~SetStmt() = default;

	Variable* getVar() const { return var_id; }
	NumExpr* getNumExpr() const { return num_expr; }

	void setVar(Variable* var) { var_id = var; }
	void setNumExpr(Variable* v) { num_expr = v; }

	void accept(Visitor* v) override;

private:
	Variable* var_id;
	NumExpr* num_expr;
};

// CLASSE INPUTSTMT 
// Classe Derivata
class InputStmt : public Statement {
public:
	InputStmt(Variable* v) : var_id{ v } { }

	InputStmt(const InputStmt& other) = default;
	InputStmt& operator=(const InputStmt& other) = default;

	~InputStmt() = default;

	Variable* getVar() const { return var_id; }

	void accept(Visitor* v) override;

private:
	Variable* var_id;
};

// CLASSE WHILESTMT
// Classe Derivata
class WhileStmt : public Statement {
public:
	
	WhileStmt(BoolExpr* be, Block* stmt_b)
		: bool_expr{ be }, stmt_block{ stmt_b } { }
	

	WhileStmt(const WhileStmt& other) = default;
	WhileStmt& operator=(const WhileStmt& other) = default;

	~WhileStmt() = default;

	BoolExpr* getBoolExpr() const { return bool_expr; }
	Block* getStmtBlock() const { return stmt_block; }

	void accept(Visitor* v);
private:
	BoolExpr* bool_expr;
	Block* stmt_block;
};

// CLASSE IFSTMT 
// Classe Derivata
class IfStmt : public Statement {
public:
	IfStmt(BoolExpr* be, Block* lb, Block* rb)
		: bool_expr{ be }, l_stmt_block{ lb }, r_stmt_block{ rb } {}

	IfStmt(const IfStmt& other) = default;
	IfStmt& operator=(const IfStmt& other) = default;

	~IfStmt() = default;

	BoolExpr* getBoolExpr() const { return bool_expr; }
	Block* getLeftBlock() const { return l_stmt_block; }
	Block* getRightBlock() const { return r_stmt_block; }

	void accept(Visitor* v);
private:
	BoolExpr* bool_expr;
	Block* l_stmt_block;
	Block* r_stmt_block;
};

#endif
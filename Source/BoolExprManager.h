#ifndef BOOLEXPRMANAGER_H
#define BOOLEXPRMANAGER_H

#include <vector>

#include "BoolExpr.h"

class BoolExprManager {
public:
	BoolExprManager() = default;
	~BoolExprManager() {
		clearMemory();
	};

	BoolExprManager(const BoolExprManager& other) = delete;
	BoolExprManager& operator=(const BoolExprManager& other) = delete;

	BoolExpr* makeRelOp(RelOp::OpCode_RelOp op, NumExpr* l_op, NumExpr* r_op) {
		BoolExpr* rel_op = new RelOp(op, l_op, r_op);
		allocated.push_back(rel_op);
		return rel_op;
	}

	BoolExpr* makeBoolConst(BoolConst::OpCode_BoolConst op) {
		BoolExpr* bc = new BoolConst(op);
		allocated.push_back(bc);
		return bc;
	}

	BoolExpr* makeBoolOp(BoolOp::OpCode_BoolOp op, BoolExpr* lbe, BoolExpr* rbe) {
		BoolExpr* be = new BoolOp(op, lbe, rbe);
		allocated.push_back(be);
		return be;
	}

	BoolExpr* makeBoolOp(BoolOp::OpCode_BoolOp op, BoolExpr* not_bool_expr) {
		BoolExpr* be = new BoolOp(op, not_bool_expr);
		allocated.push_back(be);
		return be;
	}

	void clearMemory() {
		auto i = allocated.begin();
		for (; i != allocated.end(); ++i) {
			delete(*i);
		}
		allocated.resize(0);
	}

private:
	std::vector<BoolExpr*> allocated;
};

#endif // !NUMEXPRMANAGER_H
#include "NumExpr.h"
#include "Visitor.h"

void Operator::accept(Visitor* v) {
    v->visitOperator(this);
}

void Number::accept(Visitor* v) {
    v->visitNumber(this);
}

void Variable::accept(Visitor* v) {
    v->visitVariable(this);
}
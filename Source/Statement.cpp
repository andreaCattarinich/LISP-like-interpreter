#include "Statement.h"
#include "Visitor.h"

void PrintStmt::accept(Visitor* v) {
    v->visitPrintStmt(this);
}

void SetStmt::accept(Visitor* v) {
    v->visitSetStmt(this);
}

void InputStmt::accept(Visitor* v) {
    v->visitInputStmt(this);
}

void WhileStmt::accept(Visitor* v) {
    v->visitWhileStmt(this);
}

void IfStmt::accept(Visitor* v) {
    v->visitIfStmt(this);
}
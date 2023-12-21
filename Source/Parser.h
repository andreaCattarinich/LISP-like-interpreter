#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>

#include "Exceptions.h"

#include "NumExprManager.h"
#include "BoolExprManager.h"
#include "StatementManager.h"
#include "BlockManager.h"

// CLASSE PARSE
// Utile ad effettuare il parse prendendo in input l'albero lessicale
// creato in fase di tokenizzazione.
class Parse {
public:
    Parse(BlockManager& block_manager, StatementManager& stmt_manager, NumExprManager& num_expr_manager, 
        NumExprManager& var_manager, BoolExprManager& bool_expr_manager)
        : blm{block_manager}, stmt_man{ stmt_manager }, nem{ num_expr_manager }, varm{ var_manager },
            bem{ bool_expr_manager } { }

    // Function Object, nonché l'overloading dell'operatore ().
    // Il valore di ritorno è un puntatore a Block.
    Block* operator()(const std::vector<Token>& tokenStream) {
        // Utilizzo auto per evitare di scrivere per esteso il tipo dell'iteratore
        auto tokenItr = tokenStream.begin();
        streamEnd = std::prev(tokenStream.end());
        streamStart = tokenStream.begin();

        // Chiamo la funzione ricorsiva parsing 
        Block* block = recursiveParseBlock(tokenItr);

        // Controllo di essere arrivato alla fine del vettore tokenStream
        if (tokenItr == streamEnd)
            return block;
        else
            throw ParseError("Warning: the program has not reached the end of the parsing");
    }

private:
    // Variabile che contiene l'ultimo token
    std::vector<Token>::const_iterator streamEnd;
    std::vector<Token>::const_iterator streamStart;

    // Riferimenti ai manager
    BlockManager& blm;
    StatementManager& stmt_man;
    NumExprManager& nem;
    NumExprManager& varm;
    BoolExprManager& bem;

    // Dichiarazione delle funzioni di parsing
    Block* recursiveParseBlock(std::vector<Token>::const_iterator& tokenItr);
    Statement* recursiveParseStatement(std::vector<Token>::const_iterator& tokenItr);
    NumExpr* recursiveParseNumExpr(std::vector<Token>::const_iterator& tokenItr);
    Variable* parseVariable(std::vector<Token>::const_iterator& itr);
    BoolExpr* recursiveParseBoolExpr(std::vector<Token>::const_iterator& itr);

    // Funzione che controlla di non uscire dal vettore di token
    void safe_next(std::vector<Token>::const_iterator& itr) {
        if (itr == streamEnd) {
            throw ParseError("Unexpected end of input");
        }
        ++itr;
    }

    // Funzione che controlla di non uscire dal vettore di token
    void safe_prev(std::vector<Token>::const_iterator& itr) {
        if (itr == streamStart) {
            throw ParseError("Unexpected start of input");
        }
        --itr;
    }
};

#endif
#include <sstream>

#include "Exceptions.h"
#include "Token.h"
#include "Parser.h"
#include "Block.h"
#include "Statement.h"
#include "NumExpr.h"
#include "BoolExpr.h"

// Recursive parse per i Block
Block* Parse::recursiveParseBlock(std::vector<Token>::const_iterator& itr) {
	if (itr->tag == Token::LP) {
		safe_next(itr);
		switch (itr->tag) {
			case Token::BLOCK: {
				safe_next(itr);
				// Controllo se un BLOCK è vuoto: (BLOCK ) da errore
				if (itr->tag == Token::RP)
					throw SintaxError("Empty Block");

				// Controllo se un BLOCK contiene un altro BLOCK
				// Es: (BLOCK (BLOCK (ADD 1 2))) da errore
				safe_next(itr);
				if (itr->tag == Token::BLOCK) {
					throw SintaxError("A BLOCK contains an interior BLOCK. It must have a STMT_BLOCK");
				} else {
					safe_prev(itr);
				}

				std::vector<Statement*> temporaneo;

				// RICORDARE LA GRAMMATICA:
				// 1) stmt_block -> statement | ( BLOCK statement_list )
				// 2) statement_list -> statement statement_list | statement
				// 
				// 3) statement -> variable_def | 
				//				io_stmt |
				//				cond_stmt | 
				//				loop_stmt
				// 
				// 4) variable_def -> ( SET variable_id num_expr )
				// 5) io_stmt -> ( PRINT num_expr ) | (INPUT variable_id )
				// 6) cond_stmt -> ( IF boolexpr stmt_block stmt_block )
				// 7) loop_stmt -> ( WHILE bool_expr stmt_block )
				// 
				// Osservazione: fino ad adesso abbiamo sviluppato solamente la regola 1)
				// Per sviluppare la 2) dobbiamo:
				// - Prendere il primo statement
				// - Vedere se ci sono altri statement e aggiungerli alla lista
				
				// Attenzione: gli statement NON potranno essere quelli delle num_expr e/o bool_expr.
				// Esempio:
				// (BLOCK (ADD 3 1)) -> ERRORE
				// (BLOCK (SET a 1)) -> CORRETTA
				
				// Prendo il primo statement
				temporaneo.push_back(recursiveParseStatement(itr));

				// Creo il blocco (con il primo statement)
				Block* block = new Block(temporaneo);
				
				safe_next(itr);
				// Fino a quando trovo '(' prendo gli statement e li aggiungo al block.
				while (itr->tag == Token::LP) {
					block->AddStatement(recursiveParseStatement(itr));
					safe_next(itr);
				}
				// Quando ho finito di prelevare tutti gli statement controllo di avere la parentesi chiusa
				// corrispondente al blocco, esempio:
				// ( BLOCK 
				//		....
				// )
				if(itr->tag != Token::RP)
					throw ParseError("Mismatched parenthesis");

				// Ritorno il blocco
				return blm.makeBlock(*block);
				break;
			}
			default: {
				if (itr->tag != Token::SET && itr->tag != Token::PRINT &&
					itr->tag != Token::INPUT && itr->tag != Token::IF &&
					itr->tag != Token::WHILE) {
					std::stringstream tmp{};
					tmp << "A statement cannot contains a " << Token::id2word[itr->tag] << " statement.";
					throw SintaxError(tmp.str());
				}
				safe_prev(itr);

				std::vector<Statement*> temp;
				temp.push_back(recursiveParseStatement(itr));
				Block* block = new Block(temp);
				/*
				safe_next(itr);
				if (itr->tag == Token::LP) {
					Block* another_block = recursiveParseBlock(itr);
				}
				*/
				return blm.makeBlock(*block);
				break;
			}			
		}
	} else
		throw ParseError("Unexpected token");
}

// Recursive parse per gli Statement
Statement* Parse::recursiveParseStatement(std::vector<Token>::const_iterator& itr) {
	if (itr->tag == Token::LP) {
		safe_next(itr);
		switch (itr->tag) {
			case Token::PRINT: { // ( PRINT num_expr )
				safe_next(itr);
				NumExpr* num_expr = recursiveParseNumExpr(itr);
				safe_next(itr);
				if (itr->tag == Token::RP) {
					return stmt_man.makePrintStatement(num_expr);
				} else {
					throw ParseError("Mismatched parenthesis");
				}
				break;
			}
			case Token::SET: { // ( SET variable_id num_expr )
				safe_next(itr);
				Variable* var_id = parseVariable(itr);
				safe_next(itr);
				NumExpr* num_expr = recursiveParseNumExpr(itr);
				safe_next(itr);
				if (itr->tag == Token::RP) {
					return stmt_man.makeSetStatement(var_id, num_expr);
				} else {
					throw ParseError("Mismatched parenthesis");
				}
				break;
			}
			case Token::INPUT: { // ( INPUT variable_id )
				safe_next(itr);
				Variable* var_id = parseVariable(itr);
				safe_next(itr);
				if (itr->tag == Token::RP) {
					return stmt_man.makeInputStatement(var_id);
				} else {
					throw ParseError("Mismatched parenthesis");
				}
				break;
			}
			case Token::WHILE: { // ( WHILE bool_expr stmt_block
				safe_next(itr);
				BoolExpr* bool_expr = recursiveParseBoolExpr(itr);
				safe_next(itr);
				Block* block_while = recursiveParseBlock(itr);
				safe_next(itr);
				if (itr->tag == Token::RP) {
					return stmt_man.makeWhileStatement(bool_expr, block_while);
				} else {
					throw ParseError("Mismatched parenthesis");
				}
				break;
			}
			case Token::IF: { // ( IF bool_expr stmt_block1 stmt_block2 )
				safe_next(itr);
				BoolExpr* bool_expr = recursiveParseBoolExpr(itr);
				safe_next(itr);
				Block* stmt_block1 = recursiveParseBlock(itr);
				safe_next(itr);
				Block* stmt_block2 = recursiveParseBlock(itr);
				safe_next(itr);
				if (itr->tag == Token::RP) {
					return stmt_man.makeIfStatement(bool_expr, stmt_block1, stmt_block2);
				} else {
					throw ParseError("Mismatched parenthesis");
				}
				break;
			}
			default: {
				// Se la parola chiave non è una degli statement (ovvero, PRINT, SET, INPUT, IF, WHILE)
				// Allora do un errore semantico: la parola non è una parola chiave.
				bool trovata = false;
				for (int i = 0; i < 21; i++) {
					if (Token::id2word[i] == itr->word) {
						trovata = true;
						break;
					}
				}
				if (trovata) {
					std::stringstream tmp{};
					tmp << "A block must con contains a statement or a statement_list, not a " << Token::id2word[itr->tag] << " statement.";
					throw ParseError(tmp.str());
				} else {
					std::stringstream tmp{};
					tmp << "A " << itr->word << " is not a word key for a statement.";
					throw SintaxError(tmp.str());
				}
				
				break;
			}
		} // Fine dello switch
	} else // Se non è parentesi tonda lancia un errore
		throw ParseError("Unexpected token");
}

// Recursive parse per i numeri
NumExpr* Parse::recursiveParseNumExpr(std::vector<Token>::const_iterator& itr) {
	// Una num_expr corretta inizia per: '(', oppure un Number, oppure una 
	if (itr->tag == Token::LP) { // Parentesi aperta
		safe_next(itr);
		if (itr->tag >= Token::ADD && itr->tag <= Token::DIV) { // [ADD, SUB, MUL, DIV] -> [17, 18, 19, 20]
			std::string stringa_operatore = itr->word;
			safe_next(itr);
			NumExpr* first_op = recursiveParseNumExpr(itr);
			safe_next(itr);
			NumExpr* second_op = recursiveParseNumExpr(itr);
			safe_next(itr);
			if (itr->tag == Token::RP) {
				return nem.makeOperator(Operator::stringToOpCode_Operator(stringa_operatore), first_op, second_op);
			} else {
				throw ParseError("Mismatched parenthesis");
			}
		} else { // Si entrerà in questa eccezione ogni qualvolta si ha un programma del tipo:
				 // (SET a (add 1 2)) Infatti l'operando 'add' non viene riconosciuto.
			throw SintaxError("Operand not recognized");
		}
	} else if (itr->tag == Token::NUMERO) { // Numero
		// Conversione da stringa a intero
		std::stringstream temp{};
		temp << itr->word;
		int64_t value;
		temp >> value;
		NumExpr* expr = nem.makeNumber(value);
		return expr;
	} else if (itr->tag == Token::VARIABILE) { // Variabile
		int64_t value = -1024; // Inizializzo le variabili dentro alle
						       // num_expr con il valore -1024
		NumExpr* expr = nem.makeVariable(itr->word, value);
		return expr;
	} else {
		throw ParseError("Unexpected token");
	}
}

// Parse per le variabili
Variable* Parse::parseVariable(std::vector<Token>::const_iterator& itr) {
	if (itr->tag == Token::VARIABILE) {
		int64_t value = -2048; // Inizializzo le variabili "solitarie" con il valore -2048
		Variable* expr = varm.makeVariableV(itr->word, value);
		return expr;
	} else {
		throw ParseError("Wrong identifier");
	}
}

// Recursive parse per le BoolExpr
BoolExpr* Parse::recursiveParseBoolExpr(std::vector<Token>::const_iterator& itr) {
	if (itr->tag == Token::LP) { // Parentesi aperta
		safe_next(itr);			 // Controlla se è (LT, (GT, o (EQ
		if (itr->tag == Token::LT || itr->tag == Token::GT || itr->tag == Token::EQ) {
			std::string string_rel_op = itr->word;
			safe_next(itr);
			NumExpr* first_op = recursiveParseNumExpr(itr);
			safe_next(itr);
			NumExpr* second_op = recursiveParseNumExpr(itr);
			safe_next(itr);
			if (itr->tag == Token::RP) {
				return bem.makeRelOp(RelOp::stringToOpCode_Operator(string_rel_op), first_op, second_op);
			} else {
				throw ParseError("Mismatched parenthesis");
			}					// Controlla se è (AND o (OR
		} else if (itr->tag == Token::AND || itr->tag == Token::OR) {
			std::string string_bool_op = itr->word;
			safe_next(itr);
			BoolExpr* first_op = recursiveParseBoolExpr(itr);
			safe_next(itr);
			BoolExpr* second_op = recursiveParseBoolExpr(itr);
			safe_next(itr);
			if (itr->tag == Token::RP) {
				return bem.makeBoolOp(BoolOp::stringToOpCode_Operator(string_bool_op), first_op, second_op);
			} else {
				throw ParseError("Mismatched parenthesis");
			}
		} else if (itr->tag == Token::NOT) { // Controlla se è (NOT
			std::string string_not = itr->word;
			safe_next(itr);
			BoolExpr* first_op = recursiveParseBoolExpr(itr);
			safe_next(itr);
			if (itr->tag == Token::RP) {
				return bem.makeBoolOp(BoolOp::stringToOpCode_Operator(string_not), first_op);
			} else {
				throw ParseError("Mismatched parenthesis");
			}
		} else {
			throw SintaxError("Operand not recognized");
		}
					// Controlla se è TRUE o FALSE
	} else if (itr->tag == Token::TRUE || itr->tag == Token::FALSE) {
		return bem.makeBoolConst(BoolConst::stringToOpCode_Operator(itr->word));
	} else {
		throw ParseError("Unexpected token");
	}
}
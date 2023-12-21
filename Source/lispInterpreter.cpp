#include <iostream>
#include <fstream>
#include <vector>
//#include <filesystem>

#include "Exceptions.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Parser.h"

#include "Visitor.h"

#include "SymbolTable.h"
#include "StatementManager.h"

//#define DEBUG_MODE
//#define ONLY_OUTPUT_CONSOLE

int main(int argc, char** argv) {
    // Controllo la correttezza dei parametri passati da linea di comando
    // Mi servono 2 parametri:
    // - percorso + nome file (LispInterpreter.exe) da eseguire
    // - percorso + nome file da interpretare
    
    if (argc < 2) {
        std::cerr << "File non specificato" << std::endl;
        std::cerr << "Utilizzare: " << argv[0] << " <nome_file>" << std::endl;
        return EXIT_FAILURE;
    }

    // FASE DI APERTURA FILE 
    std::ifstream inputFile;

    // Da usare manualmente:
    // std::string arg1{ "C:\\Users\\andyc\\OneDrive\\Proj_lispInterpreter\\LispInterpreter_TestFiles\\"
    //    "z.txt" };

    // Controlla che il file venga trovato nel filesystem
    inputFile.open(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "File " << argv[1] << " was not found\n" << std::endl;
        return EXIT_FAILURE;
    }

    #ifndef ONLY_OUTPUT_CONSOLE
    std::cout << "- " << argv[1] << " was opened correctly\n" << std::endl;
    #endif // !ONLY_OUTPUT_CONSOLE

    // FASE DI TOKENIZZAZIONE
    Tokenizer tokenize;
    std::vector<Token> inputTokens;

    try {
        // Chiamo il function object, ovvero la "funzione" overloaddata
        // che restituirà in inputTokens tutto l'array di Token lessicalmente corretto.
        inputTokens = tokenize(inputFile);
        inputFile.close();

        #ifndef ONLY_OUTPUT_CONSOLE
        std::cout << "- Tokenization completed successfully\n" << std::endl;
        #endif // !ONLY_OUTPUT_CONSOLE

        
    } catch (LexicalError& le) {
        std::cerr << "Lexical ERROR: ";
        std::cerr << le.what() << std::endl;
        return EXIT_FAILURE;
    } catch (SintaxError& se) {
        std::cerr << "Sintax ERROR: ";
        std::cerr << se.what() << std::endl;
        return EXIT_FAILURE;
    } catch (std::exception& exc) {
        std::cerr << "ERROR Cannot read from " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    #ifdef DEBUG_MODE
    for (Token t : inputTokens) 
        std::cout << t << std::endl;
    std::cout << "The file " << fileNameTXT << " contains " << inputTokens.size() << " tokens." << std::endl;
    #endif //DEBUG_MODE
        
    // FASE DI PARSING
    // Creo i manager per i nodi
    BlockManager BLmanager;
    StatementManager STmanager;
    NumExprManager NEmanager;
    NumExprManager VARmanager;
    BoolExprManager BEmanager;

    // Creo il function object per il parsing
    Parse parse{ BLmanager, STmanager, NEmanager, VARmanager, BEmanager };

    Block* block;
    std::vector<Statement*> vect_stmt;

    try {
        // Chiamo il function object, che restituirà in Block
        // il puntatore al primo blocco.
        block = parse(inputTokens);
        
        #ifndef ONLY_OUTPUT_CONSOLE
        std::cout << "- Parsing completed successfully\n" << std::endl;

        PrintVisitor* p = new PrintVisitor();

        std::cout << "- LISP program print in progress...\n" << std::endl;

        // Il seguente comando serve per l'identazione del programma
        p->setTabs(0);

        block->accept(p);
        std::cout << std::endl;

        // Ho eseguito anche la conversione in programma C++
        // (Non richiesto)
        std::cout << std::endl << "- Conversion to C++ in progress...\n" << std::endl;

        CVisitor* c = new CVisitor();

        std::cout << "#include <iostream>" << std::endl << std::endl;

        std::cout << "int main(){" << std::endl;
        block->accept(c);
        std::cout << "\treturn 0;" << std::endl << "}" << std::endl;
        
        std::cout << std::endl << "- LISP Evaluation in progress...\n" << std::endl;
        #endif // !ONLY_OUTPUT_CONSOLE    
        

        // Valutazione del programma LISP
        EvaluationVisitor* v = new EvaluationVisitor();
        block->accept(v);

        // Se ci fosse bisogno di controllare i valori della symbol table si può marcare DEBUG_MODE
        #ifdef DEBUG_MODE
        std::cout << "\nSYMBOL TABLE:" << std::endl;
        for (int i = 0; i < symbTable.size(); i++)
            std::cout << "- " << symbTable[i].getVariableID() << " = " << symbTable[i].getValue() << std::endl;
        #endif // DEBUG_MODE

    } catch (ParseError& pe) {
        std::cerr << "Parse ERROR: ";
        std::cerr << pe.what() << std::endl;
        return EXIT_FAILURE;
    } catch (SintaxError& se) {
        std::cerr << "Sintax ERROR: ";
        std::cerr << se.what() << std::endl;
        return EXIT_FAILURE;
    } catch (SemanticError& seme) {
        std::cerr << "Semantic ERROR: ";
        std::cerr << seme.what() << std::endl;
        return EXIT_FAILURE;
    } catch (InputError& ie) {
        std::cerr << "Input ERROR: ";
        std::cerr << ie.what() << std::endl;
        return EXIT_FAILURE;
    } catch (std::exception& exc) {
        // Catturo qualsiasi altra eccezione si verifichi
        std::cerr << "Generic ERROR " << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    //std::cout << std::endl;

    //std::cout << "Program terminated successfully" << std::endl;
    return 0;
}
# Lisp Interpreter

## Descrizione
Questo interprete Lisp è progettato per eseguire file di test `.txt` contenenti istruzioni Lisp.

## Istruzioni per l'uso

### Compilazione del programma

Prima di eseguire il programma, è necessario compilare i file sorgente utilizzando il compilatore C++ `g++`. Seguire i passaggi seguenti:

1. Assicurarsi di avere `g++` installato nel proprio sistema.
2. Aprire il terminale o la riga di comando.
3. Navigare alla directory contenente i file sorgente del programma LispInterpreter.
4. Eseguire il comando di compilazione utilizzando `g++` per tutti i file `.cpp`:

    ```bash
    g++ -o LispInterpreter nome_file_1.cpp nome_file_2.cpp ... nome_file_n.cpp
    ```

    Sostituire `nome_file_1.cpp`, `nome_file_2.cpp`, ecc., con i nomi effettivi dei file sorgente necessari per compilare il programma LispInterpreter.

### Esecuzione del programma compilato

Una volta compilato con successo il programma, è possibile eseguirlo nel seguente modo:

Assicurarsi di essere nella directory contenente il programma compilato `LispInterpreter` e il file di test `.txt`.

Eseguire il programma compilato utilizzando il comando seguente:

```bash
./LispInterpreter ../TestFiles/file_name.txt

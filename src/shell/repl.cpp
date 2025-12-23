#include "shell/repl.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace miniql {

REPL::REPL() : running_(true) {}

REPL::~REPL() {}

void REPL::run() {
    printWelcome();
    
    std::string buffer;
    
    while (running_) {
        // Define o prompt baseado se estamos continuando um comando
        std::string prompt = buffer.empty() ? "miniql> " : "     -> ";
        std::string line = readLine(prompt);
        
        // Ignora linhas vazias
        if (line.empty() && buffer.empty()) {
            continue;
        }
        
        // Comandos meta têm prioridade
        if (line[0] == '.' && buffer.empty()) {
            if (!processMetaCommand(line)) {
                continue;
            }
        } else {
            // Adiciona ao buffer
            buffer += line + " ";
            
            // Se termina com ;, processa o comando
            if (!line.empty() && line.back() == ';') {
                // Remove o ; e espaços extras
                buffer.pop_back(); // remove ;
                
                // Remove espaços em branco extras
                buffer.erase(0, buffer.find_first_not_of(" \t\n\r"));
                buffer.erase(buffer.find_last_not_of(" \t\n\r") + 1);
                
                if (!buffer.empty()) {
                    processSQLCommand(buffer);
                }
                
                buffer.clear();
            }
        }
    }
}

bool REPL::processMetaCommand(const std::string& command) {
    if (command == ".exit" || command == ".quit") {
        std::cout << "Goodbye!\n";
        running_ = false;
        return false;
    }
    else if (command == ".help") {
        printHelp();
        return false;
    }
    else if (command == ".tables") {
        std::cout << "No tables yet.\n";
        std::cout << "(Database engine not implemented)\n";
        return false;
    }
    else if (command.substr(0, 7) == ".schema") {
        std::cout << "Schema command not implemented yet.\n";
        return false;
    }
    else {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Type .help for available commands.\n";
        return false;
    }
    
    return true;
}

void REPL::processSQLCommand(const std::string& sql) {
    std::cout << "SQL Command received: " << sql << "\n";
    std::cout << "(SQL execution not implemented yet)\n";
    
    // TODO: Aqui será a integração com:
    // 1. Lexer (tokenização)
    // 2. Parser (geração de AST)
    // 3. Executor (execução)
    
    // Por enquanto, apenas exibe o comando recebido
}

std::string REPL::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::cout.flush();
    
    std::string line;
    if (!std::getline(std::cin, line)) {
        // EOF (Ctrl+D no Unix, Ctrl+Z no Windows)
        running_ = false;
        std::cout << "\n";
        return "";
    }
    
    // Remove espaços em branco das pontas
    line.erase(0, line.find_first_not_of(" \t\n\r"));
    line.erase(line.find_last_not_of(" \t\n\r") + 1);
    
    return line;
}

void REPL::printWelcome() {
    std::cout << "MiniQL v0.1.0 - Mini SQL Database Engine\n";
    std::cout << "Type \".help\" for usage hints.\n";
    std::cout << "Type \".exit\" to quit.\n\n";
}

void REPL::printHelp() {
    std::cout << "\nMiniQL Meta Commands:\n";
    std::cout << "  .help              Show this help message\n";
    std::cout << "  .exit              Exit the program\n";
    std::cout << "  .quit              Exit the program\n";
    std::cout << "  .tables            List all tables\n";
    std::cout << "  .schema <table>    Show schema of a table\n";
    std::cout << "\nSQL Commands (in development):\n";
    std::cout << "  CREATE TABLE name (col1 INT, col2 TEXT);\n";
    std::cout << "  INSERT INTO name VALUES (1, 'text');\n";
    std::cout << "  SELECT * FROM name;\n";
    std::cout << "  SELECT col FROM name WHERE col = value;\n";
    std::cout << "  DELETE FROM name WHERE col = value;\n";
    std::cout << "\nNote: SQL commands must end with semicolon (;)\n\n";
}

} // namespace miniql

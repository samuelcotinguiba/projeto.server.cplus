#ifndef MINIQL_REPL_H
#define MINIQL_REPL_H

#include <string>

namespace miniql {

class REPL {
public:
    REPL();
    ~REPL();

    // Inicia o loop interativo
    void run();

private:
    // Processa comandos meta (começam com .)
    bool processMetaCommand(const std::string& command);
    
    // Processa comandos SQL
    void processSQLCommand(const std::string& sql);
    
    // Exibe prompt e lê linha
    std::string readLine(const std::string& prompt);
    
    // Helpers
    void printWelcome();
    void printHelp();
    
    bool running_;
};

} // namespace miniql

#endif // MINIQL_REPL_H

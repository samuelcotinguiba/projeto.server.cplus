#include "lexer/scanner.h"

namespace miniql {
namespace lexer {

// ============================================================================
// FUNÇÃO: scanString()
// ============================================================================
// Reconhece literais de string delimitados por aspas simples ou duplas:
// - 'texto entre aspas simples'
// - "texto entre aspas duplas"
// 
// Padrão (Pattern): ['"'] [^'"]* ['"']
// 
// Algoritmo:
// 1. Já consumimos a aspa de abertura (quote)
// 2. Consome todos os caracteres até encontrar a aspa de fechamento
// 3. Suporta escape de aspas (ex: 'it\'s')
// 4. Detecta strings não terminadas (erro)

void Scanner::scanString(char quote) {
    std::string value;
    
    while (!isAtEnd() && peek() != quote) {
        // Suporte para strings multi-linha
        if (peek() == '\n') {
            line_++;
            column_ = 0;
        }
        
        // Suporte para escape sequences
        if (peek() == '\\' && peekNext() == quote) {
            advance();
            value += advance();
        } else {
            value += advance();
        }
    }
    
    // Verifica se a string foi fechada
    if (isAtEnd()) {
        addError("Unterminated string literal");
        return;
    }
    
    advance();
    tokens_.push_back(Token(TokenType::STRING, value, line_, column_));
}

} // namespace lexer
} // namespace miniql

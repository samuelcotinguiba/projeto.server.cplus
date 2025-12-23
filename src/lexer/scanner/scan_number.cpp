#include "lexer/scanner.h"

namespace miniql {
namespace lexer {

// ============================================================================
// FUNÇÃO: scanNumber()
// ============================================================================
// Reconhece literais numéricos no formato:
// - Inteiros: 123, 456
// - Decimais: 45.67, 3.14159
// 
// Padrão (Pattern): [0-9]+ ( '.' [0-9]+ )?
// 
// Algoritmo:
// 1. Consome todos os dígitos antes do ponto decimal
// 2. Se encontrar '.', verifica se há dígitos depois
// 3. Consome os dígitos da parte decimal
// 4. Converte o lexeme para double e armazena no token

void Scanner::scanNumber() {
    // Consome todos os dígitos inteiros
    while (isDigit(peek())) {
        advance();
    }
    
    // Verifica se há parte decimal
    if (peek() == '.' && isDigit(peekNext())) {
        // Consome o '.'
        advance();
        
        // Consome os dígitos decimais
        while (isDigit(peek())) {
            advance();
        }
    }
    
    // Extrai o lexeme completo
    std::string lexeme = source_.substr(start_, current_ - start_);
    
    // Cria o token NUMBER
    Token token(TokenType::NUMBER, lexeme, line_, column_);
    
    // Converte o valor para número
    try {
        token.number_value = std::stod(lexeme);
    } catch (const std::exception&) {
        addError("Invalid number format: " + lexeme);
        token.number_value = 0.0;
    }
    
    tokens_.push_back(token);
}

} // namespace lexer
} // namespace miniql

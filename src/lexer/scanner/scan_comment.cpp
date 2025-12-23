#include "lexer/scanner.h"

namespace miniql {
namespace lexer {

// ============================================================================
// FUNÇÃO: scanComment()
// ============================================================================
// Reconhece comentários em SQL:
// - Comentários de linha: -- até o fim da linha
// - Comentários de bloco: /* ... */ (multi-linha)
// 
// Padrões:
// - Linha:  '--' [^\n]*
// - Bloco:  '/*' .* '*/'
// 
// Algoritmo para linha única:
// 1. Já consumimos '--'
// 2. Consome até '\n' ou EOF
// 3. Não adiciona token (comentários são ignorados)
// 
// Algoritmo para bloco:
// 1. Já consumimos '/*'
// 2. Consome até encontrar '*/'
// 3. Suporta multi-linha
// 4. Detecta comentários não fechados (erro)

void Scanner::scanComment() {
    if (peek() == '-' && peekNext() == '-') {
        advance();
        advance();
        
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
        return;
    }
    
    if (peek() == '/' && peekNext() == '*') {
        advance();
        advance();
        
        while (!isAtEnd()) {
            if (peek() == '*' && peekNext() == '/') {
                advance();
                advance();
                return;
            }
            
            if (peek() == '\n') {
                line_++;
                column_ = 0;
            }
            
            advance();
        }
        
        // Se chegou aqui, comentário não foi fechado
        addError("Unterminated block comment");
    }
}

} // namespace lexer
} // namespace miniql

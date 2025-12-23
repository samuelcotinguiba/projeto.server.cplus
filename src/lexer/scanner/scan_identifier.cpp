#include "lexer/scanner.h"
#include <algorithm>

namespace miniql {
namespace lexer {

// ============================================================================
// FUNÇÃO: scanIdentifier()
// ============================================================================
// Reconhece identificadores e palavras-chave:
// - Identificadores: nome de tabelas, colunas, variáveis
// - Palavras-chave: SELECT, INSERT, CREATE, etc.
// 
// Padrão (Pattern): [a-zA-Z_] [a-zA-Z0-9_]*
// 
// Algoritmo:
// 1. Primeiro caractere deve ser letra ou underscore
// 2. Caracteres seguintes podem ser letras, dígitos ou underscore
// 3. Após reconhecer, verifica na tabela de keywords
// 4. Se for keyword, retorna o token específico
// 5. Caso contrário, retorna IDENTIFIER
// 
// SQL é case-insensitive para keywords, então convertemos para uppercase

void Scanner::scanIdentifier() {
    // Consome todos os caracteres alfanuméricos e underscore
    while (isAlphaNumeric(peek())) {
        advance();
    }
    
    // Extrai o lexeme
    std::string lexeme = source_.substr(start_, current_ - start_);
    
    // Converte para uppercase para comparação case-insensitive
    std::string upper_lexeme = lexeme;
    std::transform(upper_lexeme.begin(), upper_lexeme.end(), 
                   upper_lexeme.begin(), ::toupper);
    
    // Verifica se é uma palavra-chave
    TokenType type = TokenType::IDENTIFIER;
    auto it = keywords_.find(upper_lexeme);
    if (it != keywords_.end()) {
        type = it->second;
    }
    
    // Cria o token (mantém o lexeme original)
    addToken(type, lexeme);
}

} // namespace lexer
} // namespace miniql

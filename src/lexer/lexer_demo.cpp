#include "lexer/scanner.h"
#include <iostream>
#include <iomanip>

using namespace miniql::lexer;

void printTokens(const std::vector<Token>& tokens) {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    ANÁLISE LÉXICA - TOKENS                    ║\n";
    std::cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    std::cout << std::left << std::setw(20) << "║ TOKEN TYPE" 
              << std::setw(25) << "LEXEME" 
              << std::setw(10) << "LINE"
              << std::setw(10) << "COLUMN" << "║\n";
    std::cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::END_OF_FILE) break;
        
        std::cout << "║ " << std::left << std::setw(18) << token.typeToString()
                  << std::setw(25) << ("'" + token.lexeme + "'")
                  << std::setw(10) << token.line
                  << std::setw(10) << token.column << "║\n";
    }
    
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n\n";
}

void testLexer(const std::string& description, const std::string& sql) {
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "📋 TESTE: " << description << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "SQL: " << sql << "\n";
    
    Scanner scanner(sql);
    auto tokens = scanner.scanTokens();
    
    if (scanner.hasErrors()) {
        std::cout << "\n❌ ERROS LÉXICOS ENCONTRADOS:\n";
        for (const auto& error : scanner.getErrors()) {
            std::cout << "   • " << error << "\n";
        }
    } else {
        std::cout << "\n✅ Tokenização bem-sucedida!\n";
    }
    
    printTokens(tokens);
}

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                DEMONSTRAÇÃO DO ANALISADOR LÉXICO               ║\n";
    std::cout << "║                        MiniQL Database                         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    
    // Teste 1: SELECT simples
    testLexer(
        "SELECT Simples",
        "SELECT * FROM users;"
    );
    
    // Teste 2: SELECT com WHERE
    testLexer(
        "SELECT com WHERE",
        "SELECT name FROM users WHERE id = 1;"
    );
    
    // Teste 3: CREATE TABLE
    testLexer(
        "CREATE TABLE",
        "CREATE TABLE users (id INT, name TEXT);"
    );
    
    // Teste 4: INSERT
    testLexer(
        "INSERT INTO",
        "INSERT INTO users VALUES (1, 'Ana Silva');"
    );
    
    // Teste 5: Operadores relacionais
    testLexer(
        "Operadores Relacionais",
        "SELECT * FROM products WHERE price >= 100 AND stock < 50;"
    );
    
    // Teste 6: Comentários
    testLexer(
        "Comentários",
        "-- Isto é um comentário\nSELECT * FROM users; /* comentário de bloco */"
    );
    
    // Teste 7: Números decimais
    testLexer(
        "Números Decimais",
        "SELECT price FROM products WHERE price = 99.99;"
    );
    
    // Teste 8: Erro léxico
    testLexer(
        "Erro Léxico (caractere inválido)",
        "SELECT @ FROM users;"
    );
    
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    DEMONSTRAÇÃO CONCLUÍDA                      ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    
    return 0;
}

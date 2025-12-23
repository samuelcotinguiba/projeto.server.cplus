#ifndef MINIQL_LEXER_SCANNER_H
#define MINIQL_LEXER_SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>

namespace miniql {
namespace lexer {

enum class TokenType {
    // Palavras-chave SQL
    SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, TABLE, FROM, WHERE,
    INTO, VALUES, AND, OR, NOT, AS, JOIN, LEFT, RIGHT, INNER, OUTER,
    ON, ORDER, BY, GROUP, HAVING, LIMIT, OFFSET, ASC, DESC,
    PRIMARY, KEY, FOREIGN, REFERENCES, UNIQUE, INDEX, NULL_KW,
    INT, TEXT, REAL, BLOB, DATE, TIMESTAMP,
    
    // Identificadores e Literais
    IDENTIFIER,      // nomes de tabelas, colunas, etc
    NUMBER,          // 123, 45.67
    STRING,          // 'texto', "texto"
    
    // Operadores relacionais
    EQUAL,           // =
    NOT_EQUAL,       // != ou <>
    LESS_THAN,       // <
    LESS_EQUAL,      // <=
    GREATER_THAN,    // >
    GREATER_EQUAL,   // >=
    
    // Operadores aritméticos
    PLUS,            // +
    MINUS,           // -
    STAR,            // *
    SLASH,           // /
    PERCENT,         // %
    
    // Delimitadores
    LPAREN,          // (
    RPAREN,          // )
    COMMA,           // ,
    SEMICOLON,       // ;
    DOT,             // .
    
    // Especiais
    WHITESPACE,      // espaços, tabs (geralmente ignorados)
    COMMENT,         // -- comentário ou /* comentário */
    END_OF_FILE,     // fim do input
    UNKNOWN          // token não reconhecido (erro léxico)
};


struct Token {
    TokenType type;          // tipo do token
    std::string lexeme;      // texto original do token
    int line;                // linha onde o token aparece
    int column;              // coluna inicial do token
    
    // Para tokens literais, armazena o valor convertido
    union {
        double number_value;
        // Para strings, o valor já está no lexeme
    };
    
    Token() : type(TokenType::UNKNOWN), line(0), column(0), number_value(0.0) {}
    Token(TokenType t, const std::string& lex, int ln = 0, int col = 0)
        : type(t), lexeme(lex), line(ln), column(col), number_value(0.0) {}
    
    // Converte o tipo do token para string (debug/erro)
    std::string typeToString() const;
};

// SCANNER / LEXER:
// Responsável pela análise léxica do código SQL
// Transforma o texto de entrada em uma sequência de tokens

class Scanner {
public:
    explicit Scanner(const std::string& source);
    
    // Tokeniza todo o input de uma vez
    std::vector<Token> scanTokens();
    
    // Retorna a lista de erros léxicos encontrados
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    // Verifica se houve erros durante o scanning
    bool hasErrors() const { return !errors_.empty(); }
    
private:
    std::string source_;                    // código fonte
    std::vector<Token> tokens_;             // tokens gerados
    std::vector<std::string> errors_;       // erros léxicos
    size_t start_;                          // início do lexeme atual
    size_t current_;                        // caractere atual sendo analisado
    int line_;                              // linha atual
    int column_;                            // coluna atual
    
    static std::unordered_map<std::string, TokenType> keywords_;
    
    // Funções auxiliares de navegação
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    
    // Funções de identificação de caracteres
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    
    // Funções de scanning principal
    void scanToken();
    void addToken(TokenType type);
    void addToken(TokenType type, const std::string& lexeme);
    
    // Funções de reconhecimento de padrões (definidas em scanner/)
    void scanNumber();           // números: 123, 45.67
    void scanString(char quote); // strings: 'texto' ou "texto"
    void scanIdentifier();       // identificadores e keywords
    void scanComment();          // comentários: -- ou /* */
    
    // Gerenciamento de erros
    void addError(const std::string& message);
};

// FUNÇÕES UTILITÁRIAS
// Converte TokenType para string (para debug e mensagens de erro)
std::string tokenTypeToString(TokenType type);

// Verifica se um token é uma palavra-chave
bool isKeyword(TokenType type);

// Verifica se um token é um operador
bool isOperator(TokenType type);

// Verifica se um token é um delimitador
bool isDelimiter(TokenType type);

} // namespace lexer
} // namespace miniql

#endif // MINIQL_LEXER_SCANNER_H

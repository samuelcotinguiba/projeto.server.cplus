#include "lexer/scanner.h"
#include <sstream>

namespace miniql {
namespace lexer {

std::unordered_map<std::string, TokenType> Scanner::keywords_ = {
    // Comandos SQL principais
    {"SELECT", TokenType::SELECT},
    {"INSERT", TokenType::INSERT},
    {"UPDATE", TokenType::UPDATE},
    {"DELETE", TokenType::DELETE},
    {"CREATE", TokenType::CREATE},
    {"DROP", TokenType::DROP},
    {"TABLE", TokenType::TABLE},
    
    // Cláusulas
    {"FROM", TokenType::FROM},
    {"WHERE", TokenType::WHERE},
    {"INTO", TokenType::INTO},
    {"VALUES", TokenType::VALUES},
    
    // Operadores lógicos
    {"AND", TokenType::AND},
    {"OR", TokenType::OR},
    {"NOT", TokenType::NOT},
    
    // JOIN
    {"JOIN", TokenType::JOIN},
    {"LEFT", TokenType::LEFT},
    {"RIGHT", TokenType::RIGHT},
    {"INNER", TokenType::INNER},
    {"OUTER", TokenType::OUTER},
    {"ON", TokenType::ON},
    
    // Ordenação e agrupamento
    {"ORDER", TokenType::ORDER},
    {"BY", TokenType::BY},
    {"GROUP", TokenType::GROUP},
    {"HAVING", TokenType::HAVING},
    {"ASC", TokenType::ASC},
    {"DESC", TokenType::DESC},
    
    // Limites
    {"LIMIT", TokenType::LIMIT},
    {"OFFSET", TokenType::OFFSET},
    
    // Constraints
    {"PRIMARY", TokenType::PRIMARY},
    {"KEY", TokenType::KEY},
    {"FOREIGN", TokenType::FOREIGN},
    {"REFERENCES", TokenType::REFERENCES},
    {"UNIQUE", TokenType::UNIQUE},
    {"INDEX", TokenType::INDEX},
    {"NULL", TokenType::NULL_KW},
    
    // Tipos de dados
    {"INT", TokenType::INT},
    {"TEXT", TokenType::TEXT},
    {"REAL", TokenType::REAL},
    {"BLOB", TokenType::BLOB},
    {"DATE", TokenType::DATE},
    {"TIMESTAMP", TokenType::TIMESTAMP},
    
    // Outros
    {"AS", TokenType::AS},
};

// CONSTRUTOR E MÉTODOS PÚBLICOS
Scanner::Scanner(const std::string& source)
    : source_(source), start_(0), current_(0), line_(1), column_(0) {}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        // Início de um novo lexeme
        start_ = current_;
        scanToken();
    }
    
    // Adiciona token de fim de arquivo
    tokens_.push_back(Token(TokenType::END_OF_FILE, "", line_, column_));
    return tokens_;
}

// FUNÇÃO: scanToken(), Identifica e processa um único token
void Scanner::scanToken() {
    char c = advance();
    
    switch (c) {
        // Delimitadores simples
        case '(': addToken(TokenType::LPAREN); break;
        case ')': addToken(TokenType::RPAREN); break;
        case ',': addToken(TokenType::COMMA); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '.': addToken(TokenType::DOT); break;
        case '+': addToken(TokenType::PLUS); break;
        case '%': addToken(TokenType::PERCENT); break;
        case '*': addToken(TokenType::STAR); break;
        case '=': addToken(TokenType::EQUAL); break;
        
        case '-':
            if (match('-')) scanComment();
            else addToken(TokenType::MINUS);
            break;
        
        case '/':
            if (match('*')) scanComment();
            else addToken(TokenType::SLASH);
            break;
        
        case '<':
            if (match('=')) addToken(TokenType::LESS_EQUAL);
            else if (match('>')) addToken(TokenType::NOT_EQUAL);
            else addToken(TokenType::LESS_THAN);
            break;
        
        case '>':
            if (match('=')) addToken(TokenType::GREATER_EQUAL);
            else addToken(TokenType::GREATER_THAN);
            break;
        
        case '!':
            if (match('=')) addToken(TokenType::NOT_EQUAL);
            else addError("Unexpected character '!'");
            break;
        
        case ' ': case '\r': case '\t': break;
        case '\n': line_++; column_ = 0; break;
        case '\'': case '"': scanString(c); break;
        
        default:
            if (isDigit(c)) scanNumber();
            else if (isAlpha(c)) scanIdentifier();
            else addError(std::string("Unexpected character: '") + c + "'");
            break;
    }
}

// ============================================================================
// FUNÇÕES AUXILIARES DE NAVEGAÇÃO
// ============================================================================

bool Scanner::isAtEnd() const {
    return current_ >= source_.length();
}

char Scanner::advance() {
    column_++;
    return source_[current_++];
}

char Scanner::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Scanner::peekNext() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

bool Scanner::match(char expected) {
    if (isAtEnd() || source_[current_] != expected) return false;
    current_++;
    column_++;
    return true;
}

// ============================================================================
// FUNÇÕES DE CLASSIFICAÇÃO DE CARACTERES
// ============================================================================

bool Scanner::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Scanner::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

// ============================================================================
// FUNÇÕES DE ADIÇÃO DE TOKENS
// ============================================================================

void Scanner::addToken(TokenType type) {
    addToken(type, source_.substr(start_, current_ - start_));
}

void Scanner::addToken(TokenType type, const std::string& lexeme) {
    tokens_.push_back(Token(type, lexeme, line_, column_));
}

// ============================================================================
// GERENCIAMENTO DE ERROS
// ============================================================================

void Scanner::addError(const std::string& message) {
    std::stringstream ss;
    ss << "[Line " << line_ << ", Col " << column_ << "] " << message;
    errors_.push_back(ss.str());
}

std::string Token::typeToString() const {
    return tokenTypeToString(type);
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        // Comandos DML
        case TokenType::SELECT: return "SELECT";
        case TokenType::INSERT: return "INSERT";
        case TokenType::UPDATE: return "UPDATE";
        case TokenType::DELETE: return "DELETE";
        
        // Comandos DDL
        case TokenType::CREATE: return "CREATE";
        case TokenType::DROP: return "DROP";
        case TokenType::TABLE: return "TABLE";
        
        // Cláusulas
        case TokenType::FROM: return "FROM";
        case TokenType::WHERE: return "WHERE";
        case TokenType::INTO: return "INTO";
        case TokenType::VALUES: return "VALUES";
        
        // Operadores Lógicos
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        
        // JOINs
        case TokenType::AS: return "AS";
        case TokenType::JOIN: return "JOIN";
        case TokenType::LEFT: return "LEFT";
        case TokenType::RIGHT: return "RIGHT";
        case TokenType::INNER: return "INNER";
        case TokenType::OUTER: return "OUTER";
        case TokenType::ON: return "ON";
        
        // Ordenação e Agrupamento
        case TokenType::ORDER: return "ORDER";
        case TokenType::BY: return "BY";
        case TokenType::GROUP: return "GROUP";
        case TokenType::HAVING: return "HAVING";
        case TokenType::LIMIT: return "LIMIT";
        case TokenType::OFFSET: return "OFFSET";
        case TokenType::ASC: return "ASC";
        case TokenType::DESC: return "DESC";
        
        // Constraints
        case TokenType::PRIMARY: return "PRIMARY";
        case TokenType::KEY: return "KEY";
        case TokenType::FOREIGN: return "FOREIGN";
        case TokenType::REFERENCES: return "REFERENCES";
        case TokenType::UNIQUE: return "UNIQUE";
        case TokenType::INDEX: return "INDEX";
        case TokenType::NULL_KW: return "NULL";
        
        // Tipos de dados
        case TokenType::INT: return "INT";
        case TokenType::TEXT: return "TEXT";
        case TokenType::REAL: return "REAL";
        case TokenType::BLOB: return "BLOB";
        case TokenType::DATE: return "DATE";
        case TokenType::TIMESTAMP: return "TIMESTAMP";
        
        // Literais
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        
        // Operadores Relacionais
        case TokenType::EQUAL: return "=";
        case TokenType::NOT_EQUAL: return "!=";
        case TokenType::LESS_THAN: return "<";
        case TokenType::LESS_EQUAL: return "<=";
        case TokenType::GREATER_THAN: return ">";
        case TokenType::GREATER_EQUAL: return ">=";
        
        // Operadores Aritméticos
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::PERCENT: return "%";
        
        // Delimitadores
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::COMMA: return ",";
        case TokenType::SEMICOLON: return ";";
        case TokenType::DOT: return ".";
        
        // Especiais
        case TokenType::WHITESPACE: return "WHITESPACE";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::END_OF_FILE: return "EOF";
        
        default: return "UNKNOWN";
    }
}

} // namespace lexer
} // namespace miniql

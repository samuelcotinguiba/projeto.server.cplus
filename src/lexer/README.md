# 📝 Analisador Léxico (Lexer) - MiniQL

## ✅ Implementação Completa

O analisador léxico foi implementado com sucesso seguindo as regras padrão de análise léxica para linguagens de programação.

### 📚 Referência

Implementação baseada em: **GeeksforGeeks - Introduction of Lexical Analysis**
- Conceitos de Tokens, Lexemas e Padrões
- Reconhecimento de diferentes tipos de tokens
- Tratamento de erros léxicos

---

## 🏗️ Estrutura de Arquivos

```
include/lexer/
  └── scanner.h              # Definições de Token, TokenType e Scanner

src/lexer/
  ├── scanner.cpp            # Implementação principal do scanner
  ├── lexer_demo.cpp         # Programa de demonstração
  └── scanner/               # Funções auxiliares organizadas
      ├── scan_number.cpp    # Reconhecimento de números
      ├── scan_string.cpp    # Reconhecimento de strings
      ├── scan_identifier.cpp # Reconhecimento de identificadores/keywords
      └── scan_comment.cpp   # Reconhecimento de comentários
```

---

## 🎯 Funcionalidades Implementadas

### 1. **Reconhecimento de Tokens**

#### Keywords SQL (50+ palavras-chave)
```sql
SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, TABLE
FROM, WHERE, INTO, VALUES, AND, OR, NOT
JOIN, LEFT, RIGHT, INNER, OUTER, ON
ORDER, BY, GROUP, HAVING, LIMIT, OFFSET, ASC, DESC
PRIMARY, KEY, FOREIGN, REFERENCES, UNIQUE, INDEX, NULL
```

#### Tipos de Dados
```sql
INT, TEXT, REAL, BLOB, DATE, TIMESTAMP
```

#### Identificadores
- Nomes de tabelas e colunas
- Padrão: `[a-zA-Z_][a-zA-Z0-9_]*`
- Case-insensitive para keywords

#### Literais
- **Números Inteiros:** `123`, `456`
- **Números Decimais:** `45.67`, `3.14159`
- **Strings:** `'texto'`, `"texto"`
  - Suporte para aspas simples e duplas
  - Escape de caracteres: `'it\'s'`
  - Multi-linha suportada

#### Operadores

**Relacionais:**
```
=  (igual)
!= (diferente)
<> (diferente - alternativo)
<  (menor)
<= (menor ou igual)
>  (maior)
>= (maior ou igual)
```

**Aritméticos:**
```
+ (adição)
- (subtração)
* (multiplicação)
/ (divisão)
% (módulo)
```

#### Delimitadores
```
(  )  ,  ;  .
```

#### Comentários
- **Linha única:** `-- comentário`
- **Bloco:** `/* comentário */`
- Comentários são ignorados (não geram tokens)

---

## 🔧 Como Usar

### Compilação

```bash
# Build completo do projeto
make

# Build de demonstração do Lexer
make lexer-demo

# Ou manualmente:
g++ -std=c++17 -I./include src/lexer/lexer_demo.cpp \
    src/lexer/scanner.cpp src/lexer/scanner/*.cpp -o lexer_demo
```

### Uso em Código

```cpp
#include "lexer/scanner.h"

using namespace miniql::lexer;

// Criar scanner com código SQL
Scanner scanner("SELECT * FROM users WHERE id = 1;");

// Tokenizar
std::vector<Token> tokens = scanner.scanTokens();

// Verificar erros
if (scanner.hasErrors()) {
    for (const auto& error : scanner.getErrors()) {
        std::cerr << error << "\n";
    }
}

// Processar tokens
for (const auto& token : tokens) {
    std::cout << token.typeToString() << ": " << token.lexeme << "\n";
}
```

### Demonstração

```bash
# Compilar e executar demo do lexer
make run-lexer-demo

# Ou executar diretamente
./lexer_demo
```

---

## 📊 Exemplos de Tokenização

### SELECT Simples
```sql
SELECT * FROM users;
```
**Tokens:**
```
SELECT     'SELECT'
*          '*'
FROM       'FROM'
IDENTIFIER 'users'
;          ';'
```

### SELECT com WHERE
```sql
SELECT name FROM users WHERE id = 1;
```
**Tokens:**
```
SELECT     'SELECT'
IDENTIFIER 'name'
FROM       'FROM'
IDENTIFIER 'users'
WHERE      'WHERE'
IDENTIFIER 'id'
=          '='
NUMBER     '1'
;          ';'
```

### CREATE TABLE
```sql
CREATE TABLE users (id INT, name TEXT);
```
**Tokens:**
```
CREATE     'CREATE'
TABLE      'TABLE'
IDENTIFIER 'users'
(          '('
IDENTIFIER 'id'
INT        'INT'
,          ','
IDENTIFIER 'name'
TEXT       'TEXT'
)          ')'
;          ';'
```

### INSERT
```sql
INSERT INTO users VALUES (1, 'Ana Silva');
```
**Tokens:**
```
INSERT     'INSERT'
INTO       'INTO'
IDENTIFIER 'users'
VALUES     'VALUES'
(          '('
NUMBER     '1'
,          ','
STRING     'Ana Silva'
)          ')'
;          ';'
```

---

## 🔍 Detecção de Erros

O Lexer detecta e reporta erros léxicos com informações precisas:

```sql
SELECT @ FROM users;
```

**Erro:**
```
[Line 1, Col 8] Lexical Error: Unexpected character: '@'
```

```sql
INSERT INTO users VALUES (1, 'string não fechada;
```

**Erro:**
```
[Line 1, Col 45] Lexical Error: Unterminated string literal
```

---

## 🎓 Conceitos de Análise Léxica

### Token
- **Unidade léxica** básica da linguagem
- Exemplo: `SELECT`, `123`, `users`, `=`

### Lexema
- **Sequência de caracteres** que forma um token
- Exemplo: a palavra `SELECT` é o lexema do token SELECT

### Padrão (Pattern)
- **Regra** que descreve o conjunto de lexemas de um token
- Exemplo: `[0-9]+` descreve números inteiros

### Processo de Scanning
1. **Leitura** do código fonte caractere por caractere
2. **Agrupamento** de caracteres em lexemas
3. **Identificação** do tipo de token
4. **Remoção** de espaços em branco e comentários
5. **Geração** da lista de tokens

---

## ✨ Características Técnicas

### Case-Insensitive Keywords
```sql
SELECT * FROM users;  -- válido
select * from users;  -- também válido
SeLeCt * FrOm users;  -- também válido
```

### Múltiplos Formatos de String
```sql
'string com aspas simples'
"string com aspas duplas"
'string com \'escape\''
```

### Comentários Suportados
```sql
-- Comentário de linha única
SELECT * FROM users;  -- comentário no final

/* Comentário
   de múltiplas
   linhas */
```

### Tratamento de Whitespace
- Espaços, tabs e newlines são automaticamente ignorados
- Mantém rastreamento de linha e coluna para erros precisos

---

## 🧪 Testes

Execute o programa de demonstração para ver todos os testes:

```bash
./lexer_demo
```

**Testes incluídos:**
1. ✅ SELECT simples
2. ✅ SELECT com WHERE
3. ✅ CREATE TABLE
4. ✅ INSERT INTO
5. ✅ Operadores relacionais
6. ✅ Comentários
7. ✅ Números decimais
8. ✅ Detecção de erros

---

## 🚀 Próximos Passos

Agora que o Lexer está implementado, os próximos componentes a serem desenvolvidos são:

1. **Parser (Analisador Sintático)**
   - Análise sintática dos tokens
   - Validação da gramática SQL
   - Geração da AST (Abstract Syntax Tree)

2. **AST (Abstract Syntax Tree)**
   - Representação em árvore dos comandos SQL
   - Nodes para cada tipo de statement

3. **Semantic Analyzer**
   - Validação semântica
   - Verificação de tipos
   - Análise de escopo

---

## 📖 Documentação Relacionada

- [Architecture.md](../../doc/architecture.md) - Arquitetura completa do sistema
- [Development Guide](../../doc/development-guide.md) - Guia de desenvolvimento
- [README Principal](../../README.md) - Visão geral do projeto

---

**Status:** ✅ Implementado e testado  
**Fase:** FASE 3 concluída  
**Data:** Dezembro 2023

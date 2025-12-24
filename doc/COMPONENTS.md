# 🧩 Componentes do MiniQL

> Documentação detalhada de cada componente implementado

---

## 📖 Índice

- [Shell Interativo (REPL)](#-shell-interativo-repl)
- [Analisador Léxico (Lexer)](#-analisador-léxico-lexer)
- [Analisador Sintático (Parser)](#-analisador-sintático-parser)
- [Árvore Sintática Abstrata (AST)](#-árvore-sintática-abstrata-ast)
- [Catálogo de Schemas](#-catálogo-de-schemas)
- [Motor de Armazenamento](#-motor-de-armazenamento)
- [Executor de Queries](#-executor-de-queries)

---

## 🖥️ Shell Interativo (REPL)

**Status:** ✅ Implementado  
**Localização:** `src/shell/repl.cpp`, `include/shell/repl.h`

---

## 📝 Analisador Léxico (Lexer)

**Status:** ✅ Implementado - Tokenização completa  
**Localização:** `src/lexer/`, `include/lexer/scanner.h`

### Descrição

Analisador léxico que converte código SQL em uma sequência de tokens para processamento pelo parser.

### Funcionalidades

- ✅ Tokenização de 50+ keywords SQL (SELECT, INSERT, CREATE, etc.)
- ✅ Reconhecimento de identificadores (nomes de tabelas/colunas)
- ✅ Reconhecimento de literais (números: `123`, `45.67`, strings: `'texto'`)
- ✅ Operadores relacionais (`=`, `!=`, `<`, `>`, `<=`, `>=`)
- ✅ Operadores aritméticos (`+`, `-`, `*`, `/`, `%`)
- ✅ Delimitadores (`(`, `)`, `,`, `;`, `.`)
- ✅ Comentários de linha (`--`) e bloco (`/* */`)
- ✅ Detecção de erros léxicos com linha/coluna
- ✅ Case-insensitive para keywords SQL

### Estrutura Modular

```
src/lexer/
├── scanner.cpp            # Implementação principal (scanTokens, scanToken)
├── lexer_demo.cpp         # Programa de demonstração
└── scanner/               # Funções especializadas
    ├── scan_number.cpp    # Números inteiros e decimais
    ├── scan_string.cpp    # Strings com aspas simples/duplas
    ├── scan_identifier.cpp # Identificadores e keywords
    └── scan_comment.cpp   # Comentários de linha e bloco
```

### Uso

```bash
# Testar tokenização
make lexer-demo
./lexer_demo
```

**Exemplo:**
```sql
Entrada: SELECT * FROM users WHERE id = 1;

Tokens gerados:
[1:1]  SELECT      'SELECT'
[1:8]  STAR        '*'
[1:10] FROM        'FROM'
[1:15] IDENTIFIER  'users'
[1:21] WHERE       'WHERE'
[1:27] IDENTIFIER  'id'
[1:30] EQUAL       '='
[1:32] NUMBER      '1'
[1:33] SEMICOLON   ';'
[1:34] END_OF_FILE ''
```

### Código de Exemplo

```cpp
#include "lexer/scanner.h"

using namespace miniql::lexer;

// Criar scanner
Scanner scanner("SELECT * FROM users;");

// Tokenizar
std::vector<Token> tokens = scanner.scanTokens();

// Verificar erros
if (scanner.hasErrors()) {
    for (const auto& error : scanner.getErrors()) {
        std::cout << error << std::endl;
    }
}

// Processar tokens
for (const auto& token : tokens) {
    std::cout << token.typeToString() << " " 
              << token.lexeme << std::endl;
}
```

### Fluxo de Tokenização

```
1. Scanner recebe código SQL como string
2. scanTokens() itera sobre cada caractere
3. scanToken() identifica tipo de token:
   ├─ Whitespace → ignora
   ├─ Dígito → scanNumber()
   ├─ Letra/_ → scanIdentifier() ou keyword
   ├─ ' ou " → scanString()
   ├─ - → scanComment() ou operador
   ├─ / → scanComment() ou operador
   ├─ Operador → adiciona token
   └─ Delimitador → adiciona token
4. Retorna vetor de tokens
```

### Tratamento de Erros

- Caracteres inválidos detectados
- Strings não terminadas
- Comentários de bloco não fechados
- Erros incluem linha e coluna

**Exemplo:**
```
[Line 1, Col 8] Unexpected character: '@'
[Line 2, Col 15] Unterminated string literal
```

### Descrição

Interface interativa de linha de comando para o MiniQL, similar ao `sqlite3` ou `mysql` client.

### Funcionalidades

- ✅ Loop Read-Eval-Print interativo
- ✅ Buffer multi-linha (acumula até `;`)
- ✅ Meta comandos (`.help`, `.exit`, `.quit`)
- ✅ Prompt contextual (`miniql>` ou `     ->`)
- ✅ Tratamento de EOF (Ctrl+D)
- ✅ Mensagens de boas-vindas e ajuda

### Uso

```bash
./miniql
```

```sql
miniql> .help
Available commands:
  .help      Show this help message
  .exit      Exit the program
  .quit      Exit the program

miniql> SELECT * FROM users;
(em desenvolvimento)

miniql> .exit
Goodbye!
```

### Fluxo de Execução

```
1. printWelcome()
2. while (running_)
   ├─ Determinar prompt (miniql> ou     ->)
   ├─ readLine()
   ├─ Se vazio: continue
   ├─ Se começa com '.' e buffer vazio: processMetaCommand()
   ├─ Senão: adicionar ao buffer
   └─ Se termina com ';': processSQLCommand() e limpar buffer
3. Exit
```

### Código de Exemplo

```cpp
#include "shell/repl.h"

int main() {
    try {
        miniql::REPL repl;
        repl.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}
```

**Documentação completa:** [doc/shell-implementation.md](shell-implementation.md)

---

## 🔤 Analisador Léxico (Lexer)

**Status:** ✅ Implementado  
**Localização:** `src/lexer/`, `include/lexer/scanner.h`

### Descrição

Transforma código SQL (string) em uma sequência de tokens para processamento pelo Parser.

### Funcionalidades

- ✅ Reconhecimento de 50+ keywords SQL
- ✅ Identificadores (nomes de tabelas, colunas)
- ✅ Literais (números inteiros, decimais, strings)
- ✅ Operadores relacionais (=, <, >, !=, <=, >=)
- ✅ Operadores aritméticos (+, -, *, /, %)
- ✅ Delimitadores ((, ), ,, ;, .)
- ✅ Comentários (-- e /* */)
- ✅ Detecção de erros léxicos com linha e coluna
- ✅ Keywords case-insensitive

### Tokens Suportados

#### Keywords SQL
```
SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, TABLE
FROM, WHERE, INTO, VALUES, AND, OR, NOT, AS
JOIN, LEFT, RIGHT, INNER, OUTER, ON
ORDER, BY, GROUP, HAVING, LIMIT, OFFSET, ASC, DESC
PRIMARY, KEY, FOREIGN, REFERENCES, UNIQUE, INDEX, NULL
```

#### Tipos de Dados
```
INT, TEXT, REAL, BLOB, DATE, TIMESTAMP
```

#### Operadores
- **Relacionais:** `=`, `!=`, `<>`, `<`, `<=`, `>`, `>=`
- **Aritméticos:** `+`, `-`, `*`, `/`, `%`

### Uso

```cpp
#include "lexer/scanner.h"

using namespace miniql::lexer;

// Criar scanner
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
    std::cout << token.typeToString() << ": " 
              << token.lexeme << "\n";
}
```

### Exemplo de Tokenização

**Input:**
```sql
SELECT name FROM users WHERE id = 1;
```

**Output:**
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
EOF        ''
```

### Estrutura de Arquivos

```
src/lexer/
├── scanner.cpp            # Scanner principal
├── lexer_demo.cpp         # Programa de demonstração
└── scanner/               # Funções auxiliares
    ├── scan_number.cpp    # Reconhece números
    ├── scan_string.cpp    # Reconhece strings
    ├── scan_identifier.cpp # Reconhece identificadores/keywords
    └── scan_comment.cpp   # Reconhece comentários
```

### Demonstração

```bash
# Compilar e executar demo
make run-lexer-demo
```

**Documentação completa:** [../src/lexer/README.md](../src/lexer/README.md)

---

## 🌳 Analisador Sintático (Parser)

**Status:** ⏳ Planejado (FASE 4)  
**Localização:** `src/parser/` (futuro)

### Descrição

Valida a sintaxe dos tokens e constrói a Árvore Sintática Abstrata (AST).

### Funcionalidades Planejadas

- 🎯 Validação de sintaxe SQL
- 🎯 Construção da AST
- 🎯 Recursive descent parsing
- 🎯 Mensagens de erro claras
- 🎯 Suporte a expressões WHERE

### Gramática SQL Simplificada

```
statement       → createStmt | insertStmt | selectStmt | deleteStmt
createStmt      → "CREATE" "TABLE" identifier "(" columnList ")"
insertStmt      → "INSERT" "INTO" identifier "VALUES" "(" valueList ")"
selectStmt      → "SELECT" columnList "FROM" identifier [whereClause]
deleteStmt      → "DELETE" "FROM" identifier [whereClause]
whereClause     → "WHERE" expression
expression      → identifier operator literal
columnList      → "*" | identifier ("," identifier)*
valueList       → literal ("," literal)*
operator        → "=" | "<" | ">" | "<=" | ">=" | "!="
```

### Uso Planejado

```cpp
#include "parser/parser.h"

Parser parser(tokens);
std::unique_ptr<Statement> ast = parser.parse();

if (parser.hasErrors()) {
    for (const auto& error : parser.getErrors()) {
        std::cerr << error << "\n";
    }
}
```

---

## 🌲 Árvore Sintática Abstrata (AST)

**Status:** ⏳ Planejado (FASE 4)  
**Localização:** `include/ast/` (futuro)

### Descrição

Representação estruturada de comandos SQL em árvore, facilitando a execução.

### Hierarquia de Classes Planejada

```cpp
// Classe base
class Statement {
public:
    virtual ~Statement() = default;
    virtual StatementType getType() const = 0;
};

// CREATE TABLE
class CreateTableStmt : public Statement {
    std::string table_name;
    std::vector<ColumnDef> columns;
};

// INSERT INTO
class InsertStmt : public Statement {
    std::string table_name;
    std::vector<Value> values;
};

// SELECT
class SelectStmt : public Statement {
    std::vector<std::string> columns;  // "*" ou lista
    std::string table_name;
    std::unique_ptr<Expression> where_clause;
};

// DELETE
class DeleteStmt : public Statement {
    std::string table_name;
    std::unique_ptr<Expression> where_clause;
};

// Expressões (WHERE clause)
class Expression {
public:
    virtual ~Expression() = default;
    virtual Value evaluate(const Row& row) = 0;
};

class BinaryExpr : public Expression {
    Operator op;                          // =, <, >, etc
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
};

class ColumnExpr : public Expression {
    std::string column_name;
};

class LiteralExpr : public Expression {
    Value value;
};
```

---

## 📊 Catálogo de Schemas

**Status:** ⏳ Planejado (FASE 5)  
**Localização:** `src/catalog/` (futuro)

### Descrição

Gerencia metadados de tabelas (schemas, colunas, tipos).

### Funcionalidades Planejadas

- 🎯 Armazenar schemas de tabelas
- 🎯 Validar tipos de dados
- 🎯 Verificar existência de tabelas/colunas
- 🎯 Persistência de metadados

### Uso Planejado

```cpp
Catalog catalog;

// Registrar tabela
TableSchema schema;
schema.name = "users";
schema.columns = {
    {"id", DataType::INT},
    {"name", DataType::TEXT}
};
catalog.createTable(schema);

// Verificar existência
if (catalog.tableExists("users")) {
    auto schema = catalog.getTableSchema("users");
}
```

---

## 💾 Motor de Armazenamento

**Status:** ⏳ Planejado (FASE 6)  
**Localização:** `src/storage/` (futuro)

### Descrição

Responsável pela persistência de dados em disco (row-based storage).

### Funcionalidades Planejadas

- 🎯 Criar arquivos de tabela
- 🎯 Inserir registros (rows)
- 🎯 Ler registros
- 🎯 Deletar registros
- 🎯 Formato binário eficiente
- 🎯 Um arquivo por tabela

### Formato de Arquivo Planejado

```
users.db:
[Header]
  - Magic Number (4 bytes)
  - Version (4 bytes)
  - Row Count (8 bytes)
  - Schema Info

[Row 0]
  - ID (4 bytes)
  - Name Length (4 bytes)
  - Name Data (variable)

[Row 1]
  ...
```

---

## ⚡ Executor de Queries

**Status:** ⏳ Planejado (FASE 7-8)  
**Localização:** `src/executor/` (futuro)

### Descrição

Interpreta a AST e executa os comandos SQL, coordenando Catalog e Storage.

### Funcionalidades Planejadas

- 🎯 Executar CREATE TABLE
- 🎯 Executar INSERT
- 🎯 Executar SELECT (com/sem WHERE)
- 🎯 Executar DELETE (com/sem WHERE)
- 🎯 Avaliar expressões WHERE
- 🎯 Retornar ResultSet formatado

### Uso Planejado

```cpp
Executor executor(catalog, storage);

// Executar comando
ResultSet result = executor.execute(ast);

// Exibir resultados
result.print();
```

---

## 🗺️ Roadmap de Componentes

| Componente | Status | Fase |
|------------|--------|------|
| Shell (REPL) | ✅ Implementado | 2 |
| Lexer | ✅ Implementado | 3 |
| Parser | 🎯 Próximo | 4 |
| AST | 🎯 Próximo | 4 |
| Catalog | ⏳ Planejado | 5 |
| Storage | ⏳ Planejado | 6 |
| Executor | ⏳ Planejado | 7-8 |
| Indexação | ⏳ Futuro | 9+ |
| WAL | ⏳ Futuro | 10+ |

---

**Progresso Atual:** ~25% (3/12 fases)

**Documentação relacionada:**
- [architecture.md](architecture.md) — Visão geral da arquitetura
- [development-guide.md](development-guide.md) — Como desenvolver
- [../WORKFLOW.md](../WORKFLOW.md) — Roadmap completo

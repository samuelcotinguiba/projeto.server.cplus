# 🏗️ Arquitetura do MiniQL Database Engine

## Visão Geral

O MiniQL é um motor de banco de dados relacional construído seguindo uma **arquitetura em camadas**, inspirada em sistemas reais como SQLite e PostgreSQL. A separação clara de responsabilidades permite desenvolvimento modular, testabilidade e manutenibilidade.

---

## 📐 Diagrama de Arquitetura

```
┌─────────────────────────────────────────────────────────┐
│                    USER INTERFACE                        │
│                                                           │
│  ┌─────────────────────────────────────────────────┐   │
│  │           SQL Shell (REPL)                       │   │
│  │  • Comandos interativos                          │   │
│  │  • Meta comandos (.help, .exit)                  │   │
│  │  • Buffer de entrada multi-linha                 │   │
│  └─────────────────────┬───────────────────────────┘   │
└────────────────────────┼─────────────────────────────────┘
                         │
                         │ SQL String
                         ▼
┌─────────────────────────────────────────────────────────┐
│                 FRONTEND (Compilação)                    │
│                                                           │
│  ┌───────────────────────────────────────────────────┐  │
│  │                    LEXER                           │  │
│  │  • Tokenização do SQL                             │  │
│  │  • Keywords, identifiers, literals, operators     │  │
│  │  • Error handling léxico                          │  │
│  └──────────────────┬────────────────────────────────┘  │
│                     │ Token Stream                       │
│                     ▼                                     │
│  ┌───────────────────────────────────────────────────┐  │
│  │                   PARSER                           │  │
│  │  • Análise sintática                              │  │
│  │  • Recursive descent parsing                      │  │
│  │  • Validação de gramática SQL                     │  │
│  └──────────────────┬────────────────────────────────┘  │
│                     │ AST Nodes                          │
│                     ▼                                     │
│  ┌───────────────────────────────────────────────────┐  │
│  │              AST (Abstract Syntax Tree)            │  │
│  │  • CreateTableStmt                                │  │
│  │  • InsertStmt                                     │  │
│  │  • SelectStmt                                     │  │
│  │  • DeleteStmt                                     │  │
│  │  • Expression nodes (WHERE clauses)               │  │
│  └──────────────────┬────────────────────────────────┘  │
└────────────────────┼─────────────────────────────────────┘
                     │
                     │ AST Root
                     ▼
┌─────────────────────────────────────────────────────────┐
│                  BACKEND (Execução)                      │
│                                                           │
│  ┌───────────────────────────────────────────────────┐  │
│  │                  EXECUTOR                          │  │
│  │  • Interpreta AST                                 │  │
│  │  • Coordena operações de storage e catalog       │  │
│  │  • Gera ResultSet                                 │  │
│  └──────────┬──────────────────────────┬─────────────┘  │
│             │                          │                 │
│             ▼                          ▼                 │
│  ┌──────────────────┐      ┌──────────────────────┐    │
│  │     CATALOG      │      │   STORAGE ENGINE      │    │
│  │  • Schemas       │      │  • Table files        │    │
│  │  • Metadata      │      │  • Row storage        │    │
│  │  • Validation    │      │  • Binary I/O         │    │
│  └──────────────────┘      └──────────────────────┘    │
│             │                          │                 │
│             └──────────┬───────────────┘                 │
└────────────────────────┼─────────────────────────────────┘
                         │
                         ▼
                   ┌──────────┐
                   │   DISK   │
                   │  Files   │
                   └──────────┘
```

---

## 🧩 Componentes e Responsabilidades

### 1. **SQL Shell (REPL)** — `src/shell/repl.cpp`

**Responsabilidade:** Interface do usuário

**Funcionalidades:**
- Loop Read-Eval-Print interativo
- Leitura de comandos do `stdin`
- Buffer para comandos multi-linha (acumula até `;`)
- Processamento de meta comandos (`.help`, `.exit`, `.tables`, `.schema`)
- Prompt contextual (`miniql>` ou `     ->`)
- Tratamento de EOF (Ctrl+D/Ctrl+Z)

**Entradas:**
- Comandos do usuário via `stdin`

**Saídas:**
- SQL string para o Lexer
- Meta comando processado localmente
- Resultados formatados para `stdout`

**Estado Atual:** ✅ Implementado e funcional

---

### 2. **Lexer (Analisador Léxico)** — `src/lexer/lexer.cpp`

**Responsabilidade:** Tokenização do SQL

**Funcionalidades:**
- Transformar string SQL em stream de tokens
- Reconhecer keywords (`SELECT`, `INSERT`, `CREATE`, `TABLE`, `FROM`, `WHERE`, `VALUES`, `DELETE`, `INT`, `TEXT`)
- Identificar identificadores (nomes de tabelas/colunas)
- Extrair literais (números, strings)
- Reconhecer operadores (`=`, `<`, `>`, `<=`, `>=`, `!=`)
- Reconhecer símbolos (`(`, `)`, `,`, `;`, `*`)
- Reportar erros léxicos com linha/coluna

**Entradas:**
```cpp
std::string sql = "SELECT * FROM users WHERE id = 1;";
```

**Saídas:**
```cpp
std::vector<Token> tokens = {
    {TokenType::SELECT, "SELECT"},
    {TokenType::STAR, "*"},
    {TokenType::FROM, "FROM"},
    {TokenType::IDENTIFIER, "users"},
    {TokenType::WHERE, "WHERE"},
    {TokenType::IDENTIFIER, "id"},
    {TokenType::EQUAL, "="},
    {TokenType::NUMBER, "1"},
    {TokenType::SEMICOLON, ";"}
};
```

**Estado Atual:** ✅ **IMPLEMENTADO** (FASE 3)

**Estrutura de Arquivos:**
```
include/lexer/
  └── scanner.h              # Definições de Token, TokenType, Scanner

src/lexer/
  ├── scanner.cpp            # Implementação principal do scanner
  └── scanner/               # Funções auxiliares organizadas
      ├── scan_number.cpp    # Reconhecimento de números
      ├── scan_string.cpp    # Reconhecimento de strings
      ├── scan_identifier.cpp # Reconhecimento de identificadores/keywords
      └── scan_comment.cpp   # Reconhecimento de comentários
```

**Tokens Suportados:**
- **Keywords SQL:** SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, TABLE, FROM, WHERE, VALUES, AND, OR, NOT, etc.
- **Tipos de Dados:** INT, TEXT, REAL, BLOB, DATE, TIMESTAMP
- **Identificadores:** Nomes de tabelas, colunas (case-insensitive)
- **Literais:** Números (123, 45.67), Strings ('texto', "texto")
- **Operadores:** =, !=, <>, <, <=, >, >=, +, -, *, /, %
- **Delimitadores:** (, ), ,, ;, .
- **Comentários:** -- linha única, /* bloco */

**Características:**
- ✅ Tokenização completa de SQL
- ✅ Detecção de erros léxicos com linha/coluna
- ✅ Suporte a comentários (ignorados)
- ✅ Keywords case-insensitive
- ✅ Strings com aspas simples e duplas
- ✅ Números inteiros e decimais

**Baseado em:** Regras de análise léxica padrão (GeeksforGeeks - Introduction of Lexical Analysis)

---

### 3. **Parser (Analisador Sintático)** — `src/parser/parser.cpp`

**Responsabilidade:** Construção da AST

**Funcionalidades:**
- Validar sintaxe SQL
- Construir árvore sintática abstrata
- Implementar gramática SQL simplificada
- Recursive descent parsing
- Reportar erros sintáticos claros

**Técnica:** Recursive Descent Parser

**Gramática Simplificada:**
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

**Entradas:** Stream de tokens do Lexer

**Saídas:** AST root node

**Estado Atual:** ⏳ Planejado (FASE 4)

---

### 4. **AST (Abstract Syntax Tree)** — `include/ast/statements.h`

**Responsabilidade:** Representação estruturada de comandos SQL

**Hierarquia de Classes:**

```cpp
class Statement {
public:
    virtual ~Statement() = default;
    virtual StatementType getType() const = 0;
};

class CreateTableStmt : public Statement {
    std::string table_name;
    std::vector<Column> columns;
};

class InsertStmt : public Statement {
    std::string table_name;
    std::vector<Value> values;
};

class SelectStmt : public Statement {
    std::vector<std::string> columns;  // "*" ou lista
    std::string table_name;
    Expression* where_clause;  // nullptr se não houver WHERE
};

class DeleteStmt : public Statement {
    std::string table_name;
    Expression* where_clause;
};

// Expressões para WHERE clause
class Expression {
public:
    virtual ~Expression() = default;
    virtual Value evaluate(const Row& row) = 0;
};

class BinaryExpr : public Expression {
    Operator op;
    Expression* left;
    Expression* right;
};

class ColumnExpr : public Expression {
    std::string column_name;
};

class LiteralExpr : public Expression {
    Value value;
};
```

**Estado Atual:** ⏳ Planejado (FASE 4)

---

### 5. **Executor (Motor de Execução)** — `src/executor/executor.cpp`

**Responsabilidade:** Executar comandos SQL

**Funcionalidades:**
- Interpretar AST
- Coordenar Catalog e Storage
- Executar CREATE TABLE
- Executar INSERT
- Executar SELECT (com/sem WHERE)
- Executar DELETE (com/sem WHERE)
- Avaliar expressões WHERE
- Retornar ResultSet

**Fluxo de Execução:**

```cpp
class Executor {
    Catalog* catalog_;
    StorageEngine* storage_;

public:
    ResultSet execute(Statement* stmt) {
        switch (stmt->getType()) {
            case CREATE_TABLE:
                return executeCreate((CreateTableStmt*)stmt);
            case INSERT:
                return executeInsert((InsertStmt*)stmt);
            case SELECT:
                return executeSelect((SelectStmt*)stmt);
            case DELETE:
                return executeDelete((DeleteStmt*)stmt);
        }
    }
};
```

**Estado Atual:** ⏳ Planejado (FASE 7)

---

### 6. **Catalog (Gerenciador de Schemas)** — `src/catalog/catalog.cpp`

**Responsabilidade:** Metadados do banco de dados

**Funcionalidades:**
- Armazenar definições de tabelas
- Validar tipos de dados
- Verificar existência de tabelas/colunas
- Persistir catálogo em disco (`catalog.db`)

**Estrutura:**

```cpp
class Column {
    std::string name;
    DataType type;  // INT, TEXT
};

class TableSchema {
    std::string name;
    std::vector<Column> columns;
};

class Catalog {
    std::map<std::string, TableSchema> schemas_;

public:
    void createTable(const TableSchema& schema);
    TableSchema getTableSchema(const std::string& name);
    bool tableExists(const std::string& name);
    void dropTable(const std::string& name);
    std::vector<std::string> listTables();
    void save();  // Persistir
    void load();  // Carregar do disco
};
```

**Formato de Persistência (Texto):**
```
TABLE users
COLUMN id INT
COLUMN name TEXT
TABLE products
COLUMN id INT
COLUMN price TEXT
```

**Estado Atual:** ⏳ Planejado (FASE 5)

---

### 7. **Storage Engine** — `src/storage/table_storage.cpp`

**Responsabilidade:** Persistência de dados

**Funcionalidades:**
- Armazenar rows em disco
- Um arquivo por tabela (`users.db`)
- Layout binário row-based
- Operações: insert, scan, delete

**Layout de Arquivo:**

```
┌────────────────────────────────────────┐
│  RowCount: 4 bytes (número de rows)   │
├────────────────────────────────────────┤
│  Row 1:                                │
│    RowSize: 4 bytes                    │
│    Data: variable length               │
├────────────────────────────────────────┤
│  Row 2:                                │
│    RowSize: 4 bytes                    │
│    Data: variable length               │
├────────────────────────────────────────┤
│  ...                                   │
└────────────────────────────────────────┘
```

**Layout de Row (para `users(id INT, name TEXT)`):**

```
┌─────────────┬──────────┬─────────────┬──────────────┐
│ RowSize     │ id       │ name_length │ name_data    │
│ (4 bytes)   │ (4 bytes)│ (4 bytes)   │ (n bytes)    │
└─────────────┴──────────┴─────────────┴──────────────┘
```

**Interface:**

```cpp
class TableStorage {
    std::fstream file_;
    std::string filename_;

public:
    void open(const std::string& table_name);
    void close();
    void insertRow(const Row& row);
    std::vector<Row> scanTable();
    void deleteRow(int row_id);
};
```

**Estado Atual:** ⏳ Planejado (FASE 6)

---

## 🔄 Fluxo Completo de Execução

### Exemplo: `SELECT name FROM users WHERE id = 1;`

```
1. SHELL
   ├─ Usuário digita: SELECT name FROM users WHERE id = 1;
   ├─ Buffer acumula até encontrar ;
   └─ Envia SQL string → Lexer

2. LEXER
   ├─ Tokeniza: [SELECT][name][FROM][users][WHERE][id][=][1][;]
   └─ Tokens → Parser

3. PARSER
   ├─ Valida sintaxe
   ├─ Cria SelectStmt node
   │   ├─ columns: ["name"]
   │   ├─ table: "users"
   │   └─ where: BinaryExpr(ColumnExpr("id"), EQUAL, LiteralExpr(1))
   └─ AST → Executor

4. EXECUTOR
   ├─ Recebe SelectStmt
   ├─ Consulta Catalog: schema de "users"
   ├─ Chama Storage: scanTable("users")
   ├─ Para cada row:
   │   ├─ Avalia WHERE: id == 1?
   │   └─ Se true, adiciona ao resultado
   ├─ Projeta apenas coluna "name"
   └─ Retorna ResultSet

5. CATALOG
   ├─ Retorna schema: users(id INT, name TEXT)
   └─ Valida que coluna "name" existe

6. STORAGE
   ├─ Abre users.db
   ├─ Lê todas as rows
   └─ Retorna vector<Row>

7. SHELL
   ├─ Recebe ResultSet
   ├─ Formata tabela:
   │     name
   │     ----
   │     Ana
   └─ Exibe para usuário
```

---

## 🎯 Decisões Arquiteturais

### 1. Separação em Camadas

**Decisão:** Arquitetura em camadas (Shell → Frontend → Backend → Storage)

**Justificativa:**
- Modularidade e testabilidade
- Cada camada tem responsabilidade única
- Facilita expansão futura (ex: adicionar otimizador)
- Padrão da indústria (SQLite, PostgreSQL)

### 2. Row-Based Storage

**Decisão:** Armazenamento row-based, não column-based

**Justificativa:**
- Mais simples de implementar
- Adequado para operações transacionais (INSERT/UPDATE)
- Suficiente para V1 do projeto
- Pode migrar para columnar depois (FASE avançada)

### 3. Recursive Descent Parser

**Decisão:** Parser manual com recursive descent, não gerador (yacc/bison)

**Justificativa:**
- Controle total sobre mensagens de erro
- Mais didático e claro
- Sem dependências externas
- Gramática SQL simplificada não justifica gerador

### 4. C++17 Moderno

**Decisão:** Usar C++17 como padrão

**Justificativa:**
- `std::optional`, `std::variant`, `std::filesystem` úteis
- Smart pointers para gerenciamento de memória
- Amplo suporte em compiladores (GCC, Clang, MSVC)
- Não tão novo a ponto de causar problemas de compatibilidade

### 5. Um Arquivo por Tabela

**Decisão:** Cada tabela em arquivo separado (`users.db`, `products.db`)

**Justificativa:**
- Implementação mais simples
- Facilita debugging
- Locks por tabela mais simples
- PostgreSQL usa abordagem similar

---

## 📊 Diagrama de Classes (Simplificado)

```
┌─────────────┐
│    REPL     │
└──────┬──────┘
       │
       │ uses
       ▼
┌─────────────┐      ┌──────────────┐
│    Lexer    │─────▶│    Token     │
└──────┬──────┘      └──────────────┘
       │
       │ produces
       ▼
┌─────────────┐      ┌──────────────┐
│   Parser    │─────▶│  Statement   │◀───────┐
└──────┬──────┘      └──────┬───────┘        │
       │                    │                 │
       │                    │ inherits        │
       │                    ▼                 │
       │          ┌──────────────────┐        │
       │          │  CreateTableStmt │        │
       │          │    InsertStmt    │        │
       │          │    SelectStmt    │        │
       │          │    DeleteStmt    │        │
       │          └──────────────────┘        │
       │                                      │
       │ creates                              │ uses
       ▼                                      │
┌─────────────┐                              │
│  Executor   │──────────────────────────────┘
└──────┬──────┘
       │
       │ uses
       ▼
┌──────────────┐      ┌───────────────┐
│   Catalog    │      │ StorageEngine │
└──────────────┘      └───────────────┘
       │                      │
       │ persists to          │ persists to
       ▼                      ▼
┌──────────────┐      ┌───────────────┐
│ catalog.db   │      │  users.db     │
│              │      │  products.db  │
└──────────────┘      └───────────────┘
```

---

## 🔮 Evolução Futura da Arquitetura

### FASE 9 — Indexação
```
StorageEngine
    ├─ TableStorage (heap file)
    └─ PrimaryIndex (B-Tree)
```

### FASE 10 — WAL & Recovery
```
Storage Layer
    ├─ TableStorage
    ├─ WAL (Write-Ahead Log)
    └─ RecoveryManager
```

### Futuro — Query Optimizer
```
Parser → AST → Optimizer → ExecutionPlan → Executor
                   │
                   └─ Statistics (Catalog)
```

### Futuro — Concorrência
```
Executor
    ├─ TransactionManager
    ├─ LockManager
    └─ MVCC (Multi-Version Concurrency Control)
```

---

**Atualizado:** 23/12/2025  
**Versão:** 1.0

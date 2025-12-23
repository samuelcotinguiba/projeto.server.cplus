# 🗺️ Workflow de Desenvolvimento — Mini SQL DB Engine

> Plano detalhado de implementação fase a fase, do planejamento até o projeto avançado completo.

---

## 🔹 FASE 0 — Planejamento & Escopo

### ✅ Status: Concluído

### Objetivos
- Definir SQL subset viável
- Estabelecer limitações claras
- Evitar escopo infinito

### SQL Suportado (V1)
```sql
CREATE TABLE users (id INT, name TEXT);
INSERT INTO users VALUES (1, 'Ana');
SELECT * FROM users;
SELECT name FROM users WHERE id = 1;
DELETE FROM users WHERE id = 1;
```

### Fora do Escopo (V1)
❌ JOINs  
❌ Subqueries  
❌ Transações completas  
❌ SQL ANSI completo  

### Deliverables
- [x] Documento de escopo
- [x] Definição de arquitetura
- [x] README inicial

📌 **Commit:** `chore: define SQL subset and project scope`

---

## 🔹 FASE 1 — Estrutura Base do Projeto

### ✅ Status: Concluído

### Objetivos
- Criar base profissional e escalável
- Estrutura de diretórios limpa
- Build system configurado

### Estrutura de Diretórios
```
/src
 ├── main.cpp
 ├── shell/          # REPL interativo
 ├── lexer/          # Análise léxica
 ├── parser/         # Análise sintática
 ├── ast/            # Árvore sintática
 ├── executor/       # Motor de execução
 ├── storage/        # Persistência
 ├── catalog/        # Schema manager
 └── common/         # Tipos e utils

/include             # Headers públicos
/tests               # Testes unitários
/doc                 # Documentação técnica
/docs                # Docs adicionais
```

### Arquivos de Configuração
- `CMakeLists.txt` — Build system
- `Makefile` — Build rápido
- `.gitignore` — Arquivos ignorados
- `README.md` — Documentação principal
- `WORKFLOW.md` — Este documento
- `USAGE.md` — Guia de uso
- `QUICKSTART.md` — Quick start

### Deliverables
- [x] Estrutura de diretórios
- [x] .gitignore
- [x] README.md
- [x] WORKFLOW.md
- [x] USAGE.md
- [x] QUICKSTART.md
- [x] CMakeLists.txt
- [x] Makefile
- [x] main.cpp básico

📌 **Commit:** `chore: initial layered project structure`

---

## 🔹 FASE 2 — SQL Shell (REPL)

### ✅ Status: Concluído

### Objetivos
- Criar interface interativa estilo `sqlite3`
- Loop de leitura de comandos
- Comandos meta (`.exit`, `.help`)

### Exemplo de Uso
```
$ ./miniql
miniql v0.1.0
Type ".help" for usage hints.

miniql> CREATE TABLE users (id INT, name TEXT);
Table created.

miniql> INSERT INTO users VALUES (1, 'Ana');
1 row inserted.

miniql> SELECT * FROM users;
id | name
---+-----
1  | Ana

miniql> .exit
Goodbye!
```

### Funcionalidades Implementadas
- [x] Loop REPL básico
- [x] Comandos SQL encaminhados ao engine (placeholder)
- [x] Comandos meta:
  - [x] `.exit` — Sair
  - [x] `.quit` — Alias para exit
  - [x] `.help` — Ajuda
  - [x] `.tables` — Listar tabelas (placeholder)
  - [x] `.schema <table>` — Ver schema (placeholder)
- [x] Buffer multi-linha (acumula até `;`)
- [x] Prompt contextual (`miniql>` ou `     ->`)
- [x] Tratamento de EOF (Ctrl+D)
- [x] Trimming de whitespace
- [x] Error handling para comandos inválidos

### Implementação
- [x] `src/shell/repl.cpp`
- [x] `include/shell/repl.h`
- [x] `src/main.cpp`

### Documentação
- [x] [`doc/shell-implementation.md`](doc/shell-implementation.md) — Implementação detalhada

### Deliverables
- [x] Loop REPL funcional
- [x] Comandos meta implementados
- [x] Formatação de resultados (placeholder)
- [x] Demo script (`demo.sh`)
- [x] Documentação completa

📌 **Commit:** `feat: interactive SQL shell (REPL)`

---

## 🔹 FASE 3 — Lexer (Análise Léxica)

### 🎯 Status: Próximo (Em Planejamento)

### Objetivos
- Transformar SQL em tokens
- Reconhecer palavras-chave, identificadores, operadores
- Tratamento de erros léxicos

### Exemplo de Tokenização
```sql
SELECT name FROM users WHERE id = 1;
```

**Tokens gerados:**
```
SELECT    (keyword)
name      (identifier)
FROM      (keyword)
users     (identifier)
WHERE     (keyword)
id        (identifier)
=         (operator)
1         (number)
;         (semicolon)
```

### Tokens Suportados
- **Keywords:** SELECT, INSERT, CREATE, TABLE, FROM, WHERE, VALUES, DELETE, INT, TEXT
- **Identifiers:** nomes de tabelas, colunas
- **Literals:** números (123), strings ('text')
- **Operators:** =, <, >, <=, >=, !=
- **Symbols:** (, ), ,, ;, *

### Implementação Planejada
- [ ] `src/lexer/lexer.cpp`
- [ ] `include/lexer/lexer.h`
- [ ] `include/lexer/token.h`

### Conceitos Aplicados
- Autômatos finitos
- State machine
- Lookahead

### Deliverables
- [ ] Classe Token
- [ ] Classe Lexer
- [ ] Reconhecimento de todos os tokens
- [ ] Tratamento de erros
- [ ] Testes unitários do lexer

📌 **Commit:** `feat: SQL lexer implementation`

---

## 🔹 FASE 4 — Parser + AST

### ⏳ Status: Pendente

### Objetivos
- Criar árvore sintática a partir dos tokens
- Validar sintaxe SQL
- Estrutura orientada a objetos

### Exemplo de AST
```sql
SELECT name FROM users WHERE id = 1;
```

**AST gerada:**
```
SelectStmt
 ├── columns: [name]
 ├── table: users
 └── where:
      └── BinaryExpr (=)
           ├── left: id
           └── right: 1
```

### Classes AST Planejadas
```cpp
class Statement { virtual ~Statement() = default; };

class CreateTableStmt : public Statement {
    std::string table_name;
    std::vector<Column> columns;
};

class InsertStmt : public Statement {
    std::string table_name;
    std::vector<Value> values;
};

class SelectStmt : public Statement {
    std::vector<std::string> columns;
    std::string table_name;
    Expression* where_clause;
};

class DeleteStmt : public Statement {
    std::string table_name;
    Expression* where_clause;
};
```

### Técnicas
- **Recursive Descent Parser**
- Análise top-down
- Error recovery

### Implementação Planejada
- [ ] `src/parser/parser.cpp`
- [ ] `include/parser/parser.h`
- [ ] `include/ast/statements.h`
- [ ] `include/ast/expressions.h`

### Deliverables
- [ ] Classes AST
- [ ] Parser com recursive descent
- [ ] Parsing de CREATE TABLE
- [ ] Parsing de INSERT
- [ ] Parsing de SELECT
- [ ] Parsing de DELETE
- [ ] Mensagens de erro claras
- [ ] Testes unitários

📌 **Commit:** `feat: SQL parser and AST generation`

---

## 🔹 FASE 5 — Catalog (Schema Manager)

### ⏳ Status: Pendente

### Objetivos
- Gerenciar metadados do banco
- Armazenar schemas de tabelas
- Validar tipos de dados

### Responsabilidades
- Criar/remover tabelas
- Armazenar definições de colunas
- Validar tipos (INT, TEXT)
- Persistir catálogo em disco

### Estrutura do Catálogo
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
    void createTable(TableSchema schema);
    TableSchema getTableSchema(const std::string& name);
    bool tableExists(const std::string& name);
    void dropTable(const std::string& name);
    void save();  // Persistir em disco
    void load();  // Carregar do disco
};
```

### Persistência
- Arquivo `catalog.db` (texto ou binário)
- Formato:
  ```
  TABLE users
  COLUMN id INT
  COLUMN name TEXT
  ```

### Implementação Planejada
- [ ] `src/catalog/catalog.cpp`
- [ ] `include/catalog/catalog.h`
- [ ] `include/catalog/schema.h`

### Deliverables
- [ ] Classe Catalog
- [ ] Gerenciamento de schemas
- [ ] Validação de tipos
- [ ] Persistência do catálogo
- [ ] Testes unitários

📌 **Commit:** `feat: catalog and schema manager`

---

## 🔹 FASE 6 — Storage Engine (Persistência)

### ⏳ Status: Pendente

### Objetivos
- Armazenar dados em disco
- Um arquivo por tabela
- Layout binário eficiente

### Modelo de Storage
- **Row-based storage**
- Arquivo: `users.db`
- Layout:
  ```
  [RowCount: 4 bytes]
  [Row1: size + data]
  [Row2: size + data]
  ...
  ```

### Formato de Row
```
| RowSize (4 bytes) | id (4 bytes) | name_len (4 bytes) | name (n bytes) |
```

### Operações
```cpp
class TableStorage {
    void insertRow(const Row& row);
    std::vector<Row> scanTable();
    void deleteRow(int row_id);
    void open(const std::string& filename);
    void close();
};
```

### Implementação Planejada
- [ ] `src/storage/table_storage.cpp`
- [ ] `include/storage/table_storage.h`
- [ ] `include/storage/row.h`

### Conceitos
- Binary I/O
- File handling
- Serialização/Deserialização

### Deliverables
- [ ] Classe TableStorage
- [ ] Inserção de rows
- [ ] Scan completo de tabela
- [ ] Deleção de rows
- [ ] Testes unitários

📌 **Commit:** `feat: row-based table storage engine`

---

## 🔹 FASE 7 — Executor (Engine de Execução)

### ⏳ Status: Pendente

### Objetivos
- Executar comandos SQL
- Integrar Catalog + Storage
- Retornar resultados formatados

### Fluxo de Execução
```
AST → Executor → Catalog/Storage → Result
```

### Implementação por Comando

#### CREATE TABLE
```cpp
void Executor::execute(CreateTableStmt* stmt) {
    catalog->createTable(stmt->toSchema());
    storage->createFile(stmt->table_name);
}
```

#### INSERT
```cpp
void Executor::execute(InsertStmt* stmt) {
    auto schema = catalog->getTableSchema(stmt->table_name);
    auto row = stmt->toRow(schema);
    storage->insertRow(stmt->table_name, row);
}
```

#### SELECT
```cpp
ResultSet Executor::execute(SelectStmt* stmt) {
    auto rows = storage->scanTable(stmt->table_name);
    return projectColumns(rows, stmt->columns);
}
```

#### DELETE
```cpp
void Executor::execute(DeleteStmt* stmt) {
    auto rows = storage->scanTable(stmt->table_name);
    for (auto& row : rows) {
        if (evaluateWhere(stmt->where_clause, row)) {
            storage->deleteRow(stmt->table_name, row.id);
        }
    }
}
```

### Implementação Planejada
- [ ] `src/executor/executor.cpp`
- [ ] `include/executor/executor.h`
- [ ] `include/executor/result_set.h`

### Deliverables
- [ ] Classe Executor
- [ ] Execução de CREATE TABLE
- [ ] Execução de INSERT
- [ ] Execução de SELECT (sem WHERE)
- [ ] Execução de DELETE (sem WHERE)
- [ ] Testes de integração

📌 **Commit:** `feat: SQL execution engine`

---

## 🔹 FASE 8 — WHERE Clause (Filtro)

### ⏳ Status: Pendente

### Objetivos
- Avaliar expressões booleanas
- Suportar comparações simples
- Integrar com SELECT e DELETE

### Expressões Suportadas
```sql
WHERE id = 1
WHERE name = 'Ana'
WHERE id > 10
WHERE id <= 100
```

### Estrutura de Expressões
```cpp
class Expression { virtual Value eval(const Row& row) = 0; };

class ColumnExpr : public Expression {
    std::string column_name;
};

class LiteralExpr : public Expression {
    Value value;
};

class BinaryExpr : public Expression {
    Operator op;  // =, <, >, <=, >=, !=
    Expression* left;
    Expression* right;
};
```

### Avaliador
```cpp
class ExpressionEvaluator {
    bool evaluate(Expression* expr, const Row& row);
};
```

### Implementação Planejada
- [ ] `src/executor/expression_evaluator.cpp`
- [ ] `include/executor/expression_evaluator.h`

### Deliverables
- [ ] Classes de expressões
- [ ] Avaliador de expressões
- [ ] Integração com SELECT
- [ ] Integração com DELETE
- [ ] Testes unitários

📌 **Commit:** `feat: WHERE clause evaluation`

---

## 🔹 FASE 9 — Indexação Primária (Diferencial)

### ⏳ Status: Planejado

### Objetivos
- Acelerar buscas por chave
- Implementar índice primário
- Demonstrar domínio de estruturas de dados

### Estrutura
```cpp
class PrimaryIndex {
    std::map<int, int> index;  // key -> row_offset
    void insert(int key, int offset);
    int find(int key);
};
```

### Benefícios
- Lookup O(log n) vs O(n)
- Queries rápidas com WHERE id = X

### Implementação Planejada
- [ ] `src/storage/primary_index.cpp`
- [ ] `include/storage/primary_index.h`

### Deliverables
- [ ] Classe PrimaryIndex
- [ ] Integração com TableStorage
- [ ] Otimização de queries
- [ ] Benchmarks

📌 **Commit:** `feat: primary index support`

---

## 🔹 FASE 10 — WAL & Recovery (Projeto Avançado)

### ⏳ Status: Planejado

### Objetivos
- Garantir durabilidade (ACID)
- Write-Ahead Logging
- Recovery em crash

### Funcionamento
1. Antes de modificar dados, escrever log
2. Log commitado → operação segura
3. Crash → replay do log no startup

### Estrutura WAL
```
[BEGIN TRANSACTION]
[INSERT users 1 'Ana']
[COMMIT]
```

### Implementação Planejada
- [ ] `src/storage/wal.cpp`
- [ ] `include/storage/wal.h`

### Deliverables
- [ ] Sistema WAL
- [ ] Recovery automático
- [ ] Testes de crash

📌 **Commit:** `feat: write-ahead logging and crash recovery`

---

## 🔹 FASE 11 — Testes Automatizados

### ⏳ Status: Planejado

### Objetivos
- Suite completa de testes
- Cobertura > 80%
- CI/CD ready

### Tipos de Testes
- **Unit:** Lexer, Parser, Catalog, Storage
- **Integration:** Executor completo
- **End-to-End:** SQL queries completas

### Framework
- Google Test ou Catch2

### Implementação Planejada
- [ ] `tests/lexer_test.cpp`
- [ ] `tests/parser_test.cpp`
- [ ] `tests/executor_test.cpp`
- [ ] `tests/storage_test.cpp`

### Deliverables
- [ ] Testes unitários completos
- [ ] Testes de integração
- [ ] Testes E2E
- [ ] Script de CI

📌 **Commit:** `test: add automated tests`

---

## 🔹 FASE 12 — Documentação Profissional

### ⏳ Status: Em Progresso

### Objetivos
- Documentação técnica completa
- Diagramas de arquitetura
- Decisões de design

### Conteúdo
- Arquitetura detalhada
- Fluxo de execução SQL
- Decisões técnicas
- Limitações conhecidas
- Roadmap futuro

### Arquivos
- [x] `doc/architecture.md`
- [x] `doc/shell-implementation.md`
- [x] `doc/technical-decisions.md`
- [x] `doc/development-guide.md`
- [ ] `doc/sql_grammar.md`
- [ ] `doc/storage_format.md`

### Deliverables
- [x] Documentação base
- [ ] Documentação completa
- [ ] Diagramas
- [ ] Exemplos de uso

📌 **Commit:** `docs: full technical documentation`

---

## 🏆 Checklist Geral de Progresso

### Estrutura Base
- [x] Criar diretórios
- [x] .gitignore
- [x] README.md
- [x] WORKFLOW.md
- [x] USAGE.md
- [x] CMakeLists.txt
- [x] Makefile
- [x] main.cpp

### Funcionalidades Core
- [x] SQL Shell (REPL)
- [ ] Lexer
- [ ] Parser + AST
- [ ] Catalog
- [ ] Storage Engine
- [ ] Executor
- [ ] WHERE Clause

### Features Avançadas
- [ ] Indexação Primária
- [ ] WAL & Recovery
- [ ] Testes Automatizados
- [ ] Documentação Técnica Completa

### Portfolio Ready
- [x] README profissional
- [x] Commits organizados
- [ ] Testes passando
- [ ] Documentação completa
- [x] Demo funcional

---

## 📊 Status Atual

**Fase Ativa:** FASE 2 (Shell) — ✅ Concluído  
**Próximo:** FASE 3 (Lexer)  
**Progresso Geral:** ~20%  
**Próximo Marco:** Lexer + Parser funcionais

---

## 🎯 Próximos Passos Imediatos

1. ✅ Concluir documentação base
2. 🎯 Implementar Lexer (FASE 3)
3. 🎯 Implementar Parser + AST (FASE 4)
4. 🎯 Implementar Storage + Catalog (FASE 5-6)
5. 🎯 Implementar Executor básico (FASE 7)

---

**Última Atualização:** 23/12/2025  
**Versão do Workflow:** 2.0  
**Mantido por:** Samuel Cotinguiba

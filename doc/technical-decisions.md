# 🎯 Decisões Técnicas e Justificativas

## Visão Geral

Este documento explica **por que** certas escolhas técnicas foram feitas no projeto MiniQL, não apenas **como** foram implementadas. Serve como registro de decisões arquiteturais (ADR - Architecture Decision Records) e demonstra pensamento crítico de engenharia.

---

## 📋 Índice de Decisões

1. [Linguagem: C++ ao invés de C, Rust ou Go](#1-linguagem-c)
2. [Padrão: C++17 ao invés de C++11/C++20](#2-padrão-c17)
3. [Build System: Makefile + CMake](#3-build-system)
4. [Arquitetura: Camadas ao invés de Monolítico](#4-arquitetura-em-camadas)
5. [Parser: Manual ao invés de Gerador](#5-parser-manual)
6. [Storage: Row-based ao invés de Column-based](#6-storage-row-based)
7. [Persistência: Um arquivo por tabela](#7-um-arquivo-por-tabela)
8. [Formato: Binário ao invés de Texto](#8-formato-binário)
9. [Namespace: `miniql` ao invés de global](#9-namespace-miniql)
10. [REPL: Implementação própria ao invés de biblioteca](#10-repl-próprio)

---

## 1. Linguagem: C++

### Decisão
Usar **C++** como linguagem principal do projeto.

### Alternativas Consideradas
- **C puro** — Mais baixo nível, controle total
- **Rust** — Memory safety, moderno
- **Go** — Simples, garbage collector

### Justificativa

#### Por que C++?

**✅ Vantagens:**
- **Performance nativa** — Sem garbage collector, controle de memória
- **OOP + Procedural** — Flexibilidade arquitetural
- **STL rica** — `std::vector`, `std::map`, `std::string`, etc.
- **Industria padrão** — SQLite, MySQL, PostgreSQL são em C/C++
- **Portfólio** — C++ é valorizado em vagas de sistemas/backend
- **Smart pointers** — C++11+ oferece RAII sem GC

**❌ Desvantagens aceitas:**
- Mais complexo que Go/Python
- Gerenciamento manual de memória (mitigado por smart pointers)

#### Por que não C puro?
- Sem OOP dificultaria abstrações (AST, Statements)
- Sem `std::string`, `std::vector` (teria que implementar do zero)
- Gerenciamento de memória mais trabalhoso

#### Por que não Rust?
- Curva de aprendizado muito íngreme
- Borrow checker complicaria desenvolvimento inicial
- Menos material sobre database engines em Rust

#### Por que não Go?
- Garbage collector não é ideal para DB engine
- Menos controle sobre layout de memória
- Não é a linguagem padrão da indústria de databases

### Conclusão
C++ oferece o melhor balanço entre **performance**, **expressividade** e **alinhamento com a indústria**.

---

## 2. Padrão: C++17

### Decisão
Usar **C++17** como padrão de linguagem.

### Alternativas Consideradas
- **C++11** — Mais compatibilidade
- **C++14** — Intermediário
- **C++20** — Mais features modernas

### Justificativa

#### Por que C++17?

**✅ Features úteis:**
```cpp
// std::optional — Retornar "valor ou nada"
std::optional<TableSchema> Catalog::getTable(const std::string& name);

// std::variant — Union type-safe
using Value = std::variant<int, std::string>;

// std::filesystem — Manipulação de arquivos
std::filesystem::exists("users.db");

// Structured bindings
auto [key, value] = map.find("users");

// if with initializer
if (auto schema = catalog.getTable("users"); schema) {
    // usar schema
}
```

**✅ Vantagens:**
- Amplamente suportado (GCC 7+, Clang 5+, MSVC 2017+)
- Não tão novo a ponto de causar problemas
- Features modernas sem instabilidade de C++20

#### Por que não C++11/14?
- Sem `std::optional` (teria que usar ponteiros/flags)
- Sem `std::variant` (teria que usar union inseguro)
- Sem `std::filesystem` (manipulação de arquivos manual)

#### Por que não C++20?
- Muitos compiladores ainda não suportam completamente
- Modules, Concepts, Ranges não são essenciais para V1
- Pode causar problemas de compatibilidade

### Conclusão
C++17 é o **sweet spot**: moderno mas estável.

---

## 3. Build System: Makefile + CMake

### Decisão
Oferecer **ambos** Makefile e CMake.

### Justificativa

#### Makefile
**Uso:** Desenvolvimento rápido, iteração local

**Vantagens:**
- Simples e direto
- Build rápido para mudanças pequenas
- Não precisa gerar arquivos intermediários

**Exemplo:**
```bash
make        # Build
make clean  # Limpar
make run    # Build + Run
```

#### CMake
**Uso:** Build profissional, CI/CD, cross-platform

**Vantagens:**
- Cross-platform (Linux, macOS, Windows)
- Gera projetos para IDEs (Xcode, Visual Studio)
- Padrão da indústria para C++
- Integração com package managers (vcpkg, Conan)

**Exemplo:**
```bash
mkdir build && cd build
cmake ..
make
```

### Conclusão
**Makefile** para desenvolvimento rápido, **CMake** para produção e distribuição.

---

## 4. Arquitetura em Camadas

### Decisão
Separar projeto em **camadas distintas** (Shell → Lexer → Parser → Executor → Storage).

### Alternativas Consideradas
- **Monolítico** — Tudo em um arquivo/classe
- **Modular flat** — Vários módulos sem hierarquia clara

### Justificativa

#### Por que Camadas?

**✅ Vantagens:**
- **Separação de responsabilidades** — Cada camada tem função clara
- **Testabilidade** — Testar Lexer isolado do Storage
- **Manutenibilidade** — Mudanças em uma camada não afetam outras
- **Escalabilidade** — Adicionar Query Optimizer entre Parser e Executor
- **Padrão da indústria** — PostgreSQL, MySQL usam arquitetura similar

**Exemplo de isolamento:**
```cpp
// Posso trocar StorageEngine sem afetar Parser
class Executor {
    Parser* parser_;           // Dependência do frontend
    StorageEngine* storage_;   // Dependência do backend
    
    // Trocar storage não afeta parser
};
```

#### Camadas Definidas

```
┌──────────────┐
│    Shell     │  Interface
└──────┬───────┘
       │
┌──────▼───────┐
│    Lexer     │  Tokenização
└──────┬───────┘
       │
┌──────▼───────┐
│    Parser    │  Sintaxe
└──────┬───────┘
       │
┌──────▼───────┐
│   Executor   │  Lógica
└──┬─────────┬─┘
   │         │
┌──▼─────┐ ┌▼────────┐
│Catalog │ │ Storage │  Persistência
└────────┘ └─────────┘
```

### Conclusão
Arquitetura em camadas é **obrigatória** para projeto de qualidade.

---

## 5. Parser Manual (Recursive Descent)

### Decisão
Implementar **parser manualmente** usando recursive descent.

### Alternativas Consideradas
- **Gerador de parser** — Yacc, Bison, ANTLR
- **Parser combinator** — Bibliotecas tipo Spirit

### Justificativa

#### Por que Manual?

**✅ Vantagens:**
- **Controle total** — Mensagens de erro customizadas
- **Simplicidade** — Sem ferramentas externas
- **Didático** — Entendimento profundo de parsing
- **Gramática simples** — SQL subset não justifica gerador
- **Portfólio** — Demonstra capacidade de implementar parsers

**Exemplo de erro customizado:**
```cpp
// Parser manual permite:
throw ParserError("Expected ';' at end of statement, line 3, column 42");

// Vs gerador genérico:
syntax error
```

#### Por que não Yacc/Bison?

**❌ Desvantagens:**
- Dependência externa
- Mensagens de erro genéricas
- Curva de aprendizado de gramática BNF
- Overkill para gramática simples

#### Por que não Parser Combinator?

**❌ Desvantagens:**
- Biblioteca externa (Spirit é complexo)
- Pode ser lento em tempo de compilação
- Menos intuitivo que recursive descent

### Gramática é Simples

```
statement    → createStmt | insertStmt | selectStmt | deleteStmt
createStmt   → "CREATE" "TABLE" id "(" columnList ")"
insertStmt   → "INSERT" "INTO" id "VALUES" "(" valueList ")"
selectStmt   → "SELECT" columnList "FROM" id [whereClause]
deleteStmt   → "DELETE" "FROM" id [whereClause]
whereClause  → "WHERE" expression
expression   → id operator literal
```

**Conclusão:** Recursive descent é **adequado e suficiente**.

---

## 6. Storage: Row-Based

### Decisão
Usar **row-based storage** (heap file).

### Alternativas Consideradas
- **Column-based** — Colunas armazenadas separadamente
- **Hybrid** — Mix de row e column

### Justificativa

#### Por que Row-Based?

**✅ Vantagens:**
- **Simples de implementar** — Uma row = um bloco de dados
- **Adequado para OLTP** — Insert/Update de rows inteiras
- **Padrão tradicional** — MySQL, PostgreSQL usam row-based
- **Suficiente para V1** — Não é gargalo em escala pequena

**Estrutura de arquivo:**
```
[Row 1: id=1, name='Ana']
[Row 2: id=2, name='Bob']
[Row 3: id=3, name='Charlie']
```

#### Por que não Column-Based?

**❌ Desvantagens para nosso caso:**
- Mais complexo de implementar
- Melhor para OLAP (análise), não OLTP (transações)
- Overhead desnecessário para projeto educacional

**Quando column-based seria melhor:**
```sql
-- Análise agregada (OLAP)
SELECT AVG(price) FROM products;  -- Column-based é 10x+ mais rápido
```

**Nosso caso de uso (OLTP):**
```sql
-- Transações individuais
INSERT INTO users VALUES (1, 'Ana');
SELECT * FROM users WHERE id = 1;  -- Row-based é adequado
```

### Conclusão
Row-based é a escolha certa para um **database transacional** (V1).

---

## 7. Um Arquivo por Tabela

### Decisão
Cada tabela tem **seu próprio arquivo** (`users.db`, `products.db`).

### Alternativas Consideradas
- **Single file database** — Tudo em um arquivo (tipo SQLite)
- **Shared tablespace** — Múltiplas tabelas em poucos arquivos

### Justificativa

#### Por que Arquivo Separado?

**✅ Vantagens:**
- **Implementação simples** — Criar tabela = criar arquivo
- **Debugging fácil** — Ver conteúdo de uma tabela sem parsing complexo
- **Locks simples** — Lock por arquivo = lock por tabela
- **Crescimento independente** — Tabela grande não afeta tabelas pequenas
- **PostgreSQL faz isso** — Cada tabela/índice tem arquivo próprio

**Estrutura:**
```
data/
├── users.db          (heap file para users)
├── products.db       (heap file para products)
└── catalog.db        (metadados)
```

#### Por que não Single File?

**❌ Desvantagens:**
- Mais complexo implementar
- Precisa de sistema de páginas/blocos
- Lock global afeta todas as operações
- Mais difícil de debugar

**SQLite usa single file porque:**
- Portabilidade (um arquivo = um database)
- Embedded use case (anexar DB em email, etc)

**Nosso caso não precisa dessas features.**

### Conclusão
Arquivo por tabela é **mais simples e adequado** para V1.

---

## 8. Formato: Binário

### Decisão
Armazenar dados em **formato binário**, não texto.

### Alternativas Consideradas
- **Texto (CSV/JSON)** — Human-readable
- **Formato misto** — Metadados texto, dados binário

### Justificativa

#### Por que Binário?

**✅ Vantagens:**
- **Eficiência de espaço** — `int` = 4 bytes vs "12345" = 5 bytes
- **Performance** — Sem parsing/serialization
- **Tipos preservados** — INT é int, não string
- **Padrão da indústria** — Todos os DB sérios usam binário

**Exemplo de layout binário:**
```
Row para users(id INT, name TEXT)
┌─────────────┬──────────┬─────────────┬──────────────┐
│ RowSize     │ id       │ name_length │ name_data    │
│ 4 bytes     │ 4 bytes  │ 4 bytes     │ n bytes      │
└─────────────┴──────────┴─────────────┴──────────────┘

Exemplo: id=1, name="Ana"
[19][0x00000001][0x00000003]['A']['n']['a']
```

#### Por que não Texto?

**❌ Desvantagens:**
- **Espaço** — "123456789" ocupa 9 bytes vs 4 bytes
- **Performance** — `stoi()` é lento vs memcpy
- **Tipos** — "123" pode ser INT ou TEXT?
- **Complexidade** — Escapar strings, delimitadores

**Quando texto seria útil:**
- Debugging (mas podemos fazer dump tool)
- Portabilidade cross-platform (mas binário também funciona se controlar endianness)

### Conclusão
Binário é **mais eficiente** e **profissional**.

---

## 9. Namespace: `miniql`

### Decisão
Todo código dentro do namespace **`miniql::`**.

### Alternativa Considerada
- **Global namespace** — Sem namespace

### Justificativa

#### Por que Namespace?

**✅ Vantagens:**
- **Evita conflitos** — `miniql::Lexer` vs outra biblioteca com `Lexer`
- **Organização** — Clara separação do código do projeto
- **Profissional** — Padrão em projetos C++ reais
- **Escalável** — Permite sub-namespaces (`miniql::storage`, `miniql::parser`)

**Código:**
```cpp
namespace miniql {

class REPL { /* ... */ };
class Lexer { /* ... */ };
class Parser { /* ... */ };

} // namespace miniql

// Uso:
miniql::REPL repl;
```

#### Por que não Global?

**❌ Problemas:**
- Poluição de namespace global
- Conflitos com bibliotecas externas
- Dificulta refactoring futuro
- Não é padrão profissional

### Conclusão
Namespace é **boas práticas** obrigatórias.

---

## 10. REPL Próprio (sem readline)

### Decisão
Implementar REPL **manualmente**, sem bibliotecas externas (por enquanto).

### Alternativas Consideradas
- **GNU Readline** — Histórico, edição
- **linenoise** — Alternativa leve

### Justificativa

#### Por que Implementação Própria (V1)?

**✅ Vantagens:**
- **Zero dependências** — Build funciona em qualquer sistema
- **Simplicidade** — ~150 linhas vs integração de biblioteca
- **Didático** — Entender como REPLs funcionam
- **Portável** — Usa apenas `<iostream>` (C++ padrão)

**Implementação atual:**
```cpp
std::string line;
std::getline(std::cin, line);  // Simples e funcional
```

#### Por que não Readline (V1)?

**❌ Desvantagens:**
- Dependência externa (não está em todas as máquinas)
- Complicação de build (link contra libreadline)
- Não essencial para MVP

#### Plano Futuro (V2)

**🔄 Adicionar Readline depois:**
```cpp
#ifdef USE_READLINE
    #include <readline/readline.h>
    char* input = readline(prompt.c_str());
#else
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
#endif
```

**Features que Readline traria:**
- Histórico com ↑↓
- Edição com ←→, Ctrl+A/E
- Autocompletion
- Busca com Ctrl+R

### Conclusão
Implementação própria é **adequada para V1**. Readline pode ser adicionado como feature opcional depois.

---

## 📊 Resumo das Decisões

| # | Decisão | Alternativa | Justificativa |
|---|---------|-------------|---------------|
| 1 | C++ | C, Rust, Go | Performance + OOP + Padrão da indústria |
| 2 | C++17 | C++11, C++20 | Moderno mas estável |
| 3 | Makefile + CMake | Apenas um | Dev rápido + Build profissional |
| 4 | Camadas | Monolítico | Separação de responsabilidades |
| 5 | Parser manual | Yacc/Bison | Controle total + Simplicidade |
| 6 | Row-based | Column-based | Adequado para OLTP |
| 7 | Arquivo por tabela | Single file | Simplicidade + Locks |
| 8 | Binário | Texto | Eficiência + Performance |
| 9 | Namespace | Global | Organização + Profissionalismo |
| 10 | REPL próprio | Readline | Zero deps para V1 |

---

## 🎓 Lições Aprendidas

### 1. Simplicidade > Complexidade (quando adequado)
- Parser manual é mais simples que Yacc para nossa gramática
- Arquivo por tabela é mais simples que single file

### 2. Padrões da Indústria têm razão
- C++ para DB engines (performance)
- Row-based para OLTP (adequação)
- Binário para storage (eficiência)

### 3. MVP primeiro, features depois
- REPL sem readline funciona
- Row-based antes de column-based
- Storage simples antes de WAL

### 4. Decisões reversíveis vs irreversíveis
- **Irreversível:** Linguagem (C++ vs Rust)
- **Reversível:** REPL próprio → Readline depois

### 5. Tradeoffs são inevitáveis
- C++ performance vs segurança do Rust
- Row-based OLTP vs Column-based OLAP
- Simplicidade vs Features

---

## 🔮 Futuras Decisões

Próximas decisões técnicas a tomar:

1. **Indexação:** B-Tree próprio ou `std::map`?
2. **Concorrência:** Locks ou MVCC?
3. **WAL:** Binário ou texto estruturado?
4. **Network:** TCP raw ou usar biblioteca (Boost.Asio)?
5. **Query Optimizer:** Rule-based ou cost-based?

---

**Atualizado:** 23/12/2025  
**Versão:** 1.0  
**Nota:** Decisões podem evoluir conforme projeto amadurece.

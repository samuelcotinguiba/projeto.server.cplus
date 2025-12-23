# 🧠 Mini SQL Database Engine em C++

> Motor de banco de dados relacional simplificado escrito em C++ puro, com parser SQL próprio, AST, engine de execução e persistência em disco.

[![Language](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)](#)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

---

## 🎯 Objetivo

Desenvolver um **database engine funcional** (não um wrapper), inspirado em SQLite/PostgreSQL, que implementa:

- ✅ Linguagem SQL subset
- ✅ Parser e Lexer próprios  
- ✅ Árvore Sintática Abstrata (AST)
- ✅ Motor de execução de queries
- ✅ Persistência em disco
- ✅ Sistema de catálogo (schemas)
- 🔄 Indexação primária (planejado)
- 🔄 Write-Ahead Logging (planejado)

---

## 🚀 Quick Start

```bash
# Clone
git clone https://github.com/samuelcotinguiba/projeto.server.cplus.git
cd projeto.server.cplus

# Build
make

# Executar
./miniql

# Demo
./demo.sh
```

### Uso Básico

```sql
miniql> .help              -- Ajuda
miniql> CREATE TABLE users (id INT, name TEXT);
miniql> INSERT INTO users VALUES (1, 'Ana');
miniql> SELECT * FROM users;
miniql> .exit              -- Sair
```

---

## 📋 SQL Suportado (Roadmap V1)

```sql
-- Criação de tabelas
CREATE TABLE users (id INT, name TEXT);

-- Inserção de dados
INSERT INTO users VALUES (1, 'Ana');
INSERT INTO users VALUES (2, 'Bruno');

-- Consultas
SELECT * FROM users;
SELECT name FROM users WHERE id = 1;

-- Deleção
DELETE FROM users WHERE id = 2;
```

### Fora do Escopo (V1)

❌ JOINs  
❌ Subqueries  
❌ Transações ACID completas  
❌ Tipos complexos (DATE, BLOB)  
❌ ALTER TABLE  

---

## 🏗️ Arquitetura

```
┌─────────────┐
│  SQL Shell  │  Interface interativa
└──────┬──────┘
       │
┌──────▼──────┐
│    Lexer    │  Tokenização
└──────┬──────┘
       │
┌──────▼──────┐
│   Parser    │  Análise Sintática
└──────┬──────┘
       │
┌──────▼──────┐
│     AST     │  Árvore de comandos
└──────┬──────┘
       │
┌──────▼──────┐
│  Executor   │  Motor de execução
└──────┬──────┘
       │
   ┌───┴───┐
   │       │
┌──▼───┐ ┌─▼──────┐
│Catalog│ │Storage │
└──────┘ └────────┘
```

**Documentação Completa:** Veja [`doc/architecture.md`](doc/architecture.md)

---

## 📂 Estrutura do Projeto

```
/
├── src/                   # Código fonte C++
│   ├── main.cpp          # Entry point
│   ├── shell/            # ✅ REPL interativo
│   ├── lexer/            # ⏳ Tokenização SQL
│   ├── parser/           # ⏳ Análise sintática
│   ├── ast/              # ⏳ Árvore sintática
│   ├── executor/         # ⏳ Motor de execução
│   ├── storage/          # ⏳ Persistência
│   └── catalog/          # ⏳ Schema manager
├── include/              # Headers públicos
├── tests/                # Testes unitários
├── doc/                  # Documentação técnica detalhada
├── Makefile              # Build configuration
└── CMakeLists.txt        # CMake configuration
```

---

## 📊 Status de Desenvolvimento

### ✅ Concluído

- **FASE 0:** Planejamento & Escopo
- **FASE 1:** Estrutura base do projeto
- **FASE 2:** SQL Shell (REPL) completo e funcional

### 🔄 Em Progresso

- **FASE 3:** Lexer (tokenização SQL) — **PRÓXIMO**

### ⏳ Planejado

- **FASE 4:** Parser + AST
- **FASE 5:** Catalog (schema manager)
- **FASE 6:** Storage Engine (persistência)
- **FASE 7:** Executor (motor de execução)
- **FASE 8:** WHERE Clause (filtros)
- **FASE 9:** Indexação primária
- **FASE 10:** WAL & Recovery
- **FASE 11:** Testes automatizados
- **FASE 12:** Documentação completa

Veja o workflow completo em [`WORKFLOW.md`](WORKFLOW.md)

---

## 📚 Documentação

### Guias Rápidos
- [🚀 USAGE.md](USAGE.md) — Como usar, build, troubleshooting
- [⚡ QUICKSTART.md](QUICKSTART.md) — Começar rapidamente
- [🗺️ WORKFLOW.md](WORKFLOW.md) — Roadmap detalhado de desenvolvimento

### Documentação Técnica (pasta `doc/`)
- [🏗️ Arquitetura](doc/architecture.md) — Diagrama completo e componentes
- [🖥️ Shell Implementation](doc/shell-implementation.md) — Implementação detalhada do REPL
- [🎯 Decisões Técnicas](doc/technical-decisions.md) — Por que cada escolha foi feita
- [👨‍💻 Development Guide](doc/development-guide.md) — Guia para contribuidores

---

## 🛠️ Build & Execução

### Com Makefile (Recomendado)

```bash
# Build padrão
make

# Build debug (com símbolos)
make debug

# Build otimizado (release)
make release

# Executar
./miniql

# Build + Run
make run

# Limpar
make clean
```

### Com CMake

```bash
mkdir build && cd build
cmake ..
make
./miniql
```

---

## 🧪 Comandos do Shell

### Meta Comandos

```
.help              — Mostra ajuda
.exit / .quit      — Sai do programa
.tables            — Lista todas as tabelas
.schema <table>    — Mostra schema de uma tabela
```

### SQL (todos devem terminar com `;`)

```sql
CREATE TABLE name (col1 INT, col2 TEXT);
INSERT INTO name VALUES (1, 'text');
SELECT * FROM name;
SELECT col FROM name WHERE col = value;
DELETE FROM name WHERE col = value;
```

---

## 💡 Conceitos Técnicos Aplicados

### Já Implementados
- **Design de CLI/REPL** — Interface interativa estilo Unix
- **State Management** — Gerenciamento de estado do shell
- **Buffer Management** — Acumulação de comandos multi-linha
- **Error Handling** — Tratamento de EOF e comandos inválidos
- **Namespace Organization** — Código organizado em `miniql::`
- **Build Systems** — Makefile e CMake configurados

### Em Planejamento
- **Compiladores** — Lexer, Parser, AST
- **Estruturas de Dados** — B-Tree, Hash Tables
- **Persistência** — Binary I/O, row-based storage
- **Query Execution** — Executor engine
- **Schema Management** — Catalog system
- **Indexação** — Primary index com B-Tree
- **Durabilidade** — Write-Ahead Logging (WAL)

---

## 🎓 Por Que Este Projeto?

Este projeto demonstra:

1. **Domínio de C++** moderno (C++17)
2. **Design de compiladores** (Lexer, Parser, AST)
3. **Arquitetura de sistemas** (camadas bem definidas)
4. **Database internals** (storage, catalog, execution)
5. **Engenharia de software** (testes, docs, versionamento)

### Diferencial para Portfólio

> "Implementei um **motor de banco de dados relacional em C++**, com parser SQL próprio, AST, engine de execução, armazenamento persistente, indexação e recuperação via WAL."

Isso é **raríssimo** em portfólio de estudante/júnior.

---

## 🤝 Contribuindo

1. Fork o repositório
2. Crie uma branch: `git checkout -b feature/nome`
3. Commit com mensagem semântica: `git commit -m "feat: descrição"`
4. Push: `git push origin feature/nome`
5. Abra um Pull Request

Veja [USAGE.md](USAGE.md) para detalhes de desenvolvimento.

---

## 📝 Roadmap Futuro (V2+)

- [ ] Suporte a JOINs (INNER/LEFT)
- [ ] Transações ACID completas
- [ ] Query optimizer
- [ ] B+ Tree indexing
- [ ] Concorrência (MVCC)
- [ ] Network protocol (servidor TCP)
- [ ] SQL completo (subqueries, views, etc)

---

## 📄 Licença

MIT License — Projeto educacional para portfólio técnico.

---

## 👤 Autor

**Samuel Cotinguiba**  
Projeto de portfólio — Database Engine em C++

📧 Email: [seu-email]  
🔗 LinkedIn: [seu-linkedin]  
🐙 GitHub: [@samuelcotinguiba](https://github.com/samuelcotinguiba)

---

**⭐ Se este projeto foi útil, considere dar uma estrela no GitHub!**

# 📚 Guia Completo do MiniQL

> Guia completo de uso, compilação e desenvolvimento do MiniQL Database Engine

---

## 📖 Índice

- [Quick Start](#-quick-start)
- [Instalação e Build](#-instalação-e-build)
- [Como Usar](#-como-usar)
- [Comandos Disponíveis](#-comandos-disponíveis)
- [Demonstrações](#-demonstrações)
- [Troubleshooting](#-troubleshooting)
- [Desenvolvimento](#-desenvolvimento)

---

## 🚀 Quick Start

**Para começar rápido:**

```bash
# 1. Clone o projeto
git clone https://github.com/samuelcotinguiba/projeto.server.cplus.git
cd projeto.server.cplus

# 2. Compile
make

# 3. Execute
./miniql
```

**Dentro do shell:**

```sql
miniql> .help
miniql> CREATE TABLE users (id INT, name TEXT);
miniql> INSERT INTO users VALUES (1, 'Ana');
miniql> SELECT * FROM users;
miniql> .exit
```

---

## 🔨 Instalação e Build

### Pré-requisitos

```bash
# Compilador C++17
g++ --version       # GCC 7+ ou
clang++ --version   # Clang 5+

# Build tools
make --version      # GNU Make
cmake --version     # CMake 3.15+ (opcional)
```

### Compilação

#### Opção 1: Makefile (Recomendado para Desenvolvimento)

```bash
# Build padrão
make

# Build com símbolos de debug
make debug

# Build otimizado para produção
make release

# Build + executar
make run

# Limpar e recompilar
make clean
make rebuild
```

#### Opção 2: CMake (Recomendado para Produção)

```bash
# Criar pasta de build
mkdir build && cd build

# Configurar
cmake ..

# Compilar
make

# Instalar (opcional)
sudo make install
```

### ⚠️ O QUE NÃO FAZER

```bash
# ❌ NUNCA compile arquivos individualmente
g++ src/shell/repl.cpp  # ERRADO - vai dar erro

# ❌ NUNCA use o botão "Run" da extensão C++ do VSCode
# Use SEMPRE: make
```

### ✅ Verificar se Está Tudo OK

```bash
# Se este comando funciona, está tudo certo:
make clean && make

# Deve aparecer: "Build completo: ./miniql"
./miniql --version  # Deve mostrar a versão
```

---

## 💻 Como Usar

### Iniciar o Shell Interativo

```bash
./miniql
```

Você verá:
```
MiniQL v0.1.0 - Mini SQL Database Engine
Type ".help" for usage hints.
Type ".exit" to quit.

miniql>
```

### Comandos SQL Básicos

```sql
-- Criar tabela
CREATE TABLE users (id INT, name TEXT);

-- Inserir dados
INSERT INTO users VALUES (1, 'Ana Silva');
INSERT INTO users VALUES (2, 'Bruno Costa');

-- Consultar dados
SELECT * FROM users;
SELECT name FROM users WHERE id = 1;

-- Deletar dados
DELETE FROM users WHERE id = 2;

-- Comandos devem terminar com ponto-e-vírgula!
```

### Comandos Multi-linha

```sql
miniql> CREATE TABLE products (
     ->   id INT,
     ->   name TEXT,
     ->   price REAL
     -> );
```

O prompt muda para `     ->` indicando continuação.

---

## 🛠️ Comandos Disponíveis

### Meta Comandos (começam com `.`)

```sql
.help              -- Mostra ajuda e comandos disponíveis
.exit              -- Sai do programa
.quit              -- Alias para .exit
.tables            -- Lista todas as tabelas (em desenvolvimento)
.schema <table>    -- Mostra schema de uma tabela (em desenvolvimento)
```

### Comandos SQL Suportados

**Status Atual (v0.1.0):**

| Comando | Status | Exemplo |
|---------|--------|---------|
| `CREATE TABLE` | ⏳ Em desenvolvimento | `CREATE TABLE users (id INT, name TEXT);` |
| `INSERT INTO` | ⏳ Em desenvolvimento | `INSERT INTO users VALUES (1, 'Ana');` |
| `SELECT` | ⏳ Em desenvolvimento | `SELECT * FROM users;` |
| `DELETE` | ⏳ Em desenvolvimento | `DELETE FROM users WHERE id = 1;` |
| `UPDATE` | ⏳ Planejado | `UPDATE users SET name = 'João';` |
| `DROP TABLE` | ⏳ Planejado | `DROP TABLE users;` |

**Tipos de Dados:**
- `INT` — Números inteiros
- `TEXT` — Strings
- `REAL` — Números decimais (planejado)

---

## 🎪 Demonstrações

### Demo Automatizada

```bash
./demo.sh
```

Executa uma demonstração automática com exemplos de comandos.

### Demo do Analisador Léxico

```bash
# Compilar e executar
make run-lexer-demo

# Ou compilar separadamente
make lexer-demo
./lexer_demo
```

Mostra como o analisador léxico tokeniza comandos SQL.

---

## 🐛 Troubleshooting

### Erro: `command not found: make`

**Causa:** Build tools não instalados

**Solução:**
```bash
# macOS
xcode-select --install

# Linux (Ubuntu/Debian)
sudo apt install build-essential

# Linux (Fedora)
sudo dnf install gcc-c++ make
```

### Erro: `fatal error: 'shell/repl.h' file not found`

**Causa:** Tentou compilar arquivo individual ao invés de usar Makefile

**Solução:**
```bash
# Sempre compile do diretório raiz
cd projeto.server.cplus
make
```

### Linhas Vermelhas no VSCode mas `make` Funciona

**Causa:** IntelliSense do VSCode não configurado

**Solução:** Ignore os erros do editor se `make` compila sem problemas.

**Ou configure o IntelliSense:**

Crie `.vscode/c_cpp_properties.json`:

```json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/include",
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "macFrameworkPath": [],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "macos-gcc-arm64"
        }
    ],
    "version": 4
}
```

### Build Lento

**Causa:** Recompilação desnecessária

**Solução:** O Makefile já usa build incremental (recompila apenas o que mudou)

```bash
# Apenas arquivos modificados são recompilados
make
```

---

## 👨‍💻 Desenvolvimento

### Estrutura do Projeto

```
projeto.server.cplus/
├── src/                    # Código fonte
│   ├── main.cpp           # Entry point
│   ├── shell/             # Shell interativo (✅ implementado)
│   ├── lexer/             # Analisador léxico (✅ implementado)
│   ├── parser/            # Analisador sintático (⏳ próximo)
│   └── ...
├── include/               # Headers públicos
│   ├── shell/
│   ├── lexer/
│   └── ...
├── doc/                   # Documentação técnica
├── Makefile              # Build configuration
└── CMakeLists.txt        # CMake configuration
```

### Workflow de Desenvolvimento

```bash
# 1. Editar código
vim src/shell/repl.cpp

# 2. Recompilar
make

# 3. Testar
./miniql

# 4. Debug (se necessário)
make debug
gdb ./miniql
```

### Convenções de Código

```cpp
// Classes: PascalCase
class TableStorage {};

// Funções: camelCase
void processSQLCommand();

// Variáveis: snake_case
int row_count;
std::string table_name;

// Membros privados: snake_case com _
class REPL {
private:
    bool running_;
};

// Namespaces: lowercase
namespace miniql {
    // ...
}
```

### Testes

```bash
# Rodar testes (quando implementados)
make test

# Testes específicos
./build/tests/lexer_test
./build/tests/parser_test
```

---

## 📊 Status do Projeto

**Progresso Geral:** ~25% (3/12 fases concluídas)

### Fases Concluídas ✅

- ✅ **FASE 0:** Planejamento & Escopo
- ✅ **FASE 1:** Estrutura base do projeto
- ✅ **FASE 2:** Shell interativo (REPL)
- ✅ **FASE 3:** Analisador Léxico (Lexer)

### Em Desenvolvimento 🔄

- 🎯 **FASE 4:** Analisador Sintático (Parser) + AST

### Planejado ⏳

- ⏳ **FASE 5:** Sistema de Catálogo (schemas)
- ⏳ **FASE 6:** Storage Engine (persistência)
- ⏳ **FASE 7:** Executor básico (CREATE, INSERT, SELECT)
- ⏳ **FASE 8:** WHERE clause e expressões

### Componentes Implementados

#### ✅ Shell Interativo (REPL)
- Loop Read-Eval-Print funcional
- Meta comandos (`.help`, `.exit`)
- Buffer multi-linha
- Tratamento de erros

#### ✅ Analisador Léxico (Lexer)
- 50+ keywords SQL reconhecidas
- Tokenização de identificadores, números, strings
- Operadores relacionais e aritméticos
- Comentários (-- e /* */)
- Detecção de erros léxicos
- Organização modular em `src/lexer/scanner/`

---

## 📚 Documentação Adicional

- [doc/ARCHITECTURE.md](doc/ARCHITECTURE.md) — Arquitetura detalhada do sistema
- [doc/COMPONENTS.md](doc/COMPONENTS.md) — Documentação de componentes específicos
- [WORKFLOW.md](WORKFLOW.md) — Roadmap e workflow de desenvolvimento
- [README.md](README.md) — Visão geral do projeto

---

## 🤝 Contribuindo

1. Fork o repositório
2. Crie uma branch: `git checkout -b feature/nome`
3. Commit: `git commit -m "feat: descrição"`
4. Push: `git push origin feature/nome`
5. Abra um Pull Request

---

## 📝 Licença

MIT License — Projeto educacional para portfólio técnico.

---

**Autor:** Samuel Cotinguiba  
**Repositório:** https://github.com/samuelcotinguiba/projeto.server.cplus  
**Versão:** 0.1.0

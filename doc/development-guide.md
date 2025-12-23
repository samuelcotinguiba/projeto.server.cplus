# 👨‍💻 Guia de Desenvolvimento

## Visão Geral

Este guia orienta desenvolvedores (incluindo você no futuro) sobre como trabalhar no projeto MiniQL, entender sua estrutura, e contribuir efetivamente.

---

## 🚀 Setup Inicial

### Pré-requisitos

```bash
# Compilador C++17
g++ --version       # GCC 7+ ou
clang++ --version   # Clang 5+

# Build tools
make --version      # GNU Make
cmake --version     # CMake 3.15+

# Git
git --version
```

### Clone e Build

```bash
# Clone
git clone https://github.com/samuelcotinguiba/projeto.server.cplus.git
cd projeto.server.cplus

# Build com Makefile (rápido)
make

# Ou com CMake (profissional)
mkdir build && cd build
cmake ..
make

# Executar
./miniql
```

---

## 📂 Estrutura de Código

```
projeto.server.cplus/
├── src/                    # Código fonte (.cpp)
│   ├── main.cpp           # Entry point
│   ├── shell/
│   │   └── repl.cpp       # ✅ Shell interativo
│   ├── lexer/             # ⏳ Tokenização SQL
│   ├── parser/            # ⏳ Análise sintática
│   ├── ast/               # ⏳ Árvore sintática
│   ├── executor/          # ⏳ Motor de execução
│   ├── storage/           # ⏳ Persistência
│   ├── catalog/           # ⏳ Schemas
│   └── common/            # ⏳ Utilitários
│
├── include/               # Headers (.h)
│   ├── shell/
│   │   └── repl.h         # ✅ Interface do REPL
│   ├── lexer/
│   ├── parser/
│   ├── ast/
│   ├── executor/
│   ├── storage/
│   ├── catalog/
│   └── common/
│
├── tests/                 # Testes unitários
│   ├── lexer_test.cpp
│   ├── parser_test.cpp
│   └── ...
│
├── doc/                   # Documentação detalhada
│   ├── README.md          # Índice da documentação
│   ├── architecture.md    # Arquitetura completa
│   ├── shell-implementation.md
│   ├── technical-decisions.md
│   └── development-guide.md  # Este arquivo
│
├── docs/                  # Docs adicionais
├── data/                  # Database files (gerados)
│
├── CMakeLists.txt         # Build config (CMake)
├── Makefile               # Build config (Make)
├── .gitignore
├── README.md              # README principal
├── WORKFLOW.md            # Workflow de desenvolvimento
├── QUICKSTART.md          # Guia rápido
└── demo.sh                # Script de demonstração
```

---

## 🏗️ Workflow de Desenvolvimento

### Processo de Desenvolvimento

```
1. Planejamento
   ├─ Definir feature/bug
   ├─ Criar issue no GitHub
   └─ Atualizar WORKFLOW.md

2. Branch
   ├─ git checkout -b feature/nome-da-feature
   └─ git checkout -b fix/nome-do-bug

3. Implementação
   ├─ Escrever código
   ├─ Seguir convenções (ver abaixo)
   └─ Testar localmente

4. Commit
   ├─ git add <arquivos>
   ├─ git commit -m "tipo: mensagem"
   └─ Mensagem clara e descritiva

5. Push & PR
   ├─ git push origin feature/nome
   ├─ Criar Pull Request
   └─ Aguardar review

6. Merge
   ├─ Squash commits se necessário
   ├─ Merge para main
   └─ Deletar branch
```

---

## 📝 Convenções de Código

### Naming Conventions

```cpp
// Classes: PascalCase
class TableStorage {};
class SelectStmt {};

// Funções/Métodos: camelCase
void processSQLCommand(const std::string& sql);
bool isKeyword(const std::string& word);

// Variáveis: snake_case
int row_count;
std::string table_name;
bool is_running;

// Membros privados: snake_case com trailing _
class REPL {
private:
    bool running_;
    std::string buffer_;
};

// Constantes: UPPER_CASE
const int MAX_TABLE_NAME_LENGTH = 64;
const char* DEFAULT_DATABASE_PATH = "./data";

// Namespaces: lowercase
namespace miniql {
namespace storage {
    // ...
}
}

// Arquivos: snake_case
// repl.cpp, repl.h
// table_storage.cpp, table_storage.h
// expression_evaluator.cpp
```

### Formatação

```cpp
// Indentação: 4 espaços (não tabs)
void function() {
    if (condition) {
        doSomething();
    }
}

// Chaves: K&R style (abertura na mesma linha)
class MyClass {
public:
    void method() {
        if (condition) {
            // ...
        } else {
            // ...
        }
    }
};

// Espaços ao redor de operadores
int sum = a + b;
bool result = (x == 10) && (y < 5);

// Sem espaço antes de ( em chamadas de função
function(arg1, arg2);

// Espaço antes de ( em keywords
if (condition) {}
while (running) {}
for (int i = 0; i < n; i++) {}

// Ponteiros e referências: * e & junto ao tipo
int* ptr;
const std::string& ref;
```

### Headers

```cpp
// Ordem de includes:
// 1. Header correspondente (se for .cpp)
// 2. Headers do projeto
// 3. Headers da STL
// 4. Headers externos

// repl.cpp
#include "shell/repl.h"           // 1. Correspondente

#include "lexer/lexer.h"          // 2. Projeto
#include "parser/parser.h"

#include <iostream>               // 3. STL
#include <string>
#include <vector>

#include <boost/algorithm.hpp>    // 4. Externo (se tiver)

// Include guards: MINIQL_<PATH>_<FILE>_H
// include/shell/repl.h
#ifndef MINIQL_SHELL_REPL_H
#define MINIQL_SHELL_REPL_H
// ...
#endif // MINIQL_SHELL_REPL_H
```

### Comentários

```cpp
// Comentários explicam "por quê", não "o quê"

// ❌ Ruim (óbvio)
int count = 0;  // Inicializa count com 0

// ✅ Bom (contexto)
int count = 0;  // Precisa começar em 0 para lógica de paginação

// Comentários de documentação (para headers públicos)
/**
 * Executa um comando SQL e retorna o resultado.
 * 
 * @param sql String SQL completa terminada em ';'
 * @return ResultSet contendo rows resultantes
 * @throws ParserError se SQL for inválido
 * @throws ExecutorError se execução falhar
 */
ResultSet execute(const std::string& sql);

// TODOs sempre com contexto
// TODO(samuel): Implementar suporte a JOINs na FASE 9
// TODO: Otimizar scan com índice quando disponível
```

---

## 🧪 Testes

### Estrutura de Testes

```cpp
// tests/exemplo_test.cpp
#include "modulo/classe.h"
#include <cassert>
#include <iostream>

void test_funcionalidade_basica() {
    Classe obj;
    assert(obj.metodo() == valor_esperado);
    std::cout << "✓ test_funcionalidade_basica\n";
}

void test_caso_especial() {
    // ...
}

int main() {
    test_funcionalidade_basica();
    test_caso_especial();
    std::cout << "Todos os testes passaram!\n";
    return 0;
}
```

### Rodando Testes

```bash
# Compilar teste específico
g++ -std=c++17 -I./include tests/lexer_test.cpp src/lexer/lexer.cpp -o test_lexer

# Rodar
./test_lexer

# Futuro: Suite de testes automatizada
make test
```

---

## 🔧 Build & Debug

### Build Rápido (Desenvolvimento)

```bash
# Build completo
make

# Build debug (com símbolos)
make debug

# Build otimizado
make release

# Limpar
make clean

# Rebuild completo
make rebuild

# Build + run
make run
```

### Build Profissional (CMake)

```bash
# Setup
mkdir build && cd build

# Configure
cmake ..

# Build
make

# Install (opcional)
sudo make install

# Build específico
cmake --build . --target miniql

# Limpar
make clean
rm -rf build/
```

### Debugging

```bash
# Compilar com debug symbols
make debug

# Rodar com GDB
gdb ./miniql
(gdb) break main
(gdb) run
(gdb) next
(gdb) print variavel
(gdb) backtrace

# Ou com LLDB (macOS)
lldb ./miniql
(lldb) breakpoint set --name main
(lldb) run
(lldb) step
(lldb) print variavel
(lldb) bt
```

### Profiling

```bash
# Compilar com profiling
g++ -std=c++17 -pg -I./include src/**/*.cpp -o miniql

# Rodar
./miniql < test_queries.sql

# Gerar relatório
gprof miniql gmon.out > analysis.txt
```

---

## 📊 Git Workflow

### Commits Semânticos

```bash
# Formato: tipo(escopo): mensagem

# Tipos:
feat:     Nova funcionalidade
fix:      Correção de bug
docs:     Documentação
style:    Formatação (não muda lógica)
refactor: Refatoração
test:     Adicionar/modificar testes
chore:    Tarefas de manutenção (build, etc)
perf:     Melhoria de performance

# Exemplos:
git commit -m "feat(lexer): implement token recognition"
git commit -m "fix(parser): handle empty statements"
git commit -m "docs(architecture): add lexer diagram"
git commit -m "test(storage): add row serialization tests"
git commit -m "refactor(repl): extract buffer management"
git commit -m "chore: update CMakeLists.txt for new files"
```

### Branching Strategy

```bash
# Main branch
main         # Código estável, sempre funcional

# Feature branches
feature/lexer-implementation
feature/parser-ast
feature/storage-engine

# Bugfix branches
fix/repl-buffer-overflow
fix/lexer-string-escape

# Criar branch
git checkout -b feature/nome

# Trabalhar
git add .
git commit -m "feat: implementação"

# Push
git push origin feature/nome

# Merge (após review)
git checkout main
git merge feature/nome
git branch -d feature/nome
```

---

## 🎯 Implementando Nova Feature

### Checklist Completo

```
□ 1. Planejamento
   □ Ler WORKFLOW.md para entender a fase
   □ Revisar architecture.md para contexto
   □ Definir interfaces (headers)
   □ Listar dependências

□ 2. Estrutura
   □ Criar arquivos .h em include/
   □ Criar arquivos .cpp em src/
   □ Atualizar CMakeLists.txt se necessário
   □ Atualizar Makefile se necessário

□ 3. Implementação
   □ Escrever header com documentação
   □ Implementar funcionalidade
   □ Seguir convenções de código
   □ Adicionar error handling

□ 4. Integração
   □ Conectar com camadas adjacentes
   □ Atualizar interfaces existentes
   □ Testar integração

□ 5. Testes
   □ Escrever testes unitários
   □ Escrever testes de integração
   □ Testar casos extremos
   □ Verificar memory leaks (valgrind)

□ 6. Documentação
   □ Comentar código não-óbvio
   □ Atualizar doc/ se necessário
   □ Atualizar README.md se feature é visível
   □ Atualizar WORKFLOW.md status

□ 7. Commit
   □ Git add apenas arquivos relevantes
   □ Commit com mensagem semântica
   □ Push para branch

□ 8. Review
   □ Criar Pull Request
   □ Descrever mudanças
   □ Aguardar review (se for projeto em equipe)

□ 9. Merge
   □ Merge para main
   □ Deletar branch feature
   □ Marcar fase como concluída em WORKFLOW.md
```

---

## 🐛 Debugging Common Issues

### Erro de Compilação: Undefined Reference

```bash
# Problema
undefined reference to `miniql::Lexer::tokenize()'

# Causa
Arquivo .cpp não está sendo compilado

# Solução
# Adicionar ao Makefile:
SOURCES += src/lexer/lexer.cpp

# Ou ao CMakeLists.txt:
add_executable(miniql 
    src/main.cpp
    src/lexer/lexer.cpp  # <- adicionar
)
```

### Segmentation Fault

```bash
# Usar GDB para encontrar
make debug
gdb ./miniql
(gdb) run
# Crash ocorre
(gdb) backtrace   # Ver stack trace
(gdb) frame 0     # Ver frame específico
(gdb) print ptr   # Ver valor de ponteiro

# Verificar memory leaks
valgrind --leak-check=full ./miniql
```

### Headers não encontrados

```bash
# Problema
fatal error: 'shell/repl.h' file not found

# Causa
Include path não configurado

# Solução Makefile
CXXFLAGS = -I./include  # <- adicionar

# Solução CMake
include_directories(${CMAKE_SOURCE_DIR}/include)
```

---

## 📚 Recursos de Aprendizado

### Conceitos Essenciais

**Compiladores:**
- [Crafting Interpreters](https://craftinginterpreters.com/)
- [Dragon Book (Compilers: Principles, Techniques)](https://www.amazon.com/Compilers-Principles-Techniques-Tools-2nd/dp/0321486811)

**Database Internals:**
- [Database Internals (Alex Petrov)](https://www.databse-internals.com/)
- [Let's Build a Simple Database](https://cstack.github.io/db_tutorial/)
- [SQLite Internals](https://www.sqlite.org/arch.html)

**C++ Moderno:**
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Effective Modern C++ (Scott Meyers)](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/)

---

## ✅ Code Review Checklist

### Para Revisor

```
□ Código compila sem warnings
□ Segue convenções de naming
□ Sem memory leaks óbvios
□ Error handling adequado
□ Código comentado quando necessário
□ Testes incluídos
□ Documentação atualizada
□ Commit message claro
```

### Para Autor

```
□ Self-review antes de PR
□ Remover código comentado/debug
□ Remover TODOs resolvidos
□ Verificar formatação
□ Rodar testes localmente
□ Atualizar WORKFLOW.md
```

---

## 🚀 Próximos Passos

### Para Iniciar Contribuindo

1. **Familiarize-se:**
   - Ler [README.md](../README.md)
   - Ler [WORKFLOW.md](../WORKFLOW.md)
   - Ler [architecture.md](architecture.md)

2. **Setup:**
   - Clone repositório
   - Build e teste localmente
   - Rode demo.sh

3. **Escolha tarefa:**
   - Ver WORKFLOW.md para próximas fases
   - Procurar TODO no código
   - Propor nova feature

4. **Implemente:**
   - Seguir checklist acima
   - Pedir ajuda se travar

---

## 💡 Dicas de Produtividade

### Aliases Úteis

```bash
# Adicionar ao ~/.bashrc ou ~/.zshrc

# Build rápido
alias mb='make && ./miniql'

# Build debug e rodar com GDB
alias md='make debug && gdb ./miniql'

# Git shortcuts
alias gs='git status'
alias gd='git diff'
alias gc='git commit'
alias gp='git push'

# Limpar e rebuild
alias mcr='make clean && make release'
```

### VS Code Setup

```json
// .vscode/settings.json
{
    "C_Cpp.default.includePath": ["${workspaceFolder}/include"],
    "C_Cpp.default.cppStandard": "c++17",
    "files.associations": {
        "*.h": "cpp"
    }
}

// .vscode/tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "make",
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

---

## 📞 Suporte

### Onde Pedir Ajuda

- **Issues GitHub:** Para bugs e features
- **Documentação:** Sempre consultar `doc/` primeiro
- **Código:** Ler implementações similares como referência

---

**Atualizado:** 23/12/2025  
**Versão:** 1.0  
**Mantido por:** Samuel Cotinguiba

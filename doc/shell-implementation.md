# 🖥️ Shell/REPL — Implementação Detalhada

## Visão Geral

O Shell (REPL - Read-Eval-Print Loop) é a **interface interativa** do MiniQL, similar ao `sqlite3`, `psql` ou `mysql` client. É a primeira camada de contato do usuário com o database engine.

---

## 📁 Arquivos

- **Header:** [`include/shell/repl.h`](../include/shell/repl.h)
- **Implementação:** [`src/shell/repl.cpp`](../src/shell/repl.cpp)
- **Entry Point:** [`src/main.cpp`](../src/main.cpp)

---

## 🎯 Responsabilidades

1. **Loop Interativo** — Ler comandos do usuário continuamente
2. **Buffer Management** — Acumular comandos multi-linha até `;`
3. **Meta Comandos** — Processar comandos especiais (`.help`, `.exit`)
4. **SQL Forwarding** — Enviar comandos SQL completos para o engine
5. **Result Display** — Formatar e exibir resultados
6. **Error Handling** — Tratar EOF, comandos inválidos, erros do engine

---

## 🏗️ Estrutura da Classe

```cpp
namespace miniql {

class REPL {
public:
    REPL();                          // Construtor
    ~REPL();                         // Destrutor
    void run();                      // Loop principal

private:
    // Processamento de comandos
    bool processMetaCommand(const std::string& command);
    void processSQLCommand(const std::string& sql);
    
    // I/O
    std::string readLine(const std::string& prompt);
    
    // UI
    void printWelcome();
    void printHelp();
    
    // Estado
    bool running_;                   // Flag de controle do loop
};

} // namespace miniql
```

---

## 🔄 Fluxo de Execução

### Diagrama do Loop Principal

```
┌─────────────────────────────────────────────┐
│              run() inicia                    │
└───────────────────┬─────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────┐
│         printWelcome()                       │
│  "MiniQL v0.1.0 - Mini SQL Database Engine" │
└───────────────────┬─────────────────────────┘
                    │
                    ▼
           ┌────────────────┐
           │ while(running_)│ ◀───────────────┐
           └────────┬───────┘                 │
                    │                         │
                    ▼                         │
    ┌───────────────────────────┐            │
    │ buffer vazio?             │            │
    └───────┬───────────────────┘            │
            │                                │
       ┌────┴────┐                          │
       │  sim    │   não                    │
       ▼         ▼                          │
 prompt =   prompt =                        │
"miniql> " "     -> "                       │
       │         │                          │
       └────┬────┘                          │
            │                               │
            ▼                               │
   ┌────────────────┐                      │
   │  readLine()    │                      │
   │ (stdin input)  │                      │
   └────────┬───────┘                      │
            │                               │
            ▼                               │
   ┌─────────────────┐                     │
   │ linha vazia?    │                     │
   └────┬────────────┘                     │
        │                                  │
   ┌────┴─────┐                           │
   │ sim  │ não                            │
   ▼     ▼                                │
continue  │                                │
   │      │                                │
   └──────┤                                │
          │                                │
          ▼                                │
   ┌─────────────────┐                    │
   │ começa com '.'? │                    │
   │ e buffer vazio? │                    │
   └────┬────────────┘                    │
        │                                 │
   ┌────┴─────┐                          │
   │ sim │ não                            │
   ▼     ▼                                │
processMetaCommand()                      │
   │     │                                │
   │     │ adiciona ao buffer             │
   │     ▼                                │
   │  termina com ';'?                    │
   │     │                                │
   │  ┌──┴───┐                            │
   │  │sim│não│                           │
   │  ▼   │  │                            │
   │ processSQLCommand()                  │
   │  │   │  │                            │
   │  │   │  └────────────────────────────┘
   │  │   │
   │  │   └── continue loop
   │  │
   │  └─── limpa buffer
   │
   └────────────────────────────────────┐
                                        │
                                        ▼
                                 ┌──────────────┐
                                 │ running_ =   │
                                 │   false      │
                                 └──────┬───────┘
                                        │
                                        ▼
                                    [EXIT]
```

---

## 💻 Implementação Detalhada

### 1. Construtor e Destrutor

```cpp
REPL::REPL() : running_(true) {}

REPL::~REPL() {}
```

**Análise:**
- `running_` inicializado como `true` para permitir o loop
- Destrutor vazio (sem recursos dinâmicos para liberar no momento)
- Futuro: Conexão com database engine será inicializada aqui

---

### 2. Loop Principal (`run()`)

```cpp
void REPL::run() {
    printWelcome();
    
    std::string buffer;
    
    while (running_) {
        // Define prompt contextual
        std::string prompt = buffer.empty() ? "miniql> " : "     -> ";
        std::string line = readLine(prompt);
        
        // Ignora linhas vazias sem buffer
        if (line.empty() && buffer.empty()) {
            continue;
        }
        
        // Meta comandos têm prioridade
        if (line[0] == '.' && buffer.empty()) {
            if (!processMetaCommand(line)) {
                continue;
            }
        } else {
            // Acumula no buffer
            buffer += line + " ";
            
            // Detecta fim de comando
            if (!line.empty() && line.back() == ';') {
                // Remove ; e whitespace
                buffer.pop_back();
                buffer.erase(0, buffer.find_first_not_of(" \t\n\r"));
                buffer.erase(buffer.find_last_not_of(" \t\n\r") + 1);
                
                if (!buffer.empty()) {
                    processSQLCommand(buffer);
                }
                
                buffer.clear();
            }
        }
    }
}
```

**Decisões de Design:**

#### Prompt Contextual
```cpp
std::string prompt = buffer.empty() ? "miniql> " : "     -> ";
```
- `miniql>` quando iniciando novo comando
- `     ->` quando continuando comando multi-linha
- UX similar ao PostgreSQL `psql`

#### Buffer Management
```cpp
buffer += line + " ";
```
- Adiciona espaço entre linhas para evitar tokens colados
- Exemplo sem espaço: `SELECT id\nFROM users` → `SELECT idFROM users` ❌
- Exemplo com espaço: `SELECT id FROM users` ✅

#### Detecção de Comando Completo
```cpp
if (!line.empty() && line.back() == ';') {
```
- Semicolon é obrigatório em SQL
- Permite comandos multi-linha naturalmente
- Padrão em todos os clients SQL

#### Limpeza de Whitespace
```cpp
buffer.erase(0, buffer.find_first_not_of(" \t\n\r"));
buffer.erase(buffer.find_last_not_of(" \t\n\r") + 1);
```
- Remove espaços/tabs/newlines das pontas
- Previne comandos vazios (só espaços)
- `find_first_not_of` retorna primeiro char não-whitespace
- `find_last_not_of` retorna último char não-whitespace

---

### 3. Processamento de Meta Comandos

```cpp
bool REPL::processMetaCommand(const std::string& command) {
    if (command == ".exit" || command == ".quit") {
        std::cout << "Goodbye!\n";
        running_ = false;
        return false;
    }
    else if (command == ".help") {
        printHelp();
        return false;
    }
    else if (command == ".tables") {
        std::cout << "No tables yet.\n";
        std::cout << "(Database engine not implemented)\n";
        return false;
    }
    else if (command.substr(0, 7) == ".schema") {
        std::cout << "Schema command not implemented yet.\n";
        return false;
    }
    else {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Type .help for available commands.\n";
        return false;
    }
    
    return true;
}
```

**Meta Comandos Implementados:**

| Comando | Ação | Status |
|---------|------|--------|
| `.exit` | Sai do programa | ✅ Funcional |
| `.quit` | Alias para `.exit` | ✅ Funcional |
| `.help` | Exibe ajuda | ✅ Funcional |
| `.tables` | Lista tabelas | 🔄 Placeholder |
| `.schema <table>` | Mostra schema | 🔄 Placeholder |

**Detalhes de Implementação:**

#### Exit/Quit
```cpp
running_ = false;
```
- Define flag que controla o loop
- Próxima iteração do `while(running_)` não executa
- Mensagem amigável antes de sair

#### Schema Command
```cpp
command.substr(0, 7) == ".schema"
```
- Detecta comando mesmo com argumentos (`.schema users`)
- Placeholder para futura integração com Catalog
- Retornará schema completo da tabela especificada

#### Error Handling
```cpp
std::cout << "Unknown command: " << command << "\n";
```
- Comandos inválidos não crasham o programa
- Mensagem de erro clara
- Sugere usar `.help`

---

### 4. Processamento de SQL

```cpp
void REPL::processSQLCommand(const std::string& sql) {
    std::cout << "SQL Command received: " << sql << "\n";
    std::cout << "(SQL execution not implemented yet)\n";
    
    // TODO: Integração futura
    // 1. Lexer (tokenização)
    // 2. Parser (geração de AST)
    // 3. Executor (execução)
}
```

**Status Atual:** Placeholder que exibe o comando recebido

**Integração Futura (FASE 3+):**
```cpp
void REPL::processSQLCommand(const std::string& sql) {
    try {
        // FASE 3: Lexer
        Lexer lexer(sql);
        auto tokens = lexer.tokenize();
        
        // FASE 4: Parser
        Parser parser(tokens);
        auto ast = parser.parse();
        
        // FASE 7: Executor
        Executor executor(catalog_, storage_);
        auto result = executor.execute(ast.get());
        
        // Display result
        displayResult(result);
        
    } catch (const LexerError& e) {
        std::cerr << "Syntax error: " << e.what() << "\n";
    } catch (const ParserError& e) {
        std::cerr << "Parse error: " << e.what() << "\n";
    } catch (const ExecutorError& e) {
        std::cerr << "Execution error: " << e.what() << "\n";
    }
}
```

---

### 5. Leitura de Entrada

```cpp
std::string REPL::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::cout.flush();
    
    std::string line;
    if (!std::getline(std::cin, line)) {
        // EOF (Ctrl+D no Unix, Ctrl+Z no Windows)
        running_ = false;
        std::cout << "\n";
        return "";
    }
    
    // Remove whitespace das pontas
    line.erase(0, line.find_first_not_of(" \t\n\r"));
    line.erase(line.find_last_not_of(" \t\n\r") + 1);
    
    return line;
}
```

**Análise Detalhada:**

#### Flush do Prompt
```cpp
std::cout.flush();
```
- Força exibição imediata do prompt
- Sem flush, prompt pode não aparecer até `\n`
- Importante para UX responsiva

#### EOF Handling
```cpp
if (!std::getline(std::cin, line)) {
    running_ = false;
    std::cout << "\n";
    return "";
}
```
- `std::getline` retorna `false` em EOF
- EOF: Ctrl+D (Unix/Linux/macOS) ou Ctrl+Z (Windows)
- Define `running_ = false` para sair gracefully
- `\n` adicional para cursor não ficar na mesma linha

#### Trimming
```cpp
line.erase(0, line.find_first_not_of(" \t\n\r"));
line.erase(line.find_last_not_of(" \t\n\r") + 1);
```
- Remove espaços, tabs, newlines das pontas
- Previne comandos mal formatados
- `"  SELECT * "` → `"SELECT *"`

---

### 6. Interface do Usuário

#### Welcome Message
```cpp
void REPL::printWelcome() {
    std::cout << "MiniQL v0.1.0 - Mini SQL Database Engine\n";
    std::cout << "Type \".help\" for usage hints.\n";
    std::cout << "Type \".exit\" to quit.\n\n";
}
```

**Output:**
```
MiniQL v0.1.0 - Mini SQL Database Engine
Type ".help" for usage hints.
Type ".exit" to quit.

miniql> 
```

#### Help Message
```cpp
void REPL::printHelp() {
    std::cout << "\nMiniQL Meta Commands:\n";
    std::cout << "  .help              Show this help message\n";
    std::cout << "  .exit              Exit the program\n";
    std::cout << "  .quit              Exit the program\n";
    std::cout << "  .tables            List all tables\n";
    std::cout << "  .schema <table>    Show schema of a table\n";
    std::cout << "\nSQL Commands (in development):\n";
    std::cout << "  CREATE TABLE name (col1 INT, col2 TEXT);\n";
    std::cout << "  INSERT INTO name VALUES (1, 'text');\n";
    std::cout << "  SELECT * FROM name;\n";
    std::cout << "  SELECT col FROM name WHERE col = value;\n";
    std::cout << "  DELETE FROM name WHERE col = value;\n";
    std::cout << "\nNote: SQL commands must end with semicolon (;)\n\n";
}
```

**Decisão de Design:**
- Help completo mas conciso
- Mostra roadmap SQL (commands "in development")
- Lembra usuário sobre `;` obrigatório
- Formatação alinhada para legibilidade

---

## 🧪 Casos de Teste

### Teste 1: Comando Simples
```
Input:  SELECT * FROM users;
Buffer: "SELECT * FROM users"
Output: SQL Command received: SELECT * FROM users
```

### Teste 2: Comando Multi-linha
```
Input linha 1: SELECT *
Buffer:        "SELECT * "
Prompt:        "     -> "

Input linha 2: FROM users
Buffer:        "SELECT * FROM users "
Prompt:        "     -> "

Input linha 3: WHERE id = 1;
Buffer:        "SELECT * FROM users WHERE id = 1"
Output:        SQL Command received: SELECT * FROM users WHERE id = 1
```

### Teste 3: Meta Comando
```
Input:  .help
Output: [Help message completa]
Prompt: miniql>  (buffer permanece vazio)
```

### Teste 4: EOF (Ctrl+D)
```
Input:  [Ctrl+D]
Output: [newline]
        Goodbye!
Exit:   Program termina
```

### Teste 5: Comando Inválido
```
Input:  .invalid
Output: Unknown command: .invalid
        Type .help for available commands.
Prompt: miniql>
```

### Teste 6: Linha Vazia
```
Input:  [Enter]
Buffer: (vazio)
Output: (nenhum)
Prompt: miniql>  (volta imediatamente)
```

---

## 🎨 Experiência do Usuário

### Exemplo de Sessão Completa

```
$ ./miniql
MiniQL v0.1.0 - Mini SQL Database Engine
Type ".help" for usage hints.
Type ".exit" to quit.

miniql> .help

MiniQL Meta Commands:
  .help              Show this help message
  .exit              Exit the program
  .quit              Exit the program
  .tables            List all tables
  .schema <table>    Show schema of a table

SQL Commands (in development):
  CREATE TABLE name (col1 INT, col2 TEXT);
  INSERT INTO name VALUES (1, 'text');
  SELECT * FROM name;
  SELECT col FROM name WHERE col = value;
  DELETE FROM name WHERE col = value;

Note: SQL commands must end with semicolon (;)

miniql> CREATE TABLE users (id INT, name TEXT);
SQL Command received: CREATE TABLE users (id INT, name TEXT)
(SQL execution not implemented yet)

miniql> INSERT INTO users
     -> VALUES (1, 'Ana');
SQL Command received: INSERT INTO users VALUES (1, 'Ana')
(SQL execution not implemented yet)

miniql> SELECT *
     -> FROM users
     -> WHERE id = 1;
SQL Command received: SELECT * FROM users WHERE id = 1
(SQL execution not implemented yet)

miniql> .tables
No tables yet.
(Database engine not implemented)

miniql> .exit
Goodbye!
$
```

---

## 🔧 Melhorias Futuras

### 1. Histórico de Comandos (Readline)
```cpp
// Usar biblioteca readline/linenoise
#include <readline/readline.h>
#include <readline/history.h>

std::string REPL::readLine(const std::string& prompt) {
    char* input = readline(prompt.c_str());
    if (!input) {
        running_ = false;
        return "";
    }
    add_history(input);
    std::string result(input);
    free(input);
    return result;
}
```

**Benefícios:**
- Histórico com setas ↑↓
- Edição com ←→
- Autocompletion (Tab)

### 2. Syntax Highlighting
```cpp
// Colorir SQL keywords
std::cout << "\033[1;34mSELECT\033[0m * FROM users;\n";
```

### 3. Paginação de Resultados
```cpp
void REPL::displayResult(const ResultSet& result) {
    const int PAGE_SIZE = 20;
    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i] << "\n";
        if ((i + 1) % PAGE_SIZE == 0) {
            std::cout << "-- More -- (press Enter)";
            std::cin.get();
        }
    }
}
```

### 4. Modo Batch
```cpp
// Executar arquivo SQL
miniql --file script.sql

// Ou pipe
cat script.sql | miniql
```

### 5. Timing de Queries
```cpp
auto start = std::chrono::high_resolution_clock::now();
executor.execute(ast);
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
std::cout << "Query executed in " << duration.count() << "ms\n";
```

---

## 📊 Métricas

### Código
- **Linhas de código:** ~150
- **Funções públicas:** 2
- **Funções privadas:** 5
- **Dependências:** `<iostream>`, `<string>`, `<sstream>`, `<algorithm>`

### Complexidade
- **Complexidade ciclomática:** Baixa (~3-5 por função)
- **Nível de indentação:** Máximo 3-4 níveis
- **Manutenibilidade:** Alta (código claro e bem documentado)

---

## ✅ Checklist de Funcionalidades

- [x] Loop REPL básico
- [x] Prompt contextual (novo/continuação)
- [x] Buffer multi-linha
- [x] Detecção de `;` para fim de comando
- [x] Trimming de whitespace
- [x] Meta comando `.help`
- [x] Meta comando `.exit` / `.quit`
- [x] Meta comando `.tables` (placeholder)
- [x] Meta comando `.schema` (placeholder)
- [x] EOF handling (Ctrl+D/Ctrl+Z)
- [x] Erro para comandos inválidos
- [x] Placeholder para SQL processing
- [ ] Histórico de comandos (readline)
- [ ] Syntax highlighting
- [ ] Autocompletion
- [ ] Modo batch (arquivo input)
- [ ] Query timing

---

**Atualizado:** 23/12/2025  
**Status:** ✅ Implementação completa e funcional  
**Próximo:** Integração com Lexer (FASE 3)

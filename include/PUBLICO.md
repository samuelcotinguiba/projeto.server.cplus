# 🔓 Interface Pública do MiniQL

## 📖 O que é "Público" em C++?

Em C++, dividimos o código em duas partes:

- **`.h` (headers)** → **Interface Pública** - O que outros podem usar
- **`.cpp` (source)** → **Implementação Privada** - Como funciona por dentro

**Analogia:** 
- `.h` = Painel de controle de um carro (volante, pedais, alavanca)
- `.cpp` = Motor, transmissão, sistema elétrico (você não vê nem mexe)

---

## 📂 Por que `include/` separado de `src/`?

### Estrutura Atual:
```
include/lexer/scanner.h    ← PÚBLICO (interface, o que usar)
src/lexer/scanner.cpp      ← PRIVADO (implementação, como funciona)
```

### Motivos:

#### 1️⃣ **Compilação Eficiente**
```cpp
// Arquivo A inclui scanner.h
#include "lexer/scanner.h"

// Se mudar scanner.cpp → só recompila scanner.cpp
// Se mudar scanner.h → recompila A + scanner.cpp
```

✅ **Vantagem:** Compilação ~10x mais rápida em projetos grandes

---

#### 2️⃣ **Encapsulamento (Ocultar Implementação)**

**O QUE OUTRAS PARTES DO CÓDIGO PRECISAM SABER:**
```cpp
// include/lexer/scanner.h (PÚBLICO)
class Scanner {
public:
    Scanner(const std::string& source);      // Como criar
    std::vector<Token> scanTokens();         // Como usar
};
```

**O QUE ELAS NÃO PRECISAM SABER:**
```cpp
// src/lexer/scanner.cpp (PRIVADO)
void Scanner::scanNumber() {
    // 150 linhas de lógica complexa...
    // Ninguém precisa ver isso!
}
```

✅ **Vantagem:** Pode mudar a implementação sem quebrar nada

---

#### 3️⃣ **Distribuição como Biblioteca**

Se você quiser compartilhar o MiniQL:

```bash
# Você distribui:
include/         ← Headers públicos (.h)
libminiql.a      ← Código compilado (binário)

# Você NÃO distribui:
src/             ← Código-fonte privado (.cpp)
```

✅ **Vantagem:** Protege propriedade intelectual

---

## 🔍 Análise do `include/lexer/scanner.h`

### 1️⃣ Enums e Structs Públicos

#### `enum class TokenType`
```cpp
enum class TokenType {
    SELECT, INSERT, UPDATE, DELETE, ...
};
```

**Por que está em `.h`?**
- ✅ Outros módulos precisam saber quais tipos de tokens existem
- ✅ Parser vai usar: `if (token.type == TokenType::SELECT)`
- ✅ Não tem implementação, só definição de tipos

**Exemplo de uso externo:**
```cpp
// src/parser/parser.cpp
#include "lexer/scanner.h"

if (currentToken.type == TokenType::SELECT) {
    // processar SELECT...
}
```

---

#### `struct Token`
```cpp
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    Token(TokenType t, const std::string& lex, int ln, int col);
    std::string typeToString() const;
};
```

**Por que está em `.h`?**
- ✅ Parser precisa acessar `token.type`, `token.lexeme`
- ✅ Mensagens de erro precisam de `token.line`, `token.column`
- ✅ É uma estrutura de dados pública (todos precisam usar)

**Exemplo de uso externo:**
```cpp
// src/error/error_reporter.cpp
void reportError(const Token& token) {
    std::cout << "Erro na linha " << token.line 
              << ", coluna " << token.column << "\n";
}
```

---

### 2️⃣ Classe `Scanner` - Interface Pública

#### Métodos Públicos (em `.h`)
```cpp
class Scanner {
public:
    explicit Scanner(const std::string& source);  // Construtor
    std::vector<Token> scanTokens();               // Método principal
    const std::vector<std::string>& getErrors() const;
    bool hasErrors() const;
};
```

**Por que estão em `.h`?**
- ✅ São a **API pública** do Scanner
- ✅ Parser vai chamar: `scanner.scanTokens()`
- ✅ Shell vai chamar: `if (scanner.hasErrors())`

**Exemplo de uso externo:**
```cpp
// src/parser/parser.cpp
Scanner scanner(sqlCode);
std::vector<Token> tokens = scanner.scanTokens();

if (scanner.hasErrors()) {
    // mostrar erros...
}
```

---

#### Métodos Privados (em `.h`, mas marcados como `private:`)
```cpp
class Scanner {
private:
    void scanNumber();           // NÃO acessível de fora
    void scanString(char quote);
    void scanIdentifier();
    void scanComment();
    
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    // ...
};
```

**Por que estão em `.h` se são privados?**
- 🤔 **Paradoxo:** Estão declarados no `.h`, mas ninguém pode chamar!
- ✅ **Motivo técnico:** Compilador C++ precisa saber que existem
- ✅ **Implementação real:** Está em `src/lexer/scanner.cpp` e `src/lexer/scanner/`

**Ninguém pode fazer isso:**
```cpp
// ERRO DE COMPILAÇÃO!
Scanner scanner(code);
scanner.scanNumber();  // ❌ Erro: scanNumber() é private!
```

---

### 3️⃣ Funções Utilitárias Globais

```cpp
std::string tokenTypeToString(TokenType type);
bool isKeyword(TokenType type);
bool isOperator(TokenType type);
bool isDelimiter(TokenType type);
```

**Por que estão em `.h`?**
- ✅ São funções **auxiliares públicas**
- ✅ Outros módulos podem querer usar
- ✅ Implementação está em `src/lexer/scanner.cpp`

**Exemplo de uso externo:**
```cpp
// src/debug/token_printer.cpp
#include "lexer/scanner.h"

void printToken(const Token& token) {
    std::cout << tokenTypeToString(token.type) 
              << " (" << token.lexeme << ")\n";
}
```

---

## 📊 Comparação: O que vai em `.h` vs `.cpp`

| Item | `.h` (include/) | `.cpp` (src/) | Por quê? |
|------|-----------------|---------------|----------|
| **`enum class TokenType`** | ✅ SIM | ❌ NÃO | Outros precisam saber os tipos |
| **`struct Token`** | ✅ SIM | ❌ NÃO | Estrutura de dados pública |
| **`class Scanner` (declaração)** | ✅ SIM | ❌ NÃO | Interface pública |
| **Métodos públicos (declaração)** | ✅ SIM | ❌ NÃO | API pública |
| **Métodos privados (declaração)** | ✅ SIM | ❌ NÃO | Compilador precisa saber |
| **Implementação de métodos** | ❌ NÃO | ✅ SIM | Detalhes internos |
| **Funções auxiliares (declaração)** | ✅ SIM | ❌ NÃO | Pode ser usada externamente |
| **Implementação de funções** | ❌ NÃO | ✅ SIM | Detalhes internos |
| **Variáveis privadas** | ✅ SIM (declaração) | ✅ SIM (uso) | Compilador precisa alocar memória |

---

## 🎯 Resumo Visual

### **Arquivo `.h` = Contrato Público**
```cpp
// include/lexer/scanner.h
class Scanner {
public:
    // PROMESSA: Você pode chamar isso
    std::vector<Token> scanTokens();
    
private:
    // AVISO: Isso existe, mas você NÃO pode chamar
    void scanNumber();
};
```

### **Arquivo `.cpp` = Cumprimento do Contrato**
```cpp
// src/lexer/scanner.cpp
#include "lexer/scanner.h"

// CUMPRINDO A PROMESSA
std::vector<Token> Scanner::scanTokens() {
    // código real aqui...
}

// IMPLEMENTANDO O PRIVADO
void Scanner::scanNumber() {
    // código real aqui...
}
```

---

## 🔄 Fluxo de Uso

```
┌─────────────────────────────────────────┐
│  Outro módulo (ex: parser.cpp)         │
├─────────────────────────────────────────┤
│  #include "lexer/scanner.h"   ← Inclui │
│                                          │
│  Scanner scanner(code);       ← Usa     │
│  auto tokens = scanner.scanTokens();    │
└─────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────┐
│  include/lexer/scanner.h                │
├─────────────────────────────────────────┤
│  ✅ Vê a declaração da classe            │
│  ✅ Sabe que scanTokens() existe         │
│  ❌ NÃO vê a implementação               │
└─────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────┐
│  src/lexer/scanner.cpp (linkado)       │
├─────────────────────────────────────────┤
│  ✅ Implementação REAL do scanTokens()   │
│  ✅ Código executado em tempo de runtime │
└─────────────────────────────────────────┘
```

---

## ❓ Perguntas Frequentes

### **P: Por que não colocar tudo no `.cpp`?**
**R:** Porque outros arquivos não conseguiriam saber o que existe!

```cpp
// parser.cpp
Scanner scanner(code);  // ❌ ERRO: O que é Scanner?
```

---

### **P: Por que não colocar tudo no `.h`?**
**R:** Porque:
1. Compilação ficaria lenta (recompila tudo sempre)
2. Expõe implementação privada
3. Aumenta tempo de build em ~10x

---

### **P: Métodos privados precisam estar no `.h`?**
**R:** Sim, porque o compilador precisa saber:
- Quanta memória alocar para a classe
- Quais métodos existem (mesmo que privados)

Mas a **implementação** fica escondida no `.cpp`!

---

### **P: Posso acessar `scanner.scanNumber()` de fora?**
**R:** ❌ NÃO! É privado. Só métodos públicos são acessíveis.

```cpp
Scanner scanner(code);
scanner.scanTokens();   // ✅ OK - é público
scanner.scanNumber();   // ❌ ERRO - é privado!
```

---

## 🎓 Conclusão

**`include/lexer/scanner.h` contém:**
- ✅ Tipos públicos (`enum`, `struct`)
- ✅ Interface da classe (`class Scanner`)
- ✅ Declarações de métodos (públicos e privados)
- ✅ Funções auxiliares públicas
- ❌ **NÃO contém:** Implementação real do código

**`src/lexer/scanner.cpp` contém:**
- ✅ Implementação real de todos os métodos
- ✅ Lógica complexa escondida
- ✅ Detalhes internos

**Analogia Final:**
- `.h` = **Manual do usuário** de um carro (como usar)
- `.cpp` = **Manual de oficina** do carro (como funciona)

Você dirige o carro lendo o manual do usuário, não precisa saber como o motor funciona por dentro! 🚗

---

## 📚 Próximos Passos

Quando implementarmos o **Parser**, teremos:

```
include/parser/parser.h    ← Interface pública do Parser
src/parser/parser.cpp      ← Implementação privada
```

E o Parser vai **usar** o Scanner:
```cpp
// src/parser/parser.cpp
#include "lexer/scanner.h"  // ← Usa a interface pública!

Scanner scanner(code);
auto tokens = scanner.scanTokens();
```

Tudo conectado de forma limpa e organizada! 🎯

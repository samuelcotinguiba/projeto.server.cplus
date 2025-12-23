# Estrutura do Projeto MiniQL

## 📁 Pastas Principais

### `include/`
Headers (.h) - Declarações de classes e interfaces públicas

- **`include/lexer/`**
  - `scanner.h` - Token, TokenType enum, classe Scanner

- **`include/shell/`**
  - `repl.h` - Interface REPL (Read-Eval-Print Loop)

---

### `src/`
Implementações (.cpp) - Código-fonte do projeto

- **`main.cpp`** - Ponto de entrada principal do servidor

- **`src/shell/`**
  - `repl.cpp` - Loop interativo do shell

- **`src/lexer/`**
  - `scanner.cpp` - Implementação principal do scanner (scanTokens, scanToken)
  - `lexer_demo.cpp` - Programa de demonstração do lexer
  
  - **`src/lexer/scanner/`** - Funções especializadas de scanning:
    - `scan_number.cpp` - Reconhece números (inteiros e decimais)
    - `scan_string.cpp` - Reconhece strings ('...' ou "...")
    - `scan_identifier.cpp` - Reconhece identificadores e keywords
    - `scan_comment.cpp` - Reconhece comentários (-- e /* */)

---

### `doc/`
Documentação completa do projeto

- `GUIDE.md` - Guia de uso (compilação, comandos, troubleshooting)
- `COMPONENTS.md` - Documentação detalhada de cada componente
- `architecture.md` - Arquitetura geral do sistema
- `development-guide.md` - Guia para desenvolvedores
- `shell-implementation.md` - Detalhes da implementação do shell
- `technical-decisions.md` - Decisões técnicas do projeto
- `README.md` - Índice da documentação

---

### `tests/`
Testes unitários e de integração (a implementar)

---

## 🔧 Funções Principais

### Scanner (Analisador Léxico)

**scanner.cpp:**
- `scanTokens()` - Loop principal que processa todo o código-fonte
- `scanToken()` - Identifica um único token
- `advance()` - Avança para próximo caractere
- `peek()` / `peekNext()` - Olha caracteres sem consumir
- `match()` - Verifica e consome caractere esperado
- `isDigit()` / `isAlpha()` / `isAlphaNumeric()` - Classificação de caracteres
- `addToken()` - Adiciona token à lista
- `addError()` - Registra erro léxico

**scan_number.cpp:**
- `scanNumber()` - Processa números: 123, 45.67

**scan_string.cpp:**
- `scanString(char quote)` - Processa strings com aspas simples/duplas

**scan_identifier.cpp:**
- `scanIdentifier()` - Processa identificadores e keywords (case-insensitive)

**scan_comment.cpp:**
- `scanComment()` - Processa comentários de linha (--) e bloco (/* */)

---

## 📝 Arquivos de Configuração

- `Makefile` - Build system (targets: all, lexer-demo, clean)
- `CMakeLists.txt` - Configuração CMake
- `README.md` - Apresentação do projeto
- `WORKFLOW.md` - Fluxo de trabalho do desenvolvimento

---

## 🚀 Comandos Rápidos

```bash
# Compilar projeto principal
make

# Compilar e executar demo do lexer
make lexer-demo
./lexer_demo

# Executar projeto principal
make run

# Limpar arquivos de build
make clean
```

---

## 🎯 Estado Atual

✅ **COMPLETO:** Analisador Léxico (Scanner)
- 50+ keywords SQL
- Reconhecimento de tokens: números, strings, identificadores, operadores
- Detecção de erros com linha/coluna
- Suporte a comentários

🔄 **PRÓXIMO:** Analisador Sintático (Parser)
- Análise de estrutura SQL
- Construção de AST

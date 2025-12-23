# 🚀 Quick Start

## Build & Run

```bash
# Compilar (usando Makefile)
make

# Ou usando CMake
mkdir build && cd build
cmake ..
make

# Executar
./miniql

# Demo automatizada
./demo.sh
```

## Uso do Shell

```sql
miniql> .help              -- Mostra ajuda
miniql> CREATE TABLE users (id INT, name TEXT);
miniql> INSERT INTO users VALUES (1, 'Ana');
miniql> SELECT * FROM users;
miniql> .exit              -- Sair
```

## Status Atual

✅ **FASE 0:** Planejamento & Escopo concluído  
✅ **FASE 1:** Estrutura base criada  
✅ **FASE 2:** Shell interativo funcionando  
🎯 **FASE 3:** Lexer (próximo passo)  
⏳ **FASE 4:** Parser + AST  
⏳ **FASE 5-6:** Catalog + Storage  
⏳ **FASE 7-8:** Executor + WHERE  

**Progresso:** ~20% (2/12 fases concluídas)  

## Comandos Meta Disponíveis

- `.help` — Exibe ajuda
- `.exit` / `.quit` — Sair do programa
- `.tables` — Listar tabelas (ainda não implementado)
- `.schema <table>` — Ver schema de tabela (ainda não implementado)

## Próximos Passos

1. ✅ Shell REPL interativo
2. 🎯 Implementar Lexer (tokenização SQL)
3. 🎯 Implementar Parser (AST)
4. 🎯 Implementar Executor básico

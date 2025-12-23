# 🚀 Como Usar Este Projeto

## Para Desenvolvedores

### 1. Clone o Repositório
```bash
git clone https://github.com/samuelcotinguiba/projeto.server.cplus.git
cd projeto.server.cplus
```

### 2. Build
```bash
# Build simples (padrão)
make

# Build com debug symbols
make debug

# Build otimizado (release)
make release
```

### 3. Executar
```bash
# Rodar o shell interativo
./miniql

# Ou build + run direto
make run
```

### 4. Desenvolvimento
```bash
# Edite o código em qualquer editor
vim src/shell/repl.cpp
# ou
code src/shell/repl.cpp

# Recompile
make

# Teste
./miniql
```

### 5. Limpeza
```bash
# Limpar arquivos de build
make clean

# Rebuild completo
make rebuild
```

## Comandos do Shell

Dentro do `miniql`:

```sql
-- Meta comandos (começam com ponto)
.help              -- Mostra ajuda
.exit              -- Sai do programa
.tables            -- Lista tabelas (ainda não implementado)
.schema <table>    -- Mostra schema de tabela (ainda não implementado)

-- Comandos SQL (devem terminar com ponto-e-vírgula)
CREATE TABLE users (id INT, name TEXT);
INSERT INTO users VALUES (1, 'Ana');
SELECT * FROM users;
SELECT name FROM users WHERE id = 1;
DELETE FROM users WHERE id = 1;
```

## Demo Rápida

```bash
# Rode o script de demonstração
./demo.sh
```

## Estrutura do Projeto

```
projeto.server.cplus/
├── src/              # Código fonte (.cpp)
├── include/          # Headers (.h)
├── doc/              # Documentação detalhada
├── tests/            # Testes (futuro)
├── Makefile          # Build configuration
└── README.md         # Este arquivo
```

## Troubleshooting

### ❌ Erro: `command not found: make`
**Solução:** Instale build tools:
```bash
# macOS
xcode-select --install

# Linux (Ubuntu/Debian)
sudo apt install build-essential

# Linux (Fedora)
sudo dnf install gcc-c++ make
```

### ❌ Erro: `fatal error: 'shell/repl.h' file not found`
**Causa:** Tentou compilar arquivo individual ao invés de usar Makefile

**Solução:** Sempre use `make` no diretório raiz:
```bash
cd projeto.server.cplus
make
```

### ❌ Build lento
**Solução:** Use build incremental (padrão do Makefile). Apenas arquivos modificados são recompilados.

## Próximos Passos

Veja o [WORKFLOW.md](WORKFLOW.md) para entender as próximas fases de desenvolvimento:

- ✅ FASE 1-2: Shell interativo (concluído)
- 🔄 FASE 3: Lexer (próximo)
- ⏳ FASE 4: Parser + AST
- ⏳ FASE 5-6: Catalog + Storage
- ⏳ FASE 7-8: Executor + WHERE

## Documentação Completa

Para documentação técnica detalhada, veja a pasta [`doc/`](doc/):

- [Arquitetura](doc/architecture.md)
- [Implementação do Shell](doc/shell-implementation.md)
- [Decisões Técnicas](doc/technical-decisions.md)
- [Guia de Desenvolvimento](doc/development-guide.md)

## Contribuindo

1. Fork o repositório
2. Crie uma branch: `git checkout -b feature/nome`
3. Commit: `git commit -m "feat: descrição"`
4. Push: `git push origin feature/nome`
5. Abra um Pull Request

## Licença

MIT License — Projeto educacional para portfólio técnico.

---

**Autor:** Samuel Cotinguiba  
**Repositório:** https://github.com/samuelcotinguiba/projeto.server.cplus

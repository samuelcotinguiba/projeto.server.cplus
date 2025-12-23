# 📚 Documentação do MiniQL Database Engine

> Índice completo da documentação técnica do projeto

---

## 🎯 Começando

### Para Usuários
**[GUIDE.md](GUIDE.md)** — Guia completo de instalação, uso e troubleshooting
- Quick Start
- Compilação e Build
- Como usar o shell SQL
- Comandos disponíveis
- Demonstrações
- Troubleshooting

---

## 🏗️ Arquitetura e Design

### Visão Geral do Sistema
**[architecture.md](architecture.md)** — Arquitetura em camadas do MiniQL
- Diagrama de componentes
- Fluxo de dados
- Responsabilidades de cada camada
- Estrutura de arquivos
- Status de implementação

### Decisões Técnicas
**[technical-decisions.md](technical-decisions.md)** — Por que C++, C++17, Makefile, etc
- Justificativas de tecnologia
- Alternativas consideradas
- Trade-offs aceitos
- Padrões de design

---

## 🔧 Desenvolvimento

### Guia para Desenvolvedores
**[development-guide.md](development-guide.md)** — Como contribuir e desenvolver
- Setup do ambiente
- Estrutura de código
- Convenções de código
- Workflow de desenvolvimento
- Como adicionar novas features

---

## 📦 Componentes Específicos

### Shell Interativo (REPL)
**[shell-implementation.md](shell-implementation.md)** — Implementação detalhada do REPL
- Fluxo de execução
- Buffer multi-linha
- Meta comandos
- Tratamento de erros
- Código comentado

### Analisador Léxico (Lexer)
**[../src/lexer/README.md](../src/lexer/README.md)** — Documentação do Lexer
- Tokens suportados
- Padrões de reconhecimento
- Exemplos de tokenização
- Como usar

### Parser (Em Desenvolvimento)
**Status:** ⏳ Próxima fase

### Storage Engine (Planejado)
**Status:** ⏳ Futuro

---

## 📊 Roadmap e Progresso

**[../WORKFLOW.md](../WORKFLOW.md)** — Roadmap completo do projeto
- Fases de desenvolvimento (0-12)
- Checklist de tarefas
- Progresso atual (~25%)
- Próximos passos

---

## 📖 Estrutura da Documentação

```
doc/
├── README.md                    # Este arquivo (índice)
├── GUIDE.md                     # Guia completo do usuário
├── architecture.md              # Arquitetura do sistema
├── development-guide.md         # Guia para desenvolvedores
├── technical-decisions.md       # Decisões técnicas
└── shell-implementation.md      # Detalhes do REPL

src/
└── lexer/
    └── README.md                # Documentação do Lexer

/ (raiz)
├── README.md                    # Visão geral do projeto
└── WORKFLOW.md                  # Roadmap e fases
```

---

## 🔍 Como Navegar

### Se você quer:

**"Começar a usar o projeto"**
→ [GUIDE.md](GUIDE.md)

**"Entender a arquitetura"**
→ [architecture.md](architecture.md)

**"Saber por que certas decisões foram tomadas"**
→ [technical-decisions.md](technical-decisions.md)

**"Contribuir com código"**
→ [development-guide.md](development-guide.md)

**"Entender o REPL"**
→ [shell-implementation.md](shell-implementation.md)

**"Ver o progresso e roadmap"**
→ [../WORKFLOW.md](../WORKFLOW.md)

**"Aprender sobre o Lexer"**
→ [../src/lexer/README.md](../src/lexer/README.md)

---

## 📝 Convenções de Documentação

### Símbolos de Status
- ✅ **Implementado e testado**
- 🔄 **Em desenvolvimento**
- 🎯 **Próximo na fila**
- ⏳ **Planejado para o futuro**
- ❌ **Fora do escopo atual**

### Estrutura de Arquivos
Cada documento segue a estrutura:
1. **Visão Geral** — O que é e para que serve
2. **Conteúdo Principal** — Detalhes técnicos
3. **Exemplos** — Código e casos de uso
4. **Referências** — Links para outros docs

---

## 🤝 Contribuindo com Documentação

Se você encontrar algo confuso ou quiser adicionar conteúdo:

1. **Para correções pequenas:** Abra uma issue descrevendo o problema
2. **Para adições:** Fork → Edite → Pull Request
3. **Para sugestões:** Abra uma discussion no GitHub

### Boas Práticas:
- Use markdown com sintaxe clara
- Adicione exemplos de código quando apropriado
- Mantenha a linguagem simples e direta
- Use diagramas ASCII quando possível
- Referencie outros documentos quando relevante

---

**Última atualização:** Dezembro 2025  
**Versão:** 0.1.0  
**Autor:** Samuel Cotinguiba

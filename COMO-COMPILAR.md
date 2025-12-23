# 🔨 Como Compilar o MiniQL

## ✅ Compilação Rápida (Recomendado)

```bash
# 1. Entre na pasta do projeto
cd /Users/samuelcotinguiba/Desktop/projeto.server.cplus

# 2. Compile
make

# 3. Execute
./miniql
```

---

## 🔄 Recompilar do Zero

```bash
# Limpa tudo e recompila
make clean
make
```

---

## 🚀 Comandos Úteis

```bash
# Compilar versão debug (com símbolos para debugger)
make debug

# Compilar versão otimizada
make release

# Compilar e executar direto
make run

# Limpar arquivos temporários
make clean
```

---

## ❌ O QUE **NÃO** FAZER

```bash
# ❌ NUNCA compile arquivos individualmente
g++ src/shell/repl.cpp  # ERRADO - vai dar erro

# ❌ NUNCA compile pela extensão do VSCode (botão "Run")
# Use SEMPRE o Makefile: make
```

---

## 🐛 Erros que Aparecem no VSCode

Se você vê **linhas vermelhas** ou **erros no editor**, mas o código **compila com `make`**:

### Causa:
A extensão C/C++ do VSCode não está configurada corretamente.

### Solução:
**Ignore os erros do editor** se `make` funciona!

Ou configure o IntelliSense criando `.vscode/c_cpp_properties.json`:

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

---

## ✅ Como Verificar se Está Tudo OK

```bash
# Se este comando funciona, está tudo certo:
make clean && make

# Se aparecer "Build completo: ./miniql" → SUCESSO!
```

---

## 📝 Testando o Programa

```bash
# Execute
./miniql

# Teste comandos:
miniql> .help
miniql> SELECT * FROM users;
miniql> .exit
```

---

## 🆘 Problemas Comuns

### "command not found: make"
```bash
# Instale o Xcode Command Line Tools
xcode-select --install
```

### "fatal error: 'shell/repl.h' file not found"
```bash
# Você está compilando errado! Use:
make
# Não compile arquivos individuais
```

### "Permission denied: ./miniql"
```bash
# Dê permissão de execução
chmod +x miniql
```

---

## 📊 Status da Compilação

- ✅ **Última compilação:** Sucesso
- ✅ **Warnings:** 0
- ✅ **Erros:** 0
- ✅ **Executável:** `./miniql`

**Data:** 23/12/2025

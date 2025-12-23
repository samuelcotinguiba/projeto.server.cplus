#!/bin/bash

# Script de demonstração do MiniQL Shell

echo "=== DEMO: MiniQL Shell Interativo ==="
echo ""
echo "Testando comandos meta e entrada SQL..."
echo ""

# Simula entrada do usuário
{
    echo ".help"
    sleep 0.5
    echo ".tables"
    sleep 0.5
    echo "CREATE TABLE users (id INT, name TEXT);"
    sleep 0.5
    echo "INSERT INTO users VALUES (1, 'Ana');"
    sleep 0.5
    echo "SELECT * FROM users WHERE id = 1;"
    sleep 0.5
    echo ".exit"
} | ./miniql

echo ""
echo "=== Demo concluída! ==="

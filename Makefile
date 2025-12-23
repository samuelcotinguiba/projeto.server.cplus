# Makefile simplificado para desenvolvimento rápido
# Para build de produção, use CMake

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -I./include
LDFLAGS =

# Diretórios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = .

# Arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BIN_DIR)/miniql

# Regra principal
all: $(TARGET)

# Criar diretórios necessários
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/shell

# Linkagem
$(TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Build completo: $(TARGET)"

# Compilação dos objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rodar o programa
run: $(TARGET)
	./$(TARGET)

# Limpeza
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Limpeza completa"

# Rebuild completo
rebuild: clean all

# Debug build
debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean all

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all

.PHONY: all clean run rebuild debug release

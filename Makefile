# Makefile simplificado para desenvolvimento rápido
# Para build de produção, use CMake

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -I./include
LDFLAGS =

# Diretórios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = .

# Arquivos fonte (excluindo lexer_demo.cpp)
SOURCES = $(filter-out $(SRC_DIR)/lexer/lexer_demo.cpp, \
          $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/*.cpp))
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BIN_DIR)/miniql

# Lexer demo (compilação separada)
LEXER_DEMO_SOURCES = $(SRC_DIR)/lexer/lexer_demo.cpp $(SRC_DIR)/lexer/scanner.cpp $(wildcard $(SRC_DIR)/lexer/scanner/*.cpp)
LEXER_DEMO_TARGET = $(BIN_DIR)/lexer_demo

# Regra principal
all: $(TARGET)

# Criar diretórios necessários
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/shell
	@mkdir -p $(BUILD_DIR)/lexer
	@mkdir -p $(BUILD_DIR)/lexer/scanner

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

# Build do demo do lexer
lexer-demo: $(LEXER_DEMO_TARGET)

$(LEXER_DEMO_TARGET): $(LEXER_DEMO_SOURCES)
	$(CXX) $(CXXFLAGS) $(LEXER_DEMO_SOURCES) -o $(LEXER_DEMO_TARGET)
	@echo "Lexer demo compilado: $(LEXER_DEMO_TARGET)"

# Rodar demo do lexer
run-lexer-demo: $(LEXER_DEMO_TARGET)
	./$(LEXER_DEMO_TARGET)

# Limpeza
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(LEXER_DEMO_TARGET)
	@echo "Limpeza completa"

# Rebuild completo
rebuild: clean all

# Debug build
debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean all

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all

.PHONY: all clean run rebuild debug release lexer-demo run-lexer-demo

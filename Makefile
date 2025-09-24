# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Iinclude

# Каталоги
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DEP_DIR = obj

# Исходники и объектные файлы
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
DEP = $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$(SRC))

# Итоговый исполняемый файл
TARGET = $(BIN_DIR)/console-todo

# Цель по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Компиляция каждого .cpp в .o с генерацией зависимостей
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Подключение файлов зависимостей
-include $(DEP)

# Очистка проекта
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(DEP_DIR)

.PHONY: all clean

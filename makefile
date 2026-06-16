CC      = gcc
CFLAGS  = -pthread -std=c11 -Wall -Werror -Wpedantic
OBJ_DIR = obj
TARGET  = tinydb

SRCS = main.c parsing.c student.c query.c log.c handleinput.c command.c $(wildcard db.c)
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all main run tests clean makeclean

all: $(TARGET)

main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(TARGET)
	./$(TARGET)

tests: $(TARGET)
	mkdir -p logs
	python3 tests/run_tests.py

clean: makeclean

makeclean:
	rm -rf $(OBJ_DIR) $(TARGET)
	rm -rf logs/*
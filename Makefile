CC = gcc
AR = ar

CFLAGS = -O2 -DNDEBUG
ARFLAGS = rcs

SRC = src
INCLUDE = include
TESTS = tests
BUILD = build

ifeq ($(OS),Windows_NT)
    EXE_EXTENSION = .exe
else
	EXE_EXTENSION = .run
endif

SRCS = $(notdir $(wildcard $(SRC)/*.c))

ifeq ($(filter test, $(MAKECMDGOALS)), test)
    SRCS += $(notdir $(wildcard $(TESTS)/*.c))
    CFLAGS = -g -Wall -Wextra
endif

OBJS = $(patsubst %.c, $(BUILD)/%.o, $(SRCS))

all: libunistruct.a
test: test$(EXE_EXTENSION)

%.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

%$(EXE_EXTENSION): $(OBJS)
	$(CC) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) -c $< -o $@ $(CFLAGS) -I$(INCLUDE)

$(BUILD)/%.o: $(TESTS)/%.c | $(BUILD)
	$(CC) -c $< -o $@ $(CFLAGS) -I$(INCLUDE)
	
$(BUILD):
	mkdir $@
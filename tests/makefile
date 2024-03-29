CC = g++

CFLAGS = -Og -pipe -std=gnu++17 -Wpedantic -fprofile-arcs -ftest-coverage -Wall -Wextra -Werror -DDEBUG -g -ggdb $(shell sdl2-config --cflags)
LDLIBS = -pthread $(shell sdl2-config --libs)

LEXYACC_PATH = ../gen
INCLUDE_PATH = ../inc
LIB_PATH     = ../lib

TARGET       = tests
FILEXT       = cpp

SRCDIR       = ../src
OBJDIR       = obj

PATH_TO_EXE  = $(TARGET)


SRCSUBDIRS  := $(shell find $(SRCDIR) -type d)
INCSUBDIRS  := $(shell find $(INCLUDE_PATH) -type d)

SOURCES     := $(foreach dir, $(SRCSUBDIRS), $(wildcard $(dir)/*.$(FILEXT)))
INCLUDES    := $(foreach dir, $(INCSUBDIRS), $(wildcard $(dir)/*.h|*.hpp))
LIBS        := $(wildcard $(LIB_PATH)/*.h) $(wildcard $(LIB_PATH)/*.hpp)
OBJECTS0    := $(SOURCES:$(SRCDIR)/%.$(FILEXT)=$(OBJDIR)/%.o)
OBJECTS1    := $(filter-out $(OBJDIR)/main.o, $(OBJECTS0))

LEXSRC      := $(wildcard $(LEXYACC_PATH)/*.l)
YACCSRC     := $(wildcard $(LEXYACC_PATH)/*.y)
LEXC				:= $(LEXSRC:$(LEXYACC_PATH)/%.l=$(SRCDIR)/%.c)
YACCC				:= $(YACCSRC:$(LEXYACC_PATH)/%.y=$(SRCDIR)/%.c)
LEXOBJ			:= $(LEXSRC:$(LEXYACC_PATH)/%.l=$(OBJDIR)/%.o)
YACCOBJ			:= $(YACCSRC:$(LEXYACC_PATH)/%.y=$(OBJDIR)/%.o)

OBJECTS      = $(filter-out $(LEXOBJ) $(YACCOBJ), $(OBJECTS1))


all: check

cov: clean tests
	valgrind --leak-check=full -q ./$(PATH_TO_EXE)
	lcov -d obj -c -o report.info
	genhtml -o report -t "Coverage" report.info

check: clean tests
	valgrind --leak-check=full --show-leak-kinds=all --vgdb=full -s ./$(PATH_TO_EXE)
	make clean > /dev/null 2>&1

check_quiet: CFLAGS += -DQUIET
check_quiet: check

$(LEXC):
	flex -o $@ $(LEXSRC)

$(YACCC):
	bison -v -do $@ $(YACCSRC)

$(PATH_TO_EXE): $(OBJECTS) $(OBJDIR)/$(TARGET).o $(YACCOBJ) $(LEXOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
	@echo "\033[92mLinking complete!\033[0m"
	@echo "\033[93mRunning in debug mode!\033[0m"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(YACCC) $(LEXC)
	mkdir -p $(OBJDIR) $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)

$(LEXOBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(LEXC)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)

$(YACCOBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(YACCC)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)

$(OBJDIR)/$(TARGET).o: $(TARGET).$(FILEXT)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)


.PHONY: clean
clean:
	rm -rf $(OBJDIR)/*
	rm -f *.gcno
	rm -f $(PATH_TO_EXE)
	rm -f $(LEXC)
	rm -f $(YACCC) $(YACCC:.c=.h) $(YACCC:.c=.output)

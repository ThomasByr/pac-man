CC = g++

## variables [leave blank if not needed]

CFLAGS = -pipe -std=gnu++17 -Wpedantic -Wall -Wextra -Werror $(shell sdl2-config --cflags)
YFLAGS =
LDLIBS = -lpthread $(shell sdl2-config --libs)

LEXYACC_PATH =
INCLUDE_PATH = inc
LIB_PATH     = lib
VM_PATH      =

TARGET       = pcmn
FILEXT       = cpp

SRCDIR       = src
OBJDIR       = obj
BINDIR       = bin

## collections [do not modify]

SRCSUBDIRS  := $(shell find $(SRCDIR) -type d)
INCSUBDIRS  := $(shell find $(INCLUDE_PATH) -type d)

SOURCES     := $(foreach dir, $(SRCSUBDIRS), $(wildcard $(dir)/*.$(FILEXT)))
INCLUDES    := $(foreach dir, $(INCSUBDIRS), $(wildcard $(dir)/*.h|*.hpp))
LIBS        := $(wildcard $(LIB_PATH)/*.h|*.hpp)
OBJECTS0    := $(addprefix $(OBJDIR)/, $(SOURCES:$(SRCDIR)/%.$(FILEXT)=%.o))

LEXSRC      := $(wildcard $(LEXYACC_PATH)/*.l)
YACCSRC     := $(wildcard $(LEXYACC_PATH)/*.y)
LEXC        := $(LEXSRC:$(LEXYACC_PATH)/%.l=$(SRCDIR)/%.c)
YACCC       := $(YACCSRC:$(LEXYACC_PATH)/%.y=$(SRCDIR)/%.c)
LEXOBJ      := $(LEXSRC:$(LEXYACC_PATH)/%.l=$(OBJDIR)/%.o)
YACCOBJ     := $(YACCSRC:$(LEXYACC_PATH)/%.y=$(OBJDIR)/%.o)

OBJECTS      = $(filter-out $(LEXOBJ) $(YACCOBJ), $(OBJECTS0))

PATH_TO_EXE  = $(BINDIR)/$(TARGET)
LAUNCH_CMD   = $(PATH_TO_EXE)

## rules [do not modify]

all : debug

.PHONY : docs
docs:
	@echo "\033[95mBuilding documentation...\033[0m"
	@mkdir -p html/assets
	@cp -r assets/* html/assets
	@doxygen ./Doxyfile > /dev/null 2>&1
	@echo "\033[97mDocumentation built!\033[0m"
	@( \
	(  wslview html/index.html \
	|| xdg-open html/index.html \
	|| open html/index.html ) > /dev/null 2>&1 & ) \
	|| echo "\033[91mCould not open documentation in browser.\033[0m"

.PHONY : format
format:
# we exclude the lib directory because it may contain very large files
	@find . -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.${FILEXT}" \) -not -path "./lib/*" -exec clang-format --style=file -i {} \;
	@echo "\033[92mFormatting complete!\033[0m"

debug: CFLAGS += -Og -DDEBUG -g -ggdb -DYYDEBUG
debug: YFLAGS += -v
debug: $(PATH_TO_EXE)
	@echo "\033[93mRunning in debug mode!\033[0m"

release: CFLAGS += -march=native -O2
release: $(PATH_TO_EXE)
	@echo "\033[96mRunning in release mode!\033[0m"

generic: CFLAGS += -march=x86-64 -O2
generic: $(PATH_TO_EXE)
	@echo "\033[95mRunning in generic mode!\033[0m"

run:
ifneq ("$(wildcard $(PATH_TO_EXE))", "")
	./$(LAUNCH_CMD)
else
	@echo "\033[91mNo executable found!\033[0m"
endif

run-release: release
	./$(LAUNCH_CMD)

run-debug: debug
	valgrind --leak-check=full --show-leak-kinds=all --vgdb=full -s ./$(LAUNCH_CMD)


$(LEXC):
	flex -o $@ $(LEXSRC)

$(YACCC):
	bison $(YFLAGS) -do $@ $(YACCSRC)

$(PATH_TO_EXE): $(OBJECTS) $(YACCOBJ) $(LEXOBJ)
	mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
	@echo "\033[92mLinking complete!\033[0m"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(YACCC) $(LEXC)
	mkdir -p $(OBJDIR) $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)

$(LEXOBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(LEXC)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)

$(YACCOBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(YACCC)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)


.PHONY: clean
clean:
	rm -rf $(OBJDIR)/*
	rm -f $(PATH_TO_EXE)
	rm -f $(LEXC)
	rm -f $(YACCC) $(YACCC:.c=.h) $(YACCC:.c=.output)

CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := bin/librunner
INCDIR := include
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_DIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))
#USERPATH := ../UserDemoApp

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -I$(USERAPP)
LIB :=  -lgtest -pthread
INC := -I$(INCDIR) -I../RuntimeSystem/lib/include/

MAIN_FILE := main.cpp
TEST_OBJ := $(filter-out $(BUILDDIR)/$(MAIN_FILE:.$(SRCEXT)=.o), $(OBJECTS))

all: $(TARGET) 

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " ar rcs $(TARGET).a $(OBJECTS) $(LIB)"; ar rcs $(TARGET).a $(OBJECTS)
	#@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

test: tester
	./bin/tester

# Tests
tester:
	$(CC) $(CFLAGS) test/module_test.cpp $(INC) $(TEST_OBJ) $(LIB) -o bin/tester


.PHONY: clean

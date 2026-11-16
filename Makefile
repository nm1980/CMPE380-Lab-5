############################################################
# Makefile for search functions
############################################################

# Standard compile flags
CC       = gcc
CFLAGS   = -O1 -g -Wall -std=c99 -pedantic
VALGRIND = valgrind --tool=memcheck --leak-check=yes --track-origins=yes --error-exitcode=99

# Macros for the program
TARGET = TestSearch
SOURCE = TestSearch.c LinkedLists.c DynamicArrays.c
OBJS = $(SOURCE:.c=.o)
SEARCHWORD = space
TEXTFILE = engWords.txt
OUT = out.txt
MEM = mem.txt 

# Directives
.PHONY: all test mem clean help
.SILENT:

# Entry point to compile all
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)"
	$(CC) $(CFLAGS) $^ -o $@

# Compiling step
%.o: %.c
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@
  
# Test step by running testsearch on the searchword and then the invalid string
test: $(TARGET)
	@echo "------- Running Search Program valid input ------" > $(OUT) 2>&1
	./$(TARGET) $(TEXTFILE) $(SEARCHWORD) >> $(OUT) 2>&1
	@echo "------- Additional test, invalid input ------" >> $(OUT) 2>&1
	./$(TARGET) $(TEXTFILE) xyzzy >> $(OUT) 2>&1

# Entry point to generate Valgrind memory report
mem: $(TARGET) 
	@echo "Valgrind memory check"
	$(VALGRIND) ./$(TARGET) $(TEXTFILE) $(SEARCHWORD) > $(MEM) 2>&1

# Entry point to clean up application and remove all exes, objs, etc.
clean:
	@echo "Cleaning all binaries, object, and test files"
	rm -f $(TARGET) $(OBJS) $(OUT) $(MEM)
 
# Entry point to list all Make targets
help:
	@echo " Makefile Targets for search: "
	@echo " all: Builds the target file binary "
	@echo " test: Runs 2 search sets using the test file txt and stores into out file "
	@echo " mem: Runs Valgrind memory analysis and stores into mem file "
	@echo " clean: cleans up all object, binaries, and output files "
	@echo " help: displays Makefile guide "
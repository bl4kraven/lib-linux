CXX=g++
#CPPFLAGS=-Wall -O2 -DNDEBUG
CPPFLAGS=-Wall -g -D_DEBUG
LDFLAGS=

# source code root dir
SOURCE_DIR=src/
FILES=$(shell find $(SOURCE_DIR) -name "*.cpp")
FILES+= $(shell find $(SOURCE_DIR) -name "*.c")
HEADERS=$(shell find $(SOURCE_DIR) -name "*.h")
LIBS=-lpthread
DEPEND=
OBJECTS=$(addsuffix .o, $(basename $(FILES)))
TARGET=liblinux_tool.a

all:$(TARGET) 

# include all source file depends.
-include Makefile.deps

$(TARGET): Makefile.deps $(OBJECTS) $(DEPEND)
	ar cru $(TARGET) $(OBJECTS)

Makefile.deps: $(FILES) $(HEADERS)
	makedepend -f "$(FILES)" -c "$(CXX)" -p "$(CPPFLAGS)" > Makefile.deps

clean:
	-rm -f $(OBJECTS) $(TARGET) Makefile.deps

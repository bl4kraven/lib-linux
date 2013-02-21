CXX=g++
#CPPFLAGS=-Wall -O2 -DNDEBUG -I../src/
CPPFLAGS=-Wall -g -D_DEBUG -I../src/
LDFLAGS=-L../

# source code root dir
SOURCE_DIR=../src/
FILES=
HEADERS=
LIBS=-llinux_tool -lpthread
DEPEND=../liblinux_tool.a
OBJECTS=$(TARGET).o $(FILES:.cpp=.o) 
TARGET=stdlog_test

all:$(TARGET) 

# include all source file depends.
-include Makefile.deps

$(TARGET): Makefile.deps $(OBJECTS) $(DEPEND)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

Makefile.deps: $(FILES) $(HEADERS)
	makedepend -f "$(FILES) $(TARGET).cpp" -c "$(CXX)" -p "$(CPPFLAGS)" > Makefile.deps

clean:
	-rm -f $(OBJECTS) $(TARGET) Makefile.deps

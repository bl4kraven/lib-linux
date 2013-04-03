SRC_DIR=.

CPPFLAGS+=-Wall -g -D_DEBUG
CPPFLAGS+=-I../src -I../src/SimpleNet
CPPFILES=$(shell find $(SRC_DIR)  -maxdepth 1 -name "*.cpp")
CPPOBJS=$(CPPFILES:%.cpp=%.o)
LIBS+=../liblinux_tool.a -lpthread
TARGET=$(CPPFILES:%.cpp=%)

all:$(TARGET) 

-include $(addsuffix /*.d, $(SRC_DIR))

$(TARGET):%:%.o
	g++ -o $@ $< $(LIBS)

$(CPPOBJS):%.o:%.cpp
	g++ -c $(CPPFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<

clean:
	-rm -f $(addsuffix /*.d, $(SRC_DIR)) $(addsuffix /*.o, $(SRC_DIR)) $(TARGET)


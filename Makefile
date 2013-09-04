SRC_DIR=src src/SimpleNet

CFLAGS+=-Wall -g -D_DEBUG
#CFLAGS+=-Wall -O2
CPPFLAGS+=-Wall -g -D_DEBUG
#CPPFLAGS+=-Wall -O2
CPPFLAGS+=-Isrc

CFILES=$(shell find $(SRC_DIR)  -maxdepth 1 -name "*.c")
CPPFILES=$(shell find $(SRC_DIR)  -maxdepth 1 -name "*.cpp")

COBJS=$(CFILES:%.c=%.o) 
CPPOBJS=$(CPPFILES:%.cpp=%.o)
TARGET=liblinux_tool.a
LIBS+=

all:$(TARGET) 

-include $(addsuffix /*.d, $(SRC_DIR))

$(TARGET):$(COBJS) $(CPPOBJS)
	$(HOST)ar cru $(TARGET) $^

$(COBJS):%.o:%.c
	$(HOST)gcc -c $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<

$(CPPOBJS):%.o:%.cpp
	$(HOST)g++ -c $(CPPFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<

clean:
	-rm -f $(addsuffix /*.d, $(SRC_DIR)) $(addsuffix /*.o, $(SRC_DIR)) $(TARGET)

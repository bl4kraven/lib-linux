SRC_DIR=src src/SimpleNet

CPPFLAGS+=-Wall -g -D_DEBUG
#CPPFLAGS+=-Wall -O2
CPPFLAGS+=-Isrc -Isrc/SimpleNet
CPPFILES:=$(shell find $(SRC_DIR)  -maxdepth 1 -name "*.cpp")
CPPOBJS:=$(CPPFILES:%.cpp=%.o)
TARGET:=liblinux_tool.a
LIBS+=

ZIP_NAME:=$(basename $(TARGET))
ZIP_FILES:=$(addsuffix /*.[hc]*, $(SRC_DIR) test) Makefile README.md

TESTFILES:=$(shell find test -maxdepth 1 -name "*.cpp")
TESTOBJS:=$(TESTFILES:%.cpp=%.o)
TESTAPP:=$(TESTOBJS:%.o=%)

all:$(TARGET) 

test:$(TARGET) $(TESTAPP)

-include $(addsuffix /*.d, $(SRC_DIR))
-include $(addsuffix /*.d, test)

$(TARGET):$(CPPOBJS)
	$(HOST)ar cru $(TARGET) $^

$(CPPOBJS) $(TESTOBJS):%.o:%.cpp
	$(HOST)g++ -c $(CPPFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<

$(TESTAPP):%:%.o 
	$(HOST)g++ -o $@ $< $(TARGET) -lpthread

zip: 
	-rm -rf $(ZIP_NAME).zip
	zip -r $(ZIP_NAME).zip $(ZIP_FILES)

clean:
	-rm -f $(addsuffix /*.[do], $(SRC_DIR) test) $(TARGET) $(TESTAPP)

.PHONY: zip test

SRC_DIR=src src/SimpleNet

CFLAGS+=-Wall -g -D_DEBUG
#CFLAGS+=-Wall -O2
CPPFLAGS+=-Wall -g -D_DEBUG
#CPPFLAGS+=-Wall -O2
CPPFLAGS+=-Isrc -Isrc/SimpleNet

CFILES:=$(shell find $(SRC_DIR)  -maxdepth 1 -name "*.c")
CPPFILES:=$(shell find $(SRC_DIR)  -maxdepth 1 -name "*.cpp")

COBJS:=$(CFILES:%.c=%.o) 
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

$(TARGET):$(COBJS) $(CPPOBJS)
	$(HOST)ar cru $(TARGET) $^

$(COBJS):%.o:%.c
	$(HOST)gcc -c $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<

$(CPPOBJS) $(TESTOBJS):%.o:%.cpp
	$(HOST)g++ -c $(CPPFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<

$(TESTAPP):%:%.o 
	$(HOST)g++ -o $@ $< $(TARGET) -lpthread

zip: TMPDIR:=$(shell mktemp -d)
zip: CURDIR:=$(shell pwd)
zip: 
	mkdir $(TMPDIR)/$(ZIP_NAME)
	cp --parent $(ZIP_FILES) $(TMPDIR)/$(ZIP_NAME)
	-rm -rf $(ZIP_NAME).zip
	cd $(TMPDIR) && zip -r $(CURDIR)/$(ZIP_NAME).zip $(ZIP_NAME)
	-rm -rf $(TMPDIR)

clean:
	-rm -f $(addsuffix /*.[do], $(SRC_DIR) test) $(TARGET) $(TESTAPP)

.PHONY: zip test

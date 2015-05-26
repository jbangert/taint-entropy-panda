# Don't forget to add your plugin to config.panda!

# Set your plugin name here. It does not have to correspond to the name
# of the directory in which your plugin resides.
PLUGIN_NAME=cryptofind

# Include the PANDA Makefile rules
include ../panda.mak

# If you need custom CFLAGS or LIBS, set them up here
# CFLAGS+=
LIBS+= -ldistorm3 -lprotobuf -lcrypto
QEMU_CXXFLAGS+= -std=c++11 -O3 -ggdb

# This will make things easier.
$(PLUGIN_OBJ_DIR)/%.o: $(wildcard *.h) $(wildcard $(SRC_PATH)/panda/*.h)
# The main rule for your plugin. Please stick with the panda_ naming
# convention.

$(PLUGIN_TARGET_DIR)/trace.pb.o: trace.pb.cc $(wildcard *.h) $(wildcard $(SRC_PATH)/panda/*.h)
	$(call quiet-command,$(CXX) $(QEMU_CXXFLAGS)  -shared -fPIC -c -o $@ $< ," compile $@")

$(PLUGIN_TARGET_DIR)/panda_$(PLUGIN_NAME).so: $(PLUGIN_TARGET_DIR)/$(PLUGIN_NAME).o \
	$(PLUGIN_TARGET_DIR)/table.o $(PLUGIN_TARGET_DIR)/disas.o $(PLUGIN_TARGET_DIR)/trace.pb.o
	$(call quiet-command,$(CXX) $(QEMU_CXXFLAGS) -shared -o $@ $^ $(LIBS),"  PLUGIN  $@")

all: $(PLUGIN_TARGET_DIR)/panda_$(PLUGIN_NAME).so

trace.pb.cc trace.pb.h: trace.proto
	protoc --cpp_out=. $<
table.cpp: instructiontypes.py generate-iclass.rb geek*.html
	./instructiontypes.py < /usr/local/include/mnemonics.h > $@
#./generate-iclass.rb  icls_i32 geek32.html > $@ && ./generate-iclass.rb  icls_i64 geek64.html  >> $@
download-tables:
	wget http://ref.x86asm.net/geek32.html && wget http://ref.x86asm.net/geek64.html  

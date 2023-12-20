IRX_DIR := ./irx
OBJS_DIR := ./objs
SRC_DIR := ./ps2
TARGET_DIR := ./bin
WIN_DIR := /mnt/c/Users/dflet/Desktop/
PS2_LIB_DIR := /home/dfletcher/PS2DEMO/PS2-Development/inc

OBJEXT    := o
CEXT      := c

EE_BIN = doom.elf
IOP_MOD = sio2man.irx padman.irx sound.irx libsd.irx audsrv.irx

EE_SRC_C += $(shell find $(SRC_DIR) -type f -name *.$(CEXT))
EE_OBJS += $(patsubst $(SRC_DIR)/%, $(OBJS_DIR)/%, $(EE_SRC_C:.$(CEXT)=.$(OBJEXT)))

EE_DVP = dvp-as

EE_LIBS= -lps2 -ldma -lgraph -ldraw -lkernel -lpacket -lpad -lcdvd -lpng -lz -lunzip -laudsrv 

PS2SDK=/usr/local/ps2dev/ps2sdk

LOG_LEVEL?=3

EE_CFLAGS += -DPS_LOG_LVL=$(LOG_LEVEL) -Wall -Wno-char-subscripts --std=gnu99 -I$(PS2_LIB_DIR)
EE_LDFLAGS = -L./lib -L$(PS2SDK)/ee/common/lib -L$(PS2SDK)/ee/lib -L$(PS2SDK)/ports/lib 

ISO_TGT=doom.iso

include ./Makefile.eeglobal
include $(PS2SDK)/samples/Makefile.pref

.PHONY : all clean compile directories disc irx update_win

all: compile disc update_win

compile:
	make

directories:
	if [ ! -d $(IRX_DIR) ]; then mkdir -p $(IRX_DIR) ; fi

	if [ ! -d $(OBJS_DIR) ]; then mkdir -p $(OBJS_DIR) ; fi

	if [ ! -d $(TARGET_DIR) ]; then mkdir -p $(TARGET_DIR) ; fi

update_win:
	cp $(ISO_TGT) $(WIN_DIR)

disc:
	mkisofs -l -o $(ISO_TGT) $(TARGET_DIR)/$(EE_BIN) SYSTEM.CNF $(IRX_DIR) doom1.wad doom2.wad background.png DefaultFont.bmp DefaultFontData.dat

irx:
	mv $(IOP_MOD) "$(IRX_DIR)"

clean:
	rm -rf $(ISO_TGT) $(TARGET_DIR)/$(EE_BIN) $(EE_OBJS)

	
		

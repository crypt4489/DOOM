IRX_DIR := ./irx
OBJS_DIR := ./objs
SRC_DIR := ./ps2
TARGET_DIR := ./bin

OBJEXT    := o
CEXT      := c

EE_BIN = doom.elf
IOP_MOD = sio2man.irx padman.irx sound.irx libsd.irx audsrv.irx

EE_SRC_C += $(shell find $(SRC_DIR) -type f -name *.$(CEXT))
EE_OBJS += $(patsubst $(SRC_DIR)/%, $(OBJS_DIR)/%, $(EE_SRC_C:.$(CEXT)=.$(OBJEXT)))

EE_DVP = dvp-as

EE_LIBS=-lps2lib

PS2SDK=/Users/Fletcher_Drew/Documents/ps2build/ps2dev/ps2sdk

LOG_LEVEL?=3

EE_CFLAGS += -DPS_LOG_LVL=$(LOG_LEVEL) -Wall -Wno-char-subscripts --std=gnu99 -I$(INC_DIR)
EE_LDFLAGS = -L$(PSDSDK)/ee/common/lib -L$(PS2SDK)/ee/lib -L$(PS2SDK)/ports/lib -L./lib

ISO_TGT=doom.iso

include ./Makefile.eeglobal
include $(PS2SDK)/samples/Makefile.pref

.PHONY : all clean compile directories disc irx update_win

all: compile disc

compile:
	make

directories:
	if [ ! -d $(IRX_DIR) ]; then mkdir -p $(IRX_DIR) ; fi

	if [ ! -d $(OBJS_DIR) ]; then mkdir -p $(OBJS_DIR) ; fi

	if [ ! -d $(TARGET_DIR) ]; then mkdir -p $(TARGET_DIR) ; fi

update_win:
	cp $(ISO_TGT) $(WIN_DIR)

disc:
	mkisofs -l -o $(ISO_TGT) $(TARGET_DIR)/$(EE_BIN) SYSTEM.CNF $(IRX_DIR) $(ASSETS_DIR)

irx:
	mv $(IOP_MOD) "$(IRX_DIR)"

clean:
	rm -rf $(ISO_TGT) $(TARGET_DIR)/$(EE_BIN) $(EE_OBJS)

	
		

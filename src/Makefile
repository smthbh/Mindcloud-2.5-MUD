#Mindcloud 2 Makefile

#Change this to reflect whichever compiler you want to use.
CC      = gcc

#Type of machine to compile for. Athlon works just as well on Duron too.
#The march option needs to match the general family of CPU.
#If you don't know what to set for these options, and your system administrator doesn't either, comment this line out
#MACHINE = -mcpu=athlon -march=athlon

#Uncomment if compiling in Windows under Cygwin
#WINDOWS = 1

#IMC2 - Comment out to disable IMC2 support
IMC = 1

#SAVEMEM - Comment out to load all mobs all the time (Experimental)
#SAVEMEM = -DSAVEMEM

#Miscellaneous compiler options.
OPT_FLAG = -pipe -Os
DEBUG_FLAG = -g2
#PROF_FLAG = -p
#NOCRYPT = -DNOCRYPT

W_FLAGS = -Wall -Wformat-security -Wshadow -Wpointer-arith -Wcast-align -Wcast-qual -Wredundant-decls

C_FLAGS = $(MACHINE) $(W_FLAGS) $(DEBUG_FLAG) $(OPT_FLAG) $(PROF_FLAG) $(NOCRYPT) $(SAVEMEM)
L_FLAGS = $(MACHINE) $(DEBUG_FLAG) $(OPT_FLAG) $(PROF_FLAG) -lz -lpthread

#OLDPFILES - Uncomment to use old 2.0 pfiles
#OLDPFILES = 1

ifdef OLDPFILES
	L_FLAGS := $(L_FLAGS) -lcrypt
endif

#various Makefile variables
ECHOCMD = echo -e

ifdef WINDOWS
EXECFILE = mcloud.exe
else
EXECFILE = mcloud
endif

OBJ_DIR = obj
EXECPRM = 4770
L_RED     = \e[1;31m
L_BLUE    = \e[1;34m
L_GREEN   = \e[1;32m
L_WHITE   = \e[1;37m
L_NRM     = \e[0;00m

C_FILES = act_comm.c act_info.c act_move.c act_obj.c act_wiz.c bit.c mccp.c \
	board.c build.c clan.c comm.c daemon.c db.c angel.c x-socials.c\
	demon.c drow.c fight.c kav_wiz.c handler.c kav_fight.c kav_info.c \
	interp.c mage.c magic.c mem.c monk.c monk2.c ninja.c olc.c \
	olc_act.c olc_save.c save.c special.c string.c arena.c auction.c\
	update.c vamp.c ww.c shapeshifter.c tanarri.c undead_knight.c \
	samurai.c wizutil.c const.c cyborg.c lich.c jope.c religion.c \
	jobo_king.c jobo_fight.c jobo_act.c jobo_wiz.c jobo_util.c \
	jobo_comm.c jobo_save.c social-edit.c jobo_update.c \
	jobo_quest.c msp.c kav_soundex.c desc_map.c jobo_shop.c zarius_proj.c \
	zarius.c changes.c vote.c skyblade.c ghoul.c draconian.c thief.c \
	shadow.c priest.c jedi.c psycho.c giant.c fae.c drone.c \
	zarius_quest.c zarius_bank.c olc_mpcode.c mob_prog.c \
	mob_cmds.c recycle.c wizlist.c zarius_comm.c zarius_games.c md5.c \
	zarius_util.c zarius_hint.c version.c
	
ifdef IMC
   C_FILES := imc.c sha256.c $(C_FILES)
   C_FLAGS := $(C_FLAGS) -DIMC -DIMCMERC
endif

O_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(C_FILES))

H_FILES = $(wildcard *.h)

all:
	@touch version.c
	@$(MAKE) build
	@$(MAKE) -s mcloud

mcloud: $(O_FILES)
	@$(ECHOCMD) "[- Rebuilding MUD executable: $(L_BLUE)$(EXECFILE)$(L_NRM) -]"
	@rm -f $(EXECFILE)
	@$(CC)  -o $(EXECFILE) $(O_FILES) $(L_FLAGS)
	@$(ECHOCMD) "[- Done Compiling : $(L_BLUE)$(EXECFILE)$(L_NRM) -]"
	@$(ECHOCMD) "[- Fixing MUD permissions to $(L_WHITE)$(EXECPRM)$(L_NRM) -]"
	@chmod g+w $(EXECFILE)
	@chmod a+x $(EXECFILE)
	@chmod g+w $(O_FILES)

purge:
	rm -f $(OBJ_DIR)/*.o $(EXECFILE) core
	rm -f ../area/core*

$(OBJ_DIR)/%.o: %.c
	@$(ECHOCMD) "-->   Compiling file: $(L_WHITE)$<$(L_NRM)   <--"
	@$(CC) -c $(C_FLAGS) -o $@ $<

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

clean :
	rm -f $(OBJ_DIR)/*.o ; rm -f $(EXECFILE)

build:
	@if [ -f ../txt/buildnum ]; then \
		expr `cat ../txt/buildnum` + 1 > ../txt/buildnum; \
	else \
		echo '1' > ../txt/buildnum; \
	fi
	@if [ -f ../txt/versionnum ]; then \
		expr `cat ../txt/versionnum` + 1 > ../txt/versionnum; \
	else \
		echo '10001' > ../txt/versionnum; \
	fi
	@if [ `cat ../txt/buildnum` -ge 51 ]; then \
	echo '1' > ../txt/buildnum; \
	fi
	@echo '#ifndef BUILD_H' > build.h
	@echo '#define BUILD_H' >> build.h
	@echo "#define LINKCOUNT   `cat ../txt/buildnum`" >> build.h
	@echo '#endif' >> build.h
	@echo '#ifndef VERSION_H' > version.h
	@echo '#define VERSION_H' >> version.h
	@echo "#define VERSION   `cat ../txt/versionnum`" >> version.h
	@echo '#endif' >> version.h
	@$(ECHOCMD) "$(L_WHITE)Build number $(L_RED)`cat ../txt/buildnum`$(L_WHITE), version $(L_RED)`cat ../txt/versionnum` $(L_WHITE)set.$(L_NRM)"

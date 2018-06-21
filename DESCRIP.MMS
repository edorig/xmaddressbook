!CDEFS = /define=curses
.ifdef __DECC__
CFLAGS = $(CFLAGS)$(CDEFS)/prefix=all
.endif

EXEC = xmaddressbook

OBJS = main.obj,file.obj,quit.obj,general.obj,menu.obj,help.obj,list.obj,\
       search.obj,print.obj,error_msg.obj,duplicate.obj,goto.obj,card.obj,\
       namelist.obj,mail.obj,preferences.obj
SRCS = file.c main.c quit.c general.c menu.c help.c list.c search.c print.c \
       error_msg.c duplicate.c goto.c card.c namelist.c mail.c preferences.c
HDRS = general.h menu.h main.h prototypes.h help.h

all   :	 $(OBJS) $(EXEC) 
        !

$(EXEC) : $(OBJS)
	@ Write sys$output  "--------- Linking $(EXEC) ---------"
	$link/exe=$(EXEC).exe $(OBJS),$(EXEC).opt/opt

$(OBJS) : $(HDRS) 
	$(CC) $(CFLAGS)   $*.c

clean :
	@ write sys$output "--------- Cleaning Up ---------"
	@ dele *.obj;*

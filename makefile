FLAGS   = -Wall -pthread -D_REENTRANT -g
CC      = gcc
PROG_1  = form
OBJS_1  = race_simulator.o race_manager.o teamManager.o malfunction_manager.o
PROG_2  = comm
OBJS_2  = commands.o

all:	${PROG_1} ${PROG_2}

clean:
		rm ${OBJS_1} ${OBJS_2}

${PROG_1}:	${OBJS_1}
		 ${CC} ${FLAGS} ${OBJS_1} -o $@
		 
${PROG_2}:	${OBJS_2}
		${CC} ${FLAGS} ${OBJS_2} -o $@

.c.o:
		${CC} ${FLAGS} $< -c

##########################

race_simulator.o:	race_simulator.c race_simulator.h
race_manager.o: race_manager.c race_manager.h
teamManager.o:	teamManager.c teamManager.h
malfunction_manager.o: malfunction_manager.c malfunction_manager.h

commands.o:				commands.c

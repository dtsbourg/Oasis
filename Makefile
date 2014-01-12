#Définition de macros

INCLUDE = error/include
CC = gcc
LIBS = -lm -Llib -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXi -lstdc++ -lGL -lGLU -lglui -lglut -g
CFLAGS = -Wall -g -I${INCLUDE}

CFILES = simulation/simulation.c movement/movement.c simulation/test.c proj_1.cpp graphic/graphic.c save_data/cell.c save_data/prey.c save_data/predator.c
OFILES = $(CFILES:.c=.o) error/bin/error.o action/action.o
CIBLE = proj_3.x

#Definiton de la première regle

$(CIBLE): $(OFILES)
	$(CC) $(OFILES) $(LIBS) -o $(CIBLE)

#Definition des règles particulières
#backup: clean
#	cd .. && tar czpf $(DIRECTORY)/$(PROJECT).tar.gz $(PROJECT)

depend: 
	@(sed '#do not delete this line/q' Makefile && \
	  $(CC) -MM $(CFILES) | \
	  egrep -v "usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@/bin/rm -f *.o *.x

test.o: $^
	$(CC) $(CFLAGS) -c simulation/test.c

movement.o: $^
	$(CC) $(CFLAGS) -c  movement/movement.c

simulation.o: $^
	$(CC) $(CFLAGS) -c simulation/simulation.c

proj_3.o: $^
	$(CC) $(CFLAGS) -c proj_1.cpp

graphic.o: $^
	$(CC) $(CFLAGS) -c graphic/graphic.c

cell.o: $^
	$(CC) $(CFLAGS) -c save_data/cell.c

prey.o: $^
	$(CC) $(CFLAGS) -c save_data/prey.c

predator.o: $^
	$(CC) $(CLFAGS) -c save_data/predator.c


#Règles de dépendances générées automatiquement
simulation.o: simulation/simulation.c simulation/simulation.h \
 simulation/../error/include/error.h simulation/test.h \
 simulation/../constantes.h simulation/../save_data/save_data.h \
 simulation/../save_data/cell.h simulation/../save_data/prey.h \
 simulation/../save_data/predator.h simulation/../save_data/predator.h \
 simulation/../save_data/prey.h
movement.o: movement/movement.c movement/movement.h
test.o: simulation/test.c simulation/../error/include/error.h \
 simulation/test.h simulation/simulation.h simulation/../constantes.h
proj_1.o: proj_1.cpp constantes.h simulation/simulation.h \
 graphic/graphic.h graphic/../save_data/save_data.h save_data/save_data.h \
 save_data/cell.h save_data/prey.h save_data/predator.h \
 save_data/predator.h save_data/prey.h
graphic.o: graphic/graphic.c graphic/graphic.h \
 graphic/../save_data/save_data.h graphic/../simulation/simulation.h \
 graphic/../save_data/prey.h graphic/../save_data/predator.h \
 graphic/../save_data/prey.h graphic/../save_data/cell.h \
 graphic/../save_data/predator.h graphic/../constantes.h
cell.o: save_data/cell.c save_data/cell.h save_data/prey.h \
 save_data/predator.h save_data/../constantes.h \
 save_data/../simulation/simulation.h
prey.o: save_data/prey.c save_data/prey.h save_data/../constantes.h \
 save_data/cell.h save_data/predator.h \
 save_data/../simulation/simulation.h save_data/../movement/movement.h \
 save_data/../action/action.h
predator.o: save_data/predator.c save_data/predator.h save_data/prey.h \
 save_data/cell.h save_data/../simulation/simulation.h \
 save_data/../constantes.h save_data/../movement/movement.h \
 save_data/../action/action.h

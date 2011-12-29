############
MACHINE= $(shell uname -s)

ifeq ($(MACHINE),Darwin)
    OPENGL_INC= -FOpenGL  -I/usr/X11R6/include
    OPENGL_LIB= -framework OpenGL
    SDL_INC= `sdl-config --cflags`
    SDL_LIB= `sdl-config --libs`
else
    OPENGL_INC= -I/usr/X11R6/include
    OPENGL_LIB= -I/usr/lib64 -lGL -lGLU
    SDL_INC= `sdl-config --cflags`
    SDL_LIB= `sdl-config --libs`
endif

# object files have corresponding source files
OBJS= vec3.o camera.o cylinder.o plane.o vertexarray.o primitive.o localvertex.o hello.o 
HEADERS=$(shell echo *.class | sed 's/\([^ ]*\)\.class/\1.h \1.cc/g')
CXX=g++
CC=g++
INCLUDE= $(SDL_INC) $(OPENGL_INC)
CPPFLAGS= -g  $(INCLUDE) -Wall
LOADLIBES= $(SDL_LIB) $(OPENGL_LIB)

EXEC= hello

all: $(EXEC)

$(EXEC):	$(OBJS)

$(OBJS) : $(HEADERS)

%.cc %.h: %.class
	./cheap $(shell echo $@ | cut -d '.' -f1).class

clean:
	rm -f $(EXEC) $(OBJS)
	rm -f echo $(shell echo *.class | sed 's/\([^ ]*\)\.class/\1.h \1.cc/g')



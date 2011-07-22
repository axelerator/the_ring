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
OBJS= hello.o vec3.o
CXX=g++
COMPILER_FLAGS= -g
INCLUDE= $(SDL_INC) $(OPENGL_INC)
LIBS= $(SDL_LIB) $(OPENGL_LIB)

EXEC= sdlglsl

$(EXEC):	$(OBJS)
	$(CXX) $(COMPILER_FLAGS) -o $(EXEC) $(OBJS) $(LIBS)

%.o:	%.c
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

%.c: %.class
	./cheap $(shell echo $@ | cut -d '.' -f1).class

clean:
	rm -f $(EXEC) $(OBJS)



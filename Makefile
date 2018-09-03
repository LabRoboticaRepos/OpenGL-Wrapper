#COMPILER-LINKER
CC			= g++
#FLAGS
SDLHeads	= $(shell sdl2-config --cflags)
SDLlib		= $(shell sdl2-config --libs)

CFLAGS 		= -std=c++14 #-Wall -Werror
INCLUDES	= -Iincludes $(SDLHeads)
LFLAGS		= -ldl $(SDLlib) -lGL

#DIRECTORIES
SRC 		= sources
OUTDIR		= release
ALL = $(notdir $(shell find -name '*.cpp')) $(notdir $(shell find -name '*.c'))
OBJS = $(patsubst %.c,%.o, $(patsubst %.cpp, %.o, $(ALL)))
SHADERS = fragment.fs vertex.vs

vpath %.hpp 	includes
vpath %.h   	includes
vpath %.o 		release
vpath %			release/exec
vpath %.cpp 	src
vpath %.c   	src
vars:
	@echo "SDLHeads: ------> $(SDLHeads)"
	@echo "SDLlib: ------> $(SDLlib)"
	@echo "OBJS: ------> $(OBJS)"
	@echo "ALL: ------> $(ALL)"

run: $(OBJS) $(SHADERS)
	cd $(OUTDIR) && $(CC) $(LFLAGS) $(notdir $(OBJS)) -o exec/$(notdir $@)
	chmod +x $(OUTDIR)/exec/$@

glad.o: includes/glad/glad.h
main.o: src/main.cpp Screen.o
Screen.o: src/Screen.cpp Screen.cpp includes/Screen.hpp
Shader.o: src/Shader.cpp Screen.cpp includes/Shader.hpp
ProgramGL.o: includes/ProgramGL.hpp

fragment.fs:shaders/fragment.fs
	cp $^ release/exec/shaders
vertex.vs: shaders/vertex.vs
	cp $^ release/exec/shaders

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(OUTDIR)/$(notdir $@)

.PHONY: clean
clean:
	@find -name '*.o' |xargs -i  rm '{}'
	#@rm $(OUTDIR)/exec/shaders/*
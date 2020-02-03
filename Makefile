#OBJS specifies which files to compile as part of the project 
OBJS = main.cpp  renderOps.cpp rect2dRenderer.h ./shader_utils/shader_utils.o ./lib/tinyxml2.cpp

#CC specifies which compiler we're using 
CC = g++ 

#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
COMPILER_FLAGS = -w -I/usr/local/include/freetype2 

INCLUDE= 
#-I"./include/freetype2"

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lGL -lGLEW -lSDL2 -std=c++11 -lfreetype

#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = Test_exec 

#This is the target that compiles our executable 
all : $(OBJS) 
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	 $(RM) $(OBJ_NAME)


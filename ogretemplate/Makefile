## CS134 Spring 2014: Video Game Design
## Lab Ogre3D Makefile
##
## This file does not need to be modified
#########################################

OGRE_PREFIX = /usr/csshare/pkgs
#OGRE_PREFIX = /extra
INCLUDE_PATHS = -I $(OGRE_PREFIX)/ogre-1.8.1/include/OGRE/ -I $(OGRE_PREFIX)/ois-v1-3/includes/
LIB_PATHS = -L $(OGRE_PREFIX)/ogre-1.8.1/lib/ -L $(OGRE_PREFIX)/ogre-1.8.1/lib/OGRE/ -L $(OGRE_PREFIX)/ois-v1-3/lib/
LIBS = -lOgreMain -lOIS
CC = g++

## Global header files
INCLUDE =

## Object files and executables
MAIN_OUT = a.out
BASE_APPLICATION_OUT = BaseApplication.o

## Requirements for each command
MAIN_REQS = MainApplication.cpp $(BASE_APPLICATION_OUT)
BASE_APPLICATION_REQS = BaseApplication.cpp

## Targets to compile for each command
MAIN_TARGETS = MainApplication.cpp $(BASE_APPLICATION_OUT)
BASE_APPLICATION_TARGETS = BaseApplication.cpp

all: main

## Main 
main: $(MAIN_REQS) $(INCLUDE)
	$(CC) $(INCLUDE_PATHS) $(LIB_PATHS) $(MAIN_TARGETS) $(LIBS) -o $(MAIN_OUT)
$(BASE_APPLICATION_OUT): $(BASE_APPLICATION_REQS) $(INCLUDE)
	$(CC) -c $(INCLUDE_PATHS) $(LIB_PATHS) $(BASE_APPLICATION_TARGETS) $(LIBS) -o $(BASE_APPLICATION_OUT)

clean:
	rm -f *~ *.o *.out

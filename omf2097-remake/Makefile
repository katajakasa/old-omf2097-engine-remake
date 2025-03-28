# A simple Makefile for OMFG

BINARYNAME=omf-bin

DUMBLIB=../libdumb/lib
DUMBINC=../libdumb/include
SFMLLIB=../libsfml/lib
SFMLINC=../libsfml/include

CPP=g++
RM=rm -f
LD=ld
MKDIR=mkdir -p
MV=mv

FILES := \
	src/handlers/animation.cpp \
	src/handlers/background.cpp \
	src/engine.cpp \
	src/handlers/har.cpp \
	src/handlers/harpalette.cpp \
	src/handlers/image.cpp \
	src/handlers/animationplayer.cpp \
	src/tools/inireader.cpp \
	src/main.cpp \
	src/handlers/palette.cpp \
	src/tools/parser.cpp \
	src/scenes/scene.cpp \
	src/scenes/introscene.cpp \
	src/scenes/menuscene.cpp \
	src/handlers/stringengine.cpp \
	src/audio/music.cpp \
	src/audio/sampleloader.cpp \
	src/audio/audiosample.cpp \
	src/video/texture.cpp \
	src/video/tgawriter.cpp \
	src/video/glrenderer.cpp \
	src/video/glextloader.cpp \
	src/video/shader.cpp \
	src/video/shaderprogram.cpp \
	src/tools/exception.cpp

BINDIR=bin/
FLAGS=-Wall -W -O2
CPPFLAGS=-Isrc -D_LINUX -I$(DUMBINC) -I$(SFMLINC) $(FLAGS) -s -L$(DUMBLIB) -L$(SFMLLIB) -lsfml-audio -lsfml-window -lsfml-system -lGL -lGLU -ldumb -lGLEW 

all: 
	$(MKDIR) $(BINDIR)
	$(CPP) $(CPPFLAGS) -o $(BINDIR)/$(BINARYNAME) $(FILES)
	@echo "Make done. You still have to find OMF resource files, and copy them to bin/resources/."
clean:
	$(RM) $(OBJDIR_BASE)/Release/*.o
	$(RM) $(BINDIR)/*


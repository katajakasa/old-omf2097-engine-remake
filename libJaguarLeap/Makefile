# A simple Makefile for libJaguarLeap

LIBNAME=libjaguarleap.a

INSTALL_LIBDIR=/usr/lib
INSTALL_INCDIR=/usr/include

CC=g++
RM=rm -f
MKDIR=mkdir -p
MV=mv
AR=ar

FILES := \
    src/jaguarleap/shared/animation.cpp \
    src/jaguarleap/shared/sprite.cpp \
    src/jaguarleap/bkfile.cpp \
    src/jaguarleap/languagefile.cpp \
    src/jaguarleap/soundfile.cpp \
    src/jaguarleap/fontfile.cpp \
    src/jaguarleap/fileops/writer.cpp \
    src/jaguarleap/fileops/parser.cpp \
    src/jaguarleap/affile.cpp
    
LIBDIR=lib
INCDIR=include
OBJDIR=obj

CFLAGS=-I$(INCDIR) -O2 -std=c++0x

all: 
	$(MKDIR) $(LIBDIR)/
	$(MKDIR) $(OBJDIR)/
	$(CC) $(CFLAGS) -c $(FILES)
	$(MV) *.o $(OBJDIR)/
	$(AR) rcs $(LIBDIR)/$(LIBNAME) $(OBJDIR)/*.o
	@echo "All done!"

clean:
	$(RM) $(OBJDIR)/*.o
	$(RM) $(LIBDIR)/*


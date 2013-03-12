CC	=	cc
CFLAGS	=	-O
LFLAGS	=	
AR	=	ar rc
RANLIB	=	ranlib

ifndef OS
OS	=	$(shell uname -s)
endif

#define to use BSP (SEQ,PUB,MPI,SHM)
BSP	=	SHM

ifeq ($(OS),Linux)

CC	=	g++

ifeq ($(BSP),NONE)
CC	=	g++ -pipe -Wall
endif

ifeq ($(BSP),PUB)
CC	=	pubCC -pipe -Wall
endif

ifeq ($(BSP),MPI)
CC	=	mpicxx -pipe -Wall
endif

ifeq ($(BSP),SHM)
CC	=	g++ -pipe -Wall
endif

CFLAGS	=	-D_REENTRANT -O3 -fomit-frame-pointer -finline-functions -funroll-loops
endif

ifeq ($(OS),SunOS)
CC	=	CC
CFLAGS	=	-mt -fast -xO5
LFLAGS	=	-lpthread -lrt
AR	=	CC -mt -xar -o
endif

ifeq ($(OS),AIX)
CC	=	xlC
CFLAGS	=	-O2 -qinline -qrtti
endif

ifeq ($(OS),HP-UX)
CC	=	aCC
CFLAGS	=	-mt -AA -fast
endif


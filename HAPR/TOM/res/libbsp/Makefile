
include config.mk

SOURCES = BSP.cc 
OBJECTS = BSP.o

#
# specify BSP implementation
#

ifeq ($(BSP),SEQ)
DEFINES		+=	-DBSP_TYPE=1
endif

ifeq ($(BSP),PUB)
DEFINES		+=	-DBSP_TYPE=2
endif

ifeq ($(BSP),MPI)
DEFINES		+=	-DBSP_TYPE=3
endif

ifeq ($(BSP),SHM)
DEFINES		+=	-DBSP_TYPE=4
endif

#
# rules
#

%.o:	%.cc
	$(CC) -c $(CFLAGS) $(DEFINES) -I. $< -o $@ 

all: libbsp.a

libbsp.a:	$(SOURCES) $(OBJECTS)
	$(AR) libbsp.a $(OBJECTS)
	$(RANLIB) libbsp.a

clean:
	rm -rf *.o *~ bsp/*~ libbsp.a bench


BSP.o:	BSP.hh BSP_shm.cc BSP_mpi.cc BSP_seq.cc BSP_pub.cc TArray.cc TArray.hh TSLL.hh TSLL.cc

bench:	libbsp.a bench.cc
	$(CC) $(CFLAGS) -o bench bench.cc libbsp.a
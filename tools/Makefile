CC = gcc
CCFLAGS =
LDFLAGS = -L /opt/local/lib -l dc1394.22

all: lscam

lscam: lscam.c
	$(CC) $(CCFLAGS) $(LDFLAGS) lscam.c -o lscam
	
.cleanobj:
	rm -f *.o

clean: .cleanobj
	rm -f lscam

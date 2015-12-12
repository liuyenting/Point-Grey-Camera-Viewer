CC = gcc
CCFLAGS = -g
LDFLAGS = -L /opt/local/lib -l dc1394.22

EXECUTABLE = grab-image

all: main

main: main.c
	$(CC) $(CCFLAGS) $(LDFLAGS) main.c -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
	rm -r *.dSYM
	rm *.ppm

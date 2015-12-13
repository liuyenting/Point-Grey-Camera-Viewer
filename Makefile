export PKG_CONFIG_PATH = /opt/local/lib/PKG_CONFIG_PATH
export LD_LIBRARY_PATH = /opt/local/lib

CC = gcc
CCFLAGS = -g
CCFLAGS += `pkg-config --cflags opencv`
LDFLAGS = -L /opt/local/lib -l dc1394.22
LDFLAGS += `pkg-config --libs opencv`

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = grab-image

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
	rm -f *.ppm

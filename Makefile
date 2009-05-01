CC = gcc
CFLAGS = -Wall -c
LDFLAGS = -Wall
SOURCES = monitor.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = monitor

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -vf $(OBJECTS) $(EXECUTABLE) | sed 's/^/Removed /'

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

try: all
	touch /tmp/monitor-test /tmp/monitor-test2
	(./monitor /tmp/monitor-test /tmp/monitor-test2; echo "Monitor exited.") &
	echo "Press enter to touch test file."
	read
	touch /tmp/monitor-test2
	@sleep 0.5

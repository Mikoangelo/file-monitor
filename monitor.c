#include <stdio.h>
#include <fcntl.h>
#include <sys/event.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char *path = argv[1];
	int queue;
	int event_fd;
	struct kevent events_to_monitor;
	struct kevent event_data;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
		return 1;
	}
	
	/* Open a kernel queue. */
	queue = kqueue();
	if (queue < 0) {
		fprintf(stderr, "Could not open kernel queue: '%s'.\n", strerror(errno));
	}
	
	/* Open a file descriptor for event reading. */
	event_fd = open(path, O_EVTONLY);
	if (event_fd <= 0) {
		fprintf(stderr, "The file %s could not be opened for monitoring: '%s'.\n", path, strerror(errno));
		return 1;
	}
	
	/* Declare what we're waiting for. See kevent(2). */
	EV_SET(&events_to_monitor, event_fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_ATTRIB, 0, NULL);
	
	/* Handle one (1) event for one (1) file. Waits indefinitely. */
	int event_count = kevent(queue, &events_to_monitor, 1, &event_data, 1, NULL);
	
	if ((event_count < 0) || (event_data.flags == EV_ERROR)) {
		/* An error occurred. */
		fprintf(stderr, "An error occurred during monitoring: '%s'.\n", strerror(errno));
		return 1;
	}
	
	return 0;
}

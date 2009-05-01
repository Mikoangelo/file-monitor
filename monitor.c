#include <stdio.h>
#include <fcntl.h>
#include <sys/event.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int path_count = argc - 1;
	char **paths = &argv[1];
	int event_fds[path_count];
	struct kevent events_to_monitor[path_count];
	
	int queue;
	struct kevent event_data;
	
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <file> …\n", argv[0]);
		return 1;
	}
	
	/* Open a kernel queue. */
	queue = kqueue();
	if (queue < 0) {
		fprintf(stderr, "Could not open kernel queue: '%s'.\n", strerror(errno));
	}
	
	/* Open a file descriptor for event reading. */
	int i;
	for(i = 0; i < path_count; ++i) {
		event_fds[i] = open(paths[i], O_EVTONLY);
		if (event_fds[i] <= 0) {
			fprintf(stderr, "The file %s could not be opened for monitoring: '%s'.\n", paths[i], strerror(errno));
			return 1;
		}
		
		/* Declare what we're waiting for. See kevent(2). */
		EV_SET(&events_to_monitor[i], event_fds[i], EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_ATTRIB, 0, NULL);
	}
	
	/* Handle one (1) event for one (1) file. Waits indefinitely. */
	int event_count = kevent(queue, events_to_monitor, path_count, &event_data, 1, NULL);
	
	if ((event_count < 0) || (event_data.flags == EV_ERROR)) {
		/* An error occurred. */
		fprintf(stderr, "An error occurred during monitoring: '%s'.\n", strerror(errno));
		return 1;
	}
	
	return 0;
}

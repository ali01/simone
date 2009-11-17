#ifndef CLOCK_GET_STUB_H_HC9MY9F1
#define CLOCK_GET_STUB_H_HC9MY9F1

typedef enum {
	CLOCK_REALTIME,
	CLOCK_MONOTONIC,
	CLOCK_PROCESS_CPUTIME_ID,
	CLOCK_THREAD_CPUTIME_ID
} clockid_t;

int clock_gettime(clockid_t clk_id, struct timespec *tp);

#endif

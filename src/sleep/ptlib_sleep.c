/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "ptlib_sleep.h"

void _sleep_ns (struct timespec time_requested);

void _sleep_ns (struct timespec time_requested){
	struct timespec time_remaining;

	if (nanosleep(&time_requested, &time_remaining) != 0) {
		output(ERROR, "%s interrupted with %d.%09d seconds remaining. %s\n",
				__func__, time_remaining.tv_sec, time_remaining.tv_nsec,
				strerror(errno));
	}
}

void sleep_ms (unsigned int ms){
	struct timespec time_requested;

	time_requested.tv_sec = ms / 1000;
	time_requested.tv_nsec = (ms - (time_requested.tv_sec * 1000)) * 1000000;
	_sleep_ns(time_requested);
}

void sleep_us (unsigned int us){
	struct timespec time_requested;
	time_requested.tv_sec = us / 1000000;

	time_requested.tv_nsec = (us - (time_requested.tv_sec * 1000000)) * 1000;
	_sleep_ns(time_requested);
}

bool time_limit_reached(const struct timeval* start, long double limit)
{
	struct timeval now;
	gettimeofday(&now, 0);

	long double delta = now.tv_sec - start->tv_sec
			+ (now.tv_usec - start->tv_usec) / USEC_SEC_RATIO;

	return delta > limit;
}

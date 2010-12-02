#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	CLOCK_REALTIME,
	CLOCK_MONOTONIC,
	CLOCK_PROCESS_CPUTIME_ID,
	CLOCK_THREAD_CPUTIME_ID
} clockid_t;

int clock_gettime(clockid_t clk_id, struct timespec *tp);

#ifdef __cplusplus
}
#endif


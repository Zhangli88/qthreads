#ifdef HAVE_CONFIG_H
# include "config.h"		       /* for _GNU_SOURCE */
#endif

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>		       /* for INT_MIN & friends (according to C89) */
#include <float.h>		       /* for DBL_EPSILON (according to C89) */
#include <math.h>		       /* for fabs() */
#include <assert.h>

#include <sys/time.h>		       /* for gettimeofday() */
#include <time.h>		       /* for gettimeofday() */

#include <qthread/qutil.h>
#include "qtimer.h"

int main(int argc, char *argv[])
{
    aligned_t *ui_array, *ui_array2;
    int threads = 1;
    size_t len = 1000000, i;
    qtimer_t timer = qtimer_new();
    double cumulative_time=0.0;
    int interactive = 0;

    if (argc >= 2) {
	threads = strtol(argv[1], NULL, 0);
	if (threads < 0)
	    threads = 1;
	interactive = 1;
	printf("%i threads\n", threads);
    }
    if (argc >= 3) {
	len = strtoul(argv[2], NULL, 0);
	printf("len = %lu\n", (unsigned long)len);
    }

    qthread_init(threads);

    ui_array = calloc(len, sizeof(aligned_t));
    for (i = 0; i < len; i++) {
	ui_array[i] = random();
    }
    ui_array2 = calloc(len, sizeof(aligned_t));
    if (interactive) {
	printf("ui_array generated...\n");
    }
    for (int i=0;i<10;i++) {
	memcpy(ui_array2, ui_array, len*sizeof(aligned_t));
	qtimer_start(timer);
	qutil_aligned_qsort(qthread_self(), ui_array2, len);
	qtimer_stop(timer);
	cumulative_time += qtimer_secs(timer);
    }
    if (interactive == 1) {
	printf("sorting %lu aligned_ts took: %f seconds\n",
	       (unsigned long)len,
	       cumulative_time);
    }
    free(ui_array);

    qtimer_free(timer);

    qthread_finalize();
    return 0;
}

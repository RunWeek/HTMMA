#ifndef __HANDLE_H__
#define __HANDLE_H__

#define _GNU_SOURCE
#define __USE_GNU

#include "numap.h"
#include <pthread.h>
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <errno.h>

int numap_samping_print_test();

#endif
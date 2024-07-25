#include "init.h"

#define CPU_CORE_NUM 1
#define MMAP_PAGE_NUM 32

static struct numap_sampling_measure gSm[CPU_CORE_NUM];
struct numap_counting_measure gCm;

int gProcessFlag = 0;

__thread long long Count[3] = {0};
long long ReadSize = sizeof(long long);
__thread int fd;
__thread long long count = 0;
extern struct sample_list *samples;
FILE *fpVirtualAddr = NULL;

#define __NR_readpmudata 335
#define __NR_getperfevent 336

void to_be_profiled()
{
    long size = 50000000;
    int *data = malloc(sizeof(int) * size);
    if (data == NULL)
    {
        printf("malloc failed\n");
        exit(-1);
    }
    int res = 0;
    for (long i = 0; i < size; i++)
    {
        data[i] = i;
        res += data[i];
    }
}

void *thread_0_f(void *p)
{
    int res;
    int cpu = (int *)p;
    int run_core = sched_getcpu();

    signal(SIGUSR1, SIG_IGN); // 忽略SIGUSR1信号
    res = numap_sampling_init_measure(&gSm[cpu], 1, 2000, MMAP_PAGE_NUM);
    gSm[cpu].cpu = cpu;

    /* start sample */

    res = numap_sampling_read_start(&gSm[cpu]);
    if (res < 0)
    {
        fprintf(stderr, " -> numap_sampling_start error : %s, cnt=%ld\n", numap_error_message(res), cpu);
        return -1;
    }
    char *buff[512];

    // printf("gSm[%ld]=%ld\n", cpu, gSm[cpu].fd_per_tid[0]);
    int ret;
    struct pollfd pollfds;
    memset(&pollfds, 0, sizeof(pollfds));
    pollfds.fd = gSm[cpu].fd_per_tid[0];
    pollfds.events = POLLIN;
    for (;;)
    {
        poll(&pollfds, 1, -1);
        if (pollfds.revents & POLLIN)
        {
            printf("do something...\n");
        }

        // numap_sampling_read_stop(&gSm[cpu]);
        // numap_sampling_read_print(&gSm[cpu], 1);
        // numap_sampling_resume(&gSm[cpu]);

        // struct perf_event_mmap_page *metadata_page = gSm[cpu].metadata_pages_per_tid[0];
        // uint64_t data_tail = metadata_page->data_tail;
        // uint64_t data_head = metadata_page->data_head;
        // printf("run_core=%d, data_tail=%d, data_head=%d\n", run_core, data_tail, data_head);
    }
    // int epoll_fd, nfds;
    // struct epoll_event ev, events[10];
    // epoll_fd = epoll_create(32);
    // if (epoll_fd == -1)
    // {
    //     printf("epoll_create1 error!\n");
    //     return -1;
    // }
    // ev.data.fd = gSm[cpu].fd_per_tid[0];
    // ev.events = EPOLLIN | EPOLLET;
    // if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, gSm[cpu].fd_per_tid[0], &ev) == -1)
    // {
    //     printf("epoll_ctl error!\n");
    //     return -1;
    // }
    // for (;;)
    // {
    //     nfds = epoll_wait(epoll_fd, events, 10, -1);
    //     if (nfds == -1)
    //     {
    //         printf("epoll wait error!\n");
    //         return -1;
    //     }
    //     for (int i = 0; i < nfds; i++)
    //     {
    //         if (events[i].events & EPOLLHUP)
    //         {
    //             numap_samping_print_test();
    //         }
    //     }
    // }

    return NULL;
}

void main()
{
    for (int i = 0; i < CPU_CORE_NUM; i++)
    {
        memset(&gSm[i], 0, sizeof(struct numap_sampling_measure));
    }

    memset(&gCm, 0, sizeof(struct numap_counting_measure));
    int res = numap_init();
    if (res < 0)
    {
        fprintf(stderr, "numap_init : %s\n", numap_error_message(res));
        return -1;
    }

    // int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    // if (T0_CPU >= num_cores || T1_CPU >= num_cores)
    // {
    //     fprintf(stderr, "Can't set affinity to thread 0 to %d or to thread 1 to %d\n", T0_CPU, T1_CPU);
    //     return -1;
    // }
    // for (int i = 0; i < CPU_CORE_NUM; i++)
    // {
    //     res = numap_sampling_init_measure(&gSm[i], 1, 2000, MMAP_PAGE_NUM);
    //     gSm[i].cpu = i;
    // }

    gCm.nb_nodes = 2;
    if (res < 0)
    {
        fprintf(stderr, "numap_sampling_init error : %s\n", numap_error_message(res));
        return -1;
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < CPU_CORE_NUM; i++)
    {
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(i, &mask);
        res = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &mask);
        if (res != 0)
        {
            fprintf(stderr, "Error setting affinity to thread 0: %s\n", strerror(res));
            return -1;
        }
        pthread_t thread_0;
        if ((res = pthread_create(&thread_0, &attr, thread_0_f, (void *)i)) < 0)
        {
            fprintf(stderr, "Error creating thread 0: %d\n", res);
            return -1;
        }
    }
    while (1)
        ;
}

__attribute__((destructor)) void HandleExit(void)
{
    int res;
    res = numap_sampling_read_stop(&gSm[0]);
    if (res < 0)
    {
        printf("numap_sampling_stop error : %s\n", numap_error_message(res));
        return -1;
    }
    numap_sampling_read_print(&gSm[0], 1);
    // Stop memory read access sampling
    // for (int i = 0; i < 1; i++)
    // {
    //     int res = numap_sampling_read_stop(&gSm[i]);
    //     if (res < 0)
    //     {
    //         printf("numap_sampling_stop error : %s, cnt=%ld\n", numap_error_message(res), i);
    //         return -1;
    //     }
    // }
    // Print memory read sampling results
    // numap_sampling_read_print(&gSm, 1);
}

int numap_samping_print_test()
{
    numap_sampling_read_stop(&gSm[0]);
    numap_sampling_read_print(&gSm[0], 1);
    numap_sampling_resume(&gSm[0]);
}

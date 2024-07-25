#include <linux/perf_event.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(SYS_perf_event_open, hw_event, pid, cpu,
                  group_fd, flags);
    return ret;
}

void refresh_wrapper_handler(int signum, siginfo_t *info, void *ucontext)
{
    printf("func=%s, 111...\n", __func__);
}

int main(void)
{
    int fd;
    long long count;
    struct perf_event_attr pe;

    memset(&pe, 0, sizeof(pe));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(pe);
    pe.config = PERF_COUNT_HW_INSTRUCTIONS;
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;

    struct sigaction sigoverflow;
    memset(&sigoverflow, 0, sizeof(struct sigaction));
    sigoverflow.sa_sigaction = refresh_wrapper_handler;
    sigoverflow.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &sigoverflow, NULL) < 0)
    {
        fprintf(stderr, "could not set up signal handler\n");
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1)
    {
        fprintf(stderr, "Error opening leader %llx\n", pe.config);
        exit(EXIT_FAILURE);
    }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    fcntl(fd, F_SETSIG, SIGUSR1);
    ioctl(fd, PERF_EVENT_IOC_REFRESH, 100);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

    printf("Measuring instruction count for this printf\n");
    for (;;)
        ;

    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);

    read(fd, &count, sizeof(count));

    printf("Used %lld instructions\n", count);

    close(fd);
}

#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	struct pollfd fds[1];
	int ret;

	// 设置信号处理函数
	signal(SIGUSR1, SIG_IGN); // 忽略SIGUSR1信号

	// 设置要监视的文件描述符
	fds[0].fd = STDIN_FILENO; // 任意文件描述符都可以，这里用标准输入
	fds[0].events = POLLIN;

	printf("pid=%d\n", getpid());
	while (1)
	{
		// 使用poll等待事件
		ret = poll(fds, 1, -1);

		if (ret > 0)
		{
			if (fds[0].revents & POLLIN)
			{
				printf("Received event on file descriptor\n");
			}
		}
		else if (ret == -1)
		{
			perror("poll");
			break;
		}
	}

	return 0;
}
/*
 * File        : daemon.c
 * Author      : Vincent Cheung
 * Date        : Jan. 20, 2015
 * Description : This is used as process daemon.
 *
 * Copyright (C) Vincent Chueng<coolingfall@gmail.com>
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/system_properties.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "common.h"

#define LOG_TAG         "Daemon"
#define	MAXFILE         3
#define SLEEP_INTERVAL  2 * 60

volatile int sig_running = 1;

/* signal term handler */
static void sigterm_handler(int signo)
{
	LOGD(LOG_TAG, "handle signal: %d ", signo);
	sig_running = 0;
}

/* start daemon service */
static void start_service(char *package_name, char *service_name)
{
	/* get the sdk version */
	int version = get_version();

	pid_t pid;

	if ((pid = fork()) < 0)
	{
		exit(EXIT_SUCCESS);
	}
	else if (pid == 0)
	{
		if (package_name == NULL || service_name == NULL)
		{
			LOGE(LOG_TAG, "package name or service name is null");
			return;
		}

		char *p_name = str_stitching(package_name, "/");
		char *s_name = str_stitching(p_name, service_name);
		LOGD(LOG_TAG, "service: %s", s_name);

		if (version >= 17 || version == 0)
		{
			int ret = execlp("am", "am", "startservice",
						"--user", "0", "-n", s_name, (char *) NULL);
			LOGD(LOG_TAG, "result %d", ret);
		}
		else
		{
			execlp("am", "am", "startservice", "-n", s_name, (char *) NULL);
		}

		LOGD(LOG_TAG , "exit start-service child process");
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

int main(int argc, char *argv[])
{
	int i;
	pid_t pid;
	char *package_name = NULL;
	char *service_name = NULL;
	char *daemon_file_dir = NULL;
	int interval = SLEEP_INTERVAL;

	LOGI(LOG_TAG, "Copyright (c) 2015-2016, Vincent Cheung<coolingfall@gmail.com>");

	if (argc < 7)
	{
		LOGE(LOG_TAG, "usage: %s -p package-name -s "
		 "daemon-service-name -t interval-time", argv[0]);
		return;
	}

	for (i = 0; i < argc; i ++)
	{
		if (!strcmp("-p", argv[i]))
		{
			package_name = argv[i + 1];
			LOGD(LOG_TAG, "package name: %s", package_name);
		}

		if (!strcmp("-s", argv[i]))
		{
			service_name = argv[i + 1];
			LOGD(LOG_TAG, "service name: %s", service_name);
		}

		if (!strcmp("-t", argv[i]))
		{
			interval = atoi(argv[i + 1]);
			LOGD(LOG_TAG, "interval: %d", interval);
		}
	}

	/* package name and service name should not be null */
	if (package_name == NULL || service_name == NULL)
	{
		LOGE(LOG_TAG, "package name or service name is null");
		return;
	}

	if ((pid = fork()) < 0)
	{
		exit(EXIT_SUCCESS);
	}
	else if (pid == 0)
	{
		/* add signal */
		signal(SIGTERM, sigterm_handler);

		/* become session leader */
		setsid();
		/* change work directory */
		chdir("/");

		for (i = 0; i < MAXFILE; i ++)
		{
			close(i);
		}

		/* find pid by name and kill them */
		int pid_list[100];
		int total_num = find_pid_by_name(argv[0], pid_list);
		LOGD(LOG_TAG, "total num %d", total_num);
		for (i = 0; i < total_num; i ++)
		{
			int retval = 0;
			int daemon_pid = pid_list[i];
			if (daemon_pid > 1 && daemon_pid != getpid())
			{
				retval = kill(daemon_pid, SIGTERM);
				if (!retval)
				{
					LOGD(LOG_TAG, "kill daemon process success: %d", daemon_pid);
				}
				else
				{
					LOGD(LOG_TAG, "kill daemon process %d fail: %s", daemon_pid, strerror(errno));
					exit(EXIT_SUCCESS);
				}
			}
		}

		LOGD(LOG_TAG, "child process fork ok, daemon start: %d", getpid());

		while(sig_running)
		{
			interval = interval < SLEEP_INTERVAL ? SLEEP_INTERVAL : interval;
			select_sleep(interval, 0);

			LOGD(LOG_TAG, "check the service once, interval: %d", interval);

			/* start service */
			start_service(package_name, service_name);
		}

		exit(EXIT_SUCCESS);
	}
	else
	{
		/* parent process */
		exit(EXIT_SUCCESS);
	}
}

/*
 * File        : common.c
 * Author      : Vincent Cheung
 * Date        : Dec. 12, 2014
 * Description : Some common functions here.
 *
 * Copyright (C) Vincent Chueng<coolingfall@gmail.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/system_properties.h>

#include "common.h"

#define BUFFER_SIZE 2048

/**
 * stitch two string to one string
 *
 * @param  str1 the first string to be stitched
 * @param  str2 the second string to be stitched
 * @return      stitched string
 */
char *str_stitching(const char *str1, const char *str2)
{
	char *result;
	result = (char *) malloc(strlen(str1) + strlen(str2) + 1);
	if (!result)
	{
		return NULL;
	}

	strcpy(result, str1);
	strcat(result, str2);

    return result;
}

/* open browser with specified url */
void open_browser(char *url)
{
	/* the url cannot be null */
	if (url == NULL || strlen(url) < 4) {
		return;
	}

	/* get the sdk version */
	char value[8] = "";
	__system_property_get("ro.build.version.sdk", value);

	int version = atoi(value);
	/* is the version is greater than 17 */
	if (version >= 17 || version == 0)
	{
		execlp("am", "am", "start", "--user", "0", "-n",
				"com.android.browser/com.android.browser.BrowserActivity",
				"-a", "android.intent.action.VIEW",
				"-d", url, (char *)NULL);
	}
	else
	{
		execlp("am", "am", "start", "-n",
				"com.android.browser/com.android.browser.BrowserActivity",
				"-a", "android.intent.action.VIEW",
				"-d", url, (char *)NULL);
	}
}

/**
 * Get the version of current SDK.
 */
int get_version()
{
	char value[8] = "";
    __system_property_get("ro.build.version.sdk", value);

    return atoi(value);
}

/**
 * Find pid by process name.
 */
int find_pid_by_name(char *pid_name, int *pid_list)
{
	DIR *dir;
	struct dirent *next;
	int i = 0;
	pid_list[0] = 0;

	dir = opendir("/proc");
	if (!dir)
	{
		return 0;
	}

	while ((next = readdir(dir)) != NULL)
	{
		FILE *status;
		char proc_file_name[BUFFER_SIZE];
		char buffer[BUFFER_SIZE];
		char process_name[BUFFER_SIZE];

		/* skip ".." */
		if (strcmp(next->d_name, "..") == 0)
		{
			continue;
		}

		/* pid dir in proc is number */
		if (!isdigit(*next->d_name))
		{
			continue;
		}

		sprintf(proc_file_name, "/proc/%s/cmdline", next->d_name);
		if (!(status = fopen(proc_file_name, "r")))
		{
			continue;
		}

		if (fgets(buffer, BUFFER_SIZE - 1, status) == NULL)
		{
			fclose(status);
			continue;
		}
		fclose(status);

		/* get pid list */
		sscanf(buffer, "%[^-]", process_name);
		if (strcmp(process_name, pid_name) == 0)
		{
			pid_list[i ++] = atoi(next->d_name);
		}
	}

	if (pid_list)
    {
    	pid_list[i] = 0;
    }

    closedir(dir);

    return i;
}

/**
 * Get the process name according to pid.
 */
char *get_name_by_pid(pid_t pid)
{
	char proc_file_path[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	char *process_name;

    process_name = (char *) malloc(BUFFER_SIZE);
    if (!process_name)
    {
    	return NULL;
    }

	sprintf(proc_file_path, "/proc/%d/cmdline", pid);
	FILE *fp = fopen(proc_file_path, "r");
	if (fp != NULL)
	{
		if (fgets(buffer, BUFFER_SIZE - 1, fp) != NULL)
		{
			fclose(fp);

			sscanf(buffer, "%[^-]", process_name);
			return process_name;
		}
	}

	return NULL;
}


/**
 * Use `select` to sleep with specidied second and microsecond.
 */
void select_sleep(long sec, long msec)
{
	struct timeval timeout;

	timeout.tv_sec = sec;
	timeout.tv_usec = msec * 1000;

	select(0, NULL, NULL, NULL, &timeout);
}


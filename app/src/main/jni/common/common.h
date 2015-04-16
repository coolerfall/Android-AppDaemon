/*
 * File        : common.h
 * Author      : Vincent Cheung
 * Date        : Dec. 12, 2014
 * Description : The header file of common.c.
 *
 * Copyright (C) Vincent Chueng<coolingfall@gmail.com>
 *
 */

#ifndef	__COMMON_H__
#define __COMMON_H__

#include "log.h"

char *str_stitching(const char *str1, const char *str2);
int get_version();
void open_browser(char *url);
int find_pid_by_name(char *pid_name, int *pid_list);
char *get_name_by_pid(pid_t pid);
void select_sleep(long sec, long msec);

#endif

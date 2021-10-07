#pragma once


#include<iostream>
#include<string>
#include <direct.h> //ls  get_cwd
#include <stdio.h>
#include "tools.h"

using namespace std;

string format_time(__time64_t time_stamp);

bool cmp_name(file_info_t a, file_info_t b);

bool cmp_name_reverse(file_info_t a, file_info_t b);

bool cmp_time(file_info_t a, file_info_t b);

long ls2vector(const char * to_search, string dir_path, string cmd, vector<dir_info_t> &dir_info_list);

string format_st_mode(int st_mode);

int ls(string cmd, vector<string> path_list);

//int ls(int argc, char *argv[]);
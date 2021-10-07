#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>
#include <io.h> //ls finddata
#include <windows.h> //ls cp CopyFile
#include <vector> //ls cp
#include <ShObjIdl.h>//cp 快捷方式
#include <stdlib.h> /* realpath */
#include <limits.h> /* PATH_MAX */

using namespace std;

//获得绝对路径 并 部分格式化
string get_abs_path(const char * dir);

//依照cmd和flag将执行windows的rm和rmdir命令
int exec_rm(string path, string cmd, char flag);

//匹配带有通配符的字符串
int match_string(string m_str, string w_str);

void ResolveShortcut(LPCOLESTR LNKFILE);

char* get_cwd();

void split(const string& s, vector<string>& tokens, char delim = ' ');

struct file_info_t {
	_finddata_t files_basic;
	struct stat files_detail;
};

struct dir_info_t {
	long total_size;
	string dir_path;
	vector< file_info_t> files_info;
};
// find中的参数格式
struct param {
	string param_name;
	string param_value;
	string type; //普通的参数type为 - ，bool算符参数type参数为它自己
};
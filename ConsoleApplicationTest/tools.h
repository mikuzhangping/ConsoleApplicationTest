#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>
#include <io.h> //ls finddata
#include <windows.h> //ls cp CopyFile
#include <vector> //ls cp
#include <ShObjIdl.h>//cp ��ݷ�ʽ
#include <stdlib.h> /* realpath */
#include <limits.h> /* PATH_MAX */

using namespace std;

//��þ���·�� �� ���ָ�ʽ��
string get_abs_path(const char * dir);

//����cmd��flag��ִ��windows��rm��rmdir����
int exec_rm(string path, string cmd, char flag);

//ƥ�����ͨ������ַ���
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
// find�еĲ�����ʽ
struct param {
	string param_name;
	string param_value;
	string type; //��ͨ�Ĳ���typeΪ - ��bool�������type����Ϊ���Լ�
};
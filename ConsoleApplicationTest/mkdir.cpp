#include<iostream>
#include<string>
#include <stdio.h>
#include <vector> //ls cp
#include "tools.h"
using namespace std;

int mkdir(string cmd, vector<string> path_list) {
	for (int i = 0; i < path_list.size(); i++) {
		string dir_abs = get_abs_path(path_list[i].data());
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(dir_abs.data(), drive, dir, fname, ext);
		string father_path = drive;
		father_path += dir;

		string mkdir_cmd = "mkdir ";
		if ((_access(father_path.data(), 0) == -1) && string::npos == cmd.find("p")) {
			cout << "使用p参数";
			return -1;
		}
		mkdir_cmd += dir_abs;
		if (system(mkdir_cmd.data())) {
			cout << "目的地址不合法" << endl;
			return -1;
		}
	}
	return 0;
}

int mkdir(int argc, char *argv[])
{
	//const char * source = "C://Users//17331\\Desktop\\tes\\mkdir_t";//源文件
	//const char * destination = "C://Users//17331\\Desktop//";//目标文件

	string cmd;
	for (int i = 1; i < argc; i++) {
		if ('-' == argv[i][0]) {
			string str_cdm = argv[i];
			str_cdm = str_cdm.substr(1, str_cdm.length());
			cmd += str_cdm;
		}
	}
	cmd += "";
	vector<string> path_list;
	for (int i = 1; i < argc; i++) {
		if ('-' != argv[i][0]) {
			path_list.push_back(argv[i]);
		}
	}
	//path_list.push_back(source);
	//path_list.push_back(destination);
	//string dest_dir = path_list.back();

	mkdir(cmd, path_list);

	return 0;
}
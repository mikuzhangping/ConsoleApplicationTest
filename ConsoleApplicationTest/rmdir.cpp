#include<iostream>
#include<string>
#include <stdio.h>
#include <vector> //ls cp
#include "tools.h"
using namespace std;

int rmdir(string cmd, vector<string> path_list) {
	for (int i = 0; i < path_list.size(); i++) {
		string dir_abs = get_abs_path(path_list[i].data());
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		string father_path = dir_abs;


		string rmdir_cmd;
		do
		{
			rmdir_cmd = "rmdir ";
			rmdir_cmd += father_path;
			cout << rmdir_cmd << endl;
			_splitpath_s(father_path.data(), drive, dir, fname, ext);
			father_path = drive;
			father_path += dir;
			father_path = get_abs_path(father_path.data());

		} while ((!system(rmdir_cmd.data())) && string::npos != cmd.find("p"));

	}
	return 0;
}

int rmdir(int argc, char *argv[])
{
	//const char * source = "C://Users//17331\\Desktop\\test\\mkdir_t\\a\\b";//源文件
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
	rmdir(cmd, path_list);
	return 0;
}
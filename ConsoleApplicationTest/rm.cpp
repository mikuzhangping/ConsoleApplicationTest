#include<iostream>
#include<string>
#include <stdio.h>
#include <io.h> //ls finddata
#include <windows.h> //ls cp 各种常量
#include <vector> //ls cp
#include <stdlib.h> /* realpath */
#include <limits.h> /* PATH_MAX */
#include "tools.h"
using namespace std;


int rm(string cmd, vector<string> path_list) {
	for (int i = 0; i < path_list.size(); i++) {
		string dir_abs = get_abs_path(path_list[i].data());

		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(dir_abs.data(), drive, dir, fname, ext);
		string real_path = drive;
		real_path += dir;

		long handle;
		struct _finddata_t file_basic;
		handle = _findfirst(dir_abs.data(), &file_basic);

		real_path += file_basic.name;
		if (FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib &&
			(file_basic.name[0] != '.' || strlen(file_basic.name) != 1 && file_basic.name[1] != '.')) {
			exec_rm(real_path, cmd, 'd');
		}
		else if (!(FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib)) {
			exec_rm(real_path, cmd, 'f');
		}
		while (!_findnext(handle, &file_basic)) {

			_splitpath_s(dir_abs.data(), drive, dir, fname, ext);
			real_path = drive;
			real_path += dir;
			real_path += file_basic.name;
			if (FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib &&
				(file_basic.name[0] != '.' || strlen(file_basic.name) != 1 && file_basic.name[1] != '.')) {
				exec_rm(real_path, cmd, 'd');
			}
			else if (!(FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib)) {
				exec_rm(real_path, cmd, 'f');
			}
		}
	}
	return 0;
}


int rm(int argc, char *argv[])
{
	//const char * source = "C://Users//17331\\Desktop\\test\\gittest//*";//源文件
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
	rm(cmd, path_list);
	return 0;
}

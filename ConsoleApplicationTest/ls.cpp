#include<iostream>
#include<string>
#include <direct.h> //ls  get_cwd
#include <stdio.h>
#include <io.h> //ls finddata
//#include <windows.h> //  FILE_ATTRIBUTE_DIRECTORY
#include <iomanip> //ls localtime_s
#include <sstream> //ls
#include <vector> //ls
#include <algorithm> //ls sort
#include "tools.h"
using namespace std;

//vector<_finddata_t> files_basic;

//获取当前的工作路径（没有用了）


string format_time(__time64_t time_stamp) {
	time_t rawtime;
	struct tm  timeinfo;
	rawtime = time_stamp;
	localtime_s(&timeinfo, &rawtime);
	//std::cout << std::put_time(&timeinfo, "[%F %T]") << std::endl;
	std::stringstream ss;
	ss << std::put_time(&timeinfo, "[%F %T]");
	std::string str = ss.str();
	return str;
}

bool cmp_name(file_info_t a, file_info_t b) {
	char * a_name = a.files_basic.name;
	char * b_name = b.files_basic.name;
	_strlwr_s(a_name, strlen(a_name) + 1);
	_strlwr_s(b_name, strlen(b_name) + 1);
	//a_name = _strlwr(a_name);
	return strcmp(a_name, b_name) < 0;
}

bool cmp_name_reverse(file_info_t a, file_info_t b) {
	char * a_name = a.files_basic.name;
	char * b_name = b.files_basic.name;
	_strlwr_s(a_name, strlen(a_name) + 1);
	_strlwr_s(b_name, strlen(b_name) + 1);
	//a_name = _strlwr(a_name);
	return strcmp(a_name, b_name) > 0;
}

bool cmp_time(file_info_t a, file_info_t b) {
	return a.files_basic.time_create < b.files_basic.time_create;
}

//将遍历文件夹的文件信息存入dir_info_list
//优化：to_search 参数可以省略，信息从 dir_path中可以得到。
long ls2vector(const char * to_search, string dir_path, string cmd, vector<dir_info_t> &dir_info_list) {
	//cout << "\033[1m\033[34m" << cmd << "\033[0m" << endl;
	dir_info_t dir_info;
	long total_size;
	int old_flag = -1; //当前目录是否已经被访问过一次了
	for (int i = 0; i < dir_info_list.size(); i++) {
		if (dir_info_list[i].dir_path == dir_path)
		{
			dir_info = dir_info_list[i];
			total_size = dir_info_list[i].total_size;
			old_flag = i;
		}
	}
	if (old_flag == -1) {
		dir_info.dir_path = dir_path;
		total_size = 0;
	}
	
	long handle;                                         //用于查找的句柄
	struct _finddata_t file_basic;                        //文件信息的结构体
	handle = _findfirst(to_search, &file_basic);         //第一次查找
	if (-1 == handle) {
		return -1;
	}
		
	//去除字符串尾部的\\* 加上文件的名字方面后面调用
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath_s(to_search, drive, dir, fname, ext);

	string new_search = drive;
	//cout << new_search.find("\\*");
	new_search += dir;
	new_search += "\\";

	string filepath;
	filepath += new_search;

	filepath += file_basic.name;
	struct stat file_detail;
	stat(filepath.data(), &file_detail);
	struct file_info_t file_info;
	file_info.files_basic = file_basic;
	file_info.files_detail = file_detail;
	dir_info.files_info.push_back(file_info);
	filepath = new_search;
	total_size += file_basic.size;
	while (!_findnext(handle, &file_basic))               //循环查找其他符合的文件，知道找不到其他的为止
	{
		filepath += file_basic.name;
		stat(filepath.data(), &file_detail);
		file_info.files_basic = file_basic;
		file_info.files_detail = file_detail;
		dir_info.files_info.push_back(file_info);
		filepath = new_search;
		total_size += file_basic.size;
		if (FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib && 
			(file_basic.name[0] != '.' || strlen(file_basic.name) != 1 && file_basic.name[1] != '.')&&
			cmd.npos != cmd.find("R")) {
			filepath += file_basic.name;
			filepath += "\\*";
			ls2vector(filepath.data(), dir_path+"\\"+ file_basic.name, cmd, dir_info_list);
			filepath = new_search;
		}
	}
	sort(dir_info.files_info.begin(), dir_info.files_info.begin(), cmp_name);
	if (cmd.npos != cmd.find("r")) {
		sort(dir_info.files_info.begin(), dir_info.files_info.begin(), cmp_name_reverse);
	}
	if (cmd.npos != cmd.find("t")) {
		sort(dir_info.files_info.begin(), dir_info.files_info.begin(), cmp_time);
	}
	_findclose(handle);
	dir_info.total_size = total_size;
	if (old_flag == -1) {
		dir_info_list.insert(dir_info_list.begin(), dir_info);
	}
	else
		dir_info_list[old_flag] = dir_info;

	//dir_info_list.push_back(dir_info);
	return total_size;
}

string format_st_mode(int st_mode) {
	string result;
	int type = 0777000;
	type &= st_mode;
	switch (type)
	{
	case S_IFREG:
		result += "-";
		break;
	case S_IFDIR :
		result += "d";
		break;
	case S_IFCHR:
		result += "c";
		break;
	case _S_IFIFO:
		result += "p";
		break;
	default:
		break;
	}
	for (int i = 0; i < 3; i++) {
		int mask = 0400;
		for (int j = 0; j < 3; j++) {
			
			if (mask&st_mode) {
				switch (j)
				{
				case 0:
					result += "r";
					break;
				case 1:
					result += "w";
					break;
				case 2:
					result += "x";
					break;
				default:
					break;
				}
			}
			else {
				result += "-";
			}
			mask = mask >> 1;
			
		}
		st_mode = st_mode << 3;
	}

	return result;
}

int ls(string cmd, vector<string> path_list) {
	//cout << cmd << endl;
	//char * buffer = get_cwd();
	//vector<file_info_t> file_info_list;
	vector<dir_info_t> dir_info_list;
	for (int i = 0; i < path_list.size(); i++) {
		string to_search;
		string dir_path = path_list[i];
		struct stat file_detail_main;
		stat(path_list[i].data(), &file_detail_main);
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(path_list[i].data(), drive, dir, fname, ext);
		if ((_access(path_list[i].data(), 0)) != -1) {
			if (!(S_IFDIR&file_detail_main.st_mode)) {
				dir_path = drive;
				dir_path += dir;
				to_search = path_list[i];
			}
			else
				to_search = path_list[i] + "\\*";
			if (dir_path.back() == '\\')
				dir_path.back() = 0;
			long total_size = ls2vector(to_search.data(), dir_path, cmd, dir_info_list);
		}
		else
		{
			cout << "未能找到目录：" << path_list[i] << endl;
		}
		//to_search += "\\.\\*";
	}

	for (int i = 0; i < dir_info_list.size(); i++) {
		if (dir_info_list.size() > 2) {
			cout << "\n";
			cout << dir_info_list[i].dir_path << ":" << endl;
		}
		if (cmd.npos != cmd.find("l")) {
			cout << "total size is:" << dir_info_list[i].total_size << endl;
		}
		vector<file_info_t> file_info_list = dir_info_list[i].files_info;
		for (int j = 0; j < file_info_list.size(); j++)
		{
			//string file_name = file_info_list[i].files_basic.name;
			string creat_time = format_time(file_info_list[j].files_basic.time_create);
			string st_mode_str = format_st_mode(file_info_list[j].files_detail.st_mode);
			if (file_info_list[j].files_basic.name[0] == '.') {
				if ((strlen(file_info_list[j].files_basic.name) == 1 || file_info_list[j].files_basic.name[1] == '.') && cmd.npos != cmd.find("A")) {
					continue;
				}
				else if (cmd.npos == cmd.find("a") && cmd.npos == cmd.find("A")) {
					continue;
				}
			}
			if (cmd.npos != cmd.find("l")) {
				if ((S_IFDIR&file_info_list[j].files_detail.st_mode) != 0) {
					cout << st_mode_str << ".\t" <<
						file_info_list[j].files_detail.st_nlink << "\t0\t0\t" <<
						file_info_list[j].files_detail.st_size << "\t" <<
						creat_time << "\t" << "\033[1;34m" <<
						file_info_list[j].files_basic.name;
					if (cmd.npos != cmd.find("F")) {
						cout << "\\\n" << "\033[0m";
					}
					else {
						cout << "\n" << "\033[0m";
					}
					/*cout << st_mode_str << ".\t" <<
						file_info_list[j].files_detail.st_nlink << "\t0\t0\t" <<
						file_info_list[j].files_detail.st_size << "\t" <<
						creat_time << "\t" <<
						file_info_list[j].files_basic.name;
					if (cmd.npos != cmd.find("F")) {
						cout << "\\\n";
					}
					else {
						cout << "\n";
					}*/
				}

				else if ((S_IEXEC&file_info_list[j].files_detail.st_mode) != 0) {
					cout << st_mode_str << ".\t" <<
						file_info_list[j].files_detail.st_nlink << "\t0\t0\t" <<
						file_info_list[j].files_detail.st_size << "\t" <<
						creat_time << "\t" << "\033[1;32m" <<
						file_info_list[j].files_basic.name;
					if (cmd.npos != cmd.find("F")) {
						cout << "*\n" << "\033[0m";
					}
					else {
						cout << "\n" << "\033[0m";
					}
					/*cout << st_mode_str << ".\t" <<
						file_info_list[j].files_detail.st_nlink << "\t0\t0\t" <<
						file_info_list[j].files_detail.st_size << "\t" <<
						creat_time << "\t" <<
						file_info_list[j].files_basic.name;
					if (cmd.npos != cmd.find("F")) {
						cout << "*\n";
					}
					else {
						cout << "\n";
					}*/
				}
				else {
					cout << st_mode_str << ".\t" <<
						file_info_list[j].files_detail.st_nlink << "\t0\t0\t" <<
						file_info_list[j].files_detail.st_size << "\t" <<
						creat_time << "\t" <<
						file_info_list[j].files_basic.name << "\n";
				}
			}
			else {
				if ((S_IFDIR&file_info_list[j].files_detail.st_mode) != 0) {
					cout << "\033[1;34m" << file_info_list[j].files_basic.name;
					if (cmd.npos != cmd.find("F")) {
						cout << "\\\n" << "\033[0m";
					}
					else {
						cout << "\n" << "\033[0m";
					}
					/*cout << file_info_list[j].files_basic.name;
					if (cmd.npos != cmd.find("F")) {
						cout << "\\\n";
					}
					else {
						cout << "\n";
					}*/
				}

				else if ((S_IEXEC&file_info_list[j].files_detail.st_mode) != 0) {
					cout << "\033[1;32m" << file_info_list[j].files_basic.name;
					if (cmd.npos != cmd.find("F")) {
						cout << "*\n" << "\033[0m";
					}
					else {
						cout << "\n" << "\033[0m";
					}
					/*cout << file_info_list[j].files_basic.name;
					if (cmd.npos != cmd.find("F")) {
						cout << "\*\n";
					}
					else {
						cout << "\n";
					}*/
				}
				else {
					cout << file_info_list[j].files_basic.name << endl;
				}

			}
		}
	}
	return 0;

}

int ls(int argc, char *argv[]) {
	string cmd;
	for (int i = 1; i < argc; i++) {
		if ('-' == argv[i][0]) {
			string str_cdm = argv[i];
			str_cdm = str_cdm.substr(1, str_cdm.length());
			cmd += str_cdm;
		}
	}
	//cmd += "l";
	vector<string> path_list;
	for (int i = 1; i < argc; i++) {
		if ('-' != argv[i][0]) {
			path_list.push_back(argv[i]);
		}
	}
	//path_list.push_back(".\\Debug\\cat.obj");
	//path_list.push_back(".\\Debug\\ls.obj");
	//path_list.push_back(".\\Debug");
	if (path_list.size() == 0) {
		path_list.push_back(".");
	}
	ls(cmd, path_list);
	return 0;
}
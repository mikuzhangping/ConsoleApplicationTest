// ConsoleApplicationTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<string>
#include <stdio.h>
#include <io.h> //ls finddata
//#include <windows.h> //ls cp 各种常量
#include <iomanip> //ls localtime_s
#include <vector> //ls cp
#include <algorithm> //ls sort
#include <stdlib.h> /* realpath */
#include <limits.h> /* PATH_MAX */
#include "tools.h"
#include "ls.h"
using namespace std;


//真正执行每一步的运算
void calculate(vector<char> & operator_list, vector<bool> & operand_list) {
	//op 栈顶的运算符
	if (operator_list.empty()) {
		return;
	}
	char op = operator_list.back();
	operator_list.pop_back();
	//temp1操作数一 temp2操作数二
	bool temp1, temp2;
	switch (op) {
	case '-':

		//！1=0 等价于1-1=0，！0=1等价于1-0=1。number中存的是48+x 所以用96+1
		temp1 = !operand_list.back();
		operand_list.pop_back();
		operand_list.push_back(temp1);
		break;
	case '+':

		temp1 = operand_list.back();
		operand_list.pop_back();
		temp2 = operand_list.back();
		operand_list.pop_back();
		operand_list.push_back(temp1 || temp2);
		break;
	case '*':
		temp1 = operand_list.back();
		operand_list.pop_back();
		temp2 = operand_list.back();
		operand_list.pop_back();
		operand_list.push_back(temp1 && temp2);
		break;

	}
}

bool bool_calculate(string input_str) {
	vector<char>  operator_list;
	vector<bool>  operand_list;
	for (int i = 0; i < input_str.length(); i++) {
		//在b字符数组里面查找相应的变量，并把对应的c给压进栈
		char p = input_str[i];
		switch (p) {
		case '1':
			operand_list.push_back(true);
			break;
		case '0':
			operand_list.push_back(false);
			break;
		case '+':
			if (operator_list.size() == 0)
				operator_list.push_back('+');
			else {
				calculate(operator_list, operand_list);
				i--;
			}
			break;

		case '*':
			if (operator_list.size() == 0 || operator_list.back() == '+')
				operator_list.push_back('*');
			else {
				calculate(operator_list, operand_list);
				i--;
			}
			break;
		case '-':
			if (operator_list.size() == 0 || operator_list.back() == '+' || operator_list.back() == '*')
				operator_list.push_back('-');
			else {
				calculate(operator_list, operand_list);
				i--;
			}
			break;
		default:
			calculate(operator_list, operand_list);
		}
	}
	return operand_list.back();
}

string format_param(_finddata_t file_basic, struct stat file_detail, vector<param> param_list) {
	string result = "";
	for (int i = 0; i < int(param_list.size()); i++) {

		//优化：这里用hash（str）配合switch更好
		if (param_list[i].type[0] == '-') {
			if (param_list[i].param_name == "-amin") {
				time_t myt = time(NULL);
				int min = abs(stoi(param_list[i].param_value));
				time_t key_time = myt - min * 60;
				//int time_top = myt - (min+1) * 60;
				switch (param_list[i].param_value[0])
				{
				case '-':
					if (file_basic.time_access >= key_time)
						result += "1";
					else
						result += "0";
					break;
				case '+':
					if (file_basic.time_access < key_time)
						result += "1";
					else
						result += "0";
					break;

				default:
					if (file_basic.time_access >= key_time)
						result += "1";
					else
						result += "0";
					break;
				}
			}
			else if (param_list[i].param_name == "-anewer") {
				struct stat cmp_file;
				stat(param_list[i].param_value.data(), &cmp_file);
				time_t key_time = cmp_file.st_atime;
				if (file_basic.time_access >= key_time)
					result += "1";
				else
					result += "0";

			}
			else if (param_list[i].param_name == "-atime") {
				time_t myt = time(NULL);
				int day = abs(stoi(param_list[i].param_value));
				time_t key_time = myt - day * 86400;
				//int time_top = myt - (min+1) * 60;
				switch (param_list[i].param_value[0])
				{
				case '-':
					if (file_basic.time_access >= key_time)
						result += "1";
					else
						result += "0";
					break;
				case '+':
					if (file_basic.time_access < key_time)
						result += "1";
					else
						result += "0";
					break;

				default:
					if (file_basic.time_access >= key_time)
						result += "1";
					else
						result += "0";
					break;
				}
			}
			else if (param_list[i].param_name == "-cmin") {
				time_t myt = time(NULL);
				int min = abs(stoi(param_list[i].param_value));
				time_t key_time = myt - min * 60;
				//int time_top = myt - (min+1) * 60;
				switch (param_list[i].param_value[0])
				{
				case '-':
					if (file_basic.time_create >= key_time)
						result += "1";
					else
						result += "0";
					break;
				case '+':
					if (file_basic.time_create < key_time)
						result += "1";
					else
						result += "0";
					break;

				default:
					if (file_basic.time_create >= key_time)
						result += "1";
					else
						result += "0";
					break;
				}
			}
			else if (param_list[i].param_name == "-cnewer") {
				struct stat cmp_file;
				stat(param_list[i].param_value.data(), &cmp_file);
				time_t key_time = cmp_file.st_ctime;
				if (file_basic.time_create >= key_time)
					result += "1";
				else
					result += "0";
			}
			else if (param_list[i].param_name == "-ctime") {
				time_t myt = time(NULL);
				int day = abs(stoi(param_list[i].param_value));
				time_t key_time = myt - day * 86400;
				//int time_top = myt - (min+1) * 60;
				switch (param_list[i].param_value[0])
				{
				case '-':
					if (file_basic.time_create >= key_time)
						result += "1";
					else
						result += "0";
					break;
				case '+':
					if (file_basic.time_create < key_time)
						result += "1";
					else
						result += "0";
					break;

				default:
					if (file_basic.time_create >= key_time)
						result += "1";
					else
						result += "0";
					break;
				}
			}
			else if (param_list[i].param_name == "-empty") {
				if (file_detail.st_size == 0)
					result += "1";
				else
					result += "0";
			}
			else if (param_list[i].param_name == "-name") {
				if (match_string(file_basic.name, param_list[i].param_value))
					result += "1";
				else
					result += "0";

			}
			else if (param_list[i].param_name == "-iname") {
				string file_name = file_basic.name;
				transform(file_name.begin(), file_name.end(), file_name.begin(), ::toupper);
				transform(param_list[i].param_value.begin(), param_list[i].param_value.end(), param_list[i].param_value.begin(), ::toupper);
				if (match_string(file_name, param_list[i].param_value))
					result += "1";
				else
					result += "0";
			}
			else if (param_list[i].param_name == "-size") {
				char unit = param_list[i].param_value.back();
				string number = param_list[i].param_value.substr(0, param_list[i].param_value.length() - 1);
				int n = abs(stoi(number));
				long max_size;
				long min_size;
				switch (unit)
				{
				case 'g':
				case 'G':
					max_size = n * 1024 * 1024 * 1024;
					min_size = (n - 1) * 1024 * 1024 * 1024;
					break;
				case 'm':
				case 'M':
					max_size = n * 1024 * 1024;
					min_size = (n - 1) * 1024 * 1024;
					break;
				case 'k':
				case 'K':
					max_size = n * 1024;
					min_size = (n - 1) * 1024;
					break;
				default:
					break;
				}
				switch (param_list[i].param_value[0])
				{
				case '-':
					if (file_basic.size <= min_size)
						result += "1";
					else
						result += "0";
					break;
				case '+':
					if (file_basic.size > max_size)
						result += "1";
					else
						result += "0";
					break;

				default:
					if (file_basic.size <= max_size && file_basic.size > min_size)
						result += "1";
					else
						result += "0";
					break;

				}
			}
			else if (param_list[i].param_name == "-type") {
				switch (param_list[i].param_value[0])
				{
				case 'd':
					if (file_detail.st_mode&S_IFDIR)
						result += "1";
					else
						result += "0";
					break;
				case 'f':
					if (file_detail.st_mode&S_IFREG)
						result += "1";
					else
						result += "0";
					break;
					break;
				default:
					break;
				}
			}
			else if (param_list[i].param_name == "-perm") {
				string otc_str = "0" + param_list[i].param_value;
				char * ptr;
				long dec_int = strtol(otc_str.data(), &ptr, 8);
				//int perm = file_detail.st_mode & 0777;
				if ((file_detail.st_mode & 0777) == dec_int)
					result += "1";
				else
					result += "0";
			}
		}
		else {
			if (param_list[i].type == string("and") && i - 1 >= 0 && param_list[i - 1].type == "-" && i < int(param_list.size()) && param_list[i + 1].type == "-")
				result += "*";
			else if (param_list[i].type == string("or") && i - 1 >= 0 && param_list[i - 1].type == "-" && i < int(param_list.size()) && param_list[i + 1].type == "-")
				result += "+";
			else if (param_list[i].type == string("not") && i < int(param_list.size()) && param_list[i + 1].type == "-")
				result += "-";
			else {
				cout << "逻辑运算符错误";
				return "0";
			}
		}
	}
	return result;
}


int find2vector(const char * to_search, string dir_path, vector<param> param_list, vector<dir_info_t> &dir_info_list) {
	dir_info_t dir_info;
	dir_info.dir_path = dir_path;
	//long total_size = 0;
	long handle;                                         //用于查找的句柄
	struct _finddata_t file_basic;                      //文件信息的结构体
	handle = _findfirst(to_search, &file_basic);       //第一次查找
	if (-1 == handle) {
		return -1;
	}

	//去除字符串尾部的\\* 加上文件的名字方面后面调用
	string new_search = dir_path;
	//cout << new_search.find("\\*");
	//new_search = new_search.substr(0, new_search.find("\\*"));
	new_search += "\\";
	string filepath;
	filepath += new_search;

	filepath += file_basic.name;
	struct stat file_detail;
	stat(filepath.data(), &file_detail);

	string format_result = format_param(file_basic, file_detail, param_list);
	//bool tset = bool_calculate(format_result + "#");

	struct file_info_t file_info;
	if (bool_calculate(format_result + "#")) {
		file_info.files_basic = file_basic;
		file_info.files_detail = file_detail;
		dir_info.files_info.push_back(file_info);
	}
	filepath = new_search;
	while (!_findnext(handle, &file_basic))               //循环查找其他符合的文件，知道找不到其他的为止
	{
		filepath += file_basic.name;
		stat(filepath.data(), &file_detail);
		format_result = format_param(file_basic, file_detail, param_list);
		bool_calculate(format_result + "#");

		if (bool_calculate(format_result + "#")) {
			file_info.files_basic = file_basic;
			file_info.files_detail = file_detail;
			dir_info.files_info.push_back(file_info);
		}
		filepath = new_search;
		if (FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib &&
			(file_basic.name[0] != '.' || strlen(file_basic.name) != 1 && file_basic.name[1] != '.')) {
			filepath += file_basic.name;
			filepath += "\\*";
			find2vector(filepath.data(), dir_path + "\\" + file_basic.name, param_list, dir_info_list);
			filepath = new_search;
		}
		//
	}
	_findclose(handle);
	dir_info.total_size = 0;
	if (dir_info.files_info.size() > 0) {
		dir_info_list.insert(dir_info_list.begin(), dir_info);
	}
	
	//dir_info_list.push_back(dir_info);
	return 0;
}

//这里的exec_cmd 考虑用cmd 和pathlist代替
int find(vector<string> path_list, vector<param> param_list, string exec_cmd) {
	vector<dir_info_t> find_result;
	for (int i = 0; i < int(path_list.size()); i++) {
		string dir_abs = path_list[i].data();
		string to_search = path_list[i];
		if (string::npos != dir_abs.find("*") || string::npos != dir_abs.find("?")) {
			string dir_abs = path_list[i].data();
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			_splitpath_s(dir_abs.data(), drive, dir, fname, ext);
			string real_path = drive;
			real_path += dir;
			int j;
			for (j = 0; j < real_path.length(); j++) {
				if (dir_abs[j] == '/') {
					dir_abs[j] = '\\';
				}
			}
			if (dir_abs[j - 1] == '\\') {
				dir_abs[j - 1] = 0;
			}
			if ((_access(real_path.data(), 0)) != -1) {

				find2vector(to_search.data(), real_path.data(), param_list, find_result);
			}
		}
		else {
			int j;
			for (j = 0; j < path_list[i].length(); j++) {
				if (path_list[i][j] == '/') {
					path_list[i][j] = '\\';
				}
			}
			if (path_list[i][j - 1] == '\\') {
				path_list[i][j - 1] = 0;
			}
			if ((_access(path_list[i].data(), 0)) != -1) {
				const char * format_path = path_list[i].data();
				to_search = format_path;
				to_search += "\\*";
				find2vector(to_search.data(), path_list[i].data(), param_list, find_result);

			}
		}
	}
	for (int i = 0; i < find_result.size(); i++) {
		vector<file_info_t> file_info_list = find_result[i].files_info;
		string path_l = "";

		for (int j = 0; j < file_info_list.size(); j++) {
			if (exec_cmd != "") {
				string path = find_result[i].dir_path + "\\" + file_info_list[j].files_basic.name;
				//char * img2 = path.data();
				//new_param[param_num + j] = new char[path.length() + 1];
				//strcpy_s(new_param[param_num + j], path.length() + 1, path.data());
				path_l += path + " ";
			}
			else
				cout << find_result[i].dir_path << "\\" << file_info_list[j].files_basic.name << endl;
		}
		if (exec_cmd != "") {
			path_l.back() = 0;
			exec_cmd.replace(exec_cmd.find("{}"), 4, path_l.data());
			exec_cmd = "C:\\Users\\17331\\Desktop\\bin\\" + exec_cmd;
			if (system(exec_cmd.data())) {
				cout << "失败";
			}
		}
		//if (strlen(new_param[0]) != 1)
			//ls(param_num + file_info_list.size(), new_param);
	}
	return 0;
}


int find(int argc, char *argv[])
{
//system("ls");
//argc = 9;
//	//argv = new char *[3];
//	argv[0] = "0";
//	argv[1] = "./Debug/";
//	argv[2] = "-name";
//	argv[3] = "*.obj";
//	argv[4] = "-exec";
//	argv[5] = "ls";
//	argv[6] = "-l";
//	argv[7] = "{}";
//	argv[8] = "\\";
//	argv[3] = "*.obj";
//	argv[4] = "-size";
//	argv[5] = "1m";
//	argv[6] = "-o";
//	argv[7] = "-perm";
//	argv[8] = "666";

	vector<string> path_list;
	for (int i = 1; i < argc; i++) {
		if ('-' != argv[i][0] && '!' != argv[i][0]) {
			path_list.push_back(argv[i]);
		}
		else {
			break;
		}

	}
	vector<param> param_list;
	string exec_cmd = "";
	char ** new_param = new char*[256];
	//char ** new_param = NULL;
	new_param[0] = (char *)"0";
	int param_num;
	
	for (int i = 1; i < argc; i++) {
		if ('-' == argv[i][0] || '!' == argv[i][0]) {
			string str_cdm = argv[i];
			param param_data;
			//str_cdm = str_cdm.substr(1, str_cdm.length());
			if (str_cdm == string("-and") || str_cdm == string("-a")) {
				param_data.type = "and";
				param_data.param_name = str_cdm;
				//param_data.type = "not";
				param_list.push_back(param_data);
			}
			else if (str_cdm == string("-o") || str_cdm == string("-or")) {
				param_data.type = "or";
				param_data.param_name = str_cdm;
				//param_data.type = "not";
				param_list.push_back(param_data);
			}
			else if (str_cdm == string("-not") || str_cdm == string("!")) {
				param_data.type = "not";
				param_data.param_name = str_cdm;
				//param_data.type = "not";
				param_list.push_back(param_data);
			}
			else if (str_cdm == string("-empty")) {
				param_data.type = "-";
				param_data.param_name = str_cdm;
				//param_data.type = "not";
				param_list.push_back(param_data);
			}
			else if (str_cdm == string("-exec")) {
				int cmd_num = argc - i - 1;
				for (int j = 0; j < cmd_num; j++) {
					exec_cmd += argv[i + j + 1];
					exec_cmd += " ";
				}
				exec_cmd.back() = 0;
				param_num = argc - i - 3;
				new_param[0] = argv[i + 0 + 1];
				for (int j = 1; j < param_num; j++) {
					new_param[j] = argv[i + j+1];
				}
				break;
			}
			else if (i + 1 < argc && ('-' != argv[i + 1][0] || isdigit(argv[i + 1][1]))) {
				if (!param_list.empty() && param_list.back().type == "-") {
					param_data.param_name = "-and";
					//param_data.param_value = argv[i + 1];
					param_data.type = "and";
					param_list.push_back(param_data);
				}
				param_data.param_name = str_cdm;
				param_data.param_value = argv[i + 1];
				param_data.type = "-";
				param_list.push_back(param_data);
				i++;
			}
			else {
				cout << "参数错误";
				return -1;
			}
		}
	}

	find(path_list, param_list, exec_cmd);
	
	return 0;
}



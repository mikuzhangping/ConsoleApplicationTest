// ConsoleApplicationTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<string>
#include <stdio.h>
#include <vector>
#include<sstream>
#include <windows.h>
#include <Lmcons.h>
#include "tools.h"
#include"cat.h"
#include"cd.h"
#include"cp.h"
#include"find.h"
#include"ls.h"
#include"mkdir.h"
#include"mv.h"
#include"rm.h"
#include"rmdir.h"
using namespace std;


int mai(int argc, char *argv[])
{
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName((LPWSTR)username, &username_len);

	TCHAR computername[MAX_COMPUTERNAME_LENGTH + 2];
	DWORD buf_size = sizeof computername - 1;
	GetComputerName(computername, &buf_size);

	string cwd = get_cwd();
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char dirname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath_s(cwd.data(), drive, dir, dirname, ext);

	while (true) {
		cout << "[" << username << "@" << computername << " " << dirname << "]$";
		//cout << cwd << "%";
		//string input_cmd = "find ./Debug -name *.obj -exec ls -l {} \\";
		string input_cmd;
		getline(cin, input_cmd);
		if (input_cmd == "")
			continue;
		vector<string> tokens;
		split(input_cmd, tokens);

		if (tokens[0] == "cat" ||
			tokens[0] == "cp" ||
			tokens[0] == "ls" ||
			tokens[0] == "mkdir" ||
			tokens[0] == "mv" ||
			tokens[0] == "rm" ||
			tokens[0] == "rmdir"
			) {
			string cmd;
			for (int i = 1; i < tokens.size(); i++) {
				if ('-' == tokens[i][0]) {
					string str_cdm = tokens[i];
					str_cdm = str_cdm.substr(1, str_cdm.length());
					cmd += str_cdm;
				}
			}
			//cmd += "Rf";
			vector<string> path_list;
			for (int i = 1; i < tokens.size(); i++) {
				if ('-' != tokens[i][0]) {
					path_list.push_back(tokens[i]);
				}
			}
			if (tokens[0] == "cat")
				cat(cmd, path_list);
			else if (tokens[0] == "cp")
				cp(cmd, path_list);
			else if (tokens[0] == "ls") {
				if (path_list.size() == 0) {
					path_list.push_back(".");
				}
				ls(cmd, path_list);
			}
				
			else if (tokens[0] == "mkdir")
				mkdir(cmd, path_list);
			else if (tokens[0] == "mv")
				mv(cmd, path_list);
			else if (tokens[0] == "rm")
				rm(cmd, path_list);
			else if (tokens[0] == "rmdir")
				rmdir(cmd, path_list);
		}
		else if (tokens[0] == "cd" ) {
			if (tokens.size() > 1) {
				string dest_dir = get_abs_path(tokens[1].data());
				cd(dest_dir, cwd);
				_splitpath_s(cwd.data(), drive, dir, dirname, ext);
			}
		}
		else if (tokens[0] == "find") {
			vector<string> path_list;
			for (int i = 1; i < tokens.size(); i++) {
				if ('-' != tokens[i][0] && '!' != tokens[i][0]) {
					path_list.push_back(tokens[i]);
				}
				else {
					break;
				}

			}
			vector<param> param_list;
			string exec_cmd = "";
			//char ** new_param = new char*[256];
			//char ** new_param = NULL;
			//new_param[0] = "0";
			int param_num;
			for (int i = 1; i < tokens.size(); i++) {
				if ('-' == tokens[i][0] || '!' == tokens[i][0]) {
					string str_cdm = tokens[i];
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
						//int cmd_num = i + 1;
						while (tokens[i + 1] != "\\") {
							exec_cmd += tokens[i + 1];
							exec_cmd += " ";
							i++;
						}
						i++;
						exec_cmd.back() = 0;
						/*param_num = argc - i - 3;
						new_param[0] = argv[i + 0 + 1];
						for (int j = 1; j < param_num; j++) {
							new_param[j] = argv[i + j + 1];
						}*/
						//break;
					}
					else if (i + 1 < tokens.size() && ('-' != tokens[i + 1][0] || isdigit(tokens[i + 1][1]))) {
						if (!param_list.empty() && param_list.back().type == "-") {
							param_data.param_name = "-and";
							//param_data.param_value = argv[i + 1];
							param_data.type = "and";
							param_list.push_back(param_data);
						}
						param_data.param_name = str_cdm;
						param_data.param_value = tokens[i + 1];
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
		}
		else if (tokens[0] == "exit")
			return 0;
		else {
			cout << "未知命令" << tokens[0] << endl;
			//return -1;
		}
	}

	return 0;
}

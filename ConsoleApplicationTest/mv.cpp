// ConsoleApplicationTest.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include<iostream>
#include<string>
#include <stdio.h>
#include <vector> //ls cp
#include "tools.h"
using namespace std;


int mv(string cmd, vector<string> path_list) {
	string dest_dir = path_list.back();
	for (int i = 0; i < path_list.size() - 1; i++) {
		string dir_abs = get_abs_path(path_list[i].data());
		string mv_cmd = "move ";
		for (int i = 0; i < cmd.size(); i++) {
			switch (cmd[i])
			{
			case 'i':
				mv_cmd += "/-Y";
				break;
			case 'f':
				mv_cmd += "/Y";
				break;
			default:
				break;
			}
		}
		mv_cmd += dir_abs + " " + dest_dir;
		if (system(mv_cmd.data())) {
			cout << "Ŀ�ĵ�ַ���Ϸ�" << endl;
			return -1;
		}
	}
	return 0;
}


int mv(int argc, char *argv[])
{
	//const char * source = "C://Users//17331\\Desktop\\test\\gittest";//Դ�ļ�
	//const char * destination = "C://Users//17331\\Desktop//";//Ŀ���ļ�

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
	mv(cmd, path_list);
	return 0;
}

#include<iostream>
#include<string>
#include <stdio.h>
#include <vector> //ls cp
#include<direct.h>
#include "tools.h"
using namespace std;

int cd(string dest_dir, char * & filename) {
	if (!_chdir(dest_dir.data())) {
		string cwd = dest_dir;
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(cwd.data(), drive, dir, fname, ext);
		filename = fname;
	}
	else
		cout << "Ŀ¼������\n";
	return 0;
}

int cd(string dest_dir, string & cwd) {
	//cout << "�䶯ǰĿ¼�ǣ�" << get_cwd() << endl;
	//string dest_dir = source;
	if (!_chdir(dest_dir.data()))
		cwd = dest_dir;
	else
		cout << "Ŀ¼������\n";
	//SetCurrentDirectory(TEXT(dest_dir.data()));
	//string cd_cmd = "cd /D ";

	//cd_cmd += get_abs_path(dest_dir.data());
	//system("cd /D c:/temp");
	//system("pause");
	/*if (system(cd_cmd.data())) {
		cout << "Ŀ�ĵ�ַ����" << endl;
		return -1;
	}*/
	//cout << "�䶯��Ŀ¼�ǣ�" << get_cwd() << endl;
	return 0;
}

int cd(int argc, char *argv[])
{
	
	//const char * source = "/Temp";//Դ�ļ�
	//const char * destination = "C://Users//17331\\Desktop//";//Ŀ���ļ�
	string dest_dir = get_abs_path(argv[1]);
	string cwd = "";
	//string dest_dir = get_abs_path(destination);
	cd(dest_dir, cwd);
	return 0;
}

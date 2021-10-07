#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>
#include <io.h> //ls finddata
//#include <windows.h> //ls cp CopyFile
#include <vector> //ls cp
#include <ShObjIdl.h>//cp 快捷方式
#include <stdlib.h> /* realpath */
#include <limits.h> /* PATH_MAX */
#include "tools.h"
using namespace std;
// 创建快捷方式
void CreateShortcut(LPCSTR TARGET, LPCOLESTR LNKFILE)
{
	LPVOID lp = NULL;
	CoInitialize(lp);
	IShellLink *psl;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&psl));
	if (SUCCEEDED(hr))
	{
		psl->SetPath(TARGET);
		psl->SetDescription(TEXT("This is just an example."));

		IPersistFile *ppf;
		hr = psl->QueryInterface(&ppf);
		if (SUCCEEDED(hr))
		{
			hr = ppf->Save(LNKFILE, TRUE);
			if (SUCCEEDED(hr))
				cout << TEXT("创建快捷方式成功") << endl;
			else
				cout << TEXT("创建快捷方式失败") << endl;
			ppf->Release();
		}
		else
			cout << TEXT("创建快捷方式时遇到系统错误") << endl;

		psl->Release();
	}
	else
		cout << TEXT("操作失败") << endl;
}




//复制单个文件到目标目录
int copyfile(const char * source, const char * dest_dir, string cmd) {
	long handle;                                         //用于查找的句柄
	struct _finddata_t file_basic;                        //文件信息的结构体
	handle = _findfirst(source, &file_basic);
	if (handle == -1) {
		cout << source << " 不存在" << endl;
		return -1;
	}
	string dest_path = dest_dir;
	dest_path += "\\";
	dest_path += file_basic.name;
	if ((_access(dest_path.data(), 0) != -1) && (string::npos == cmd.find("f") && string::npos == cmd.find("F"))) {
		cout << "是否覆盖：" << dest_path << endl;
		cout << "输入Y/y确认" << endl;
		string in;
		cin >> in;
		if (string::npos == in.find("y") || string::npos != in.find("Y"))
			return -1;
	}

	//判断文件是否相同
	FILE* File_sour;
	fopen_s(&File_sour, source, "rb");
	FILE* File_dest;
	fopen_s(&File_dest, dest_path.data(), "wb");
	if (!File_dest) {
		cout << '\'' << source << '\'' << "and" << '\'' << dest_dir << '\'' << "is same file" << endl;
		return -1;
	}
	fclose(File_dest);
	fclose(File_sour);

	ifstream in;
	ofstream out;
	in.open(source, ios::binary);		//读取文件
	out.open(dest_path, ios::binary);
	//cout << (in == out);
	if (!in)
	{
		cerr << "打开失败！" << endl;
		return 0;
	}
	if (!out)
	{
		cerr << "打开失败！" << endl;
		return 0;
	}
	out << in.rdbuf();
	//filebuf * xx;
	//while (in >> xx)
	//{
	//	out << xx;
	//}
	//out << endl;
	out.close();
	in.close();
	return 0;
}


int copy_dir(const char * sour_dir, const char * dest_dir, string cmd) {
	struct stat file_detail;
	stat(dest_dir, &file_detail);
	if (!(S_IFDIR&file_detail.st_mode)) {
		string cmd_mkdir = "mkdir ";
		cmd_mkdir += dest_dir;
		if (system(cmd_mkdir.data())) {
			cout << "目的地址不合法" << endl;
			return -1;
		}
		//return -1;
	}
	string to_search;
	to_search += sour_dir;
	to_search += "\\*";
	long handle;
	struct _finddata_t file_basic;
	handle = _findfirst(to_search.data(), &file_basic);
	if (-1 == handle)
		return -1;
	while (!_findnext(handle, &file_basic)) {
		if (FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib &&
			(file_basic.name[0] != '.' || strlen(file_basic.name) != 1 && file_basic.name[1] != '.')) {
			string sour_dir_new = sour_dir;
			sour_dir_new += "\\";
			sour_dir_new += file_basic.name;
			string dest_dir_new = dest_dir;
			dest_dir_new += "\\";
			dest_dir_new += file_basic.name;
			copy_dir(sour_dir_new.data(), dest_dir_new.data(), cmd);

		}
		else if (!(FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib)) {
			string sour_path = sour_dir;
			sour_path += "\\";
			sour_path += file_basic.name;
			int result = copyfile(sour_path.data(), dest_dir, cmd);
		}
	}
	//对找到的文件进行复制，如果是文件夹就递归调用

	return 0;
}

int cp(string cmd, vector<string> path_list) {
	
	string dest_dir = path_list.back();
	struct stat file_detail;
	stat(dest_dir.data(), &file_detail);
	string dest_dir_abs = get_abs_path(dest_dir.data());
	if (!(S_IFDIR&file_detail.st_mode)) {
		string cmd_mkdir = "mkdir " + dest_dir_abs;
		if (system(cmd_mkdir.data())) {
			cout << "目的地址不合法" << endl;
			return -1;
		}
		//return -1;
	}
	/*else if ((_access(dest_dir.data(), 0)) == -1) {
		cout << "目的地址不存在" << endl;
	}*/
	else {
		for (int i = 0; i < path_list.size() - 1; i++) {

			string sour_dir_abs = get_abs_path(path_list[i].data());
			if (string::npos != dest_dir_abs.find(sour_dir_abs)) {
				cout << sour_dir_abs << endl;

				cout << "源地址不合法";
				return -1;
			}
			long handle;
			struct _finddata_t file_basic;
			handle = _findfirst(sour_dir_abs.data(), &file_basic);

			if (FILE_ATTRIBUTE_DIRECTORY&file_basic.attrib) {
				if (string::npos != cmd.find("R") || string::npos != cmd.find("r")) {
					//cout << "dir" << " ";
					dest_dir_abs += "\\";
					dest_dir_abs += file_basic.name;
					if (!dest_dir_abs.compare(sour_dir_abs)) {
						//cout << "same";
						cout << "输入不合法";
						return -1;
					}
					copy_dir(sour_dir_abs.data(), dest_dir_abs.data(), cmd);

				}
				else {
					cout << "使用R参数";
					return -1;
				}

			}
			else {
				if (string::npos != cmd.find("l") || string::npos != cmd.find("L")) {
					dest_dir_abs += "\\";
					dest_dir_abs += file_basic.name;
					dest_dir_abs += ".lnk";
					LPCSTR pszSrc = dest_dir_abs.c_str();
					int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
					wchar_t* pwszDst = new wchar_t[nLen];
					MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
					LPCOLESTR linkfile = pwszDst;    //L的作用wide_char 每个字符变成两个字节的Unicode

					CreateShortcut(sour_dir_abs.data(), linkfile);
				}
				else
					copyfile(path_list[i].data(), dest_dir.data(), cmd);
				//CopyFile(source, destination, FALSE);
			}
		}
	}

	//LPCOLESTR linkfile = L"C://Users//17331//Desktop//text.lnk";
	//copyfile(source, destination);
	//CreateShortcut(source, linkfile);
	//ResolveShortcut(linkfile);
	//CopyFile(source, destination, FALSE);//false代表覆盖，true不覆盖 实现了d和p
	return 0;
}

int cp(int argc, char *argv[])
{
	const char * source = "C://Users//17331\\Desktop//gittest";//源文件
	const char * destination = "C://Users//17331\\Desktop//test";//目标文件

	string cmd;
	for (int i = 1; i < argc; i++) {
		if ('-' == argv[i][0]) {
			string str_cdm = argv[i];
			str_cdm = str_cdm.substr(1, str_cdm.length());
			cmd += str_cdm;
		}
	}
	//cmd += "Rf";
	vector<string> path_list;
	for (int i = 1; i < argc; i++) {
		if ('-' != argv[i][0]) {
			path_list.push_back(argv[i]);
		}
	}
	//path_list.push_back(source);
	//path_list.push_back(destination);
	cp(cmd, path_list);
	return 0;
}
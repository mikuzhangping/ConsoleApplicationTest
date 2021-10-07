#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>
#include <direct.h> //ls  get_cwd
#include <io.h> //ls finddata
#include <windows.h> //ls cp CopyFile
#include <vector> //ls cp
#include <ShObjIdl.h>//cp ��ݷ�ʽ
#include <stdlib.h> /* realpath */
#include <limits.h> /* PATH_MAX */
#include "tools.h"
using namespace std;

int match_string(string m_str, string w_str) //match wildcard ͨ���
{
	int m_len = m_str.size();
	int w_len = w_str.size();
	vector<vector<int> > b_dp(w_len + 1, vector<int>(m_len + 1, 0));
	//���һ��һ����Ϊ��ʼ��ֵ����
	b_dp[0][0] = 1;
	for (int i = 1; i <= w_len; i++)
	{
		char ch = w_str[i - 1];
		//����ÿ��ѭ���ĳ�ֵ�������ǺŲ���������λʱ��ƥ���ַ����ĳ�ֵ��Ϊfalse
			b_dp[i][0] = b_dp[i - 1][0] && (ch == '*');
		for (int j = 1; j <= m_len; j++)
		{
			char ch2 = m_str[j - 1];
			if (ch == '*')
				b_dp[i][j] = b_dp[i - 1][j] || b_dp[i][j - 1]; //��ƥ���ַ�Ϊ*��ʱ��״̬ȡ��������״̬�����״̬��ֵ
			else
				b_dp[i][j] = b_dp[i - 1][j - 1] && (ch == '?' || ch2 == ch);//��������һ�κͱ���
		}
	}
	return b_dp[w_len][m_len];
}

string get_abs_path(const char * dir) {
	char dir_abs[MAX_PATH] = "";
	_fullpath(dir_abs, dir, MAX_PATH);
	int i;
	for (i = 0; dir_abs[i] != 0 && i < MAX_PATH; i++) {
		if (dir_abs[i] == '/') {
			dir_abs[i] = '\\';
		}
	}
	if (dir_abs[i - 1] == '\\') {
		dir_abs[i - 1] = 0;
	}
	string result = dir_abs;
	return result;
}

int exec_rm(string path, string cmd, char flag) {
	string rm_cmd = "del ";
	string rmdir_cmd = "rmdir ";
	if (flag == 'f') {
		for (int i = 0; i < cmd.size(); i++) {
			switch (cmd[i])
			{
			case 'i':
				rm_cmd += "/p";
				break;
			case 'f':
				rm_cmd += "/f /q";
				break;
			case 'r':
				rm_cmd += "/s";
				break;
			default:
				break;
			}
		}
		rm_cmd += " " + path;
		cout << rm_cmd << endl;
		if (system(rm_cmd.data())) {
			cout << "ɾ��ʧ��" << endl;
			return -1;
		}
	}
	else if (flag == 'd') {
		for (int i = 0; i < cmd.size(); i++) {
			switch (cmd[i])
			{
			case 'f':
				rmdir_cmd += "/q";
				break;
			case 'r':
				rmdir_cmd += "/s";
				break;
			default:
				break;
			}

		}
		rmdir_cmd += " " + path;
		cout << rmdir_cmd << endl;
		if (system(rmdir_cmd.data())) {
			cout << "ɾ��ʧ��" << endl;
			return -1;
		}
	}
	return 0;
}

// ������ݷ�ʽ
void ResolveShortcut(LPCOLESTR LNKFILE)
{
	LPVOID lp = NULL;
	CoInitialize(lp);
	IShellLink *psl;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&psl));
	if (SUCCEEDED(hr))
	{
		IPersistFile *ppf;
		hr = psl->QueryInterface(&ppf);
		if (SUCCEEDED(hr))
		{
			hr = ppf->Load(LNKFILE, STGM_READ);
			if (SUCCEEDED(hr))
			{
				TCHAR szPath[MAX_PATH];
				TCHAR szDesc[INFOTIPSIZE];
				hr = psl->GetPath(szPath, MAX_PATH, NULL, SLGP_RAWPATH);
				if (SUCCEEDED(hr))
					cout << TEXT("��ݷ�ʽָ����ļ�: ") << szPath << endl;
				else
					cout << TEXT("��ȡ��ݷ�ʽָ����ļ�ʧ�ܣ�") << endl;
				hr = psl->GetDescription(szDesc, INFOTIPSIZE);
				if (SUCCEEDED(hr))
					cout << TEXT("����: ") << szDesc << endl;
				else
					cout << TEXT("��ȡ��ݷ�ʽ����ʧ�ܣ�") << endl;
			}
			else
				cout << TEXT("���ļ�ʧ��") << endl;
			ppf->Release();
		}
		else
			cout << TEXT("��ȡ�ļ�ʱ����ϵͳ����") << endl;
		psl->Release();
	}
	else
		cout << TEXT("����ʧ��") << endl;
}

char* get_cwd() {
	char *buffer;
	//const char *to_search = "C:\\Users\\17331\\source\\repos\\ConsoleApplicationTest\\*";
	//Ҳ���Խ�buffer��Ϊ�������
	if ((buffer = _getcwd(NULL, 0)) == NULL)
	{
		perror("getcwd error");
	}

	return buffer;
}

void split(const string& s, vector<string>& tokens, char delim) {
	tokens.clear();
	auto string_find_first_not = [s, delim](size_t pos = 0) -> size_t {
		for (size_t i = pos; i < s.size(); i++) {
			if (s[i] != delim) return i;
		}
		return string::npos;
	};
	size_t lastPos = string_find_first_not(0);
	size_t pos = s.find(delim, lastPos);
	while (lastPos != string::npos) {
		tokens.emplace_back(s.substr(lastPos, pos - lastPos));
		lastPos = string_find_first_not(pos);
		pos = s.find(delim, lastPos);
	}
}
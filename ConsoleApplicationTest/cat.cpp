#include<iostream>
#include<string.h>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
using namespace std;
//constexpr auto MAXLEN = 80;

int cat(string cmd, vector<string> path_list) {
	if (path_list.size() > 0) {
		int number = 0;
		for (int i = 0; i < path_list.size(); i++) {

			//�ļ�������
			const char * path = path_list[i].data();
			ifstream file;
			file.open(path);
			cout << path << endl;
			if (!file) {
				cout << "open error" << endl;
				exit(1);
			}
			string line;
			int empty_line_flag = 1;
			while (getline(file, line)) // line�в�����ÿ�еĻ��з�  
			{
				//�����v���� ��ô ���Ǵ�ӡ�ַ��滻
				if (line.npos != cmd.find("v")) {
					while (line.npos != line.find(" "))
					{
						line = line.replace(line.find(" "), 1, "\\0");
					}
					while (line.npos != line.find("	"))
					{
						line = line.replace(line.find("	"), 1, "\\t");
					}
				}

				if (!line.empty()) {
					empty_line_flag = 1;
					//�����b��n��ô������ǿ��е�ͬʱҪnumber++
					if (line.npos != cmd.find("b") || line.npos != cmd.find("n")) {
						number++;
						cout << number << " " << line << endl;
					}
					else
					{
						cout << line << "" << endl;
					}
				}
				else if (empty_line_flag != 0 || line.npos == cmd.find("s")) {
					//������������empty_line_flag = 0 ������һ�����оͲ��������
					empty_line_flag = 0;
					if (line.npos != cmd.find("n")) {
						//ͬ����ֻҪ�����ͬʱ��n���� ��ônumber++
						number++;
						cout << number << " " << line << endl;
					}
					else
					{
						cout << line << endl;
					}
				}


			}
		}
	}
	else {
		printf(
			"ʹ�÷�ʽ��cat [-AbeEnstTuv] [--help] [--version] fileName\n"
			"˵�����ѵ��������Ӻ󴫵����������өĻ��� > fileName ����һ��������\n"
			"������\n"
			"-n �� --number �� 1 ��ʼ������������������\n"
			"-b �� --number-nonblank �� -n ���ƣ�ֻ�������ڿհ��в����\n"
			"-s �� --squeeze-blank �������������������ϵĿհ��У��ʹ���Ϊһ�еĿհ���\n"
			"-v �� --show-nonprinting\n"
			"������\n"
			"cat -n textfile1 > textfile2 �� textfile1 �ĵ������ݼ����кź����� textfile2 ���������\n"
			"cat -b textfile1 textfile2 >> textfile3 �� textfile1 �� textfile2 �ĵ������ݼ����кţ��հ��в��ӣ�֮�����ݸ��ӵ� textfile3 �\n"
		);
	}
	return 0;
}

int cat(int argc, char **argv) {

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

	cat(cmd, path_list);
	
	return 0;
}

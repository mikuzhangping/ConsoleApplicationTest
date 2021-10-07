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

			//文件名处理
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
			while (getline(file, line)) // line中不包括每行的换行符  
			{
				//如果有v参数 那么 将非打印字符替换
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
					//如果有b或n那么在输出非空行的同时要number++
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
					//遇到空行设置empty_line_flag = 0 这样下一个空行就不用输出了
					empty_line_flag = 0;
					if (line.npos != cmd.find("n")) {
						//同样的只要输出的同时有n参数 那么number++
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
			"使用方式：cat [-AbeEnstTuv] [--help] [--version] fileName\n"
			"说明：把档案串连接后传到基本输出（萤幕或加 > fileName 到另一个档案）\n"
			"参数：\n"
			"-n 或 --number 由 1 开始对所有输出的行数编号\n"
			"-b 或 --number-nonblank 和 -n 相似，只不过对于空白行不编号\n"
			"-s 或 --squeeze-blank 当遇到有连续两行以上的空白行，就代换为一行的空白行\n"
			"-v 或 --show-nonprinting\n"
			"范例：\n"
			"cat -n textfile1 > textfile2 把 textfile1 的档案内容加上行号后输入 textfile2 这个档案里\n"
			"cat -b textfile1 textfile2 >> textfile3 把 textfile1 和 textfile2 的档案内容加上行号（空白行不加）之后将内容附加到 textfile3 里。\n"
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

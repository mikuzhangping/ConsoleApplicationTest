// C++WithFun.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <memory>

std::unique_ptr<char[]> unicode_to_utf8(uint16_t unicode) {
	const int k_buffer_length = 3;
	auto buffer = std::make_unique<char[]>(k_buffer_length);
	buffer[0] = 0xE0 | (unicode >> 12);
	buffer[1] = 0x80 | ((unicode >> 6) & 0x3F);
	buffer[2] = 0x80 | (unicode & 0x3F);
	return buffer;
}

void write_all_chinese_charter() {
	const auto k_chinese_encoding_begin = 0x4E00;
	const auto k_chinese_encoding_end = 0x9FFF;
	const auto k_output_filename = "test_01.txt";

	//实例化一个 out对象，是不是不记得了。。。
	std::ofstream out(k_output_filename, std::ios::out | std::ios::binary);
	for (int i = k_chinese_encoding_begin; i <= k_chinese_encoding_end; i++) {
		uint16_t unicode = i;
		auto buffer = unicode_to_utf8(unicode);
		out.write(buffer.get(), 3);
		out.write("\n", 1);
	}

	out.close();
}

int main() {
	char ch = '我';
	int chInt = '我';
	char gbk[4] = "我";
	char utf8[4] = { 0xE4, 0xB8, 0x80 ,gbk[2] };
	std::cout << std::hex << chInt << std::endl;
	std::cout << std::hex << '我' << std::endl;
	std::cout << utf8 << std::endl;
	
}
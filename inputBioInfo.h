#pragma once
# include <string>
# include <fstream>
# include <iostream>
# include <sstream>
# include <algorithm>

# include "biology.h"
#include "tools.h"

/*
* 定义读入生物信息的类，该类包含一个公共读取静态函数，以及一些私有辅助函数。
*/
class inputBioInfo
{

public:

	inputBioInfo();
	~inputBioInfo();

	// 输入文件名称/路径，输出一个数据结构 vector，用来存储所有读取到的生物信息
	static BIO_Type read(const std::string& path);

private:

	// 一些辅助函数，验证读入的生物信息是否合法。
	static bool check_name(const std::string& s);
	static bool check_integer(const std::string& s);
	static bool check_attribute(const std::string& s);
	static bool check_path(const std::string& s);
	static bool is_alpha(char c);
	static bool is_digit(char c);
	static std::string& to_lower(std::string& s);

};


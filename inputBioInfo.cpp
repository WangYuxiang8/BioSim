#include "inputBioInfo.h"

inputBioInfo::inputBioInfo()
{
}

inputBioInfo::~inputBioInfo()
{
}

BIO_Type inputBioInfo::read(const std::string& path)
{
	BIO_Type bio;

	std::ifstream infile;
	int error_line = 0, sum_line = 0;

	infile.open(path, std::ios::in);
	if (!infile.is_open())
	{
		const std::string msg = "Fail to open creature file: " + path;
		throw std::exception(msg.c_str());
	}

	std::string strline;
	while (!infile.eof())
	{
		getline(infile, strline);
		// 是否读成功
		if (infile.fail() && !infile.eof()) {
			throw std::exception("Read creature file error!");
		}
		if (strline == "") continue;	// 跳过空行

		std::stringstream ss(strline);
		std::string token;
		std::vector<std::string> v;

		// 用逗号分隔字符串
		while (std::getline(ss, token, ','))
			v.push_back(to_lower(token));

		sum_line += 1;
		error_line += 1;

		// 判断本行数据是否齐全，或其他检查？
		if (v.size() != 6) {
			std::cout << "Error line " << sum_line << ", missing field." << std::endl;
		}
		else if (!check_name(v[0]))
		{
			std::cout << "Error line " << sum_line << ", name field error." << std::endl;
		}
		else if (!check_integer(v[1]) || !check_integer(v[2]) || !check_integer(v[3]))
		{
			std::cout << "Error line " << sum_line << ", strength/speed/life field error." << std::endl;
		}
		else if (!check_attribute(v[4]))
		{
			std::cout << "Error line " << sum_line << ", attribute field error." << std::endl;
		}
		else if (!check_path(v[5]))
		{
			std::cout << "Error line " << sum_line << ", image path field error." << std::endl;
		}
		else {
			error_line -= 1;
			biology b(sum_line-1, v[0], atoi(v[1].c_str()), atoi(v[2].c_str()), atoi(v[3].c_str()), v[4], v[5]);
			bio.emplace_back(std::move(b));
		}
	}
	infile.close();

	// 打印信息
	std::cout << "All lines: " << sum_line 
		<< ", Error lines: " << error_line 
		<< ", Correct lines: " << (sum_line - error_line) 
		<< std::endl;
	for (const auto &b : bio)
	{
		std::cout << "name: " << b.get_name() 
			<< ", Strength: " << b.get_strength()
			<< ", Speed: " << b.get_speed() 
			<< ", Life: " << b.get_life() 
			<< ", Attribute: " << b.get_attribute() 
			<< ", Image: " << b.get_image_path() 
			<< std::endl;
	}

	return bio;
}

bool inputBioInfo::check_name(const std::string& s)
{
	if (s.empty()) return false;
	for (char c : s)
	{
		if (!is_alpha(c) && c != ' ') return false;
	}
	return true;
}

bool inputBioInfo::check_integer(const std::string& s)
{	
	if (s.empty() || s[0] == '-') return false;
	for (char c : s)
	{
		if (!is_digit(c)) return false;
	}
	return true;
}

bool inputBioInfo::check_attribute(const std::string& s)
{
	for (char c : s)
	{
		if (!is_alpha(c) && c != ' ' && c != '_' && !is_digit(c)) return false;
	}
	return true;
}

bool inputBioInfo::check_path(const std::string& s)
{
	if (s.empty()) return false;
	char c1;
	char c2 = s[0];
	for (int i = 0; i < s.size(); i++)
	{
		c1 = c2;
		c2 = s[i];
		if (!is_alpha(c2) && c2 != '/' && c2 != '.' && c2 != '_' && !is_digit(c2)) return false;
		// 两个连续的 ‘/’ 也是错误
		if (c1 == '/' && c2 == '/') return false;
	}
	return true;
}

bool inputBioInfo::is_alpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return true;
	else return false;
}

bool inputBioInfo::is_digit(char c)
{
	if (c >= '0' && c <= '9') return true;
	else return false;
}

std::string& inputBioInfo::to_lower(std::string& s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

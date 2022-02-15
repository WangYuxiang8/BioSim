#pragma once
# include <string>
# include <fstream>
# include <iostream>
# include <sstream>
# include <algorithm>

# include "biology.h"
#include "tools.h"

/*
* �������������Ϣ���࣬�������һ��������ȡ��̬�������Լ�һЩ˽�и���������
*/
class inputBioInfo
{

public:

	inputBioInfo();
	~inputBioInfo();

	// �����ļ�����/·�������һ�����ݽṹ vector�������洢���ж�ȡ����������Ϣ
	static BIO_Type read(const std::string& path);

private:

	// һЩ������������֤�����������Ϣ�Ƿ�Ϸ���
	static bool check_name(const std::string& s);
	static bool check_integer(const std::string& s);
	static bool check_attribute(const std::string& s);
	static bool check_path(const std::string& s);
	static bool is_alpha(char c);
	static bool is_digit(char c);
	static std::string& to_lower(std::string& s);

};


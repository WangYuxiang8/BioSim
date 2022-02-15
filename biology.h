#pragma once
# include <string>
# include <vector>
# include <sstream>
# include <iostream>
# include <math.h>
#include "image.h"
#include "StateFsm.h"
#include "tools.h"

/*
�����࣬������ʾһ������ĸ�������

�����Ƕ�Ӧ�����������ֶ���Ϣ��
	name - ��������
	strength - ����
	speed - �ٶ�
	life - ����
	attribute - ����
	image - ͼ��
*/

class biology
{
public:

	biology(unsigned int id, 
		const std::string& name,
		unsigned int strength,
		unsigned int speed,
		unsigned int life,
		const std::string& attr, 
		const std::string& image);
	~biology();

	unsigned int get_id() const;
	const std::string& get_name() const;
	unsigned int get_strength() const;
	unsigned int get_speed() const;
	unsigned int get_life() const;
	const std::string&  get_attribute() const;
	const std::string&  get_image_path() const;
	unsigned char get_type() const;

private:

	// �����ֶ�
	unsigned int id;	// ����
	const std::string name;
	unsigned int strength;
	unsigned int speed;
	unsigned int life;
	const std::string attr_str;
	const std::string image_path;
	unsigned char type;	// ˮ����½��

};

class ActiveBiology : public biology
{
public:
	ActiveBiology(const biology& other, unsigned int uid, unsigned int x, unsigned int y);
	~ActiveBiology();

	bool operator == (const ActiveBiology& other);

	unsigned int get_uid() const;
	// ��ȡ�����ã���ǰ����
	unsigned int getX() const;
	unsigned int getY() const;
	float getXF() const;
	float getYF() const;
	void set_loc(unsigned int a, unsigned int b);
	void set_loc(float a, float b);
	// ��ȡ�����ã�ʣ������ֵ
	unsigned int get_LD() const;
	void set_LD(int ld);
	short get_dead() const;
	void set_dead(short d);

	// ��ʼFSMһ��ģ��
	void start_fsm(Model& model, Present& present);

private:
	unsigned int unique_id;	// Ψһ���
	unsigned int LD;		// ʣ������ֵ
	float x, y;				// ����
	short dead;				// �Ƿ�������10��

	std::unique_ptr<IFsmCallback> fsm;	// ״̬��
};

typedef std::shared_ptr<ActiveBiology> ActiveBiologyPtr;
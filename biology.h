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
生物类，用来表示一个生物的各种属性

以下是对应的生物数据字段信息：
	name - 生物名称
	strength - 力量
	speed - 速度
	life - 寿命
	attribute - 属征
	image - 图像
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

	// 生物字段
	unsigned int id;	// 索引
	const std::string name;
	unsigned int strength;
	unsigned int speed;
	unsigned int life;
	const std::string attr_str;
	const std::string image_path;
	unsigned char type;	// 水生或陆生

};

class ActiveBiology : public biology
{
public:
	ActiveBiology(const biology& other, unsigned int uid, unsigned int x, unsigned int y);
	~ActiveBiology();

	bool operator == (const ActiveBiology& other);

	unsigned int get_uid() const;
	// 获取（设置）当前坐标
	unsigned int getX() const;
	unsigned int getY() const;
	float getXF() const;
	float getYF() const;
	void set_loc(unsigned int a, unsigned int b);
	void set_loc(float a, float b);
	// 获取（设置）剩余生命值
	unsigned int get_LD() const;
	void set_LD(int ld);
	short get_dead() const;
	void set_dead(short d);

	// 开始FSM一步模拟
	void start_fsm(Model& model, Present& present);

private:
	unsigned int unique_id;	// 唯一编号
	unsigned int LD;		// 剩余生命值
	float x, y;				// 坐标
	short dead;				// 是否死亡，10步

	std::unique_ptr<IFsmCallback> fsm;	// 状态机
};

typedef std::shared_ptr<ActiveBiology> ActiveBiologyPtr;
#pragma once
#ifndef tile_H
#define tile_H

#include <qlabel.h>
#include <qpainter.h>
#include <QMouseEvent>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "image.h"
#include "biology.h"
#include "tools.h"

class tile
{
public:

	tile(uint terrain);
	~tile();

	uint get_terrain() const;

	// 获取正在活动生物
	ActiveBiology& get_active_bio(unsigned int index);
	ActiveBiologyPtr get_active_bio(ActiveBiology& abio);
	// 获取正在活动生物数量
	unsigned int get_active_bio_num() const;
	// 加入一个活动状态的生物
	void add_active_bio(ActiveBiologyPtr& abio);
	// 删除传入的活动状态的生物
	void del_active_bio(ActiveBiologyPtr& abio);
	void del_active_bio(ActiveBiology& bio);

	bool is_select() const;
	void set_selected(bool b);

	// 当前图块是否拥有传入生物
	bool has_same_bio(ActiveBiology& abio) const;
	bool has_same_type_bio(ActiveBiology& abio) const;
	ActiveBiologyPtr has_prey(ActiveBiology& abio, Model& model) const;

private:

	uint terrain;		// 当前tile的气候类型
	bool selected;		// 是否被选中
	std::vector<ActiveBiologyPtr> active_bios;		// 存储生物实例指针

};


#endif // tile_H

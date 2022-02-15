#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <qobject.h>
#include <map>
#include <vector>

#include "inputBioInfo.h"
#include "tile.h"
#include "tools.h"

/*
存储数据模型类
*/
class Model
{
public:
	Model(BIO_Type&& b);		// 右值引用
	~Model();

	/* 根据生物下标查找、并返回生物信息 */
	biology& get_bio(unsigned int index);
	unsigned int get_bio_index(biology& b) const;
	/* 获取生物名称列表 */
	std::vector<std::string> get_bioname_list() const;

	/* 根据图片下标查找图片 */
	image& get_img(unsigned int index);
	image& get_cursor_img();
	image& get_dead_img();
	image& get_path_img();
	const image& get_bio_img(unsigned int index);
	bool has_bio_img(unsigned int index);
	/* 加入一个 image 对象*/
	void add_tools_image(image&& img);
	void add_bio_image(image&& img, unsigned int index);

	/* 获取 tile 和 tile 总数*/
	tile& get_tile(uint i);
	unsigned int get_tile_sum() const;
	/* 初始化 tile view */
	void set_tile(tileSetType&& b);

	// 加入活动生物实例
	void add_active_bio(ActiveBiologyPtr& abio, bool is_add_model = true);
	// 删除活动生物实例
	void del_active_bio(ActiveBiologyPtr& abio, bool is_del_model = true);
	void del_active_bio(ActiveBiology& bio, bool is_del_model = true);
	// 获取指定下标的生物实例
	ActiveBiologyPtr& get_active_bio(unsigned int index);
	ActiveBiologyPtr get_active_bio(ActiveBiology& abio);
	// 获取vector最后一个位置的生物实例
	ActiveBiologyPtr& get_last_active_bio();
	// 获取当前生物实例的数量
	unsigned int get_active_bios_size() const;

	// 获取唯一序号
	unsigned int get_unique_id();
	void recycle_uid(unsigned int i);

	// 判断是否是食肉动物
	bool is_carnivores(biology& bio);
	bool is_herbivores(biology& bio);

public:
	// 海水和陆地的花费表
	const std::vector<int> land_cost_table = { -1, -1, 1, 1, 4, 2 };
	const std::vector<int> water_cost_table = { 3, 1, -1, -1, -1, -1 };

private:

	// 存储所有生物信息
	BIO_Type bio;

	// 存储所有活动生物
	std::vector<ActiveBiologyPtr> active_bios;

	// 存储所有单元格的数据结构
	tileSetType tile_arr;

	// 存储生物图片
	std::map<unsigned int, image> bio_images;

	/*
	存储除了生物的其他图像：
		- 0 ~ 5表示地图图像
		- 6 表示 cursor
		- 7 表示 dead
		- 8 表示 path
	*/
	imageListType tools_images;	

	// 唯一序号生成器
	Tools::UniqueIDGen unique_id_gen;

	// 食肉动物
	std::unordered_set<int> carnivores{ 10, 13, 14, 17, 19, 21 };
	// 食草动物
	std::unordered_set<int> herbivores{ 11, 12, 15, 16, 18, 20 };

};

#endif	// MODEL_H
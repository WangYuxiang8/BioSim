#pragma once
#include <vector>
#include <memory>
#include <unordered_set>
#include "config.h"

class biology;
class image;
class tile;
class Model;
class Present;
class IFsmCallback;
class ActiveBiology;

typedef std::shared_ptr<ActiveBiology> ActiveBiologyPtr;
typedef std::vector<biology> BIO_Type;	//定义所有生物列表的类型，每个元素是一个biology类的对象。
typedef std::vector<tile> tileSetType;
typedef std::vector<image> imageListType;
typedef std::vector<std::pair<unsigned int, unsigned int>> PathOrdered;

namespace Tools {
	// 从地图的下标到地图类型（字符）的转换
	unsigned char from_terrain_index_to_terrain_type(unsigned int i);
	// 从图块下标到坐标的转换
	std::pair<unsigned int, unsigned int> from_tile_index_to_axis(unsigned int index);
	// 从图块坐标到下标的转换
	unsigned int from_tile_axis_to_index(std::pair<unsigned int, unsigned int>& axis);

	// 获取活动生物的唯一标识
	class UniqueIDGen
	{
	public:
		UniqueIDGen();
		~UniqueIDGen();
		
		unsigned int getID();
		void recycle_uid(unsigned int n);

	private:
		// 当前已使用的最大id
		unsigned int max;
		// 当前已回收id
		std::vector<unsigned int> reuse;
	};
}
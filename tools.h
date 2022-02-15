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
typedef std::vector<biology> BIO_Type;	//�������������б�����ͣ�ÿ��Ԫ����һ��biology��Ķ���
typedef std::vector<tile> tileSetType;
typedef std::vector<image> imageListType;
typedef std::vector<std::pair<unsigned int, unsigned int>> PathOrdered;

namespace Tools {
	// �ӵ�ͼ���±굽��ͼ���ͣ��ַ�����ת��
	unsigned char from_terrain_index_to_terrain_type(unsigned int i);
	// ��ͼ���±굽�����ת��
	std::pair<unsigned int, unsigned int> from_tile_index_to_axis(unsigned int index);
	// ��ͼ�����굽�±��ת��
	unsigned int from_tile_axis_to_index(std::pair<unsigned int, unsigned int>& axis);

	// ��ȡ������Ψһ��ʶ
	class UniqueIDGen
	{
	public:
		UniqueIDGen();
		~UniqueIDGen();
		
		unsigned int getID();
		void recycle_uid(unsigned int n);

	private:
		// ��ǰ��ʹ�õ����id
		unsigned int max;
		// ��ǰ�ѻ���id
		std::vector<unsigned int> reuse;
	};
}
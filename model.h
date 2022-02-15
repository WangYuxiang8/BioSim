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
�洢����ģ����
*/
class Model
{
public:
	Model(BIO_Type&& b);		// ��ֵ����
	~Model();

	/* ���������±���ҡ�������������Ϣ */
	biology& get_bio(unsigned int index);
	unsigned int get_bio_index(biology& b) const;
	/* ��ȡ���������б� */
	std::vector<std::string> get_bioname_list() const;

	/* ����ͼƬ�±����ͼƬ */
	image& get_img(unsigned int index);
	image& get_cursor_img();
	image& get_dead_img();
	image& get_path_img();
	const image& get_bio_img(unsigned int index);
	bool has_bio_img(unsigned int index);
	/* ����һ�� image ����*/
	void add_tools_image(image&& img);
	void add_bio_image(image&& img, unsigned int index);

	/* ��ȡ tile �� tile ����*/
	tile& get_tile(uint i);
	unsigned int get_tile_sum() const;
	/* ��ʼ�� tile view */
	void set_tile(tileSetType&& b);

	// ��������ʵ��
	void add_active_bio(ActiveBiologyPtr& abio, bool is_add_model = true);
	// ɾ�������ʵ��
	void del_active_bio(ActiveBiologyPtr& abio, bool is_del_model = true);
	void del_active_bio(ActiveBiology& bio, bool is_del_model = true);
	// ��ȡָ���±������ʵ��
	ActiveBiologyPtr& get_active_bio(unsigned int index);
	ActiveBiologyPtr get_active_bio(ActiveBiology& abio);
	// ��ȡvector���һ��λ�õ�����ʵ��
	ActiveBiologyPtr& get_last_active_bio();
	// ��ȡ��ǰ����ʵ��������
	unsigned int get_active_bios_size() const;

	// ��ȡΨһ���
	unsigned int get_unique_id();
	void recycle_uid(unsigned int i);

	// �ж��Ƿ���ʳ�⶯��
	bool is_carnivores(biology& bio);
	bool is_herbivores(biology& bio);

public:
	// ��ˮ��½�صĻ��ѱ�
	const std::vector<int> land_cost_table = { -1, -1, 1, 1, 4, 2 };
	const std::vector<int> water_cost_table = { 3, 1, -1, -1, -1, -1 };

private:

	// �洢����������Ϣ
	BIO_Type bio;

	// �洢���л����
	std::vector<ActiveBiologyPtr> active_bios;

	// �洢���е�Ԫ������ݽṹ
	tileSetType tile_arr;

	// �洢����ͼƬ
	std::map<unsigned int, image> bio_images;

	/*
	�洢�������������ͼ��
		- 0 ~ 5��ʾ��ͼͼ��
		- 6 ��ʾ cursor
		- 7 ��ʾ dead
		- 8 ��ʾ path
	*/
	imageListType tools_images;	

	// Ψһ���������
	Tools::UniqueIDGen unique_id_gen;

	// ʳ�⶯��
	std::unordered_set<int> carnivores{ 10, 13, 14, 17, 19, 21 };
	// ʳ�ݶ���
	std::unordered_set<int> herbivores{ 11, 12, 15, 16, 18, 20 };

};

#endif	// MODEL_H
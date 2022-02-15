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

	// ��ȡ���ڻ����
	ActiveBiology& get_active_bio(unsigned int index);
	ActiveBiologyPtr get_active_bio(ActiveBiology& abio);
	// ��ȡ���ڻ��������
	unsigned int get_active_bio_num() const;
	// ����һ���״̬������
	void add_active_bio(ActiveBiologyPtr& abio);
	// ɾ������Ļ״̬������
	void del_active_bio(ActiveBiologyPtr& abio);
	void del_active_bio(ActiveBiology& bio);

	bool is_select() const;
	void set_selected(bool b);

	// ��ǰͼ���Ƿ�ӵ�д�������
	bool has_same_bio(ActiveBiology& abio) const;
	bool has_same_type_bio(ActiveBiology& abio) const;
	ActiveBiologyPtr has_prey(ActiveBiology& abio, Model& model) const;

private:

	uint terrain;		// ��ǰtile����������
	bool selected;		// �Ƿ�ѡ��
	std::vector<ActiveBiologyPtr> active_bios;		// �洢����ʵ��ָ��

};


#endif // tile_H

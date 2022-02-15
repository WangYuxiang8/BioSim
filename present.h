#pragma once
#ifndef PRESENT_H
#define PRESENT_H

#include <qobject.h>
#include <noise/noise.h>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <queue>
#include <QTime>
#include <QCoreApplication>
#include "config.h"
#include "noiseutils.h"
#include "view.h"
#include "model.h"
#include "tile.h"
#include "image.h"
#include "tools.h"
#include "path.h"
#include "StateFsm.h"

/*
����ҵ���߼�����
*/
class Present
{
public:

	Present(Model& model);
	~Present();

	/* �� combox ��ѡ�����ݷ����仯ʱ��ִ��������� */
	void bioinfo_selection_changed_execute(uint index) const;

	/* ��ʾһ���źŴ��ڣ������¼�����ʱ����Ӧ */
	void show_message_box_execute(const std::string& message, const std::string& title) const;

	/* ��ʼ�� combox �����б� */
	void place_bioinfo_list_execute() const;

	/* ��ȡ��ǰtile��ͼ���б� */
	std::vector<image>& get_tile_image_list(std::vector<image>& image_list, uint x, uint y) const;

	/* �������ѡ��ĳ��ͼ��ʱ��Ӧ */
	void click_tile_event_execute(uint x, uint y);

	/* ��tile�±�ת����tile�ڴ�ͼ���е��������� */
	QPoint from_tile_index_to_axis() const;
	QPoint from_last_tile_index_to_axis() const;

	/* ������ð�ť����������ͼ�� */
	void set_bio_image_execute(uint bio_index);

	/* ����view���� */
	void set_view(View& view);

	/* �ҵ�һ�����·���������Ƿ�ɹ��ҵ� */
	bool find_shortest_path(PathOrdered& p, int sx, int sy, int ex, int ey);

	/* ��ʼģ�� */
	void simulate(uint interval, bool step = false);

	/* ����ģ��ֹͣ */
	void set_sim_stop(bool b);

private:

	View* m_view;
	Model& m_model;

	const std::vector<std::string> terrain_name = 
	{ "deep_sea", "shallow_water", "sand", "earth", "rocks", "snow" };
	int curr_select_tile_index = -1;
	int last_select_tile_index = -1;

	// Coordinate_List path; // �ҵ���·��
	
	bool sim_stop;	// ģ���Ƿ�ֹͣ
	QTime sim_time;			// ģ��ʱ����

	/* ��ʼ��tile set */
	tileSetType init_tile_set(unsigned int col, unsigned int row) const;
	/* ����ͼƬ����ɫ */
	void load_terrain_and_tools_image() const;

	/* ��ʼ�� perlin noise */
	std::vector<float>& get_perlin_noise(
		std::vector<float>& perlin_noise_arr,
		uint dest_width,
		uint dest_height,
		double lower_x,
		double upper_x,
		double lower_z,
		double upper_z) const;
	uint from_perlin_noise_to_terrain(float v) const;
	/* �ж�����ֵ�ĵ�ͼ���ͣ�'w'��ʾˮ��'l'��ʾ½�� */
	unsigned char from_terrain_index_to_terrain_type(uint i) const;
	/* ��ӡ����ǰ�� perlin noise ��ͼ�����ڲ��� */
	void print_terrain_map_image(const utils::NoiseMap& heightMap) const;

	/* ��������ǰ׺Ŀ¼���ļ���������·���� */
	std::string& gen_image_path(std::string& prefix, const std::string& name) const;
	/* ��������ǰ׺·������������·���� */
	std::string& gen_image_path(std::string& prefix) const;
	std::string gen_image_path(const std::string& prefix) const;

	/* ����ģ���߼� */
	void one_step_sim_logic();

};

#endif	// PRESENT_H
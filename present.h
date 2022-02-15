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
处理业务逻辑的类
*/
class Present
{
public:

	Present(Model& model);
	~Present();

	/* 当 combox 中选择内容发生变化时，执行这个函数 */
	void bioinfo_selection_changed_execute(uint index) const;

	/* 显示一个信号窗口，用于事件发生时的响应 */
	void show_message_box_execute(const std::string& message, const std::string& title) const;

	/* 初始化 combox 生物列表 */
	void place_bioinfo_list_execute() const;

	/* 获取当前tile的图像列表 */
	std::vector<image>& get_tile_image_list(std::vector<image>& image_list, uint x, uint y) const;

	/* 当鼠标点击选中某个图块时响应 */
	void click_tile_event_execute(uint x, uint y);

	/* 从tile下标转换到tile在大图景中的像素坐标 */
	QPoint from_tile_index_to_axis() const;
	QPoint from_last_tile_index_to_axis() const;

	/* 点击放置按钮，集成生物图像 */
	void set_bio_image_execute(uint bio_index);

	/* 设置view引用 */
	void set_view(View& view);

	/* 找到一条最短路径并返回是否成功找到 */
	bool find_shortest_path(PathOrdered& p, int sx, int sy, int ex, int ey);

	/* 开始模拟 */
	void simulate(uint interval, bool step = false);

	/* 设置模拟停止 */
	void set_sim_stop(bool b);

private:

	View* m_view;
	Model& m_model;

	const std::vector<std::string> terrain_name = 
	{ "deep_sea", "shallow_water", "sand", "earth", "rocks", "snow" };
	int curr_select_tile_index = -1;
	int last_select_tile_index = -1;

	// Coordinate_List path; // 找到的路径
	
	bool sim_stop;	// 模拟是否停止
	QTime sim_time;			// 模拟时间间隔

	/* 初始化tile set */
	tileSetType init_tile_set(unsigned int col, unsigned int row) const;
	/* 加载图片和颜色 */
	void load_terrain_and_tools_image() const;

	/* 初始化 perlin noise */
	std::vector<float>& get_perlin_noise(
		std::vector<float>& perlin_noise_arr,
		uint dest_width,
		uint dest_height,
		double lower_x,
		double upper_x,
		double lower_z,
		double upper_z) const;
	uint from_perlin_noise_to_terrain(float v) const;
	/* 判断输入值的地图类型，'w'表示水，'l'表示陆地 */
	unsigned char from_terrain_index_to_terrain_type(uint i) const;
	/* 打印出当前的 perlin noise 的图像，用于测试 */
	void print_terrain_map_image(const utils::NoiseMap& heightMap) const;

	/* 根据输入前缀目录和文件名称生成路径名 */
	std::string& gen_image_path(std::string& prefix, const std::string& name) const;
	/* 根据输入前缀路径名生成完整路径名 */
	std::string& gen_image_path(std::string& prefix) const;
	std::string gen_image_path(const std::string& prefix) const;

	/* 单步模拟逻辑 */
	void one_step_sim_logic();

};

#endif	// PRESENT_H
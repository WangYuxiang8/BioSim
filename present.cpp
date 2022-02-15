#include "present.h"

Present::Present(Model& model)
    :
    m_model(model),
    m_view(nullptr),
    sim_stop(true)
{
    // 加载地图图片，工具图片
    load_terrain_and_tools_image();
    // 初始化tile set
    m_model.set_tile(init_tile_set(COL, ROW));
}

Present::~Present()
{
}

void Present::bioinfo_selection_changed_execute(uint index) const
{
    // 从 model 中获取数据，如果没有找到，那么证明查找数据出错
    try {
        biology& b = m_model.get_bio(index);
        m_view->bioinfo_selection_changed(b.get_strength(), b.get_speed(), b.get_life(), b.get_attribute());
    }
    catch (const std::exception& e) {
        show_message_box_execute("Find bio info error!", "Error message");
    }
}

void Present::show_message_box_execute(const std::string& message, const std::string& title) const
{
	m_view->show_message_box(message, title);
}

void Present::place_bioinfo_list_execute() const
{
    std::vector<std::string> v = m_model.get_bioname_list();
    // 如果获取名称列表失败，则显示信息
    if (v.empty())
        show_message_box_execute("Init bioinfo list error!", "Error message");
    else
        for (const auto& it : v)
            m_view->add_item_to_bio_combox(it);
}

std::vector<image>& Present::get_tile_image_list(std::vector<image>& image_list, uint x, uint y) const
{
    uint tile_index = y * ROW + x;
    tile& t = m_model.get_tile(tile_index);
    uint terrain_index = t.get_terrain();
    image_list.emplace_back(m_model.get_img(terrain_index));
    
    unsigned int active_bio_num = t.get_active_bio_num();
    if (active_bio_num > 0) {
        /* 获取生物图片信息 */
        for (int i = 0; i < active_bio_num; i++) {
            ActiveBiology& abio = t.get_active_bio(i);
            // 加入生物死亡图片
            if (abio.get_dead() >= 0)
                image_list.emplace_back(m_model.get_dead_img());
            else 
                image_list.emplace_back(m_model.get_bio_img(abio.get_id()));
        }
    }

    /* 加入光标 */
    if (t.is_select())
        image_list.emplace_back(m_model.get_cursor_img());

    /* 加入路径 */
    //Location loc{ (int)x, (int)y };
    //if (path.find(loc) != path.end())
    //    image_list.emplace_back(m_model.get_path_img());

    return image_list;
}

void Present::click_tile_event_execute(uint tx, uint ty)
{
    // 获得当前选中的图块
    uint tile_index = ty * ROW + tx;
    tile& curr_tile = m_model.get_tile(tile_index);
    curr_tile.set_selected(true);

    // 将上一次的选中的图块的"cursor"图标去掉
    if (curr_select_tile_index != -1 && curr_select_tile_index != tile_index) {
        tile& curr_tile = m_model.get_tile(curr_select_tile_index);
        curr_tile.set_selected(false);
    }

    last_select_tile_index = curr_select_tile_index;
    // 更新当前图块的index
    curr_select_tile_index = tile_index;
}

QPoint Present::from_tile_index_to_axis() const
{
    if (curr_select_tile_index == -1)
        return QPoint(-1, -1);
    uint y = curr_select_tile_index / COL;
    uint x = curr_select_tile_index - y * COL;
    return QPoint(x, y);
}

QPoint Present::from_last_tile_index_to_axis() const
{
    if (last_select_tile_index == -1)
        return QPoint(-1, -1);
    uint y = last_select_tile_index / COL;
    uint x = last_select_tile_index - y * COL;
    return QPoint(x, y);
}

void Present::set_bio_image_execute(uint bio_index)
{
    // 获得当前选中的生物
    biology& bio = m_model.get_bio(bio_index);

    // 如果没有选中就点击放置，则显示错误
    if (curr_select_tile_index == -1)
    {
        show_message_box_execute("Please select one block!", "Error message");
        return;
    }

    // 获得当前选中的图块
    tile& t = m_model.get_tile(curr_select_tile_index);

    // 判断当前生物和地图类型是否匹配
    if (from_terrain_index_to_terrain_type(t.get_terrain()) != bio.get_type())
    {
        show_message_box_execute("Terrain type and creature type do not match!", "Error message");
        return;
    }

    // 如果生物图片路径错误，那么捕获异常
    try {
        // 如果当前生物的图片还没有加载就加载进来
        if (!m_model.has_bio_img(bio.get_id())) {
            const std::string& img_path = bio.get_image_path();
            m_model.add_bio_image(image::load_tga(gen_image_path(img_path)), bio.get_id());
        }
        // 将当前生物实例加入
        std::pair<unsigned int, unsigned int> axis = Tools::from_tile_index_to_axis(curr_select_tile_index);
        ActiveBiologyPtr abio(new ActiveBiology(bio, m_model.get_unique_id(), axis.first, axis.second));
        m_model.add_active_bio(abio);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        show_message_box_execute(e.what(), "Error message");
    }
}

void Present::set_view(View& view)
{
    m_view = &view;
}

bool Present::find_shortest_path(PathOrdered& p, int sx, int sy, int gx, int gy)
{
    Location source = { sx, sy };
    Location target = { gx, gy };

    // 检查源和目的坐标是否有效
    if (source.x < 0 || source.y < 0 || target.x < 0 || target.y < 0) {
        show_message_box_execute("Invalid source or target coordinates!", "Error");
        return false;
    }

    // 判断源生物是否可以行走（速度大于0）
    tile& t = m_model.get_tile(source.y * COL + source.x);
    unsigned int active_bio_num = t.get_active_bio_num();
    unsigned int i = 0;
    if (active_bio_num > 0) {
        for (i = 0; i < active_bio_num; i++) {
            biology& bio = t.get_active_bio(i);
            if (bio.get_speed() > 0) break;
        }
    }
    if (i == active_bio_num)
    {
        show_message_box_execute("No moved biology in current tile!", "Error message");
        return false;
    }

    /* 启动 A star 搜索算法 */
    AStar astar(COL, ROW, m_model);
    p.clear();
    p = astar.find_path(p, source, target);

    // 没有找到路径 
    if (p.size() == 0) {
        // std::cout << "No path or in current location!" << std::endl;
        return false;
    }
    return true;
}

void Present::simulate(uint interval, bool step)
{
    // 单步调试
    if (step) {
        one_step_sim_logic();
        sim_stop = true;
    }
    // 正常模拟
    else {
        sim_stop = false;
        while (!sim_stop) {
            sim_time.start();
            one_step_sim_logic();
            // elapsed() 返回自上次调用start()或restart()以来经过的毫秒数。
            while (sim_time.elapsed() < interval)
                QCoreApplication::processEvents();   //处理事件
        }
    }
}

// 一步模拟，生物自动机在单步需要执行的操作
// 遍历active_bio生物列表，执行所有生物关联的自动机
void Present::one_step_sim_logic()
{
    for (int i = 0; i < m_model.get_active_bios_size(); i++)
    {
        try {
            ActiveBiologyPtr& abio = m_model.get_active_bio(i);
            abio->start_fsm(m_model, *this);
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    m_view->update();
}

void Present::set_sim_stop(bool b)
{
    sim_stop = b;
}


std::vector<float>& Present::get_perlin_noise(
    std::vector<float>& perlin_noise_arr,
    uint dest_width,
    uint dest_height,
    double lower_x,
    double upper_x,
    double lower_z,
    double upper_z) const
{
    module::Perlin myModule;
    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(myModule);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(dest_width, dest_height);
    heightMapBuilder.SetBounds(lower_x, upper_x, lower_z, upper_z);
    heightMapBuilder.Build();
    const float* map_ptr = heightMap.GetConstSlabPtr();

    for (int i = 0; i < COL * ROW; i++)
        perlin_noise_arr.emplace_back(*map_ptr++);

    // print_terrain_map_image(heightMap);
    return perlin_noise_arr;
}

tileSetType Present::init_tile_set(unsigned int col, unsigned int row) const
{
    // 获取perlin noise
    // 因为在每个tile实例中将气候类型存储为一个索引
    // 所以这里的perlin noise值不需要存储，局部生命周期用完就销毁了
    std::vector<float> perlin_noise_arr;
    perlin_noise_arr.reserve(col * row);
    perlin_noise_arr = get_perlin_noise(perlin_noise_arr, col, row,
        BOUND[0], BOUND[1], BOUND[2], BOUND[3]);

    // 初始化全局景观的tile
    tileSetType tile_set;
    tile_set.reserve(col * row);
    for (int j = 0; j < row; j++)
    {
        for (int k = 0; k < col; k++)
        {
            tile t(from_perlin_noise_to_terrain(perlin_noise_arr.at((row - j - 1) * row + k)));
            tile_set.emplace_back(t);
        }
    }
    return tile_set;
}

void Present::load_terrain_and_tools_image() const
{
    for (auto it : terrain_name)
    {
        std::string path = "terrain";
        m_model.add_tools_image(image::load_tga(gen_image_path(path, it)));
    }
    m_model.add_tools_image(image::load_tga(gen_image_path("cursor/cursor.tga")));
    m_model.add_tools_image(image::load_tga(gen_image_path("dead/dead.tga")));
    m_model.add_tools_image(image::load_tga(gen_image_path("path/path.tga")));
}

uint Present::from_perlin_noise_to_terrain(float v) const
{
    if (v < -0.25)
        return 0;    // deep_water
    else if (v < 0)
        return 1;    // shallow_water
    else if (v < 0.125)
        return 2;    // sand
    else if (v < 0.375)
        return 3;    // earth
    else if (v < 0.75)
        return 4;    // rocks
    else
        return 5;    // snow
}

unsigned char Present::from_terrain_index_to_terrain_type(uint i) const
{
    if (i < 2) return 'w';
    else return 'l';
}

void Present::print_terrain_map_image(const utils::NoiseMap& heightMap) const
{
    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.ClearGradient();
    renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
    renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
    renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
    renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
    renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
    renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
    renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
    renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
    renderer.Render();

    utils::WriterBMP writer;
    writer.SetSourceImage(image);
    writer.SetDestFilename("test.bmp");
    writer.WriteDestFile();
}

std::string& Present::gen_image_path(std::string& prefix, const std::string& name) const
{
    prefix = "./all_graphics/" + prefix + "/" + name + ".tga";
    return prefix;
}

std::string& Present::gen_image_path(std::string& prefix) const
{
    prefix = "./all_graphics/" + prefix;
    return prefix;
}

std::string Present::gen_image_path(const std::string& prefix) const
{
    return "./all_graphics/" + prefix;
}

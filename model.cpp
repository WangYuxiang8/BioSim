#include "model.h"

Model::Model(BIO_Type&& b)
    :
    bio(std::move(b))       // “∆∂Ø”Ô“Â
{
}

Model::~Model()
{
}

biology& Model::get_bio(unsigned int index)
{
    if (index < 0 || index >= bio.size()) {
        throw std::exception("Model Error: wrong index i to get bio!");
    }
    return bio.at(index);
}

image& Model::get_img(unsigned int index)
{
    if (index < 0 || index >= tools_images.size()) {
        throw std::exception("Model Error: wrong index i to get image!");
    }
    return tools_images.at(index);
}

image& Model::get_cursor_img()
{
    return tools_images.at(6);
}

image& Model::get_dead_img()
{
    return tools_images.at(7);
}

image& Model::get_path_img()
{
    return tools_images.at(8);
}

const image& Model::get_bio_img(unsigned int index)
{
    std::map<unsigned int, image>::iterator it = bio_images.find(index);
    if (it == bio_images.end()) {
        throw std::exception("Model Error: wrong key i to get image!");
    }
    return it->second;
}

bool Model::has_bio_img(unsigned int index)
{
    std::map<unsigned int, image>::iterator it = bio_images.find(index);
    if (it == bio_images.end())
        return false;
    return true;
}

void Model::add_bio_image(image&& img, unsigned int index)
{
    bio_images.emplace(index, std::move(img));
}

unsigned int Model::get_bio_index(biology& b) const
{
    for (int i = 0; i < bio.size(); i++) {
        if (bio.at(i).get_name() == b.get_name())
            return i;
    }
    return -1;
}

std::vector<std::string> Model::get_bioname_list() const
{
    std::vector<std::string> v;
    for (const auto &it : bio)
    {
        v.push_back(it.get_name());
    }
    return v;
}

void Model::set_tile(tileSetType&& b)
{
    tile_arr = std::move(b);
}

void Model::add_active_bio(ActiveBiologyPtr& abio, bool is_add_model)
{
    if (is_add_model)
        active_bios.emplace_back(abio);
    tile& t = get_tile(Tools::from_tile_axis_to_index(std::pair<unsigned int, unsigned int>(abio->getX(), abio->getY())));
    t.add_active_bio(abio);
}

void Model::del_active_bio(ActiveBiologyPtr& abio, bool is_del_model)
{
    if (is_del_model) {
        std::vector<ActiveBiologyPtr>::iterator it = active_bios.begin();
        for (; it != active_bios.end(); it++) {
            if (abio == *it) {
                active_bios.erase(it);
                break;
            }
        }
    }
    tile& t = get_tile(Tools::from_tile_axis_to_index(std::pair<unsigned int, unsigned int>(abio->getX(), abio->getY())));
    t.del_active_bio(abio);
}

void Model::del_active_bio(ActiveBiology& abio, bool is_del_model)
{
    if (is_del_model) {
        std::vector<ActiveBiologyPtr>::iterator it = active_bios.begin();
        for (; it != active_bios.end(); it++) {
            if (abio == *it->get()) {
                active_bios.erase(it);
                break;
            }
        }
    }
    tile& t = get_tile(Tools::from_tile_axis_to_index(std::pair<unsigned int, unsigned int>(abio.getX(), abio.getY())));
    t.del_active_bio(abio);
}

ActiveBiologyPtr& Model::get_active_bio(unsigned int index)
{
    if (index < 0 || index >= active_bios.size()) {
        std::string msg = "Model Error: wrong index " + std::to_string(index) + " to get active bio!";
        throw std::exception(msg.c_str());
    }
    return active_bios.at(index);
}

ActiveBiologyPtr Model::get_active_bio(ActiveBiology& abio)
{
    for (auto& it : active_bios) {
        if (it->get_uid() == abio.get_uid()) return it;
    }
    return nullptr;
}

ActiveBiologyPtr& Model::get_last_active_bio()
{
    return active_bios.back();
}

unsigned int Model::get_active_bios_size() const
{
    return active_bios.size();
}

unsigned int Model::get_unique_id()
{
    return unique_id_gen.getID();
}

void Model::recycle_uid(unsigned int i)
{
    unique_id_gen.recycle_uid(i);
}

bool Model::is_carnivores(biology& bio)
{
    if (carnivores.find((int)bio.get_id()) != carnivores.end())
        return true;
    return false;
}

bool Model::is_herbivores(biology& bio)
{
    if (herbivores.find((int)bio.get_id()) != herbivores.end())
        return true;
    return false;
}


tile& Model::get_tile(uint i)
{
    if (i < 0 || i >= tile_arr.size()) {
        std::string msg = "Model Error: wrong index " + std::to_string(i) + " to get tile!";
        throw std::exception(msg.c_str());
    }
    return tile_arr.at(i);
}

unsigned int Model::get_tile_sum() const
{
    return tile_arr.size();
}

void Model::add_tools_image(image&& img)
{
    tools_images.emplace_back(std::move(img));
}

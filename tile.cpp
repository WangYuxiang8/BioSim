#include "tile.h"

tile::tile(uint terrain)
	: 
	terrain(terrain),
	selected(false)
{
}

tile::~tile()
{
}

uint tile::get_terrain() const
{
	return terrain;
}

ActiveBiology& tile::get_active_bio(unsigned int index)
{
	if (index < 0 || index >= active_bios.size()) {
		throw std::exception("Model Error: wrong index i to get active biology!");
	}
	return *active_bios.at(index);
}

ActiveBiologyPtr tile::get_active_bio(ActiveBiology& abio)
{
	for (auto& it : active_bios) {
		if (it->get_uid() == abio.get_uid()) return it;
	}
	return nullptr;
}

unsigned int tile::get_active_bio_num() const
{
	return active_bios.size();
}

void tile::add_active_bio(ActiveBiologyPtr& abio)
{
	active_bios.emplace_back(abio);
}

void tile::del_active_bio(ActiveBiologyPtr& abio)
{
	std::vector<ActiveBiologyPtr>::iterator it = active_bios.begin();
	for (; it != active_bios.end();it++) {
		if (abio == *it) {
			active_bios.erase(it);
			break;
		}
	}
}

void tile::del_active_bio(ActiveBiology& bio)
{
	std::vector<ActiveBiologyPtr>::iterator it = active_bios.begin();
	for (; it != active_bios.end(); it++) {
		if (bio == *it->get()) {
			active_bios.erase(it);
			break;
		}
	}
}


bool tile::is_select() const
{
	return selected;
}

void tile::set_selected(bool b)
{
	selected = b;
}

bool tile::has_same_bio(ActiveBiology& abio) const
{
	// 这里主要指相同类型的生物，用名字区分
	for (auto& it : active_bios) {
		if (it->get_name() == abio.get_name()) return true;
	}
	return false;
}

bool tile::has_same_type_bio(ActiveBiology& abio) const
{
	for (auto& it : active_bios) {
		// 同是植物
		if (abio.get_speed() == 0 && it->get_speed() == 0 && it->get_type() == abio.get_type())
			return true;
		// 同是动物
		if (abio.get_speed() != 0 && it->get_speed() != 0 && it->get_type() == abio.get_type())
			return true;
	}
	return false;
}

ActiveBiologyPtr tile::has_prey(ActiveBiology& abio, Model& model) const
{
	// 食肉
	if (model.is_carnivores(abio)) {
		for (auto& it : active_bios) {
			if (model.is_herbivores(*it)) return it;
		}
	}
	// 食草
	else {
		for (auto& it : active_bios) {
			if (it->get_speed() == 0) return it;
		}
	}
	return nullptr;
}

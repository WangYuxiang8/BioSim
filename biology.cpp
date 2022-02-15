#include "biology.h"

biology::biology(unsigned int id,
	const std::string& name,
	unsigned int strength,
	unsigned int speed,
	unsigned int life,
	const std::string& attr,
	const std::string& image) 
	: 
	id(id),
	name(name), 
	strength(strength),
	speed(speed),
	life(life),
	attr_str(attr),
	image_path(image),
	type(image_path[0])
{
}

biology::~biology()
{
}

unsigned int biology::get_id() const
{
	return id;
}

const std::string& biology::get_name() const
{
	return name;
}

unsigned int biology::get_strength() const
{
	return strength;
}

unsigned int biology::get_speed() const
{
	return speed;
}

unsigned int biology::get_life() const
{
	return life;
}

const std::string& biology::get_attribute() const
{
	return attr_str;
}

const std::string& biology::get_image_path() const
{
	return image_path;
}

unsigned char biology::get_type() const
{
	return type;
}


ActiveBiology::ActiveBiology(const biology& other, unsigned int uid, unsigned int x, unsigned int y)
	:
	biology(other.get_id(), other.get_name(), other.get_strength(), 
		other.get_speed(), other.get_life(), other.get_attribute(), other.get_image_path()),
	LD(other.get_life()),
	unique_id(uid),
	x(x + 0.5),
	y(y + 0.5),
	dead(-1)
{
	if (other.get_speed() == 0) {
		fsm = std::unique_ptr<IFsmCallback>(new PlantFSM());
	}
	else {
		fsm = std::unique_ptr<IFsmCallback>(new AnimalFSM());
	}
}

ActiveBiology::~ActiveBiology()
{
}

bool ActiveBiology::operator==(const ActiveBiology& other)
{
	if (this->unique_id == other.unique_id) return true;
	return false;
}

unsigned int ActiveBiology::get_uid() const
{
	return unique_id;
}

unsigned int ActiveBiology::getX() const
{
	return floor(x);
}

unsigned int ActiveBiology::getY() const
{
	return floor(y);
}

float ActiveBiology::getXF() const
{
	return x;
}

float ActiveBiology::getYF() const
{
	return y;
}

void ActiveBiology::set_loc(unsigned int a, unsigned int b)
{
	x = a + 0.5;
	y = b + 0.5;
}

void ActiveBiology::set_loc(float a, float b)
{
	x = a;
	y = b;
}

unsigned int ActiveBiology::get_LD() const
{
	return LD;
}

void ActiveBiology::set_LD(int ld)
{
	ld = ld >= 0 ? ld : 0;
	ld = ld <= get_life() ? ld : get_life();
	LD = ld;
}

short ActiveBiology::get_dead() const
{
	return dead;
}

void ActiveBiology::set_dead(short b)
{
	dead = b;
}

void ActiveBiology::start_fsm(Model& model, Present& present)
{
	fsm->StartFSM(*this, model, present);
}

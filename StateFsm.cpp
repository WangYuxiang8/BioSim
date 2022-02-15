#include "StateFsm.h"

using std::pair;

StateFSM::StateFSM()
{
}

StateFSM::~StateFSM()
{
	for (auto it = mapState.begin(); it != mapState.end(); ) {
		it = mapState.erase(it);
	}
}

int StateFSM::StartFsm(int stateID, IFsmCallback& fsmobj)
{
	std::map<int, fsmState>::iterator stateIter;
	stateIter = mapState.find(stateID);
	if (stateIter != mapState.end())
	{
		fsmState& state = stateIter->second;
		if (state.vecTransition.size() == 0) {
			return -2; // only one state
		}
	}
	else {
		return -1; // no such state
	}

	fsmobj.FsmSetCurState(stateID);
	return 0;
}

int StateFSM::AddFsmConfig(int curState, int eventID, int nextState, int actionID)
{
	fsmTransition transition;
	transition.eventID = eventID;
	transition.nextState = nextState;
	transition.actionID = actionID;

	std::map<int, fsmState>::iterator stateIter;
	stateIter = mapState.find(curState);
	if (stateIter != mapState.end()) {
		stateIter->second.vecTransition.push_back(transition);
	}
	else {
		fsmState state;
		state.vecTransition.push_back(transition);
		mapState.insert(pair <int, fsmState>(curState, state));
	}
	return 0;
}

int StateFSM::doFsmEvent(int eventID, IFsmCallback& fsmobj, ActiveBiology& abio, Model& model, Present& present)
{
	int curState = fsmobj.FsmGetCurState();

	int actionID;
	int nextState;
	int iRet = getActionID(curState, eventID, nextState, actionID);
	if (iRet == 0) {
		fsmobj.FsmSetCurState(nextState);
		fsmobj.FsmCallback(actionID, abio, model, present);
	}
	return 0;
}

int StateFSM::getActionID(int curState, int eventID, int& nextState, int& actionID)
{
	std::map<int, fsmState>::iterator stateIter;
	stateIter = mapState.find(curState);
	if (stateIter != mapState.end()) {
		fsmState& state = stateIter->second;
		std::vector<fsmTransition>::iterator traniter;
		for (traniter = state.vecTransition.begin(); traniter != state.vecTransition.end(); traniter++)
		{
			fsmTransition& transition = *traniter;
			if (transition.eventID == eventID) {
				nextState = transition.nextState;
				actionID = transition.actionID;
				return 0;
			}
		}
		return -2; //curstate has no such event
	}
	else {
		return -1; //no such state
	}
}

PlantFSM::PlantFSM()
{
	fsm.AddFsmConfig(PlantState::s_WAIT, PlantState::e_EVENT_1, PlantState::s_GROW, PlantState::a_ACTION_1);
	fsm.AddFsmConfig(PlantState::s_WAIT, PlantState::e_EVENT_2, PlantState::s_WAIT, PlantState::a_ACTION_2);
	fsm.AddFsmConfig(PlantState::s_GROW, PlantState::e_EVENT_3, PlantState::s_WAIT, PlantState::a_ACTION_3);
	fsm.AddFsmConfig(PlantState::s_WAIT, PlantState::e_EVENT_4, PlantState::s_DEAD, PlantState::a_ACTION_4);
	fsm.AddFsmConfig(PlantState::s_GROW, PlantState::e_EVENT_4, PlantState::s_DEAD, PlantState::a_ACTION_4);
	fsm.AddFsmConfig(PlantState::s_DEAD, PlantState::e_EVENT_4, PlantState::s_DEAD, PlantState::a_ACTION_4);
	FsmSetCurState(PlantState::s_WAIT);
	FsmSetCurEvent(PlantState::e_EVENT_2);
	fsm.StartFsm(FsmGetCurState(), *this);

	LD_decay = 0;
	wait_step = 0;
}

PlantFSM::~PlantFSM()
{
}

int PlantFSM::FsmCallback(int actionID, ActiveBiology& abio, Model& model, Present& present)
{
	switch (actionID) {
		case PlantState::a_ACTION_1:
			execute_action_1(abio, model, present);
			break;
		case PlantState::a_ACTION_2:
			execute_action_2(abio, model, present);
			break;
		case PlantState::a_ACTION_3: 
			execute_action_3(abio, model, present);
			break;
		case PlantState::a_ACTION_4:
			execute_action_dead(abio, model, present);
			break;
		default:
			printf("invalid action id\n");
			break;
	}
	return 0;
}

void PlantFSM::StartFSM(ActiveBiology& abio, Model& model, Present& present)
{
	// 计算生命衰减值
	if (LD_decay == 0) {
		tile& t = model.get_tile(abio.getY() * ROW + abio.getY());
		unsigned int tile_type = t.get_terrain();
		if (tile_type == 1 || tile_type == 2 || tile_type == 3) 
			LD_decay = 10;
		else 
			LD_decay = 25;
	}
	fsm.doFsmEvent(FsmGetCurEvent(), *this, abio, model, present);
}

void PlantFSM::execute_action_1(ActiveBiology& abio, Model& model, Present& present)
{
	FsmSetCurEvent(PlantState::e_EVENT_3);
}

void PlantFSM::execute_action_2(ActiveBiology& abio, Model& model, Present& present)
{
	// 死亡
	if (get_LD(abio) <= 0) {
		FsmSetCurEvent(PlantState::e_EVENT_4);
		return;
	}
	set_LD(get_LD(abio) - LD_decay, abio);
	wait_step++;
	// std::cout << "Wait: " << wait_step << " Life: " << get_LD(abio) << std::endl;
	unsigned int neighbor_num = same_bio_number_within_k(abio, model, 5);
	if (wait_step > (get_LD(abio) / 100) && neighbor_num > 2 && neighbor_num < 10) {
		FsmSetCurEvent(PlantState::e_EVENT_1);
	}
	else {
		FsmSetCurEvent(PlantState::e_EVENT_2);
	}
}

void PlantFSM::execute_action_3(ActiveBiology& abio, Model& model, Present& present)
{
	wait_step = 0;
	//printf("Create plant\n");
	std::pair<unsigned int, unsigned int> axis = random_axis_within_k(abio, model, 5);
	if (axis.first != -1 && axis.second != -1) {
		ActiveBiologyPtr new_bio(new ActiveBiology(abio, model.get_unique_id(), axis.first, axis.second));
		model.add_active_bio(new_bio);
	}
	FsmSetCurEvent(PlantState::e_EVENT_2);
}

unsigned int IFsmCallback::same_bio_number_within_k(ActiveBiology& abio, Model& model, unsigned int k)
{
	unsigned int number = 0;
	unsigned int X = abio.getX();
	unsigned int Y = abio.getY();
	int startX, startY, endX, endY;
	cal_within_k_range(X, Y, startX, endX, startY, endY, k);
	for (int j = startY; j < endY; j++) {
		for (int i = startX; i < endX; i++) {
			tile& t = model.get_tile(j * COL + i);
			if ((std::abs((int)X - (int)i) + std::abs((int)Y - (int)j)) <= k && 
				Tools::from_terrain_index_to_terrain_type(t.get_terrain()) == abio.get_type() &&
				t.has_same_bio(abio))
				number += 1;
		}
	}
	return number;
}

ActiveBiologyPtr IFsmCallback::has_prey_within_k(ActiveBiology& abio, Model& model, unsigned int k, std::pair<unsigned int, unsigned int>& loc)
{
	unsigned int X = abio.getX();
	unsigned int Y = abio.getY();
	int startX, startY, endX, endY;
	cal_within_k_range(X, Y, startX, endX, startY, endY, k);
	for (int j = startY; j < endY; j++) {
		for (int i = startX; i < endX; i++) {
			tile& t = model.get_tile(j * COL + i);
			ActiveBiologyPtr prey;
			// 曼哈顿距离
			if ((std::abs((int)X - (int)i) + std::abs((int)Y - (int)j)) <= k &&
				Tools::from_terrain_index_to_terrain_type(t.get_terrain()) == abio.get_type() &&
				(prey = t.has_prey(abio, model)) != nullptr) {
				loc.first = i;
				loc.second = j;
				return prey;
			}	
		}
	}
	return nullptr;
}

std::pair<unsigned int, unsigned int> IFsmCallback::random_axis_within_k(ActiveBiology& abio, Model& model, unsigned int k)
{
	unsigned int X = abio.getX();
	unsigned int Y = abio.getY();
	int startX = 0, startY = 0, endX, endY;
	cal_within_k_range(X, Y, startX, endX, startY, endY, k);
	unsigned int randX, randY;
	unsigned int rand_idx = 4 * k * k;
	while (rand_idx > 0) {
		randX = random_XY(startX, endX);
		randY = random_XY(startY, endY);
		tile& t = model.get_tile(randY * COL + randX);
		// 曼哈顿距离
		if ((std::abs((int)X - (int)randX) + std::abs((int)Y - (int)randY)) <= k &&
			Tools::from_terrain_index_to_terrain_type(t.get_terrain()) == abio.get_type() &&
			!t.has_same_type_bio(abio))
			break;
		rand_idx--;
	}
	if (rand_idx == 0)
		return std::pair<unsigned int, unsigned int>(-1, -1);
	return std::pair<unsigned int, unsigned int>(randX, randY);
}

void IFsmCallback::cal_within_k_range(int x, int y,
	int& sx, int& ex, int& sy, int& ey, int k)
{
	sx = (x - k) >= 0 ? (x - k) : 0;
	sy = (y - k) >= 0 ? (y - k) : 0;
	ex = (x + k) < COL ? (x + k) : (COL - 1);
	ey = (y + k) < ROW ? (y + k) : (ROW - 1);
}

AnimalFSM::AnimalFSM()
{
	fsm.AddFsmConfig(AnimalState::s_WAIT, AnimalState::e_EVENT_1, AnimalState::s_WAIT, AnimalState::a_ACTION_1);
	fsm.AddFsmConfig(AnimalState::s_WAIT, AnimalState::e_EVENT_2, AnimalState::s_MOVE, AnimalState::a_ACTION_2);
	fsm.AddFsmConfig(AnimalState::s_MOVE, AnimalState::e_EVENT_3, AnimalState::s_MOVE, AnimalState::a_ACTION_3);
	fsm.AddFsmConfig(AnimalState::s_MOVE, AnimalState::e_EVENT_4, AnimalState::s_WAIT, AnimalState::a_ACTION_4);
	fsm.AddFsmConfig(AnimalState::s_MOVE, AnimalState::e_EVENT_5, AnimalState::s_MOVE, AnimalState::a_ACTION_5);
	fsm.AddFsmConfig(AnimalState::s_MOVE, AnimalState::e_EVENT_6, AnimalState::s_CATCH, AnimalState::a_ACTION_6);
	fsm.AddFsmConfig(AnimalState::s_CATCH, AnimalState::e_EVENT_7, AnimalState::s_CATCH, AnimalState::a_ACTION_7);
	fsm.AddFsmConfig(AnimalState::s_CATCH, AnimalState::e_EVENT_8, AnimalState::s_WAIT, AnimalState::a_ACTION_8);
	fsm.AddFsmConfig(AnimalState::s_CATCH, AnimalState::e_EVENT_9, AnimalState::s_ATTACK, AnimalState::a_ACTION_9);
	fsm.AddFsmConfig(AnimalState::s_ATTACK, AnimalState::e_EVENT_10, AnimalState::s_ATTACK, AnimalState::a_ACTION_10);
	fsm.AddFsmConfig(AnimalState::s_ATTACK, AnimalState::e_EVENT_11, AnimalState::s_WAIT, AnimalState::a_ACTION_11);
	fsm.AddFsmConfig(AnimalState::s_ATTACK, AnimalState::e_EVENT_12, AnimalState::s_CATCH, AnimalState::a_ACTION_12);
	fsm.AddFsmConfig(AnimalState::s_WAIT, AnimalState::e_EVENT_DEAD, AnimalState::s_DEAD, AnimalState::a_ACTION_DEAD);
	fsm.AddFsmConfig(AnimalState::s_MOVE, AnimalState::e_EVENT_DEAD, AnimalState::s_DEAD, AnimalState::a_ACTION_DEAD);
	fsm.AddFsmConfig(AnimalState::s_CATCH, AnimalState::e_EVENT_DEAD, AnimalState::s_DEAD, AnimalState::a_ACTION_DEAD);
	fsm.AddFsmConfig(AnimalState::s_ATTACK, AnimalState::e_EVENT_DEAD, AnimalState::s_DEAD, AnimalState::a_ACTION_DEAD);
	fsm.AddFsmConfig(AnimalState::s_DEAD, AnimalState::e_EVENT_DEAD, AnimalState::s_DEAD, AnimalState::a_ACTION_DEAD);
	FsmSetCurState(PlantState::s_WAIT);
	FsmSetCurEvent(PlantState::e_EVENT_1);
	fsm.StartFsm(FsmGetCurState(), *this);

	LD_decay = 0;
	wait_step = 0;
	match_step = 0;
}

AnimalFSM::~AnimalFSM()
{
}

int AnimalFSM::FsmCallback(int actionID, ActiveBiology& abio, Model& model, Present& present)
{
	switch (actionID) {
	case AnimalState::a_ACTION_1:
		execute_action_1(abio, model, present);
		break;
	case AnimalState::a_ACTION_2:
		execute_action_2(abio, model, present);
		break;
	case AnimalState::a_ACTION_3:
		execute_action_3(abio, model, present);
		break;
	case AnimalState::a_ACTION_4:
		execute_action_4(abio, model, present);
		break;
	case AnimalState::a_ACTION_5:
		execute_action_5(abio, model, present);
		break;
	case AnimalState::a_ACTION_6:
		execute_action_6(abio, model, present);
		break;
	case AnimalState::a_ACTION_7:
		execute_action_7(abio, model, present);
		break;
	case AnimalState::a_ACTION_8:
		execute_action_8(abio, model, present);
		break;
	case AnimalState::a_ACTION_9:
		execute_action_9(abio, model, present);
		break;
	case AnimalState::a_ACTION_10:
		execute_action_10(abio, model, present);
		break;
	case AnimalState::a_ACTION_11:
		execute_action_11(abio, model, present);
		break;
	case AnimalState::a_ACTION_12:
		execute_action_12(abio, model, present);
		break;
	case AnimalState::a_ACTION_DEAD:
		execute_action_dead(abio, model, present);
		break;
	default:
		printf("invalid action id\n");
		break;
	}
	return 0;
}

void AnimalFSM::StartFSM(ActiveBiology& abio, Model& model, Present& present)
{
	// 计算生命衰减值
	if (LD_decay == 0) {
		tile& t = model.get_tile(abio.getY() * ROW + abio.getY());
		unsigned int tile_type = t.get_terrain();
		if (tile_type == 1 || tile_type == 2 || tile_type == 3)
			LD_decay = 10;
		else
			LD_decay = 25;
	}
	fsm.doFsmEvent(FsmGetCurEvent(), *this, abio, model, present);
}

void AnimalFSM::execute_action_1(ActiveBiology& abio, Model& model, Present& present)
{
	// 死亡
	if (get_LD(abio) <= 0) {
		FsmSetCurEvent(AnimalState::e_EVENT_DEAD);
		return;
	}
	std::cout << "Wait: " << wait_step << " Life: " << get_LD(abio) << std::endl;
	unsigned int ld = get_LD(abio);
	if (wait_step <= (5 * ld / abio.get_life())) {
		set_LD(ld - 5, abio);
		wait_step++;
		FsmSetCurEvent(AnimalState::e_EVENT_1);
	}
	else {
		wait_step = 0;
		FsmSetCurEvent(AnimalState::e_EVENT_2);
	}
	match_step++;
}

void AnimalFSM::execute_action_2(ActiveBiology& abio, Model& model, Present& present)
{
	path.clear();
	target_loc = random_axis_within_k(abio, model, 10);
	if (target_loc.first != -1 && target_loc.second != -1) {
		present.find_shortest_path(path, abio.getX(), abio.getY(), target_loc.first, target_loc.second);
	}//std::cout << "Find path..." << std::endl;
	FsmSetCurEvent(AnimalState::e_EVENT_3);
}

void AnimalFSM::execute_action_3(ActiveBiology& abio, Model& model, Present& present)
{
	// 死亡
	if (get_LD(abio) <= 0) {
		FsmSetCurEvent(AnimalState::e_EVENT_DEAD);
		return;
	}
	// 配对
	unsigned int same_bio_number = same_bio_number_within_k(abio, model, 3);
	if (get_LD(abio) > 0.5 * abio.get_life() && match_step > (get_LD(abio) / 50) &&
		same_bio_number > 1 && same_bio_number < 5) {
		FsmSetCurEvent(AnimalState::e_EVENT_5);
		return;
	}
	// 发现猎物
	std::pair<unsigned int, unsigned int> loc;
	if (get_LD(abio) < (0.6 * abio.get_life()) && has_prey_within_k(abio, model, 10, loc)) {
		FsmSetCurEvent(AnimalState::e_EVENT_6);
		return;
	}
	// 随机游走
	if (!path.empty()) {
		// 先把abio从对应的图块中删除
		model.del_active_bio(abio, false);
		walk_to_somewhere(abio, model, path, 2, 0.5);
		ActiveBiologyPtr abio_ptr = model.get_active_bio(abio);
		model.add_active_bio(abio_ptr, false);
		FsmSetCurEvent(AnimalState::e_EVENT_3);
	}
	// 到达目的地
	else {
		FsmSetCurEvent(AnimalState::e_EVENT_4);
	}
	match_step++;
}

void AnimalFSM::execute_action_4(ActiveBiology& abio, Model& model, Present& present)
{
	FsmSetCurEvent(AnimalState::e_EVENT_1);
}

void AnimalFSM::execute_action_5(ActiveBiology& abio, Model& model, Present& present)
{
	//printf("Create animal\n");
	std::pair<unsigned int, unsigned int> axis = random_axis_within_k(abio, model, 3);
	if (axis.first != -1 && axis.second != -1) {
		ActiveBiologyPtr new_bio(new ActiveBiology(abio, model.get_unique_id(), axis.first, axis.second));
		model.add_active_bio(new_bio);
		new_bio->set_LD(abio.get_LD() * 0.5);
		abio.set_LD(abio.get_LD() * 0.75);
	}
	FsmSetCurEvent(PlantState::e_EVENT_3);
	match_step = 0;
}

void AnimalFSM::execute_action_6(ActiveBiology& abio, Model& model, Present& present)
{
	FsmSetCurEvent(AnimalState::e_EVENT_7);
	match_step++;
}

void AnimalFSM::execute_action_7(ActiveBiology& abio, Model& model, Present& present)
{
	match_step++;
	// 死亡
	if (get_LD(abio) <= 0) {
		FsmSetCurEvent(AnimalState::e_EVENT_DEAD);
		return;
	}
	std::pair<unsigned int, unsigned int> loc;
	// 猎物在相邻图块
	if (has_prey_within_k(abio, model, 1, loc)) {
		FsmSetCurEvent(AnimalState::e_EVENT_9);
		return;
	}
	// 找到猎物
	if (has_prey_within_k(abio, model, 10, loc)) {
		path.clear();
		if (!present.find_shortest_path(path, abio.getX(), abio.getY(), loc.first, loc.second)) {
			// std::cout << "no path" << std::endl;
			return;
		}
		//std::cout << "Find prey: " << loc.first << ", " << loc.second << std::endl;
		model.del_active_bio(abio, false);
		walk_to_somewhere(abio, model, path, 10, 1.0);
		ActiveBiologyPtr abio_ptr = model.get_active_bio(abio);
		model.add_active_bio(abio_ptr, false);
		FsmSetCurEvent(AnimalState::e_EVENT_7);
	}
	else {
		FsmSetCurEvent(AnimalState::e_EVENT_8);
	}
}

void AnimalFSM::execute_action_8(ActiveBiology& abio, Model& model, Present& present)
{
	match_step++;
	FsmSetCurEvent(AnimalState::e_EVENT_1);
}

void AnimalFSM::execute_action_9(ActiveBiology& abio, Model& model, Present& present)
{
	FsmSetCurEvent(AnimalState::e_EVENT_10);
}

void AnimalFSM::execute_action_10(ActiveBiology& abio, Model& model, Present& present)
{
	match_step++;
	// 死亡
	if (get_LD(abio) <= 0) {
		FsmSetCurEvent(AnimalState::e_EVENT_DEAD);
		return;
	}
	std::pair<unsigned int, unsigned int> loc;
	ActiveBiologyPtr prey;
	// 猎物在相邻图块
	if (prey = has_prey_within_k(abio, model, 1, loc)) {
		// 猎物死或捕食者足
		if (prey->get_LD() <= 0 || get_LD(abio) >= abio.get_life()) {
			FsmSetCurEvent(AnimalState::e_EVENT_11);
			return;
		}
		set_LD(get_LD(abio) + abio.get_strength() / 2, abio);
		prey->set_LD(prey->get_LD() - abio.get_strength());
		//std::cout << abio.get_name() << " Attack " << prey->get_name() 
		//	<< ": " << abio.get_LD() << ", " << prey->get_LD() << std::endl;
		FsmSetCurEvent(AnimalState::e_EVENT_10);
	}
	else if (prey = has_prey_within_k(abio, model, 10, loc)) {
		FsmSetCurEvent(AnimalState::e_EVENT_12);
	}
	else {
		FsmSetCurEvent(AnimalState::e_EVENT_11);
	}
}

void AnimalFSM::execute_action_11(ActiveBiology& abio, Model& model, Present& present)
{
	match_step++;
	FsmSetCurEvent(AnimalState::e_EVENT_1);
}

void AnimalFSM::execute_action_12(ActiveBiology& abio, Model& model, Present& present)
{
	match_step++;
	FsmSetCurEvent(AnimalState::e_EVENT_7);
}

void AnimalFSM::walk_to_somewhere(
	ActiveBiology& abio,
	Model& model,
	PathOrdered& path,
	unsigned int decay,
	float spd_boost)
{
	float move_range = (double)abio.get_speed() * spd_boost / 100.0;	// spd * spd-boost / 100
	tile& t = model.get_tile(abio.getY() * COL + abio.getX());
	if (abio.get_type() == 'l')
		move_range /= model.land_cost_table.at(t.get_terrain());
	else
		move_range /= model.water_cost_table.at(t.get_terrain());
	std::pair<unsigned int, unsigned int> next_int;
	while (move_range > 0.0 && !path.empty()) {
		float move = move_range >= 1.0 ? 1.0 : move_range;
		move_range -= move;
		next_int = path.back();
		// 横坐标相同，则是纵向走
		if (next_int.first == abio.getX()) {
			if (next_int.second < abio.getY())
				abio.set_loc(abio.getXF(), abio.getYF() - move);
			else
				abio.set_loc(abio.getXF(), abio.getYF() + move);
		}
		// 否则是横向走
		else {
			if (next_int.first < abio.getX())
				abio.set_loc(abio.getXF() - move, abio.getYF());
			else
				abio.set_loc(abio.getXF() + move, abio.getYF());
		}
		// 相等说明离开了上一个图块，到达了下一个图块
		if (next_int.first == abio.getX() && next_int.second == abio.getY()) {
			path.pop_back();
			set_LD(get_LD(abio) - decay, abio);
		}
	}
	std::cout << "Move range: " << move_range << ", Coordinate: " 
		<< abio.getXF() << ", " << abio.getYF() << std::endl;
}

void IFsmCallback::execute_action_dead(ActiveBiology& abio, Model& model, Present& present)
{
	short dead = abio.get_dead();
	if (dead < 0) {
		abio.set_dead(9);
		//printf("Dead!\n");
	}
	else if (dead == 0) {
		model.recycle_uid(abio.get_uid());
		model.del_active_bio(abio);
		//printf("Delete!\n");
	}
	else {
		abio.set_dead(dead - 1);
	}
}

IFsmCallback::IFsmCallback()
{
}

IFsmCallback::~IFsmCallback()
{
}

int IFsmCallback::FsmGetCurState() const
{
	return _curState;
}

void IFsmCallback::FsmSetCurState(int curState)
{
	_curState = curState;
}

int IFsmCallback::FsmGetCurEvent() const
{
	return _curEvent;
}

void IFsmCallback::FsmSetCurEvent(int curEvent)
{
	_curEvent = curEvent;
}

unsigned int IFsmCallback::get_LD(ActiveBiology& abio) const
{
	return abio.get_LD();
}

void IFsmCallback::set_LD(int ld, ActiveBiology& abio)
{
	ld = ld > 0 ? ld : 0;
	abio.set_LD(ld);
}

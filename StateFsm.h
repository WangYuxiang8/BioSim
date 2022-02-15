#pragma once
#ifndef __STATE_FSM_HPP__
#define __STATE_FSM_HPP__

#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include "State.h"
#include "biology.h"
#include "model.h"
#include "tools.h"
#include "tile.h"
#include "present.h"


// 随机选一个数字在0~x-1之间
#define random_X(x)         rand()%(x)
// 随机选一个数字在[a,b]之间
#define random_XY(x, y)		(rand() % (y-x+1)) + x; 

// 转移过程定义，包括事件、动作、下一状态
typedef struct _fsmTransition {
	int eventID;
	int nextState;
	int actionID;
}fsmTransition;

// 一个状态对应多个转移过程
typedef struct _fsmState {
	std::vector<fsmTransition> vecTransition;
}fsmState;

// 有限状态机模块抽象，包括事件、动作、状态
class StateFSM {
public:
	StateFSM();
	~StateFSM();

	int StartFsm(int stateID, IFsmCallback& fsmobj);
	int AddFsmConfig(int curState, int eventID, int nextState, int actionID);
	int doFsmEvent(int eventID, IFsmCallback& fsmobj, ActiveBiology& abio, Model& model, Present& present);

private:
	int getActionID(int curState, int eventID, int& nextState, int& actionID);

private:
	std::map<int, fsmState> mapState;

};

// FSM的回调类，存储当前状态，并执行相应动作
class IFsmCallback {

public:
	IFsmCallback();
	virtual ~IFsmCallback();

	virtual int FsmCallback(int actionID, ActiveBiology& abio, Model& model, Present& present) = 0;
	virtual void StartFSM(ActiveBiology& abio, Model& model, Present& present) = 0;

	int FsmGetCurState() const;
	void FsmSetCurState(int curState);
	int FsmGetCurEvent() const;
	void FsmSetCurEvent(int curEvent);

protected:
	unsigned int get_LD(ActiveBiology& abio) const;
	void set_LD(int ld, ActiveBiology& abio);
	// 获取k格内的一个随机坐标
	std::pair<unsigned int, unsigned int> random_axis_within_k(
		ActiveBiology& abio, 
		Model& model, 
		unsigned int k);
	// 计算k格范围
	void cal_within_k_range(int x, int y, int& sx, int& ex, int& sy, int& ey, int k);
	// 计算在k格之内相同生物的数量
	unsigned int same_bio_number_within_k(ActiveBiology& abio, Model& model, unsigned int k);
	// 计算在k格之内是否有猎物，如果有返回猎物指针，并且将猎物的坐标赋值到loc里；否则返回nullptr
	ActiveBiologyPtr has_prey_within_k(ActiveBiology& abio, Model& model, unsigned int k, std::pair<unsigned int, unsigned int>& loc);

	void execute_action_dead(ActiveBiology& abio, Model& model, Present& present);

private:
	int _curState;
	int _curEvent;
};

// 植物有限自动机回调类
class PlantFSM :public IFsmCallback {
public:
	PlantFSM();
	~PlantFSM();

	virtual int FsmCallback(int actionID, ActiveBiology& abio, Model& model, Present& present);
	virtual void StartFSM(ActiveBiology& abio, Model& model, Present& present);

private:
	void execute_action_1(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_2(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_3(ActiveBiology& abio, Model& model, Present& present);

private:
	StateFSM fsm;
	unsigned int LD_decay;
	unsigned int wait_step;
};

// 动物有限自动机回调类
class AnimalFSM :public IFsmCallback {
public:
	AnimalFSM();
	~AnimalFSM();

	virtual int FsmCallback(int actionID, ActiveBiology& abio, Model& model, Present& present);
	virtual void StartFSM(ActiveBiology& abio, Model& model, Present& present);

private:
	void execute_action_1(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_2(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_3(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_4(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_5(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_6(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_7(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_8(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_9(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_10(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_11(ActiveBiology& abio, Model& model, Present& present);
	void execute_action_12(ActiveBiology& abio, Model& model, Present& present);

	void walk_to_somewhere(
		ActiveBiology& abio, 
		Model& model, 
		PathOrdered& path,
		unsigned int decay,
		float spd_boost);

private:
	StateFSM fsm;
	unsigned int LD_decay;
	unsigned int wait_step;
	unsigned int match_step;
	// 动物移动的目标位置
	std::pair<unsigned int, unsigned int> target_loc;
	// 寻路路径
	PathOrdered path;
};

#endif


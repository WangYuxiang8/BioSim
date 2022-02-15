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


// ���ѡһ��������0~x-1֮��
#define random_X(x)         rand()%(x)
// ���ѡһ��������[a,b]֮��
#define random_XY(x, y)		(rand() % (y-x+1)) + x; 

// ת�ƹ��̶��壬�����¼�����������һ״̬
typedef struct _fsmTransition {
	int eventID;
	int nextState;
	int actionID;
}fsmTransition;

// һ��״̬��Ӧ���ת�ƹ���
typedef struct _fsmState {
	std::vector<fsmTransition> vecTransition;
}fsmState;

// ����״̬��ģ����󣬰����¼���������״̬
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

// FSM�Ļص��࣬�洢��ǰ״̬����ִ����Ӧ����
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
	// ��ȡk���ڵ�һ���������
	std::pair<unsigned int, unsigned int> random_axis_within_k(
		ActiveBiology& abio, 
		Model& model, 
		unsigned int k);
	// ����k��Χ
	void cal_within_k_range(int x, int y, int& sx, int& ex, int& sy, int& ey, int k);
	// ������k��֮����ͬ���������
	unsigned int same_bio_number_within_k(ActiveBiology& abio, Model& model, unsigned int k);
	// ������k��֮���Ƿ����������з�������ָ�룬���ҽ���������긳ֵ��loc����򷵻�nullptr
	ActiveBiologyPtr has_prey_within_k(ActiveBiology& abio, Model& model, unsigned int k, std::pair<unsigned int, unsigned int>& loc);

	void execute_action_dead(ActiveBiology& abio, Model& model, Present& present);

private:
	int _curState;
	int _curEvent;
};

// ֲ�������Զ����ص���
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

// ���������Զ����ص���
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
	// �����ƶ���Ŀ��λ��
	std::pair<unsigned int, unsigned int> target_loc;
	// Ѱ··��
	PathOrdered path;
};

#endif


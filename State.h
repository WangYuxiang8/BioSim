#pragma once
namespace PlantState
{
	enum State {
		s_WAIT = 0,
		s_GROW = 1,
		s_DEAD = 2
	};

	enum Event {
		e_EVENT_1 = 100,
		e_EVENT_2 = 101,
		e_EVENT_3 = 102,
		e_EVENT_4 = 103
	};

	enum Action {
		a_ACTION_1 = 200,
		a_ACTION_2 = 201,
		a_ACTION_3 = 202,
		a_ACTION_4 = 203
	};
}

namespace AnimalState
{
	enum State {
		s_WAIT = 0,
		s_DEAD = 1,
		s_MOVE = 2,
		s_CATCH = 3,
		s_ATTACK = 4
	};

	enum Event {
		e_EVENT_1 = 100,
		e_EVENT_2 = 101,
		e_EVENT_3 = 102,
		e_EVENT_4 = 103,
		e_EVENT_5 = 104,
		e_EVENT_6 = 105,
		e_EVENT_7 = 106,
		e_EVENT_8 = 107,
		e_EVENT_9 = 108,
		e_EVENT_10 = 109,
		e_EVENT_11 = 110,
		e_EVENT_12 = 111,
		e_EVENT_DEAD = 112
	};

	enum Action {
		a_ACTION_1 = 200,
		a_ACTION_2 = 201,
		a_ACTION_3 = 202,
		a_ACTION_4 = 203,
		a_ACTION_5 = 204,
		a_ACTION_6 = 205,
		a_ACTION_7 = 206,
		a_ACTION_8 = 207,
		a_ACTION_9 = 208,
		a_ACTION_10 = 209,
		a_ACTION_11 = 210,
		a_ACTION_12 = 211,
		a_ACTION_DEAD = 212
	};
}
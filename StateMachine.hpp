#pragma once
#include <memory>
#include <stack>
#include "State.hpp"

namespace rstar
{	
	using StatePtr = std::unique_ptr<State>;

	// class that manages all states actions
	// by using std::stack of class State pointers
	class StateMachine
	{
	public:
		void SetState(StatePtr newState, bool isReplacing = true);
		void RemoveActiveState();
		void UpdateStatesStack();

		StatePtr& GetActiveState();

	private:
		std::stack<StatePtr> states_;
		StatePtr newState_;
		bool replacing_{ false };
		bool adding_{ false };
		bool removing_{ false };
	};
}


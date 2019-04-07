#include "StateMachine.hpp"
#include <string>
#include <iostream>
#include <algorithm>


namespace rstar
{
	void StateMachine::SetState(StatePtr newState, bool isReplacing)
	{
		if (isReplacing && !states_.empty())
		{
			replacing_ = true;
		}
		newState_ = std::move(newState);
		adding_ = true;
	}

	void StateMachine::UpdatingStatesStack()
	{
		if (adding_)
		{
			if (replacing_)
			{
				states_.pop();
				replacing_ = false;
			}
			states_.push(std::move(newState_));
			adding_ = false;
		}
	}

	StatePtr& StateMachine::GetActiveState()
	{
		return states_.top();
	}
}

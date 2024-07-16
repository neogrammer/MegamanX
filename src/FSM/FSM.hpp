#ifndef FSM_HPP__
#define FSM_HPP__
#include <variant>
#include <optional>
template <typename Derived, typename StateVariant>
class FSM
{
	StateVariant state_;
	AnimType currType_{ AnimType::Count };
public:
	template<typename Event>
	void dispatch(Event&& event)
	{
		Derived& child = static_cast<Derived&>(*this);
		auto new_state = std::visit(
			[&](auto& s) -> std::optional<StateVariant>
			{
				return child.On_Event(s, std::forward<Event>(event));
			}, state_);
		if (new_state)
		{
			state_ = *std::move(new_state);
		}
	}

	
	AnimType getAnimType()
	{
		Derived& child = static_cast<Derived&>(*this);
		AnimType new_type = std::visit(
			[&](auto& s) -> AnimType
			{
				return child.getType(s);
			}, state_);
		currType_ = new_type;
		return currType_;
	}
};

#endif
#ifndef DUCK_FOLD_HPP__
#define DUCK_FOLD_HPP__
#include <misc/globals.hpp>
template<typename Fsm, typename... Events>
void dispatch(Fsm& fsm, Events&&... events)
{
	(fsm.dispatch(std::forward<Events>(events)), ...);
}
template<typename Fsm>
AnimType getAnimType(Fsm& fsm)
{
	auto t = fsm.getAnimType();
	return t;
}

#endif
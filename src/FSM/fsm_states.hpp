#ifndef FSM_STATES_HPP__
#define FSM_STATES_HPP__

#include <variant>
#include <misc/globals.hpp>
struct state_Standing { AnimType type_{ AnimType::Idle }; AnimType getType() { return type_; } };
struct state_Running { AnimType type_{ AnimType::Run }; AnimType getType() { return type_; } };
struct state_Rising { AnimType type_{ AnimType::Jump }; AnimType getType() { return type_; } };
struct state_Falling { AnimType type_{ AnimType::Jump }; AnimType getType() { return type_; } };
struct state_ShootingWhileStanding { AnimType type_{ AnimType::Idle }; AnimType getType() { return type_; } };
struct state_ShootingWhileRunning { AnimType type_{ AnimType::Idle }; AnimType getType() { return type_; } };
struct state_ShootingWhileRising { AnimType type_{ AnimType::Jump }; AnimType getType() { return type_; } };
struct state_ShootingWhileFalling { AnimType type_{ AnimType::Jump }; AnimType getType() { return type_; } };

using PlayerStateVar = std::variant<state_Standing, state_Running, state_Rising, state_Falling>;

#endif
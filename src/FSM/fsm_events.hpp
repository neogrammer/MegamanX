#ifndef FSM_EVENTS_HPP__
#define FSM_EVENTS_HPP__

struct evt_StartedMoving {};
struct evt_StoppedMoving {};
struct evt_StartedShooting {};
struct evt_StoppedShooting {};
struct evt_Jumped {};
struct evt_ReachedJumpPeak {};
struct evt_Landed {};
struct evt_Fell {};
struct evt_TransToRiseCompleted {};
struct evt_TransToFallingCompleted {};
struct evt_LandingTransCompleted {};


#endif
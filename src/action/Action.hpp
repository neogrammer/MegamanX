#ifndef ACTION_H__
#define ACTION_H__

#include <SFML/Window/Event.hpp>

struct Action
{
    enum Type
    {
        RealTime = 1,
        Pressed = 1 << 1,
        Released = 1 << 2,
        Tilted = 1 << 3
    };

    Action(const sf::Keyboard::Key& key,int type=Type::RealTime | Type::Pressed);
    Action(const sf::Mouse::Button& button,int type=Type::RealTime | Type::Pressed);

    Action(const unsigned int& joyButtonNum, int type = Type::RealTime | Type::Pressed);
    Action(const sf::Joystick::Axis& axis, int type = Type::RealTime | Type::Tilted);


    bool test()const;

    bool operator==(const sf::Event& event)const;
    bool operator==(const Action& other)const;


private:
    template<typename> friend class ActionTarget;
    sf::Event _event;
    int _type;
};

#endif

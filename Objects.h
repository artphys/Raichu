
#ifndef RAICHU_OBJECTS_H
#define RAICHU_OBJECTS_H
#endif //RAICHU_OBJECTS_H

#include "Field.h"

class Pikachu : public Player{
protected:
    int size;

public:
//    ViewPoint vp;

    Pikachu() = default;

    Pikachu(const Point& position, int size, PlayerCommands* commands);

    PlayerTypeParameters Parameters(int scale);

    void Update();

};

class Discharge : public Bullet{
public:
    Discharge (double size);
};

class IronTale : public Bullet{
public:
    IronTale(double size);
};

class Charmander : public Player{
protected:
    int size;

public:

    Charmander() = default;

    Charmander(const Point& position, int size, PlayerCommands* commands);

    PlayerTypeParameters Parameters(int scale);

    void Update();
};

class FireBall : public Bullet{
public:
    FireBall(double size);
};

class FireHit : public Bullet{
public:
    FireHit(double size);
};
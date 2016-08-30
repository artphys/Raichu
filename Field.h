#include <vector>
#include "TXLib.h"
#include "Point.h"

#ifndef RAICHU_FIELD_H
#define RAICHU_FIELD_H
#endif //RAICHU_FIELD_H

using namespace std;

class FieldObject;

struct FieldParameters{
    Point upper_left;
    Point down_right;
    COLORREF color;

    FieldParameters (const Point& upper_left, const Point& down_right, COLORREF color) :
            upper_left(upper_left), down_right(down_right), color(color) {}
};

class Field {
    FieldParameters parameters;
    vector <FieldObject*> objects;

    void ProcessCollisions ();

    void AddObjectsChildren ();

    void AddObject(FieldObject* new_obj_ptr);

    void DeleteNonExistingObjects ();

    void DrawBackground ();

public:

    Field (FieldParameters parameters, vector <FieldObject*> objects);

    void Draw ();

    void Update ();

    void PrintObjects();

    ~Field();
};

class FieldObject{
    bool exist;
    vector <FieldObject*> new_children = {};
    friend class Field;
protected:
    void Kill();

    void AddChild (FieldObject* child);

    virtual void Draw();

    virtual void Print();

    virtual bool IfCollides (const FieldObject&);

    virtual void Collide (FieldObject&);

    virtual void Update ();
public:
    FieldObject();

    bool IsDead();
};
//-------------------------------------------

class FieldLine : public FieldObject {
    friend class FieldCircle;
protected:
    Point start_pos;
    Point end_pos;
    COLORREF color;

public:

    FieldLine() = default;

    FieldLine (const Point& start_pos, const Point& end_pos, COLORREF color);

    void Draw ();

    Point Normalised();

    void Print ();

};

class FieldCircle : public FieldObject {
    friend class FieldLine;
protected:
    Point position;
    Point velocity;
    int radius;
    COLORREF color;

    bool IfCollides (const FieldObject& object);

    bool IfCollides (const FieldCircle& object);

    bool IfCollides (const FieldLine& object);

    void SetDirectionPosition (double new_direction, const Point& new_position);

    void Update ();

public:
    FieldCircle () = default;

    FieldCircle (int radius, COLORREF color);

    FieldCircle (const Point& position, int radius, COLORREF color);

    void Draw ();

    virtual void Print ();

};
//-------------------------------------------

class SolidLine : public FieldLine{
    friend class Player;
    friend class Bullet;
public:
    SolidLine(Point begin, Point end, COLORREF color) : FieldLine(begin, end, color){}
};

class Player;

class Bullet : public FieldCircle{
    friend class Player;
protected:
    int damage;

    void Collide (FieldObject& object);

    virtual void Collide (Bullet& bullet);

    virtual void Collide (SolidLine& line);

    virtual void Collide (Player& player);

    void Draw();

public:

    Bullet (int radius, COLORREF color, int damage, double speed);

    Bullet (const Point& position, int radius, COLORREF color, int damage, double direction, double speed);

    int Damage();
};

enum class ShootType {NONE, PUNCH, SHOT};

struct PlayerCommands{
    double direction;
    double speed;
    ShootType hit;
};

struct PlayerTypeParameters{
    COLORREF color;
    int radius;
    int max_health;
    int max_mana;
    double max_speed;
};

class Player : public FieldCircle{
protected:
    bool collided_solid = false;
    int health, max_health;
    int mana, max_mana;
    double max_speed;
    int time = 0;
    const PlayerCommands* commands;
public:

    Player (const Point& position, PlayerTypeParameters parameters, PlayerCommands* commands);

    void Collide (FieldObject& object);

    virtual void Collide (Bullet& bullet);

    virtual void Collide (SolidLine& line);

    virtual void Collide (Player& player);

    void ShootBullet (Bullet* bullet);

    void Draw();

   // void AddToViewPoint (ViewPoint& view);

    void Update ();

};

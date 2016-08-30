#include "Field.h"

//------------------------Field-------------------------

Field::Field(FieldParameters parameters, vector<FieldObject*> objects) : parameters (parameters), objects(objects){
    txCreateWindow (parameters.down_right.X() - parameters.upper_left.X(),
                    parameters.down_right.Y() - parameters.upper_left.Y());
}

void Field::ProcessCollisions() {
    for (auto object1 : objects)
        for (auto object2 : objects) {
            if (object1 == object2)
                continue;

            bool obj1_collides_obj2 = object1->IfCollides(*object2);
            bool obj2_collides_obj1 = object1->IfCollides(*object2);
            if (obj1_collides_obj2) {
                object1->Collide(*object2);
                if (!obj2_collides_obj1)
                    object2->Collide(*object1);
            }
        }
}

void Field::DrawBackground() {
    txSetColor (TX_BLACK);
    txSetFillColor (parameters.color);
    txRectangle (parameters.upper_left.X(), parameters.upper_left.Y(),
                 parameters.down_right.X(), parameters.down_right.Y());
}

void Field::Draw() {
    DrawBackground();
    for (auto object : objects) {
        object->Draw();
    }
}

void Field::AddObject(FieldObject* new_obj_ptr) {
    objects.push_back(new_obj_ptr);
    cout <<"Object Added" << endl;
}

void Field::AddObjectsChildren() {

    for (int i = 0; i < objects.size(); i++) {
        for (auto new_obj_ptr : objects[i]->new_children) {
            AddObject(new_obj_ptr);
        }
        objects[i]->new_children.clear();
    }
}

void Field::DeleteNonExistingObjects() {
    for (int i = 0; i < objects.size(); i++) {
        if (!objects[i]->IsDead()) {
            objects.erase(objects.begin() + i);
            i--;
            cout << "Object deleted" << endl;
        }
    }
}

void Field::Update() {
    AddObjectsChildren();
    for (auto object : objects)
        object->Update();
    ProcessCollisions();
    DeleteNonExistingObjects();
}

void Field::PrintObjects() {
    for (auto object : objects)
        object->Print();
}

Field::~Field() {
    for (auto object : objects)
        delete object;
}


/*
ViewPoint* Field::GeneratePlayerView(FieldObject* player) {
    auto vp = new ViewPoint();
    for (auto object : objects){
        if (object == player)
            continue;
        object->AddToViewPoint(*vp);
    }
    return vp;
}

void Field::DrawViewPoint(ViewPoint vp) {
    double  x = 0;
    double  y = 0;
    for (int i = 0; i < vp.array_size; i++) {
        if (vp.bullet_distances[i] == 0)
            continue;
        x = vp.position.X() + vp.bullet_distances[i]*cos(2*PI*i/vp.array_size + vp.direction);
        y = vp.position.Y() + vp.bullet_distances[i]*sin(2*PI*i/vp.array_size + vp.direction);
        txSetColor (TX_BLACK);
        txCircle (x, y, 4);
    }
}*/
//-------------FieldObject-------------------------------

FieldObject::FieldObject() : exist(true){

}

void FieldObject::Draw() {}

bool FieldObject::IfCollides(const FieldObject&) {
    return false;
}

void FieldObject::Collide(FieldObject &) {}

void FieldObject::Update() {}

void FieldObject::Print() {
    cout << "FieldObject :" << endl;
}



void FieldObject::Kill() {
    exist = false;
}

bool FieldObject::IsDead() {
    return exist;
}

void FieldObject::AddChild(FieldObject* child) {
    new_children.push_back(child);
}


//-------------------FieldLine---------------------------

FieldLine::FieldLine(const Point& start_pos, const Point& end_pos, COLORREF color) :
        FieldObject(), start_pos (start_pos), end_pos(end_pos), color(color) {

}

void FieldLine::Draw() {
    txSetColor (color);
    txLine (start_pos.X(), start_pos.Y(), end_pos.X(), end_pos.Y());
}

//--------------------FieldCircle-------------------------
FieldCircle::FieldCircle(int radius, COLORREF color) : radius(radius), color(color) {

}

FieldCircle::FieldCircle(const Point& position, int radius, COLORREF color)
        : position(position), radius(radius), color(color), velocity(0, 0) {
}

void FieldCircle::Draw() {
    txSetColor (TX_BLACK);
    txSetFillColor (color);
    txCircle (position.X(), position.Y(), radius);
}

bool FieldCircle::IfCollides(const FieldObject& object) {

    try{
        auto field_circle = dynamic_cast<const FieldCircle&> (object);
        return IfCollides (field_circle);
    }
    catch(...){}
    try{
        auto field_line = dynamic_cast<const FieldLine&> (object);
        return IfCollides (field_line);
    }
    catch(...){}
    return false;
}

bool FieldCircle::IfCollides(const FieldCircle& object) {
    if ((object.position - position).Length() <= (object.radius + radius))
        return true;
    return false;
}

bool IfCircleCollidesPoint (const Point& circle_position, double radius, Point point_position){
    if ((circle_position - point_position).Length() <= radius)
        return true;
    return false;
}

bool IfCircleCollidesLineMiddle (const Point& circle_position, double radius, const Point& start_pos, const Point& end_pos){
    Point line_vector = end_pos - start_pos;
    Point circle_vector = circle_position - start_pos;
    double circle_vector_length = circle_vector.Length();
    double projection_length = circle_vector*line_vector/line_vector.Length();
    double line_circle_distance_squared = circle_vector_length*circle_vector_length - projection_length*projection_length;

    if (projection_length < 0 || projection_length > line_vector.Length())
        return false;
    if (line_circle_distance_squared > radius*radius)
        return false;
    return true;
}

bool FieldCircle::IfCollides(const FieldLine& object) { // Rewrite using operators

    if (IfCircleCollidesPoint     (position, radius, object.start_pos) ||
        IfCircleCollidesPoint     (position, radius, object.end_pos)   ||
        IfCircleCollidesLineMiddle(position, radius, object.start_pos, object.end_pos))
        return true;
}

void FieldCircle::Update() {
    position += velocity;
}


void FieldCircle::Print() {
    cout << "FieldCircle:" << endl;
    cout << "pos(" << position.X() << ", " << position.Y() << ")" << endl;
    cout << "radius (" << radius << ")" << endl;
    cout << endl;
}

void FieldCircle::SetDirectionPosition(double new_direction, const Point& new_position) {
    position = new_position;
    velocity.SetDirection(new_direction);
}

//-----------------------FieldLine--------------------------
void FieldLine::Print() {
    cout << "FieldLine:" << endl;
    cout << "start_pos(" << start_pos.X() << ", " << start_pos.Y() << ")" << endl;
    cout <<   "end_pos(" <<   end_pos.X() << ", " <<   end_pos.Y() << ")" << endl;
    cout << endl;
}

Point FieldLine::Normalised() {
    Point p = start_pos - end_pos;
    return p/p.Length();
}

//-----------------------Player-----------------------------
Player::Player(const Point& position, PlayerTypeParameters parameters, PlayerCommands* commands)
        : FieldCircle(position, parameters.radius, parameters.color), commands(commands),
          max_health(parameters.max_health), max_mana(parameters.max_mana),
          max_speed(parameters.max_speed), health(max_health), mana(max_mana) {
}

void Player::ShootBullet(Bullet *bullet) {
    double bullet_relative_start_distance = 1.5;
    Point bullet_pos = position + velocity.Normalized()*radius*bullet_relative_start_distance;
    bullet->SetDirectionPosition(velocity.Direction(), bullet_pos);
    AddChild(bullet);
}

void Player::Draw() {
    FieldCircle::Draw();
    txSetColor(TX_BLACK);
    txLine (position.X(), position.Y(), position.X() + radius*cos(commands->direction), position.Y() + radius*sin(commands->direction));
    txSetColor(TX_GREEN);
    txLine (position.X(), position.Y(), position.X() + radius*cos(velocity.Direction()), position.Y() + radius*sin(velocity.Direction()));
}

void Player::Update() {
    velocity.SetDirectionLength(commands->direction, max_speed*commands->speed);
    FieldCircle::Update();
    time ++;
    collided_solid = false;
}

void Player::Collide (FieldObject& object) {
    try{
        auto bullet = dynamic_cast<Bullet&> (object);
        Collide (bullet);
        return;
    }
    catch(...){}
    try{
        auto player = dynamic_cast<Player&> (object);
        Collide (player);
        return;
    }
    catch(...){}
    try{
        auto solid_line = dynamic_cast<SolidLine&> (object);
        Collide (solid_line);
        return;
    }
    catch(...){}
    cout << "Unknown object!" << endl;
}

void Player::Collide (Bullet& bullet) {
    health -= bullet.Damage();
}

void Player::Collide (SolidLine& line) { // TODO : Rewrite using functions instead of copy-paste

    bool collided_start = IfCircleCollidesPoint     (position, radius, line.start_pos);
    bool collided_end = IfCircleCollidesPoint     (position, radius, line.end_pos);
    bool collided_middle = IfCircleCollidesLineMiddle(position, radius, line.start_pos, line.end_pos);

    if (collided_middle){
        collided_start = false;
        collided_end   = false;
    }

    Point collide_point;

    if (collided_start)
        collide_point = line.start_pos;
    if (collided_end)
        collide_point = line.end_pos;

    if (collided_start || collided_end) {
        position -= velocity;
        auto norm_vect = (position - collide_point).Normalized();
        velocity -= norm_vect * (norm_vect * velocity);
        if (!collided_solid)
            position += velocity;
    }

    collided_middle = IfCircleCollidesLineMiddle(position, radius, line.start_pos, line.end_pos);

    if (collided_middle) {
        position -= velocity;
        velocity = line.Normalised() * (velocity * line.Normalised());
        if (!collided_solid)
            position += velocity;
    }
    collided_solid = true;
}

void Player::Collide (Player& player) {
}

//----------------------Bullet------------------------------

void Bullet::Collide (FieldObject& object) {
    try{
        auto bullet = dynamic_cast<Bullet&> (object);
        Collide (bullet);
        return;
    }
    catch(...){}
    try{
        auto player = dynamic_cast<Player&> (object);
        Collide (player);
        return;
    }
    catch(...){}
    try{
        auto solid_line = dynamic_cast<SolidLine&> (object);
        Collide (solid_line);
        return;
    }
    catch(...){}
    cout << "Unknown object!" << endl;
}

void Bullet::Collide (Bullet& bullet) { //--------------------------------------------------------------------------------++++
    Kill();
}

void Bullet::Collide (SolidLine& line) {
    position -= velocity;
    velocity.SetDirection(2*line.Normalised().Direction() - velocity.Direction());
    position += velocity;
}

void Bullet::Collide (Player& player) {
    Kill();
}


Bullet::Bullet(int radius, COLORREF color, int damage, double speed)
        : FieldCircle(radius, color), damage(damage) {
    velocity.SetLength(speed);
}

Bullet::Bullet(const Point& position, int radius, COLORREF color, int damage, double direction, double speed)
        : FieldCircle(position, radius, color), damage(damage) {
    velocity.SetDirectionLength(direction, speed);
}

int Bullet::Damage() {
    return damage;
}

void Bullet::Draw() {
    if (radius <= 7){
        txSetColor (TX_BLUE);
        txSetFillColor (TX_BLUE);
        txCircle (position.X(), position.Y(), 7);
    }
    FieldCircle::Draw();
}

/*

double Cycle (double num, double max_num) {
     if (num >= max_num)
         return Cycle(num - max_num, max_num);
     if (num < 0)
         return Cycle(num + max_num, max_num);
     return num;
 }

double AngleToStandardRange (double angle) { // from 0 to 2*PI
    angle = Cycle (angle, 2*PI);
    return angle;
}

int AngleToNum (double angle, int max_num) {
    angle = AngleToStandardRange(angle);
    return angle*max_num/2/PI;
}

double NumToAngle (int num, int max_num) {
    num = Cycle(num, max_num);
    return 2*PI*num/max_num;
}*/

/*void Bullet::AddToViewPoint(ViewPoint &view) {
    Point relative_position =  position - view.position;
    double half_angle_size = radius/relative_position.Length();
    double relative_direction = AngleToStandardRange(relative_position.Direction() - view.direction);

    cout << "relative direction = " << relative_direction << endl;
    cout << "half angle size = " << half_angle_size << endl;


    int i0 = AngleToNum(relative_direction - half_angle_size, view.array_size);
    cout << "i0 = " << i0 << endl;
    cout << "zero_angle = " << NumToAngle(i0, view.array_size)<< endl;

    cout << "final_num" << AngleToNum(relative_direction + half_angle_size, view.array_size) << endl;

    double j = 0;
    for (int i = AngleToNum(relative_direction - half_angle_size, view.array_size), j = half_angle_size*view.array_size/PI;
            j > 0; i++, j--, i = Cycle(i, view.array_size)){
        if (view.bullet_distances[i] > relative_position.Length() || view.bullet_distances[i] == 0)
            view.bullet_distances[i] = relative_position.Length();
        cout << "i = " << i << endl;
        cout <<"angle = " << NumToAngle(i, view.array_size) << endl;

    }
}*/


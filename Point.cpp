#include "Point.h"

Point::Point(double x, double y)
        : x(x), y(y) {
}

Point Point::operator+(const Point& p) const {
    Point p_output(p.X() + x, p.Y() + y);
    return p_output;
}

Point Point::operator-(const Point& p) const {
    Point p_output(x - p.X(), y - p.Y());
    return p_output;
}

Point Point::operator*(double t) const {
    Point p_output(x*t, y*t);
    return p_output;
}

Point Point::operator/(double t) const {
    if (!t){
        throw ("ZERO DIVIDER!\n");
    }
    Point p_output(x/t, y/t);
    return p_output;
}

double Point::operator*(const Point& P) const {
    return P.X()*x + P.Y()*y;
}

Point Point::operator+=(const Point& P) {
    SetXY(x + P.X(), y + P.Y());
    return *this;
}

Point Point::operator-=(const Point& P) {
    SetXY(x - P.X(), y - P.Y());
    return *this;
}

double Point::Length() {
    if (!length_calculated)
        length = hypot(x, y);
    length_calculated = true;
    return length;
}

void Point::SetXY(double new_x, double new_y) {
    x = new_x;
    y = new_y;
    direction_calculated = false;
    length_calculated = false;
}

Point Point::operator*=(double t) {
    SetXY (x*t, y*t);
}

void Point::SetDirectionLength(double new_direction, double new_length) {
    direction = new_direction;
    length = new_length;
    direction_calculated = true;
    length_calculated = true;
    XYFromDirectionLength();
}

void Point::XYFromDirectionLength() {
    x = length*cos(direction);
    y = length*sin(direction);
}

double Point::X() const{
    return x;
}

double Point::Y() const{
    return y;
}

double Point::Direction() {

    if ( Length() != 0 && !direction_calculated){
        direction = acos(x/Length());
        if (y < 0)
            direction *= -1;
    }
    direction_calculated = true;
    return direction;
}

void Point::Print() {
    cout << "Point : {";
    cout << " x = " << x;
    cout << " y = " << y;
    cout << " direction = " << direction;
    cout << " length = " << length;
    cout << " }" << endl;
}


Point Point::Normalized() {
    Point p_output;
    p_output.SetDirectionLength(Direction(), 1);
    return p_output;
}

void Point::SetDirection(double new_direction) {
    direction = new_direction;
    direction_calculated = true;
    XYFromDirectionLength();
}

void Point::SetLength(double new_length) {
    length = new_length;
    length_calculated = true;
    XYFromDirectionLength();
}

Point operator*(double t, const Point& p){
    return p*t;
}






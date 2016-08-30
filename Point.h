#include "math.h"
#include "iostream"

#ifndef RAICHU_POINT_H
#define RAICHU_POINT_H

#endif //RAICHU_POINT_H

using namespace std;

const double PI = 3.1415926;

struct Point{
private:
    double x;
    double y;
    double direction;
    double length;
    bool direction_calculated = false;
    bool length_calculated = false;

    void XYFromDirectionLength();

public:

    Point() = default;

    Point (double x, double y);

    double Direction();

    double Length();

    double X() const;

    double Y() const;

    Point Normalized ();

    void SetXY(double new_x, double new_y);

    void SetDirectionLength(double new_direction, double new_length);

    void SetDirection(double new_direction);

    void SetLength(double new_length);

    Point operator*(double t) const;

    Point operator/(double t) const;

    double operator*(const Point& p) const;

    Point operator+(const Point& p) const;

    Point operator-(const Point& p) const;

    Point operator*=(double t);

    Point operator+=(const Point& p);

    Point operator-=(const Point& p);

    void Print();
};

Point operator*(double t, const Point& p);



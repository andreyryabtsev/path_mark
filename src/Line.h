/*
Minimal line struct
*/
#ifndef _LINE_H_
#define _LINE_H_
struct Line {
 public:
    double x1, y1, x2, y2;
    Line(double x1_, double y1_, double x2_, double y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
};
#endif
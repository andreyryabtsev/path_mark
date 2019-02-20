/*
Minimal rectangle struct
*/
#ifndef _RECT_H_
#define _RECT_H_
struct Rect {
 public:
    double x1, y1, x2, y2;
    Rect(double x1_, double y1_, double x2_, double y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
};
#endif
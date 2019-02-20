/*
This interface is extended by all classes that need to be visualized.
*/

#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_
#include "Renderer.h"
class Renderer;
class Drawable {
 public:
    virtual void draw(Renderer& renderer) const = 0;
};
#endif
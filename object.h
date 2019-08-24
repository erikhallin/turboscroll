#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <gl/gl.h>
#include "definitions.h"

class object
{
    public:
        object();
        object(float left,float right,float top,float low);

        bool init(float left,float right,float top,float low);
        bool update();
        bool draw();

        bool collide_test(float left,float right,float top,float low);
        bool collide_test2(float left,float right,float top,float low);



        float m_pos_tl[2];
        float m_pos_tr[2];
        float m_pos_ll[2];
        float m_pos_lr[2];

    private:
};

#endif // OBJECT_H

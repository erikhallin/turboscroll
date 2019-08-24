#ifndef HAT_H
#define HAT_H

#include <iostream>
#include <gl/gl.h>
#include "definitions.h"

class hat
{
    public:
        hat();
        hat(int color,int texture,float pos[2],float speed[2]);

        bool init(int color,int texture,float pos[2],float speed[2]);
        bool update();
        bool draw();
        bool collide_test(float left,float right,float top,float low);

        bool m_idle;//no col test with objects
        bool m_highlight;

        float m_pos[2];
        float m_speed[2];
        float m_size[2];

        int m_texture;
        int m_color;

    private:
};

#endif // HAT_H

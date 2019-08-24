#ifndef RACK_H
#define RACK_H

#include <iostream>
#include <gl/gl.h>
#include "definitions.h"

class rack
{
    public:
        rack();
        rack(int texture,float pos[2]);

        bool init(int texture,float pos[2]);
        bool update();
        bool draw();

        bool collide_test(float left,float right,float top,float low);

        bool m_highlight;

        int  m_hats_left;

    private:

        float m_pos[2];


        int  m_texture;

};

#endif // RACK_H

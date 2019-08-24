#include "rack.h"

rack::rack()
{
    //ctor
}

rack::rack(int texture,float pos[2])
{
    init(texture,pos);
}

bool rack::init(int texture,float pos[2])
{
    m_pos[0]=pos[0];
    m_pos[1]=pos[1];

    m_texture=texture;

    m_hats_left=4;
    m_highlight=false;

    return true;
}

bool rack::update()
{


    return true;
}

bool rack::draw()
{
    float tex_size_x=510.0;
    float tex_size_y=204.0;
    float frame_size=102.0;
    int rack_frame=4-m_hats_left;
    float tex_x_pos=rack_frame*frame_size/tex_size_x;

    //temp
    float row_shift_hi=0.0;
    float row_shift_lo=frame_size/tex_size_y;
    if(m_highlight)
    {
        row_shift_hi=(frame_size+1.0)/tex_size_y;
        row_shift_lo=1.0;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,m_texture);
    glEnable(GL_BLEND);

    glBegin(GL_QUADS);
    glTexCoord2f(tex_x_pos,row_shift_hi);
    glVertex2f(m_pos[0]-frame_size*0.5,m_pos[1]-frame_size*0.5);
    glTexCoord2f(tex_x_pos,row_shift_lo);
    glVertex2f(m_pos[0]-frame_size*0.5,m_pos[1]+frame_size*0.5);
    glTexCoord2f(tex_x_pos+frame_size/tex_size_x,row_shift_lo);
    glVertex2f(m_pos[0]+frame_size*0.5,m_pos[1]+frame_size*0.5);
    glTexCoord2f(tex_x_pos+frame_size/tex_size_x,row_shift_hi);
    glVertex2f(m_pos[0]+frame_size*0.5,m_pos[1]-frame_size*0.5);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    return true;
}

bool rack::collide_test(float left,float right,float top,float low)
{
    return ! ( left >= m_pos[0]+_rack_width || right <= m_pos[0]-_rack_width
            || top >= m_pos[1]+_rack_height || low <= m_pos[1]-_rack_height );
}

#include "hat.h"

hat::hat()
{
    //ctor
}

hat::hat(int color,int texture,float pos[2],float speed[2])
{
    init(color,texture,pos,speed);
}

bool hat::init(int color,int texture,float pos[2],float speed[2])
{
    m_pos[0]=pos[0];
    m_pos[1]=pos[1];

    m_speed[0]=speed[0];
    m_speed[1]=speed[1]*0.5-100.0;

    m_color=color;
    //m_color=rand()%4;
    m_texture=texture;
    m_size[0]=_hat_width;
    m_size[1]=_hat_height;
    m_idle=false;
    m_highlight=false;

    return true;
}

bool hat::update()
{
    if(!m_idle)
    {
        //update pos
        m_pos[0]+=m_speed[0]*_time_step*0.001;
        m_pos[1]+=m_speed[1]*_time_step*0.001;

        //update speed
        m_speed[1]+=_hat_gravity;
    }

    return true;
}

bool hat::draw()
{
    float tex_size_x=144.0;
    float tex_size_y=36.0*2.0;
    float frame_size=36.0;
    float tex_x_pos=m_color*frame_size/tex_size_x;

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

bool hat::collide_test(float left,float right,float top,float low)
{
    return ! ( left >= m_pos[0]+m_size[0] || right <= m_pos[0]-m_size[0]
            || top >= m_pos[1]+m_size[1] || low <= m_pos[1]-m_size[1] );
}


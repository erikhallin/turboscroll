#include "object.h"

object::object()
{
    //ctor
}

object::object(float left,float right,float top,float low)
{
    init(left,right,top,low);
}

bool object::init(float left,float right,float top,float low)
{
    m_pos_tl[0]=left; m_pos_tl[1]=top;
    m_pos_tr[0]=right; m_pos_tr[1]=top;
    m_pos_ll[0]=left; m_pos_ll[1]=low;
    m_pos_lr[0]=right; m_pos_lr[1]=low;

    return true;
}

bool object::update()
{

    return true;
}

bool object::draw()
{
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
    glVertex2f(m_pos_tl[0],m_pos_tl[1]);
    glVertex2f(m_pos_ll[0],m_pos_ll[1]);
    glVertex2f(m_pos_lr[0],m_pos_lr[1]);
    glVertex2f(m_pos_tr[0],m_pos_tr[1]);
    glEnd();

    return true;
}

bool object::collide_test(float left,float right,float top,float low)
{
    return ! ( left >= m_pos_tr[0] || right <= m_pos_tl[0]
            || top >= m_pos_lr[1] || low <= m_pos_tr[1] );
}

bool object::collide_test2(float left,float right,float top,float low)
{
    return ! ( left > m_pos_tr[0] || right < m_pos_tl[0]
            || top > m_pos_lr[1] || low <= m_pos_tr[1] );
}

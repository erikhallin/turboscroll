#include "player.h"

player::player()
{
    //ctor
}

player::player(int color,int tex_player,int tex_hat,float pos[2])
{
    init(color,tex_player,tex_hat,pos);
}

bool player::init(int color,int tex_player,int tex_hat,float pos[2])
{
    m_pos_new[0]=m_pos[0]=pos[0];
    m_pos_new[1]=m_pos[1]=pos[1];
    m_speed[0]=m_speed[1]=0;
    m_size[0]=_player_width;
    m_size[1]=_player_height;
    m_jump_ready=false;
    m_trig_jump=false;
    m_throw_ready=true;
    m_trig_key_x=false;
    m_face_right=true;
    m_throw_force=0;
    m_tex_player=tex_player;
    m_tex_hat=tex_hat;
    m_run_prog=0;
    m_throw_delay=0;
    m_hat_in_hand=false;
    m_throw_block=false;
    m_color=color;
    m_jump_prolong=false;

    return true;
}

bool player::update(bool jumping)
{
    //update pos
    float up_speed_cap=m_speed[1];
    if(up_speed_cap<-_fall_speed_max) up_speed_cap=-_fall_speed_max;
    m_pos_new[0]=m_pos[0]+m_speed[0]*_time_step*0.001;
    m_pos_new[1]=m_pos[1]+up_speed_cap*_time_step*0.001;

    //cap min pos, max?
    if(m_pos_new[0]<0) m_pos_new[0]=0;
    if(m_pos_new[1]<0) m_pos_new[1]=0;
    //if(m_pos_new[0]>) m_pos_new[0]=0;

    if(jumping && m_speed[1]<0)
     m_speed[1]+=_player_gravity*0.5;
    else
     m_speed[1]+=_player_gravity;

    if(m_speed[1]>_fall_speed_max) m_speed[1]=_fall_speed_max;

    while(m_run_prog>=1.0) m_run_prog-=1.0;

    if(m_throw_delay>0) m_throw_delay-=_time_step*0.001;

    //hat limit reach test
    if((int)m_vec_hats_on.size()>=_player_hat_limit)
    {
        return false;
    }

    return true;
}

bool player::draw()
{
    glPushMatrix();

    float frame_size_x=96.0;
    float frame_size_y=96.0;
    float tex_size_x=480.0;
    float tex_size_y=383.0;
    float frame_torso=int(m_run_prog*5.0);
    float frame_leg=int(m_run_prog*5.0);
    float row_torso=0.0;//0=run torso, 1=fall/throw, 2=leg run, 3=idle
    float row_leg=2.0;//0=run torso, 1=fall/throw, 2=leg run, 3=idle
    float height_shift=18.0;

    //jump
    if(!m_jump_ready)
    {
        row_torso=1.0;
        row_leg=3.0;
        frame_torso=0.0;
        frame_leg=0.0;
    }
    //throw
    if(m_throw_force>_throw_force_min)
    {
        row_torso=1.0;
        frame_torso=1.0;
    }
    if(m_throw_delay>0.0)
    {
        row_torso=1.0;
        frame_torso=2.0;
    }

    glColor3f(1,1,1);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,m_tex_player);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(m_face_right)
    {
        //torso
        glBegin(GL_QUADS);
        glTexCoord2f( frame_size_x*(frame_torso+1.0)/tex_size_x, frame_size_y*(row_torso+0.0)/tex_size_y );
        glVertex2f(m_pos[0]-frame_size_x*0.5,m_pos[1]-frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_torso+1.0)/tex_size_x, frame_size_y*(row_torso+1.0)/tex_size_y );
        glVertex2f(m_pos[0]-frame_size_x*0.5,m_pos[1]+frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_torso+0.0)/tex_size_x, frame_size_y*(row_torso+1.0)/tex_size_y );
        glVertex2f(m_pos[0]+frame_size_x*0.5,m_pos[1]+frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_torso+0.0)/tex_size_x, frame_size_y*(row_torso+0.0)/tex_size_y );
        glVertex2f(m_pos[0]+frame_size_x*0.5,m_pos[1]-frame_size_y*0.5-height_shift);
        glEnd();

        //legs
        glBegin(GL_QUADS);
        glTexCoord2f( frame_size_x*(frame_leg+1.0)/tex_size_x, frame_size_y*(row_leg+0.0)/tex_size_y );
        glVertex2f(m_pos[0]-frame_size_x*0.5,m_pos[1]-frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_leg+1.0)/tex_size_x, frame_size_y*(row_leg+1.0)/tex_size_y );
        glVertex2f(m_pos[0]-frame_size_x*0.5,m_pos[1]+frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_leg+0.0)/tex_size_x, frame_size_y*(row_leg+1.0)/tex_size_y );
        glVertex2f(m_pos[0]+frame_size_x*0.5,m_pos[1]+frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_leg+0.0)/tex_size_x, frame_size_y*(row_leg+0.0)/tex_size_y );
        glVertex2f(m_pos[0]+frame_size_x*0.5,m_pos[1]-frame_size_y*0.5-height_shift);
        glEnd();

        //cout<<frame_size_y*(row_leg+1.0)/tex_size_y<<endl;
    }
    else
    {

        //torso
        glBegin(GL_QUADS);
        glTexCoord2f( frame_size_x*(frame_torso+0.0)/tex_size_x, frame_size_y*(row_torso+0.0)/tex_size_y );
        glVertex2f(m_pos[0]-frame_size_x*0.5,m_pos[1]-frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_torso+0.0)/tex_size_x, frame_size_y*(row_torso+1.0)/tex_size_y );
        glVertex2f(m_pos[0]-frame_size_x*0.5,m_pos[1]+frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_torso+1.0)/tex_size_x, frame_size_y*(row_torso+1.0)/tex_size_y );
        glVertex2f(m_pos[0]+frame_size_x*0.5,m_pos[1]+frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_torso+1.0)/tex_size_x, frame_size_y*(row_torso+0.0)/tex_size_y );
        glVertex2f(m_pos[0]+frame_size_x*0.5,m_pos[1]-frame_size_y*0.5-height_shift);
        glEnd();

        //legs
        glBegin(GL_QUADS);
        glTexCoord2f( frame_size_x*(frame_leg+0.0)/tex_size_x, frame_size_y*(row_leg+0.0)/tex_size_y );
        glVertex2f(m_pos[0]-frame_size_x*0.5,m_pos[1]-frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_leg+0.0)/tex_size_x, frame_size_y*(row_leg+1.0)/tex_size_y );
        glVertex2f(m_pos[0]-frame_size_x*0.5,m_pos[1]+frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_leg+1.0)/tex_size_x, frame_size_y*(row_leg+1.0)/tex_size_y );
        glVertex2f(m_pos[0]+frame_size_x*0.5,m_pos[1]+frame_size_y*0.5-height_shift);
        glTexCoord2f( frame_size_x*(frame_leg+1.0)/tex_size_x, frame_size_y*(row_leg+0.0)/tex_size_y );
        glVertex2f(m_pos[0]+frame_size_x*0.5,m_pos[1]-frame_size_y*0.5-height_shift);
        glEnd();
    }

    //draw hats
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,m_tex_hat);
    int hat_wobble_dist=-3;
    if(int(frame_torso)%2!=0 && row_torso==0) hat_wobble_dist=0;
    glTranslatef(0,-_player_height+12+hat_wobble_dist,0);
    int hat_shift=10;
    for(unsigned int hat_i=0;hat_i<m_vec_hats_on.size();hat_i++)
    {
        float tex_size_x=144.0;
        //float tex_size_y=36.0;
        float frame_size=36.0;
        float tex_x_pos=m_vec_hats_on[hat_i]*frame_size/tex_size_x;


        glTranslatef(0,-hat_shift,0);
        glBegin(GL_QUADS);
        glTexCoord2f(tex_x_pos,0.0);
        glVertex2f(m_pos[0]-frame_size*0.5,m_pos[1]-frame_size*0.5);
        glTexCoord2f(tex_x_pos,0.5);
        glVertex2f(m_pos[0]-frame_size*0.5,m_pos[1]+frame_size*0.5);
        glTexCoord2f(tex_x_pos+frame_size/tex_size_x,0.5);
        glVertex2f(m_pos[0]+frame_size*0.5,m_pos[1]+frame_size*0.5);
        glTexCoord2f(tex_x_pos+frame_size/tex_size_x,0.0);
        glVertex2f(m_pos[0]+frame_size*0.5,m_pos[1]-frame_size*0.5);
        glEnd();
    }
    glPopMatrix();

    //draw hat to throw
    if(m_throw_force>_throw_force_min)
    {
        glPushMatrix();

        float tex_size_x=144.0;
        //float tex_size_y=36.0;
        float frame_size=36.0;
        float tex_x_pos=m_color*frame_size/tex_size_x;

        if(m_face_right)
        {
            glTranslatef(_player_width*0.9-m_throw_force*8.0,-4,0);
        }
        else
        {
            glTranslatef(-_player_width*0.9+m_throw_force*8.0,-4,0);
        }
        glBegin(GL_QUADS);
        glTexCoord2f(tex_x_pos,0.0);
        glVertex2f(m_pos[0]-frame_size*0.5,m_pos[1]-frame_size*0.5);
        glTexCoord2f(tex_x_pos,0.5);
        glVertex2f(m_pos[0]-frame_size*0.5,m_pos[1]+frame_size*0.5);
        glTexCoord2f(tex_x_pos+frame_size/tex_size_x,0.5);
        glVertex2f(m_pos[0]+frame_size*0.5,m_pos[1]+frame_size*0.5);
        glTexCoord2f(tex_x_pos+frame_size/tex_size_x,0.0);
        glVertex2f(m_pos[0]+frame_size*0.5,m_pos[1]-frame_size*0.5);
        glEnd();

        glPopMatrix();
    }


    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glPopMatrix();

    return true;
}

bool player::accept_pos(bool pos_ok)
{
    if(pos_ok)
    {
        m_pos[0]=m_pos_new[0];
        m_pos[1]=m_pos_new[1];
    }
    else
    {
        m_pos_new[0]=m_pos[0];
        m_pos_new[1]=m_pos[1];
    }

    return true;
}


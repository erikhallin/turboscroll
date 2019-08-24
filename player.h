#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <gl/gl.h>
#include <vector>
#include "definitions.h"

using namespace std;

class player
{
    public:
        player();
        player(int color,int tex_player,int tex_hat,float pos[2]);

        bool init(int color,int tex_player,int tex_hat,float pos[2]);
        bool update(bool jumping);
        bool draw();

        bool accept_pos(bool pos_ok);

        float m_pos[2];
        float m_pos_new[2];
        float m_speed[2];
        float m_size[2];
        float m_throw_force;
        float m_run_prog;
        float m_throw_delay;

        int   m_tex_player,m_tex_hat;
        int   m_color;

        bool  m_jump_ready,m_trig_jump,m_jump_prolong;
        bool  m_throw_ready,m_trig_key_x,m_throw_block;
        bool  m_face_right;
        bool  m_hat_in_hand;

        vector<int> m_vec_hats_on;

    private:
};

#endif // PLAYER_H

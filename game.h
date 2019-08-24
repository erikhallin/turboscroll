#ifndef GAME_H
#define GAME_H

#include <SOIL/SOIL.h>
#include <iostream>
#include <gl/gl.h>
#include <vector>
#include <time.h>
#include "player.h"
#include "object.h"
#include "gamepad.h"
#include "hat.h"
#include "rack.h"
#include "sound.h"

/*TODO

win, gameover

huvudhitbox större

fysik

? jump pixel bugg

? player join om trycker start

*/

enum game_states
{
    gs_init=0,
    gs_menu,
    gs_info,
    gs_ingame
};

using namespace std;

class game
{
    public:
        game();

        bool init(bool* pKeys,int screen_width,int screen_height);
        bool update();
        bool draw();

    private:

        int m_game_state;

        bool* m_pKeys;
        vector<player>  m_vec_players;
        vector<object>  m_vec_objects;
        vector<hat>     m_vec_hats;
        vector<rack>    m_vec_racks;

        gamepad m_gamepad[4];
        bool    m_active_player[4];

        int m_tex_player[4],m_tex_hat,m_tex_rack,m_tex_level;
        int m_tex_menu,m_tex_info;

        int m_screen_size[2];
        float m_screen_scale[2];
        int m_screen_height_adjust;

        bool m_trig_menu;

        sound*  m_pSound;

        bool load_texture();
        bool load_sound();
};

#endif // GAME_H

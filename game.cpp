#include "game.h"

game::game()
{
    m_game_state=gs_init;
}

bool game::init(bool* pKeys,int screen_width,int screen_height)
{
    cout<<"Game init\n";

    m_screen_size[0]=screen_width;
    m_screen_size[1]=screen_height;
    m_screen_height_adjust=0;

    float scale_x=m_screen_size[0]/1920.0;
    float scale_y=m_screen_size[1]/1080.0;
    //cap y ratio to 16:9
    if(m_screen_size[0]/m_screen_size[1]<16.0/9.0)
    {
        //force smaller screen height
        m_screen_size[1]=m_screen_size[0]/(16.0/9.0);
        scale_y=m_screen_size[1]/1080.0;

        //calc dist to center of screen
        m_screen_height_adjust=(screen_height-m_screen_size[1])*0.5;
    }

    m_screen_scale[0]=scale_x;
    m_screen_scale[1]=scale_y;
    scale_x=1;
    scale_y=1;

    srand(time(0));

    m_pKeys=pKeys;

    //gamepads
    for(int i=0;i<4;i++)
    {
        m_active_player[i]=false;

        m_gamepad[i]=gamepad(i);
        if( m_gamepad[i].IsConnected() )
         m_active_player[i]=true;
    }

    //texture
    if(!load_texture()) return false;

    //sound
    if(!load_sound())   return false;

    //play music
    m_pSound->playSimpleSound(ogg_music_main,1.0,_sound_music_chan,true);

    //create players
    float pos[2]={0,0};
    pos[0]=345*scale_x;
    pos[1]=900*scale_y;
    m_vec_players.push_back( player( 0,m_tex_player[0],m_tex_hat,pos ) );
    pos[0]=735*scale_x;
    pos[1]=900*scale_y;
    m_vec_players.push_back( player( 1,m_tex_player[1],m_tex_hat,pos ) );
    pos[0]=1185*scale_x;
    pos[1]=900*scale_y;
    m_vec_players.push_back( player( 2,m_tex_player[2],m_tex_hat,pos ) );
    pos[0]=1575*scale_x;
    pos[1]=900*scale_y;
    m_vec_players.push_back( player( 3,m_tex_player[3],m_tex_hat,pos ) );


    //create platform objects
    m_vec_objects.push_back( object( 30*scale_x,(30+60)*scale_x,390*scale_y,(390+60)*scale_y ) );
    m_vec_objects.push_back( object( 30*scale_x,(30+60)*scale_x,750*scale_y,(750+60)*scale_y ) );
    m_vec_objects.push_back( object( 90*scale_x,(90+90)*scale_x,420*scale_y,(420+30)*scale_y ) );
    m_vec_objects.push_back( object( 90*scale_x,(90+150)*scale_x,780*scale_y,(780+30)*scale_y ) );
    m_vec_objects.push_back( object( 240*scale_x,(240+30)*scale_x,210*scale_y,(210+60)*scale_y ) );
    m_vec_objects.push_back( object( 270*scale_x,(270+210)*scale_x,240*scale_y,(240+30)*scale_y ) );
    m_vec_objects.push_back( object( 270*scale_x,(270+150)*scale_x,600*scale_y,(600+30)*scale_y ) );
    m_vec_objects.push_back( object( 360*scale_x,(360+360)*scale_x,420*scale_y,(420+30)*scale_y ) );
    m_vec_objects.push_back( object( 540*scale_x,(540+90)*scale_x,390*scale_y,(390+30)*scale_y ) );
    m_vec_objects.push_back( object( 450*scale_x,(450+180)*scale_x,780*scale_y,(780+30)*scale_y ) );
    m_vec_objects.push_back( object( 660*scale_x,(660+210)*scale_x,600*scale_y,(600+30)*scale_y ) );
    m_vec_objects.push_back( object( 1050*scale_x,(1050+210)*scale_x,600*scale_y,(600+30)*scale_y ) );
    m_vec_objects.push_back( object( 1500*scale_x,(1500+150)*scale_x,600*scale_y,(600+30)*scale_y ) );
    m_vec_objects.push_back( object( 840*scale_x,(840+240)*scale_x,780*scale_y,(780+30)*scale_y ) );
    m_vec_objects.push_back( object( 1290*scale_x,(1290+180)*scale_x,780*scale_y,(780+30)*scale_y ) );
    m_vec_objects.push_back( object( 1680*scale_x,(1680+150)*scale_x,780*scale_y,(780+30)*scale_y ) );
    m_vec_objects.push_back( object( 1830*scale_x,(1830+60)*scale_x,750*scale_y,(750+60)*scale_y ) );
    m_vec_objects.push_back( object( 1200*scale_x,(1200+360)*scale_x,420*scale_y,(420+30)*scale_y ) );
    m_vec_objects.push_back( object( 1290*scale_x,(1290+90)*scale_x,390*scale_y,(390+30)*scale_y ) );
    m_vec_objects.push_back( object( 1740*scale_x,(1740+90)*scale_x,420*scale_y,(420+30)*scale_y ) );
    m_vec_objects.push_back( object( 1830*scale_x,(1830+60)*scale_x,390*scale_y,(390+60)*scale_y ) );
    m_vec_objects.push_back( object( 780*scale_x,(780+120)*scale_x,240*scale_y,(240+30)*scale_y ) );
    m_vec_objects.push_back( object( 870*scale_x,(870+30)*scale_x,210*scale_y,(210+30)*scale_y ) );
    m_vec_objects.push_back( object( 1020*scale_x,(1020+120)*scale_x,240*scale_y,(240+30)*scale_y ) );
    m_vec_objects.push_back( object( 1020*scale_x,(1020+30)*scale_x,210*scale_y,(210+30)*scale_y ) );
    m_vec_objects.push_back( object( 1440*scale_x,(1440+240)*scale_x,240*scale_y,(240+30)*scale_y ) );
    m_vec_objects.push_back( object( 1650*scale_x,(1650+30)*scale_x,210*scale_y,(210+30)*scale_y ) );

    /*m_vec_objects.push_back( object( m_screen_size[0]*0.2, m_screen_size[0]*0.3,
                                     m_screen_size[1]*0.85,m_screen_size[1]*0.90 ) );
    m_vec_objects.push_back( object( m_screen_size[0]*0.6, m_screen_size[0]*0.7,
                                     m_screen_size[1]*0.85,m_screen_size[1]*0.90 ) );*/

    /*//walls outside
    m_vec_objects.push_back( object( -10*scale_x,0,0,screen_height*scale_y ) );
    m_vec_objects.push_back( object( screen_width*scale_x,(screen_width+10)*scale_x,0,screen_height*scale_y ) );
    m_vec_objects.push_back( object( 0,screen_width*scale_x,-10*scale_y,0 ) );
    m_vec_objects.push_back( object( 0,screen_width*scale_x,screen_height*scale_y,(screen_height+10)*scale_y ) );*/
    //walls inside
    m_vec_objects.push_back( object( 0,30*scale_x, 0,1080*scale_y ) );
    m_vec_objects.push_back( object( (1920-30)*scale_x,1920*scale_x, 0,1080*scale_y ) );
    m_vec_objects.push_back( object( 30*scale_x,(1920-30)*scale_x, 0,30*scale_y ) );
    m_vec_objects.push_back( object( 30*scale_x,(1920-30)*scale_x, 960*scale_y,1080*scale_y ) );


    //create hat racks
    pos[0]=165*scale_x;
    pos[1]=(780-_rack_height)*scale_y;
    m_vec_racks.push_back( rack(m_tex_rack,pos) );
    pos[0]=585*scale_x;
    pos[1]=(390-_rack_height)*scale_y;
    m_vec_racks.push_back( rack(m_tex_rack,pos) );
    pos[0]=1335*scale_x;
    pos[1]=(390-_rack_height)*scale_y;
    m_vec_racks.push_back( rack(m_tex_rack,pos) );
    pos[0]=1755*scale_x;
    pos[1]=(780-_rack_height)*scale_y;
    m_vec_racks.push_back( rack(m_tex_rack,pos) );

    m_trig_menu=false;
    m_game_state=gs_menu;

    return true;
}

bool game::update()
{
    switch(m_game_state)
    {
        case gs_menu:
        {
            //player input
            bool no_presses=true;
            st_gamepad_data gamepad_data[4];
            for(int pad_i=0;pad_i<4;pad_i++)
            {
                if( m_gamepad[pad_i].IsConnected() )
                 m_active_player[pad_i]=true;
                else m_active_player[pad_i]=false;

                if(m_active_player[pad_i] && (int)m_vec_players.size()>pad_i)
                {
                    //get state
                    gamepad_data[pad_i]=m_gamepad[pad_i].GetState();

                    //reset test
                    if(m_trig_menu)
                    {
                        if(gamepad_data[pad_i].button_A || gamepad_data[pad_i].button_start || m_pKeys[13])
                        {
                            no_presses=false;
                        }
                    }
                    else
                    {
                        if(gamepad_data[pad_i].button_A || gamepad_data[pad_i].button_start || m_pKeys[13])
                        {
                            m_game_state=gs_info;
                            m_trig_menu=true;
                            no_presses=false;
                            break;
                        }
                    }
                }
            }

            //button activation test
            if(no_presses)
            {
                m_trig_menu=false;
            }

        }break;

        case gs_info:
        {
            //player input
            bool no_presses=true;
            st_gamepad_data gamepad_data[4];
            for(int pad_i=0;pad_i<4;pad_i++)
            {
                if( m_gamepad[pad_i].IsConnected() )
                 m_active_player[pad_i]=true;
                else m_active_player[pad_i]=false;

                if(m_active_player[pad_i] && (int)m_vec_players.size()>pad_i)
                {
                    //get state
                    gamepad_data[pad_i]=m_gamepad[pad_i].GetState();

                    //reset test
                    if(m_trig_menu)
                    {
                        if(gamepad_data[pad_i].button_A || gamepad_data[pad_i].button_start || m_pKeys[13])
                        {
                            no_presses=false;
                        }
                    }
                    else
                    {
                        if(gamepad_data[pad_i].button_A || gamepad_data[pad_i].button_start || m_pKeys[13])
                        {
                            m_game_state=gs_ingame;
                            m_trig_menu=true;
                            no_presses=false;
                            break;
                        }
                    }
                }
            }

            //button activation test
            if(no_presses)
            {
                m_trig_menu=false;
            }

        }break;

        case gs_ingame:
        {
            //player input
            bool no_presses=true;
            st_gamepad_data gamepad_data[4];
            for(int pad_i=0;pad_i<4;pad_i++)
            {
                if( m_gamepad[pad_i].IsConnected() )
                 m_active_player[pad_i]=true;
                else m_active_player[pad_i]=false;

                if( m_active_player[pad_i] && (int)m_vec_players.size()>pad_i )
                {
                    //get state
                    gamepad_data[pad_i]=m_gamepad[pad_i].GetState();

                    //reset test
                    if(m_trig_menu)
                    {
                        if(gamepad_data[pad_i].button_start)
                        {
                            no_presses=false;
                        }
                    }
                    else
                    {
                        if(gamepad_data[pad_i].button_start)
                        {
                            m_game_state=gs_info;
                            m_trig_menu=true;
                            no_presses=false;
                            break;
                        }
                    }

                    //button activation test
                    if(no_presses)
                    {
                        m_trig_menu=false;
                    }

                    //move dpad and left thumbstick
                    bool moved=false;
                    int deadzone=15000;
                    float move_sens=20.0;
                    if(gamepad_data[pad_i].dpad_left || gamepad_data[pad_i].thumbstick_left_x<-deadzone)
                    {
                        moved=true;

                        if(m_vec_players[pad_i].m_speed[0]>0) m_vec_players[pad_i].m_speed[0]=0;

                        //max val for dpad, analog for stick
                        if(gamepad_data[pad_i].dpad_left || true)//true=digital movement
                        {
                            m_vec_players[pad_i].m_speed[0]-=move_sens;

                            float movement_ratio=fabs(m_vec_players[pad_i].m_speed[0])/_player_move_speed_max;
                            m_vec_players[pad_i].m_run_prog+=_anim_run_speed*movement_ratio;
                        }
                        else
                        {
                            m_vec_players[pad_i].m_speed[0]+=move_sens*gamepad_data[pad_i].thumbstick_left_x/32768.0;

                            float movement_ratio=fabs(m_vec_players[pad_i].m_speed[0])/_player_move_speed_max;
                            m_vec_players[pad_i].m_run_prog+=_anim_run_speed*fabs(gamepad_data[pad_i].thumbstick_left_x/32768.0)*movement_ratio;
                        }

                        m_vec_players[pad_i].m_face_right=false;


                    }
                    else if(m_vec_players[pad_i].m_speed[0]<-_player_move_speed_max) m_vec_players[pad_i].m_speed[0]+=_player_move_damp*2.0;

                    if(gamepad_data[pad_i].dpad_right || gamepad_data[pad_i].thumbstick_left_x>deadzone)
                    {
                        moved=true;

                        if(m_vec_players[pad_i].m_speed[0]<0) m_vec_players[pad_i].m_speed[0]=0;

                        //max val for dpad, analog for stick
                        if(gamepad_data[pad_i].dpad_right || true)//true=digital movement
                        {
                            m_vec_players[pad_i].m_speed[0]+=move_sens;
                            m_vec_players[pad_i].m_run_prog+=_anim_run_speed;
                        }
                        else
                        {
                            m_vec_players[pad_i].m_speed[0]+=move_sens*gamepad_data[pad_i].thumbstick_left_x/32768.0;
                            m_vec_players[pad_i].m_run_prog+=_anim_run_speed*fabs(gamepad_data[pad_i].thumbstick_left_x/32768.0);
                        }

                        m_vec_players[pad_i].m_face_right=true;
                    }
                    else if(m_vec_players[pad_i].m_speed[0]>_player_move_speed_max) m_vec_players[pad_i].m_speed[0]-=_player_move_damp*2.0;

                    //cap move speed
                    if(m_vec_players[pad_i].m_speed[0]>_player_move_speed_max)
                     m_vec_players[pad_i].m_speed[0]=_player_move_speed_max;
                    if(m_vec_players[pad_i].m_speed[0]<-_player_move_speed_max)
                     m_vec_players[pad_i].m_speed[0]=-_player_move_speed_max;

                    if(!moved)
                    {
                        m_vec_players[pad_i].m_run_prog=0;
                    }


                    //jump
                    if(gamepad_data[pad_i].button_A)
                    {
                        if(!m_vec_players[pad_i].m_trig_jump && m_vec_players[pad_i].m_jump_ready)
                        {
                            m_vec_players[pad_i].m_jump_prolong=true;
                            m_vec_players[pad_i].m_jump_ready=false;
                            m_vec_players[pad_i].m_pos[1]-=10;
                            m_vec_players[pad_i].m_speed[1]=-_jump_force;

                            //play sound
                            m_pSound->playSimpleSound(wav_jump,1.0);
                        }

                        //test for jump prolongation
                        if(m_vec_players[pad_i].m_jump_prolong)
                        {
                            m_vec_players[pad_i].m_speed[1]-=_jump_prolong_force;
                        }

                        m_vec_players[pad_i].m_trig_jump=true;
                    }
                    else
                    {
                        m_vec_players[pad_i].m_trig_jump=false;
                        m_vec_players[pad_i].m_jump_prolong=false;
                    }



                    //hat throw release
                    if(!gamepad_data[pad_i].button_X &&
                        m_vec_players[pad_i].m_trig_key_x &&
                        m_vec_players[pad_i].m_hat_in_hand)
                    {
                        if(m_vec_players[pad_i].m_throw_block)
                        {
                            //triggered due to hat pickup, do not throw hat, reset block
                            m_vec_players[pad_i].m_throw_block=false;
                            m_vec_players[pad_i].m_trig_key_x=false;
                        }
                        else
                        {
                            m_vec_players[pad_i].m_throw_ready=false;

                            //create hat
                            //float hat_speed=m_vec_players[pad_i].m_speed[0]*_hat_throw_force*m_vec_players[pad_i].m_throw_force;
                            //float hat_speed=(m_vec_players[pad_i].m_speed[0]-m_vec_players[pad_i].m_throw_force)*_hat_throw_force;
                            float hat_speed=m_vec_players[pad_i].m_throw_force*_hat_throw_force;
                            //force invert direction
                            if(!m_vec_players[pad_i].m_face_right) hat_speed*=-1.0;
                            /*if( ( m_vec_players[pad_i].m_face_right && m_vec_players[pad_i].m_speed[0]<0) ||
                                (!m_vec_players[pad_i].m_face_right && m_vec_players[pad_i].m_speed[0]>0) )
                            {
                                hat_speed*=-1.0;
                            }*/
                            float hat_pos[2]={m_vec_players[pad_i].m_pos[0],m_vec_players[pad_i].m_pos[1]};
                            if(m_vec_players[pad_i].m_face_right)
                             hat_pos[0]+=_hat_width+_player_width;
                            else
                             hat_pos[0]-=_hat_width+_player_width;

                            if( hat_speed< _hat_speed_min &&  m_vec_players[pad_i].m_face_right ) hat_speed=_hat_speed_min;
                            if( hat_speed>-_hat_speed_min && !m_vec_players[pad_i].m_face_right ) hat_speed=-_hat_speed_min;

                            float speed[2]={hat_speed,m_vec_players[pad_i].m_speed[1]};
                            m_vec_hats.push_back( hat(pad_i,m_tex_hat,hat_pos,speed) );

                            m_vec_players[pad_i].m_throw_delay=0.2;
                            m_vec_players[pad_i].m_hat_in_hand=false;

                            //play sound
                            m_pSound->playSimpleSound(wav_throw,1.0);

                            cout<<m_vec_players[pad_i].m_throw_force<<endl;
                        }

                        //reset throw force
                        m_vec_players[pad_i].m_throw_force=_throw_force_min;
                    }

                    //throw hat load
                    if(gamepad_data[pad_i].button_X && m_vec_players[pad_i].m_hat_in_hand)
                    {
                        m_vec_players[pad_i].m_trig_key_x=true;

                        if(!m_vec_players[pad_i].m_throw_block)
                        {
                            m_vec_players[pad_i].m_throw_force+=_throw_load_speed*_time_step*0.001;
                            if(m_vec_players[pad_i].m_throw_force>1.0) m_vec_players[pad_i].m_throw_force=1.0;

                            //cout<<m_vec_players[pad_i].m_throw_force<<endl;
                        }
                    }
                    else
                    {
                        m_vec_players[pad_i].m_trig_key_x=false;
                    }

                    //rack/hat pickup
                    if(gamepad_data[pad_i].button_X && !m_vec_players[pad_i].m_hat_in_hand)
                    {
                        //near rack
                        if(!m_vec_players[pad_i].m_trig_key_x)
                        {
                            //test if near a rack
                            for(unsigned int rack_i=0;rack_i<m_vec_racks.size();rack_i++)
                            {
                                bool col=
                                m_vec_racks[rack_i].collide_test( m_vec_players[pad_i].m_pos_new[0]-m_vec_players[pad_i].m_size[0],
                                                                  m_vec_players[pad_i].m_pos_new[0]+m_vec_players[pad_i].m_size[0],
                                                                  m_vec_players[pad_i].m_pos_new[1]-m_vec_players[pad_i].m_size[1],
                                                                  m_vec_players[pad_i].m_pos_new[1]+m_vec_players[pad_i].m_size[1] );
                                if(col)
                                {
                                    //test if that hat color is already out
                                    bool block_hat_grab=false;
                                    for(unsigned int hat_i=0;hat_i<m_vec_hats.size();hat_i++)
                                    {
                                        if(m_vec_hats[hat_i].m_color==pad_i)
                                        {
                                            block_hat_grab=true;
                                            break;
                                        }
                                    }
                                    if(block_hat_grab) break;

                                    //test if any hats left
                                    if(m_vec_racks[rack_i].m_hats_left>0)
                                    {
                                        //grab a hat
                                        m_vec_players[pad_i].m_hat_in_hand=true;

                                        //reduce hats on rack
                                        m_vec_racks[rack_i].m_hats_left--;

                                        m_vec_players[pad_i].m_throw_block=true;

                                        //play sound
                                        m_pSound->playSimpleSound(wav_hat_pickup,1.0);

                                        //no more rack test
                                        break;
                                    }
                                }
                            }
                        }

                        //test if near hat on ground
                        if(!m_vec_players[pad_i].m_trig_key_x && !m_vec_players[pad_i].m_hat_in_hand)
                        {
                            //test if near a hat
                            for(unsigned int hat_i=0;hat_i<m_vec_hats.size();hat_i++)
                            {
                                //owner test
                                if(m_vec_hats[hat_i].m_color!=pad_i) continue;

                                bool col=
                                m_vec_hats[hat_i].collide_test( m_vec_players[pad_i].m_pos_new[0]-m_vec_players[pad_i].m_size[0],
                                                                  m_vec_players[pad_i].m_pos_new[0]+m_vec_players[pad_i].m_size[0],
                                                                  m_vec_players[pad_i].m_pos_new[1]-m_vec_players[pad_i].m_size[1],
                                                                  m_vec_players[pad_i].m_pos_new[1]+m_vec_players[pad_i].m_size[1] );
                                if(col)
                                {
                                    //grab hat
                                    m_vec_players[pad_i].m_hat_in_hand=true;

                                    //remove hat from vec
                                    m_vec_hats[hat_i]=m_vec_hats.back();
                                    m_vec_hats.pop_back();
                                    hat_i--;

                                    //alt. keep hat order
                                    //m_vec_hats.erase(m_vec_hats.begin()+hat_i);

                                    m_vec_players[pad_i].m_throw_block=true;

                                    //play sound
                                    m_pSound->playSimpleSound(wav_hat_pickup,1.0);

                                    //no more hat tests
                                    break;
                                }
                            }
                        }

                        m_vec_players[pad_i].m_trig_key_x=true;
                    }

                }
            }

            //update players
            for(unsigned int player_i=0;player_i<m_vec_players.size();player_i++)
            {
                m_vec_players[player_i].update( gamepad_data[player_i].button_A );
            }

            //update hats
            for(unsigned int hat_i=0;hat_i<m_vec_hats.size();hat_i++)
            {
                m_vec_hats[hat_i].update();

                //if near player without hat in hands, set glow
                bool col_detected=false;
                for(unsigned int player_i=0;player_i<m_vec_players.size();player_i++)
                {
                    if(m_vec_players[player_i].m_hat_in_hand || m_vec_hats[hat_i].m_color!=(int)player_i)
                     continue;

                    bool col=
                    m_vec_hats[hat_i].collide_test( m_vec_players[player_i].m_pos_new[0]-m_vec_players[player_i].m_size[0],
                                                  m_vec_players[player_i].m_pos_new[0]+m_vec_players[player_i].m_size[0],
                                                  m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1],
                                                  m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1] );
                    if(col)
                    {
                        col_detected=true;
                        //draw outline
                        m_vec_hats[hat_i].m_highlight=true;
                        break;
                    }
                }

                if(!col_detected)
                {
                    m_vec_hats[hat_i].m_highlight=false;
                }
            }

            //update racks
            for(unsigned int rack_i=0;rack_i<m_vec_racks.size();rack_i++)
            {
                m_vec_racks[rack_i].update();

                //if near player without hat in hands, set glow
                bool col_detected=false;
                for(unsigned int player_i=0;player_i<m_vec_players.size();player_i++)
                {
                    if(m_vec_players[player_i].m_hat_in_hand) continue;

                    bool col=
                    m_vec_racks[rack_i].collide_test( m_vec_players[player_i].m_pos_new[0]-m_vec_players[player_i].m_size[0],
                                                  m_vec_players[player_i].m_pos_new[0]+m_vec_players[player_i].m_size[0],
                                                  m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1],
                                                  m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1] );
                    if(col && m_vec_racks[rack_i].m_hats_left>0)
                    {
                        //test if that hat color is already out
                        bool block_hat_grab=false;
                        for(unsigned int hat_i=0;hat_i<m_vec_hats.size();hat_i++)
                        {
                            if(m_vec_hats[hat_i].m_color==(int)player_i)
                            {
                                block_hat_grab=true;
                                break;
                            }
                        }
                        if(block_hat_grab) break;

                        col_detected=true;
                        //draw outline if hats left
                        m_vec_racks[rack_i].m_highlight=true;
                        break;
                    }
                }

                if(!col_detected)
                {
                    m_vec_racks[rack_i].m_highlight=false;
                }
            }

            //test player col
            for(unsigned int player_i=0;player_i<m_vec_players.size();player_i++)
            {
                m_vec_players[player_i].m_jump_ready=false;

                bool col_detected=false;
                for(unsigned int obj_i=0;obj_i<m_vec_objects.size();obj_i++)
                {
                    bool col=
                    m_vec_objects[obj_i].collide_test( m_vec_players[player_i].m_pos_new[0]-m_vec_players[player_i].m_size[0],
                                                       m_vec_players[player_i].m_pos_new[0]+m_vec_players[player_i].m_size[0],
                                                       m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1],
                                                       m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1] );



                    if(col)
                    {
                        col_detected=true;

                        float move_dist_up=-1;
                        float move_dist_down=-1;
                        float move_dist_left=-1;
                        float move_dist_right=-1;

                        /*//height col test
                        if( m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1]>m_vec_objects[obj_i].m_pos_ll[1] &&
                            m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1]<m_vec_objects[obj_i].m_pos_tl[1] )
                        {
                            //height ok
                            move_dist_left =m_vec_players[player_i].m_pos_new[0]-m_vec_objects[obj_i].m_pos_tr[0]-m_vec_players[player_i].m_size[0];
                            move_dist_right=m_vec_players[player_i].m_pos_new[0]-m_vec_objects[obj_i].m_pos_tl[0]+m_vec_players[player_i].m_size[0];

                            //find shortest dist
                            if( fabs(move_dist_left)<fabs(move_dist_right) )
                            {
                                //move left
                                move_dist_right=0;
                            }
                            else
                            {
                                //move right
                                move_dist_left=0;
                            }

                            //move player
                            m_vec_players[player_i].m_pos_new[0]-=move_dist_left;
                            m_vec_players[player_i].m_pos_new[0]-=move_dist_right;

                            //stop speed
                            m_vec_players[player_i].m_speed[0]=0;
                        }
                        else*/
                        {
                            //height not ok
                            move_dist_down =m_vec_players[player_i].m_pos_new[1]-m_vec_objects[obj_i].m_pos_ll[1]-m_vec_players[player_i].m_size[1];
                            move_dist_up=m_vec_players[player_i].m_pos_new[1]-m_vec_objects[obj_i].m_pos_tl[1]+m_vec_players[player_i].m_size[1];

                            //find shortest dist
                            if( fabs(move_dist_up)<fabs(move_dist_down) )
                            {
                                //move up
                                move_dist_down=0;

                                //m_vec_players[player_i].m_jump_ready=true;

                                //stop speed
                                //if(m_vec_players[player_i].m_speed[1]>0.1) m_vec_players[player_i].m_speed[1]=0.1;
                            }
                            else
                            {
                                //move down
                                move_dist_up=0;

                                //stop speed
                                //if(m_vec_players[player_i].m_speed[1]<0) m_vec_players[player_i].m_speed[1]=0;
                            }

                            //also check sideways
                            move_dist_right =m_vec_players[player_i].m_pos_new[0]-m_vec_objects[obj_i].m_pos_tr[0]-m_vec_players[player_i].m_size[0];
                            move_dist_left  =m_vec_players[player_i].m_pos_new[0]-m_vec_objects[obj_i].m_pos_tl[0]+m_vec_players[player_i].m_size[0];

                            //find shortest dist
                            if( fabs(move_dist_left)<fabs(move_dist_right) )
                            {
                                //move left
                                move_dist_right=0;
                            }
                            else
                            {
                                //move right
                                move_dist_left=0;
                            }



                            //find shortest of up/down and sideways
                            if( fabs(move_dist_right+move_dist_left)<fabs(move_dist_up+move_dist_down) )
                            {
                                move_dist_up=move_dist_down=0;
                            }
                            else
                            {
                                move_dist_right=move_dist_left=0;
                            }

                            //stop speed
                            if(move_dist_right!=0 || move_dist_left!=0)
                            {
                                m_vec_players[player_i].m_speed[0]=0;
                            }
                            else
                            {
                                //stop speed
                                if(m_vec_players[player_i].m_speed[1]>0.1) m_vec_players[player_i].m_speed[1]=0.1;

                                if(fabs(move_dist_up)<fabs(move_dist_down))
                                {
                                    if(m_vec_players[player_i].m_speed[1]<0)
                                    {
                                        m_vec_players[player_i].m_speed[1]=0;
                                        m_vec_players[player_i].m_jump_prolong=false;
                                    }
                                }
                                else
                                {
                                    m_vec_players[player_i].m_jump_ready=true;
                                    m_vec_players[player_i].m_jump_prolong=false;
                                }

                            }

                            //move player
                            m_vec_players[player_i].m_pos_new[0]-=move_dist_right;
                            m_vec_players[player_i].m_pos_new[0]-=move_dist_left;
                            m_vec_players[player_i].m_pos_new[1]-=move_dist_down;
                            m_vec_players[player_i].m_pos_new[1]-=move_dist_up;

                            /*//retest col
                            col=
                            m_vec_objects[obj_i].collide_test( m_vec_players[player_i].m_pos_new[0]-m_vec_players[player_i].m_size[0],
                                                       m_vec_players[player_i].m_pos_new[0]+m_vec_players[player_i].m_size[0],
                                                       m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1],
                                                       m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1] );*/

                            /*if(col)
                            {
                                //also adjust sideways
                                move_dist_right =m_vec_players[player_i].m_pos_new[0]-m_vec_objects[obj_i].m_pos_tr[0]-m_vec_players[player_i].m_size[0];
                                move_dist_left=m_vec_players[player_i].m_pos_new[0]-m_vec_objects[obj_i].m_pos_tl[0]+m_vec_players[player_i].m_size[0];

                                //find shortest dist
                                if( fabs(move_dist_left)<fabs(move_dist_right) )
                                {
                                    //move left
                                    move_dist_right=0;
                                }
                                else
                                {
                                    //move right
                                    move_dist_left=0;
                                }

                                //move player
                                m_vec_players[player_i].m_pos_new[0]-=move_dist_left;
                                m_vec_players[player_i].m_pos_new[0]-=move_dist_right;
                            }*/
                        }


                        /*//side collision (above or below object)
                        if( m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1]>m_vec_objects[obj_i].m_pos_ll[1] &&
                            m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1]<m_vec_objects[obj_i].m_pos_tl[1] )
                        {
                            //no col with head or feet, test sides
                            if(m_vec_players[player_i].m_pos_new[0]-m_vec_players[player_i].m_size[0]<m_vec_objects[obj_i].m_pos_tr[0])
                            {
                                //move to right side
                                //m_vec_players[player_i].m_pos_new[0]=m_vec_objects[obj_i].m_pos_tr[0]+m_vec_players[player_i].m_size[0];

                                //move_dist_x=m_vec_players[player_i].m_pos_new[0]-m_vec_objects[obj_i].m_pos_tr[0]-m_vec_players[player_i].m_size[0];

                                //stop speed
                                //if(m_vec_players[player_i].m_speed[0]<0) m_vec_players[player_i].m_speed[0]=0;
                            }
                            if(m_vec_players[player_i].m_pos_new[0]+m_vec_players[player_i].m_size[0]>m_vec_objects[obj_i].m_pos_tl[0])
                            {
                                //move to left side
                                //m_vec_players[player_i].m_pos_new[0]=m_vec_objects[obj_i].m_pos_tl[0]-m_vec_players[player_i].m_size[0];

                                //move_dist_x=m_vec_players[player_i].m_pos_new[0]-m_vec_objects[obj_i].m_pos_tl[0]+m_vec_players[player_i].m_size[0];

                                //stop speed
                                //if(m_vec_players[player_i].m_speed[0]>0) m_vec_players[player_i].m_speed[0]=0;
                            }

                        }
                        //else
                        {
                            //playerpos-playerheight<objectlowpos AND
                            //playerpos+playerheight<objecttoppos

                                //height col
                            if(m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1]<m_vec_objects[obj_i].m_pos_ll[1] ||
                               m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1]>m_vec_objects[obj_i].m_pos_tl[1])
                            {
                                //cout<<"asd";
                                //move to low side
                                //m_vec_players[player_i].m_pos_new[1]=m_vec_objects[obj_i].m_pos_ll[1]+m_vec_players[player_i].m_size[1];

                                //move_dist_y=m_vec_players[player_i].m_pos_new[1]-m_vec_objects[obj_i].m_pos_ll[1]-m_vec_players[player_i].m_size[1];
                                //move_dist_y=(m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1])-m_vec_objects[obj_i].m_pos_ll[1];

                                //stop speed
                                //if(m_vec_players[player_i].m_speed[1]<0) m_vec_players[player_i].m_speed[1]=0;
                            }
                            if(m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1]>m_vec_objects[obj_i].m_pos_tl[1])
                            {
                                //cout<<"asd";
                                //move to top side
                                //m_vec_players[player_i].m_pos_new[1]=m_vec_objects[obj_i].m_pos_tl[1]-m_vec_players[player_i].m_size[1];

                                //move_dist_y=m_vec_players[player_i].m_pos_new[1]-m_vec_objects[obj_i].m_pos_tl[1]+m_vec_players[player_i].m_size[1];
                                //move_dist_y=m_vec_players[player_i].m_pos_new[1]-m_vec_objects[obj_i].m_pos_tl[1]+m_vec_players[player_i].m_size[1];

                                //stop speed
                                //if(m_vec_players[player_i].m_speed[1]>0) m_vec_players[player_i].m_speed[1]=0;

                                //allow new jump
                                //m_vec_players[player_i].m_jump_ready=true;
                            }


                            //head or feet
                            float dist_top=fabs(m_vec_players[player_i].m_pos_new[1]-m_vec_objects[obj_i].m_pos_tl[1]);
                            float dist_low=fabs(m_vec_players[player_i].m_pos_new[1]-m_vec_objects[obj_i].m_pos_ll[1]);
                            if(dist_low<dist_top)
                            {
                                //head col, move down
                                if(m_vec_players[player_i].m_speed[1]<0) m_vec_players[player_i].m_speed[1]=0;
                            }
                            else
                            {
                                //feet col, move up


                                //move to ground height
                                if( fabs(m_vec_players[player_i].m_pos_new[1]-m_vec_objects[obj_i].m_pos_tl[1])<10.0 )
                                {
                                    m_vec_players[player_i].m_pos_new[1]=m_vec_players[player_i].m_pos[1]=m_vec_objects[obj_i].m_pos_tl[1];
                                }

                                //force push up
                                //m_vec_players[player_i].m_pos[1]-=_player_climb_speed;
                                //m_vec_players[player_i].m_pos_new[1]-=_player_climb_speed;

                                //stop fall
                                m_vec_players[player_i].m_speed[1]=0;

                                //allow new jump
                                m_vec_players[player_i].m_jump_ready=true;
                            }
                        }*/

                        /*if(move_dist_x!=-1 && move_dist_y!=-1)
                        {
                            //find shortest move dist
                            if( fabs(move_dist_x) < fabs(move_dist_y) )
                            {
                                //m_vec_players[player_i].m_pos_new[0]-=move_dist_x;

                                //stop speed
                                //if(move_dist_x>0 && m_vec_players[player_i].m_speed[0]<0) m_vec_players[player_i].m_speed[0]=0;
                                //if(move_dist_x<0 && m_vec_players[player_i].m_speed[0]>0) m_vec_players[player_i].m_speed[0]=0;
                            }
                            else
                            {
                                //m_vec_players[player_i].m_pos_new[1]-=move_dist_y;

                                //stop speed
                                //if(move_dist_y>0 && m_vec_players[player_i].m_speed[1]<0) m_vec_players[player_i].m_speed[1]=0;
                                //if(move_dist_y<0 && m_vec_players[player_i].m_speed[1]>0) m_vec_players[player_i].m_speed[1]=0;

                                if(move_dist_y>0)
                                {
                                    //allow new jump
                                    //m_vec_players[player_i].m_jump_ready=true;
                                }
                            }
                        }
                        else
                        {
                            if(move_dist_y==-1 && move_dist_x!=-1)
                            {
                                //m_vec_players[player_i].m_pos_new[0]-=move_dist_x;

                                //stop speed
                                //if(move_dist_x>0 && m_vec_players[player_i].m_speed[0]<0) m_vec_players[player_i].m_speed[0]=0;
                                //if(move_dist_x<0 && m_vec_players[player_i].m_speed[0]>0) m_vec_players[player_i].m_speed[0]=0;
                            }
                            else if(move_dist_x==-1 && move_dist_y!=-1)
                            {
                                //m_vec_players[player_i].m_pos_new[1]-=move_dist_y;

                                //stop speed
                                //if(move_dist_y>0 && m_vec_players[player_i].m_speed[1]<0) m_vec_players[player_i].m_speed[1]=0;
                                //if(move_dist_y<0 && m_vec_players[player_i].m_speed[1]>0) m_vec_players[player_i].m_speed[1]=0;

                                if(move_dist_y>0)
                                {
                                    //allow new jump
                                    //m_vec_players[player_i].m_jump_ready=true;
                                }
                            }
                        }*/


                    }
                    /*else
                    {
                        //on ground test
                        col=
                        m_vec_objects[obj_i].collide_test( m_vec_players[player_i].m_pos_new[0]-m_vec_players[player_i].m_size[0],
                                                           m_vec_players[player_i].m_pos_new[0]+m_vec_players[player_i].m_size[0],
                                                           m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1],
                                                           m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1] );
                        if(col)
                         m_vec_players[player_i].m_jump_ready=true;
                    }*/
                }

                //slow side movement
                if(m_vec_players[player_i].m_speed[0]>_player_move_speed_min)
                {
                    m_vec_players[player_i].m_speed[0]-=_player_move_damp*0.5;
                }
                else
                if(m_vec_players[player_i].m_speed[0]<-_player_move_speed_min)
                {
                    m_vec_players[player_i].m_speed[0]+=_player_move_damp*0.5;
                }
                else m_vec_players[player_i].m_speed[0]=0;

                //if(!col_detected)//accept new pos
                {
                    m_vec_players[player_i].accept_pos(true);
                }




                //player-player push
                for(unsigned int player_i2=0;player_i2<m_vec_players.size();player_i2++)
                {
                    if(player_i==player_i2) continue;

                    if( !( m_vec_players[player_i].m_pos_new[0]-m_vec_players[player_i].m_size[0] >
                            m_vec_players[player_i2].m_pos_new[0]+m_vec_players[player_i2].m_size[0] ||
                           m_vec_players[player_i].m_pos_new[0]+m_vec_players[player_i].m_size[0] <
                            m_vec_players[player_i2].m_pos_new[0]-m_vec_players[player_i2].m_size[0] ||
                           m_vec_players[player_i].m_pos_new[1]-m_vec_players[player_i].m_size[1] >
                            m_vec_players[player_i2].m_pos_new[1]+m_vec_players[player_i2].m_size[1] ||
                           m_vec_players[player_i].m_pos_new[1]+m_vec_players[player_i].m_size[1] <
                            m_vec_players[player_i2].m_pos_new[1]-m_vec_players[player_i2].m_size[1] ) )
                    {
                        //push players sideways
                        if( m_vec_players[player_i].m_pos_new[0] > m_vec_players[player_i2].m_pos_new[0] )
                        {
                            m_vec_players[player_i].m_pos[0]+=_player_push_force;
                            m_vec_players[player_i].m_pos_new[0]+=_player_push_force;
                            m_vec_players[player_i2].m_pos[0]-=_player_push_force;
                            m_vec_players[player_i2].m_pos_new[0]-=_player_push_force;
                        }
                        else
                        {
                            m_vec_players[player_i].m_pos[0]-=_player_push_force;
                            m_vec_players[player_i].m_pos_new[0]-=_player_push_force;
                            m_vec_players[player_i2].m_pos[0]+=_player_push_force;
                            m_vec_players[player_i2].m_pos_new[0]+=_player_push_force;
                        }

                    }
                }
            }

            //test hat-player col
            for(unsigned int hat_i=0;hat_i<m_vec_hats.size();hat_i++)
            {
                if(m_vec_hats[hat_i].m_idle) continue;

                for(unsigned int player_i=0;player_i<m_vec_players.size();player_i++)
                {
                    float hat_tower_shift=(float)m_vec_players[player_i].m_vec_hats_on.size()*_hat_height;
                    float dist2=(m_vec_players[player_i].m_pos[0]-m_vec_hats[hat_i].m_pos[0])*
                                (m_vec_players[player_i].m_pos[0]-m_vec_hats[hat_i].m_pos[0])+
                                (m_vec_players[player_i].m_pos[1]-_player_height-hat_tower_shift-m_vec_hats[hat_i].m_pos[1])*
                                (m_vec_players[player_i].m_pos[1]-_player_height-hat_tower_shift-m_vec_hats[hat_i].m_pos[1]);
                    if(dist2<_hat_player_dist2)
                    {
                        //add hat on player
                        m_vec_players[player_i].m_vec_hats_on.push_back( m_vec_hats[hat_i].m_color );

                        //remove hat from vec
                        m_vec_hats[hat_i]=m_vec_hats.back();
                        m_vec_hats.pop_back();
                        hat_i--;

                        //alt. keep hat order
                        //m_vec_hats.erase(m_vec_hats.begin()+hat_i);

                        //play sound
                        m_pSound->playSimpleSound(wav_hat_hit,1.0);
                    }
                }
            }

            //hat-object col
            for(unsigned int hat_i=0;hat_i<m_vec_hats.size();hat_i++)
            {
                //ignore idle hat
                if(m_vec_hats[hat_i].m_idle) continue;

                for(unsigned int obj_i=0;obj_i<m_vec_objects.size();obj_i++)
                {
                    bool col=
                    m_vec_objects[obj_i].collide_test( m_vec_hats[hat_i].m_pos[0]-m_vec_hats[hat_i].m_size[0],
                                                       m_vec_hats[hat_i].m_pos[0]+m_vec_hats[hat_i].m_size[0],
                                                       m_vec_hats[hat_i].m_pos[1]-m_vec_hats[hat_i].m_size[1],
                                                       m_vec_hats[hat_i].m_pos[1]+m_vec_hats[hat_i].m_size[1] );
                    if(col)
                    {
                        //if collided with floor, set to idle

                        //adjust pos
                        {
                            float move_dist_up=-1;
                            float move_dist_down=-1;
                            float move_dist_left=-1;
                            float move_dist_right=-1;

                            //height not ok
                            move_dist_down =m_vec_hats[hat_i].m_pos[1]-m_vec_objects[obj_i].m_pos_ll[1]-m_vec_hats[hat_i].m_size[1];
                            move_dist_up   =m_vec_hats[hat_i].m_pos[1]-m_vec_objects[obj_i].m_pos_tl[1]+m_vec_hats[hat_i].m_size[1];

                            //find shortest dist
                            if( fabs(move_dist_up)<fabs(move_dist_down) )
                            {
                                //move up
                                move_dist_down=0;
                            }
                            else
                            {
                                //move down
                                move_dist_up=0;
                            }

                            //also check sideways
                            move_dist_right =m_vec_hats[hat_i].m_pos[0]-m_vec_objects[obj_i].m_pos_tr[0]-m_vec_hats[hat_i].m_size[0];
                            move_dist_left  =m_vec_hats[hat_i].m_pos[0]-m_vec_objects[obj_i].m_pos_tl[0]+m_vec_hats[hat_i].m_size[0];

                            //find shortest dist
                            if( fabs(move_dist_left)<fabs(move_dist_right) )
                            {
                                //move left
                                move_dist_right=0;
                            }
                            else
                            {
                                //move right
                                move_dist_left=0;
                            }



                            //find shortest of up/down and sideways
                            if( fabs(move_dist_right+move_dist_left)<fabs(move_dist_up+move_dist_down) )
                            {
                                move_dist_up=move_dist_down=0;
                            }
                            else
                            {
                                move_dist_right=move_dist_left=0;
                            }

                            //stop speed
                            if(move_dist_right!=0 || move_dist_left!=0)
                            {
                                //swap direction
                                m_vec_hats[hat_i].m_speed[0]*=-0.5;
                                if(fabs(m_vec_hats[hat_i].m_speed[0])<_hat_speed_min_in_air)
                                {
                                    m_vec_hats[hat_i].m_speed[0]=0;
                                }

                                //alt. stop
                                //m_vec_hats[hat_i].m_speed[0]=0;
                            }
                            else
                            {
                                //stop speed
                                if(m_vec_hats[hat_i].m_speed[1]>0.0) m_vec_hats[hat_i].m_speed[1]=0.0;

                                if(fabs(move_dist_up)<fabs(move_dist_down))
                                {
                                    if(m_vec_hats[hat_i].m_speed[1]<0) m_vec_hats[hat_i].m_speed[1]=0;
                                }
                                else
                                {
                                    //hat have touched ground, set to idle
                                    m_vec_hats[hat_i].m_idle=true;
                                }

                            }

                            //move hat
                            m_vec_hats[hat_i].m_pos[0]-=move_dist_right;
                            m_vec_hats[hat_i].m_pos[0]-=move_dist_left;
                            m_vec_hats[hat_i].m_pos[1]-=move_dist_down;
                            m_vec_hats[hat_i].m_pos[1]-=move_dist_up;
                        }
                    }
                }
            }

            //hat-hat col
            for(unsigned int hat_i1=0;hat_i1<m_vec_hats.size();hat_i1++)
            for(unsigned int hat_i2=0;hat_i2<m_vec_hats.size();hat_i2++)
            {
                //ignore idle hat
                //if(m_vec_hats[hat_i2].m_idle || hat_i1==hat_i2) continue;
                if(hat_i1==hat_i2) continue;

                bool col=
                m_vec_hats[hat_i1].collide_test( m_vec_hats[hat_i2].m_pos[0]-m_vec_hats[hat_i2].m_size[0],
                                                 m_vec_hats[hat_i2].m_pos[0]+m_vec_hats[hat_i2].m_size[0],
                                                 m_vec_hats[hat_i2].m_pos[1]-m_vec_hats[hat_i2].m_size[1],
                                                 m_vec_hats[hat_i2].m_pos[1]+m_vec_hats[hat_i2].m_size[1] );
                if(col)
                {
                    //cout<<hat_i1<<endl;

                    //adjust pos
                    {
                        float move_dist_up=-1;
                        float move_dist_down=-1;
                        float move_dist_left=-1;
                        float move_dist_right=-1;

                        //height not ok
                        move_dist_down =(m_vec_hats[hat_i2].m_pos[1]-m_vec_hats[hat_i2].m_size[1]-m_vec_hats[hat_i1].m_size[1])-m_vec_hats[hat_i1].m_pos[1];
                        move_dist_up   =(m_vec_hats[hat_i2].m_pos[1]+m_vec_hats[hat_i2].m_size[1]+m_vec_hats[hat_i1].m_size[1])-m_vec_hats[hat_i1].m_pos[1];
                        //move_dist_down =m_vec_hats[hat_i2].m_pos[1]-(m_vec_hats[hat_i2].m_pos[1]+m_vec_hats[hat_i2].m_size[1])-m_vec_hats[hat_i2].m_size[1];
                        //move_dist_up   =m_vec_hats[hat_i2].m_pos[1]-(m_vec_hats[hat_i2].m_pos[1]-m_vec_hats[hat_i2].m_size[1])+m_vec_hats[hat_i2].m_size[1];
                        //move_dist_down =m_vec_hats[hat_i2].m_pos[1]-m_vec_hats[hat_i2].m_pos_ll[1]-m_vec_hats[hat_i2].m_size[1];
                        //move_dist_up   =m_vec_hats[hat_i2].m_pos[1]-m_vec_hats[hat_i2].m_pos_tl[1]+m_vec_hats[hat_i2].m_size[1];

                        //cout<<move_dist_up<<"\t"<<move_dist_down;

                        //find shortest dist
                        if( fabs(move_dist_up)<fabs(move_dist_down) )
                        {
                            //move up
                            move_dist_down=0;
                        }
                        else
                        {
                            //move down
                            move_dist_up=0;
                        }

                        //also check sideways
                        move_dist_right = (m_vec_hats[hat_i2].m_pos[0]-m_vec_hats[hat_i2].m_size[0]-m_vec_hats[hat_i1].m_size[0])-m_vec_hats[hat_i1].m_pos[0];
                        move_dist_left  = (m_vec_hats[hat_i2].m_pos[0]+m_vec_hats[hat_i2].m_size[0]+m_vec_hats[hat_i1].m_size[0])-m_vec_hats[hat_i1].m_pos[0];
                        //move_dist_right =m_vec_hats[hat_i2].m_pos[0]-m_vec_hats[hat_i2].m_pos_tr[0]-m_vec_hats[hat_i2].m_size[0];
                        //move_dist_left  =m_vec_hats[hat_i2].m_pos[0]-m_vec_hats[hat_i2].m_pos_tl[0]+m_vec_hats[hat_i2].m_size[0];

                        //cout<<"\t\t"<<move_dist_right<<"\t"<<move_dist_left<<endl;;

                        //find shortest dist
                        if( fabs(move_dist_left)<fabs(move_dist_right) )
                        {
                            //move left
                            move_dist_right=0;
                        }
                        else
                        {
                            //move right
                            move_dist_left=0;
                        }



                        //find shortest of up/down and sideways
                        if( fabs(move_dist_right+move_dist_left)<fabs(move_dist_up+move_dist_down) )
                        {
                            move_dist_up=move_dist_down=0;
                        }
                        else
                        {
                            move_dist_right=move_dist_left=0;
                        }

                        //stop speed
                        if(move_dist_right!=0 || move_dist_left!=0)
                        {
                            //swap directions
                            m_vec_hats[hat_i1].m_speed[0]*=-0.5;
                            if(fabs(m_vec_hats[hat_i1].m_speed[0])<_hat_speed_min_in_air)
                            {
                                m_vec_hats[hat_i1].m_speed[0]=0;
                            }
                            m_vec_hats[hat_i2].m_speed[0]*=-0.5;
                            if(fabs(m_vec_hats[hat_i2].m_speed[0])<_hat_speed_min_in_air)
                            {
                                m_vec_hats[hat_i2].m_speed[0]=0;
                            }



                            //alt. stop
                            //m_vec_hats[hat_i1].m_speed[0]=0;
                            //m_vec_hats[hat_i2].m_speed[0]=0;
                        }
                        else
                        {
                            //stop speed
                            if(m_vec_hats[hat_i2].m_speed[1]>0.0) m_vec_hats[hat_i2].m_speed[1]=0.0;
                            //m_vec_hats[hat_i2].m_speed[1]=0.0;
                            //m_vec_hats[hat_i1].m_speed[1]=0;

                            if(fabs(move_dist_up)<fabs(move_dist_down))
                            {
                                if(m_vec_hats[hat_i2].m_speed[1]<0) m_vec_hats[hat_i2].m_speed[1]=0;
                                if(m_vec_hats[hat_i1].m_speed[1]<0) m_vec_hats[hat_i1].m_speed[1]=0;
                            }
                            else
                            {
                                //hat have touched ground, set to idle (not if hat on hat, have to fall down later)
                                //m_vec_hats[hat_i2].m_idle=true;
                            }

                            //reduce x-speed, friction
                            //cout<<m_vec_hats[hat_i2].m_speed[0]<<endl;
                            if(m_vec_hats[hat_i1].m_speed[0]>0)
                            {
                                m_vec_hats[hat_i1].m_speed[0]-=_hat_friction;
                                if(m_vec_hats[hat_i1].m_speed[0]<0) m_vec_hats[hat_i1].m_speed[0]=0;
                            }
                            if(m_vec_hats[hat_i1].m_speed[0]<0)
                            {
                                m_vec_hats[hat_i1].m_speed[0]+=_hat_friction;
                                if(m_vec_hats[hat_i1].m_speed[0]>0) m_vec_hats[hat_i1].m_speed[0]=0;
                            }
                            //same for other hat
                            if(m_vec_hats[hat_i2].m_speed[0]>0)
                            {
                                m_vec_hats[hat_i2].m_speed[0]-=_hat_friction;
                                if(m_vec_hats[hat_i2].m_speed[0]<0) m_vec_hats[hat_i2].m_speed[0]=0;
                            }
                            if(m_vec_hats[hat_i2].m_speed[0]<0)
                            {
                                m_vec_hats[hat_i2].m_speed[0]+=_hat_friction;
                                if(m_vec_hats[hat_i2].m_speed[0]>0) m_vec_hats[hat_i2].m_speed[0]=0;
                            }

                        }

                        //cout<<move_dist_up<<"\t"<<move_dist_down<<"\t\t"<<move_dist_right<<"\t"<<move_dist_left<<endl;;

                        //test if new pos cause col with object
                        bool caused_col=false;
                        for(unsigned int obj_i=0;obj_i<m_vec_objects.size();obj_i++)
                        {
                            bool col2=
                            m_vec_objects[obj_i].collide_test( m_vec_hats[hat_i1].m_pos[0]+(move_dist_right+move_dist_left)-m_vec_hats[hat_i1].m_size[0],
                                                               m_vec_hats[hat_i1].m_pos[0]+(move_dist_right+move_dist_left)+m_vec_hats[hat_i1].m_size[0],
                                                               m_vec_hats[hat_i1].m_pos[1]+(move_dist_down+move_dist_up)-m_vec_hats[hat_i1].m_size[1],
                                                               m_vec_hats[hat_i1].m_pos[1]+(move_dist_down+move_dist_up)+m_vec_hats[hat_i1].m_size[1] );
                            if(col2)
                            {
                                caused_col=true;
                                break;
                            }
                        }
                        if(caused_col)
                        {
                            //cancel movement and stop fallspeed of the other hat
                            if(m_vec_hats[hat_i2].m_speed[1]>0) m_vec_hats[hat_i2].m_speed[1]=0;
                        }
                        else
                        {
                            //move hat
                            m_vec_hats[hat_i1].m_pos[0]+=move_dist_right;
                            m_vec_hats[hat_i1].m_pos[0]+=move_dist_left;
                            m_vec_hats[hat_i1].m_pos[1]+=move_dist_down;
                            m_vec_hats[hat_i1].m_pos[1]+=move_dist_up;
                        }


                    }
                }
            }

        }break;

    }

    return true;
}

bool game::draw()
{
    glPushMatrix();

    //move to center of screen height
    glTranslatef(0,m_screen_height_adjust,0);

    switch(m_game_state)
    {
        case gs_menu:
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,m_tex_menu);
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex2f(0,0);
            glTexCoord2f(0,1);
            glVertex2f(0,m_screen_size[1]);
            glTexCoord2f(1,1);
            glVertex2f(m_screen_size[0],m_screen_size[1]);
            glTexCoord2f(1,0);
            glVertex2f(m_screen_size[0],0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }break;

        case gs_info:
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,m_tex_info);
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex2f(0,0);
            glTexCoord2f(0,1);
            glVertex2f(0,m_screen_size[1]);
            glTexCoord2f(1,1);
            glVertex2f(m_screen_size[0],m_screen_size[1]);
            glTexCoord2f(1,0);
            glVertex2f(m_screen_size[0],0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }break;

        case gs_ingame:
        {
            //background
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,m_tex_level);
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex2f(0,0);
            glTexCoord2f(0,1);
            glVertex2f(0,m_screen_size[1]);
            glTexCoord2f(1,1);
            glVertex2f(m_screen_size[0],m_screen_size[1]);
            glTexCoord2f(1,0);
            glVertex2f(m_screen_size[0],0);
            glEnd();
            glDisable(GL_TEXTURE_2D);

            glPushMatrix();
            glScalef(m_screen_scale[0],m_screen_scale[1],0);

            //objects, debug
            if(m_pKeys[32])
             for(unsigned int obj_i=0;obj_i<m_vec_objects.size();obj_i++)
            {
                m_vec_objects[obj_i].draw();
            }

            //racks
            for(unsigned int rack_i=0;rack_i<m_vec_racks.size();rack_i++)
            {
                m_vec_racks[rack_i].draw();
            }

            //players
            for(unsigned int player_i=0;player_i<m_vec_players.size();player_i++)
            {
                m_vec_players[player_i].draw();
            }

            //hats
            for(unsigned int hat_i=0;hat_i<m_vec_hats.size();hat_i++)
            {
                m_vec_hats[hat_i].draw();
            }

            glPopMatrix();

        }break;
    }

    glPopMatrix();

    return true;
}

bool game::load_texture()
{
    m_tex_player[0]=SOIL_load_OGL_texture
	(
		"data\\texture\\player0.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //| SOIL_FLAG_MIPMAPS
	);

	m_tex_player[1]=SOIL_load_OGL_texture
	(
		"data\\texture\\player1.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //SOIL_FLAG_MIPMAPS
	);

	m_tex_player[2]=SOIL_load_OGL_texture
	(
		"data\\texture\\player2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //SOIL_FLAG_MIPMAPS
	);

	m_tex_player[3]=SOIL_load_OGL_texture
	(
		"data\\texture\\player3.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //SOIL_FLAG_MIPMAPS
	);


	m_tex_hat=SOIL_load_OGL_texture
	(
		"data\\texture\\hat.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //SOIL_FLAG_MIPMAPS
	);

	m_tex_rack=SOIL_load_OGL_texture
	(
		"data\\texture\\rack.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //SOIL_FLAG_MIPMAPS
	);

	m_tex_level=SOIL_load_OGL_texture
	(
		"data\\texture\\level.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //SOIL_FLAG_MIPMAPS
	);

	m_tex_menu=SOIL_load_OGL_texture
	(
		"data\\texture\\menu.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //SOIL_FLAG_MIPMAPS
	);

	m_tex_info=SOIL_load_OGL_texture
	(
		"data\\texture\\info.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_COMPRESS_TO_DXT //SOIL_FLAG_MIPMAPS
	);

	if(m_tex_player[0]==0 ||
       m_tex_player[1]==0 ||
       m_tex_player[2]==0 ||
       m_tex_player[3]==0 ||
       m_tex_hat==0 ||
       m_tex_rack==0 ||
       m_tex_level==0 ||
       m_tex_menu==0 ||
       m_tex_info==0
       )
    {
        cout<<"ERROR: Could not load textures\n";
        return false;
    }

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
}

bool game::load_sound(void)
{
    cout<<"Game: Loading sound\n";

    m_pSound=new sound();

    bool error_flag=false;

    if( !m_pSound->load_WAVE_from_file( wav_jump,"data\\sound\\jump.wav" ) ) error_flag=true;
    if( !m_pSound->load_WAVE_from_file( wav_throw,"data\\sound\\throw.wav" ) ) error_flag=true;
    if( !m_pSound->load_WAVE_from_file( wav_hat_hit,"data\\sound\\hat_hit.wav" ) ) error_flag=true;
    if( !m_pSound->load_WAVE_from_file( wav_hat_pickup,"data\\sound\\hat_pickup.wav" ) ) error_flag=true;


    if( !m_pSound->load_OGG_from_file( ogg_music_main,"data\\sound\\music.ogg" ) ) error_flag=true;

    if(error_flag)
    {
        cout<<"ERROR: Could not load sound\n";
        return false;
    }

    return true;
}

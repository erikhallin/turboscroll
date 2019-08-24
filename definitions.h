#ifndef DEF_H
#define DEF_H

#include <math.h>
#include <stdlib.h>

const float _version=0.4;
const float _time_step=10;

const float _player_gravity=30.0;
const float _hat_gravity=6.0;

const float _player_width=14.0;
const float _player_height=30.0;
const float _player_climb_speed=0.5;
const float _player_push_force=2.0;
const float _player_move_speed_max=300.0;
const float _player_move_damp=20.0;
const float _player_move_speed_min=10.0;
const int   _player_hat_limit=3;

const float _throw_load_speed=1.5;
const float _throw_force_min=0.1;

const float _jump_force=600.0;
const float _jump_prolong_force=5.0;
const float _fall_speed_max=700;

const float _anim_run_speed=0.02;

const float _hat_width=10;
const float _hat_height=7;
const float _hat_throw_force=500.0;
const float _hat_speed_min=50.0;
const float _hat_speed_min_in_air=5.0;
const float _hat_player_dist2=100;
const float _hat_friction=1.00;

const float _rack_width=27;
const float _rack_height=51;

const int   _sound_music_chan=30;

#endif

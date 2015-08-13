#include "mgl_level.h"
#include "mgl_entity.h"
#include "mgl_collision_ent.h"
#include "mgl_audio.h"
#include "mgl_logger.h"
#include "mgl_config.h"
#include "mgl_graphics.h"
#include "mgl_actor.h"
#include <string.h>
#include <SDL.h>

/**
 * @purpose mgl_audio_test is meant to test the audio file resource manager system
 */

void init_all();

MglUint sw = 0,sh = 0;

void mecha_think(void *data,void *context)
{
    MglEntity *ent;
    MglVec2D position;
    MglActor *actor;
    ent = context;
    if (!ent)return;
    position = mgl_entity_get_position(ent);
    actor = mgl_entity_get_actor(ent);
    if (position.x > (32 * 16))
    {
        mgl_entity_set_velocity(ent,mgl_vec2d(-2,0));
        mgl_actor_set_action(
            actor,
            "walk_left"
        );
    }
    if (position.x < (32 * 4))
    {
        mgl_entity_set_velocity(ent,mgl_vec2d(2,0));
        mgl_actor_set_action(
            actor,
            "walk_right"
        );
    }

}

MglEntity *spawn_mech(MglVec2D position,MglLevel *level, MglLine collision, MglLine entity, MglLine tiles)
{
    MglEntity * ent;
    MglActor *actor;
    MglCallback think = mgl_callback(mecha_think,NULL);
    ent = mgl_entity_new();
    if (!ent)return NULL;
    mgl_entity_set_name(ent,"mech");
    actor = mgl_actor_load("../test_data/actors/mecha.actor");
    mgl_actor_set_action(actor,"walk_right");
    mgl_entity_set_actor(ent, actor);
    mgl_entity_set_callbacks(
        ent,
        &think,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
    mgl_entity_make_circle_shape(ent,100, 14,mgl_vec2d(14,14));
    mgl_entity_set_position(ent,position);
    mgl_entity_set_draw_offset(ent,mgl_vec2d(14,14));
    
    mgl_entity_assign_tilemap(ent,mgl_level_get_layer_tilemap_by_name(level,tiles));
    mgl_level_append_draw_item_to_layer(level,entity,ent);
    mgl_entity_add_to_collision_space(ent, mgl_level_get_layer_collision_by_name(level,collision));
    return ent;
}

int main(int argc,char *argv[])
{
    int done = 0,i;
    const Uint8 *keys = NULL;
    MglCamera *cam;
    MglParallax *par;
    MglUint bgw,bgh;
    MglVec2D pos = {0,0};
    MglLevel *level;
    MglCollision * collision;

    if ((argc == 2) && (strcmp(argv[1],"-h")==0))
    {
        fprintf(stdout,"usage:\n");
        fprintf(stdout,"%s [FILES]\n",argv[0]);
        return 0;
    }
    init_all();
    

    mgl_graphics_get_screen_resolution(&sw,&sh);

    cam = mgl_camera_new(mgl_vec2d(sw,sh));
    par = mgl_parallax_load("../test_data/images/testlevel/testbg.def",cam);
    mgl_parallax_get_size(par,&bgw,&bgh);
    
    mgl_logger_info("camera layer size: (%i,%i)",bgw,bgh);
    
    mgl_camera_set_bounds(cam,mgl_rect(0,0,bgw,bgh));
    
    mgl_level_init(5,cam);
    
    /*setup entity layer*/
    level = mgl_level_load("../test_data/maps/testmap.def");
    mgl_entity_register_layer_callbacks(level,"entityLayer");
    mgl_tilemap_add_to_collision("tileLayer","collisionLayer",level);
    collision = mgl_level_get_layer_collision_by_name(level,"collisionLayer");

    for (i = 0; i < 10; i++)
    {
        spawn_mech(mgl_vec2d(32 * (10 + i),32*4 + 16),level,"collisionLayer","entityLayer","tileLayer");
    }
    
    /*mgl_entity_set_velocity(ent,mgl_vec2d(2,0));*/
    
    fprintf(stdout,"mgl_entity_test begin\n");
    while (!done)
    {
        /* system updating */
        mgl_entity_think_all();
        mgl_entity_update_all();
        mgl_entity_pre_physics_all();
        mgl_collision_update(collision);
        mgl_entity_post_physics_all();

        pos = mgl_camera_get_position(cam);
        
        /*drawing*/
        mgl_graphics_clear_screen();
        mgl_level_draw(level);
        
        /*input updating*/
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_RIGHT])
        {
            pos.x += 5;
        }
        if (keys[SDL_SCANCODE_LEFT])
        {
            pos.x -= 5;
        }
        if (keys[SDL_SCANCODE_DOWN])
        {
            pos.y += 5;
        }
        if (keys[SDL_SCANCODE_UP])
        {
            pos.y -= 5;
        }
        mgl_camera_change_position(cam, pos);
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            done = 1;
        }
        mgl_grahics_next_frame();
    }
    fprintf(stdout,"mgl_entity_test end\n");
}

void init_all()
{
    mgl_logger_init();
    mgl_logger_set_stdout_echo(MglTrue);
    mgl_logger_set_threshold(MGL_LOG_ALL & ~MGL_LOG_DEBUG);
    mgl_config_init();
    if (mgl_graphics_init_by_config("../test_data/graphics.def") != 0)
    {
        mgl_logger_info("failed to load graphics, exiting...");
        return;
    }
    mgl_sprite_init_from_config("../test_data/graphics.def");
    mgl_actor_init(
        1000,
        0,
        33
    );
    mgl_font_init(
        10,
        "../test_data/fonts/Exo-Regular.otf",
        16
    );

    mgl_audio_init(
        1000,
        100,
        90,
        25,
        MglTrue,
        MglTrue);
    mgl_camera_init(1);
    mgl_parallax_init(5,NULL);
    mgl_tileset_init(2);
    mgl_tilemap_init(10,MglFalse);
    
    mgl_entity_init(
        1024,
        MglFalse,
        0,
        NULL);
}

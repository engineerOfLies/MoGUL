#include "mgl_particle.h"
#include "mgl_logger.h"
#include "mgl_draw.h"
#include "mgl_resource.h"

static MglVec2D __mgl_particle_scale = {0,0};
static MglResourceManager * __mgl_particle_resource_manager = NULL;
static MglParticleZHandler __mgl_particle_z_strategy = MglParticleZNone;

void mgl_particle_close();
MglBool mgl_particle_load_resource(char *filename,void *data);
void mgl_particle_delete(void *data);

void mgl_particle_init(
    MglUint maxParticles,
    MglParticleZHandler strategy,
    MglVec2D *scale
)
{
    __mgl_particle_resource_manager = mgl_resource_manager_init(
        "mgl particle",
        maxParticles,
        sizeof(MglParticle),
        MglTrue,
        mgl_particle_delete,
        mgl_particle_load_resource
    );
    atexit(mgl_particle_close);
    if (scale)__mgl_particle_scale = *scale;
    __mgl_particle_z_strategy = strategy;
}

void mgl_particle_close()
{
    mgl_resource_manager_free(&__mgl_particle_resource_manager);    
}

MglBool mgl_particle_load_resource(char *filename,void *data)
{
    return MglTrue;
}

void mgl_particle_delete(void *data)
{
    MglParticle * particle;
    if (!data)return;
    particle = (MglParticle*)data;
    mgl_actor_free(&particle->actor);
}


MglParticle * mgl_particle_new(MglUint lifeSpan)
{
    MglParticle * particle;
    particle = (MglParticle *)mgl_resource_new_element(__mgl_particle_resource_manager);
    if (!particle)return NULL;
    particle->lifeSpan = lifeSpan;
    particle->timeToLive = lifeSpan;
    particle->color.w = 255;
    return particle;
}

void mgl_particle_update()
{
    MglParticle *particle,*tmp;
    float factor;
    for (particle = mgl_resource_get_next_data(__mgl_particle_resource_manager,NULL);
         particle != NULL;
         particle = mgl_resource_get_next_data(__mgl_particle_resource_manager,particle))
    {
        particle->timeToLive--;
        if (particle->timeToLive <= 0)
        {
            tmp = particle;
            mgl_resource_free_element(__mgl_particle_resource_manager,(void **)&tmp);
            continue;
        }
        mgl_vec2d_add(particle->position,particle->position,particle->velocity);
        mgl_vec2d_add(particle->velocity,particle->acceleration,particle->velocity);
        if (particle->useColorVec)
        {
            mgl_vec2d_add(particle->color,particle->colorVec,particle->color);
            mgl_color_clamp(particle->color);
        }
        if ((particle->useColorTarget)&&(particle->lifeSpan > 0))
        {
            factor = (particle->timeToLive/(float)particle->lifeSpan);
            particle->color.x = (particle->color.x * factor) + (particle->colorTarget.x * (1 - factor) );
            particle->color.y = (particle->color.x * factor) + (particle->colorTarget.y * (1 - factor) );
            particle->color.z = (particle->color.x * factor) + (particle->colorTarget.z * (1 - factor) );
            particle->color.w = (particle->color.x * factor) + (particle->colorTarget.w * (1 - factor) );
        }
        if (particle->actor)
        {
            mgl_actor_next_frame(particle->actor);
        }
    }
}

void mgl_particle_draw()
{
    MglParticle *particle;
    MglVec2D position = {0,0};
    for (particle = mgl_resource_get_next_data(__mgl_particle_resource_manager,NULL);
         particle != NULL;
         particle = mgl_resource_get_next_data(__mgl_particle_resource_manager,particle))
    {
        switch(__mgl_particle_z_strategy)
        {
            default:
            case MglParticleZNone:
                position.x = particle->position.x;
                position.y = particle->position.y;
                break;
            case MglParticleZScale:
                position.x = particle->position.x + (__mgl_particle_scale.x * particle->position.z);
                position.y = particle->position.y + (__mgl_particle_scale.y * particle->position.z);
                break;
            case MglParticleZMatrix:
                /*TODO*/
                position.x = particle->position.x;
                position.y = particle->position.y;
                break;
        }
        if (particle->actor)
        {
            mgl_actor_draw(
                particle->actor,
                position,
                0,
                NULL,
                NULL,
                &particle->color
            );
        }
        else
        {
            mgl_draw_pixel(position,particle->color);
        }
    }
}

void mgl_particle_spray(MglVec2D position,MglVec2D direction,MglFloat spread,MglUint count,MglUint life, MglVec4D color, MglFloat colorSpread)
{
    int i;
    MglParticle *particle;
    for (i = 0;i < count;i++)
    {
        particle = mgl_particle_new(life);
        if (!particle)return;
        mgl_vec2d_copy(particle->position,position);
        mgl_vec2d_set(
            particle->velocity,
            direction.x + mgl_crandom() * spread,
            direction.y + mgl_crandom() * spread
        );
        mgl_vec4d_set(
            particle->color,
            color.x + mgl_crandom()*colorSpread,
            color.y + mgl_crandom()*colorSpread,
            color.z + mgl_crandom()*colorSpread,
            color.w
        );
        mgl_color_clamp(particle->color);
    }
}

/*eol@eof*/

#include "raylib.h" 

struct Sprite_Pack {
    Texture2D tex;
    Rectangle rect;
}

struct Sprite_Pool {
    Sprite_Pack explosion;
    Sprite_Pack thrust;
    Sprite_Pack bullet;
}

void init_sprite_pool() {
    Sprite_Pool pool;

    pool.explosion.tex = LoadTexture("data/anims/explosion.png");
    pool.explosion.rect = {0.0f, 0.0f, (float) explosion_texture.width / 11, (float) explosion_texture.height };
        
    pool.thrust.tex = LoadTexture("data/anims/exhaust.png");
    pool.thrust.rect = {0.0f, 0.0f, (float) thrust_texture.width / 4, (float) thrust_texture.height };

    pool.bullet.tex = LoadTexture("data/anims/shoot.png");
    pool.bullet.rect = {0.0f, 0.0f, (float) bullet_texture.width / 10, (float) bullet_texture.height };

    return pool;
}

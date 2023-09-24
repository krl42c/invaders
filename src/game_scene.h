#pragma once

#include <vector>
#include <random>
#include <stdio.h> 
#include <algorithm>
#include "raylib.h"
#include "types.h" 

struct Bullet {
    i32 x;
    i32 y;
    u8 speed = 5;
    u8 damage = 5;

    bool is_alive = false;
};

struct Entity {
    Texture2D raylib_texture;
    Texture2D bullet_texture;
    const char* bullet_texture_path = "data/PNG_Animations/Shots/Shot1/shot1_3.png";

    i32 x;
    i32 y;

    u8 hp = 80;
    u8 speed = 5;
    u8 shoot_speed = 30;
    u16 score = 0;
    u8 damage;

    std::vector<Bullet> bullets;
    void shoot();
};

enum Item_Type {
   SPEED,
   SHOOT_SPEED,
   DAMAGE
};

enum DIRECTION : uint8_t {
    RIGHT = 0,
    LEFT
};

struct Item {
    Texture2D raylib_texture;
    Item_Type type;
    int x;
    int y;
};

struct Game_Scene {
    Entity player;
    std::vector<Entity> enemies;
    std::vector<Bullet> enemy_bullets;
    bool playing = true;
};

struct TexturePool {
    Texture2D bullet;
    Texture2D enemy_ship;
};

int rand_range(i32 left, i32 right);
void update_bullet(Entity* entity, DIRECTION direction);

Entity init_player(const char* texture_path, i32 x, i32 y);
Entity create_enemy(const int y);

void update_player(Entity *player, int *tick, std::vector<Entity> enemies, std::vector<Bullet>* enemy_bullets);
void update_enemy(Entity *enemy, double dt, std::vector<Bullet>* bullets);

void destroy_bullet(Bullet *bullet);
void destroy_enemy(Entity *enemy);

Item create_item(Item_Type type);
void destroy_item(Item *item);

void update_all_enemies(Game_Scene *game_scene, double dt);
bool check_for_collisions(Entity *ship, std::vector<Bullet> *bullets);

void init_bullets(Game_Scene* game);
Bullet* grab_bullet(std::vector<Bullet> *enemy_bullets);

inline bool aabb_colliding(i32 x, i32 y, i32 x1, i32 y1, i32 off_x, i32 off_y);

void update_enemy_bullets(std::vector<Bullet>* bullets);

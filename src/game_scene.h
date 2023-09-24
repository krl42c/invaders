#pragma once

#include <vector>
#include <random>
#include <stdio.h> 
#include <algorithm>
#include "raylib.h"

struct Bullet {
    int x;
    int y;
    int speed = 5;
    int damage = 5;

    bool is_alive = false;
};

struct Entity {
    Texture2D raylib_texture;
    Texture2D bullet_texture;
    const char* bullet_texture_path = "data/PNG_Animations/Shots/Shot1/shot1_3.png";

    int x;
    int y;

    int hp = 80;
    int speed = 5;
    int shoot_speed = 30;
    int score = 0;
    int damage;

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

int rand_range(int left, int right);
void update_bullet(Entity* entity, DIRECTION direction);

Entity init_player(const char* texture_path, int x, int y);
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

inline bool aabb_colliding(int x, int y, int x1, int y1, int off_x, int off_y);


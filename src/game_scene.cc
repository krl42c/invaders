#include "game_scene.h"

Entity init_player(const char* texture_path, int x, int y) {
    Entity p = {}; 
    p.raylib_texture = LoadTexture(texture_path);
    p.x = x;
    p.y = y;
    return p;
}

void update_player(Entity *player, int *tick, std::vector<Entity> enemies) {
    if (IsKeyDown(KEY_W)) {
        player->y -= player->speed;  
    }

    if (IsKeyDown(KEY_S)) {
        player->y += player->speed; 
    }

    if (IsKeyPressed(KEY_SPACE)) {
        if (*tick == 0) {
            player->shoot();
            *tick = *tick + 1;
        }
    }
    
    update_bullet(player, RIGHT);
    
    // @TODO: Check for enemy collisions
    //
    for (auto enemy : enemies) {
        bool has_collided = check_for_collisions(player, &enemy.bullets);
        if (has_collided) {
            player->hp -= 5;
        }
    }

}

// target_x: object to check collision on
// object_x: object that is colliding with target
inline bool is_in_range_with_offset(int target_x, int object_x, int offset) {
    if (target_x == object_x) return true;
    
}

int rand_range(int left, int right) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(left, right); // define the range
    return distr(gen);
}

void update_enemy(Entity *enemy, int *tick) {
    enemy->x -= enemy->speed;    
    int random = rand_range(0, 500);

    if (random < 20) {
        enemy->shoot();
    } 

    int bullet_index = 0;
    for (auto& bullet : enemy->bullets) {
        if (bullet.x < 0) {
            enemy->bullets.erase(enemy->bullets.begin() + bullet_index);
        }
        bullet.x -= 20;
        bullet_index++;
    }
}

void update_all_enemies(Game_Scene* game_scene, int *tick) {
    int enemy_index = 0;
    for (auto& enemy : game_scene->enemies) {
        update_enemy(&enemy, tick);
        bool has_collided = check_for_collisions(&enemy, &game_scene->player.bullets);
        if (has_collided) {
            if (enemy.hp <= 0) {
                game_scene->enemies.erase(game_scene->enemies.begin() + enemy_index);
            }
        }
        enemy_index++;
    }
}

void Entity::shoot() {
    //Bullet b = { LoadTexture(this->bullet_texture_path), this->x + raylib_texture.width - 10, this->y + (raylib_texture.height / 3)
    //};
    
    Bullet b;
    b.x = this->x + this->raylib_texture.width;
    b.y = this->y + (this->raylib_texture.height / 3);

    bullets.push_back(b);
}

void update_bullet(Entity* entity, DIRECTION direction) {
    int index = 0;
    for (auto& bullet : entity->bullets) {
        if(bullet.x < 850 && bullet.x > -20) {
            if (direction == RIGHT)
                bullet.x += bullet.speed;
            else
                bullet.x -= bullet.speed;
        } else {
            entity->bullets.erase(entity->bullets.begin() + index);
        }
        index++;
    }
}

Entity create_enemy(const int y) {
    Entity p = {};
    p.x = 800;
    p.y = y;
    return p;
}

void destroy_enemy(Entity *enemy) {
    UnloadTexture(enemy->raylib_texture);
}


bool check_for_collisions(Entity *ship, std::vector<Bullet> *bullets) {
    int bullet_index = 0;
    for (auto bullet : *bullets) {
        if (bullet.x > (ship->x + 5)) {
            if (bullet.y < (ship->y + 20) && bullet.y > (ship->y - 20)) {
                ship->hp -= bullet.damage;
                if (ship-> hp <= 10) {
                    bullets->erase(bullets->begin() + bullet_index);
                }
                return true;
            }
        }
        bullet_index++;
    }
    return false;
}

// @TODO: Items
Item create_item(Item_Type type, int x, int y) {
    Item item;
    item.x = x;
    item.y = y;

    switch(type) {
        case SPEED:
            item.raylib_texture = LoadTexture("data/PNG/Items/speed.png");
            break;
        case SHOOT_SPEED:
            item.raylib_texture = LoadTexture("data/PNG/Items/shoot_speed.png");
            break;
        case DAMAGE:
            item.raylib_texture = LoadTexture("data/PNG/Items/damage.png");
            break;
    }

    return item;
}

void apply_item(Entity *player, Item *item) {
     switch(item->type) {
        case SPEED:
            player->speed += 20;
            break;
        case SHOOT_SPEED:
            player->shoot_speed += 20;
            break;
        case DAMAGE:
            player->damage += 5;
            break;
    }
}


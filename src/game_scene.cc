#include "game_scene.h"

Entity init_player(const char* texture_path, int x, int y) {
    Entity p = {}; 
    p.raylib_texture = LoadTexture(texture_path);
    p.x = x;
    p.y = y;
    return p;
}

void update_player(Entity *player, int *tick, std::vector<Entity> enemies, std::vector<Bullet>* enemy_bullets) {
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

    for (auto& bullet : *enemy_bullets) {
        if (bullet.is_alive) {
            if (aabb_colliding(bullet.x, bullet.y, player->x, player->y, 5, 90)) {
                 player->hp -= bullet.damage; 
                bullet.is_alive = false;
            }
        }
    }

}

int rand_range(int left, int right) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(left, right); // define the range
    return distr(gen);
}

void update_enemy(Entity *enemy, double dt, std::vector<Bullet>* bullets) {
    enemy->x -= enemy->speed;    
    int random = rand_range(0, 700);

    if (random <= 10) {
        Bullet* b = grab_bullet(bullets);
        if (b != nullptr) {
            b->is_alive = true;
            b->x = enemy->x;
            b->y = enemy->y + 50;
        }
    }
    // @TODO: bullets die whenever a ship dies, unattach the update function
    for (auto& bullet : *bullets) {
        if (bullet.is_alive) {
            bullet.x -= 10;
            if (bullet.x < 0) {
                bullet.is_alive = false;
            }
        }
    }
}

void update_all_enemies(Game_Scene* game_scene, double dt) {
    int enemy_index = 0;
    for (auto& enemy : game_scene->enemies) {
        update_enemy(&enemy, dt, &game_scene->enemy_bullets);
        bool has_collided = check_for_collisions(&enemy, &game_scene->player.bullets);
        if ((has_collided && enemy.hp <= 0) || enemy.x < 0) {
            //game_scene->enemies.erase(game_scene->enemies.begin() + enemy_index);
        }
        enemy_index++;
    }
}

void Entity::shoot() {
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

void init_bullets(Game_Scene* game) {
    size_t total_bullets = 30;
    for (size_t i = 0; i < total_bullets; i++) {
        game->enemy_bullets.push_back( Bullet {0,0} );
    }
}

Bullet* grab_bullet(std::vector<Bullet>* enemy_bullets) {
    for (auto& bullet : *enemy_bullets) {
        if (!bullet.is_alive)
            return &bullet;
    }

    return nullptr;
}


bool check_for_collisions(Entity *ship, std::vector<Bullet> *bullets) {
    int bullet_index = 0;
    for (auto bullet : *bullets) {
        if (aabb_colliding(bullet.x, bullet.y, ship->x, ship->y, 3, 90)) {
            //ship->hp -= bullet.damage; @TODO
            ship->hp = 0;
            bullets->erase(bullets->begin() + bullet_index);
            return true;
        }
        bullet_index++;
    }
    return false;
}

inline bool aabb_colliding(int x, int y, int x1, int y1, int off_x, int off_y) {
    return ( (x >= x1 - off_x) && (x <= x1 + off_x) && (y <= y1 + off_y) && (y > y1 + 5) );
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


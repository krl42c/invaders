#include "raylib.h"
#include "game_scene.h"
#include "config.h"
#include "types.h" 
#include <vector>
#include <iostream>
#include <assert.h>
#include <random>
#include <string>

#define WIDTH 800
#define HEIGHT 600

int random_position(int left, int right) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(left, right); // define the range
    return distr(gen);
}

enum Scene {
    START_SCREEN,
    PLAYING,
    PAUSE,
    GAME_OVER
};

int TARGET_FRAMERATE = 60;

int player_hp_to_rect_width(u8 hp) {
    u8 ret = 1;
    for (u8 i = 1; i < hp; i++) 
        ret = ret + 20;
    return ret;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "invaders");
    SetTargetFPS(TARGET_FRAMERATE);

    Texture2D background_image = LoadTexture("data/static/Space_Stars2.png");

    Game_Scene game;
    Config conf = read_config("config.ini"); 

    
    game.player = init_player("data/static/Ship2.png", 50, 50);
    init_bullets(&game);

    int shoot_timer = 0;
    int enemy_shoot_timer = 0;
    int enemy_spawn_timer = 0;

    int enemy_spawn_ticks = 100;

    Scene game_scene = START_SCREEN;
    
    TexturePool texture_pool;
    texture_pool.bullet = LoadTexture("data/static/shot1_3.png");
    texture_pool.enemy_ship = LoadTexture("data/static/Ship4.png");
    
    Texture2D explosion_texture = LoadTexture("data/anims/explosion.png");
    Rectangle frame_rect = {0.0f, 0.0f, (float) explosion_texture.width / 11, (float) explosion_texture.height };
        
    Texture2D thrust_texture = LoadTexture("data/anims/exhaust.png");
    Rectangle thrust_rect = {0.0f, 0.0f, (float) thrust_texture.width / 4, (float) thrust_texture.height };

    Texture2D bullet_texture = LoadTexture("data/anims/shoot.png");
    Rectangle bullet_rect = {0.0f, 0.0f, (float) bullet_texture.width / 10, (float) bullet_texture.height };

    double t = 0.0;
    double dt = 1.0 / (double) TARGET_FRAMERATE;
    
    int current_frame = 0;
    int frame_counter = 0;
    int frame_speed = 10; 


    for (auto prop : conf.properties) {
        if (prop.name == "player.speed") {
            game.player.speed = stoi(prop.value);
        }
        if (prop.name == "player.shoot_speed") {
            game.player.shoot_speed = stoi(prop.value);
        }

        if (prop.name == "player.damage") {
            game.player.damage = stoi(prop.value); 
        }

        if (prop.name == "player.hp") {
            game.player.hp = stoi(prop.value);
        }
        
        if (prop.name == "enemies.spawn_rate") {
            enemy_spawn_ticks = stoi(prop.value);
        }
    }

    Vector2 explosion_position;
    bool play_explosion = false;

    u8 menu_option = 0;
    while (!WindowShouldClose() && game.playing) {
        if (game_scene == START_SCREEN) {

            if (IsKeyDown(KEY_SPACE)) {
                switch (menu_option) {
                    case 0:
                        game_scene = PLAYING;
                        break;
                    case 1:
                        break;
                    case 2:
                        game.playing = false;
                        break;
                }
            }

            if (IsKeyPressed(KEY_S)) {
                if (menu_option < 2) {
                    menu_option++;
                } else {
                    menu_option = 0;
                }
            }

            if (IsKeyPressed(KEY_W)) {
                if (menu_option > 0) {
                    menu_option--;
                } else {
                    menu_option = 2;
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            for(int j = 0; j < HEIGHT; j+= background_image.height) {
                for(int i = 0; i < WIDTH; i += background_image.width)
                    DrawTexture(background_image, i, j,WHITE);
            }
            
            DrawText("Play", 50, 50, 40, menu_option == 0 ? WHITE : MAROON);
            DrawText("Options", 50, 100, 40, menu_option == 1 ? WHITE : MAROON);
            DrawText("Quit", 50, 150, 40, menu_option == 2 ? WHITE : MAROON);
            EndDrawing();

        }

        if (game_scene == GAME_OVER) {
            if (IsKeyPressed(KEY_SPACE)) game_scene = START_SCREEN; // @TODO: Reload game state

            BeginDrawing();
            ClearBackground(RAYWHITE);
            for(int j = 0; j < HEIGHT; j+= background_image.height) {
                for(int i = 0; i < WIDTH; i += background_image.width)
                    DrawTexture(background_image, i, j,WHITE);
            }
            DrawText("GAME OVER", 50, 50, 40, MAROON);
            EndDrawing();
        }

        if (game_scene == PLAYING || game_scene == PAUSE) {

            if (game_scene == PAUSE) {
                if (IsKeyReleased(KEY_SPACE)) {
                    game_scene = PLAYING;
                }
            }
            if (game_scene == PLAYING) {
                frame_counter++;

                if (game.player.hp <= 0) game_scene = GAME_OVER;
                if (IsKeyDown(KEY_P)) game_scene = PAUSE;

                update_player(&game.player, &shoot_timer, game.enemies, &game.enemy_bullets);

                if (shoot_timer != 0)
                    shoot_timer++;

                if (enemy_shoot_timer != 0)
                    enemy_shoot_timer++;

                if (shoot_timer == game.player.shoot_speed)
                    shoot_timer = 0; 

                if (enemy_shoot_timer == 10)
                    enemy_shoot_timer = 0;

                if (enemy_spawn_timer == 0) {
                    game.enemies.push_back(create_enemy(random_position(10, 550)));   
                }
                
                enemy_spawn_timer++;
                if (enemy_spawn_timer == enemy_spawn_ticks) {
                    enemy_spawn_timer = 0;
                }
                
                update_all_enemies(&game, t);
                update_enemy_bullets(&game.enemy_bullets);
                
                size_t enemy_index = 0;
                for (auto enemy : game.enemies) {
                    if (enemy.hp <= 0 || enemy.x < -10) {
                        current_frame = 0;
                        frame_counter = 0;
                        explosion_position = { (float)enemy.x - 40, (float)enemy.y - 60 };
                        game.enemies.erase(game.enemies.begin() + enemy_index); 
                        play_explosion = true;
                        enemy_index++;
                    }
                }

                if (frame_counter >= (60 / frame_speed)) {
                    frame_counter = 0;
                    current_frame++;

                    if (current_frame > 5) current_frame = 0;
                    frame_rect.x = (float) current_frame * (float) explosion_texture.width / 11;
                    thrust_rect.x = (float) current_frame * (float) thrust_texture.width / 4;
                    bullet_rect.x = (float) current_frame * (float) bullet_rect.width / 10;
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            for(int j = 0; j < HEIGHT; j+= background_image.height) {
                for(int i = 0; i < WIDTH; i += background_image.width)
                    DrawTexture(background_image, i, j,WHITE);
            }

            for (auto bullet : game.player.bullets) {
                Vector2 pos = { (float) bullet.x, (float) bullet.y };
                DrawTextureRec(bullet_texture, bullet_rect, pos, WHITE);
            }
            
            for (auto enemy : game.enemies) {
                DrawTexture(texture_pool.enemy_ship, enemy.x, enemy.y, WHITE);
            }

            if (play_explosion) {
                DrawTextureRec(explosion_texture, frame_rect, explosion_position, WHITE);
                if (current_frame == 5) play_explosion = false;
            }

            for (auto& bullet : game.enemy_bullets) {
                if (bullet.is_alive) {
                    Vector2 pos = { (float) bullet.x, (float) bullet.y };
                    DrawTextureRec(bullet_texture, bullet_rect, pos, WHITE);
                }
            }

            DrawTexture(game.player.raylib_texture, game.player.x, game.player.y, WHITE);

            DrawRectangle(game.player.x + 30,game.player.y + 90,player_hp_to_rect_width(game.player.hp),10,RED);
            DrawRectangleLines(game.player.x + 30,game.player.y + 90, 80 ,10, WHITE);

            Vector2 thrust_pos = { (float) game.player.x - 10, (float) game.player.y + 60 };
            DrawTextureRec(thrust_texture, thrust_rect, thrust_pos, WHITE);
            //DrawText(std::to_string(game.player.score).c_str(), 10, 10, 20, MAROON);      
                                                                         
            if (game_scene == PAUSE) {
                DrawText("PRESS SPACE TO RESUME", 50, 50, 40, MAROON);       
            }

            EndDrawing();
        }
        t += dt;
    }

    CloseWindow();

    return 0;
}

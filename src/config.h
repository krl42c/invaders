#pragma once

#include <fstream>
#include <stdint.h>
#include <map>
#include <string>
#include "game_scene.h"

enum Property_Type {
    INTEGER,
    FLOAT
};

struct Property {
    std::string name; 
    std::string value;

    Property_Type type;
};

const std::vector<std::string> available_options = {
    "player.speed",
    "player.shoot_speed", 
    "player.damage",
    "player.hp",
    "enemies.spawn_rate"
};

struct Config {
    std::vector<Property> properties;    
    std::map<std::string, std::string> property_map;
};


Config read_config(const char* file_path);
void apply_config(Config *config, Game_Scene *game);

Property construct_property(std::string line);

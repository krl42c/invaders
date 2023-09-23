#include "config.h"

Config read_config(const char* file_path) {
    Config config;

    std::ifstream file(file_path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            auto property = construct_property(line);
            config.property_map.insert({std::string(property.name), std::string(property.value)});
            config.properties.push_back(property);
        }
    }
    file.close();
    return config;
}


Property construct_property(std::string line) {
    Property property;
    std::string name;
    std::string value;
    bool is_right = false; 
    for(char c : line) {
        if (c != ' ') {
            if (is_right) {
                value += c;
            } else {
                name += c;
            }
        }
        if (c == ' ') is_right = true;    
    }
        
    if (!name.empty() && !value.empty())  { 
        property.name = name;
        property.value = value;
    }
    return property;
}

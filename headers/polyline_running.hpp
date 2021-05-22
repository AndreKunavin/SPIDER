#pragma once

#include "SFML/Graphics.hpp"
#include "geom.hpp"
#include "defines.h"

class Pauchok {
public:
    
    Pauchok (std::vector<Point> &path_) {
        position = path_[0];
        velocity = (path_[1] - path_[0]).normalize() * VELOCITY_ABS;
        target = path_[1];
        target_id = 1;
        for (auto &it : path_) {
            path.push_back(it);
        }
    }

    std::vector<Point> path;
    Point position;
    Point target;
    std::size_t target_id;
    Vector velocity;

    void update(float dt) {
        Vector rel_pos = target - position;
        if (rel_pos.len() < dt * velocity.len()) {
            if (target_id == path.size() - 1) {
                return;
            } else {
                target_id++;
                target = path[target_id];
                velocity = (target - position).normalize() * VELOCITY_ABS;
            }
        } 
        position = position + velocity * dt;
    }
};

void draw(Pauchok &P, sf::RenderWindow& window) {
    sf::Texture pict;
    pict.loadFromFile("textures/pauchok");
    sf::Vector2f pos(P.position.x, P.position.y);
    sf::Sprite spider;
    spider.setTexture(pict);
    spider.setTextureRect(sf::IntRect(0, 0, SPIDER_RAD * 2, 27));

    sf::Vector2f shift(SPIDER_RAD, 27/2);
    spider.setPosition(pos - shift);

    sf::VertexArray track;
    track.setPrimitiveType(sf::LinesStrip);
    
    for (auto &it : P.path) {
        sf::Vertex vertex;
        vertex.position.x = it.x;
        vertex.position.y = it.y;
        track.append(vertex);
    }
    window.draw(track);
    window.draw(spider);
}
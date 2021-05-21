#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "geom2.hpp"

#define VELOCITY_ABS 200
#define WINDOW_WIDTH 600
#define WINDOW_HIGHT 600
#define SPIDER_RAD 30

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
        std::cout << target_id << std::endl;
        position = position + velocity * dt;
    }
};

void draw(Pauchok &P, sf::RenderWindow& window) {
    sf::Texture pict;
    pict.loadFromFile("pauchok");
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

int main() {

    std::vector<Point> path;

    /*for (auto &it: path) {
        std::cout << it.x << ' ' << it.y << std::endl;
    }*/

    Point dep_point(100, 100);
    path.push_back(dep_point);
    for (std::size_t i = 0; i < 10; i++) {
        Point rand;
        path.push_back(rand.rand(WINDOW_HIGHT));
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HIGHT), "PAUCHOK");
    window.setFramerateLimit(60);

    sf::Texture texture;
    if (!texture.loadFromFile("gradient")) {
        std::cout << "Can not load a texture" << std::endl;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    Pauchok P(path);

    sf::Clock clock;
    while(window.isOpen()) {
        sf::Event event;
        
        sf::Time time = clock.getElapsedTime();
        clock.restart();
        float dt = time.asSeconds();
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {}
        }
        window.clear();
        window.draw(sprite);
        
        P.update(dt);
        draw(P, window);
        window.display();
    }
}
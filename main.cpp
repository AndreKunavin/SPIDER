#include "headers/geom.hpp"
#include "headers/graph_process.h"
#include "SFML/Graphics.hpp"
#include "headers/defines.h"
#include "headers/polyline_running.hpp"


void make_walls(std::vector<Point> &site) {
    
    for (std::size_t i = 0; i <= (WINDOW_WIDTH - 2 * WALL_DANGER) / SMALL_DIST; i++) {
        
        Point celling(WALL_DANGER + i * SMALL_DIST, WINDOW_HIGHT - WALL_DANGER);
        site.push_back(celling);

        Point floor(WINDOW_WIDTH - WALL_DANGER - i * SMALL_DIST, WALL_DANGER);
        site.push_back(floor);
    }
    for (std::size_t i = 0; i <= ((WINDOW_HIGHT - 2 * WALL_DANGER) / SMALL_DIST); i++) {
        
        Point left_wall(WALL_DANGER, WALL_DANGER + i * SMALL_DIST);
        site.push_back(left_wall);

        Point right_wall(WINDOW_WIDTH - WALL_DANGER, WINDOW_HIGHT - WALL_DANGER - i * SMALL_DIST);
        site.push_back(right_wall);
    }
}

void transfer(std::vector<Polygon> &from, std::vector<Point> &to) {
    for (auto &pol : from) {
        for (auto &pnt : pol) {
            to.push_back(pnt);
        }
    }
}




int main() {
    std::vector<Point> sites;

    /********testing**********/
    
    /*std::vector<Polygon> obstacles;
    for (std::size_t j = 0; j < 4; j++) {
        Polygon test;
        for (std::size_t i = 0; i < 10; i++) {
            Point rand;
            Point p = rand.rand(SQUARE);
            test.push_back(p);
            std::cout << p.x << ' ' << p.y << std::endl;
        }
        std::cout << std::endl;
    obstacles.push_back(test);
    }
    transfer(obstacles, sites);
   
    /*************************/

    make_walls(sites);

    for (std::size_t i = 0; i < S_SIZE; i++) {
        Point rand;
        sites.push_back(rand.rand(SQUARE));
    }

    Voronoi V(sites);
    //V.out();

    Point start(50, 50);
    Point finish(550, 550);
    Graph net;
    net.build(V.diagram, SMALL_DIST);
    net.add_start_end(start, finish);

    
    /*for (auto &it : sites) {
        std::cout << it.x << ' ' << it.y << std::endl;
    }
    for (auto &it : vi) {
        std::cout << it.x << ' ' << it.y << std::endl;
    }*/
    std::vector<Point> path = net.build_way();

    /*for (auto &it: path) {
        std::cout << it.x << ' ' << it.y << std::endl;
    }*/


    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HIGHT), "PAUCHOK");
    window.setFramerateLimit(60);

    sf::Texture texture;
    if (!texture.loadFromFile("textures/gradient")) {
        std::cout << "Can not load a texture" << std::endl;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    Pauchok P(path);

    std::vector<sf::CircleShape> balls(sites.size());
    for (std::size_t i = 0; i < sites.size(); i++) {
        sf::Vector2f new_pos;
        new_pos.x = sites[i].x;
        new_pos.y = sites[i].y;
        balls[i].setPosition(new_pos);
        balls[i].setRadius(3);
        balls[i].setFillColor(sf::Color::Red);
    }

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
        for (auto &it : balls) {
            window.draw(it);
        }
        P.update(dt);
        draw(P, window);
        window.display();
    }
}
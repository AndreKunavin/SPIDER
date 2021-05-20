#include "geom2.hpp"
#include "Voronoi.hpp"

#define S_SIZE 500


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
    std::size_t walls_pnt_num = sites.size();

    for (std::size_t i = 0; i < S_SIZE; i++) {
        Point rand;
        sites.push_back(rand.rand(SQUARE));
    }
    
    Voronoi V(sites, walls_pnt_num);
    V.out();

    for (auto &it : sites) {
        std::cout << it.x << ' ' << it.y << std::endl;
    }
}
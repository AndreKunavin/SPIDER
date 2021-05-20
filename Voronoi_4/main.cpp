#include "geom2.hpp"
#include "Voronoi.hpp"

#define S_SIZE 20
#define WINDOW_WIDTH 600
#define WINDOW_HIGHT 600
#define SPIDER_RAD 10.0f
#define SMALL_DIST 49
#define WALL_DANGER 15

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

int main() {
    std::vector<Point> site;

    make_walls(site);
    std::size_t walls_pnt_num = site.size();
    
    for (std::size_t i = 0; i < S_SIZE; i++) {
        Point rand;
        site.push_back(rand.rand(SQUARE));
    }
    
    Voronoi V(site, walls_pnt_num);
    V.out();

    for (auto &it : site) {
        std::cout << it.x << ' ' << it.y << std::endl;
    }
}
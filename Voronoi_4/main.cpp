#include "geom2.hpp"
#include "Voronoi.hpp"

#define S_SIZE 100

int main() {
    std::vector<Point> site;

    for (std::size_t i = 0; i < S_SIZE; i++) {
        Point rand;
        site.push_back(rand.rand(SQUARE));
    }
    Voronoi V(site);
    V.out();
}
#include "geom.hpp"

#define S_SIZE 10
#define SQUARE 100.0f

class Cell {
public:
    Cell(Point &site_, Polygon &convex_) {
        site = site_;
        for (auto& it : convex_) {
            convex.push_back(it);
        }
    }
    Point site;
    Polygon convex;
};

class Voronoi {
public:
    Voronoi(std::vector<Point> &site) {
        plot_diagram(site);
    }
    std::vector<Cell> diagram;

private:   
    void plot_diagram(std::vector<Point> &site) {

        for (std::size_t i = 0; i < site.size(); i++) {
            Polygon cell;
            plot_cell(i, site, cell);
        }

    }
    void plot_cell(std::size_t i, std::vector<Point> &site, Polygon &cell) {
        
        std::vector<Point> perps_intersection;
        std::vector<Line> perps;
        Polygon convex;
        
        for (std::size_t j = 0; j < site.size(); j++) {
            if (i != j) {
                Segment edge(site[i], site[j]);
                perps.push_back(edge.midpoint_perpendicular());
            }
        }

        for (std::size_t k = 0; k < perps.size(); k++) {
            for (std::size_t j = k; j < perps.size(); j++) {
                if (k != j) {
                    Point p = get_intersection(perps[k], perps[j]);
                    perps_intersection.push_back(p);
                }
            }
        }

        for (std::size_t j = 0; j < perps_intersection.size(); j++) {
            if (belong_to_cell(site[i], j, perps_intersection, perps)) {
                convex.push_back(perps_intersection[j]);
            }
        }

        Cell c(site[i], convex);
        diagram.push_back(c);
    }
    std::size_t belong_to_cell(Point site, std::size_t intersection_number, std::vector<Point> &perps_intersection, std::vector<Line> &perps) {
        
        return 1;
    }
};

int main() {
    std::vector<Point> site;
    
    for (std::size_t i = 0; i < S_SIZE; i++) {
        Point rand;
        site.push_back(rand.rand(SQUARE));
    }
    Voronoi V(site);
    
    std::cout << "Sites:" << std::endl;
    for (std::size_t i = 0; i < S_SIZE; i++) {
    //    std::cout << site[i].x << " " << site[i].y << std::endl;

    }

    std::cout << "Vertexes:" << std::endl;
    for (std::size_t i = 0; i < 20; i++) {
    //    std::cout << V.perps_intersection[i].x << " " << V.perps_intersection[i].y << std::endl;

    }
}
#include "geom2.hpp"
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

    void out() {
        for (std::size_t i = 0; i < diagram.size(); i++) {
            std::cout <<  "cell " << i << " (site " << diagram[i].site.x << ' ' << diagram[i].site.y << ")" << std::endl;
            for (auto& v : diagram[i].convex) {
                std::cout << v.x << ' ' << v.y << std::endl;
            }
            std::cout << std::endl;
        }       
    }

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
            //std::cout << perps_intersection[j].x << ' ' << perps_intersection[j].y << std::endl;
            if (belong_to_cell(site[i], perps_intersection[j], perps)) {
                convex.push_back(perps_intersection[j]);
            }
        }

        Cell c(site[i], convex);
        diagram.push_back(c);
    }

    std::size_t belong_to_cell(Point site, Point vertex, std::vector<Line> &perps) {
        std::size_t value = 0;
        //std::cout << site.x << ' ' << site.y << std::endl;
        for (std::size_t k = 0; k < perps.size(); k++) {
            std::size_t a = intersection(perps[k], site, vertex);
            if (a == 0) {
                return 0;
            }
            value += a;
        }
        //std::cout << value << std::endl;
        //std::cout << std::endl;
        
        if (value == perps.size()) {
            return 1;
        } else {
            return 0;
        }
    }

};
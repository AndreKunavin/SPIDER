#include "geom2.hpp"
#define SQUARE 600.0f

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
    Voronoi(std::vector<Point> &site, std::size_t walls_pnt_num_) {
        walls_pnt_num = walls_pnt_num_;
        plot_diagram(site);
    }
    std::vector<Cell> diagram;

    void out() {
        for (std::size_t i = 0; i < diagram.size(); i++) {
            //std::cout <<  "cell " << i << " (site " << diagram[i].site.x << ' ' << diagram[i].site.y << ")" << std::endl;
            for (auto& v : diagram[i].convex) {
                std::cout << v.x << ' ' << v.y << std::endl;
            }
            std::cout << std::endl;
        }       
    }
    std::size_t walls_pnt_num;

private:   
    void plot_diagram(std::vector<Point> &site) {
        
        for (std::size_t i = walls_pnt_num; i < site.size(); i++) {
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
                    if ((p.x != INF) && (p.y != INF)) {
                        perps_intersection.push_back(p);
                    }
                }
            }
        }

        for (std::size_t j = 0; j < perps_intersection.size(); j++) {
            if (belong_to_cell(site[i], perps_intersection[j], perps)) {
                convex.push_back(perps_intersection[j]);
            }
        }
        make_order(convex);
        Cell c(site[i], convex);
        diagram.push_back(c);
    }

    std::size_t belong_to_cell(Point site, Point vertex, std::vector<Line> &perps) {
        std::size_t value = 0;

        for (std::size_t k = 0; k < perps.size(); k++) {
            std::size_t a = intersection(perps[k], site, vertex);
            if (a == 0) {
                return 0;
            }
            value += a;
        }
        
        if (value == perps.size()) {
            return 1;
        } else {
            return 0;
        }
    }

    void make_order(Polygon &convex) {
        std::size_t size = convex.size();
        if (size > 3) {
            for (std::size_t i = 0; i < size; i++) {
                for (std::size_t j = i + 1; j < size; j++) {
                    Line check_line(convex[i], convex[j]);
                    std::vector<std::size_t> attitudes;
                    for (std::size_t k = 0; k < size; k++) {
                        if ((k != i) && (k != j)) {
                            attitudes.push_back(line_point_att(check_line, convex[k]));
                        }
                    }
                    std::size_t flag = 0;
                    std::size_t test = attitudes[0];
                    for (auto &it : attitudes) {
                        if (it != test) {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1) continue;
                    if (j != (i + 1)) {
                        swap(convex, i + 1, j);
                    }
                }
            }
        }
    }

    void swap(std::vector<Point> &vertex, std::size_t a, std::size_t b) {
        Point c = vertex[a];
        vertex[a] = vertex[b];
        vertex[b] = c;
    }


};
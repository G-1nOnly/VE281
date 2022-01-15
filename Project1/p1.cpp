#include <iostream>
#include <vector>
#include "sort.hpp"

struct point
{
    const struct point *origin = nullptr;
    int x, y;
    point() : x(0), y(0) {}
    point(int a, int b) : x(a), y(b) {}
    void set_origin(struct point &p0) { origin = &p0; }
};

inline int ccw(struct point p1, struct point p2, struct point p3)
{
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

bool check_xy(const struct point &a, const struct point &b)
{
    if (a.y == b.y)
        return a.x < b.x;
    return a.y < b.y;
}

bool check_dist(const struct point &a, const struct point &b)
{
    int dist_a = (a.x - a.origin->x) * (a.x - a.origin->x) + (a.y - a.origin->y) * (a.y - a.origin->y);
    int dist_b = (b.x - b.origin->x) * (b.x - b.origin->x) + (b.y - b.origin->y) * (b.y - b.origin->y);
    return dist_a < dist_b;
}

bool compare(struct point &a, struct point &b)
{
    if (ccw(*a.origin, a, b) == 0)
        return check_dist(a, b);
    return (ccw(*a.origin, a, b) > 0);
}

int main()
{
    int n;
    std::cin >> n;
    if (n == 0)
        return 0;
    std::vector<struct point> points(n);

    for (point &p : points)
    {
        int a, b;
        std::cin >> a >> b;
        struct point temp(a, b);
        p = temp;
    }
    merge_sort(points, check_xy);
    point p0 = points[0];
    for (point &p : points)
        p.set_origin(p0);
    merge_sort(points, compare);
    std::vector<struct point> convex;
    convex.push_back(p0);
    for (point &p : points)
    {
        if (!((p.x == p0.x) && (p.y == p0.y)))
        {
            while ((convex.size() > 1) && (ccw(convex[convex.size() - 2], convex[convex.size() - 1], p) <= 0))
                convex.pop_back();
            convex.push_back(p);
        }
    }

    for (point convex_p : convex)
        std::cout << convex_p.x << ' ' << convex_p.y << std::endl;

    return 0;
}
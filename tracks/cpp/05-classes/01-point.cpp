#include <iostream>

class Point {
   public:
    Point(int x, int y) : x_(x), y_(y) {}

    int squared_distance_from_origin() const {
        return x_ * x_ + y_ * y_;
    }

   private:
    int x_;
    int y_;
};

int main() {
    const Point point(3, 4);
    std::cout << point.squared_distance_from_origin() << '\n';
}

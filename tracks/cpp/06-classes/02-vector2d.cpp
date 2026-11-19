#include <iostream>

class Vector2D {
   public:
    Vector2D(double x, double y) : x_(x), y_(y) {}

    Vector2D operator+(const Vector2D &other) const {
        return Vector2D(x_ + other.x_, y_ + other.y_);
    }

    void print() const {
        std::cout << x_ << ' ' << y_ << '\n';
    }

   private:
    double x_;
    double y_;
};

int main() {
    (Vector2D(1.5, 2.0) + Vector2D(2.5, 3.0)).print();
}

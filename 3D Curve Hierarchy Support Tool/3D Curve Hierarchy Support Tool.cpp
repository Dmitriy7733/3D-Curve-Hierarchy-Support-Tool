#include <iostream>
#include <vector>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

class Point3D {
public:
    double x, y, z;
    Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

class Curve3D {
public:
    virtual Point3D getPoint(double t) const = 0;
    virtual Point3D getDerivative(double t) const = 0;
    virtual ~Curve3D() {}
};

class Circle : public Curve3D {
private:
    double radius;
public:
    Circle(double _radius) : radius(_radius) {}

    Point3D getPoint(double t) const override {
        double x = radius * cos(t);
        double y = radius * sin(t);
        return Point3D(x, y, 0.0);
    }

    Point3D getDerivative(double t) const override {
        double dx_dt = -radius * sin(t);
        double dy_dt = radius * cos(t);
        return Point3D(dx_dt, dy_dt, 0.0);
    }
};

class Ellipse : public Curve3D {
private:
    double a, b;
public:
    Ellipse(double _a, double _b) : a(_a), b(_b) {}

    Point3D getPoint(double t) const override {
        double x = a * cos(t);
        double y = b * sin(t);
        return Point3D(x, y, 0.0);
    }

    Point3D getDerivative(double t) const override {
        double dx_dt = -a * sin(t);
        double dy_dt = b * cos(t);
        return Point3D(dx_dt, dy_dt, 0.0);
    }
};

class Spiral3D : public Curve3D {
private:
    double a, step;
public:
    Spiral3D(double _a, double _step) : a(_a), step(_step) {}

    Point3D getPoint(double t) const override {
        double x = a * t * cos(t);
        double y = a * t * sin(t);
        double z = t * step;
        return Point3D(x, y, z);
    }

    Point3D getDerivative(double t) const override {
        double dx_dt = a * (cos(t) - t * sin(t));
        double dy_dt = a * (sin(t) + t * cos(t));
        double dz_dt = step;
        return Point3D(dx_dt, dy_dt, dz_dt);
    }
};

int main() {
    std::vector<Curve3D*> curves;
    std::vector<Circle*> circles;

    // Заполнение первого контейнера случайными кривыми
    std::default_random_engine generator;
    std::uniform_real_distribution<double> radius_distribution(1.0, 10.0);
    std::uniform_real_distribution<double> a_distribution(1.0, 10.0);
    std::uniform_real_distribution<double> step_distribution(0.1, 1.0);

    for (int i = 0; i < 10; ++i) {
        double radius = radius_distribution(generator);
        double a = a_distribution(generator);
        double step = step_distribution(generator);

        if (i % 3 == 0) {
            curves.push_back(new Circle(radius));
        }
        else if (i % 3 == 1) {
            curves.push_back(new Ellipse(radius, radius * 0.5));
        }
        else 
            curves.push_back(new Spiral3D(radius, step));
        }
    // Вывод координат точек и производных всех кривых при t=PI/4
#define M_PI 3.14159265358979323846
    double t = M_PI / 4.0;
    for (const auto& curve : curves) {
        Point3D point = curve->getPoint(t);
        Point3D derivative = curve->getDerivative(t);
        std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << "), ";
        std::cout << "Derivative: (" << derivative.x << ", " << derivative.y << ", " << derivative.z << ")\n";
    }

    // Заполнение второго контейнера только окружностями
    for (Curve3D* curve : curves) {
        if (dynamic_cast<Circle*>(curve)) {
            circles.push_back(static_cast<Circle*>(curve));
        }
    }

    // Сортировка второго контейнера по возрастанию радиусов
    std::sort(circles.begin(), circles.end(), [t](const Circle* a, const Circle* b) {
        return a->getPoint(t).x < b->getPoint(t).x;
        });

    //Вычисление общей суммы радиусов всех окружностей
    double sumOfRadii = 0.0;
    for (const Circle* circle : circles) {
        sumOfRadii += circle->getPoint(t).x;
    }

    std::cout << "Sum of radii in the second container: " << sumOfRadii << "\n";

    // Освобождение памяти
    for (Curve3D* curve : curves) {
        delete curve;
    }

    return 0;
}
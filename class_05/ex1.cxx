class Point {
  float coord_x, coord_y;
public:
  Point() : coord_x{0}, coord_y{0} {}
  Point(float x, float y = 0) : coord_x{x}, coord_y{y} {}
};

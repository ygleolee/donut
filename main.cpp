#include "core.h"
#include "shapes.h"

int main() {
  cuboid();
  cube();
  ellipsoid();
  donut();
  ch4();
  donut(12, 30, 0.08, 0.02, 0.04, {0, 1, -1}, {15, 15, 0});
  sphere();
}


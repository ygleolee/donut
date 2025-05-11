#include "core.h"
#include "shapes.h"

int main() {
  donut();
  ch4();
  ellipsoid();
  cuboid();
  donut(15, 32, 0.08, 0.02, 0.04, {0, 1, -1}, {6, 8, 0});
  cube();
  sphere();
}


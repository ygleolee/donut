#include "./ascii_art.h"
#include "./shapes.h"

// void donut(double r1=12, double r2=30, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, 1, -1}, vec disp = {0, 0, 0}, long long iter=1000) {

int main() {
  cube();
  donut();
  ch4();
  donut(12, 30, 0.08, 0.02, 0.04, {0, 1, -1}, {15, 15, 0});
  sphere();
}


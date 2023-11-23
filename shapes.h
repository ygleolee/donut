void donut(double r1=12, double r2=30, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {0, 0, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  double cp, sp, ct, st;
  for (double phi=0; phi<6.28; phi+=0.015) {
    cp=cos(phi), sp=sin(phi);
    for (double the=0; the<6.28; the+=0.04) {
      ct=cos(the), st=sin(the);
      pts.push_back(app({ {cp, 0, -sp}, {0, 1, 0}, {sp, 0, cp} }, { r2+r1*ct, r1*st, 0 }));
      nor.push_back(app({ {cp, 0, -sp}, {0, 1, 0}, {sp, 0, cp} }, { ct, st, 0 }));
    }
  }
  for (int i=0; i<pts.size(); ++i) pts[i] = add(pts[i], disp);
  draw(pts, nor, dX, dY, dZ, light_dir, iter);
}

void cube(double len=50, double dX=0.08, double dY=0.04, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {0, 0, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  for (double i=-len/2; i<=len/2; i+=0.5) {
    for (double j=-len/2; j<=len/2; j+=0.5) {
      pts.push_back({ len/2, i, j}); nor.push_back({ 1, 0, 0});
      pts.push_back({-len/2, i, j}); nor.push_back({-1, 0, 0});
      pts.push_back({i,  len/2, j}); nor.push_back({0,  1, 0});
      pts.push_back({i, -len/2, j}); nor.push_back({0, -1, 0});
      pts.push_back({i, j,  len/2}); nor.push_back({0, 0,  1});
      pts.push_back({i, j, -len/2}); nor.push_back({0, 0, -1});
    }
  }
  draw(pts, nor, dX, dY, dZ, light_dir, iter);
}

void cuboid(double a=60, double b=45, double c=30, double dX=0.05, double dY=0.02, double dZ=0.03, vec light_dir = {0, -1, -1}, vec disp = {0, 0, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  double inc=0.5;
  for (double i=-a/2; i<=a/2; i+=inc) {
    for (double j=-b/2; j<=b/2; j+=inc) {
      pts.push_back({i, j,  c/2}); nor.push_back({0, 0,  1});
      pts.push_back({i, j, -c/2}); nor.push_back({0, 0, -1});
    }
  }
  for (double i=-b/2; i<=b/2; i+=inc) {
    for (double j=-c/2; j<=c/2; j+=inc) {
      pts.push_back({ a/2, i, j}); nor.push_back({ 1, 0, 0});
      pts.push_back({-a/2, i, j}); nor.push_back({-1, 0, 0});
    }
  }
  for (double i=-a/2; i<=a/2; i+=inc) {
    for (double j=-c/2; j<=c/2; j+=inc) {
      pts.push_back({i,  b/2, j}); nor.push_back({0,  1, 0});
      pts.push_back({i, -b/2, j}); nor.push_back({0, -1, 0});
    }
  }
  draw(pts, nor, dX, dY, dZ, light_dir, iter);
}

void sphere(double r=20, double dX=0.08, double dY=0.04, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {15, 15, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  for (double the=0; the<6.28; the+=0.05) {
    double ct=cos(the), st=sin(the);
    for (double phi=0; phi<3.14; phi+=0.05) {
      double cp=cos(phi), sp=sin(phi);
      pts.push_back({r*ct*sp, r*st*sp, r*cp});
      nor.push_back({r*ct*sp, r*st*sp, r*cp});
    }
  }
  for (int i=0; i<pts.size(); ++i) pts[i] = add(pts[i], disp);
  draw(pts, nor, dX, dY, dZ, light_dir, iter);
}

void ellipsoid(double a=25, double b=32, double c=30, double l=50, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {0, 0, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  for (double the=0; the<6.28; the+=0.02) {
    double ct=cos(the), st=sin(the);
    for (double phi=0; phi<3.14; phi+=0.02) {
      double cp=cos(phi), sp=sin(phi);
      pts.push_back({ct*sp*a, st*sp*b, cp*c});
      nor.push_back({ct*sp/a, st*sp/b, cp/c});
    }
  }
  for (int i=0; i<pts.size(); ++i) pts[i] = add(pts[i], disp);
  draw(pts, nor, dX, dY, dZ, light_dir, iter);
}

void ch4(double r1=16, double r2=6, double l=40, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {2, 0, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  double co=cos(20.5*3.1415926/180), si=sin(19.5*3.1415926/180);
  vec dis[4] = { {0, l, 0}, {co*l, -si*l, 0}, {-co*l/2, -si*l, co*l*sqrt(3)/2}, {-co*l/2, -si*l, -co*l*sqrt(3)/2} };
  for (double the=0; the<6.28; the+=0.02) {
    double ct=cos(the), st=sin(the);
    for (double phi=0; phi<3.14; phi+=0.02) {
      double cp=cos(phi), sp=sin(phi), x=r2*ct*sp, y=r2*st*sp, z=r2*cp;
      pts.push_back({r1*ct*sp, r1*st*sp, r1*cp});
      nor.push_back({r1*ct*sp, r1*st*sp, r1*cp});
      for (int i=0; i<4; ++i) {
        pts.push_back({x+dis[i][0], y+dis[i][1], z+dis[i][2]});
        nor.push_back({x, y, z});
      }
    }
  }
  for (int i=0; i<pts.size(); ++i) pts[i] = add(pts[i], disp);
  draw(pts, nor, dX, dY, dZ, light_dir, iter);
}

void cone(double r=20, double h=40, double l=50, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, 1}, vec disp = {0, 0, 0}, long long iter=10000) {
}
void droplet(double r=20, double h=40, double l=50, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, 1}, vec disp = {0, 0, 0}, long long iter=10000) {
}
void pill(double r=20, double h=40, double l=50, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, 1}, vec disp = {0, 0, 0}, long long iter=10000) {
}



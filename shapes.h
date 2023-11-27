void donut(double r1=12, double r2=30, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {0, 0, 0}) {
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
  draw(pts, nor, dX, dY, dZ, light_dir);
}

void cube(double len=70, double dX=0.08, double dY=0.04, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {0, 0, 0}) {
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
  draw(pts, nor, dX, dY, dZ, light_dir);
}

void cuboid(double a=60, double b=45, double c=30, double dX=0.05, double dY=0.02, double dZ=0.03, vec light_dir = {0, -1, -1}, vec disp = {0, 0, 0}) {
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
  draw(pts, nor, dX, dY, dZ, light_dir);
}

void sphere(double r=20, double dX=0.08, double dY=0.04, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {15, 15, 0}) {
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
  draw(pts, nor, dX, dY, dZ, light_dir);
}

void ellipsoid(double a=25, double b=32, double c=30, double l=50, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, -1}, vec disp = {0, 0, 0}) {
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
  draw(pts, nor, dX, dY, dZ, light_dir);
}

void ch4(double r1=16, double r2=7, double r3=1.2, double l=45, double dX=0.08, double dY=0.03, double dZ=0.05, vec light_dir = {0, -1, -1}, vec disp = {1, 0, 0}) {
  vector<vec> pts, nor;
  double co=cos(109.5*3.1415926/180), si=sin(109.5*3.14151926/180), ct, st, cp, sp, x, y, z;
  mat rot1 = { {co, -si, 0}, {si, co, 0}, {0, 0, 1} }, rot2 = { { (-1.0)/2, 0, -sqrt(3)/2}, {0, 1, 0}, {sqrt(3)/2, 0, (-1.0)/2} };
  vec v={0, l, 0}, v1=app(rot1, v), v2=app(rot2, v1), v3=app(rot2, v2), n, n1, n2, n3;
  vec dis[4] = {v, v1, v2, v3};
  for (double the=0; the<6.28; the+=0.02) {
    ct=cos(the), st=sin(the);
    for (double phi=0; phi<3.14; phi+=0.02) {
      cp=cos(phi), sp=sin(phi), x=r2*ct*sp, y=r2*st*sp, z=r2*cp;
      pts.push_back({r1*ct*sp, r1*st*sp, r1*cp});
      nor.push_back({r1*ct*sp, r1*st*sp, r1*cp});
      for (int i=0; i<4; ++i) {
        pts.push_back({x+dis[i][0], y+dis[i][1], z+dis[i][2]});
        nor.push_back({x, y, z});
      }
    }
  }
  for (double the=0; the<6.28; the+=0.02) {
    co=cos(the), si=sin(the);
    for (double h=r1; h<=l-r2; h+=0.3) {
      v={r3*co, h, r3*si};
      n={r3*co, 0, r3*si};
      v1=app(rot1, v);
      n1=app(rot1, n);
      v2=app(rot2, v1);
      n2=app(rot2, n1);
      v3=app(rot2, v2);
      n3=app(rot2, n2);
      pts.push_back(v);
      pts.push_back(v1);
      pts.push_back(v2);
      pts.push_back(v3);
      nor.push_back(n);
      nor.push_back(n1);
      nor.push_back(n2);
      nor.push_back(n3);
    }
  }
  for (int i=0; i<pts.size(); ++i) pts[i] = add(pts[i], disp);
  draw(pts, nor, dX, dY, dZ, light_dir);
}

void cone(double r=20, double h=40, double l=50, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, 1}, vec disp = {0, 0, 0}) {
}
void droplet(double r=20, double h=40, double l=50, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, 1}, vec disp = {0, 0, 0}) {
}
void pill(double r=20, double h=40, double l=50, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, 1}, vec disp = {0, 0, 0}) {
}



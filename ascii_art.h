#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

#define cout if(0) cerr
#define inrange(x, y) (x>=0&&x<wid&&y>=0&&y<hei)

typedef vector<vector<double>> mat;
typedef vector<double> vec;

mat mul(mat A, mat B) {
  int n=A.size();
  mat ans(n, vector<double>(n, 0));
  for (int i=0; i<n; ++i)
    for (int j=0; j<n; ++j)
      for (int k=0; k<n; ++k)
        ans[i][j]+=A[i][k]*B[k][j];
  return ans;
}

vec app(mat A, vec x) {
  int n=x.size();
  vec ans(n, 0);
  for (int i=0; i<n; ++i)
    for (int j=0; j<n; ++j)
      ans[i]+=A[i][j]*x[j];
  return ans;
}

vec add(vec x, vec y) {
  return { x[0]+y[0], x[1]+y[1], x[2]+y[2] };
}

double cosang(vec x, vec y) {
  return (x[0]*y[0] + x[1]*y[1] + x[2]*y[2]) / sqrt((x[0]*x[0] + x[1]*x[1] + x[2]*x[2]) * (y[0]*y[0] + y[1]*y[1] + y[2]*y[2]));
}

void draw(vector<vec>& pts, vector<vec>& nors, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, 1, -1}, long long iter=10000) {
  printf("\x1b[2J");
  double r1=9, r2=24, X=0, Y=0, Z=0, cX, cY, cZ, sX, sY, sZ, bri;
  double usr=0, scr=80, obj=160; // user at z=0, screen at z=80, object at z=160
  int wid=160, hei=80; // screen is 80*80, wid 160 for scaling
  vec pt, nor, cen={static_cast<double>(wid/2), static_cast<double>(hei/2), 50};
  mat Rx, Ry, Rz, rot;
  vector<vector<char>> pix(wid, vector<char>(hei));
  mat dep(wid, vector<double>(hei));
  string s=".........,,,,,,:>+r=csu?3V[Y0Q@";
  for (int cnt=0; cnt<iter; ++cnt, X+=dX, Y+=dY, Z+=dZ) {
    cX=cos(X), sX=sin(X), cY=cos(Y), sY=sin(Y), cZ=cos(Z), sZ=sin(Z);
    Rx = { {1, 0, 0}, {0, cX, -sX}, {0, sX, cX} };
    Ry = { {cY, 0, -sY}, {0, 1, 0}, {sY, 0, cY} };
    Rz = { {cZ, -sZ, 0}, {sZ, cZ, 0}, {0, 0, 1} };
    rot = mul(Rx, mul(Ry, Rz));
    for (int i=0; i<wid; ++i) for (int j=0; j<hei; ++j) {
        dep[i][j]=10000000;
        pix[i][j]=' ';
    }
    for (int i=0; i<pts.size(); ++i) {
      pt = app(rot, pts[i]);
      pt[0]*=2; // to scale back
      pt[2] += obj; // assume points centered at origin, move to z=160
      pt[0]*=(scr/pt[2]);
      pt[1]*=(scr/pt[2]);
      pt = add(pt, cen);
      if (!inrange(pt[0], pt[1]) || pt[2] > dep[(int)pt[0]][(int)pt[1]]) continue;
      dep[(int)pt[0]][(int)pt[1]] = pt[2];
      nor = app(rot, nors[i]);
      bri = cosang(light_dir, nor);
      bri = (bri+1)/2;
      pix[(int)pt[0]][(int)pt[1]] = s[(int)(bri*(s.length()-1))];
    }
    printf("\x1b[H");
    for (int i=hei-1; i>=0; --i) {
      for (int j=0; j<wid; ++j) {
        putchar(pix[j][i]);
      }
      putchar('\n');
    }
    usleep(3000);
  }
}

void donut(double r1=12, double r2=30, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, 1, -1}, vec disp = {0, 0, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  for (double phi=0; phi<6.28; phi+=0.05) {
    double cp=cos(phi), sp=sin(phi);
    for (double the=0; the<6.28; the+=0.05) {
      double ct=cos(the), st=sin(the);
      pts.push_back(app({ {cp, 0, -sp}, {0, 1, 0}, {sp, 0, cp} }, { r2+r1*ct, r1*st, 0 }));
      nor.push_back({ ct, st, 0 });
    }
  }
  for (int i=0; i<pts.size(); ++i) pts[i] = add(pts[i], disp);
  draw(pts, nor, dX, dY, dZ, light_dir, iter);
}

void cube(double len=50, double dX=0.08, double dY=0.04, double dZ=0.04, vec light_dir = {0, 1, -1}, vec disp = {0, 0, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  for (double i=-len/2; i<=len/2; i+=1) {
    for (double j=-len/2; j<=len/2; j+=1) {
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

void sphere(double r=20, double dX=0.08, double dY=0.04, double dZ=0.04, vec light_dir = {0, 1, -1}, vec disp = {15, 15, 0}, long long iter=10000) {
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

void ch4(double r1=10, double r2=5, double l=30, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, 1, -1}, vec disp = {0, 0, 0}, long long iter=10000) {
  vector<vec> pts, nor;
  double co=cos(20.5*3.1415926/180), si=sin(19.5*3.1415926/180);
  vec dis[4] = { {0, l, 0}, {co*l, -si*l, 0}, {-co*l/2, -si*l, co*l*sqrt(3)/2}, {-co*l/2, -si*l, -co*l*sqrt(3)/2} };
  for (double the=0; the<6.28; the+=0.05) {
    double ct=cos(the), st=sin(the);
    for (double phi=0; phi<3.14; phi+=0.05) {
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


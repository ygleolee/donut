#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

#define cout if(1) cerr
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

void neg(vec& v) {
  v[0]*=-1;
  v[1]*=-1;
  v[2]*=-1;
}

double cosang(vec x, vec y) {
  return (x[0]*y[0] + x[1]*y[1] + x[2]*y[2]) / sqrt((x[0]*x[0] + x[1]*x[1] + x[2]*x[2]) * (y[0]*y[0] + y[1]*y[1] + y[2]*y[2]));
}

int getcols() {
  char res[101];
  unique_ptr<FILE, decltype(&pclose)> pipe(popen("tput cols", "r"), pclose);
  fgets(res, 100, pipe.get());
  return stoi(res);
}

int getrows() {
  char res[101];
  unique_ptr<FILE, decltype(&pclose)> pipe(popen("tput lines", "r"), pclose);
  fgets(res, 100, pipe.get());
  return stoi(res);
}

void draw(vector<vec>& pts, vector<vec>& nors, double dX=0.08, double dY=0.02, double dZ=0.04, vec light_dir = {0, -1, 1}) {
  printf("\x1b[2J");
  double r1=9, r2=24, X=0, Y=0, Z=0, cX, cY, cZ, sX, sY, sZ, bri;
  double usr=160, scr=80, obj=0, scale; // user at z=0, screen at z=80, object at z=160
  // int wid=120, hei=60; // screen is 80*80, wid 160 for scaling
  int wid=getcols(), hei=getrows();
  vec pt, nor, cen={wid/2.0, hei/2.0, 0};
  neg(light_dir);
  mat Rx, Ry, Rz, rot;
  vector<vector<char>> pix(wid, vector<char>(hei));
  mat dep(wid, vector<double>(hei));
  string s=".......,,,,,:>+r=csu3V0Q@";
  // string s="...........:>+r=csu3V0Q@";
  // string s="......,,,-~:;=!*#$@";
  // string s=",,,,,,,,,,,,:>+r=csu3V0Q@";
  for (;;X+=dX, Y+=dY, Z+=dZ) {
    cX=cos(X), sX=sin(X), cY=cos(Y), sY=sin(Y), cZ=cos(Z), sZ=sin(Z);
    Rx = { {1, 0, 0}, {0, cX, -sX}, {0, sX, cX} };
    Ry = { {cY, 0, -sY}, {0, 1, 0}, {sY, 0, cY} };
    Rz = { {cZ, -sZ, 0}, {sZ, cZ, 0}, {0, 0, 1} };
    rot = mul(Rx, mul(Ry, Rz));
    for (int i=0; i<wid; ++i) for (int j=0; j<hei; ++j) {
      dep[i][j]=-1e9;
      pix[i][j]=' ';
    }
    for (int i=0; i<pts.size(); ++i) {
      pt = app(rot, pts[i]);
      pt[0]*=2; // to scale back
      pt[2]+=obj; // assume points centered at origin, move to z=160
      scale=(usr-scr)/(usr-pt[2]);
      pt[0]*=scale;
      pt[1]*=scale;
      pt = add(pt, cen);
      if (!inrange(pt[0], pt[1]) || pt[2] < dep[(int)pt[0]][(int)pt[1]]) continue;
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
    usleep(20000);
  }
}


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
  // cout << "app " << ans[0] << ' ' << ans[1] << ' ' << ans[2] << endl;
  return ans;
}

vec add(vec x, vec y) {
  return { x[0]+y[0], x[1]+y[1], x[2]+y[2] };
}

double cosang(vec x, vec y) {
  return (x[0]*y[0] + x[1]*y[1] + x[2]*y[2]) / sqrt((x[0]*x[0] + x[1]*x[1] + x[2]*x[2]) * (y[0]*y[0] + y[1]*y[1] + y[2]*y[2]));
}


int main() {
  printf("\x1b[2J");
  double r1=9, r2=24, X=0, Y=0, Z=0, dX=0.08, dY=0.02, dZ=0.04, cX, cY, cZ, sX, sY, sZ, the, phi, ct, st, cp, sp, dthe=0.05, dphi=0.05, bri;
  int wid=160, hei=80;
  vec light_dir = {1, 1, -1}, dis = {static_cast<double>(wid/2), static_cast<double>(hei/2), 50}, org = {0, 0, 0}, pt, nor;
  mat Rx, Ry, Rz, rot;
  vector<vector<char>> pix(wid, vector<char>(hei));
  mat dep(wid, vector<double>(hei));
  // string s=".-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
  // string s=".,-~:;=!*#$@";
  string s=".........,,,,,,:><++r=csu*?3V[Y0Q@";
  // string s=".-:=><+!rc*?LvJ|{1[Yak9pUD0Q@";
  for (; ; X+=dX, Y+=dY, Z+=dZ) {
    cout << X << " " << Y << " " << Z << endl;
    cX=cos(X), sX=sin(X), cY=cos(Y), sY=sin(Y), cZ=cos(Z), sZ=sin(Z);
    Rx = { {1, 0, 0}, {0, cX, -sX}, {0, sX, cX} };
    Ry = { {cY, 0, -sY}, {0, 1, 0}, {sY, 0, cY} };
    Rz = { {cZ, -sZ, 0}, {sZ, cZ, 0}, {0, 0, 1} };
    rot = mul(Rx, mul(Ry, Rz));
    for (int i=0; i<3; ++i) {
      for (int j=0; j<3; ++j) {
        cout << rot[i][j] << " ";
      }
      cout<< endl;
    }
    for (int i=0; i<wid; ++i)
      for (int j=0; j<hei; ++j) {
        dep[i][j]=10000000;
        pix[i][j]=' ';
      }
    for (phi=0; phi<6.28; phi+=dphi) {
      cp=cos(phi), sp=sin(phi);
      for (the=0; the<6.28; the+=dthe) {
        ct=cos(the), st=sin(the);
        pt = add(app(rot, app({ {cp, 0, -sp}, {0, 1, 0}, {sp, 0, cp} }, { r2+r1*ct, r1*st, 0 })), dis);
        cout << "pt " << pt[0] << " " << pt[1] << " " << pt[2] << ' ' << dep[(int)pt[0]][(int)pt[1]] << endl;
        if (!inrange(pt[0], pt[1]) || pt[2] > dep[(int)pt[0]][(int)pt[1]]) continue;
        dep[(int)pt[0]][(int)pt[1]] = pt[2];
        nor = app(rot, {ct, st, 0});
        bri = cosang(light_dir, nor);
        bri = (bri+1)/2;
        pix[(int)pt[0]][(int)pt[1]] = s[(int)(bri*(s.length()-1))];
      }
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

#include <bits/stdc++.h>
using namespace std;


int getcols() {
  array<char, 128> buffer;
  string result;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen("tput col", "r"), pclose);
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    cout << "yay" << buffer.data() << endl;
    result += buffer.data();
    // cout << result << endl;
  }
  return stoi(result);
}
int main() {
  int cols = getcols();
  cout << cols << endl;
}

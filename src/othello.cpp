
//
// version 8.0
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include "asi80.h"

using namespace std;

typedef vector<vector<char> > Mat;

pair<int,int> put_player (Mat m, int me) {
  struct AI80 x;
  return x.put_disk(m, me);
}

int main (int argc, char *argv[]) {
  Mat m(10, vector<char>(10, 3));
  for(int i = 1; i < 9; i++) {
    string s; cin >> s;
    for(int j = 0; j < 8; j++) {
      if (s[j] == '.') m[i][j+1] = 0;
      if (s[j] == 'x') m[i][j+1] = 1;
      if (s[j] == 'o') m[i][j+1] = 2;
    }
  }

	pair<int,int> p = put_player(m, 2);

  cout << p.first - 1 << " " << p.second - 1 << endl;
  return 0;
}

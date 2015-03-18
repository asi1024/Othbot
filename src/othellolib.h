#pragma once

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

using namespace std;

typedef vector<vector<char> > Mat;

const char xx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const char yy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int rx[64] = {1,2,3,4,5,6,7,8,8,8,8,8,8,8,8,7,6,5,4,3,2,1,1,1,1,1,1,1, 2,3,4,5,6,7,7,7,7,7,7,6,5,4,3,2,2,2,2,2, 3,4,5,6,6,6,6,5,4,3,3,3, 4,5,5,4};
const int ry[64] = {1,1,1,1,1,1,1,1,2,3,4,5,6,7,8,8,8,8,8,8,8,8,7,6,5,4,3,2, 2,2,2,2,2,2,3,4,5,6,7,7,7,7,7,7,6,5,4,3, 3,3,3,3,4,5,6,6,6,6,5,4, 4,4,5,5};

double tlims;

clock_t start;

char can_rev (const Mat &m, int me, int y, int x, int yy, int xx) {
  if (m[y][x] == me) return 0;
  if (m[y][x] == 3-me) return 1 + can_rev(m,me,y+yy,x+xx,yy,xx);
  return -99;
}

bool can_put (const Mat &m, int me, int y, int x) {
  if (m[y][x] != 0) return false;
  for (char i = 0; i < 8; i++)
    if (can_rev(m, me, y+yy[i], x+xx[i], yy[i], xx[i]) > 0) return true;
  return false;
}

bool rev (Mat &m, int me, int y, int x, int yy, int xx) {
  if (m[y][x] == me) return true;
  else if (m[y][x] == 3-me) {
    if (rev(m, me, y+yy, x+xx, yy, xx)){
      m[y][x]=me; return true;
    }
  }
  return false;
}
    
void put_it (Mat &m, int me, int y, int x) {
  for (int i = 0; i < 8; i++)
    rev(m,me,y+yy[i],x+xx[i],yy[i],xx[i]);
  m[y][x] = me;
}

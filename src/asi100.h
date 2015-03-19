
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
#include <fstream>
#include "othellolib.h"

#define DEBUG false
#define S_DEBUG true

using namespace std;

typedef vector<vector<char> > Mat;

const int elect_num = 5;

// 10
struct AI100 {
  vector<vector<double> > dat;
  map<Mat,pair<int,int> > elect;
  AI100() {
    dat.assign(4,vector<double>(6561,0.0));
    ifstream ifs("asi100_eval.dat");
    int a, b, c;
    double d;
    int num = -1;
    while(ifs >> a >> b >> c >> d) {
      if (a == 0) num++;
      dat[num][a] = d;
    }
    ifstream ifss("asi100_elect.dat");
    Mat m(10,vector<char>(10,3));
    while (ifss >> a) {
      for (int i = 1; i < 9; i++) {
	for (int j = 1; j < 9; j++) {
	  int in;
	  ifss >> in;
	  m[i][j] = in;
	}
      }
      ifss >> a >> b;
      elect[m] = make_pair(a, b);
    }
  }
  ~AI100() {
    ofstream ofs("asi100_elect.dat");
    for (map<Mat,pair<int,int> >::iterator it = elect.begin(); it != elect.end(); ++it) {
      ofs << 1 << endl;
      Mat m = (*it).first;
      int a = (*it).second.first, b = (*it).second.second;
      for (int i = 1; i < 9; i++) {
	for (int j = 1; j < 9; j++) {
	  if (j != 1) ofs << " ";
	  ofs << (int)m[i][j];
	}
	ofs << endl;
      }
      ofs << a << " " << b << endl;
    }
  }
  double eval4_sub(const Mat &m) {
    double res = 0;
    int num = 0;
    for (int i = 1; i < 3; i++)
      for (int j = 1; j < 5; j++)
	num = num * 3 + m[i][j];
    res += dat[0][num];
    num = 0;
    for (int i = 1; i < 9; i++)
      num = num * 3 + m[1][i];
    res += dat[1][num];
    num = 0;
    for (int i = 1; i < 9; i++)
      num = num * 3 + m[2][i];
    res += dat[2][num];
    num = 0;
    for (int i = 1; i < 4; i++)
      for (int j = 1; j < 4 && i + j < 6; j++)
	num = num * 3 + m[i][j];
    res += dat[3][num];
    return res;
  }
  int eval4(Mat m, int me) {
    if (me == 2) {
      for (int i = 1; i < 9; i++) {
	for (int j = 1; j < 9; j++) {
	  if (m[i][j] == 1) m[i][j] = 2;
	  else if (m[i][j] == 2) m[i][j] = 1;
	}
      }
    }
    double res = 0;
    res += eval4_sub(m);
    rotate(m);
    res += eval4_sub(m);
    rotate(m);
    res += eval4_sub(m);
    rotate(m);
    res += eval4_sub(m);
    horizonal(m);
    res += eval4_sub(m);
    rotate(m);
    res += eval4_sub(m);
    rotate(m);
    res += eval4_sub(m);
    rotate(m);
    res += eval4_sub(m);
    return res * 5;
  }
  int eval3(const Mat &m, int me) {
    int a = 0, b = 0;
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++) {
	if (m[i][j] == me) a++;
	if (m[i][j] == 3-me) b++;
      }
    if (a == 0) return -999999;
    if (b == 0) return 999999;
    return (a - b) * 60;
  }
  
  int eval2(const Mat &m, int me) {
    bool certain[2][10][10];
    int evalmap[8][8] = {
      {150,-50,  5, -5, -5,  5,-50,150},
      {-50,-75,-15, -9, -9,-15,-75,-50},
      {  5,-15,  8, -6, -6,  8,-15,  5},
      { -5, -9, -6,  0,  0, -6, -9, -5},
      { -5, -9, -6,  0,  0, -6, -9, -5},
      {  5,-15,  8, -6, -6,  8,-15,  5},
      {-50,-75,-15, -9, -9,-15,-75,-50},
      {150,-50,  5, -5, -5,  5,-50,150} };
    int res = 0;
    
    // Certain
  
    for (int i = 0; i < 10; i++)
      for (int j = 0; j < 10; j++)
	certain[0][i][j] = certain[1][i][j] = true;
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++)
	certain[0][i][j] = certain[1][i][j] = false;
    for (int k = 0; k < 2; k++)
      for (int i = 0; i < 128; i++) {
	int num=0, xit = (i<64?rx[i]:ry[i-64]), yit = (i<64?ry[i]:rx[i-64]);
	if (m[xit][yit] != k + 1) continue;
	for (int j = 0; j < 8; j+=2)
	  num += (certain[k][xit+xx[j]][yit+yy[j]] || certain[k][xit+xx[j+1]][yit+yy[j+1]]);
	if (num >= 4) {
	  certain[k][xit][yit] = true;
	  evalmap[xit-1][yit-1] = max(evalmap[xit-1][yit-1], 60);
	}
      }
    
    // Edge
    bool flag = true; int i;
    for(i=2;i<8&&flag;i++)if(m[1][i]!=me)flag=false;if(flag)res+=140;flag=true;
    for(i=2;i<8&&flag;i++)if(m[8][i]!=me)flag=false;if(flag)res+=140;flag=true;
    for(i=2;i<8&&flag;i++)if(m[i][1]!=me)flag=false;if(flag)res+=140;flag=true;
    for(i=2;i<8&&flag;i++)if(m[i][8]!=me)flag=false;if(flag)res+=140;flag=true;
    for(i=2;i<8&&flag;i++)if(m[1][i]!=3-me)flag=false;if(flag)res-=140;flag=true;
    for(i=2;i<8&&flag;i++)if(m[8][i]!=3-me)flag=false;if(flag)res-=140;flag=true;
    for(i=2;i<8&&flag;i++)if(m[i][1]!=3-me)flag=false;if(flag)res-=140;flag=true;
    for(i=2;i<8&&flag;i++)if(m[i][8]!=3-me)flag=false;if(flag)res-=140;flag=true;
    
    // Culc
    
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++) {
	if (m[i][j] == 0) continue;
	else {
	  res += evalmap[i-1][j-1] * (m[i][j]==me ? 1 : -1);
	  for (int k = 0; k < 8; k++) {
	    if (m[i+xx[k]][j+yy[k]] == 0) {
	      if (m[i][j] == me) res -= 5;
	      else res += 5;
	    }
	  }
	}
      }
    return res;
  }
  
  int eval1(const Mat &m, int me) {
    bool certain[2][10][10];
    int evalmap[8][8] = {
      {150,-60,  5, -5, -5,  5,-60,150},
      {-60,-75,-15, -9, -9,-15,-75,-60},
      {  5,-15,  8, -6, -6,  8,-15,  5},
      { -5, -9, -6,  0,  0, -6, -9, -5},
      { -5, -9, -6,  0,  0, -6, -9, -5},
      {  5,-15,  8, -6, -6,  8,-15,  5},
      {-60,-75,-15, -9, -9,-15,-75,-60},
      {150,-60,  5, -5, -5,  5,-60,150} };
    int res = 0;
    
    // Certain
  
    for (int i = 0; i < 10; i++)
      for (int j = 0; j < 10; j++)
	certain[0][i][j] = certain[1][i][j] = true;
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++)
	certain[0][i][j] = certain[1][i][j] = false;
    for (int k = 0; k < 2; k++)
      for (int i = 0; i < 128; i++) {
	int num=0, xit = (i<64?rx[i]:ry[i-64]), yit = (i<64?ry[i]:rx[i-64]);
	if (m[xit][yit] != k + 1) continue;
	for (int j = 0; j < 8; j+=2)
	  num += (certain[k][xit+xx[j]][yit+yy[j]] || certain[k][xit+xx[j+1]][yit+yy[j+1]]);
	if (num >= 4) {
	  certain[k][xit][yit] = true;
	  evalmap[xit-1][yit-1] = max(evalmap[xit-1][yit-1], 60);
	}
      }
    
    // Edge
    bool flag = true;
    for(int i=2;i<8&&flag;i++)if(m[1][i]!=me)flag=false;if(flag)res+=140;flag=true;
    for(int i=2;i<8&&flag;i++)if(m[8][i]!=me)flag=false;if(flag)res+=140;flag=true;
    for(int i=2;i<8&&flag;i++)if(m[i][1]!=me)flag=false;if(flag)res+=140;flag=true;
    for(int i=2;i<8&&flag;i++)if(m[i][8]!=me)flag=false;if(flag)res+=140;flag=true;
    for(int i=2;i<8&&flag;i++)if(m[1][i]!=3-me)flag=false;if(flag)res-=140;flag=true;
    for(int i=2;i<8&&flag;i++)if(m[8][i]!=3-me)flag=false;if(flag)res-=140;flag=true;
    for(int i=2;i<8&&flag;i++)if(m[i][1]!=3-me)flag=false;if(flag)res-=140;flag=true;
    for(int i=2;i<8&&flag;i++)if(m[i][8]!=3-me)flag=false;if(flag)res-=140;flag=true;
    
    // Culc
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++) {
	if (m[i][j] == 0) continue;
	else {
	  res += evalmap[i-1][j-1] * (m[i][j]==me ? 1 : -1);
	  for (int k = 0; k < 8; k++) {
	    if (m[i+xx[k]][j+yy[k]] == 0) {
	      if (m[i][j] == me) res -= 7;
	      else res += 7;
	    }
	  }
	}
      }
    return res;
  }
  
  int eval(const Mat &m, int me, int check) {
    if (check) return eval3(m, me);
    int opens = 0;
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++)
	if (m[i][j] == 0) opens++;
    if (opens < 38) return (eval4(m, me) + eval2(m, me)) / 2;
    //if (opens < 21) return eval2(m, me);
    return eval1(m, me);
  }
	
  int alpha_beta_sub (const Mat &m, int me, int n, int pass, int alpha, int beta) {
    if (n == 0) return eval(m, me, 0);
    bool flag = false;
    Mat mm;
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++)
	if (can_put (m, me, i, j)) {
	  mm = m; flag = true;
	  put_it(mm, me, i, j);
	  int val = - alpha_beta_sub(mm, 3-me, n-1, 0, -beta, -alpha);
	  alpha = max(alpha, val);
	  if (alpha >= beta) return alpha;
	}
    if (flag) return alpha;
    if (pass == 1) return eval(m, me, 1);
    return - alpha_beta_sub(m, 3-me, n, pass+1, -beta, -alpha);
  }
  
  int alpha_beta (const Mat &m, int me, int n, int pass, int alpha, int beta) {
    if((double)clock() - start > CLOCKS_PER_SEC * tlims) return -99999;
    if (n < 5) return alpha_beta_sub(m, me, n, pass, alpha, beta);
    if (n == 0) return eval(m, me, 0);
    vector<pair<int,pair<Mat,pair<int,int> > > > v;
    bool flag = false;
    Mat mm;
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++)
	if (can_put (m, me, i, j)) {
	  v.push_back(make_pair(0, make_pair(m, make_pair(i, j))));
	  flag = true;
	  put_it(v[v.size()-1].second.first, me, i, j);
	  v[v.size()-1].first = eval(m, me, 0);
	}
    sort(v.rbegin(),v.rend());
    
    for (unsigned i = 0; i < v.size(); i++) {
      int val = - alpha_beta(v[i].second.first, 3-me, n-1, 0, -beta, -alpha);
      alpha = max(alpha, val);
      if (alpha >= beta) return alpha;
    }
    if (flag) return alpha;
    if (pass == 1) return eval(m, me, 1);
    return - alpha_beta(m, 3-me, n, pass+1, -beta, -alpha);
  }
  
  pair<int,int> AI (const Mat &m, int me, int n) {
    int value = -999999999;
    Mat mm;
    vector<pair<int,pair<Mat,pair<int,int> > > > v;
    pair<int,int> res;
    
    int opens = 0;
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++)
	opens += (m[i][j] == 0);
    
    for (int i = 1; i < 9; i++)
      for (int j = 1; j < 9; j++)
	if (can_put (m, me, i, j)) {
	  v.push_back(make_pair(0, make_pair(m, make_pair(i, j))));
	  put_it(v[v.size()-1].second.first, me, i, j);
	  v[v.size()-1].first = eval(v[v.size()-1].second.first, me, 0);
	}
    
    sort(v.rbegin(),v.rend());
    
    if (DEBUG) cout << " == " << n << " == " << endl;
    
    for (unsigned i = 0; i < v.size(); i++) {
      int val = - alpha_beta(v[i].second.first, 3-me, n, 0, -99999999, -value);
      if (val > value) {
        value = val; res = v[i].second.second;
      }
      if(DEBUG){
        cout << (char)('a'+v[i].second.second.second-1);
        cout << v[i].second.second.first << " : ";
        if(abs(val) == 99999) cout << "*" << endl; else cout << val << endl;
      }
    }
    if(DEBUG) cout << "Value : " << value << endl;
    if (S_DEBUG) fprintf (stderr, "=> %.2lf : %.2lf\n", 32+value/120.0, 32-value/120.0);
    
    return res;
  }
  
  pair<int,int> put_disk(const Mat &m, int me) {
    int rest=0;
    for(int i=1;i<9;i++)for(int j=1;j<9;j++)rest+=(m[i][j]==0);
    pair<int,int> p;
    if (61-rest < elect_num) tlims = 7.0;
    else tlims = 0.1;
    cerr << rest << endl;
    if(rest==60) return make_pair(5, 6);
    if (61-rest < elect_num) {
      if(elect.count(m)) return elect[m];
      Mat mmm = m;
      rotate(mmm);
      rotate(mmm);
      if(elect.count(mmm)) {
	pair<int,int> p = elect[mmm];
	p = make_pair(9-p.second, p.first);
	p = make_pair(9-p.second, p.first);
	return p;
      }
      horizonal(mmm);
      rotate(mmm);
      if(elect.count(mmm)) {
	pair<int,int> p = elect[mmm];
	p = make_pair(9-p.second, p.first);
	p = make_pair(9-p.first, p.second);
	p = make_pair(9-p.second, p.first);
	p = make_pair(9-p.second, p.first);
	return p;
      }
      rotate(mmm);
      rotate(mmm);
      if(elect.count(mmm)) {
	pair<int,int> p = elect[mmm];
	p = make_pair(9-p.second, p.first);
	p = make_pair(9-p.second, p.first);
	p = make_pair(9-p.second, p.first);
	p = make_pair(9-p.first, p.second);
	p = make_pair(9-p.second, p.first);
	p = make_pair(9-p.second, p.first);
	return p;
      }
    }
    start = clock();
    int i;
    i = min(rest, 1);
    if (rest <= 16) i = min(rest, 1);
    for (; i <= rest; i++) {
      pair<int,int> q = AI(m, me, i);
      if((double)clock() - start > CLOCKS_PER_SEC * tlims) {
	if (61-rest < elect_num) elect[m]=p;
	return p;
      }
      p = q;
    }
    if (S_DEBUG) cerr << "COMPLETE!!" << endl;
    //while((double)clock() - start < CLOCKS_PER_SEC * 1.0);
    return p;
  }
};

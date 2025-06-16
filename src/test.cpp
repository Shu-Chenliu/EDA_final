#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <sstream>
#include <typeinfo>
#include "../inc/Board.h"
using namespace std;

vector<string> split(const string& s){
  istringstream iss(s);
  string ss;
  vector<string> str;
  while (iss >> ss){
    str.push_back(ss);
  }
  return str;
}

void readFile(string file){
  ifstream f(file);

  if (!f.is_open()) {
    cerr << "Fail to open" << endl;
    return;
  }

  string s;
  vector<string> str;
  while (getline(f, s)){
    cout << "/" << s << "/" << endl;
    str = split(s);
    for (int i=0; i<(int)str.size(); i++){
      cout << " ~" << str[i] << "~ " << endl;
    }
  }
  f.close();
  return;
} 

class A{
  public:
    int a;
    A(){a = -1;}
    A(int x){
      a = x;
    }
};

class AA : public A{
  public:
    float aa;
    AA(float x){
      aa = x;
    }
};

// void erase(string& s, char c){
//   s.erase(remove(s.begin(), s.end(), c), s.end());
// }

int main(){
  Board B;
  string file = "testcase1/testcase1";
  B.readWeight(file);
  // B.print();

  B.readDef(file);
  B.readV(file);

  B.print(1, 1, 0, 0);

  // readFile("testcase1/123.txt");
}
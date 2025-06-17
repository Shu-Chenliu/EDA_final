#ifndef MBFF_H
#define MBFF_H
#include <iostream>
#include <set>
#include <vector>
#include "Rect.h"
#include "Pin.h"
#include "Coor.h"
using namespace std;
enum DriveStrength { LOW, HIGH };

struct MBFFBit {
	string name;
	double slack;
	bool is_empty = false;
	DriveStrength strength;
};
struct Bin {
	int x_idx, y_idx;
	Rect area;
	int rank;
	bool occupied;
	Bin(int x, int y, const Rect& r)
		: x_idx(x), y_idx(y), area(r), rank(-1), occupied(false) {}
};
class MBFF{
  private:
    set<string> members;
    vector<Pin> pins;
    Rect feasible_region;
    Rect preferred_region;
    int driving_strength;
    vector<MBFFBit> bits;
    Coor position;
  public:
    MBFF();
    ~MBFF();
    void setMembers(set<string>& member);
    void setPosition(Coor coor);
    void setPins(vector<Pin> pins);
    void setFeasibleRegion(Rect feasible_region);
    void setPreferredRegion(Rect preferred_region);
    vector<Pin> getPins() const;
    Coor getCoor() const;
    float getX() const;
    float getY()const;
    set<string> getMembers()const;
    Rect getPreferredRegion()const;
    Rect getFeasibleRegion()const;
    int getDrivingStrength()const;
    vector<MBFFBit> getBits()const;
};


#endif
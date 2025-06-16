#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <string>
#include <limits.h>
using namespace std;

struct Point {
  double x, y;
	Point() : x(0), y(0) {}
	Point(double x, double y) : x(x), y(y) {}
	Point rotate45() const {
		double sqrt2_inv = 1.0 / sqrt(2.0);
		double rx = (x - y) * sqrt2_inv;
		double ry = (x + y) * sqrt2_inv;
		return Point(rx, ry);
	}
};
struct Rect {
	int x_min, x_max, y_min, y_max;
	string name;
	Rect() : x_min(0), x_max(0), y_min(0), y_max(0) {}
	Rect(int x1,int x2,int y1,int y2): x_min(x1), x_max(x2), y_min(y1), y_max(y2) {}
	bool intersects(const Rect& other) const {
		return !(x_max < other.x_min || other.x_max < x_min ||y_max < other.y_min || other.y_max < y_min);
	}
	Rect rotate45() {
    vector<Point> corners = {
			{ (double)x_min, (double)y_min },
			{ (double)x_min, (double)y_max },
			{ (double)x_max, (double)y_min },
			{ (double)x_max, (double)y_max }
    };

    double new_x_min = 1e9, new_x_max = -1e9;
    double new_y_min = 1e9, new_y_max = -1e9;

    for (const Point& pt : corners) {
			Point rotated = pt.rotate45();
			if (rotated.x < new_x_min) new_x_min = rotated.x;
			if (rotated.x > new_x_max) new_x_max = rotated.x;
			if (rotated.y < new_y_min) new_y_min = rotated.y;
			if (rotated.y > new_y_max) new_y_max = rotated.y;
    }

    // 這裡可以選擇用floor/ceil轉回int
    return Rect((int)floor(new_x_min), (int)ceil(new_x_max), (int)floor(new_y_min), (int)ceil(new_y_max));
}


};
// struct RegionGraph {
// 	unordered_map<string, Rect> ff_regions;
// 	unordered_map<string, unordered_set<string>> adj_list;
// };
struct Pin {
	Point position;
	double slack;
	int switching_rate;
};
struct FF {
  int cluster = -1;
  double relocatedX = -1, relocatedY = -1;
  string name;
	int original_drive;
	double area;
	Point position;
	vector<Pin> fanins;
	vector<Pin> fanouts;
	
	vector<FF*> next;
	FF(int drive, double a, double x, double y, const string& n = "", const vector<Pin>& fi = {}, const vector<Pin>& fo = {},const vector<FF*>&next={})
  : name(n), original_drive(drive),area(a), position(x, y), fanins(fi), fanouts(fo), next(next) {}

};

struct Cluster {
  double cx, cy;
  vector<FF*> flip_flops;
};

enum DriveStrength { LOW, HIGH };

struct MBFFBit {
	string name;
	double slack;
	bool is_empty = false;
	DriveStrength strength;
};
struct MBFF {
	set<string> members;
	vector<Pin> fanins, fanouts;
	Rect feasible_region;
	Rect preferred_region;
	int driving_strength;
	vector<MBFFBit> bits;
	// ... slot assignment fields
};
struct Bin {
	int x_idx, y_idx;
	Rect area;
	int rank;
	bool occupied;
	Bin(int x, int y, const Rect& r)
		: x_idx(x), y_idx(y), area(r), rank(-1), occupied(false) {}
};

#endif

#ifndef MBFFGENERATION_H
#define MBFFGENERATION_H

#include "classes.cpp"
#include "Board.h"
#include "Coor.h"
#include "Rect.h"
#include "Pin.h"
#include "FF.h"
#include "MBFF.h"
#include <set>
#include <cstdlib> // Needed for rand() and srand()
#include <ctime>
using namespace std;

struct Cluster {
  double cx, cy;
  vector<FF*> flip_flops;
};
class MBFFgeneration
{
	public:
		MBFFgeneration(vector<FF*> flipflops,int maxDrivingStrength,double beta);
		~MBFFgeneration();
		// Rect feasibleRegion(int driving_strength,FF* flipflop);
		// void generateGraph(int driving_strength);
		// vector<set<string>> findMaximalCliques(int driving_strength);
		vector<set<string>> generateMBFF();
		Rect feasibleRegionForClique(MBFF mbff);
		vector<MBFF> locationAssignment(vector<Bin>& bins);
		void MBFFsizing(vector<MBFF>& mbffs);
		int cost(set<string> c);
		pair<int,pair<set<string>,set<string>>> MBFFcost(set<string> c);
		
	protected:
	private:
		vector<FF*> flipflops;
		int maxDrivingStrength;
		// unordered_map<int, RegionGraph> driving_graphs;//can change to vector later
		// unordered_map<string, Rect> ff_regions;
		vector<set<string>> mbff_candidates;
		unordered_map<string,FF*> map;
		double beta;

};

#endif

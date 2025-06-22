#ifndef MBFFGENERATION_H
#define MBFFGENERATION_H

// #include "classes.cpp"
#include "Board.h"
#include "Coor.h"
#include "Rect.h"
#include "Pin.h"
#include "FF.h"
#include "MBFF.h"
#include "MST.h"
#include <algorithm>
#include <map>
#include <queue>
#include <unordered_set>
#include <set>
#include <cstdlib> // Needed for rand() and srand()
#include <ctime>
#include <limits>
using namespace std;

struct Cluster {
  double cx, cy;
  vector<FF*> flip_flops;
	double getX()const {
		return cx;
	}
	double getY()const {
		return cy;
	}
};
class MBFFgeneration
{
	//TODO: update maxdriving strength
	public:
		MBFFgeneration(vector<FF*> flipflops,int maxDrivingStrength,double b,float alpha,float beta,float gamma,float kt,float kp,float ka);
		~MBFFgeneration();
		// Rect feasibleRegion(int driving_strength,FF* flipflop);
		// void generateGraph(int driving_strength);
		// vector<set<string>> findMaximalCliques(int driving_strength);
		vector<set<string>> generateMBFF();
		Rect feasibleRegionForClique(MBFF mbff);
		vector<MBFF> locationAssignment(unordered_map<int,unordered_map<int,Bin*>>& bins,Board& board,double exactPower);
		void MBFFsizing(vector<MBFF>& mbffs);
		double cost(set<string> c);
		pair<double,pair<set<string>,set<string>>> MBFFcost(set<string> c);
		// void handleConnection(vector<MBFF>& mbffs);
		void downsizeMBFFs(vector<MBFF>& mbffs, double avg_slack);
		
	protected:
	private:
		vector<FF*> flipflops;
		int maxDrivingStrength;
		// unordered_map<int, RegionGraph> driving_graphs;//can change to vector later
		// unordered_map<string, Rect> ff_regions;
		vector<set<string>> mbff_candidates;
		unordered_map<string,FF*> map;
		double b;
		float alpha;
		float beta;
		float gamma;
		float kp;
		float ka;
		float kt;
};

#endif

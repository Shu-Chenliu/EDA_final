#ifndef MBFFGENERATION_H
#define MBFFGENERATION_H

#include "classes.cpp"
#include <cstdlib> // Needed for rand() and srand()
#include <ctime>
using namespace std;


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
		vector<MBFF> locationAssignment(Rect chip_area);
		void MBFFsizing(vector<MBFF>& mbffs);
		
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

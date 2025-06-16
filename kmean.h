#ifndef KMEAN_H
#define KMEAN_H

#include "classes.cpp"
#include "Coor.h"
#include "Rect.h"
#include "Pin.h"
#include "FF.h"
#include <cstdlib> // Needed for rand() and srand()
#include <ctime>
using namespace std;


class kmean
{
	public:
		kmean(int SIZE_LIMIT,int MAX_ITER,int DISP_LIMIT);
		~kmean();
    void assignPoints(vector<FF*>& flip_flops, vector<Cluster>& clusters, bool weighted);
		vector<Cluster> kmeansWeighted(vector<FF*>& flip_flops);
		void resolveOverDisplacement(vector<FF*>& flip_flops, vector<Cluster>& clusters);
		void resolveOverflow(vector<FF*>& flip_flops, vector<Cluster>& clusters);
    double weightedCost(const FF* p, const Cluster& c);
	protected:
	private:
		int SIZE_LIMIT;
    int MAX_ITER;
		double DISP_LIMIT;

};

#endif

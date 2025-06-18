#include "MBFFgeneration.h"

MBFFgeneration::MBFFgeneration(vector<FF*> flipflops, int maxDrivingStrength,double beta)
	: flipflops(flipflops), maxDrivingStrength(maxDrivingStrength), beta(beta) {
		for(size_t i=0;i<flipflops.size();i++){
			map[flipflops[i]->getName()]=flipflops[i];
		}
}

MBFFgeneration::~MBFFgeneration()
{
}
int slackToWireLength(int slack){
	return slack;
}
Rect manhattanCircle(const Coor& center, int radius) {
	return Rect(center.getX() - radius, center.getX() + radius, center.getY() - radius, center.getY() + radius);
}
Rect intersectRects(const vector<Rect>& rects) {
	Rect result = rects[0];
	for (const auto& r : rects) {
		result.setX(max(result.getX(), r.getX()));
		result.setW(min(result.getX()+result.getW() ,r.getX()+r.getW())-result.getX());
		result.setY(max(result.getY(), r.getY()));
		result.setH(min(result.getY()+result.getH() ,r.getY()+r.getH())-result.getY());
	}
	if(result.getW()<0||result.getH()<0){
		return Rect();
	}
	return result;
}
Rect feasibleRegion(int driving_strength,FF* flipflop){
	if (driving_strength < flipflop->getBit()) {
		return Rect();
	}
	vector<Rect> manhattanRegions;
	for (const auto& pin : flipflop->getPins()) {
		int radius = slackToWireLength(pin.getSlack());
		manhattanRegions.push_back(manhattanCircle(pin.getCoor()+flipflop->getRelocateCoor(), radius));
	}
	if (manhattanRegions.empty()){
		return Rect();
	}
	return intersectRects(manhattanRegions);
}
// void MBFFgeneration::generateGraph(int driving_strength){
// 	for(int i=0;i<flipflops.size();i++){
// 		Rect feasible_region=feasibleRegion(driving_strength,flipflops[i]);
// 		RegionGraph& graph=driving_graphs[driving_strength];
// 		for(const auto& region:graph.ff_regions){
// 			if(region.first==flipflops[i]->name){
// 				continue;
// 			}
// 			if(feasible_region.intersects(region.second)){
// 				graph.adj_list[flipflops[i]->name].insert(region.first);
// 				graph.adj_list[region.first].insert(flipflops[i]->name);
// 			}
// 		}
// 	}
// }
vector<set<string>> findMaximalCliquesSweepLine(vector<Rect>& rects) {
	vector<set<string>> maximal_cliques;
	vector<int> x_boundaries;
	cout<<"maximal clique"<<endl;
	for (const auto& r : rects) {
		if(r.getX()+r.getW()==0){
			continue;
		}
		x_boundaries.push_back(r.getX());
		x_boundaries.push_back(r.getX()+r.getW());
	}

	sort(x_boundaries.begin(), x_boundaries.end());
	x_boundaries.erase(unique(x_boundaries.begin(), x_boundaries.end()), x_boundaries.end());

	for (size_t i = 0; i + 1 < x_boundaries.size(); ++i) {
		int x_start = x_boundaries[i];
		int x_end = x_boundaries[i + 1];
		//TOOD: change to pointer
		vector<pair<int, string>> events; // (y, +name) for top, (y, -name) for bottom
		map<string, Rect> active_rects;

		for (const auto& r : rects) {
			if (r.getX() <= x_start && r.getX()+r.getW() >= x_end) {
				events.emplace_back(r.getY(), "+" + r.getName());
				events.emplace_back(r.getY()+r.getH(), "-" + r.getName());
				active_rects[r.getName()] = r;
			}
		}
		sort(events.begin(), events.end());

		set<string> active;
		set<string> prev_clique;
		vector<set<string>> FFinRegion;

		for (const auto& event : events) {
			bool is_entering = event.second[0] == '+';
			string name = event.second.substr(1);
			if (is_entering) {
				active.insert(name);
			} else {
				active.erase(name);
			}

			// if (active.size() >= 2 && active != prev_clique) {
			// 	maximal_cliques.push_back(active);
			// 	prev_clique = active;
			// }
			FFinRegion.push_back(active);
		}
		for(size_t i=1;i<FFinRegion.size()-1;i++){
			if(FFinRegion[i].size()>FFinRegion[i-1].size()&&FFinRegion[i].size()>FFinRegion[i+1].size()){
				maximal_cliques.push_back(FFinRegion[i]);
			}
		}
	}
	// Filter subset cliques
	set<set<string>> filtered;
	for (const auto& clique : maximal_cliques) {
		bool is_subset = false;
		for (const auto& other : maximal_cliques) {
			if (clique != other && includes(other.begin(), other.end(), clique.begin(), clique.end())) {
				is_subset = true;
				break;
			}
		}
		if (!is_subset) filtered.insert(clique);
	}
	return vector<set<string>>(filtered.begin(), filtered.end());
}

int MBFFgeneration::cost(set<string> c){
	double totalCost=0;
	double totalArea=0;
	double total_slack = 0;
  int total_switching = 0;
	int count=0;
	// double areaAfter=0;
	for(const auto&name:c){
		FF* ff=map[name];
		totalArea+=ff->getW()*ff->getH();
		for(const auto&pin:ff->getPins()){
			total_slack+=pin.getSlack();
			total_switching+=pin.getSR();
			count++;
		}
	}
	//TODO: delta 算法要改
	//TODO: add MST method
	double delta_area=c.size()/3;
	double delta_power=total_switching*0.15;
	double delta_tns=total_slack*0.15;
	totalCost-=delta_area;
	totalCost-=delta_power;
	totalCost-=delta_tns;
	return (int)totalCost;
}
pair<int, pair<set<string>, set<string>>> MBFFgeneration::MBFFcost(set<string> c) {
	int size = c.size();
	int currCost = INT_MAX;
	set<string> currClique;

	vector<string> elements(c.begin(), c.end()); // for random selection

	for (int i = 0; i < size * size; i++) {
		set<string> ff;
		for (const auto& ffs : elements) {
			int randomNum = rand() % 2;
			if (randomNum == 1) {
				ff.insert(ffs);
			}
		}

		if (ff.empty()) {
			int randomIndex = rand() % size;
			ff.insert(elements[randomIndex]);
		}

		int randomCost = cost(ff);
		if (currCost > randomCost) {
			currCost = randomCost;
			currClique = ff;
		}
	}
	return {currCost, {currClique,c}};
}
vector<set<string>> MBFFgeneration::generateMBFF(){
	cout << "[DEBUG] Start MBFF Generation" << endl;
	for (int strength = 1; strength <= 1; ++strength) {
		cout << "  -> Trying driving strength: " << strength << endl;
		vector<Rect> regions;
		// Build feasible regions
		for (FF* ff : flipflops) {
			if (strength < ff->getBit()) continue;
			Rect region = feasibleRegion(strength, ff);
			cout<<ff->getName()<<":"<<region.getX()<<" "<<region.getX()+region.getW()<<" "<<region.getY()<<" "<<region.getY()+region.getH()<<endl;
			region.setName(ff->getName());
			regions.push_back(region);
		}
		cout << "    Collected " << regions.size() << " feasible regions." << endl;
		//TODO: add total driving strength
		vector<set<string>> cliques = findMaximalCliquesSweepLine(regions);
		cout << "    Found " << cliques.size() << " maximal cliques." << endl;
		// Store the cliques
		for(set<string> clique:cliques){
			cout<<"clique"<<endl;
			mbff_candidates.push_back(clique);
			for(const auto& c:clique){
				cout<<c<<endl;
			}
		}
		
	}
	cout << "  => Total MBFF candidates: " << mbff_candidates.size() << endl;
	priority_queue<pair<int,pair<set<string>,set<string>>>,vector<pair<int,pair<set<string>,set<string>>>>,greater<pair<int,pair<set<string>,set<string>>>>> pq;
	vector<set<string>> non_conflictMBFF;
	for(const auto& maxclique:mbff_candidates){
		pq.push(MBFFcost(maxclique));
	}
	unordered_set<string> marked;
	while(!pq.empty()){
		pair<int,pair<set<string>,set<string>>> m=pq.top();
		pq.pop();
		bool isValid=true;
		for(const auto& ff:m.second.first){
			if(marked.count(ff)){
				isValid=false;
				break;
			}
		}
		if(isValid){
			non_conflictMBFF.push_back(m.second.first);
			cout << "  [Selected MBFF] with " << m.second.first.size() << " FFs." << endl;
			for(const auto& ff:m.second.first){
				marked.insert(ff);
			}
		}
		if(m.second.second.size()-m.second.first.size()>=2){
			for(const auto&ff:m.second.first){
				m.second.second.erase(ff);
			}
			pq.push(MBFFcost(m.second.second));
		}
	}
	for(FF* ff:flipflops){
		if(marked.count(ff->getName())){
			continue;
		}
		set<string> set;
		set.insert(ff->getName());
		non_conflictMBFF.push_back(set);
	}
	cout << "[DEBUG] Final selected MBFF count: " << non_conflictMBFF.size() << endl;
	return non_conflictMBFF;
}
int weightedMedian(vector<pair<int, int>>& coords_weights) {
	sort(coords_weights.begin(), coords_weights.end());
	int total_weight = 0;
	for (auto& p : coords_weights) total_weight += p.second;

	int prefix_sum = 0;
	for (auto& p : coords_weights) {
		prefix_sum += p.second;
		if (prefix_sum >= (total_weight + 1) / 2)
			return p.first;
	}
	return coords_weights.back().first;
}

Rect computePreferredRegion(const MBFF& mbff) {
	vector<pair<int, int>> x_weights, y_weights;
	cout<<mbff.getPins().size();
	for (auto& pin : mbff.getPins()) {
		x_weights.emplace_back(pin.getX()+mbff.getX(), pin.getSR());
		y_weights.emplace_back(pin.getY()+mbff.getY(), pin.getSR());
	}

	int x_center = weightedMedian(x_weights);
	int y_center = weightedMedian(y_weights);
	int margin = 5; // or some constant

	return Rect(margin, margin, x_center, y_center);
}

void assignMBFFLocation(MBFF& mbff, vector<Bin>& bins) {
	cout << "  [Bin Assignment] For MBFF with " << mbff.getMembers().size() << " members" << endl;
	for (auto& bin : bins) {
		if (bin.area.intersect(mbff.getPreferredRegion())) {
			bin.rank = 0;
		} else if (bin.area.intersect(mbff.getFeasibleRegion())) {
			// Calculate Manhattan distance to preferred region center
			int cx = (mbff.getPreferredRegion().getX()*2+mbff.getPreferredRegion().getW()) / 2;
			int cy = (mbff.getPreferredRegion().getY()*2+mbff.getPreferredRegion().getH()) / 2;
			int bx = (bin.area.getX()*2+bin.area.getW()) / 2;
			int by = (bin.area.getY()*2+bin.area.getH()) / 2;
			bin.rank = abs(cx - bx) + abs(cy - by);
		} else {
			bin.rank = -1; // invalid
		}
	}

	// 找出可用最低 rank 的 bin
	int min_rank = INT_MAX;
	Bin* best_bin = nullptr;
	for (auto& bin : bins) {
		if (!bin.occupied && bin.rank >= 0 && bin.rank < min_rank) {
			min_rank = bin.rank;
			best_bin = &bin;
		}
	}

	if (best_bin) {
		best_bin->occupied = true;
		mbff.setFeasibleRegion(best_bin->area);
		cout << "    Assigned to bin (" << best_bin->x_idx << "," << best_bin->y_idx << ") rank = " << best_bin->rank << endl;
	} else {
		cout << "    [Warning] No available bin found!" << endl;
	}
}
Rect MBFFgeneration::feasibleRegionForClique(MBFF mbff){
	int new_x_min = INT_MAX, new_x_max = INT_MIN;
  int new_y_min = INT_MAX, new_y_max = INT_MIN;
	for(const auto&ff:mbff.getMembers()){
		Rect region= feasibleRegion(mbff.getDrivingStrength(),map[ff]);
		new_x_min=max(new_x_min,(int)region.getX());
		new_x_max=min(new_x_max,(int)region.getX()+(int)region.getW());
		new_y_min=max(new_y_min,(int)region.getY());
		new_y_max=min(new_y_max,(int)region.getY()+(int)region.getW());
	}
	return Rect(new_x_min,new_x_max,new_y_min,new_y_max);
}
vector<MBFF> MBFFgeneration::locationAssignment(vector<Bin>& bins) {
	cout << "[DEBUG] Start MBFF Location Assignment" << endl;
	cout << "  -> Total bins generated: " << bins.size() << endl;
	vector<set<string>> non_conflictMBFF=generateMBFF();
	vector<MBFF> placed_mbffs;
	int count = 0;
	for (auto& clique : non_conflictMBFF) {
		cout << "  [MBFF#" << (++count) << "] Placing..." << endl;
		MBFF mbff;
		mbff.setMembers(clique);
		float x=0;
		float y=0;
		for (auto& name : clique) {
			cout<<name<<endl;
			FF* ff = map[name];
			x+=ff->getRelocateCoor().getX();
			y+=ff->getRelocateCoor().getY();
			mbff.setPins(ff->getPins());
		}
		mbff.setPosition(Coor(x/clique.size(),y/clique.size()));
		mbff.setFeasibleRegion(feasibleRegionForClique(mbff)); // intersect all FF feasible regions]
		mbff.setPreferredRegion(computePreferredRegion(mbff));
		assignMBFFLocation(mbff, bins);

		placed_mbffs.push_back(mbff);
	}
	cout << "[DEBUG] All MBFFs placed: " << placed_mbffs.size() << endl;
	return placed_mbffs;
}
void downsizeMBFFs(vector<MBFF>& mbffs, double avg_slack, double beta) {
	for (MBFF& mbff : mbffs) {
		int l = 0;
		// Step 1~5: 根據 slack 做降速處理
		for (MBFFBit& bit : mbff.getBits()) {
			if (!bit.is_empty && bit.slack > beta * avg_slack) {
				bit.strength = LOW;
				l++;
			}
		}
		// Step 6~8: 處理空 bit（都可降速）
		for (MBFFBit& bit : mbff.getBits()) {
			if (bit.is_empty) {
				bit.strength = LOW;
				l++;
			}
		}

		// int k = mbff.bits.size();
		// int h = k - l;
		// Step 10: 已原地更新 MBFF，可加上 power report 或 output log
	}
}

double computeAvgSlack(const vector<MBFF>& mbffs) {
	double total_slack = 0;
	int count = 0;
	for (const auto& mbff : mbffs) {
		for (const auto& bit : mbff.getBits()) {
			if (!bit.is_empty) {
				total_slack += bit.slack;
				count++;
			}
		}
	}
	return count > 0 ? total_slack / count : 0.0;
}
void MBFFgeneration::MBFFsizing(vector<MBFF>& mbffs){
	double avg_slack = computeAvgSlack(mbffs);
	cout << "[DEBUG] Average Slack: " << avg_slack << endl;
	downsizeMBFFs(mbffs, avg_slack,beta);
}
void MBFFgeneration::handleConnection(vector<MBFF>& mbffs){
	for(int i=0;i<mbffs.size();i++){
		unordered_set<int> nextConn;
		for(const auto&memberName:mbffs[i].getMembers()){
			FF* ff=map[memberName];
			vector<string> nexts=ff->getNextName();
			for(int j=0;j<mbffs.size();j++){
				if(i!=j){
					for(const auto&next:nexts){
						if(mbffs[j].getMembers().count(next)){
							nextConn.insert(j);
							break;
						}
					}
				}
			}
		}
		for(const auto& index:nextConn){
			mbffs[i].addNext(index);
		}
	}
}
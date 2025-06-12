#include "MBFFgeneration.h"

MBFFgeneration::MBFFgeneration(vector<FF*> flipflops, int maxDrivingStrength,double beta)
	: flipflops(flipflops), maxDrivingStrength(maxDrivingStrength), beta(beta) {

}

MBFFgeneration::~MBFFgeneration()
{
}
int slackToWireLength(int slack){
	return slack*0.1;
}
Rect manhattanCircle(const Point& center, int radius) {
	return Rect(center.x - radius, center.x + radius, center.y - radius, center.y + radius);
}
Rect intersectRects(const vector<Rect>& rects) {
	Rect result = rects[0];
	for (const auto& r : rects) {
		result.x_min = max(result.x_min, r.x_min);
		result.x_max = min(result.x_max, r.x_max);
		result.y_min = max(result.y_min, r.y_min);
		result.y_max = min(result.y_max, r.y_max);
	}
	return result;
}
Rect feasibleRegion(int driving_strength,FF* flipflop){
	if (driving_strength < flipflop->original_drive) {
		return Rect();
	}
	vector<Rect> manhattanRegions;
	for (const auto& pin : flipflop->fanins) {
		int radius = slackToWireLength(pin.slack);
		manhattanRegions.push_back(manhattanCircle(pin.position, radius));
	}
	for (const auto& pin : flipflop->fanouts) {
		int radius = slackToWireLength(pin.slack);
		manhattanRegions.push_back(manhattanCircle(pin.position, radius).rotate45());
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

	for (const auto& r : rects) {
		x_boundaries.push_back(r.x_min);
		x_boundaries.push_back(r.x_max);
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
			if (r.x_min <= x_start && r.x_max >= x_end) {
				events.emplace_back(r.y_min, "+" + r.name);
				events.emplace_back(r.y_max, "-" + r.name);
				active_rects[r.name] = r;
			}
		}

		sort(events.begin(), events.end());

		set<string> active;
		set<string> prev_clique;

		for (const auto& event : events) {
			bool is_entering = event.second[0] == '+';
			string name = event.second.substr(1);

			if (is_entering) {
				active.insert(name);
			} else {
				active.erase(name);
			}

			if (active.size() >= 2 && active != prev_clique) {
				maximal_cliques.push_back(active);
				prev_clique = active;
			}
		}
	}

	// Filter subset cliques
	vector<set<string>> filtered;
	for (const auto& clique : maximal_cliques) {
		bool is_subset = false;
		for (const auto& other : maximal_cliques) {
			if (clique != other && includes(other.begin(), other.end(), clique.begin(), clique.end())) {
				is_subset = true;
				break;
			}
		}
		if (!is_subset) filtered.push_back(clique);
	}

	return filtered;
}

pair<int,pair<set<string>,set<string>>> cost(set<string> c){
	int totalCost=0;
	//TODO: 可以generate binary code去random產生要取的ff有哪些 做個c.size次之類的
	return {totalCost,{c,c}};
}
vector<set<string>> MBFFgeneration::generateMBFF(){
	cout << "[DEBUG] Start MBFF Generation" << endl;
	for (int strength = 1; strength <= maxDrivingStrength; ++strength) {
		cout << "  -> Trying driving strength: " << strength << endl;
		vector<Rect> regions;
		// Build feasible regions
		for (FF* ff : flipflops) {
			if (strength < ff->original_drive) continue;
			Rect region = feasibleRegion(strength, ff);
			region.name = ff->name;
			regions.push_back(region);
		}
		cout << "    Collected " << regions.size() << " feasible regions." << endl;
		//TODO: add total driving strength
		vector<set<string>> cliques = findMaximalCliquesSweepLine(regions);
		cout << "    Found " << cliques.size() << " maximal cliques." << endl;
		// Store the cliques
		for(set<string> clique:cliques){
			mbff_candidates.push_back(clique);
		}
		
	}
	cout << "  => Total MBFF candidates: " << mbff_candidates.size() << endl;
	priority_queue<pair<int,pair<set<string>,set<string>>>,vector<pair<int,pair<set<string>,set<string>>>>,greater<pair<int,pair<set<string>,set<string>>>>> pq;
	vector<set<string>> non_conflictMBFF;
	for(const auto& maxclique:mbff_candidates){
		pq.push(cost(maxclique));
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
			pq.push(cost(m.second.second));
		}
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
	for (auto& pin : mbff.fanins) {
		x_weights.emplace_back(pin.position.x, pin.switching_rate);
		y_weights.emplace_back(pin.position.y, pin.switching_rate);
	}
	for (auto& pin : mbff.fanouts) {
		x_weights.emplace_back(pin.position.x, pin.switching_rate);
		y_weights.emplace_back(pin.position.y, pin.switching_rate);
	}

	int x_center = weightedMedian(x_weights);
	int y_center = weightedMedian(y_weights);
	int margin = 5; // or some constant

	return Rect(x_center - margin, x_center + margin, y_center - margin, y_center + margin);
}
vector<Bin> generateBins(Rect chip_area, int bin_width, int bin_height) {
	vector<Bin> bins;
	int x_bins = (chip_area.x_max - chip_area.x_min) / bin_width;
	int y_bins = (chip_area.y_max - chip_area.y_min) / bin_height;

	for (int i = 0; i < x_bins; ++i) {
		for (int j = 0; j < y_bins; ++j) {
			Rect area(
				chip_area.x_min + i * bin_width,
				chip_area.x_min + (i + 1) * bin_width,
				chip_area.y_min + j * bin_height,
				chip_area.y_min + (j + 1) * bin_height
			);
			bins.push_back({i, j, area});
		}
	}
	return bins;
}
void assignMBFFLocation(MBFF& mbff, vector<Bin>& bins) {
	cout << "  [Bin Assignment] For MBFF with " << mbff.members.size() << " members" << endl;
	for (auto& bin : bins) {
		if (bin.area.intersects(mbff.preferred_region)) {
			bin.rank = 0;
		} else if (bin.area.intersects(mbff.feasible_region)) {
			// Calculate Manhattan distance to preferred region center
			int cx = (mbff.preferred_region.x_min + mbff.preferred_region.x_max) / 2;
			int cy = (mbff.preferred_region.y_min + mbff.preferred_region.y_max) / 2;
			int bx = (bin.area.x_min + bin.area.x_max) / 2;
			int by = (bin.area.y_min + bin.area.y_max) / 2;
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
		mbff.feasible_region = best_bin->area;
		cout << "    Assigned to bin (" << best_bin->x_idx << "," << best_bin->y_idx << ") rank = " << best_bin->rank << endl;
	} else {
		cout << "    [Warning] No available bin found!" << endl;
	}
}
Rect MBFFgeneration::feasibleRegionForClique(MBFF mbff){
	int new_x_min = INT_MAX, new_x_max = INT_MIN;
  int new_y_min = INT_MAX, new_y_max = INT_MIN;
	for(const auto&ff:mbff.members){
		Rect region= feasibleRegion(mbff.driving_strength,map[ff]);
		new_x_min=max(new_x_min,region.x_min);
		new_x_max=min(new_x_max,region.x_max);
		new_y_min=max(new_y_min,region.y_min);
		new_y_max=min(new_y_max,region.y_max);
	}
	return Rect(new_x_min,new_x_max,new_y_min,new_y_max);
}
vector<MBFF> MBFFgeneration::locationAssignment(Rect chip_area) {
	cout << "[DEBUG] Start MBFF Location Assignment" << endl;
	int bin_width = 20, bin_height = 20;
	vector<Bin> bins = generateBins(chip_area, bin_width, bin_height);
	cout << "  -> Total bins generated: " << bins.size() << endl;
	vector<set<string>> non_conflictMBFF=generateMBFF();
	vector<MBFF> placed_mbffs;
	int count = 0;
	for (auto& clique : non_conflictMBFF) {
		cout << "  [MBFF#" << (++count) << "] Placing..." << endl;
		MBFF mbff;
		mbff.members = clique;

		for (auto& name : clique) {
			FF* ff = map[name];
			mbff.fanins.insert(mbff.fanins.end(), ff->fanins.begin(), ff->fanins.end());
			mbff.fanouts.insert(mbff.fanouts.end(), ff->fanouts.begin(), ff->fanouts.end());
		}

		mbff.feasible_region = feasibleRegionForClique(mbff); // intersect all FF feasible regions
		mbff.preferred_region = computePreferredRegion(mbff);
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
		for (MBFFBit& bit : mbff.bits) {
			if (!bit.is_empty && bit.slack > beta * avg_slack) {
				bit.strength = LOW;
				l++;
			}
		}
		// Step 6~8: 處理空 bit（都可降速）
		for (MBFFBit& bit : mbff.bits) {
			if (bit.is_empty) {
				bit.strength = LOW;
				l++;
			}
		}

		int k = mbff.bits.size();
		int h = k - l;
		// Step 10: 已原地更新 MBFF，可加上 power report 或 output log
	}
}

double computeAvgSlack(const vector<MBFF>& mbffs) {
	double total_slack = 0;
	int count = 0;
	for (const auto& mbff : mbffs) {
		for (const auto& bit : mbff.bits) {
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

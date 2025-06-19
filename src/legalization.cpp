#include "legalization.h"

Legalization::Legalization(){

}
Legalization::~Legalization(){

}

void Legalization::legalizePlacing(vector<FF*>& flipflops, vector<Bin>& bins, Board& board) {
    sort(flipflops.begin(), flipflops.end(), [](const FF* a, const FF* b) {
        return a->getMinSlack() < b->getMinSlack();
    });

    int x_bins = board.getW() / board.getBinW();
    int y_bins = board.getH() / board.getBinH();

    auto inBounds = [&](int x, int y) {
        return x >= 0 && y >= 0 && x < x_bins && y < y_bins;
    };

    for (FF* ff : flipflops) {
        cout<<ff->getName()<<endl;
        int required_bins_x = ceil(ff->getW() / board.getBinW());
        int required_bins_y = ceil(ff->getH() / board.getBinH());

        int start_x = ff->getX() / board.getBinW();
        int start_y = ff->getY() / board.getBinH();

        queue<pair<int, int>> q;
        unordered_set<int> visited;
        q.push({start_x, start_y});
        visited.insert(start_y * x_bins + start_x);

        bool placed = false;
        const vector<pair<int, int>> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        while (!q.empty()) {
            auto [cx, cy] = q.front(); 
            q.pop();

            // 檢查從 (cx, cy) 開始右下方是否有足夠空間
            bool valid = true;
            vector<Bin*> candidate_bins;

            for (int dx = 0; dx < required_bins_x; ++dx) {
                for (int dy = 0; dy < required_bins_y; ++dy) {
                    int nx = cx + dx;
                    int ny = cy + dy;

                    if (!inBounds(nx, ny)) {
                        valid = false;
                        break;
                    }

                    int idx = ny * x_bins + nx;
                    if (bins[idx].occupied) {
                        valid = false;
                        break;
                    }

                    candidate_bins.push_back(&bins[idx]);
                }
                if (!valid) break;
            }

            if (valid) {
                // 所有必要的 bin 都沒被佔用，可放置
                for (auto* b : candidate_bins)
                    b->occupied = true;

                ff->setX(bins[cy * x_bins + cx].area.getX());
                ff->setY(bins[cy * x_bins + cx].area.getY());
                placed = true;
                break;
            }

            for (auto [dx, dy] : directions) {
                int nx = cx + dx;
                int ny = cy + dy;
                int index = ny * x_bins + nx;
                if (inBounds(nx, ny) && !visited.count(index)) {
                    visited.insert(index);
                    q.push({nx, ny});
                }
            }
        }

        if (!placed) {
            cerr << "[Warning] Cannot place FF " << ff->getName() << " of size " 
                 << ff->getW() << "x" << ff->getH() << endl;
        }
    }
}
void Legalization::legalizePlacing(vector<MBFF>& mbffs, vector<Bin>& bins, Board& board) {
    sort(mbffs.begin(), mbffs.end(), [](const MBFF* a, const MBFF* b) {
        return a->getMinSlack() < b->getMinSlack();
    });

    int x_bins = board.getW() / board.getBinW();
    int y_bins = board.getH() / board.getBinH();

    auto inBounds = [&](int x, int y) {
        return x >= 0 && y >= 0 && x < x_bins && y < y_bins;
    };

    for (MBFF mbff : mbffs) {
        int required_bins_x = ceil(mbff->getW() / board.getBinW());
        int required_bins_y = ceil(mbff->getH() / board.getBinH());

        int start_x = mbff->getX() / board.getBinW();
        int start_y = mbff->getY() / board.getBinH();

        queue<pair<int, int>> q;
        unordered_set<int> visited;
        q.push({start_x, start_y});
        visited.insert(start_y * x_bins + start_x);

        bool placed = false;
        const vector<pair<int, int>> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        while (!q.empty()) {
            auto [cx, cy] = q.front(); 
            q.pop();

            // 檢查從 (cx, cy) 開始右下方是否有足夠空間
            bool valid = true;
            vector<Bin*> candidate_bins;

            for (int dx = 0; dx < required_bins_x; ++dx) {
                for (int dy = 0; dy < required_bins_y; ++dy) {
                    int nx = cx + dx;
                    int ny = cy + dy;

                    if (!inBounds(nx, ny)) {
                        valid = false;
                        break;
                    }

                    int idx = ny * x_bins + nx;
                    if (bins[idx].occupied) {
                        valid = false;
                        break;
                    }

                    candidate_bins.push_back(&bins[idx]);
                }
                if (!valid) break;
            }

            if (valid) {
                // 所有必要的 bin 都沒被佔用，可放置
                for (auto* b : candidate_bins)
                    b->occupied = true;

                mbff->setX(bins[cy * x_bins + cx].area.getX());
                mbff->setY(bins[cy * x_bins + cx].area.getY());
                placed = true;
                break;
            }

            for (auto [dx, dy] : directions) {
                int nx = cx + dx;
                int ny = cy + dy;
                int index = ny * x_bins + nx;
                if (inBounds(nx, ny) && !visited.count(index)) {
                    visited.insert(index);
                    q.push({nx, ny});
                }
            }
        }

        if (!placed) {
            cerr << "[Warning] Cannot place MBFF of size " 
                 << mbff->getW() << "x" << mbff->getH() << endl;
        }
    }
}

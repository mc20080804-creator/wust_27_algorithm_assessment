#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<cstdlib>
using namespace std;

struct State {
    int r, c;
    int g, f;
};

struct CompareState {
    bool operator()(const State& a, const State& b) const {
        return a.f > b.f;  // f 较大的排在后面
    }
};

int manhattan(int r, int c, int gr, int gc) {
    return abs(r - gr) + abs(c - gc);
}

int astar(const vector<string>& grid, int sr, int sc, int gr, int gc) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    const int INF = 1000000000;
    vector<vector<int>> best(rows, vector<int>(cols, INF));
    priority_queue<State, vector<State>, CompareState> pending;

    best[sr][sc] = 0;
    pending.push({sr, sc, 0, manhattan(sr, sc, gr, gc)});

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!pending.empty()) {
        State cur = pending.top();
        pending.pop();

        if (cur.g != best[cur.r][cur.c]) continue;
        if (cur.r == gr && cur.c == gc) return cur.g;

        for (int i = 0; i < 4; ++i) {
            int nr = cur.r + dr[i];
            int nc = cur.c + dc[i];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;

            char cell = grid[nr][nc];
            if (cell == '#') continue;
            int step = (cell >= '2' && cell <= '9') ? cell - '0' : 1;
            int new_g = cur.g + step;

            if (new_g < best[nr][nc]) {
                best[nr][nc] = new_g;
                pending.push({nr, nc, new_g, new_g + manhattan(nr, nc, gr, gc)});
            }
        }
    }

    return -1;
}

int dfs(int r, int c, const vector<string>& grid, vector<vector<bool>>& visited)
{
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    // 先判断有没有越界，再访问数组。
    if (r < 0 || r >= rows || c < 0 || c >= cols)
    {
        return 0;
    }
    if (grid[r][c] == '#' || visited[r][c])
    {
        return 0;
    }

    visited[r][c] = true;
    int count = 1;

    // 上、下、左、右的坐标变化。
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i)
    {
        count += dfs(r + dr[i], c + dc[i], grid, visited);
    }

    return count;
}

int main() 
{
    int rows, cols;
    cin >> rows >> cols;

    vector<string> grid(rows);
    int sr = -1, sc = -1;
    int gr = -1, gc = -1;

    for (int r = 0; r < rows; ++r) 
    {
        cin >> grid[r];
        for (int c = 0; c < cols; ++c)
        {
            if (grid[r][c] == 'S')
            {
                sr = r;
                sc = c;
            }
            else if (grid[r][c] == 'G')
            {
                gr = r;
                gc = c;
            }
        }
    }

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    int reachable_cells = dfs(sr, sc, grid, visited);
    bool goal_reachable = visited[gr][gc];
    cout << "reachable_cells: " << reachable_cells << '\n';

    int regions = 1;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != '#' && !visited[r][c]) {
                ++regions;
                dfs(r, c, grid, visited);
            }
        }
    }

    cout << "regions: " << regions << '\n';
    int cost = -1;
    if (goal_reachable) {
        cost = astar(grid, sr, sc, gr, gc);
    }
    cout << "cost: " << cost << '\n';
}
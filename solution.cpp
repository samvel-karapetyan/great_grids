#include <iostream>
#include <vector>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;
        
        int total = (n - 1) + (m - 1);
        vector<vector<pair<int, int>>> adj(total);
        
        for (int i = 0; i < k; i++) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            
            int rNode = x1 - 1;
            int cNode, edgeType;
            
            if (y2 == y1 + 1) {
                cNode = (n - 1) + (y1 - 1);
                edgeType = 1;
            } else {
                cNode = (n - 1) + (y1 - 2);
                edgeType = 0;
            }
            
            adj[rNode].push_back({cNode, edgeType});
            adj[cNode].push_back({rNode, edgeType});
        }
        
        vector<int> color(total, -1);
        bool possible = true;
        
        for (int start = 0; start < total && possible; start++) {
            if (color[start] != -1) continue;
            
            vector<int> q;
            q.push_back(start);
            color[start] = 0;
            
            for (size_t i = 0; i < q.size() && possible; i++) {
                int u = q[i];
                for (auto& [v, type] : adj[u]) {
                    int expected = (type == 0) ? color[u] : (1 - color[u]);
                    if (color[v] == -1) {
                        color[v] = expected;
                        q.push_back(v);
                    } else if (color[v] != expected) {
                        possible = false;
                        break;
                    }
                }
            }
        }
        
        cout << (possible ? "YES" : "NO") << "\n";
    }
    
    return 0;
}

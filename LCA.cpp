
struct LCA {
    struct Edge {
        int to, w;
    };

    int n, LOG;
    vector<vector<Edge>> adj;
    vector<vector<int>> up, val;
    vector<int> depth, dis;

    LCA(int n) : n(n), LOG(20) {
        adj.resize(n + 1);
        up.assign(n + 1, vector<int>(LOG));
        val.assign(n + 1, vector<int>(LOG));
        depth.assign(n + 1, 0);
        dis.assign(n + 1, 0);
    }

    int merge(int a, int b) {
        return max(a, b);
    }

    void add_edge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void dfs(int u, int p, int w) {
        up[u][0] = p;
        val[u][0] = w;

        for (auto [v, cost] : adj[u]) {
            if (v == p) continue;

            depth[v] = depth[u] + 1;
            dis[v] = dis[u] + cost;

            dfs(v, u, cost);
        }
    }

    void build(int root = 1) {
        dfs(root, root, LLONG_MIN);

        for (int j = 1; j < LOG; j++) {
            for (int i = 1; i <= n; i++) {
                up[i][j] = up[up[i][j - 1]][j - 1];

                val[i][j] = merge(
                    val[i][j - 1],
                    val[up[i][j - 1]][j - 1]
                );
            }
        }
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);

        int diff = depth[u] - depth[v];

        for (int j = LOG - 1; j >= 0; j--) {
            if (diff & (1LL << j))
                u = up[u][j];
        }

        if (u == v) return u;

        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }

        return up[u][0];
    }

    int kth_ancestor(int u, int k) {
        for (int j = LOG - 1; j >= 0; j--) {
            if (k & (1LL << j))
                u = up[u][j];
        }

        return u;
    }

    int get_node_on_path(int A, int B, int p, int k) {
        int distA = depth[A] - depth[p];

        if (k <= distA)
            return kth_ancestor(A, k);

        int distB = depth[B] - depth[p];
        int remaining = distA + distB - k;

        return kth_ancestor(B, remaining);
    }

    int dist(int u, int v) {
        int p = lca(u, v);
        return dis[u] + dis[v] - 2 * dis[p];
    }

    int query(int u, int v) {
        int ans = LLONG_MIN;

        if (depth[u] < depth[v])
            swap(u, v);

        int diff = depth[u] - depth[v];

        for (int j = LOG - 1; j >= 0; j--) {
            if (diff & (1LL << j)) {
                ans = merge(ans, val[u][j]);
                u = up[u][j];
            }
        }

        if (u == v) return ans;

        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                ans = merge(ans, val[u][j]);
                ans = merge(ans, val[v][j]);

                u = up[u][j];
                v = up[v][j];
            }
        }

        ans = merge(ans, val[u][0]);
        ans = merge(ans, val[v][0]);

        return ans;
    }
};

#include <iostream>
#include <cstdint>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>

using Vertex = int64_t;

class Graph {
protected:
    Vertex vertex_size;

public:
    const Vertex& Get_vertex_size() const {
        return vertex_size;
    }
    Graph() {
        vertex_size = 0;
    }

    virtual void Add_edge(Vertex first, Vertex second) = 0;
    virtual std::unordered_multiset<Vertex> Get_neighbors(const Vertex& vertex) const = 0;

};

class Graph_list : public Graph {
private:
    std::vector<std::unordered_multiset<Vertex>> list;

public:
    explicit Graph_list(const Vertex& size) {
        list.resize(size);
        vertex_size = size;
    }

    std::unordered_multiset<Vertex> Get_neighbors(const Vertex& ver) const override {
        return list[ver];
    }

    void Add_edge(Vertex first, Vertex second) override {
        list[--first].insert(--second);
        list[second].insert(first);
    }
    int64_t Segment_size(Vertex first, Vertex second) {
        return list[first].count(second);
    }
};

void dfs (Graph_list& G, Vertex v, std::vector<int64_t>& used, std::set<Vertex>& answer, int64_t timer, std::vector<int64_t>& tin, std::vector<int64_t>& fup, std::map<std::pair<Vertex, Vertex>, int64_t>& segment, int64_t p = -1) {

    used[v] = 1;
    tin[v] = fup[v] = timer++;
    const auto neighbors = G.Get_neighbors(v);
    for (auto elem: neighbors) {
        if (elem == p) {
            continue;
        }
        if (used[elem] == 1) {
            fup[v] = std::min(fup[v], tin[elem]);
        }
        else {
            dfs(G, elem, used, answer, timer, tin, fup, segment, v);
            fup[v] = std::min(fup[v], fup[elem]);
            if (fup[elem] > tin[v]) {
                if (G.Segment_size(v, elem) == 1) {
                    std::pair<Vertex, Vertex> points(v + 1, elem + 1);
                    answer.insert(segment[points]);
                }
            }
        }
    }
}

std::set<Vertex> find_bridges(Graph_list& G, std::map<std::pair<Vertex, Vertex>, int64_t>& segment) {

    std::vector<int64_t> used(G.Get_vertex_size(), 0);
    std::vector<int64_t> tin(G.Get_vertex_size(), 0);
    std::vector<int64_t> fup(G.Get_vertex_size(), 0);
    std::set<Vertex> answer;

    for (Vertex i = 0; i < G.Get_vertex_size(); ++i) {
        if (used[i] == 0) {
            int64_t timer = 0;
            dfs(G, i, used, answer, timer, tin, fup, segment);
        }
    }
    return answer;
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    Vertex n, m;

    std::cin >> n >> m;

    Graph_list graph(n);

    std::map<std::pair<Vertex, Vertex>, int64_t> segment;

    for (int64_t i = 1; i <= m; ++i) {
        Vertex first, second;
        std::cin >> first >> second;
        graph.Add_edge(first, second);
        std::pair<Vertex, Vertex> points1(first, second);
        std::pair<Vertex, Vertex> points2(second, first);
        segment[points1] = i;
        segment[points2] = i;
    }

    auto answer = find_bridges(graph, segment);

    std::cout << answer.size() << std::endl;
    for (auto elem: answer) {
        std::cout << elem << " " << std::endl;
    }
    return 0;
}
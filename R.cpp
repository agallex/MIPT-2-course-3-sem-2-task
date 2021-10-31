#include <iostream>
#include <cstdint>
#include <vector>
#include <set>

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
    virtual std::vector<Vertex> Get_neighbors(const Vertex& vertex) const = 0;

};

class Graph_with_matrix : public Graph {
    std::vector<std::vector<Vertex>> matrix;

public:
    explicit Graph_with_matrix(const Vertex& size) {
        for (Vertex i = 0; i < size; ++i) {
            std::vector<Vertex> tmp(size, 0);
            matrix.push_back(tmp);
        }
        vertex_size = size;
    }

    std::vector<Vertex> Get_neighbors(const Vertex& ver) const override {
        std::vector<Vertex> answer;
        for (Vertex i = 0; i < Get_vertex_size(); ++i) {
            if (matrix[ver][i] != 0) {
                answer.push_back(i);
            }
        }
        return answer;
    }

    void Add_edge(Vertex first, Vertex second) override {
        matrix[--first][--second] = 1;
        matrix[second][first] = 1;
    }
};

class Graph_list : public Graph {
private:
    std::vector<std::vector<Vertex>> list;

public:
    explicit Graph_list(const Vertex& size) {
        list.resize(size);
        vertex_size = size;
    }

    std::vector<Vertex> Get_neighbors(const Vertex& ver) const override {
        return list.at(ver);
    }

    void Add_edge(Vertex first, Vertex second) override {
        list[--first].push_back(--second);
        list[second].push_back(first);
    }
};

class Oriented_Graph {
protected:
    Vertex vertex_size;

public:
    const Vertex& Get_vertex_size() const {
        return vertex_size;
    }
    Oriented_Graph() {
        vertex_size = 0;
    }

    virtual void Add_edge(Vertex first, Vertex second) = 0;
    virtual const std::vector<Vertex>& Get_neighbors(const Vertex& vertex) const = 0;

};

class Oriented_Graph_list : public Oriented_Graph {
private:
    std::vector<std::vector<Vertex>> list;

public:
    explicit Oriented_Graph_list(const Vertex& size) {
        list.resize(size);
        vertex_size = size;
    }

    const std::vector<Vertex>& Get_neighbors(const Vertex& ver) const override {
        return list.at(ver);
    }

    void Add_edge(Vertex first, Vertex second) override {
        list[--first].push_back(--second);
    }
};

void dfs (Graph_list& G, Vertex v, std::vector<int64_t>& used, std::set<Vertex>& answer, int64_t timer, std::vector<int64_t>& tin, std::vector<int64_t>& fup, int64_t p = -1) {

    used[v] = 1;
    tin[v] = fup[v] = timer++;
    int64_t children = 0;
    const auto neighbors = G.Get_neighbors(v);
    for (auto elem: neighbors) {
        if (elem == p) {
            continue;
        }
        if (used[elem] == 1) {
            fup[v] = std::min(fup[v], tin[elem]);
        }
        else {
            dfs(G, elem, used, answer, timer, tin, fup, v);
            fup[v] = std::min(fup[v], fup[elem]);
            if (fup[elem] >= tin[v] && p != -1) {
                answer.insert(v);
            }
            ++children;
        }
    }
    if (p == -1 && children > 1) {
        answer.insert(v);
    }
}

std::set<Vertex> points_of_articulation(Graph_list& G) {

    std::vector<int64_t> used(G.Get_vertex_size(), 0);
    std::vector<int64_t> tin(G.Get_vertex_size(), 0);
    std::vector<int64_t> fup(G.Get_vertex_size(), 0);
    std::set<Vertex> answer;

    for (Vertex i = 0; i < G.Get_vertex_size(); ++i) {
        if (used[i] == 0) {
            int64_t timer = 0;
            dfs(G, i, used, answer, timer, tin, fup);
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

    for (int64_t i = 0; i < m; ++i) {
        Vertex first, second;
        std::cin >> first >> second;
        graph.Add_edge(first, second);
    }

    auto answer = points_of_articulation(graph);

    std::cout << answer.size() << std::endl;
    for (auto elem: answer) {
        std::cout << elem + 1 << " " << std::endl;
    }
    return 0;
}

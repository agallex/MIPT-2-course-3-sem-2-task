#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>

using Vertex = uint64_t;

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

std::vector<Vertex> Short_path(const Graph& G, Vertex begin, Vertex end) {
    --begin;
    --end;
    std::vector<uint64_t> distance(G.Get_vertex_size(), __LONG_MAX__);
    std::vector<Vertex> parent(G.Get_vertex_size(), __LONG_MAX__);
    std::queue<Vertex> queue;

    distance[begin] = 0;
    queue.push(begin);

    while (!queue.empty()) {

        Vertex v = queue.front();
        queue.pop();

        auto neighbors = G.Get_neighbors(v);
        for (auto& u: neighbors) {
            if (distance[u] == __LONG_MAX__) {
                distance[u] = distance[v] + 1;
                parent[u] = v;
                queue.push(u);
            }
        }
    }

    std::vector<Vertex> answer;
    if (distance[end] == __LONG_MAX__) {
        return answer;
    }

    for (Vertex v = end; v != begin; v = parent[v]) {
        answer.push_back(v);
    }
    answer.push_back(begin);

    for (uint64_t i = 0; i < answer.size() / 2; ++i) {
        std::swap(answer[i], answer[answer.size() - 1 - i]);
    }

    return answer;
}

int main() {
    Vertex n = 0;
    Vertex m = 0;
    std::cin >> n >> m;

    Vertex beg = 0;
    Vertex end = 0;
    std::cin >> beg >> end;

    Graph_list G(n);

    for (uint64_t i = 0; i < m; ++i) {
        Vertex first = 0;
        Vertex second = 0;
        std::cin >> first >> second;
        G.Add_edge(first, second);
    }

    auto ans = Short_path(G, beg, end);

    if (ans.size() == 0) {
        std::cout << -1;
        return 0;
    }

    std::cout << ans.size() - 1 << std::endl;

    for (auto& elem: ans) {
        std::cout << elem + 1 << ' ';
    }

    return 0;
}

#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>

using Vertex = uint32_t;

#define NO_EDGE 0
#define IS_EDGE 1

class Graph {
protected:
    size_t vertex_size;

public:
    const size_t& Get_vertex_size() const {
        return vertex_size;
    }
    Graph() {
        vertex_size = 0;
    }

    virtual void Add_edge(const Vertex& first, const Vertex& second) = 0;
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
        std::vector<Vertex> neighbors;
        for (Vertex i = 0; i < Get_vertex_size(); ++i) {
            if (matrix[ver][i] != NO_EDGE) {
                neighbors.push_back(i);
            }
        }
        return neighbors;
    }

    void Add_edge(const Vertex& first, const Vertex& second) override {
        matrix[first][second] = 1;
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

    void Add_edge(const Vertex& first, const Vertex& second) override {
        list[first].push_back(second);
        list[second].push_back(first);
    }
};

std::vector<Vertex> Short_path(const Graph& G, const Vertex& begin, const Vertex& end) {
    std::vector<uint32_t> distance(G.Get_vertex_size(), __INT_MAX__);
    std::vector<Vertex> parent(G.Get_vertex_size(), __INT_MAX__);
    std::queue<Vertex> queue;

    distance[begin] = 0;
    queue.push(begin);

    while (!queue.empty()) {

        Vertex top = queue.front();
        queue.pop();

        for (auto& elem: G.Get_neighbors(top)) {
            if (distance[elem] == __INT_MAX__) {
                distance[elem] = distance[top] + 1;
                parent[elem] = top;
                queue.push(elem);
            }
        }
    }

    std::vector<Vertex> short_path;
    if (distance[end] == __INT_MAX__) {
        return short_path;
    }

    for (Vertex v = end; v != begin; v = parent[v]) {
        short_path.push_back(v);
    }
    short_path.push_back(begin);

    std::reverse(short_path.begin(), short_path.end());

    return short_path;
}

int main() {
    Vertex vertex_size = 0;
    Vertex segment_size = 0;
    std::cin >> vertex_size >> segment_size;

    Vertex beg = 0;
    Vertex end = 0;
    std::cin >> beg >> end;

    Graph_list G(vertex_size);

    for (uint64_t i = 0; i < segment_size; ++i) {
        Vertex first = 0;
        Vertex second = 0;
        std::cin >> first >> second;
        G.Add_edge(--first, --second);
    }

    auto ans = Short_path(G, --beg, --end);

    if (ans.empty()) {
        std::cout << -1;
        return 0;
    }

    std::cout << ans.size() - 1 << std::endl;

    for (auto& elem: ans) {
        std::cout << elem + 1 << ' ';
    }

    return 0;
}

#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>

using Vertex = uint32_t;

#define NO_EDGE 0
#define IS_EDGE 1

enum COLORS { UNDEFINED = 0, COLOR1 = 1, COLOR2 = 2};

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

bool Bipartite_graph(const Graph& G) {

    std::vector<uint64_t> color(G.Get_vertex_size(), 0);

    for (uint64_t begin = 0; begin < G.Get_vertex_size(); ++begin) {
        if (color[begin] == UNDEFINED) {
            std::queue<Vertex> queue;
            color[begin] = COLOR1;
            queue.push(begin);

            while (!queue.empty()) {

                Vertex v = queue.front();
                queue.pop();

                for (auto &u: G.Get_neighbors(v)) {
                    if (color[u] == UNDEFINED) {
                        if (color[v] == COLOR1) {
                            color[u] = COLOR2;
                        } else {
                            color[u] = COLOR1;
                        }
                        queue.push(u);
                    } else {
                        if (color[u] == color[v]) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

int main() {
    Vertex vertex_size = 0;
    Vertex segment_size = 0;
    std::cin >> vertex_size >> segment_size;

    Graph_list G(vertex_size);

    for (uint64_t i = 0; i < segment_size; ++i) {
        Vertex first = 0;
        Vertex second = 0;
        std::cin >> first >> second;
        G.Add_edge(first, second);
    }

    if (Bipartite_graph(G)) {
        std::cout << "YES" << std::endl;
    }
    else {
        std::cout << "NO" << std::endl;
    }
}

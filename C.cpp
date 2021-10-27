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

std::vector<Vertex> Get_neighbors_horse(int64_t v, int64_t n) {
    std::vector<Vertex> answer;
    if (v / n - 1 >= 0 && v % n - 2 >= 0) {
        answer.push_back(v - n - 2);
    }
    if (v / n - 2 >= 0 && v % n - 1 >= 0) {
        answer.push_back(v - 2 * n - 1);
    }
    if (v / n - 1 >= 0 && v % n + 2 < n) {
        answer.push_back(v - n + 2);
    }
    if (v / n - 2 >= 0 && v % n + 1 < n) {
        answer.push_back(v - 2 * n + 1);
    }
    if (v / n + 1 < n && v % n - 2 >= 0) {
        answer.push_back(v + n - 2);
    }
    if (v / n + 2 < n && v % n - 1 >= 0) {
        answer.push_back(v + 2 * n - 1);
    }
    if (v / n + 1 < n && v % n + 2 < n) {
        answer.push_back(v + n + 2);
    }
    if (v / n + 2 < n && v % n + 1 < n) {
        answer.push_back(v + 2 * n + 1);
    }
    return answer;
}

std::vector<Vertex> Short_path_horse(const Graph& G, size_t n, Vertex begin, Vertex end) {
    std::vector<uint64_t> distance(G.Get_vertex_size(), __LONG_MAX__);
    std::vector<Vertex> parent(G.Get_vertex_size(), __LONG_MAX__);
    std::queue<Vertex> queue;

    distance[begin] = 0;
    queue.push(begin);

    while (!queue.empty()) {

        Vertex v = queue.front();
        queue.pop();

        auto neighbors = Get_neighbors_horse(v, n);
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

bool Bipartite_graph(const Graph& G) {

    std::vector<uint64_t> color(G.Get_vertex_size(), 0);

    for (uint64_t begin = 0; begin < G.Get_vertex_size(); ++begin) {
        if (color[begin] == 0) {
            std::queue<Vertex> queue;
            color[begin] = 1;
            queue.push(begin);

            while (!queue.empty()) {

                Vertex v = queue.front();
                queue.pop();

                for (auto &u: G.Get_neighbors(v)) {
                    if (color[u] == 0) {
                        if (color[v] == 1) {
                            color[u] = 2;
                        } else {
                            color[u] = 1;
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
    Vertex n = 0;
    std::cin >> n;
    Vertex x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    --x1;
    --y1;
    --x2;
    --y2;
    Graph_list G(n * n);

    auto ans = Short_path_horse(G, n, x1 * n + y1, x2 * n + y2);

    std::cout << ans.size() - 1 << std::endl;

    for (auto& elem: ans) {
        std::cout << elem / n + 1 << ' ' << elem % n + 1 << std::endl;
    }

    return 0;
}

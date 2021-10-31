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

std::vector<Vertex> Get_neighbors_digit(int64_t v) {
    std::vector<Vertex> answer;
    if (v / 1000 < 9) {
        answer.push_back(v + 1000);

    }
    if (v % 10 != 1) {
        answer.push_back(v - 1);
    }
    answer.push_back(v % 10 * 1000 + v / 1000 * 100 + v / 100 % 10 * 10 + v / 10 % 10);
    answer.push_back(v % 10 * 10 + v / 1000 + v / 100 % 10 * 1000 + v / 10 % 10 * 100);

    return answer;
}

std::vector<Vertex> Short_path_digit(const Graph& G, Vertex begin, Vertex end) {
    std::vector<uint64_t> distance(G.Get_vertex_size(), __LONG_MAX__);
    std::vector<Vertex> parent(G.Get_vertex_size(), __LONG_MAX__);
    std::queue<Vertex> queue;

    distance[begin] = 0;
    queue.push(begin);

    while (!queue.empty()) {

        Vertex v = queue.front();
        queue.pop();

        auto neighbors = Get_neighbors_digit(v);
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

std::vector<Vertex> Get_neighbors_sub(int64_t v, int64_t n, int64_t m) {
    std::vector<Vertex> answer;
    if (v / m - 1 >= 0) {
        answer.push_back(v - m);
    }
    if (v / m + 1 < n) {
        answer.push_back(v + m);
    }
    if (v % m - 1 >= 0) {
        answer.push_back(v - 1);
    }
    if (v % m + 1 < m) {
        answer.push_back(v + 1);
    }
    return answer;
}

std::vector<Vertex>  Short_path_sub(const std::vector<Vertex>& sub, int64_t n, int64_t m) {

    std::vector<Vertex> distance(n * m, __LONG_MAX__);
    std::queue<Vertex> queue;

    for (auto elem: sub) {
        distance[elem] = 0;
        queue.push(elem);
    }

    while (!queue.empty()) {

        Vertex v = queue.front();
        queue.pop();

        auto neighbors = Get_neighbors_sub(v, n, m);
        for (auto &u: neighbors) {
            if (distance[u] == __LONG_MAX__) {
                distance[u] = distance[v] + 1;
                queue.push(u);
            }
        }
    }
    return distance;
}

std::vector<Vertex> Get_neighbors_car(int64_t v, std::vector<char>& track, int64_t n, int64_t m) {
    std::vector<Vertex> answer;
    int64_t i = v;
    int64_t len = 0;
    while (i < n * m && track[i] != '#') {
        i += m;
        ++len;
    }
    if (len != 1) {
        answer.push_back(v + len / 2 * m);
    }
    i = v;
    len = 0;
    while (i >= 0 && track[i] != '#') {
        i -= m;
        ++len;
    }
    if (len != 1) {
        answer.push_back(v - len / 2 * m);
    }
    i = v;
    len = 0;
    while (track[i] != '#') {
        ++len;
        if (i % m + 1 == m) {
            break;
        }
        ++i;
    }
    if (len != 1) {
        answer.push_back(v + len / 2);
    }
    i = v;
    len = 0;
    while (track[i] != '#') {
        ++len;
        if (i % m - 1 == -1) {
            break;
        }
        --i;
    }
    if (len != 1) {
        answer.push_back(v - len / 2);
    }
    return answer;
}

int64_t Short_path_car(std::vector<char>& track, size_t n, size_t m, Vertex begin, Vertex end) {

    std::vector<uint64_t> distance(n * m, __LONG_MAX__);
    std::queue<Vertex> queue;

    distance[begin] = 0;
    queue.push(begin);

    while (!queue.empty()) {

        Vertex v = queue.front();
        queue.pop();

        auto neighbors = Get_neighbors_car(v, track, n, m);
        for (auto& u: neighbors) {
            if (distance[u] == __LONG_MAX__) {
                distance[u] = distance[v] + 1;
                queue.push(u);
            }
        }
    }

    if (distance[end] == __LONG_MAX__) {
        return -1;
    }

    return distance[end];
}

int main() {
    int64_t n, m;
    std::cin >> n >> m;

    std::vector<char> track;

    int64_t begin = 0;
    int64_t end = 0;

    for (int64_t i = 0; i < n * m; ++i) {
        char in;
        std::cin >> in;
        if (in == 'S') {
            begin = i;
        }
        if (in == 'T') {
            end = i;
        }
        track.push_back(in);
    }

    std::cout << Short_path_car(track, n, m, begin, end) << std::endl;

    return 0;
}

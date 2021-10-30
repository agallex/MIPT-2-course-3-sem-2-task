#include <iostream>
#include <cstdint>
#include <vector>

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
    virtual std::vector<Vertex> Get_neighbors(const Vertex& vertex) const = 0;

};

class Oriented_Graph_with_matrix : public Oriented_Graph {
    std::vector<std::vector<Vertex>> matrix;

public:
    explicit Oriented_Graph_with_matrix(const Vertex& size) {
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
    }
};

class Oriented_Graph_list : public Oriented_Graph {
private:
    std::vector<std::vector<Vertex>> list;

public:
    explicit Oriented_Graph_list(const Vertex& size) {
        list.resize(size);
        vertex_size = size;
    }

    std::vector<Vertex> Get_neighbors(const Vertex& ver) const override {
        return list.at(ver);
    }

    void Add_edge(Vertex first, Vertex second) override {
        list[--first].push_back(--second);
    }
};

void is_cycle_impl(const Oriented_Graph& G, const Vertex& v, std::vector<int64_t>& used, std::vector<Vertex>& path, std::vector<Vertex>& answer) {
    auto neighbors = G.Get_neighbors(v);
    for (auto elem: neighbors) {
        if (used[elem] == 0) {
            path[elem] = v;
            used[elem] = 1;
            is_cycle_impl(G, elem, used, path, answer);
            used[elem] = 2;
            if (!answer.empty()) {
                return;
            }
        }
        if (used[elem] == 1) {
            for (Vertex i = v; i != elem; i = path[i]) {
                answer.push_back(i);
            }

            answer.push_back(elem);

            for (uint64_t i = 0; i < answer.size() / 2; ++i) {
                std::swap(answer[i], answer[answer.size() - 1 - i]);
            }
            return;
        }
    }
}
std::vector<Vertex> is_cycle(const Oriented_Graph& G) {

    std::vector<int64_t> used(G.Get_vertex_size(), 0);
    std::vector<Vertex> path(G.Get_vertex_size(), 0);
    std::vector<Vertex> answer;
    for (Vertex i = 0; i < G.Get_vertex_size(); ++i) {
        if (used[i] == 0) {
            used[i] = 1;
            is_cycle_impl(G, i, used, path, answer);
            used[i] = 2;
            if (!answer.empty()) {
                return answer;
            }
        }
    }
    return answer;
}

void dfs(const Oriented_Graph_list& G, const Vertex& v, std::vector<int64_t>& used, std::vector<Vertex>& answer) {
    used[v] = 1;
    auto neighbors = G.Get_neighbors(v);
    for (auto elem: neighbors) {
        if (used[elem] == 0) {
            dfs(G, elem, used, answer);
        }
    }
    answer.push_back(v);
}

std::vector<Vertex> sort(const Oriented_Graph_list& G) {

    std::vector<int64_t> used(G.Get_vertex_size(), 0);
    std::vector<Vertex> answer;
    for (Vertex i = 0; i < G.Get_vertex_size(); ++i) {
        if (used[i] == 0) {
            used[i] = 1;
            dfs(G, i, used, answer);
        }
    }

    for (uint64_t i = 0; i < answer.size() / 2; ++i) {
        std::swap(answer[i], answer[answer.size() - 1 - i]);
    }

    return answer;
}

int main() {

    Vertex n, m;

    std::cin >> n >> m;

    Oriented_Graph_list graph(n);

    for (Vertex i = 0; i < m; ++i) {

        Vertex first, second;
        std::cin >> first >> second;
        graph.Add_edge(first, second);

    }

    if (!is_cycle(graph).empty()) {
        std::cout << -1 << std::endl;
    }
    else {
        auto answer = sort(graph);

        for (auto elem: answer) {
            std::cout << elem + 1 << " ";
        }
    }

    return 0;
}

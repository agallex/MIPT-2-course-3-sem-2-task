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

void dfs(const Graph& G, const Vertex& v, std::vector<int64_t>& used, std::vector<Vertex>& path) {
    auto neighbors = G.Get_neighbors(v);
    for (auto elem: neighbors) {
        if (used[elem] == 0) {
            used[elem] = 1;
            dfs(G, elem, used, path);
            path.push_back(elem);
        }
    }
}

std::vector<std::vector<Vertex>> chat(const Graph& G) {

    std::vector<int64_t> used(G.Get_vertex_size(), 0);
    std::vector<std::vector<Vertex>> answer;
    for (Vertex i = 0; i < G.Get_vertex_size(); ++i) {
        if (used[i] == 0) {
            std::vector<Vertex> path;
            path.push_back(i);
            used[i] = 1;
            dfs(G, i, used, path);
            answer.push_back(path);
        }
    }
    return answer;
}

int main() {

    Vertex n, m;

    std::cin >> n >> m;

    Graph_list graph(n);

    for (Vertex i = 0; i < m; ++i) {

        Vertex first, second;
        std::cin >> first >> second;
        graph.Add_edge(first, second);

    }

    auto answer = chat(graph);

    std::cout << answer.size() << std::endl;
    for (auto vector: answer) {
        std::cout << vector.size() << std::endl;
        for (auto elem: vector) {
            std::cout << elem + 1 << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

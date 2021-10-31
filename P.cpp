#include <iostream>
#include <cstdint>
#include <vector>

using Vertex = int64_t;

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


void dfs1(const Oriented_Graph_list& graph, int64_t v, std::vector<int64_t>& used, std::vector<int64_t>& order) {
    used[v] = 1;
    auto neighbors = graph.Get_neighbors(v);
    for (auto elem: neighbors) {
        if (used[elem] == 0) {
            dfs1(graph, elem, used, order);
        }
    }
    order.push_back(v);
}

void dfs2(const Oriented_Graph_list& transpose_graph, int64_t v, std::vector<int64_t>& used, std::vector<int64_t>& component) {
    used[v] = true;
    component.push_back(v);

    auto neighbors = transpose_graph.Get_neighbors(v);
    for (auto elem: neighbors) {
        if (used[elem] == 0) {
            dfs2(transpose_graph, elem, used, component);
        }
    }
}

std::vector<std::vector<Vertex>> strong_components(const Oriented_Graph_list& graph, const Oriented_Graph_list& transpose_graph) {
    std::vector<int64_t> used(graph.Get_vertex_size(), 0);
    std::vector<Vertex> order;
    std::vector<Vertex> component;
    std::vector<std::vector<Vertex>> answer;
    int64_t n = graph.Get_vertex_size();

    for (int64_t i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs1(graph, i, used, order);
        }
    }
    used.assign(n, 0);
    for (int64_t i = 0; i < n; ++i) {
        int v = order[n - 1 - i];
        if (!used[v]) {
            dfs1(transpose_graph, v, used, component);
            answer.push_back(component);
            component.clear();
        }
    }
    return answer;
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    Vertex n, m;

    std::cin >> n >> m;

    Oriented_Graph_list graph(n);
    Oriented_Graph_list transpose_graph(n);

    for (int64_t i = 0; i < m; ++i) {
        Vertex first, second;
        std::cin >> first >> second;
        graph.Add_edge(first, second);
        transpose_graph.Add_edge(second, first);
    }

    auto components = strong_components(graph, transpose_graph);

    std::vector<Vertex> answer(n, 0);
    for (int64_t i = 0; i < components.size(); ++i) {
        for (auto elem: components[i]) {
            answer[elem] = i + 1;
        }
    }

    std::cout << components.size() << std::endl;
    for (auto elem: answer) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;

    return 0;
}


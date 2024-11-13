#pragma once
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>


class Graph
{
public:
    using Entry = void *;
    using Key = std::size_t;
    struct Node {
        Entry *entry;
        Key id;
    };
public:
    virtual void add(Node) = 0; // adds a node
    /* Adds an edge.
    If node not in Graph, insert it. */
    virtual void add(std::pair<Node, Key>) = 0;

    template <typename... Args>
    void add(Node, Args...); // adds multiple nodes
    template <typename... Args>
    void add(std::pair<Node, Key>, Args...); // adds multiple edges

    virtual Entry remove(Key) = 0; // removes a node
    virtual Entry remove(std::pair<Key, Key>) = 0; // removes an edge

    template <typename... Args>
    void remove_all(Key, Args...);

    template <typename... Args>
    void remove_all(std::pair<Key, Key>, Args...);
public:
    friend std::ifstream& operator>>(std::ifstream& stream, Graph& g);
    friend std::ostream& operator<<(std::ostream& stream, Graph& g);
public:
    inline std::size_t m_edges() { return m; }
    inline std::size_t n_nodes() { return n; }
private:
    std::size_t m = 0, n = 0;
    std::string name;
};


class DynamicGraph : public Graph
{
private:
    struct List {
        Node node;
        std::vector<Key> edges;
    };
    //using List = Node*;
public:
    void add(Node) override;
    void add(std::pair<Node, Key>) override;

    Entry remove(Key) override;
    Entry remove(std::pair<Key, Key>) override;

    DynamicGraph(std::size_t _m, std::size_t n)
    {
        adj_list.reserve(n);
    }
private:
    std::vector<List> adj_list; // adjacency list
};


template <std::size_t M, std::size_t N>
class StaticGraph : public Graph
{
private:
    using Entries = Node[N];
public:
    void add(Node) override;
    void add(std::pair<Node, Key>) override;

    Entry remove(Key) override;
    Entry remove(std::pair<Key, Key>) override;
public:
    Entries nodes;
    Key adj_mat[M * N]; // adjacency matrix
};

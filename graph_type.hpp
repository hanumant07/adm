#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#pragma once

//**************************************************************************************************
// Types
//**************************************************************************************************
typedef enum {
  kGraphErrorSuccess = 0,
  kGraphErrorNoMem = -1,
  kGraphErrorBadArgs = -2,
  kGraphErrorSearchAbort = -3,
  kGraphErrorNoPath = -4,
} GraphError;

//**************************************************************************************************
// Base class for vertex type
//**************************************************************************************************
class Vertex {
public:
  // Constructors
  Vertex(int i) : id(i) {}
  Vertex(int i, int w) : id(i), weight(w) {}
  int getWeight() { return weight; }
  friend std::ostream &operator<<(std::ostream &os, const Vertex &v) {
    os << v.id;
    return os;
  }
  // return id
  int getId() const { return id; }

protected:
  // Weight of vertex
  int weight;
  // Id of vertex
  int id;
};

//**************************************************************************************************
// Graph Edge
//**************************************************************************************************
class Edge {
public:
  int getWeight() { return weight; }
  const Vertex *getVertex() const { return end; };
  bool operator<(const Edge &e) { return weight < e.weight; }

private:
  Edge() {}
  Edge(int w, const Vertex *v) : weight(w), end(v) {}
  Edge(Edge &e) {
    weight = e.getWeight();
    end = e.getVertex();
  }
  // The adjacent vertex
  const Vertex *end;
  // Weight if any of edge
  int weight;
  friend class Graph;
};

// List of edges and vertices
typedef std::vector<const Edge *> EdgeList;
typedef std::unordered_map<int, const Vertex *> VertexList;

//**************************************************************************************************
// Graph class
//**************************************************************************************************
class Graph {
public:
  int V() { return num_nodes; }
  int E() { return num_edges; }
  friend class VertexListIterator;
  friend class EdgeListIterator;
  // Iterator to iterate through vertices in a graph
  class VertexListIterator {
  public:
    VertexListIterator(Graph &G) : g(G) {}
    // Iterator operations
    VertexListIterator &begin() {
      v_it = g.vertex_list.begin();
      return *this;
    }
    bool end() { return v_it == g.vertex_list.end(); }
    VertexListIterator &operator++() {
      if (v_it == g.vertex_list.end()) {
        return *this;
      }
      ++v_it;
      return *this;
    }
    // get vertex at iterator position
    const Vertex *getVertex() {
      if (v_it == g.vertex_list.end()) {
        throw std::range_error("Attempt to deref empty vertex list");
      }
      return v_it->second;
    }
    VertexListIterator &operator=(VertexListIterator &v_it) { return v_it; }

  private:
    friend class Graph;
    VertexList::iterator v_it;
    // Make constructors private
    Graph &g;
  };
  // Iterator to iterate through all the edges from a given source vertex in a
  // graph
  class EdgeListIterator {
  public:
    EdgeListIterator(Graph &G, const Vertex *v) : g(G), source(v) {
      if (!v || !g.validVertex(v)) {
        throw std::invalid_argument("Invalid vertex");
      }
    }
    // Get the begining iterator
    EdgeListIterator &begin() {
      e_it = g.adj_list[source].begin();
      return *this;
    }
    // Get the end of iteration
    bool end() { return g.adj_list[source].end() == e_it; }
    // Move iterator to next position
    EdgeListIterator &operator++() {
      if (e_it == g.adj_list[source].end()) {
        return *this;
      }
      ++e_it;
      return *this;
    }
    // Get the edge at current iterator position
    const Edge *getEdge() {
      if (e_it == g.adj_list[source].end()) {
        throw std::range_error("Attempt to deref empty edge list");
      }
      return *e_it;
    }

  private:
    friend class Graph;
    // Make constructors private to allow only Graph to create iterator
    // instances
    EdgeListIterator(Graph &G) : g(G) {}
    // tracking within edgelist
    std::vector<const Edge *>::iterator e_it;
    // The source vertex for which edgelist iterator is created
    const Vertex *source;
    // The graph for which edge list iterator is constructed
    Graph &g;
  };
  // Graph Constructor
  Graph(int n, bool d)
      : num_nodes(n), directed(d), edge_list_end(*this),
        vertex_list_end(*this) {}
  // Insert Edge variations
  GraphError InsertEdge(const Vertex *u, const Vertex *v, int weight);
  GraphError InsertEdge(const Vertex *u, const Vertex *v);
  GraphError InsertEdge(int v, int u, int weight);
  // Check if vertex is present in graph
  bool validVertex(const Vertex *v) {
    if (!v)
      return false;
    return vertex_list.find(v->getId()) != vertex_list.end();
  }
  bool isDirected() { return directed; }

private:
  // number of nodes in the graph
  int num_nodes;
  // number of edges in the graph
  int num_edges;
  // are edges directed
  bool directed;
  // list of vertices in the graph
  VertexList vertex_list;
  // vertex degree of individual intervices
  std::unordered_map<const Vertex *, int> vertex_degree;
  // adjacency list of vertices in the graph
  std::unordered_map<const Vertex *, EdgeList> adj_list;
  // end of edgelist iteration
  EdgeListIterator edge_list_end;
  // end of vertex list iteration
  VertexListIterator vertex_list_end;
};

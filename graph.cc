#include "graph_type.hpp"

//**************************************************************************************************
// Insert an edge given end point ids and weight
//**************************************************************************************************
GraphError Graph::InsertEdge(int v, int u, int weight) {
  Vertex *vertex_v = new Vertex(v);
  Vertex *vertex_u = new Vertex(u);

  if (!vertex_v || !vertex_u) {
    return kGraphErrorNoMem;
  }
  return InsertEdge(vertex_v, vertex_u, weight);
}

//**************************************************************************************************
// Insert an edge given 2 end point ids
//**************************************************************************************************
GraphError Graph::InsertEdge(const Vertex *v, const Vertex *u) {

  if (!v || !u) {
    return kGraphErrorNoMem;
  }

  return InsertEdge(v, u, 0);
}

//**************************************************************************************************
// Insert an edge in the graph
//**************************************************************************************************
GraphError Graph::InsertEdge(const Vertex *u, const Vertex *v, int weight) {

  if (!v || !u) {
    return kGraphErrorBadArgs;
  }

  vertex_list.insert(v);
  vertex_list.insert(u);

  adj_list[v].push_back(new Edge(0, u));

  if (!directed) {
    adj_list[u].push_back(new Edge(0, v));
  }

  vertex_degree[u]++;
  vertex_degree[v]++;
  return kGraphErrorSuccess;
}

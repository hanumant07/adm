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

  const Vertex *v1;
  const Vertex *v2;
  if (!v || !u || v->getId() >= num_nodes || u->getId() >= num_nodes) {
    return kGraphErrorBadArgs;
  }

  // Check if Vertex with that id already exists
  if (vertex_list.find(u->getId()) == vertex_list.end()) {
    vertex_list.insert(std::make_pair(u->getId(), u));
    v1 = u;
  } else {
    // Already exsts delete passed vertex
    v1 = vertex_list[u->getId()];
    delete u;
  }

  // Perform same check as above
  if (vertex_list.find(v->getId()) == vertex_list.end()) {
    vertex_list.insert(std::make_pair(v->getId(), v));
    v2 = v;
  } else {
    v2 = vertex_list[v->getId()];
    delete v;
  }

  adj_list[v1].push_back(new Edge(0, v2));
  num_edges++;
  vertex_degree[v1]++;

  if (!directed) {
    adj_list[v2].push_back(new Edge(0, v1));
    num_edges++;
    vertex_degree[v2]++;
  }

  return kGraphErrorSuccess;
}

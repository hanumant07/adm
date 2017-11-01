#include "bfs.h"
#include <iostream>

//**************************************************************************************************
// Construct BFS for a given a graph instance
//**************************************************************************************************
Bfs::Bfs(Graph &G) : g(G) {
  Graph::VertexListIterator v_it(G);
  const Vertex *curr_vertex;

  for (v_it.begin(); !v_it.end(); ++v_it) {
    curr_vertex = v_it.getVertex();
    discovered.insert(std::make_pair(curr_vertex, false));
    processed.insert(std::make_pair(curr_vertex, false));
    parent.insert(std::make_pair(curr_vertex, curr_vertex));
  }
  this->terminate = false;
}

//**************************************************************************************************
// Destructor
//**************************************************************************************************
Bfs::~Bfs() {
  discovered.clear();
  processed.clear();
  parent.clear();
  while (!search_queue.empty()) {
    search_queue.pop();
  }
}

//**************************************************************************************************
// Perform Search
//**************************************************************************************************
GraphError Bfs::PerformSearch() {
  GraphError err;
  const Vertex *curr_vertex;
  Graph::VertexListIterator v_it(g);

  for (v_it.begin(); !v_it.end(); ++v_it) {
    curr_vertex = v_it.getVertex();
    if (terminate) {
      err = kGraphErrorSearchAbort;
      break;
    }
    if (!discovered[curr_vertex]) {

      err = PerformSearch(curr_vertex);
      if (err != kGraphErrorSuccess) {
        break;
      }
    }
  }

  while (!search_queue.empty()) {
    search_queue.pop();
  }
  return err;
}

//**************************************************************************************************
// Perform Search with starting vertex
//**************************************************************************************************
GraphError Bfs::PerformSearch(const Vertex *start_vertex) {

  GraphError ret;
  const Vertex *curr_vertex;

  if (!start_vertex || !g.validVertex(start_vertex)) {
    return kGraphErrorBadArgs;
  }

  // empty search queue at start of search
  while (!search_queue.empty()) {
    search_queue.pop();
  }

  discovered[start_vertex] = true;
  this->search_queue.push(start_vertex);

  while (!this->search_queue.empty() && !terminate) {
    curr_vertex = this->search_queue.front();
    this->search_queue.pop();

    ProcessVertexEarly(curr_vertex);

    Graph::EdgeListIterator e_it(g, curr_vertex);
    const Edge *e;
    processed[curr_vertex] = true;

    for (e_it.begin(); !e_it.end(); ++e_it) {
      e = e_it.getEdge();
      const Vertex *neighbor = e->getVertex();

      if (!processed[neighbor] || g.isDirected()) {
        ProcessEdge(curr_vertex, neighbor);
      }
      if (!discovered[neighbor]) {
        discovered[neighbor] = true;
        parent[neighbor] = curr_vertex;
        // Fixme catch exceptions
        this->search_queue.push(neighbor);
      }
    }
    ProcessVertexLate(curr_vertex);
  }
  return kGraphErrorSuccess;
}

//**************************************************************************************************
// retrieve path from given start to destination. Call after PerformSearch().
// if no path found, return kGraphErrorNoPath
//**************************************************************************************************

GraphError Bfs::GetPathFromTo(const Vertex *from, const Vertex *to,
                              std::list<const Vertex *> &out_path) {

  if (!from || !g.validVertex(from) || !to || !g.validVertex(to)) {
    return kGraphErrorBadArgs;
  }

  if (parent.empty()) {
    return kGraphErrorNoPath;
  }

  if (from == to) {
    out_path.push_front(from);
    return kGraphErrorSuccess;
  }

  if (parent[to] == to) {
    return kGraphErrorNoPath;
  }

  out_path.push_front(to);
  return GetPathFromTo(from, parent[to], out_path);
}

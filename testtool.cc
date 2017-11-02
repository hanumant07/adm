#include "bfs.h"
#include "graph_type.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//**************************************************************************************************
// Macros
//**************************************************************************************************
#define TEST_GRAPH_NUM_ARGS 2
#define ERROR(...) fprintf(stderr, ##__VA_ARGS__);

//**************************************************************************************************
// Globals
//*************************************************************************************************
static Graph *gGraphInstance;

//**************************************************************************************************
// Functions
//*************************************************************************************************

//**************************************************************************************************
// Map error to graph error
//**************************************************************************************************
static int MapGraphErrorToErrno(GraphError err) {
  switch (err) {
  case kGraphErrorSuccess:
    return 0;
  case kGraphErrorNoMem:
    return -ENOMEM;
  case kGraphErrorBadArgs:
    return -EINVAL;
  case kGraphErrorSearchAbort:
    return -EIO;
  case kGraphErrorNoPath:
    return -EIO;
  }
  return 0;
}

//**************************************************************************************************
// ShowUsage
//**************************************************************************************************
static void ShowUsage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "test_graph <input graph descriptor file path>" << std::endl;
}

//**************************************************************************************************
// Get graph
//**************************************************************************************************
int GetGraphInstance(const char *graph_desc_path) {
  GraphError err;
  int num_nodes, i, j;
  std::ifstream in(graph_desc_path);

  in >> num_nodes;
  gGraphInstance = new Graph(num_nodes, true);

  while (!in.eof()) {
    in >> i >> j;
    std::cout << "Inserting Edge " << i << " " << j << std::endl;
    err = gGraphInstance->InsertEdge(i, j, 0);
    if (err != kGraphErrorSuccess) {
      ERROR("inserting edge %d %d failed, ret = %d", i, j, err);
      return MapGraphErrorToErrno(err);
    }
  }
  return 0;
}

//**************************************************************************************************
// Perform Bfs
//**************************************************************************************************
int TestBfs() {
  Bfs bfs_algo(*gGraphInstance);
  Graph::VertexListIterator v_it(*gGraphInstance);
  GraphError err;
  const Vertex *start = nullptr;
  const Vertex *end = nullptr;
  int num_nodes = gGraphInstance->V();
  std::list<const Vertex *> path;
  int cnt = 0;

  for (v_it = v_it.begin(); !v_it.end(); ++v_it) {
    start = !start ? v_it.getVertex() : start;
    cnt++;
    if (cnt == num_nodes) {
      end = v_it.getVertex();
    }
  }

  err = bfs_algo.PerformSearch(start);
  if (err != kGraphErrorSuccess) {
    ERROR("PerformSearch failed with err = %d\n", err);
    return MapGraphErrorToErrno(err);
  }

  err = bfs_algo.GetPathFromTo(start, end, path);
  if (err != kGraphErrorSuccess) {
    ERROR("GetPath failed with err = %d\n", err);
    return MapGraphErrorToErrno(err);
  }

  std::cout << "Path from " << *start << " to " << *end << std::endl;
  for (auto it = path.begin(); it != path.end(); ++it) {
    std::cout << (*it)->getId() << " ";
  }
  std::cout << std::endl;

  return 0;
}

//**************************************************************************************************
// main
//**************************************************************************************************
int main(int argc, char **argv) {

  int ret;

  if (argc != TEST_GRAPH_NUM_ARGS) {
    ShowUsage();
    exit(-1);
  }

  ret = GetGraphInstance(argv[1]);
  if (ret) {
    ERROR("GetGraphInstance Failed, ret = %d\n", ret);
    exit(ret);
  }

  ret = TestBfs();
  if (ret) {
    ERROR("Bfs failed with ret = %d\n", ret);
    exit(ret);
  }
  return 0;
}

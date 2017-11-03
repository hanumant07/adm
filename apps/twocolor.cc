#include "bfs.h"
#include "graph_parser.h"
#include <iostream>

class TwoColor : public Bfs {
public:
  enum ColorType {
    kColorTypeNone = 0,
    kColorTypeBlack = 1,
    kColorTypeWhite = 2,
  };
  TwoColor(Graph &g) : Bfs(g) {
    Graph::VertexListIterator v_it(GetGraphInstance());

    for (v_it.begin(); !v_it.end(); ++v_it) {
      color_map[v_it.getVertex()] = kColorTypeNone;
    }
  }
  GraphError IsBipartite(bool *out);
  ~TwoColor();

private:
  void ProcessVertexEarly(const Vertex *v);
  void ProcessEdge(const Vertex *from, const Vertex *to);
  ColorType ComplementColor(ColorType &input) {
    if (input == kColorTypeBlack) {
      return kColorTypeWhite;
    }
    if (input == kColorTypeWhite) {
      return kColorTypeBlack;
    }
    return kColorTypeNone;
  }
  std::unordered_map<const Vertex *, ColorType> color_map;
};

//**************************************************************************************************
// Destructor
//**************************************************************************************************
TwoColor::~TwoColor() { color_map.clear(); }

//**************************************************************************************************
// Plugin to BFS, do pre processing on vertex
//**************************************************************************************************
void TwoColor::ProcessVertexEarly(const Vertex *v) {
  if (color_map[v] == TwoColor::kColorTypeNone) {
    color_map[v] = TwoColor::kColorTypeWhite;
  }
}

//**************************************************************************************************
// Plugin to BFS, compute colors and check for failure condition of bipartite
// testing
//**************************************************************************************************
void TwoColor::ProcessEdge(const Vertex *from, const Vertex *to) {

  if (color_map[to] == TwoColor::kColorTypeNone) {
    color_map[to] = ComplementColor(color_map[from]);
    return;
  }

  // Adjacent nodes with same color
  if (color_map[from] == color_map[to]) {
    terminate = true;
    return;
  }
}

//**************************************************************************************************
// Public interface to test if a given graph instance is bipartite
//**************************************************************************************************
GraphError TwoColor::IsBipartite(bool *out) {
  GraphError ret;

  if (!out) {
    return kGraphErrorBadArgs;
  }

  ret = PerformSearch();

  if (ret == kGraphErrorSearchAbort) {
    *out = false;
    return kGraphErrorSuccess;
  } else if (ret == kGraphErrorSuccess) {
    *out = true;
    return kGraphErrorSuccess;
  } else {
    std::cout << "Search encountered error " << ret << std::endl;
    return ret;
  }
}

//**************************************************************************************************
// main
//**************************************************************************************************

int main(int argc, char **argv) {
  GraphError ret;
  std::vector<Graph *> graph_vec;
  bool bipartite;

  ret = GraphParser::GetGraphsFromFile(argv[1], graph_vec);
  if (ret != kGraphErrorSuccess) {
    std::cout << "Parsing of Graph Desciprion failed, ret = " << ret
              << std::endl;
    return ret;
  }

  for (auto it : graph_vec) {
    TwoColor two_color(*it);

    ret = two_color.IsBipartite(&bipartite);
    if (ret != kGraphErrorSuccess) {
      std::cout << "Two Color algorithm failed with error = " << ret
                << std::endl;
      goto cleanup;
    }

    if (bipartite) {
      std::cout << "BICOLORABLE." << std::endl;
    } else {
      std::cout << "NOT BICOLORABLE." << std::endl;
    }
  }
cleanup:
  GraphParser::CleanupGraphs(graph_vec);
  exit(ret);
}

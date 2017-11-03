#include "graph_parser.h"
#include <fstream>
#include <iostream>

//**************************************************************************************************
// Macros
//**************************************************************************************************
#ifdef DEBUG
static int debug = 1;
#else
static int debug = 0;
#endif

// Send error to stderr
#define ERROR(...) fprintf(stderr, ##__VA_ARGS__)

// Send log to stdout
#define LOG(...)                                                               \
  do {                                                                         \
    if (debug)                                                                 \
      fprintf(stdout, ##__VA_ARGS__);                                          \
  } while (0)

//**************************************************************************************************
// Types
//**************************************************************************************************
//**************************************************************************************************
// Parser Base class
//**************************************************************************************************
class GParser {
public:
  virtual GraphError GetGraphFromInput(std::vector<Graph *> &out_graphs) = 0;
};

//**************************************************************************************************
// Uva judge graph input parser
//**************************************************************************************************
class UvaParser : public GParser {
public:
  GraphError GetGraphFromInput(std::vector<Graph *> &out_graphs) {

    if (!istr.good()) {
      ERROR("input stream not initialized\n");
      return kGraphErrorBadArgs;
    }

    bool directed = false;
    std::string graph_type;

    istr >> graph_type;

    if (graph_type == "directed") {
      directed = true;
    }

    while (!istr.eof()) {
      int num_nodes;
      int num_edges;
      int v1, v2;
      GraphError ret;

      istr >> num_nodes;

      if (!num_nodes) {
        break;
      }

      Graph *g = new Graph(num_nodes, directed);

      istr >> num_edges;
      while (num_edges--) {
        istr >> v1 >> v2;
        // Assume weight as 0
        LOG("Inserting edge %d %d\n", v1, v2);
        ret = g->InsertEdge(v1, v2, 0);
        if (ret != kGraphErrorSuccess) {
          ERROR("Unable to insert edge %d %d, ret = %d\n", v1, v2, ret);
          return ret;
        }
      }

      out_graphs.push_back(g);
    }
    return kGraphErrorSuccess;
  }

private:
  // method to check compatibility
  static bool Compatible(const std::string &str) {
    if (str == "uva") {
      return true;
    }
    return false;
  }
  UvaParser(std::istream &is) : istr(is){};
  std::istream &istr;
  friend class GParserFactory;
};

//**************************************************************************************************
// Factory to create parsers based on format compatibility
//**************************************************************************************************
class GParserFactory {
public:
  // Factory method to get parser instance based on input format
  static GParser *GetParser(std::istream &is) {
    std::string format;

    is >> format;
    if (UvaParser::Compatible(format)) {
      return new UvaParser(is);
    }
    return nullptr;
  }
};

//**************************************************************************************************
// Functions
//**************************************************************************************************

//**************************************************************************************************
// Get Graph Instance from input file
//**************************************************************************************************
GraphError GraphParser::GetGraphsFromFile(const char *path,
                                          std::vector<Graph *> &out_graphs) {
  GraphError ret;
  std::fstream fstr;
  GParser *gp;

  try {
    fstr.open(path, std::fstream::in);
  } catch (...) {
    ERROR("Unable to open file %s\n", path);
    return kGraphErrorBadArgs;
  }

  gp = GParserFactory::GetParser(fstr);
  if (!gp) {
    ERROR("input file %s parser not found", path);
    return kGraphErrorUnhandled;
  }

  ret = gp->GetGraphFromInput(out_graphs);
  if (ret != kGraphErrorSuccess) {
    ERROR("GetGraphFromInput(): ret = %d\n", ret);
    goto cleanup;
  }

  return ret;
cleanup:
  GraphParser::CleanupGraphs(out_graphs);
  return ret;
}

//**************************************************************************************************
// Cleanup Graph Instance
//**************************************************************************************************
GraphError GraphParser::CleanupGraphs(std::vector<Graph *> &out_graphs) {
  for (auto it = out_graphs.begin(); it != out_graphs.end(); ++it) {
    delete *it;
    *it = 0;
  }
  out_graphs.clear();
  return kGraphErrorSuccess;
}

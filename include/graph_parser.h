#include "graph_type.hpp"

namespace GraphParser {
GraphError GetGraphsFromFile(const char *path,
                             std::vector<Graph *> &out_graphs);
GraphError CleanupGraphs(std::vector<Graph *> &graphs);
};

#include "graph_type.hpp"
#include <list>
#include <queue>

//**************************************************************************************************
// Breadth first search
//**************************************************************************************************
class Bfs {
public:
  Bfs(Graph &g);
  // search variants
  virtual GraphError PerformSearch(const Vertex *s);
  virtual GraphError PerformSearch();
  // retrieve path from source to destination. Call only after
  // PerformSearch(from).
  // if no path return kGraphErrorNoPath. If this function called without
  // running search first
  // it will return kGraphErrorNoPath.
  GraphError GetPathFromTo(const Vertex *from, const Vertex *to,
                           std::list<const Vertex *> &out_path);
  virtual ~Bfs();

protected:
  // plugins to modify breadth first search
  virtual void ProcessVertex(const Vertex *v) {}
  virtual void ProcessVertexEarly(const Vertex *v) {}
  virtual void ProcessVertexLate(const Vertex *v) {}
  virtual void ProcessEdge(const Vertex *curr, const Vertex *end) {}
  Graph &GetGraphInstance() { return g; }
  // terminate search
  bool terminate;

private:
  Bfs(const Bfs &);
  Bfs &operator=(const Bfs &);
  // discovered vertices during search
  std::unordered_map<const Vertex *, bool> discovered;
  // processed vertices
  std::unordered_map<const Vertex *, bool> processed;
  // trace parent
  std::unordered_map<const Vertex *, const Vertex *> parent;
  // queue for breadth first search
  std::queue<const Vertex *> search_queue;
  Graph &g;
};

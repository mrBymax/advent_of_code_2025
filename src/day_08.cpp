#include "shared/io.hpp"
#include <iostream>
#include <sstream>
#include <vector>

struct Box {
  int x, y, z;
};

struct Edge {
  int u, v;
  long long weight;
  bool operator<(const Edge &other) const { return weight < other.weight; }
};

int find(int i, std::vector<int> &parent) {
  if (parent[i] == i)
    return i;
  return parent[i] = find(parent[i], parent);
}

bool unite(int i, int j, std::vector<int> &parent, std::vector<int> &sz) {
  i = find(i, parent);
  j = find(j, parent);
  if (i == j)
    return false; // already connected
  if (sz[i] < sz[j])
    std::swap(i, j);
  parent[j] = i;
  sz[i] += sz[j];
  return true; // a new connection is formed
}

void solve(const std::vector<std::string> &lines) {
  std::vector<Box> boxes;
  // line: "x, y, z"
  for (const auto &line : lines) {
    std::istringstream iss(line);
    int x, y, z;
    char c;
    iss >> x >> c >> y >> c >> z;
    boxes.push_back({x, y, z});
  }

  // DSU prep
  std::vector<int> parent(boxes.size());
  std::vector<int> sz(boxes.size(), 1);
  std::vector<Edge> edges;

  int components = boxes.size();

  // Each node is initially its parent
  for (size_t i = 0; i < boxes.size(); ++i) {
    parent[i] = i;
  }

  for (int i = 0; i < boxes.size(); ++i) {
    for (int j = i + 1; j < boxes.size(); ++j) {
      // calculate squared distance between boxes
      long long dx = boxes[i].x - boxes[j].x;
      long long dy = boxes[i].y - boxes[j].y;
      long long dz = boxes[i].z - boxes[j].z;
      long long dist = dx * dx + dy * dy + dz * dz;
      // add the edge to the list of edges
      edges.push_back({i, j, dist});
    }
  }

  // sort by distance
  std::sort(edges.begin(), edges.end());

  long long ans = 1;
  // process the loop and unite
  for (size_t k = 0; k < edges.size(); ++k) {
    int i = edges[k].u;
    int j = edges[k].v;
    if (unite(i, j, parent, sz)) {
      components--;
      if (components == 1) {
        ans = (long long)boxes[i].x * boxes[j].x;
        break;
      }
    }
  }

  std::cout << ans << std::endl;
}

int main() {
  auto lines = aoc::io::read_lines();

  solve(lines);

  return 0;
}

/** Notes
 *
 * We have list of 3D coordinates representing boxes; we need to connect them
 * based on their proximity. This sounds a lot like a graph problem. We could
 * determine the distance between each pair of boxes and use that as a weight
 * for edges in the graph. Basically, we need to find the minimum spanning
 * tree of this graph. Also, we have a hard stop at 1000 pairs.
 *
 * We can use Kruskal's algorithm. Why not Prim's algorithm? Because we can
 * assume that the graph is very dense, and Kruskal's algorithm is
 * efficient for dense graphs.
 *
 * A point will be represented by a struct with x, y, and z coordinates.
 *
 * TC: O(E log V) -> Kruskal's algorithm
 * SC: O(V + E) -> we need to store the graph and the disjoint set data
 * structure.
 *
 * For part 2, we need to construct one single MST instead of stopping at 1000
 * boxes. It's enough to modify the stopping condition, while keeping track of
 * the number of connected components. The complexity stays the same, although
 * the implementation becomes more complex because we need to catch the moment
 * when we have only one connected component (because the result is a product
 * between the last two components).
 */

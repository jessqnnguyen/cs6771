#include <stdexcept>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#include <map>

namespace gdwg {
   template <typename N, typename E>
   class Graph {
   public:
     Graph() : it(0) {}
     // Copy constructor.
     Graph(const Graph& g);

     // Move constructor.
     Graph(Graph&& g);

     // Copy operator.
     Graph& operator=(const Graph& g);

     // Move operator.
     Graph& operator=(Graph&& g);

     bool addNode(const N& val);
     bool addEdge(const N& src, const N& dst, const E& w);
     bool isNode(const N& val) const;
     bool replace(const N& oldData, const N& newData);
     void mergeReplace(const N& oldData, const N& newData);
     void deleteNode(const N& val) noexcept;
     void printNodes() const;
     void deleteEdge(const N& src, const N& dst, const E& w) noexcept;
     bool isConnected(const N& src, const N& dst) const;
     void clear() noexcept;
     void printEdges(const N& val) const;

     // Internal iterators.
     void begin() const;
     bool end() const;
     void next() const;
     const N& value() const;

     void setIt(int i) { it = i; }

   private:
     // ---------------------------------------------------------------
   	 // Private inner class to hold nodes with data value of type N
     // in the graph.
     // ---------------------------------------------------------------
     class Node {
     public:
       // Constructor.
       Node(const N& data) : data{std::make_shared<N>(data)}, edgeCount(0) {}
       // Copy constructor.
       Node(const Node& n) : data(n.data), edgeCount(n.edgeCount) {}
       // Getters and setters.
       N& getData() const { return *data; }
       void setData(const N& val) { *data = val; }
       std::shared_ptr<N> getDataPtr() { return data; }
       double getEdgeCount() { return edgeCount; }
       void setEdgeCount(const double& n) { edgeCount = n; }
     private:
       std::shared_ptr<N> data;
       double edgeCount; // count of outgoing edges.
     };
     // ---------------------------------------------------------------
     // Private inner class to hold edges with edge weight value of type
     // E in the graph.
     // ---------------------------------------------------------------
     class Edge {
     public:
       Edge(const N& src, const N& dest,
            const E weight) :
            src{std::make_shared<N>(src)}, dest{std::make_shared<N>(dest)},
            weight{std::make_shared<E>(weight)} {}
       N& getSrc() const { return *src; }
       N& getDest() const { return *dest; }
       E& getWeight() const { return *weight; }
       std::shared_ptr<N> getSrcPtr() { return src; }
       std::shared_ptr<N> getDestPtr() { return dest; }
       std::shared_ptr<E> getWeightPtr() { return weight; }
     private:
       std::shared_ptr<N> src;
       std::shared_ptr<N> dest;
       std::shared_ptr<E> weight;
     };
     // ---------------------------------------------------------------
     // Private graph member variables.
     // ---------------------------------------------------------------
     std::vector<std::shared_ptr<Node>> nodes;
     std::vector<std::shared_ptr<Edge>> edges;
     mutable typename std::vector<std::shared_ptr<Node>>::const_iterator it;
   };
   #include "Graph.tem"
}

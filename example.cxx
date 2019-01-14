#include <iostream>
#include <graph.h>
#include <block.h>
#include <limits>


int main() {

  int maxI = 255; //maximum pixel intensity
  int nCol = 2; //number of columns
  int nColNode = 5; //number of nodes per column
  double inf = std::numeric_limits<double>::infinity();

  typedef Graph<int,int,int> GraphType;
  GraphType *g = new GraphType(nCol*nColNode,nCol*nColNode*4);

  int nodeI[nCol][nColNode] = {{0,1,2,3,4},{5,6,7,8,9}};
  int nodeVal[nCol][nColNode] = {{215,225,195,120,110},{230,220,190,200,110}}; // intensities (based on object/background char values)

  // Build columns
  for(int c = 0; c < nCol; c++) {
    for(int n = 0; n < nColNode; n++) {
      g->add_node(nodeI[c][n]);
    }
  }

  // Intracolumn edges
  std::cout << "Intracolumn arcs:" << std::endl;
  for(int c = 0; c < nCol; c++) {
    for(int n = 0; n < nColNode-1; n++) {
      int intraColW = maxI - abs(nodeVal[c][n] - nodeVal[c][n+1]);
      g->add_edge(nodeI[c][n],nodeI[c][n+1],intraColW,inf);
      std::cout << nodeI[c][n] << "---" << intraColW << "--->" << nodeI[c][n+1] << std::endl;
    }
  }
  std::cout << std::endl;

  // Connect columns to source and sink
  std::cout << "Source/Sink connections:" << std::endl;
  int toSourceVal[2] = {205,210};
  int toSinkVal[2] = {100,105};
  for(int c = 0; c < nCol; c++) {
    int toSourceW = maxI - abs(nodeVal[c][0] - toSourceVal[c]);
    g->add_tweights(nodeI[c][0],toSourceW,0);
    std::cout << nodeI[c][0] << "<---" << toSourceW << "---SOURCE" << std::endl;
    int toSinkW = maxI - abs(nodeVal[c][nColNode-1] - toSinkVal[c]);
    g->add_tweights(nodeI[c][nColNode-1],0,toSinkW);
    std::cout << nodeI[c][nColNode-1] << "---" << toSinkW << "--->SINK" << std::endl;
  }
  std::cout << std::endl;
  
    // Intercolumn edges
  std::cout << "Intercolumn arcs:" << std::endl;
  int dSmooth = 1;
  for(int n = 0; n < nColNode-dSmooth; n++) {
    int intraColW = maxI - abs(nodeVal[0][n] - nodeVal[1][n+dSmooth]);
    g->add_edge(nodeI[0][n],nodeI[1][n+dSmooth],0,inf);
    std::cout << nodeI[1][n+dSmooth] << "---inf--->" << nodeI[0][n] << std::endl;
    g->add_edge(nodeI[1][n],nodeI[0][n+dSmooth],0,inf);
    std::cout << nodeI[0][n+dSmooth] << "---inf--->" << nodeI[1][n] << std::endl;
  }
  int intraColW = maxI - abs(nodeVal[0][(nColNode-1)/2] - nodeVal[1][(nColNode-1)/2]);
  g->add_edge(nodeI[0][(nColNode-1)/2],nodeVal[1][(nColNode-1)/2],intraColW,intraColW);
  std::cout << std::endl;

  // Compute maximum flow
  int flow = g->maxflow();
  std::cout << "Max flow: " << flow << std::endl;

  // Print out nodes in each set
  std::cout << "Nodes connected to source: " << std::flush;
  for(int n = 0; n < nCol*nColNode; n++) {
    if(g->what_segment(n) == GraphType::SOURCE) {
      std::cout << n << ", " << std::flush;
    }
  }
  std::cout << std::endl;
  std::cout << "Nodes connected to sink: " << std::flush;
  for(int n = 0; n < nCol*nColNode; n++) {
    if(g->what_segment(n) == GraphType::SINK) {
      std::cout << n << ", " << std::flush;
    }
  }
  std::cout << std::endl;

  return 0;
}

#include <tulip/Face.h>
#include <tulip/SuperGraphMap.h>
#include <tulip/Node.h>
#include <vector>
#include <map>
#include <stdio.h>
#include <tulip/MutableContainer.h>


class Ordering : public std::vector<std::vector<node> >
{
  
 public :

  typedef struct FaceAndPos_ {
    Face face;
    node n_first;
    node n_last;
  } FaceAndPos;


 private :
 
  SuperGraphMap::SuperGraphMap * Gp;
  MutableContainer<int>::MutableContainer oute;
  MutableContainer<int>::MutableContainer outv;
  MutableContainer<bool>::MutableContainer visitedNodes;
  MutableContainer<bool>::MutableContainer visitedFaces;
  MutableContainer<bool>::MutableContainer markedFaces;
  MutableContainer<int>::MutableContainer seqP;
  MutableContainer<bool>::MutableContainer isOuterFace;
  MutableContainer<bool>::MutableContainer contour;
  MutableContainer<bool>::MutableContainer is_selectable;
  MutableContainer<bool>::MutableContainer is_selectable_visited;
  MutableContainer<bool>::MutableContainer is_selectable_face;
  MutableContainer<bool>::MutableContainer is_selectable_visited_face;
  MutableContainer<node>::MutableContainer left;
  MutableContainer<node>::MutableContainer right;
  bool existMarkedF;
  FaceAndPos minMarkedFace ; 
  std::vector<node> v1;
  Face ext;
  std::vector<edge> dummy_edge;

  node getLastOfQ(Face f, node prec, node n, edge e);  // return last node of q
  node getLastOfP(Face f, node prec, node n,edge e); // return last node of p 
  std::vector<node> getNeighborsFacesAndUpdate(node n, node prec,edge ed, std::vector<Face> v_faces);  // return the neighbor of n and update contour and visitedNodes
  std::vector<node> getPathFrom(std::vector<node> fn, int from); // return the longest path of fn from position "from"
  int infFaceSize();                              // return the number of node of the infinite face

  void updateOutAndVisitedFaces(Face f); // update outv, oute and visitedFaces for f and its nodes
  void updateContourLeftRight(node prec, node n, edge e, node last); // add nodes between n and last on the contour to the member contourn and update left and right
  void updateNewSelectableNodes(node node_f, node no_tmp2, edge ed_tmp, node node_last,std::vector<Face> v_faces, bool one_face = false, bool was_visited = false, bool selection_face = false); // update is_selectable and is_selectable_visited for nodes between prec and last on the contour
  void updateSelectableFaces(std::vector<Face> v_faces); // update is_selectable_face and is_selectable_visited_face
  int seqp(Face f);                              // return the number of ci in f such as ci+1 is in f
  
  struct augmentableAndNodes_ getAugAndNodes(Face f);
  //  struct MinMaxAndNodes_ getMinMax(Face f);               
  void minMarkedf();                              // put the "highest" marked face into minMarkedFace
  void setMinMarkedFace(Face f);                 // set minMarkedFace at f
  
  void augment(Face f, node prec, node n, node prec_last, node last, int nbNewFace, bool pair);   // augment the face f and update the graph
  void selectAndUpdate(Face f);//,node prec=NULL, node n=NULL,int n=0); // select  the face f and update
  void selectAndUpdate(node n); // select node n and update 
  bool isSelectable(node n);                       // return true if node n can be selected

  void init();      // initialize all attributes
  void init_v1(std::vector<node> fn); //Inittialize v1
  void init_selectableNodes(); // initialize is_selectable and is_selectable_visited
  void init_selectableFaces(); // initialize is_selectable_face and is_selectable_visited_face
  void init_outv_oute();  // initialize outv and oute
  void init_seqP();   // initialize seqP
  void init_outerface(); // initialize ext, isOuterFace

 public :
  std::vector<edge> getDummyEdges(){ return dummy_edge; }
  
  Ordering(SuperGraphMap * G);

  ~Ordering();
  
  std::vector<std::vector<node> > computeCanonicalOrdering();//SuperGraphMap * G);
};

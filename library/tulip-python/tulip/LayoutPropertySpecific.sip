
	tlp::Coord getMax(tlp::Graph *graph=0);
	
	tlp::Coord getMin(tlp::Graph *graph=0);

	void translate(const tlp::Coord& v, tlp::Graph *graph=0);
 
	void translate(const tlp::Coord& v, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void scale(const tlp::Coord& v, tlp::Graph *graph=0);
	
	void scale(const tlp::Coord& v, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
  
	void rotateZ(const double& alpha, tlp::Graph *graph=0);

	void rotateX(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void rotateY(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void rotateZ(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void center(tlp::Graph *graph = 0);
	
	void normalize(tlp::Graph *graph = 0);
	
	void perfectAspectRatio();
	
	void resetBoundingBox();
  
	double edgeLength(const tlp::edge e) const;
  
	double averageAngularResolution(const tlp::Graph *graph = 0) const;
  
  	double averageAngularResolution(const tlp::node n, const tlp::Graph *graph = 0) const;
  
  	std::vector<double> angularResolutions(const tlp::node n, const tlp::Graph *graph = 0) const;

	void computeEmbedding(tlp::Graph *sg = 0);
  
  	void computeEmbedding(const tlp::node n, tlp::Graph *sg = 0);
  
  	unsigned int crossingNumber() const;
  	
  	
#ifndef EMBEDDERDEFS_H
#define EMBEDDERDEFS_H

struct vtx_data {
  int nedges;
  int *edges;
  float *ewgts;
};

typedef int DistType; // must be signed!!

inline double max(double x, double y) {
	if (x>=y)
		return x;
	else
		return y;
}

inline double min(double x, double y) {
	if (x<=y)
		return x;
	else
		return y;
}

inline int max(int x, int y) {
	if (x>=y)
		return x;
	else
		return y;
}

inline int min(int x, int y) {
	if (x<=y)
		return x;
	else
		return y;
}

struct Point {
	double x;
	double y;
	int operator==(Point other) { return x==other.x && y==other.y; }
};


#endif

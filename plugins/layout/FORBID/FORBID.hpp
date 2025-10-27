#include <tuple>
#include <string>
#include <vector>
#include <cassert>

#include "randomkit.h"

using namespace std;

struct Vec2d
{
    double a;
    double b;
    Vec2d(double x, double y) : a(x), b(y) {}
    Vec2d() = default;

    inline void set(double a, double b)
    {
        this->a = a;
        this->b = b;
    }
};

struct Coord : Vec2d
{
    Coord(double x, double y) : Vec2d(x, y) {}
    Coord() = default;

    inline double x() const { return a; }
    inline double y() const { return b; }
    inline void set_x(double x) { a = x; }
    inline void set_y(double y) { b = y; }
};

struct Size : Vec2d
{
    Size(double width, double height) : Vec2d(width, height) {}
    Size() = default;

    inline double width() const { return this->a; }
    inline double height() const { return this->b; }
    inline void set_width(double x) { a = x; }
    inline void set_height(double y) { b = y; }
};

struct Layout
{
    vector<Coord> pos;
    vector<Size> sizes;

    Layout(vector<Coord> pos, vector<Size> sizes) : pos(pos), sizes(sizes)
    {
        assert(pos.size() == sizes.size());
    }

    unsigned int N() const
    {
        return this->pos.size();
    }
};

struct term
{
    int i, j;
    double d, w;
    bool o;
    term(int i, int j, double d, double w, bool o) : i(i), j(j), d(d), w(w), o(o) {}
    term(int i, int j, double d, double w) : i(i), j(j), d(d), w(w) {}
};

// Nodes intersection
double maxOfLeft(Coord const &p1, const Size &s1, Coord const &p2, const Size &s2);
double minOfRight(Coord const &p1, const Size &s1, Coord const &p2, const Size &s2);
double minOfTop(Coord const &p1, const Size &s1, Coord const &p2, const Size &s2);
double maxOfBot(Coord const &p1, const Size &s1, Coord const &p2, const Size &s2);
tuple<double, double> nodeRectanglesIntersection(Coord const &p1, const Size &s1, Coord const &p2, const Size &s2);

// Getting overlaps
vector<size_t> sort_indexes(const double *v, int size);
bool overlapCheck(Coord const &p1, Coord const &p2, Size const &s1, Size const &s2);
bool scanLineOverlapCheck(vector<double> &X, vector<double> &S);
vector<tuple<int, int>> getAllOverlaps(vector<double> &X, vector<double> &S);

// Compute distances
double FORBID_delta(Coord const &ci, Coord const &cj, Size const &si, Size const &sj, double intersec_width, double intersec_height);
double SIDE2SIDE_delta(Coord const &ci, Coord const &cj, Size const &si, Size const &sj, double intersec_width, double intersec_height);
double PRISM_delta(Coord const &ci, Coord const &cj, Size const &si, Size const &sj, double intersec_width, double intersec_height);
double eucl(double x1, double y1, double x2, double y2);
double eucl(Coord const &p1, Coord const &p2);
double vecNorm2D(double vec_x, double vec_y);

struct Parametrizer
{
    double K = 4;
    double ALPHA = 2;
    double MINIMUM_MOVEMENT = 1e-6;
    int MAX_ITER = 30;
    int MAX_PASSES = 100;
    double SCALE_STEP = 0.1;
    double (*distance_fn)(Coord const &, Coord const &, Size const &, Size const &, double, double) = &FORBID_delta;
    double delta = 0.03;
    double eps = 0.01;
    int seed = 0;
    bool PRIME = false;
};

// Scaling
double maxScaleRatio(vector<double> &X, vector<double> &S);
void scaleLayout(vector<double> &X, double scaleFactor);
bool isCurrentScaleSolvable(vector<double> &X, vector<double> &S);

// Loading and saving
Layout loadOriginalDistance(string input_path);
void loadParams(Parametrizer &params, char *argv[]);
vector<double> initLayout(Layout const &g);
vector<double> initSizes(Layout const &g);
void copyLayout(vector<double> const &in, vector<double> &out);
void save(vector<double> const &X, vector<double> const &S, string input_path);

// Main functions, terms = delta ; adaptation of SGD2
vector<term> layoutToTerms(vector<double> &X, vector<double> &init_X, vector<double> &S, Parametrizer &params);
void OPTIMIZATION_PASS(vector<double> &X, vector<term> &terms, vector<double> &init_X, vector<double> &S, const vector<double> &etas, Parametrizer &params);
void fisheryates_shuffle(vector<term> &terms, rk_state &rstate);
vector<double> schedule(const vector<term> &terms, int t_max, double eps);

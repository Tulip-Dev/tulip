#include <cmath>
#include <string>
#include <utility>
#include <algorithm>
#include <tulip/TulipPlugin.h>
#include <tulip/Glyph.h>
#include <tulip/TreeTest.h>
#include <tulip/GlTools.h>
#include "tulip/Border.h"
#include <tulip/GlTextureManager.h>


#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

using namespace std;
using namespace tlp;

const node NOTATREE;

const float PARAM_F = 1.f;
const float PARAM_W = 15.f;

const float PARAM_MAX = 255.f;
const float PARAM_A = -1.f;
const float PARAM_B = 16.f;

typedef GLfloat Point3[3];

/** \addtogroup glyph */
/*@{*/
/// - An implementation of square with variable border glyph.
/** 
 * This glyph is an implementation of a square with a variable border.
 * The size of the border depend on the depth of the node, decreasing from the 
 * root. 
 *
 * AUTHOR:
 *
 * Julien Testut, 
 * Antony Durand, 
 * Pascal Ollier, 
 * Yashvin Nababsing, 
 * Sebastien Leclerc, 
 * Ruchon Thibault, 
 * Eric Dauchier,
 * University Bordeaux I France
 */
 
//====================================================================
class SquareBorderTextured: public Glyph, public GraphObserver {

public:
    SquareBorderTextured(GlyphContext* gc = NULL);
    virtual ~SquareBorderTextured();
    virtual void draw(node n,float lod);
    virtual Coord getAnchor(const Coord& vector) const;

protected:
    typedef stdext::hash_map<node, int> mapNodeLevel_t; 

    struct TreeCache {
        bool           isTree;
        node           root;
        int            maxDepth;
        mapNodeLevel_t mapNodeLevel;
        GLuint         texObj;
        float          paramA;
        float          paramB;
        float          paramBorderMaxWidth;      

        TreeCache():texObj(0){
        }
    };
    typedef stdext::hash_map<Graph*, TreeCache> mapGraphCache_t;
    mapGraphCache_t mapTree;

    inline node findRoot(node n);
    
  bool  initializeNewGraph(Graph*, node);
    void  unInitializeNewGraph(Graph*);
    void  drawSquare(node n, float borderSize);
    int   attributeNodeLevel(node n, int depth, mapNodeLevel_t& mapnodelevel);
    void  setTulipGLState(node n);
    float calcBorderSum(int level);
    void  generateTexture(Graph*);

    virtual void addNode(Graph*, const node);
    virtual void addEdge(Graph*, const edge);
    virtual void delNode(Graph*, const node);
    virtual void delEdge(Graph*, const edge);
    virtual void destroy(Graph*);

private:
    Graph* tree;
};

//====================================================================
GLYPHPLUGIN(SquareBorderTextured, "2D - Square Border Textured",
            "Julien Testut, Antony Durand, Pascal Ollier,"
            "Yashvin Nababsing, Sebastien Leclerc, Ruchon Thibault,"
            "Eric Dauchier",
            "09/07/2004", "Textured square22", "1.0", 101);
            
//====================================================================
SquareBorderTextured::SquareBorderTextured(GlyphContext* gc):Glyph(gc){
}

//====================================================================
SquareBorderTextured::~SquareBorderTextured() {
}
//====================================================================
bool SquareBorderTextured::initializeNewGraph(Graph* sg, node n) {
    TreeCache& treec = mapTree[sg];          
    treec.isTree     = TreeTest::isTree(sg);
     
    sg->addObserver(this);
    
    if (treec.isTree) {
        treec.root     = findRoot(n);
        treec.maxDepth = attributeNodeLevel(treec.root, 1, treec.mapNodeLevel);   
        generateTexture(sg);
    }
    else {
        treec.root = NOTATREE;
        treec.mapNodeLevel.clear();
    }   
    return treec.isTree;
}

//===================================================================
void SquareBorderTextured::unInitializeNewGraph(Graph* sg) {
    mapGraphCache_t::iterator itMap = mapTree.find(sg);
    
    if (itMap != mapTree.end()) {
        if (glIsTexture(itMap->second.texObj))
            glDeleteTextures(1, &(itMap->second.texObj));

        mapTree.erase(itMap);
    }
    sg->removeObserver(this);
}

//====================================================================
float SquareBorderTextured::calcBorderSum(int level) {
    float sumBorder = 0;
    
    for (int i = 1; i < level; i++)
        sumBorder += evaluateBorderSize(i);
    
    return sumBorder;
}
//====================================================================
void SquareBorderTextured::generateTexture(Graph* sg) {
    TreeCache& treec = mapTree[sg];

    const float MAXCOLOR = 255.f;

    treec.paramBorderMaxWidth = calcBorderSum(treec.maxDepth + 1);            
    treec.paramB              = 4 * MAXCOLOR / treec.paramBorderMaxWidth;
    treec.paramA              = -treec.paramB / treec.paramBorderMaxWidth;

    const int TEXTURE_SIZE = 256;
    /* 
     * Define linear texture of 1x 256+2 for border (for trilinear filtering)
     */
    unsigned int texture[TEXTURE_SIZE + 2][3];

    for (int i = 0 ; i < TEXTURE_SIZE ; i++) {
        const float r     = float(i) / TEXTURE_SIZE* treec.paramBorderMaxWidth;
        const float color = treec.paramA * r * r + treec.paramB * r; 

        texture[i][0] = (unsigned int) color;
        texture[i][1] = (unsigned int) color;
        texture[i][2] = (unsigned int) color;
    }

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &treec.texObj);    
    glBindTexture(GL_TEXTURE_2D, treec.texObj);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXTURE_SIZE, 1, 0, GL_RGB,
                 GL_UNSIGNED_INT, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, 0);        
}

//====================================================================
void SquareBorderTextured::draw(node n,float lod) {
  tree = glGraphInputData->getGraph();
  
  if (mapTree.find(tree) == mapTree.end())
    initializeNewGraph(tree, n);    
  TreeCache& treecache = mapTree[tree];
  
  if (treecache.isTree) {
    Size size = 
      tree->getLocalProperty<SizeProperty>("viewSize")->getNodeValue(n);
    const float borderSize = evaluateBorderSize(treecache.mapNodeLevel[n],
						RectangleArea(size));                                                    
        drawSquare(n, borderSize);
  }
  else
    drawSquare(n, 0);       
}

//====================================================================
Coord SquareBorderTextured::getAnchor(const Coord& vector) const {
    Coord v(vector);
    float x, y, z, fmax;
    
    v.get(x, y, z);
    v.setZ(0.0f);
    fmax = max(fabsf(x), fabsf(y));
    
    if (fmax > 0.0f)
        return v * (0.5f / fmax);
    else
        return v;
}

//====================================================================
void SquareBorderTextured::setTulipGLState(node n) {
    setMaterial(glGraphInputData->elementColor->getNodeValue(n));
    string texFile = glGraphInputData->elementTexture->getNodeValue(n);
    
    if (texFile != "") {
      string texturePath=glGraphInputData->parameters->getTexturePath();
      GlTextureManager::getInst().activateTexture(texturePath+texFile);
    }
}

//====================================================================
void SquareBorderTextured::drawSquare(node n, float borderSizePixel) {
    Size size = tree->getLocalProperty<SizeProperty>("viewSize")->getNodeValue(n);

    float widthBorderRatio = borderSizePixel / size.getW();
    const float posX       = min(widthBorderRatio, 0.45f);  
    const float posXL      = -0.5f + posX;
    const float posXR      = 0.5f - posX;

    float heighthBorderRatio = borderSizePixel / size.getH();
    const float posY       = min(heighthBorderRatio, 0.45f);
    const float posYT      = 0.5f - posY;
    const float posYB      = -0.5f + posY;

    /**************************************************
    *    b1              b2
    *       p1        p2
    *           (0,0)
    *       p3        p4
    *    b3              b4
    ***************************************************/
    Point3 b1 = {-0.5f, 0.5f, 0};
    Point3 b2 = {0.5f, 0.5f, 0};
    Point3 b3 = {-0.5f, -0.5f, 0};
    Point3 b4 = {0.5f, -0.5f, 0};

    Point3 p1 = {posXL, posYT, 0};
    Point3 p2 = {posXR, posYT, 0};
    Point3 p3 = {posXL, posYB, 0};
    Point3 p4 = {posXR, posYB, 0};

    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mapTree[tree].texObj);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    const int level  = mapTree[tree].mapNodeLevel[n];
    const float sl   = calcBorderSum(level);    
    const float bm   = mapTree[tree].paramBorderMaxWidth; 
    const float minu = 0 + sl / bm;  
    const float maxu = (evaluateBorderSize(level) + sl) / bm;

    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(minu, 0);
    glVertex3fv(b1);
    glTexCoord2f(maxu, 0); 
    glVertex3fv(p1);
    glTexCoord2f(minu, 0);
    glVertex3fv(b2);
    glTexCoord2f(maxu, 0);
    glVertex3fv(p2);
    glTexCoord2f(minu, 0);
    glVertex3fv(b4);
    glTexCoord2f(maxu, 0);
    glVertex3fv(p4);
    glTexCoord2f(minu, 0);
    glVertex3fv(b3);         
    glTexCoord2f(maxu, 0);
    glVertex3fv(p3);         
    glTexCoord2f(minu, 0);
    glVertex3fv(b1);         
    glTexCoord2f(maxu, 0);
    glVertex3fv(p1);         
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    setTulipGLState(n);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex2fv(p3);
    glTexCoord2f(1, 0);
    glVertex2fv(p4);
    glTexCoord2f(1, 1); 
    glVertex2fv(p2);
    glTexCoord2f(0, 1);
    glVertex2fv(p1);
    glEnd();        
    glEnable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}

//====================================================================
inline node SquareBorderTextured::findRoot(node n) {
    while (tree->indeg(n) != 0)
        n = tree->getInNode(n, 1);
    return n;
}

//====================================================================
int SquareBorderTextured::attributeNodeLevel(node n, int depth,
                                        mapNodeLevel_t& mapnodelevel) {
    int maxDepth    = 0;   
    mapnodelevel[n] = depth;

    Iterator<node>* itNode = tree->getOutNodes(n);
    while (itNode->hasNext()) {
        int nodeLevel = attributeNodeLevel(itNode->next(), depth + 1, 
                                           mapnodelevel);
        maxDepth      = max(nodeLevel, maxDepth);
    }
    delete itNode;
    return maxDepth + 1;
}

//====================================================================
void SquareBorderTextured::addNode(Graph* sg, const node) {
    unInitializeNewGraph(sg);
}

//====================================================================
void SquareBorderTextured::addEdge(Graph* sg, const edge) {
    unInitializeNewGraph(sg);
}

//====================================================================  
void SquareBorderTextured::delNode(Graph* sg, const node) {
    unInitializeNewGraph(sg);
}

//====================================================================
void SquareBorderTextured::delEdge(Graph* sg, const edge) {
    unInitializeNewGraph(sg);
}

//====================================================================
void SquareBorderTextured::destroy(Graph* sg) {
    unInitializeNewGraph(sg);
}
/*@}*/

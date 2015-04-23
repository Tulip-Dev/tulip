/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/* Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  */
enum yytokentype {
  NUMBER = 258,
  IDENT = 259,
  STRING = 260,
  _STRICT = 261,
  _DIGRAPH = 262,
  _GRAPH = 263,
  _NODE = 264,
  _EDGE = 265,
  _SUBGRAPH = 266,
  _EDGEOP = 267
};
#endif
#define NUMBER 258
#define IDENT 259
#define STRING 260
#define _STRICT 261
#define _DIGRAPH 262
#define _GRAPH 263
#define _NODE 264
#define _EDGE 265
#define _SUBGRAPH 266
#define _EDGEOP 267




/* Copy the first part of user declarations.  */
#line 16 "dotImportParser.y"


#define TLP_LAYOUT_PROXY_NAME "viewLayout"
#define TLP_LABEL_PROXY_NAME "viewLabel"
#define TLP_EXTLABEL_PROXY_NAME "externLabel"
#define TLP_HEAD_LABEL_PROXY_NAME "headLabel" // edge head label
#define TLP_TAIL_LABEL_PROXY_NAME "tailLabel" // edge tail label
#define TLP_COMMENT_PROXY_NAME "comment"
#define TLP_URL_PROXY_NAME "URL"
#define TLP_SIZE_PROXY_NAME "viewSize"
#define TLP_SHAPE_PROXY_NAME "viewShape"
#define TLP_COLOR_PROXY_NAME "viewColor"
#define TLP_BORDERCOLOR_PROXY_NAME "viewBorderColor"
#define TLP_FONTCOLOR_PROXY_NAME "viewLabelColor"
#define DEFAULT_SHAPE 1111 // ellipse


int yylex( void );
void yyerror( char const * ) {}
extern char * yytext;
//extern int yylen;

typedef TLP_HASH_MAP<std::string,node> NodeMap;
typedef std::vector<node> NodeA;
typedef std::vector<edge> EdgeA;

template < typename T > inline
std::vector<T> & operator += ( std::vector<T> & inA,
                               const std::vector<T> & inB ) {
  inA.insert( inA.end(), inB.begin(), inB.end() );
  return inA;
}



//
// Shapes equivalencies
//

struct ShapeAttr {
  const char * name;
  int idx;
};

ShapeAttr ShapeAttrA[] = {
  // tulip shapes
  {"box", 0},
  {"boxoutlined", 1},
  {"sphere", 2},
  {"cone", 3},
  {"square", 4},
  {"teapot", 5},
  {"cyclinder", 6},
  {"billboard", 7},

  // dot shapes
  {"circle", 2}, // as a sphere
  {"ellipse", DEFAULT_SHAPE}, // as an ellipse (distorsed sphere)
};

//
// dot attributes
// This object class enables the managment of various typed attributes
// and some operations such as merge & definition.
// A mask (bitset) is used internally to retrieve the subset of attributes
// which have been really defined.
//

struct DOT_ATTR {
  // attributes
  enum {
    LAYOUT = (1<<0),
    SHAPE = (1<<1),
    WIDTH = (1<<2),
    HEIGHT = (1<<3),
    DEPTH = (1<<4),
    LABEL = (1<<5),
    HEAD_LABEL = (1<<6),
    TAIL_LABEL = (1<<7),
    URL = (1<<8),
    COMMENT = (1<<9),
    FILL_COLOR = (1<<10),
    FONT_COLOR = (1<<11),
    COLOR = (1<<12),
    STYLE = (1<<13)
  };

  unsigned int mask; // defined mask

  Coord layout;
  int shape;
  Size size;
  std::string label;
  std::string headLabel;
  std::string tailLabel;
  std::string url;
  std::string comment;
  Color color;
  Color fontcolor;
  Color fillcolor;
  int style;

  DOT_ATTR ( ) {
    clear();
  }

  void clear ( ) {
    mask = 0;
  }

  DOT_ATTR operator + ( const DOT_ATTR & inRef ) {
    DOT_ATTR attr = *this;
    attr.mask |= inRef.mask;

    if( inRef.mask & LAYOUT ) attr.layout = inRef.layout;

    if( inRef.mask & SHAPE ) attr.shape = inRef.shape;

    if( inRef.mask & WIDTH ) attr.size.setW( inRef.size.getW() );

    if( inRef.mask & HEIGHT ) attr.size.setH( inRef.size.getH() );

    if( inRef.mask & DEPTH ) attr.size.setD( inRef.size.getD() );

    if( inRef.mask & LABEL ) attr.label = inRef.label;

    if( inRef.mask & HEAD_LABEL ) attr.headLabel = inRef.headLabel;

    if( inRef.mask & TAIL_LABEL ) attr.tailLabel = inRef.tailLabel;

    if( inRef.mask & LABEL ) attr.label = inRef.label;

    if( inRef.mask & FILL_COLOR ) attr.fillcolor = inRef.fillcolor;

    if( inRef.mask & FONT_COLOR ) attr.fontcolor = inRef.fontcolor;

    if( inRef.mask & COLOR ) attr.color = inRef.color;

    if( inRef.mask & STYLE ) {
      if (inRef.style == 1 && !(attr.mask & FILL_COLOR)) {
        // filled
        attr.fillcolor = attr.color;
        attr.mask |= FILL_COLOR;
      }

      attr.style = inRef.style;
    }

    if( inRef.mask & COMMENT ) attr.comment = inRef.comment;

    if( inRef.mask & URL ) attr.url = inRef.url;

    return attr;
  }

  void operator += ( const DOT_ATTR & inRef ) {
    *this = *this + inRef;
  }

  void setValue ( const std::string & inId,
                  const std::string & inValue ) {
    if( inId == "pos" || inId == "layout" ) {
      float x, y, z;

      if( sscanf(inValue.c_str(),"%f,%f,%f",&x,&y,&z) == 3 ) {
        layout = Coord(x,y,z);
        mask |= LAYOUT;
      }
      else if( sscanf(inValue.c_str(),"%f,%f",&x,&y) == 2 ) {
        layout = Coord(x,y,0);
        mask |= LAYOUT;
      }
      else if( sscanf(inValue.c_str(),"%f",&x) == 1 ) {
        layout = Coord(x,0,0);
        mask |= LAYOUT;
      }
    }
    else if( inId == "label" ) {
      label = inValue;
      mask |= LABEL;
    }
    else if( inId == "headlabel" ) {
      headLabel = inValue;
      mask |= HEAD_LABEL;
    }
    else if( inId == "taillabel" ) {
      tailLabel = inValue;
      mask |= TAIL_LABEL;
    }
    else if( inId == "fontcolor" ) { //label color
      Color c;

      if( DecodeColor(c,inValue) ) {
        fontcolor = c;
        mask |= FONT_COLOR;
      }
    }
    else if( inId == "color" ) { //border color
      Color c;

      if( DecodeColor(c,inValue) ) {
        color = c;
        mask |= COLOR;
      }
    }
    else if( inId == "style" ) { // color ?
      if (inValue == "filled")
        style = 1;
      else
        style = 2;

      mask |= STYLE;
    }
    else if( inId == "fillcolor" ) { //fill color
      Color c;

      if( DecodeColor(c,inValue) ) {
        fillcolor = c;
        mask |= FILL_COLOR;
      }
    }
    else if( inId == "width" ) {
      size.setW( atof(inValue.c_str()) );
      mask |= WIDTH;
    }
    else if( inId == "height" ) {
      size.setH( atof(inValue.c_str()) );
      mask |= HEIGHT;
    }
    else if( inId == "depth" ) {
      size.setD( atof(inValue.c_str()) );
      mask |= DEPTH;
    }
    else if( inId == "shape" ) {
      unsigned int N = sizeof(ShapeAttrA) / sizeof(ShapeAttr);

      for( unsigned int i = 0 ; i < N ; i++ ) {
        if( !strcasecmp(ShapeAttrA[i].name,inValue.c_str()) ) {
          shape = ShapeAttrA[i].idx;
          mask |= SHAPE;
          break;
        }
      }
    }
    else if( inId == "comment" ) {
      comment = inValue;
      mask |= COMMENT;
    }
    else if( inId == "URL" ) {
      url = inValue;
      mask |= URL;
    }
  }
};


//
// Bison semantic value
//

struct DOT_YYType {
  std::string s;
  bool created;
  bool subg;

  NodeA nodeA; // stmt node set
  EdgeA edgeA; // stmt edge set
  DOT_ATTR attr; // stmt accumulated attributes

  void clear() {
    s.resize(0);
    created = false;
    subg = false;
    nodeA.clear();
    edgeA.clear();
    attr.clear();
  }
};


//
// Global data needed by the parser
// as a current context.
//


struct DOT_YY {
  Graph * graph;
  FILE* fd;
  long fSize;
  long fOffset;
  PluginProgress* pProgress;
  ProgressState pStatus;
  NodeMap nodeMap;
  bool isDirected; // directed graph ?
  bool isStrict; // strict graph ?
  DOT_ATTR nodeAttr; // def. node's attribute
  DOT_ATTR edgeAttr; // def. edge's attribute
  DOT_ATTR subgAttr; // def. subgraph's attribute

  DOT_YY(FILE* file, Graph* g, PluginProgress* pluginProgress)
    : graph(g), fd(file), pProgress(pluginProgress), pStatus(TLP_CONTINUE),
      isDirected(true), isStrict(true) {
    fseek(file, 0, SEEK_END);
    fSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fOffset = 0;
  }

  void checkProgress() {
    if (pProgress) {
      long curOffset = ftell(fd);

      if (curOffset - fOffset >= fSize/1000) {
        fOffset = curOffset;
        pStatus = pProgress->progress(fOffset, fSize);

        if (pStatus != TLP_CONTINUE)
          // in order to stop the parsing we go to the end of the file
          fseek(fd, 0, SEEK_END);
      }
    }
  }

  // bind a node

  node bindNode( const std::string & inId,
                 bool & inCreated ) {
    NodeMap::iterator it = nodeMap.find( inId );

    if( it != nodeMap.end() ) {
      inCreated = false;
      return it->second;
    }

    node n = graph->addNode();
    checkProgress();
    inCreated = true;
    return nodeMap[inId] = n;
  }

  // bind some edges (cartesian product A X B)

  EdgeA bindEdge( const std::string & inOp, // "--" or "->"
                  const NodeA & inA,
                  const NodeA & inB ) {
    EdgeA edgeA;
    bool di = isStrict ? isDirected : ( inOp == "->" );

    // produit cartesien A x B
    for( unsigned int i = 0 ; i < inA.size(); i++ ) {
      for( unsigned int j = 0 ; j < inB.size() ; j++ ) {
        edgeA.push_back( graph->addEdge(inA[i],inB[j]) );

        if( !di ) {
          edgeA.push_back( graph->addEdge(inB[j],inA[i]) );
        }
      }
    }

    checkProgress();
    return edgeA;
  }

  EdgeA bindEdge( const std::string & inOp, // "--" or "->"
                  const NodeA & inA,
                  const node & inB ) {
    NodeA nA;
    nA.push_back( inB );
    return bindEdge( inOp, inA, nA );
  }

  EdgeA bindEdge( const std::string & inOp, // "--" or "->"
                  const node & inA,
                  const NodeA & inB ) {
    NodeA nA;
    nA.push_back( inA );
    return bindEdge( inOp, nA, inB );
  }

  EdgeA bindEdge( const std::string & inOp, // "--" or "->"
                  const node & inA,
                  const node & inB ) {
    NodeA nA, _nA;
    nA.push_back( inA );
    _nA.push_back( inB );
    return bindEdge( inOp, nA, _nA );
  }

  std::string doStringEscaping (const std::string &str) {
    std::string nstr(str);
    size_t bsPos = nstr.find('\\');

    while (bsPos != std::string::npos) {
      char c = nstr[bsPos + 1];

      if (c == 'n' || c == 'l' || c == 'r') {
        nstr.replace(bsPos, 2, 1, '\n');
        bsPos = nstr.find('\\', bsPos + 1);
      }
      else
        bsPos = nstr.find('\\', bsPos + 2);
    }

    return nstr;
  }

  // Setup nodes with a context of attributes

  void SetupNode( const NodeA & inA,
                  const DOT_ATTR & inAttr ) {
    // Layout

    if( inAttr.mask & DOT_ATTR::LAYOUT ) {
      LayoutProperty * layoutP = graph->getProperty<LayoutProperty>(TLP_LAYOUT_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        layoutP->setNodeValue( inA[i], inAttr.layout );
    }

    // Label

    if( (inAttr.mask & DOT_ATTR::LABEL) && inAttr.label.size() ) {
      StringProperty * stringP = graph->getProperty<StringProperty>(TLP_LABEL_PROXY_NAME);
      StringProperty * string2P = graph->getProperty<StringProperty>(TLP_EXTLABEL_PROXY_NAME);
      std::string label = doStringEscaping(inAttr.label);

      for( unsigned int i = 0 ; i < inA.size() ; i++ ) {
        stringP->setNodeValue( inA[i], label );
        string2P->setNodeValue( inA[i], inAttr.label );
      }
    }

    // Size

    {
      // because default shape is an ellipse
      // we distorse the size
      Size s( 0.75, 0.5f, 0.5f ); // in inches unit

      if ((inAttr.mask & DOT_ATTR::SHAPE ) &&
          (inAttr.shape != DEFAULT_SHAPE))
        // no distorsion if it not the default shape
        s.setW(0.5);

      if( inAttr.mask & DOT_ATTR::WIDTH ) s.setW( inAttr.size.getW() );

      if( inAttr.mask & DOT_ATTR::HEIGHT ) s.setH( inAttr.size.getH() );

      if( inAttr.mask & DOT_ATTR::DEPTH ) s.setD( inAttr.size.getD() );

      SizeProperty * sizeP = graph->getProperty<SizeProperty>(TLP_SIZE_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        sizeP->setNodeValue( inA[i], s );
    }

    // Color

    if( inAttr.mask & DOT_ATTR::FILL_COLOR ) {
      ColorProperty * colP = graph->getProperty<ColorProperty>(TLP_COLOR_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        colP->setNodeValue( inA[i], inAttr.fillcolor );
    }

    if( inAttr.mask & DOT_ATTR::COLOR ) {
      ColorProperty * colP = graph->getProperty<ColorProperty>(TLP_BORDERCOLOR_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        colP->setNodeValue( inA[i], inAttr.color );
    }

    if( inAttr.mask & DOT_ATTR::FONT_COLOR ) {
      ColorProperty * colP = graph->getProperty<ColorProperty>(TLP_FONTCOLOR_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        colP->setNodeValue( inA[i], inAttr.fontcolor );
    }

    // Shape
    {
      IntegerProperty * shapeP = graph->getProperty<IntegerProperty>(TLP_SHAPE_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ ) {
        int shape = DEFAULT_SHAPE;

        if( inAttr.mask & DOT_ATTR::SHAPE )
          shape = inAttr.shape;

        // default shape is an ellipse
        // we use a distored circle
        shapeP->setNodeValue( inA[i],  (shape != DEFAULT_SHAPE) ? shape : 2);
      }
    }

    // Comment

    if( inAttr.mask & DOT_ATTR::COMMENT ) {
      StringProperty * cmntP = graph->getProperty<StringProperty>(TLP_COMMENT_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        cmntP->setNodeValue( inA[i], inAttr.comment );
    }

    // URL

    if( (inAttr.mask & DOT_ATTR::URL) && inAttr.url.size() ) {
      StringProperty * urlP = graph->getProperty<StringProperty>(TLP_URL_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        urlP->setNodeValue( inA[i], inAttr.url );
    }
  }

  // Setup edges with a context of attributes

  void SetupEdge( const EdgeA & inA,
                  const DOT_ATTR & inAttr ) {
    if (!inAttr.mask)
      return;

    // Label

    if( (inAttr.mask & DOT_ATTR::LABEL) && inAttr.label.size() ) {
      StringProperty * stringP = graph->getProperty<StringProperty>(TLP_LABEL_PROXY_NAME);
      StringProperty * string2P = graph->getProperty<StringProperty>(TLP_EXTLABEL_PROXY_NAME);
      std::string label = doStringEscaping(inAttr.label);

      for( unsigned int i = 0 ; i < inA.size() ; i++ ) {
        stringP->setEdgeValue( inA[i], label );
        string2P->setEdgeValue( inA[i], inAttr.label );
      }
    }

    // Head-Label

    if( (inAttr.mask & DOT_ATTR::HEAD_LABEL) && inAttr.headLabel.size() ) {
      StringProperty * labelP = graph->getProperty<StringProperty>(TLP_HEAD_LABEL_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        labelP->setEdgeValue( inA[i], inAttr.headLabel );
    }

    // Tail-Label

    if( (inAttr.mask & DOT_ATTR::TAIL_LABEL) && inAttr.tailLabel.size() ) {
      StringProperty * labelP = graph->getProperty<StringProperty>(TLP_TAIL_LABEL_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        labelP->setEdgeValue( inA[i], inAttr.tailLabel );
    }

    // Color
    if( inAttr.mask & DOT_ATTR::COLOR ) {
      ColorProperty * colP = graph->getProperty<ColorProperty>(TLP_COLOR_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        colP->setEdgeValue( inA[i], inAttr.color );
    }

    // Comment

    if( inAttr.mask & DOT_ATTR::COMMENT ) {
      StringProperty * cmntP = graph->getProperty<StringProperty>(TLP_COMMENT_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        cmntP->setEdgeValue( inA[i], inAttr.comment );
    }

    // URL

    if( (inAttr.mask & DOT_ATTR::URL) && inAttr.url.size() ) {
      StringProperty * urlP = graph->getProperty<StringProperty>(TLP_URL_PROXY_NAME);

      for( unsigned int i = 0 ; i < inA.size() ; i++ )
        urlP->setEdgeValue( inA[i], inAttr.url );
    }
  }

} * dotyy;


#define YYSTYPE DOT_YYType

// Semantic value DOT_YYType is a C++ object, with constructor/destructor.
// Dynamic stack managment should be done in futur version.
// For a purpose of simplicity, this parser uses a large and fixed array.
// You can change theses values. Only take care to keep YYMAXDEPTH = YYINITDEPTH
// to avoid the dynamic relocation of the stack.
#define YYINITDEPTH 2048
#define YYMAXDEPTH YYINITDEPTH // No reallocation !



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 577 "dotImportParser.h"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
/* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
   || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc {
  short int yyss;
  YYSTYPE yyvs;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))     \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)    \
      do          \
  {         \
    register YYSIZE_T yyi;    \
    for (yyi = 0; yyi < (Count); yyi++) \
      (To)[yyi] = (From)[yyi];    \
  }         \
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)          \
    do                  \
      {                 \
  YYSIZE_T yynewbytes;            \
  YYCOPY (&yyptr->Stack, Stack, yysize);        \
  Stack = &yyptr->Stack;            \
  yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
  yyptr += yynewbytes / sizeof (*yyptr);        \
      }                 \
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
typedef signed char yysigned_char;
#else
typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   132

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  25
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  27
/* YYNRULES -- Number of rules. */
#define YYNRULES  62
/* YYNRULES -- Number of states. */
#define YYNSTATES  92

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   267

#define YYTRANSLATE(YYX)            \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] = {
  0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  21,    22,     2,    24,    15,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,    20,    19,
  2,    18,     2,     2,    23,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,    16,     2,    17,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,    13,     2,    14,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
  2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
  5,     6,     7,     8,     9,    10,    11,    12
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] = {
  0,     0,     3,     4,     6,    12,    13,    15,    17,    20,
  23,    24,    26,    27,    31,    32,    34,    38,    39,    42,
  44,    48,    50,    52,    55,    58,    61,    63,    64,    67,
  69,    72,    74,    76,    78,    80,    82,    85,    87,    88,
  90,    92,    95,    98,   101,   108,   111,   114,   119,   124,
  126,   128,   132,   136,   141,   146,   150,   152,   155,   157,
  159,   161,   163
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] = {
  26,     0,    -1,    -1,     1,    -1,    27,    28,    13,    37,
  14,    -1,    -1,     7,    -1,     8,    -1,     6,     7,    -1,
  6,     8,    -1,    -1,    50,    -1,    -1,    35,    30,    29,
  -1,    -1,    15,    -1,    16,    29,    17,    -1,    -1,    32,
  31,    -1,    32,    -1,    50,    18,    50,    -1,    34,    -1,
  50,    -1,     8,    31,    -1,     9,    31,    -1,    10,    31,
  -1,    34,    -1,    -1,    37,    38,    -1,    39,    -1,    39,
  19,    -1,     1,    -1,    45,    -1,    46,    -1,    36,    -1,
  48,    -1,    41,    42,    -1,    50,    -1,    -1,    43,    -1,
  44,    -1,    44,    43,    -1,    43,    44,    -1,    20,    50,
  -1,    20,    21,    50,    15,    50,    22,    -1,    23,    50,
  -1,    40,    33,    -1,    40,    12,    47,    33,    -1,    48,
  12,    47,    33,    -1,    40,    -1,    48,    -1,    40,    12,
  47,    -1,    48,    12,    47,    -1,    49,    13,    37,    14,
  -1,    11,    13,    37,    14,    -1,    13,    37,    14,    -1,
  49,    -1,    11,    50,    -1,     4,    -1,     3,    -1,    51,
  -1,     5,    -1,    51,    24,    51,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] = {
  0,   490,   490,   491,   492,   495,   496,   497,   498,   499,
  502,   503,   506,   507,   510,   511,   514,   517,   518,   521,
  524,   527,   528,   531,   532,   533,   534,   537,   538,   545,
  546,   547,   550,   551,   552,   553,   556,   559,   573,   574,
  575,   576,   577,   580,   581,   584,   587,   595,   607,   621,
  622,   623,   634,   647,   648,   649,   650,   653,   656,   657,
  658,   661,   662
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] = {
  "$end", "error", "$undefined", "NUMBER", "IDENT", "STRING", "_STRICT",
  "_DIGRAPH", "_GRAPH", "_NODE", "_EDGE", "_SUBGRAPH", "_EDGEOP", "'{'",
  "'}'", "','", "'['", "']'", "'='", "';'", "':'", "'('", "')'", "'@'",
  "'+'", "$accept", "file", "graph_type", "graph_name", "inside_attr_list",
  "optcomma", "attr_list", "rec_attr_list", "opt_attr_list", "attr_set",
  "iattr_set", "attr_stmt", "stmt_list", "stmt", "stmt1", "node_id",
  "node_name", "node_port", "port_location", "port_angle", "node_stmt",
  "edge_stmt", "edgeRHS", "subg_stmt", "subg_hdr", "id", "qid", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] = {
  0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
  265,   266,   267,   123,   125,    44,    91,    93,    61,    59,
  58,    40,    41,    64,    43
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] = {
  0,    25,    26,    26,    26,    27,    27,    27,    27,    27,
  28,    28,    29,    29,    30,    30,    31,    32,    32,    33,
  34,    35,    35,    36,    36,    36,    36,    37,    37,    38,
  38,    38,    39,    39,    39,    39,    40,    41,    42,    42,
  42,    42,    42,    43,    43,    44,    45,    46,    46,    47,
  47,    47,    47,    48,    48,    48,    48,    49,    50,    50,
  50,    51,    51
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] = {
  0,     2,     0,     1,     5,     0,     1,     1,     2,     2,
  0,     1,     0,     3,     0,     1,     3,     0,     2,     1,
  3,     1,     1,     2,     2,     2,     1,     0,     2,     1,
  2,     1,     1,     1,     1,     1,     2,     1,     0,     1,
  1,     2,     2,     2,     6,     2,     2,     4,     4,     1,
  1,     3,     3,     4,     4,     3,     1,     2,     1,     1,
  1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] = {
  0,     3,     0,     6,     7,     0,    10,     8,     9,     1,
  59,    58,    61,     0,    11,    60,    27,     0,     0,    62,
  31,     0,     0,     0,     0,    27,     4,    26,    34,    28,
  29,    17,    38,    32,    33,    35,    56,    37,    12,    23,
  24,    25,    27,    57,     0,    30,     0,    19,    46,     0,
  0,    36,    39,    40,     0,    27,     0,     0,    21,    14,
  22,     0,    55,    49,    17,    50,    37,    18,     0,    43,
  45,    42,    41,    17,     0,    20,    16,    15,    12,    54,
  0,    47,     0,     0,    48,    53,    13,    51,    52,     0,
  0,    44
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] = {
  -1,     5,     6,    13,    57,    78,    39,    47,    48,    27,
  59,    28,    18,    29,    30,    31,    32,    51,    52,    53,
  33,    34,    64,    35,    36,    37,    15
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -50
static const yysigned_char yypact[] = {
  119,   -50,     6,   -50,   -50,    20,    65,   -50,   -50,   -50,
  -50,   -50,   -50,    13,   -50,     5,   -50,    22,    50,     5,
  -50,    14,    14,    14,    12,   -50,   -50,   -50,   -50,   -50,
  16,    30,   -11,   -50,   -50,    33,    36,    28,    65,   -50,
  -50,   -50,   -50,   -50,    76,   -50,    62,    14,   -50,     3,
  65,   -50,    29,    37,    62,   -50,    65,    39,   -50,    56,
  28,    90,   -50,    66,   -50,    70,   -50,   -50,    65,   -50,
  -50,   -50,   -50,   -50,   104,   -50,   -50,   -50,    65,   -50,
  62,   -50,    62,    73,   -50,   -50,   -50,   -50,   -50,    65,
  74,   -50
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] = {
  -50,   -50,   -50,   -50,    19,   -50,     0,   -50,   -45,   -37,
  -50,   -50,   -21,   -50,   -50,   -44,   -50,   -50,    49,    40,
  -50,   -50,   -49,   -43,   -50,    -6,    89
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -6
static const yysigned_char yytable[] = {
  14,    58,    63,    65,    44,    73,    10,    11,    12,    49,
  63,    65,    50,     7,     8,    10,    11,    12,    43,    81,
  9,    61,    40,    41,    68,    42,    16,    12,    84,    17,
  38,    87,    60,    88,    74,    45,    63,    65,    63,    65,
  66,    58,    46,    69,    70,    54,    56,    67,    66,    55,
  75,    20,    50,    10,    11,    12,    76,    49,    21,    22,
  23,    24,    83,    25,    26,    10,    11,    12,    10,    11,
  12,    77,    60,    24,    66,    25,    66,    20,    80,    10,
  11,    12,    82,    90,    21,    22,    23,    24,    89,    25,
  62,    20,    71,    10,    11,    12,    91,    86,    21,    22,
  23,    24,    72,    25,    79,    20,    19,    10,    11,    12,
  0,     0,    21,    22,    23,    24,     0,    25,    85,    -2,
  1,     0,    -5,    -5,    -5,     2,     3,     4,     0,     0,
  0,     0,    -5
};

static const yysigned_char yycheck[] = {
  6,    38,    46,    46,    25,    54,     3,     4,     5,    20,
  54,    54,    23,     7,     8,     3,     4,     5,    24,    64,
  0,    42,    22,    23,    21,    13,    13,     5,    73,    24,
  16,    80,    38,    82,    55,    19,    80,    80,    82,    82,
  46,    78,    12,    49,    50,    12,    18,    47,    54,    13,
  56,     1,    23,     3,     4,     5,    17,    20,     8,     9,
  10,    11,    68,    13,    14,     3,     4,     5,     3,     4,
  5,    15,    78,    11,    80,    13,    82,     1,    12,     3,
  4,     5,    12,    89,     8,     9,    10,    11,    15,    13,
  14,     1,    52,     3,     4,     5,    22,    78,     8,     9,
  10,    11,    53,    13,    14,     1,    17,     3,     4,     5,
  -1,    -1,     8,     9,    10,    11,    -1,    13,    14,     0,
  1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
  -1,    -1,    13
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] = {
  0,     1,     6,     7,     8,    26,    27,     7,     8,     0,
  3,     4,     5,    28,    50,    51,    13,    24,    37,    51,
  1,     8,     9,    10,    11,    13,    14,    34,    36,    38,
  39,    40,    41,    45,    46,    48,    49,    50,    16,    31,
  31,    31,    13,    50,    37,    19,    12,    32,    33,    20,
  23,    42,    43,    44,    12,    13,    18,    29,    34,    35,
  50,    37,    14,    40,    47,    48,    50,    31,    21,    50,
  50,    44,    43,    47,    37,    50,    17,    15,    30,    14,
  12,    33,    12,    50,    33,    14,    29,    47,    47,    15,
  50,    22
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok   (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)
#define YYEMPTY   (-2)
#define YYEOF   0

#define YYACCEPT  goto yyacceptlab
#define YYABORT   goto yyabortlab
#define YYERROR   goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL    goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)          \
do                \
  if (yychar == YYEMPTY && yylen == 1)        \
    {               \
      yychar = (Token);           \
      yylval = (Value);           \
      yytoken = YYTRANSLATE (yychar);       \
      YYPOPSTACK;           \
      goto yybackup;            \
    }               \
  else                \
    {                 \
      yyerror ("syntax error: cannot back up");\
      YYERROR;              \
    }               \
while (0)


#define YYTERROR  1
#define YYERRCODE 256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)        \
    do                  \
      if (N)                \
  {               \
    (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;  \
    (Current).first_column = YYRHSLOC (Rhs, 1).first_column;  \
    (Current).last_line    = YYRHSLOC (Rhs, N).last_line;   \
    (Current).last_column  = YYRHSLOC (Rhs, N).last_column; \
  }               \
      else                \
  {               \
    (Current).first_line   = (Current).last_line   =    \
      YYRHSLOC (Rhs, 0).last_line;        \
    (Current).first_column = (Current).last_column =    \
      YYRHSLOC (Rhs, 0).last_column;        \
  }               \
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)      \
     fprintf (File, "%d.%d-%d.%d",      \
              (Loc).first_line, (Loc).first_column, \
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)      \
do {            \
  if (yydebug)          \
    YYFPRINTF Args;       \
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)    \
do {                \
  if (yydebug)              \
    {               \
      YYFPRINTF (stderr, "%s ", Title);       \
      yysymprint (stderr,           \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");         \
    }               \
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
short int *bottom;
short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");

  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);

  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)        \
do {                \
  if (yydebug)              \
    yy_stack_print ((Bottom), (Top));       \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);

  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);

  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)    \
do {          \
  if (yydebug)        \
    yy_reduce_print (Rule);   \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Returns the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
char *yydest;
const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
FILE *yyoutput;
int yytype;
YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT

  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);

# endif

  switch (yytype) {
  default:
    break;
  }

  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
const char *yymsg;
int yytype;
YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";

  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype) {

  default:
    break;
  }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;   /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  yyvsp[0] = yylval;

  goto yysetstate;

  /*------------------------------------------------------------.
  | yynewstate -- Push a new state, which is found in yystate.  |
  `------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp) {
    /* Get the current used size of the three stacks, in elements.  */
    YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
    {
      /* Give user a chance to reallocate the stack. Use copies of
         these so that the &'s don't force the real ones into
         memory.  */
      YYSTYPE *yyvs1 = yyvs;
      short int *yyss1 = yyss;


      /* Each stack pointer address is followed by the size of the
         data in use in that stack, in bytes.  This used to be a
         conditional around just the two extra args, but that might
         be undefined if yyoverflow is a macro.  */
      yyoverflow ("parser stack overflow",
                  &yyss1, yysize * sizeof (*yyssp),
                  &yyvs1, yysize * sizeof (*yyvsp),

                  &yystacksize);

      yyss = yyss1;
      yyvs = yyvs1;
    }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
    goto yyoverflowlab;
# else

    /* Extend the stack our own way.  */
    if (YYMAXDEPTH <= yystacksize)
      goto yyoverflowlab;

    yystacksize *= 2;

    if (YYMAXDEPTH < yystacksize)
      yystacksize = YYMAXDEPTH;

    {
      short int *yyss1 = yyss;
      union yyalloc *yyptr =
            (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));

      if (! yyptr)
        goto yyoverflowlab;

      YYSTACK_RELOCATE (yyss);
      YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE

      if (yyss1 != yyssa)
        YYSTACK_FREE (yyss1);
    }
# endif
#endif /* no yyoverflow */

    yyssp = yyss + yysize - 1;
    yyvsp = yyvs + yysize - 1;


    YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                (unsigned long int) yystacksize));

    if (yyss + yystacksize - 1 <= yyssp)
      YYABORT;
  }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

  /*-----------.
  | yybackup.  |
  `-----------*/
yybackup:

  /* Do appropriate processing given the current state.  */
  /* Read a look-ahead token if we need one and don't already have one.  */
  /* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];

  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY) {
    YYDPRINTF ((stderr, "Reading a token: "));
    yychar = YYLEX;
  }

  if (yychar <= YYEOF) {
    yychar = yytoken = YYEOF;
    YYDPRINTF ((stderr, "Now at end of input.\n"));
  }
  else {
    yytoken = YYTRANSLATE (yychar);
    YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
  }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;

  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;

  yyn = yytable[yyn];

  if (yyn <= 0) {
    if (yyn == 0 || yyn == YYTABLE_NINF)
      goto yyerrlab;

    yyn = -yyn;
    goto yyreduce;
  }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];

  if (yyn == 0)
    goto yyerrlab;

  goto yyreduce;


  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);

  switch (yyn) {
  case 6:
#line 496 "dotImportParser.y"
    {
      dotyy->isDirected = true;
      dotyy->isStrict = false; ;
    }
    break;

  case 7:
#line 497 "dotImportParser.y"
    {
      dotyy->isDirected = false;
      dotyy->isStrict = false; ;
    }
    break;

  case 8:
#line 498 "dotImportParser.y"
    {
      dotyy->isDirected = true;
      dotyy->isStrict = true; ;
    }
    break;

  case 9:
#line 499 "dotImportParser.y"
    {
      dotyy->isDirected = false;
      dotyy->isStrict = true; ;
    }
    break;

  case 12:
#line 506 "dotImportParser.y"
    {
      (yyval).clear(); ;
    }
    break;

  case 13:
#line 507 "dotImportParser.y"
    {
      (yyval.attr) = (yyvsp[-2].attr) + (yyvsp[0].attr); ;
    }
    break;

  case 16:
#line 514 "dotImportParser.y"
    {
      (yyval) = (yyvsp[-1]); ;
    }
    break;

  case 17:
#line 517 "dotImportParser.y"
    {
      (yyval).clear(); ;
    }
    break;

  case 18:
#line 518 "dotImportParser.y"
    {
      (yyval.attr) = (yyvsp[-1].attr) + (yyvsp[0].attr); ;
    }
    break;

  case 19:
#line 521 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 20:
#line 524 "dotImportParser.y"
    {
      (yyval).clear();
      (yyval.attr).setValue( (yyvsp[-2].s), (yyvsp[0].s) ); ;
    }
    break;

  case 21:
#line 527 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 22:
#line 528 "dotImportParser.y"
    {
      (yyval).clear(); ;
    }
    break;

  case 23:
#line 531 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]);
      dotyy->subgAttr += (yyvsp[0].attr); ;
    }
    break;

  case 24:
#line 532 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]);
      dotyy->nodeAttr += (yyvsp[0].attr); ;
    }
    break;

  case 25:
#line 533 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]);
      dotyy->edgeAttr += (yyvsp[0].attr); ;
    }
    break;

  case 26:
#line 534 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 27:
#line 537 "dotImportParser.y"
    {
      (yyval).clear(); ;
    }
    break;

  case 28:
#line 538 "dotImportParser.y"
    {
      (yyval) = (yyvsp[-1]);
      (yyval.nodeA) += (yyvsp[0].nodeA);
      (yyval.edgeA) += (yyvsp[0].edgeA);
      ;
    }
    break;

  case 29:
#line 545 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 30:
#line 546 "dotImportParser.y"
    {
      (yyval) = (yyvsp[-1]); ;
    }
    break;

  case 32:
#line 550 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 33:
#line 551 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 34:
#line 552 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 35:
#line 553 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 36:
#line 556 "dotImportParser.y"
    {
      (yyval) = (yyvsp[-1]); ;
    }
    break;

  case 37:
#line 559 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]);
      node n = dotyy->bindNode( (yyval.s), (yyval.created) );
      (yyval.nodeA).clear();
      (yyval.nodeA).push_back( n );
      (yyval.attr) = dotyy->nodeAttr;

      if( (yyval.created) ) {
        if( !((yyval.attr).mask & DOT_ATTR::LABEL) || (yyval.attr).label.empty() )
          (yyval.attr).setValue( "label", (yyval.s) );

        dotyy->SetupNode( (yyval.nodeA), (yyval.attr) );
      }

      ;
    }
    break;

  case 46:
#line 587 "dotImportParser.y"
    {
      (yyval) = (yyvsp[-1]);
      (yyval.attr) += (yyvsp[0].attr);
      // if( $<created>$ )
      dotyy->SetupNode( (yyval.nodeA), (yyval.attr) );
      ;
    }
    break;

  case 47:
#line 595 "dotImportParser.y"
    {
      (yyval).clear();
      (yyval.nodeA) = (yyvsp[-3].nodeA);
      (yyval.nodeA) += (yyvsp[-1].nodeA);
      (yyval.edgeA) = (yyvsp[-3].edgeA);
      (yyval.edgeA) += (yyvsp[-1].edgeA);

      if( (yyvsp[-1].subg) )
        (yyval.edgeA) += dotyy->bindEdge( (yyvsp[-2].s), (yyvsp[-3].nodeA), (yyvsp[-1].nodeA) );
      else if( (yyvsp[-1].nodeA).size() )
        (yyval.edgeA) += dotyy->bindEdge( (yyvsp[-2].s), (yyvsp[-3].nodeA), (yyvsp[-1].nodeA)[0]);

      dotyy->SetupEdge( (yyval.edgeA), dotyy->edgeAttr + (yyvsp[0].attr) );
      ;
    }
    break;

  case 48:
#line 607 "dotImportParser.y"
    {
      (yyval).clear();
      (yyval.nodeA) = (yyvsp[-3].nodeA);
      (yyval.nodeA) += (yyvsp[-1].nodeA);
      (yyval.edgeA) = (yyvsp[-3].edgeA);
      (yyval.edgeA) += (yyvsp[-1].edgeA);

      if( (yyvsp[-1].subg) )
        (yyval.edgeA) += dotyy->bindEdge( (yyvsp[-2].s), (yyvsp[-3].nodeA), (yyvsp[-1].nodeA) );
      else if( (yyvsp[-1].nodeA).size() )
        (yyval.edgeA) += dotyy->bindEdge( (yyvsp[-2].s), (yyvsp[-3].nodeA), (yyvsp[-1].nodeA)[0]);

      dotyy->SetupEdge( (yyval.edgeA), dotyy->edgeAttr + (yyvsp[0].attr) );
      ;
    }
    break;

  case 49:
#line 621 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 50:
#line 622 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 51:
#line 623 "dotImportParser.y"
    {
      (yyval).clear();
      (yyval.nodeA) = (yyvsp[-2].nodeA);
      (yyval.nodeA) += (yyvsp[0].nodeA);
      (yyval.edgeA) = (yyvsp[-2].edgeA);
      (yyval.edgeA) += (yyvsp[0].edgeA);

      if( (yyvsp[0].subg) )
        (yyval.edgeA) += dotyy->bindEdge( (yyvsp[-1].s), (yyvsp[-2].nodeA), (yyvsp[0].nodeA) );
      else if( (yyvsp[0].nodeA).size() )
        (yyval.edgeA) += dotyy->bindEdge( (yyvsp[-1].s), (yyvsp[-2].nodeA), (yyvsp[0].nodeA)[0]);

      ;
    }
    break;

  case 52:
#line 634 "dotImportParser.y"
    {
      (yyval).clear();
      (yyval.nodeA) = (yyvsp[-2].nodeA);
      (yyval.nodeA) += (yyvsp[0].nodeA);
      (yyval.edgeA) = (yyvsp[-2].edgeA);
      (yyval.edgeA) += (yyvsp[0].edgeA);

      if( (yyvsp[0].subg) )
        (yyval.edgeA) += dotyy->bindEdge( (yyvsp[-1].s), (yyvsp[-2].nodeA), (yyvsp[0].nodeA) );
      else if( (yyvsp[0].nodeA).size() )
        (yyval.edgeA) += dotyy->bindEdge( (yyvsp[-1].s), (yyvsp[-2].nodeA), (yyvsp[0].nodeA)[0]);

      ;
    }
    break;

  case 53:
#line 647 "dotImportParser.y"
    {
      (yyval) = (yyvsp[-1]);
      (yyval.subg) = true; ;
    }
    break;

  case 54:
#line 648 "dotImportParser.y"
    {
      (yyval) = (yyvsp[-1]);
      (yyval.subg) = true; ;
    }
    break;

  case 55:
#line 649 "dotImportParser.y"
    {
      (yyval) = (yyvsp[-1]);
      (yyval.subg) = true; ;
    }
    break;

  case 56:
#line 650 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]);
      (yyval.subg) = true; ;
    }
    break;

  case 57:
#line 653 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 58:
#line 656 "dotImportParser.y"
    {
      (yyval).clear();
      (yyval.s) = (yyvsp[0].s); ;
    }
    break;

  case 59:
#line 657 "dotImportParser.y"
    {
      (yyval).clear();
      (yyval.s) = (yyvsp[0].s); ;
    }
    break;

  case 60:
#line 658 "dotImportParser.y"
    {
      (yyval) = (yyvsp[0]); ;
    }
    break;

  case 61:
#line 661 "dotImportParser.y"
    {
      (yyval).clear();
      (yyval.s) = (yyvsp[0].s); ;
    }
    break;

  case 62:
#line 662 "dotImportParser.y"
    {
      (yyval.s) = (yyvsp[-2].s) + (yyvsp[0].s); ;
    }
    break;


  }

  /* Line 1037 of yacc.c.  */
#line 1859 "dotImportParser.h"
  
  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;

  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
yyerrlab:

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus) {
    ++yynerrs;
#if YYERROR_VERBOSE
    yyn = yypact[yystate];

    if (YYPACT_NINF < yyn && yyn < YYLAST) {
      YYSIZE_T yysize = 0;
      int yytype = YYTRANSLATE (yychar);
      const char* yyprefix;
      char *yymsg;
      int yyx;

      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 0;

      yyprefix = ", expecting ";

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR) {
          yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
          yycount += 1;

          if (yycount == 5) {
            yysize = 0;
            break;
          }
        }

      yysize += (sizeof ("syntax error, unexpected ")
                 + yystrlen (yytname[yytype]));
      yymsg = (char *) YYSTACK_ALLOC (yysize);

      if (yymsg != 0) {
        char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
        yyp = yystpcpy (yyp, yytname[yytype]);

        if (yycount < 5) {
          yyprefix = ", expecting ";

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR) {
              yyp = yystpcpy (yyp, yyprefix);
              yyp = yystpcpy (yyp, yytname[yyx]);
              yyprefix = " or ";
            }
        }

        yyerror (yymsg);
        YYSTACK_FREE (yymsg);
      }
      else
        yyerror ("syntax error; also virtual memory exhausted");
    }
    else
#endif /* YYERROR_VERBOSE */
      yyerror ("syntax error");
  }



  if (yyerrstatus == 3) {
    /* If just tried and failed to reuse look-ahead token after an
       error, discard it.  */

    if (yychar <= YYEOF) {
      /* If at end of input, pop the error token,
             then the rest of the stack, then return failure.  */
      if (yychar == YYEOF)
        for (;;) {

          YYPOPSTACK;

          if (yyssp == yyss)
            YYABORT;

          yydestruct ("Error: popping",
                      yystos[*yyssp], yyvsp);
        }
    }
    else {
      yydestruct ("Error: discarding", yytoken, &yylval);
      yychar = YYEMPTY;
    }
  }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__

  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
    goto yyerrorlab;

#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;  /* Each real token shifted decrements this.  */

  for (;;) {
    yyn = yypact[yystate];

    if (yyn != YYPACT_NINF) {
      yyn += YYTERROR;

      if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR) {
        yyn = yytable[yyn];

        if (0 < yyn)
          break;
      }
    }

    /* Pop the current state because it cannot handle the error token.  */
    if (yyssp == yyss)
      YYABORT;


    yydestruct ("Error: popping", yystos[yystate], yyvsp);
    YYPOPSTACK;
    yystate = *yyssp;
    YY_STACK_PRINT (yyss, yyssp);
  }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
yyabortlab:
  yydestruct ("Error: discarding lookahead",
              yytoken, &yylval);
  yychar = YYEMPTY;
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
  /*----------------------------------------------.
  | yyoverflowlab -- parser overflow comes here.  |
  `----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow

  if (yyss != yyssa)
    YYSTACK_FREE (yyss);

#endif
  return yyresult;
}


#line 666 "dotImportParser.y"







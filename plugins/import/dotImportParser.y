//
// DOT language parser for the Tulip software
// [ http://www.research.att.com/sw/tools/graphviz/ ]
//
// 08/02/2004 - Gerald Gainant (aka maGicG)
//
//		First (quick) support of the AT&T DOT language
//		o main graph entities are extracted (node/edges)
//		o subgraphes are not already supported
//		o several attributes (node & edge) are supported
//		o based on a modified grammar file available with the graphviz' softwares
//		o this parser can be largely optimised ...
//


%{

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
#define DEFAULT_SHAPE 1 // ellipse


 int yylex( void );
 void yyerror( char const * ) {}
 extern char * yytext;
extern int yylen;

typedef stdext::hash_map<std::string,node> NodeMap;
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

struct ShapeAttr
{
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
  {"ellipse", 2}, // as a sphere
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

  DOT_ATTR ( ){
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
    if( inRef.mask & STYLE ) attr.style = inRef.style;
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
      } else if( sscanf(inValue.c_str(),"%f,%f",&x,&y) == 2 ) {
	layout = Coord(x,y,0);
	mask |= LAYOUT;
      } else if( sscanf(inValue.c_str(),"%f",&x) == 1 ) {
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
    else if( inId == "fontcolor" ) { //border color
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
  DOT_YY() : sg(0), isDirected(true), isStrict(true) {}

  Graph * sg;
  NodeMap nodeMap;
  bool isDirected; // directed graph ?
  bool isStrict; // strict graph ?
  DOT_ATTR nodeAttr; // def. node's attribute
  DOT_ATTR edgeAttr; // def. edge's attribute
  DOT_ATTR subgAttr; // def. subgraph's attribute

  // bind a node

  node bindNode( const std::string & inId,
		 bool & inCreated ) {
    NodeMap::iterator it = nodeMap.find( inId );
    if( it != nodeMap.end() ) {
      inCreated = false;
      return it->second;
    }
    node n = sg->addNode();
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
	edgeA.push_back( sg->addEdge(inA[i],inB[j]) );
	if( !di )
	  edgeA.push_back( sg->addEdge(inB[j],inA[i]) );
      }
    }
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
      } else
	bsPos = nstr.find('\\', bsPos + 2);
    }
    return nstr;
  }

  // Setup nodes with a context of attributes

  void SetupNode( const NodeA & inA,
		  const DOT_ATTR & inAttr ) {
    // Layout

    if( inAttr.mask & DOT_ATTR::LAYOUT ) {
      LayoutProperty * layoutP = sg->getProperty<LayoutProperty>(TLP_LAYOUT_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	layoutP->setNodeValue( inA[i], inAttr.layout );
    }

    // Label

    if( (inAttr.mask & DOT_ATTR::LABEL) && inAttr.label.size() ) {
      StringProperty * stringP = sg->getProperty<StringProperty>(TLP_LABEL_PROXY_NAME);
      StringProperty * string2P = sg->getProperty<StringProperty>(TLP_EXTLABEL_PROXY_NAME);
      std::string label = doStringEscaping(inAttr.label);
      for( unsigned int i = 0 ; i < inA.size() ; i++ ) {
	stringP->setNodeValue( inA[i], label );
	string2P->setNodeValue( inA[i], inAttr.label );
      }
    }

    // Size

    {
      Size s( 0.75f, 0.5f, 0.5f ); // in inches unit
      if( inAttr.mask & DOT_ATTR::WIDTH ) s.setW( inAttr.size.getW() );
      if( inAttr.mask & DOT_ATTR::HEIGHT ) s.setH( inAttr.size.getH() );
      if( inAttr.mask & DOT_ATTR::DEPTH ) s.setD( inAttr.size.getD() );
      SizeProperty * sizeP = sg->getProperty<SizeProperty>(TLP_SIZE_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	sizeP->setNodeValue( inA[i], s );
    }

    // Color

    if( inAttr.mask & DOT_ATTR::FILL_COLOR ) {
      ColorProperty * colP = sg->getProperty<ColorProperty>(TLP_COLOR_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	colP->setNodeValue( inA[i], inAttr.fillcolor );
    }
    if( inAttr.mask & DOT_ATTR::COLOR ) {
      ColorProperty * colP = sg->getProperty<ColorProperty>(TLP_BORDERCOLOR_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	colP->setNodeValue( inA[i], inAttr.color );
    }
    if( inAttr.mask & DOT_ATTR::FONT_COLOR ) {
      ColorProperty * colP = sg->getProperty<ColorProperty>(TLP_FONTCOLOR_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	colP->setNodeValue( inA[i], inAttr.fontcolor );
    }

    // Shape
    {
      IntegerProperty * shapeP = sg->getProperty<IntegerProperty>(TLP_SHAPE_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	if( inAttr.mask & DOT_ATTR::SHAPE )
	  shapeP->setNodeValue( inA[i], inAttr.shape );
	else
	  shapeP->setNodeValue( inA[i], DEFAULT_SHAPE );
    }

    // Comment

    if( inAttr.mask & DOT_ATTR::COMMENT ) {
      StringProperty * cmntP = sg->getProperty<StringProperty>(TLP_COMMENT_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	cmntP->setNodeValue( inA[i], inAttr.comment );
    }

    // URL

    if( (inAttr.mask & DOT_ATTR::URL) && inAttr.url.size() ) {
      StringProperty * urlP = sg->getProperty<StringProperty>(TLP_URL_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	urlP->setNodeValue( inA[i], inAttr.url );
    }
  }

  // Setup edges with a context of attributes

  void SetupEdge( const EdgeA & inA,
		  const DOT_ATTR & inAttr ) {
    // Label

    if( (inAttr.mask & DOT_ATTR::LABEL) && inAttr.label.size() ) {
      StringProperty * stringP = sg->getProperty<StringProperty>(TLP_LABEL_PROXY_NAME);
      StringProperty * string2P = sg->getProperty<StringProperty>(TLP_EXTLABEL_PROXY_NAME);
      std::string label = doStringEscaping(inAttr.label);
      for( unsigned int i = 0 ; i < inA.size() ; i++ ) {
	stringP->setEdgeValue( inA[i], label );
	string2P->setEdgeValue( inA[i], inAttr.label );
      }
    }

    // Head-Label

    if( (inAttr.mask & DOT_ATTR::HEAD_LABEL) && inAttr.headLabel.size() ) {
      StringProperty * labelP = sg->getProperty<StringProperty>(TLP_HEAD_LABEL_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	labelP->setEdgeValue( inA[i], inAttr.headLabel );
    }

    // Tail-Label

    if( (inAttr.mask & DOT_ATTR::TAIL_LABEL) && inAttr.tailLabel.size() ) {
      StringProperty * labelP = sg->getProperty<StringProperty>(TLP_TAIL_LABEL_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	labelP->setEdgeValue( inA[i], inAttr.tailLabel );
    }

    // Color
    if( inAttr.mask & DOT_ATTR::COLOR ) {
      ColorProperty * colP = sg->getProperty<ColorProperty>(TLP_COLOR_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	colP->setEdgeValue( inA[i], inAttr.color );
    }

    // Comment

    if( inAttr.mask & DOT_ATTR::COMMENT ) {
      StringProperty * cmntP = sg->getProperty<StringProperty>(TLP_COMMENT_PROXY_NAME);
      for( unsigned int i = 0 ; i < inA.size() ; i++ )
	cmntP->setEdgeValue( inA[i], inAttr.comment );
    }

    // URL

    if( (inAttr.mask & DOT_ATTR::URL) && inAttr.url.size() ) {
      StringProperty * urlP = sg->getProperty<StringProperty>(TLP_URL_PROXY_NAME);
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

%}


%token NUMBER IDENT STRING
%token _STRICT _DIGRAPH _GRAPH _NODE _EDGE _SUBGRAPH _EDGEOP

%start file

%%

file : /* empty */
 | error
 | graph_type graph_name '{' stmt_list '}'
 ;

graph_type : /* empty */
 | _DIGRAPH { dotyy->isDirected = true; dotyy->isStrict = false; }
 | _GRAPH { dotyy->isDirected = false; dotyy->isStrict = false; }
 | _STRICT _DIGRAPH { dotyy->isDirected = true; dotyy->isStrict = true; }
 | _STRICT _GRAPH { dotyy->isDirected = false; dotyy->isStrict = true; }
 ;

graph_name : /* empty */
 | id
 ;

inside_attr_list : /* empty */ { $$.clear(); }
 | iattr_set optcomma inside_attr_list { $<attr>$ = $<attr>1 + $<attr>3; }
 ;

optcomma : /* empty */
 | ','
 ;

attr_list : '[' inside_attr_list ']' { $$ = $2; }
 ;

rec_attr_list : /* empty */ { $$.clear(); }
 | rec_attr_list attr_list { $<attr>$ = $<attr>1 + $<attr>2; }
 ;

opt_attr_list : rec_attr_list { $$ = $1; }
 ;

attr_set : id '=' id { $$.clear(); $<attr>$.setValue( $<s>1, $<s>3 ); }
 ;

iattr_set : attr_set { $$ = $1; }
 | id { $$.clear(); }
 ;

attr_stmt : _GRAPH attr_list { $$ = $2; dotyy->subgAttr += $<attr>2; }
 | _NODE attr_list { $$ = $2; dotyy->nodeAttr += $<attr>2; }
 | _EDGE attr_list { $$ = $2; dotyy->edgeAttr += $<attr>2; }
 | attr_set { $$ = $1; }
 ;

stmt_list : /* empty */ { $$.clear(); }
 | stmt_list stmt {
 $$ = $1;
 $<nodeA>$ += $<nodeA>2;
 $<edgeA>$ += $<edgeA>2;
 }
 ;

stmt : stmt1 { $$ = $1; }
 | stmt1 ';' { $$ = $1; }
 | error
 ;

stmt1 : node_stmt { $$ = $1; }
 | edge_stmt { $$ = $1; }
 | attr_stmt { $$ = $1; }
 | subg_stmt { $$ = $1; }
 ;

node_id : node_name node_port { $$ = $1; }
 ;

node_name : id {
 $$ = $1;
 node n = dotyy->bindNode( $<s>$, $<created>$ );
 $<nodeA>$.clear();
 $<nodeA>$.push_back( n );
 $<attr>$ = dotyy->nodeAttr;
 if( $<created>$ ) {
 if( !($<attr>$.mask & DOT_ATTR::LABEL) || $<attr>$.label.size()==0 )
 $<attr>$.setValue( "label", $<s>$ );
 dotyy->SetupNode( $<nodeA>$, $<attr>$ );
 }
 }
 ;

node_port : /* empty */
 | port_location
 | port_angle /* undocumented */
 | port_angle port_location /* undocumented */
 | port_location port_angle /* undocumented */
 ;

port_location : ':' id
 | ':' '(' id ',' id ')'
 ;

port_angle : '@' id
 ;

node_stmt : node_id opt_attr_list {
  $$ = $1;
  $<attr>$ += $<attr>2;
  // if( $<created>$ )
  dotyy->SetupNode( $<nodeA>$, $<attr>$ );
}
;

edge_stmt : node_id _EDGEOP edgeRHS opt_attr_list {
  $$.clear();
  $<nodeA>$ = $<nodeA>1;
  $<nodeA>$ += $<nodeA>3;
  $<edgeA>$ = $<edgeA>1;
  $<edgeA>$ += $<edgeA>3;
  if( $<subg>3 )
    $<edgeA>$ += dotyy->bindEdge( $<s>2, $<nodeA>1, $<nodeA>3 );
  else if( $<nodeA>3.size() )
    $<edgeA>$ += dotyy->bindEdge( $<s>2, $<nodeA>1, $<nodeA>3[0]);
  dotyy->SetupEdge( $<edgeA>$, dotyy->edgeAttr + $<attr>4 );
}
 | subg_stmt _EDGEOP edgeRHS opt_attr_list {
 $$.clear();
 $<nodeA>$ = $<nodeA>1;
 $<nodeA>$ += $<nodeA>3;
 $<edgeA>$ = $<edgeA>1;
 $<edgeA>$ += $<edgeA>3;
 if( $<subg>3 )
 $<edgeA>$ += dotyy->bindEdge( $<s>2, $<nodeA>1, $<nodeA>3 );
 else if( $<nodeA>3.size() )
 $<edgeA>$ += dotyy->bindEdge( $<s>2, $<nodeA>1, $<nodeA>3[0]);
 dotyy->SetupEdge( $<edgeA>$, dotyy->edgeAttr + $<attr>4 );
 }
 ;

edgeRHS : node_id { $$ = $1; }
| subg_stmt { $$ = $1; }
| node_id _EDGEOP edgeRHS {
  $$.clear();
  $<nodeA>$ = $<nodeA>1;
  $<nodeA>$ += $<nodeA>3;
  $<edgeA>$ = $<edgeA>1;
  $<edgeA>$ += $<edgeA>3;
  if( $<subg>3 )
    $<edgeA>$ += dotyy->bindEdge( $<s>2, $<nodeA>1, $<nodeA>3 );
  else if( $<nodeA>3.size() )
    $<edgeA>$ += dotyy->bindEdge( $<s>2, $<nodeA>1, $<nodeA>3[0]);
}
 | subg_stmt _EDGEOP edgeRHS {
   $$.clear();
   $<nodeA>$ = $<nodeA>1;
   $<nodeA>$ += $<nodeA>3;
   $<edgeA>$ = $<edgeA>1;
   $<edgeA>$ += $<edgeA>3;
   if( $<subg>3 )
     $<edgeA>$ += dotyy->bindEdge( $<s>2, $<nodeA>1, $<nodeA>3 );
   else if( $<nodeA>3.size() )
     $<edgeA>$ += dotyy->bindEdge( $<s>2, $<nodeA>1, $<nodeA>3[0]);
 }
 ;

subg_stmt : subg_hdr '{' stmt_list '}'%prec '{' { $$ = $3; $<subg>$ = true; }
 | _SUBGRAPH '{' stmt_list '}' { $$ = $3; $<subg>$ = true; }
 | '{' stmt_list '}' { $$ = $2; $<subg>$ = true; }
 | subg_hdr %prec _SUBGRAPH { $$ = $1; $<subg>$ = true; }
 ;

subg_hdr : _SUBGRAPH id { $$ = $2; }
 ;

id : IDENT { $$.clear(); $<s>$ = $<s>1; }
 | NUMBER { $$.clear(); $<s>$ = $<s>1; }
 | qid { $$ = $1; }
 ;

qid : STRING { $$.clear(); $<s>$ = $<s>1; }
 | qid '+' qid { $<s>$ = $<s>1 + $<s>3; }
 ;


%%




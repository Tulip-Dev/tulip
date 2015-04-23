/*
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
*/

%option noyywrap

%{

#define YY_NO_UNPUT
int line_num  = 1;
int html_nest = 0;

%}

mep							[ \t\r]+
alpha						[A-Za-z]
digit						[0-9]
id							([_]|{alpha})([_]|{alpha}|{digit})*
inum						[-+]?{digit}+\.?([eE][-+]?{digit}+)?
fnum						[-+]?{digit}*\.{digit}+([eE][-+]?{digit}+)?
num							{inum}|{fnum}

%x comment qstring hstring

%%

{mep}
<INITIAL,comment>\n			line_num++;
"/*"						BEGIN(comment);
<comment>[^*\n]*
<comment>"*"+[^*/\n]*
<comment>"*"+"/"			BEGIN(INITIAL);
"//".*
"#".*
"strict"					yylval.s = yytext; return _STRICT;
"digraph"					yylval.s = yytext; return _DIGRAPH;
"graph"						yylval.s = yytext; return _GRAPH;
"node"						yylval.s = yytext; return _NODE;
"edge"						yylval.s = yytext; return _EDGE;
"subgraph"					yylval.s = yytext; return _SUBGRAPH;
"--"|"->"					yylval.s = yytext; return _EDGEOP;
{num}						yylval.s = yytext; return NUMBER;
{id}						yylval.s = yytext; return IDENT;
["]							BEGIN(qstring); yylval.s.resize(0);
<qstring>["]				BEGIN(INITIAL); return STRING;
<qstring>[\\][\n]			line_num++;
<qstring>[^"\\]*			yylval.s += yytext;
<qstring>[\\].				yylval.s += yytext;
[<]							BEGIN(hstring); html_nest = 1; yylval.s.resize(0);
<hstring>[>]				html_nest--; if(html_nest) yylval.s+=yytext; else {BEGIN(INITIAL); return STRING;}
<hstring>[<]				html_nest++; yylval.s+=yytext;
<hstring>[\\][<]			yylval.s+="<";
<hstring>[\\][>]			yylval.s+=">";
<hstring>[\\][\n]			line_num++;
<hstring>([^><\\]*|[\\].)	yylval.s+=yytext;
.							yylval.s = yytext; return yytext[0];

%%


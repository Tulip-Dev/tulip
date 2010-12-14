/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <QtGui/qapplication.h>
#include <QtCore/qtimer.h>
#include <iostream>
#include <tulip/TulipPlugin.h>
#include "WebImport.h"

using namespace std;
using namespace tlp;

class UrlElement {
public:
  bool is_http;
  std::string data;
  unsigned int serverport;
  std::string server;
  std::string url;
  std::string clean_url;

  UrlElement();
  UrlElement(const UrlElement &c);
  void load();
  void clear();
  bool isHtmlPage();
  UrlElement parseUrl(const std::string &href);
  void setUrl(const std::string&);
  std::string getUrl() const {
    return clean_url.size() ? clean_url : url;
  }

private:
  void fill(std::string &result);
  bool siteconnect(const std::string &server, const std::string &url,
		   const int serverport, bool headonly);

private:
  HttpContext *context;

 public:
  bool isValid() {
    return !server.empty();
  }

  bool isRedirected() {
    return context && context->redirected;
  }

  UrlElement getRedirection() {
    return parseUrl(context->newLocation);
  }
};

namespace std {
template <>
  struct less<UrlElement> {
    bool operator()(const UrlElement &lhs, const UrlElement &rhs) {
      if (lhs.server.compare(rhs.server) < 0 ) return true;
      if (lhs.server.compare(rhs.server) > 0 ) return false;
      return lhs.getUrl().compare(rhs.getUrl()) < 0;
    }
  };
}

HttpContext::HttpContext() {
  // connect the requestFinished signal to our finished slot
  connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(finished(int, bool)));
  connect(this, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
	  this, SLOT(headerReceived(const QHttpResponseHeader &)));
}

void HttpContext::finished(int id, bool error) {
  // check to see if it is the request we made
  if(id !=rqid) return;
  // set status of the request
  status = !error;
  // OK
  processed = true;
}

void HttpContext::headerReceived(const QHttpResponseHeader & resp) {
  if ((isHtml = resp.isValid())) {
    int code = resp.statusCode();
    if (code > 399) /* error codes */
      isHtml = false;
    else if ((code > 299) &&
	     (code < 305 || code == 307)) {
      /* redirection codes */
      redirected = true;
      newLocation = resp.value(QString("Location")).toAscii().data();
    } else /* normal codes */
      isHtml = resp.hasContentType() &&
	resp.contentType().contains(QString("text/html"));
  }
}

void HttpContext::timeout() {
  cerr << "time-out occurs" << endl;
  // if timeout occurs
  // just indicates the we failed during processing
  processed = true;
  status = false;
}

void HttpContext::setTimer(QTimer *timer) {
  connect(timer, SIGNAL(timeout()), SLOT(timeout()));
}

UrlElement::UrlElement():is_http(true),data(""),serverport(80),context(0)  {
}
UrlElement::UrlElement(const UrlElement &c):
  is_http(c.is_http),
  data(""),serverport(c.serverport),server(c.server),url(c.url),clean_url(c.clean_url),context(0) {
}

void UrlElement::setUrl(const std::string& theUrl) {
  url = theUrl;
  size_t len = theUrl.find_first_of("?", 0);
  if (len != string::npos)
    clean_url = theUrl.substr(0, len);
  else
    clean_url.clear();
}

void UrlElement::fill(std::string &result) {
  if (context->bytesAvailable() > 0)
    result += context->readAll().data();
}

void UrlElement::clear() {
  if (context) {
    context->clearPendingRequests();
    delete context;
    context = 0;
  }
  data = "";
}


void UrlElement::load() {
  if (!siteconnect(server,url,serverport,false))
    return;
  fill(data);
}

static const char* not_html_extensions[] = {
  ".bmp", ".css", ".doc", ".ico", ".exe", ".gif", ".gz", ".js", ".jpeg", ".jpg",
  ".pdf", ",.png", ".ps", ".tar", ".tgz", ".wav", ".zip", ".z",
  0 /* must be the last */
};

bool UrlElement::isHtmlPage() {
  string lowercase(url);
  size_t len = lowercase.length();
  for (size_t i = 0; i < len; ++i)
    lowercase[i] = tolower(lowercase[i]);
  for (unsigned int i = 0; not_html_extensions[i]; i++)
    if (lowercase.rfind(not_html_extensions[i], len) != string::npos)
      return false;
  if(!siteconnect(server,url,serverport,true))
    return false;
  return context->isHtml;
}

bool UrlElement::siteconnect(const std::string &server, const std::string &url,const int serverport, bool headonly) {
  // check that we actually got data..
  if (server.empty()) return -1;

  if (!context)
    context = new HttpContext();

  context->setHost(QString(server.c_str()), serverport);
  string theUrl("/");
  // prefix the url with / if it doesn't start with it..
  if (url.empty() || url.c_str()[0] != '/')
    theUrl += url;
  else
    theUrl = url;
  context->processed = context->isHtml = context->redirected = false;
  // start the request and store the request ID
  if (headonly)
    context->rqid = context->head(QString(theUrl.c_str()));
  else
    context->rqid = context->get(QString(theUrl.c_str()));
  // block until the request is finished
  // or there is timeout
  QTimer timer;
  timer.setSingleShot(true);
  context->setTimer(&timer);
  timer.start(2000);
  while(!context->processed) {
    QCoreApplication::processEvents();
  }
  timer.stop();
  return context->status;
} /* end, siteconnect */

static const char * rejected_protocols[] = {
  "https:", "ftp:", "gopher:", "sftp:", "javascript:", "mms:", "mailto:",
  0 /* must be the last */
};

UrlElement UrlElement::parseUrl (const std::string &href) {
  UrlElement newUrl;
  string lowercase(href);
  size_t i, len = lowercase.length();
  for (i = 0; i < len; ++i)
    lowercase[i] = tolower(lowercase[i]);
  for (i = 0; rejected_protocols[i] != 0; i++) {
    if (lowercase.find(rejected_protocols[i]) != string::npos)
      break;
  }
  if (rejected_protocols[i]) {
    newUrl.is_http = false;
    if (i != 3 /* no javascript */)
      newUrl.server = href;
    return newUrl;
  }
  size_t pos=0;
  bool host = false;
  pos = lowercase.rfind("http://", len);
  if (pos == string::npos)
    pos=0;
  else {
    host=true;
    pos+=7;
  }
  if (host) {
    size_t endhost = lowercase.find_first_of("/ ",pos);
    if (endhost == string::npos)
      endhost=len;
    string hostname = href.substr(pos,endhost-pos);
    newUrl.server = hostname;
    newUrl.setUrl(href.substr(endhost));
  } else {
    size_t querystart = lowercase.find_first_of("#",pos); /* previously ?#  instead of # */
    if (querystart != string::npos)
      len = querystart;
    string theUrl = href.substr(pos,len-pos);
    if (theUrl.empty())
      return newUrl;
    //Manage relative urls
    if (theUrl[0]!='/') {
      string urlreference(this->url);
      size_t findUp = urlreference.rfind("/", urlreference.length());
      if (findUp==string::npos) {
	urlreference.clear();
	urlreference.append(1, '/');
      }
      else
	urlreference = urlreference.substr(0, findUp + 1);
      size_t pos;
      // remove space chars at the beginning
      for (pos = 0; pos < theUrl.size(); ++pos) {
	char c = theUrl[pos];
	if (c != ' ' && c != '\t')
	  break;
      }
      if (pos > 0)
	theUrl = theUrl.substr(pos);
      while ((pos = theUrl.find("./")) != string::npos) {
	if (pos == 0) {
	  theUrl = theUrl.substr(2);
	  continue;
	}
	if (theUrl[pos - 1] == '.') {
	  theUrl = theUrl.substr(3);
	  findUp = urlreference.rfind('/', findUp - 1);
	  if (findUp==string::npos) {
	    cerr << "bad url reference, to much ../" << endl;
	    return newUrl;
	  }
	  urlreference = urlreference.substr(0, findUp + 1);
	} else {
	  cerr << "bad url reference, to much ../" << endl;
	  return newUrl;
	}
      }
      theUrl = urlreference + theUrl;
    }
    if (theUrl != "/") {
      newUrl.setUrl(theUrl);
      newUrl.server = this->server;
      newUrl.serverport = this->serverport;
    }
  }
  return newUrl;
}


namespace {
  const char * paramHelp[] = {
    // server
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "string" ) \
    HTML_HELP_DEF( "default", "www.labri.fr" ) \
    HTML_HELP_BODY() \
    "This parameter defines the web server that you want to inspect. No need for http:// at the beginning; http protocol is always assumed. No need for / at the end." \
    HTML_HELP_CLOSE(),
    // initial page
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "string" ) \
    HTML_HELP_DEF( "default", "" ) \
    HTML_HELP_BODY() \
    "This parameter defines the first web page to visit. No need for / at the beginning."\
    HTML_HELP_CLOSE(),
    // max number of links
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "unsigned int" ) \
    HTML_HELP_DEF( "default", "1000" ) \
    HTML_HELP_BODY() \
    "This parameter defines the maximum number of nodes (different pages) allowed in the extracted graph." \
    HTML_HELP_CLOSE(),
    // non http links
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "boolean" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This parameter indicates if non http links (https, ftp, mailto...) have to be extracted." \
    HTML_HELP_CLOSE(),
    // other server
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "boolean" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "This parameter indicates if links or redirection to other server pages have to be followed." \
    HTML_HELP_CLOSE(),
    // compute layout
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "boolean" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This parameter indicates if a layout of the extracted graph has to be computed." \
    HTML_HELP_CLOSE(),
    // page color
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "color" ) \
    HTML_HELP_DEF( "default", "red" ) \
    HTML_HELP_BODY() \
    "This parameter indicated the color used to display nodes." \
    HTML_HELP_CLOSE(),
    // link color
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "color" ) \
    HTML_HELP_DEF( "default", "blue" ) \
    HTML_HELP_BODY() \
    "This parameter indicated the color used to display links." \
    HTML_HELP_CLOSE(),
    // link color
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "color" ) \
    HTML_HELP_DEF( "default", "yellow" ) \
    HTML_HELP_BODY() \
    "This parameter indicated the color used to display redirections." \
    HTML_HELP_CLOSE()
  };
}

struct WebImport:public ImportModule {
  std::deque<UrlElement> toVisit;
  std::set<UrlElement> visited;
  std::map<UrlElement, tlp::node> nodes;
  tlp::StringProperty *labels;
  tlp::StringProperty *urls;
  tlp::ColorProperty *colors;
  tlp::Color *redirectionColor;
  unsigned int maxSize, nbNodes;
  bool visitOther;
  bool extractNonHttp;

  WebImport(AlgorithmContext context):ImportModule(context) {
    addParameter<string>("server",paramHelp[0],"www.labri.fr");
    addParameter<string>("web page",paramHelp[1], "");
    addParameter<int>("max size",paramHelp[2], "1000");
    addParameter<bool>("non http links",paramHelp[3], "false");
    addParameter<bool>("other server",paramHelp[4], "false");
    addParameter<bool>("compute layout",paramHelp[5], "true");
    addParameter<Color>("page color",paramHelp[6],"(240, 0, 120, 128)");
    addParameter<Color>("link color",paramHelp[7],"(96,96,191,128)");
    addParameter<Color>("redirection color",paramHelp[8],"(191,175,96,128)");
    addDependency<LayoutAlgorithm>("GEM (Frick)", "1.0");
  }

  string urlDecode(const string& url) { 
    string buffer = "";
    int len = url.length();

    for (int i = 0; i < len; ++i) {
      char ch = url.at(i);
      if (ch == '%'){
	int chnum = 0;
	ch = url.at(++i);
	chnum = ch - '0';
	if (chnum > 9) {
	  if (ch >= 'A')
	    chnum = 10 + (ch - 'A');
	  else
	    chnum = 10 + (ch - 'a');
	}
	chnum *= 16;
	ch = url.at(++i);
	if (ch - '0' > 9) {
	  if (ch >= 'A')
	    chnum += ch - 'A';
	  else
	    chnum += ch - 'a';
	} else
	  chnum += ch - '0';
	buffer += chnum;
      } else {
	buffer += ch;
      }
    }
    return buffer;
  }

  //========================================================
  bool addNode(const UrlElement &url, node& n) {
    if (nodes.find(url)==nodes.end()) {
      // no more added node after maxSize
      if (nbNodes == maxSize) {
	n = node();
	return false;
      }
      n=graph->addNode();
      ++nbNodes;
      stringstream str;
      str << url.server;
      if (url.url[0] != '/')
	str << "/";
      str << url.getUrl();
      labels->setNodeValue(n, urlDecode(str.str()));
      ostringstream oss;
      if(url.is_http){
    	  oss<<"http://";
      }
      oss << str.str();
      urls->setNodeValue(n,oss.str());
      nodes[url] = n;
      return true;
    }
    n = nodes[url];
    return false;
  }

  //========================================================
  bool addEdge(const UrlElement &source, const UrlElement &target,
	       const char* type, const Color *color) {
    node sNode,tNode;
    bool sAdded = addNode(source, sNode);
    bool tAdded = addNode(target, tNode);
    // if new nodes can node be added (nbNodes > maxSize)
    // stop adding edge
    if (!sNode.isValid() || !tNode.isValid())
      return false;
    if (sAdded || tAdded || ((sNode != tNode) && !graph->existEdge(sNode, tNode).isValid())) {
      edge e = graph->addEdge(sNode, tNode);
      if (type)
	labels->setEdgeValue(e, type);
      if (color)
	colors->setEdgeValue(e, *color);
    }
    return true;
  }
  //========================================================
  bool nextUrl(UrlElement & url) {
    //cerr << __PRETTY_FUNCTION__ << endl;
    while(!toVisit.empty()) {
      url = toVisit.front();
      toVisit.pop_front();
      if (visited.find(url)==visited.end()) {
	visited.insert(url);
	return true;
      }
    }
    return false;
  }
  //========================================================
  void findAndTreatUrls(const string&lowercase,
			const string&balise, UrlElement &url) {
    size_t llen = lowercase.length();
    size_t len = llen;
    while ( len != string::npos ) {
      len = lowercase.rfind(balise, len);
      bool urlFound = true;
      if( len != string::npos ) {
	// find the next '=' then the first '"'
	size_t start = len + balise.length();
	len--;
	char c = '=';
	for (; start < llen; start++) {
	  if (lowercase[start] == c) {
	    // found =
	    if (c == '=') {
	      // now looking for "
	      c = '"';
	      continue;
	    }
	    else
	      break;
	  }
	  // only space chars allowed between balise and '='
	  // and between '=' and the first '"' too
	  if (lowercase[start] != ' ') {
	    urlFound = false;
	    break;
	  }
	}
	if (urlFound) {
	  ++start;
	  size_t end = start;
	  // find the end of the string
	  for (; end < llen; end++) {
	    if (lowercase[end] == '"')
	      break;
	  }
	  if (end != start) {
	    string newurl = url.data.substr(start, end - start);
	    parseUrl(newurl, url);
	  }
	}
      }
    }
  }
  //========================================================
  // parsehtml - parse the given html and add to queue
  void parseHtml (UrlElement &url) {
    //cerr << __PRETTY_FUNCTION__ << endl << flush;
    if (url.data.empty()) return;
    string lowercase(url.data);
    for (size_t i = 0; i< lowercase.length(); ++i)
      lowercase[i] = tolower(lowercase[i]);
    findAndTreatUrls(lowercase," href",url);
    findAndTreatUrls(lowercase," src",url);
  }
  //========================================================
  void addUrl (const UrlElement &url, bool _toVisit) {
    if (visited.find(url)!=visited.end())
      return;
    if (_toVisit && url.is_http)
      toVisit.push_back(url);
  }
  //========================================================
  void addServer(const string &servername) {
    UrlElement tmpUrl;
    tmpUrl.server = servername;
    tmpUrl.setUrl("/");
    tmpUrl.serverport = 80;
    tmpUrl.data = "";
    if (visited.find(tmpUrl)!=visited.end())
      return;
    toVisit.push_back(tmpUrl);
  }
  //========================================================
  void parseUrl (const string &href, UrlElement &starturl) {
    UrlElement newUrl = starturl.parseUrl(href);
    if (newUrl.isValid() && (extractNonHttp || newUrl.is_http) &&
	addEdge(starturl, newUrl, 0, 0))
      addUrl(newUrl, visitOther || (newUrl.server == starturl.server));
  }
  //========================================================
  bool start() {
    UrlElement url;
    unsigned step = 20;
    while (nextUrl(url)) {
      if (url.isHtmlPage()) {
	if (pluginProgress && ((nbNodes % step) == 0)) {
	  pluginProgress->setComment(string("Visiting ") +
				     urlDecode(url.server + url.url));
	  if (pluginProgress->progress(nbNodes, maxSize) !=TLP_CONTINUE)
	    return pluginProgress->state()!= TLP_CANCEL;
	}
#ifndef NDEBUG
	cerr << "Visiting: " << url.server << url.url << " ..." << flush;
#endif
	if (url.isRedirected()) {
	  UrlElement redirection = url.getRedirection();
	  if (redirection.isValid()) {
#ifndef NDEBUG
	    cerr << endl << "redirected to " << redirection.server << redirection.url << endl;
#endif
	    if (addEdge(url, redirection,  "redirection", redirectionColor))
	      addUrl(redirection,
		     visitOther || redirection.server == url.server);
	  } else
	    cerr << endl << "invalid redirection" << endl;
	} else {
	  url.load();
	  parseHtml(url);
	  url.clear();
#ifndef NDEBUG
	  cerr << " done" << endl << flush;
#endif
	}
      }
#ifndef NDEBUG
      else
	cerr << "Omitting : " << url.server << url.url << " ==> [not html]"<< endl;
#endif
    }
    return true;
  }


  bool import(const string &) {
    string server = "www.labri.fr";
    string url;
    bool computelayout = true;
    Color pColor(255,0,0);
    Color lColor(0,0,255,128);
    Color rColor(255,255,0,128);
    maxSize = 1000;
    nbNodes = 0;
    visitOther = false;
    extractNonHttp = true;
    if (dataSet != 0) {
      dataSet->get("server", server);
      dataSet->get("web page", url);
      dataSet->get("max size", maxSize);
      dataSet->get("non http links", extractNonHttp);
      dataSet->get("other server", visitOther);
      dataSet->get("compute layout", computelayout);
      dataSet->get("page color", pColor);
      dataSet->get("link color", lColor);
      dataSet->get("redirection color", rColor);
    }

    UrlElement mySite;
    size_t pos = server.find("http://");
    if (pos == 0)
      // remove http:// prefix
      server = server.substr(7);
    // remove / prfix
    if (server[0] == 0)
      server = server.substr(1);
    if (server[server.size() - 1] == '/')
      // remove / suffix
      server = server.substr(0, server.size() - 1); 
    mySite.server = server;
    if (url[0] == '/')
      // remove / prefix
      url = url.substr(1);
    mySite.setUrl(string("/") + url);
    mySite.serverport = 80;
    mySite.data = "";

    labels = graph->getProperty<StringProperty>("viewLabel");
    labels->setAllEdgeValue(string("link"));
    urls = graph->getProperty<StringProperty>("url");
    colors = graph->getProperty<ColorProperty>("viewColor");
    colors->setAllNodeValue(pColor);
    colors->setAllEdgeValue(lColor);
    redirectionColor = &rColor;

    graph->getProperty<IntegerProperty>("viewShape")
      ->setAllNodeValue(14); // GlyphManager::getInst().glyphId("2D - Circle")
    node n;
    toVisit.push_back(mySite);
    addNode(mySite, n);

    if (pluginProgress) {
      pluginProgress->showPreview(false);
      pluginProgress->setComment(std::string("Visiting ") + mySite.server + mySite.url);
    }
    if (!start())
      return false;
    if (computelayout) {
      pluginProgress->setComment("Layouting extracted graph using GEM...");
      string errMsg;
      // apply GEM
      DataSet tmp;
      LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
      tmp.set("initial layout", layout);
      return graph->computeProperty("GEM (Frick)", layout,
				    errMsg, pluginProgress, &tmp);
    }
    return true;
  }
};

IMPORTPLUGINOFGROUP(WebImport,"Web Site","Auber","15/11/2004","Import Web site plugin","1.0","Misc")

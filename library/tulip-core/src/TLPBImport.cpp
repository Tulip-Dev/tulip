/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <fstream>
#include <errno.h>
#include <sys/stat.h>
#include <tulip/TLPBExportImport.h>
#include <tulip/TlpTools.h>
#include <tulip/GraphAbstract.h>
#include <tulip/GraphProperty.h>

PLUGIN(TLPBImport)

using namespace tlp;
using namespace std;

// a template class to iterate into a range of nodes or edges
template <typename T>
struct RangeIterator : public Iterator<T> {
  T first, last, current;

  RangeIterator(T begin, T end) : Iterator<T>(), first(begin), last(end), current(begin) {
    assert(begin.id <= last.id);
  }
  ~RangeIterator() {}
  T next() {
    T tmp = current;
    ++(current.id);
    return tmp;
  }

  bool hasNext() {
    return current.id <= last.id;
  }
};

bool errorTrap(void *buf = NULL) {
  if (buf)
    free(buf);

  return false;
}

//================================================================================
TLPBImport::TLPBImport(tlp::PluginContext *context) : ImportModule(context) {
  addInParameter<std::string>("file::filename", "The pathname of the TLPB file to import.", "");
}
//================================================================================
bool TLPBImport::importGraph() {
  std::string filename;
  std::istream *is = NULL;

  if (dataSet->exist("file::filename")) {
    dataSet->get<std::string>("file::filename", filename);
    struct stat infoEntry;
    bool result = stat(filename.c_str(), &infoEntry) == 0;

    if (!result) {
      std::stringstream ess;
      ess << filename.c_str() << ": " << strerror(errno);
      pluginProgress->setError(ess.str());
      tlp::error() << pluginProgress->getError() << std::endl;
      return errorTrap();
    }

    bool gzip(false);
    std::list<std::string> gext(gzipFileExtensions());

    for (std::list<std::string>::const_iterator it = gext.begin(); it != gext.end(); ++it) {
      if (filename.rfind(*it) == (filename.length() - (*it).length())) {
        is = tlp::getIgzstream(filename);
        gzip = true;
        break;
      }
    }

    if (!gzip)
      is = tlp::getInputFileStream(filename, std::ifstream::in | std::ifstream::binary);
  } else {
    pluginProgress->setError("No file to open: 'file::filename' parameter is missing");
    tlp::error() << pluginProgress->getError() << std::endl;
    return errorTrap();
  }

  pluginProgress->showPreview(false);
  pluginProgress->setComment(std::string("Loading ") + filename + "...");

  // header
  TLPBHeader header;

  // read header
  if (!bool(is->read(reinterpret_cast<char *>(&header), sizeof(header))))
    return (delete is, errorTrap());

  if (!header.checkCompatibility()) {
    pluginProgress->setError("file is not in TLPB format.");
    tlp::error() << pluginProgress->getError() << std::endl;
    return (delete is, errorTrap());
  }

  // add nodes
  graph->addNodes(header.numNodes);

  // loop to read edges
  {
    // we can use a buffer to limit the disk reads
    std::vector<std::pair<node, node> > vEdges(MAX_EDGES_TO_READ);
    unsigned int nbEdges = header.numEdges;
    pluginProgress->setComment(filename + ": reading edges...");

    while (nbEdges) {
      unsigned int edgesToRead = nbEdges > MAX_EDGES_TO_READ ? MAX_EDGES_TO_READ : nbEdges;
      vEdges.resize(edgesToRead);

      // read a bunch of edges
      if (!bool(is->read(reinterpret_cast<char *>(vEdges.data()), edgesToRead * sizeof(vEdges[0]))))
        return (delete is, errorTrap());

      if (pluginProgress->progress(header.numEdges - nbEdges, header.numEdges) != TLP_CONTINUE)
        return pluginProgress->state() != TLP_CANCEL;

      // add edges in the graph
      graph->addEdges(vEdges);
      // decrement nbEdges
      nbEdges -= edgesToRead;
    }
  }
  // read subgraphs
  unsigned int numSubGraphs = 0;
  MutableContainer<Graph *> subgraphs;
  subgraphs.set(0, graph);
  {
    // read the number of subgraphs
    if (!bool(is->read(reinterpret_cast<char *>(&numSubGraphs), sizeof(numSubGraphs))))
      return (delete is, errorTrap());

    // read loop for subgraphs
    pluginProgress->setComment(filename + ": reading subgraphs...");

    for (unsigned int i = 0; i < numSubGraphs; ++i) {
      std::pair<unsigned int, unsigned int> ids;

      // read subgraph id and parent id
      if (!bool(is->read(reinterpret_cast<char *>(&ids), sizeof(ids))))
        return (delete is, errorTrap());

      // add subgraph
      Graph *parent = subgraphs.get(ids.second);
      Graph *sg = static_cast<GraphAbstract *>(parent)->addSubGraph(ids.first);
      // record sg
      subgraphs.set(ids.first, sg);
      // read sg nodes ranges
      {
        unsigned int numRanges = 0;

        // read the number of nodes ranges
        if (!bool(is->read(reinterpret_cast<char *>(&numRanges), sizeof(numRanges))))
          return (delete is, errorTrap());

        // we can use a buffer to limit the disk reads
        std::vector<std::pair<node, node> > vRanges(MAX_RANGES_TO_READ);

        // loop to read ranges
        while (numRanges) {
          unsigned int rangesToRead =
              numRanges > MAX_RANGES_TO_READ ? MAX_RANGES_TO_READ : numRanges;
          vRanges.resize(rangesToRead);

          // read a bunch of ranges
          if (!bool(is->read(reinterpret_cast<char *>(vRanges.data()),
                             rangesToRead * sizeof(vRanges[0]))))
            return (delete is, errorTrap());

          // loop to add nodes
          for (unsigned int i = 0; i < rangesToRead; ++i) {
            std::pair<node, node> &range = vRanges[i];
            RangeIterator<node> itr(range.first, range.second);
            sg->addNodes(&itr);
          }

          numRanges -= rangesToRead;
        }
      }
      // read sg edges ranges
      {
        unsigned int numRanges = 0;

        // read the number of edges ranges
        if (!bool(is->read(reinterpret_cast<char *>(&numRanges), sizeof(numRanges))))
          return (delete is, errorTrap());

        // loop to read ranges
        std::vector<std::pair<edge, edge> > vRanges(MAX_RANGES_TO_READ);

        while (numRanges) {
          unsigned int rangesToRead =
              numRanges > MAX_RANGES_TO_READ ? MAX_RANGES_TO_READ : numRanges;
          vRanges.resize(rangesToRead);

          // read a bunch of ranges
          if (!bool(is->read(reinterpret_cast<char *>(vRanges.data()),
                             rangesToRead * sizeof(vRanges[0]))))
            return (delete is, errorTrap());

          // loop to add edges
          for (unsigned int i = 0; i < rangesToRead; ++i) {
            std::pair<edge, edge> &range = vRanges[i];
            RangeIterator<edge> itr(range.first, range.second);
            sg->addEdges(&itr);
          }

          numRanges -= rangesToRead;
        }
      }

      if (pluginProgress->progress(i + 1, numSubGraphs) != TLP_CONTINUE)
        return pluginProgress->state() != TLP_CANCEL;
    }
  }
  // read properties
  {
    unsigned int numProperties = 0;

    // read number of properties
    if (!bool(is->read(reinterpret_cast<char *>(&numProperties), sizeof(numProperties))))
      return (delete is, errorTrap());

    // read loop on properties
    pluginProgress->setComment(filename + ": reading properties...");

    for (unsigned int i = 0; i < numProperties; ++i) {
      unsigned int size = 0;

      // read name length
      if (!bool(is->read(reinterpret_cast<char *>(&size), sizeof(size))))
        return (delete is, errorTrap());

      // + 1 to ensure a null terminated string
      std::string propName(size + 1, 0);

      // read name
      if (!is->read(const_cast<char *>(propName.data()), size))
        return (delete is, errorTrap());

      propName.resize(size);

      // special treament for pathnames view properties
      bool pnViewProp = (propName == string("viewFont") || propName == string("viewTexture"));

      // read graph id
      if (!bool(is->read(reinterpret_cast<char *>(&size), sizeof(size))))
        return (delete is, errorTrap());

      // get corresponding graph
      Graph *g = subgraphs.get(size);
      assert(g);

      if (g == NULL)
        return (delete is, errorTrap());

      // read type length
      if (!bool(is->read(reinterpret_cast<char *>(&size), sizeof(size))))
        return (delete is, errorTrap());

      // + 1 to ensure a null terminated string
      std::string propType(size + 1, 0);

      // read type
      if (!is->read(const_cast<char *>(propType.data()), size))
        return (delete is, errorTrap());

      propType.resize(size);
      PropertyInterface *prop = NULL;

      // create property
      if (propType == GraphProperty::propertyTypename)
        prop = g->getLocalProperty<GraphProperty>(propName);
      else if (propType == DoubleProperty::propertyTypename)
        prop = g->getLocalProperty<DoubleProperty>(propName);
      else if (propType == LayoutProperty::propertyTypename)
        prop = g->getLocalProperty<LayoutProperty>(propName);
      else if (propType == SizeProperty::propertyTypename)
        prop = g->getLocalProperty<SizeProperty>(propName);
      else if (propType == ColorProperty::propertyTypename)
        prop = g->getLocalProperty<ColorProperty>(propName);
      else if (propType == IntegerProperty::propertyTypename)
        prop = g->getLocalProperty<IntegerProperty>(propName);
      else if (propType == BooleanProperty::propertyTypename)
        prop = g->getLocalProperty<BooleanProperty>(propName);
      else if (propType == StringProperty::propertyTypename)
        prop = g->getLocalProperty<StringProperty>(propName);
      else if (propType == SizeVectorProperty::propertyTypename)
        prop = g->getLocalProperty<SizeVectorProperty>(propName);
      else if (propType == ColorVectorProperty::propertyTypename)
        prop = g->getLocalProperty<ColorVectorProperty>(propName);
      else if (propType == CoordVectorProperty::propertyTypename)
        prop = g->getLocalProperty<CoordVectorProperty>(propName);
      else if (propType == DoubleVectorProperty::propertyTypename)
        prop = g->getLocalProperty<DoubleVectorProperty>(propName);
      else if (propType == IntegerVectorProperty::propertyTypename)
        prop = g->getLocalProperty<IntegerVectorProperty>(propName);
      else if (propType == BooleanVectorProperty::propertyTypename)
        prop = g->getLocalProperty<BooleanVectorProperty>(propName);
      else if (propType == StringVectorProperty::propertyTypename)
        prop = g->getLocalProperty<StringVectorProperty>(propName);

      assert(prop);

      if (prop == NULL)
        return (delete is, errorTrap());

      if (pnViewProp) {
        std::string value;
        StringType::readb(*is, value);
        // if needed replace symbolic path by real path
        size_t pos = value.find("TulipBitmapDir/");

        if (pos != std::string::npos)
          value.replace(pos, 15, TulipBitmapDir);

        static_cast<StringProperty *>(prop)->setAllNodeValue(value);

        StringType::readb(*is, value);
        // if needed replace symbolic path by real path
        pos = value.find("TulipBitmapDir/");

        if (pos != std::string::npos)
          value.replace(pos, 15, TulipBitmapDir);

        static_cast<StringProperty *>(prop)->setAllEdgeValue(value);
      } else {
        // read and set property node default value
        if (!prop->readNodeDefaultValue(*is))
          return (delete is, errorTrap());

        // read and set property edge default value
        if (!prop->readEdgeDefaultValue(*is))
          return (delete is, errorTrap());
      }

      // nodes / edges values
      {
        unsigned int numValues = 0;

        // read the number of nodes values
        if (!bool(is->read(reinterpret_cast<char *>(&numValues), sizeof(numValues))))
          return (delete is, errorTrap());

        // std::basic_streambuf::pubsetbuf is a no-op in libcxx (LLVM implementation of STL)
        // see https://github.com/llvm-mirror/libcxx/blob/master/include/streambuf#L150
        // and https://github.com/llvm-mirror/libcxx/blob/master/include/streambuf#L360
        // and also in STL implementation of Microsoft Visual C++
        // so fallback writing directly to the output stream in these cases
        bool canUsePubSetBuf = true;
#if defined(_LIBCPP_VERSION) || defined(_MSC_VER)
        canUsePubSetBuf = false;
#endif

        // loop on nodes values
        size = prop->nodeValueSize();

        // for backward compatibility with TLPB format <= 1.1, (see sourceforge commit #11536)
        if (header.major == 1 && header.minor <= 1 && propType == GraphProperty::propertyTypename) {
          size = sizeof(tlp::Graph *);
        }

        if (size && canUsePubSetBuf) {
          // as the size of any value is fixed
          // we can use a buffer to limit the number of disk reads
          char *vBuf;

          if (numValues < MAX_VALUES_TO_READ)
            vBuf = static_cast<char *>(malloc(numValues * (sizeof(unsigned int) + size)));
          else
            vBuf = static_cast<char *>(malloc(MAX_VALUES_TO_READ * (sizeof(unsigned int) + size)));

          while (numValues) {
            // read a bunch of <node, prop_value>
            unsigned int valuesToRead =
                (numValues > MAX_VALUES_TO_READ) ? MAX_VALUES_TO_READ : numValues;

            if (!is->read(reinterpret_cast<char *>(vBuf),
                          valuesToRead * (sizeof(unsigned int) + size)))
              return (delete is, errorTrap(vBuf));

            // use a stringstream to read nodes and properties
            stringstream vs;
            // set read buffer of stringstream to vBuf
            vs.rdbuf()->pubsetbuf(reinterpret_cast<char *>(vBuf),
                                  valuesToRead * (sizeof(unsigned int) + size));

            for (unsigned int i = 0; i < valuesToRead; ++i) {
              node n;

              // read node id
              if (!bool(vs.read(reinterpret_cast<char *>(&n.id), sizeof(unsigned int))))
                return (delete is, errorTrap(vBuf));

              // read and set node value
              assert(g->isElement(n));

              if (!prop->readNodeValue(vs, n))
                return (delete is, errorTrap(vBuf));
            }

            numValues -= valuesToRead;
          }

          free(vBuf);
        } else {
          // we cannot predict the size of property values
          // so the loop is simpler but with more disk reads
          for (unsigned int i = 0; i < numValues; ++i) {
            node n;

            // read node id
            if (!bool(is->read(reinterpret_cast<char *>(&(n.id)), sizeof(unsigned int))))
              return (delete is, errorTrap());

            assert(g->isElement(n));

            if (pnViewProp) {
              std::string value;

              // must ensure ascendant compatibility
              // after clang bug fix in commit #11142
              if (header.major == 1 && header.minor == 0)
                StringType::read(*is, value);
              else
                StringType::readb(*is, value);

              // if needed replace symbolic path by real path
              size_t pos = value.find("TulipBitmapDir/");

              if (pos != std::string::npos)
                value.replace(pos, 15, TulipBitmapDir);

              static_cast<StringProperty *>(prop)->setNodeValue(n, value);
            } else {
              // read and set node value
              if (!prop->readNodeValue(*is, n))
                return (delete is, errorTrap());
            }
          }
        }

        // read the number of edges values
        if (!bool(is->read(reinterpret_cast<char *>(&numValues), sizeof(numValues))))
          return (delete is, errorTrap());

        // loop on edges values
        size = prop->edgeValueSize();

        if (size && canUsePubSetBuf) {
          // as the size of any value is fixed
          // we can use a buffer to limit the number of disk reads
          char *vBuf;

          if (numValues < MAX_VALUES_TO_READ)
            vBuf = static_cast<char *>(malloc(numValues * (sizeof(unsigned int) + size)));
          else
            vBuf = static_cast<char *>(malloc(MAX_VALUES_TO_READ * (sizeof(unsigned int) + size)));

          while (numValues) {
            // read a bunch of <edge, prop_value> in vBuf
            unsigned int valuesToRead =
                (numValues > MAX_VALUES_TO_READ) ? MAX_VALUES_TO_READ : numValues;

            if (!is->read(reinterpret_cast<char *>(vBuf),
                          valuesToRead * (sizeof(unsigned int) + size)))
              return (delete is, errorTrap(vBuf));

            // use a stringstream to read edges and properties
            stringstream vs;
            // set read buffer of stringstream to vBuf
            vs.rdbuf()->pubsetbuf(vBuf, valuesToRead * (sizeof(unsigned int) + size));

            for (unsigned int i = 0; i < valuesToRead; ++i) {
              edge e;

              // read edge id
              if (!bool(vs.read(reinterpret_cast<char *>(&e.id), sizeof(unsigned int))))
                return (delete is, errorTrap(vBuf));

              assert(g->isElement(e));

              if (pnViewProp) {
                std::string value;

                // must ensure ascendant compatibility
                // after clang bug fix in commit #11142
                if (header.major == 1 && header.minor == 0)
                  StringType::read(*is, value);
                else
                  StringType::readb(*is, value);

                // if needed replace symbolic path by real path
                size_t pos = value.find("TulipBitmapDir/");

                if (pos != std::string::npos)
                  value.replace(pos, 15, TulipBitmapDir);

                static_cast<StringProperty *>(prop)->setEdgeValue(e, value);
              } else

                  // read and set edge value
                  if (!prop->readEdgeValue(vs, e))
                return (delete is, errorTrap(vBuf));
            }

            numValues -= valuesToRead;
          }

          free(vBuf);
        } else {
          // we cannot predict the size of property values
          // so the loop is simpler but with more disk reads
          for (unsigned int i = 0; i < numValues; ++i) {
            edge e;

            // read edge id
            if (!bool(is->read(reinterpret_cast<char *>(&e.id), sizeof(unsigned int))))
              return (delete is, errorTrap());

            // read and set edge value
            assert(g->isElement(e));

            if (!prop->readEdgeValue(*is, e))
              return (delete is, errorTrap());
          }
        }
      }

      if (pluginProgress->progress(i + 1, numProperties) != TLP_CONTINUE)
        return pluginProgress->state() != TLP_CANCEL;
    }
  }
  // read graphs (root graph + subgraphs) attributes
  pluginProgress->setComment(filename + ": reading attributes of graphs...");

  for (unsigned int i = 0; i < numSubGraphs + 1; ++i) {
    unsigned int id = 0;

    // read graph id
    if (!bool(is->read(reinterpret_cast<char *>(&id), sizeof(id))))
      return (delete is, errorTrap());

    Graph *g = id ? subgraphs.get(id) : graph;
    assert(g);

    if (g == NULL)
      return (delete is, errorTrap());

    // read graph attributes
    DataSet::read(*is, const_cast<DataSet &>(g->getAttributes()));
    // do not forget to read the end marker
    char c = '\0';
    is->get(c);
    assert(c == ')');

    if (c != ')')
      return false;

    if (pluginProgress->progress(i + 1, numSubGraphs + 1) != TLP_CONTINUE) {
      delete is;
      return pluginProgress->state() != TLP_CANCEL;
    }
  }

  delete is;
  return true;
}

#include "index.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace pt = boost::property_tree;

namespace tilecone {

Index::Index(std::string const& fName) {
  pt::ptree tree;

  pt::read_ini(fName, tree);

  bucketZoom_ = tree.get<uint64_t>("Tiles.BucketZoom");
  tileZoom_ = tree.get<uint64_t>("Tiles.TileZoom");
}

Index::~Index() {

}

}

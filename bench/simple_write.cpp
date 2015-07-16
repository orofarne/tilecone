#include "db.hpp"

#include <celero/Celero.h>

#include <memory>
#include <fstream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
using namespace tilecone;

class SimpleWriteBenchFixture : public celero::TestFixture {
public:
	virtual void setUp(int64_t experimentValue) {
		line_ = "My bench string\n";

		dirName0_ = "__bench_db0";
	  fs::path dPath0(dirName0_);
		fs::remove_all(dPath0);
		fs::create_directory(dPath0);
		fout_.reset(new std::ofstream((dPath0 / "bench.log").native()));

		dirName1_ = "__bench_db1";
	  fs::path dPath1(dirName1_);
		fs::remove_all(dPath1);
		fs::create_directory(dPath1);
		{
			std::ofstream fout((dPath1 / "index.ini").native());
			fout
				<< "[Tiles]\n"
				<< "BucketZoom = 15\n"
				<< "TileZoom = 19\n"
				<< "BlockSize = 100\n";
		}

		db_.reset(new DB(dirName1_, 10));
	}

	virtual void tearDown() {
	}

protected:
	std::string dirName0_;
	std::string dirName1_;
	std::string line_;
	std::unique_ptr<std::ofstream> fout_;
	std::unique_ptr<DB> db_;
};


BASELINE_F(SimpleWrite, Baseline, SimpleWriteBenchFixture, 20, 1000) {
	(*fout_) << line_;
}

BENCHMARK_F(SimpleWrite, TileConeOneTile, SimpleWriteBenchFixture, 20, 1000) {
	db_->setTile(316893, 163547, line_.data(), line_.size());
}

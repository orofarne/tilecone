#include "db.hpp"

#include <celero/Celero.h>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
using namespace tilecone;

class SimpleWriteBenchFixture : public celero::TestFixture {
public:
	virtual void setUp(int64_t experimentValue) {
		dirName_ = "__bench_db1";
	  fs::path dPath(dirName_);
		fs::remove_all(dPath);
	}

	virtual void tearDown() {
	}

protected:
	std::string dirName_;
};


BASELINE_F(Synthetic1Bench, Baseline, SimpleWriteBenchFixture, 2, 5) {
	// TODO
}

BENCHMARK_F(Synthetic1Bench, 10kLayers, SimpleWriteBenchFixture, 2, 5) {
	// TODO
}

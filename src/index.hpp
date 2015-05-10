#pragma once

#include <string>
#include <memory>

#include "idx_types.h"

namespace tilecone {

std::shared_ptr<IndexFile> LoadIndex(std::string const& fName);
void LoadIndex(std::string const& fName, std::shared_ptr<IndexFile>);

}

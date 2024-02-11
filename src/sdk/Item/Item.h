#pragma once

#include "../../utils/mem.h"

class Item {
public:
	BUILD_ACCESS(this, std::string, AtlasTextureFile, 0x8);
	BUILD_ACCESS(this, std::string, Namespace, 0xF8);
	BUILD_ACCESS(this, std::string, Name, 0xD0);
};
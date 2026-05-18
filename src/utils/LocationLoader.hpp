#pragma once

#include "LocationManager.hpp"
#include <nlohmann/json.hpp>
#include <string>

class LocationLoader {
	public:
		static LocationManager load(const std::string &directory);
};

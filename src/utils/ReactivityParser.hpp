#pragma once

#include <vector>
#include "Requirement.hpp"
#include "Effect.hpp"
#include <nlohmann/json.hpp>

std::vector<Requirement> parseRequirements(const nlohmann::json &reqs);
std::vector<Effect> parseEffects(const nlohmann::json &effs);

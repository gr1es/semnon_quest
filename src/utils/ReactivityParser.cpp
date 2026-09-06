#include "ReactivityParser.hpp"
#include "StateType.hpp"
#include <string>
#include <stdexcept>

static StateType stringToStateType(const std::string &s)
{
	if (s == "Flag")
		return (StateType::Flag);
	if (s == "Skill")
		return (StateType::Skill);
	if (s == "Counter")
		return (StateType::Counter);
	if (s == "Feat")
		return (StateType::Feat);
	if (s == "Item")
		return (StateType::Item);
	if (s == "Standing")
		return (StateType::Standing);
	throw(std::invalid_argument("ERROR: ReactivityParser found no match for StateType."));
}

std::vector<Requirement> parseRequirements(const nlohmann::json &reqs)
{
	std::vector<Requirement> res;

	for (const nlohmann::json &req : reqs)
	{
		Requirement temp;
		// .at is a nlohmann method that returns the value to the entered key
		// throws if no match for key is found! the throw is desired to ring alarm about broken JSON
		temp.type = stringToStateType(req.at("type"));
		temp.key = req.at("key");
		// min and max below rely on nlohmann's optional support; should work fine on newest versions
		// these fields in temp are only populated when they exist in req
		// alternative that always works:
		// req.at("min").get<int>()
		if (req.contains("min"))
			temp.min = req.at("min");
		if (req.contains("max"))
			temp.max = req.at("max");
		if (req.contains("expected"))
			temp.expected = req.at("expected");
		res.push_back(temp);
	}
	return (res);
}

std::vector<Effect> parseEffects(const nlohmann::json &effs)
{
	std::vector<Effect> res;

	for (const nlohmann::json &eff : effs)
	{
		Effect temp;

		// same as above: use of .at to generate a throw if required type or key fields do not exist (malformed JSON!)
		temp.type = stringToStateType(eff.at("type"));
		temp.key = eff.at("key");
		// value: reads the value to the entered key and returns it OR, if the key is not there, returns the second argument (in this case 0)
		// this is used here as fallback to default values should the JSON be missing these fields (perfectly fine possibility, hereby handled)
		temp.delta = eff.value("delta", 0);
		temp.flagValue = eff.value("flagValue", true);
		res.push_back(temp);
	}
	return (res);
}

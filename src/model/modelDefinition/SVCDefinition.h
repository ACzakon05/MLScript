#include <iostream>
#include <unordered_map>

#include "ModelDefinition.h"

struct SVCDefinition : public ModelDefinition {
    std::string name = "SVC";

    std::unordered_map<std::string, std::string> params = {
        {"c", "1.0"},
        {"kernel", "'rbf'"},
        {"degree", "3"},
        {"gamma", "'scale'"},
        {"coef0", "0.0"},
        {"probability", "False"}
    };

    std::ostream& toScikit(std::ostream& os) override {
        os << name << "(\n";

        for (const auto& [interalName, scikitName] : mlsToScikitParam) {
            os << "\t" << scikitName << "=" << params.at(interalName) << ",\n";
        }

        os << ")\n";

        return os;
    }

private:
    std::unordered_map<std::string, std::string> mlsToScikitParam = {
        {"c", "C"},
        {"kernel", "kernel"},
        {"degree", "degree"},
        {"gamma", "gamma"},
        {"coef0", "coef0"},
        {"probability", "probability"}
    };
};
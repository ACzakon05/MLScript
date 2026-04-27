#include <iostream>
#include <unordered_map>

#include "ModelDefinition.h"

struct KNNRegDefinition : public ModelDefinition {
    std::string name = "KNeighborsRegressor";

    std::unordered_map<std::string, std::string> params = {
        {"nneighbors", "5"},
        {"weights", "'uniform'"},
        {"algorithm", "'auto'"},
        {"njobs", "None"}
    };

    std::ostream& toScikit(std::ostream& os) override {
        os << name << "(\n";

        for (const auto& [interalName, scikitName] : mlsToScikitParam) {
            os << "\t" << scikitName << "=" << params.at(interalName) << ",\n";
        }

        os << ")\n";

        return os;
    }

    std::string getRequiredImport() const override {
        return "from sklearn.neighbors import KNeighborsRegressor\n";
    }

private:
    std::unordered_map<std::string, std::string> mlsToScikitParam = {
        {"nneighbors", "n_neighbors"},
        {"weights", "weights"},
        {"algorithm", "algorithm"},
        {"njobs", "n_jobs"}
    };
};
#include <iostream>
#include <unordered_map>

#include "ModelDefinition.h"

struct RidgeDefinition : public ModelDefinition {
    std::string name = "Ridge";

    std::unordered_map<std::string, std::string> params = {
        {"alpha", "1.0"},
        {"fitintercept", "True"},
        {"maxiter", "None"},
        {"tol", "0.0001"},
        {"solver", "'auto'"},
        {"positive", "False"}
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
        return "from sklearn.linear_model import Ridge\n";
    }

private:
    std::unordered_map<std::string, std::string> mlsToScikitParam = {
        {"alpha", "alpha"},
        {"fitintercept", "fit_intercept"},
        {"maxiter", "max_iter"},
        {"tol", "tol"},
        {"solver", "solver"},
        {"positive", "positive"}
    };
};
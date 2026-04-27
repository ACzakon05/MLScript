#include <iostream>
#include <unordered_map>

#include "ModelDefinition.h"

struct LinRegDefinition : public ModelDefinition {
    std::string name = "LinearRegression";

    std::unordered_map<std::string, std::string> params = {
        {"fitintercept", "True"},
        {"tol", "1e-6"},
        {"njobs", "None"},
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
        return "from sklearn.linear_model import LinearRegression\n";
    }

private:
    std::unordered_map<std::string, std::string> mlsToScikitParam = {
        {"fitintercept", "fit_intercept"},
        {"tol", "tol"},
        {"njobs", "n_jobs"},
        {"positive", "positive"}
    };
};
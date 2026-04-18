#pragma once
#include <iostream>

struct ModelDefinition {
    virtual ~ModelDefinition() = default;

    virtual std::ostream& toScikit(std::ostream& os) = 0;
};
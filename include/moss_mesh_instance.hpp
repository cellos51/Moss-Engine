#pragma once

#include <moss_entity.hpp>

#include <string>

class MeshInstance : public Entity
{
public:
    virtual const char* getClass() const override { return "MeshInstance"; }
    std::string mesh = "";
};
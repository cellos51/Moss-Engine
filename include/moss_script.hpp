#pragma once

namespace script
{
    bool init();
    void processUpdate(float deltaTime);
    void processFixedUpdate(float deltaTime);
    void cleanup();
}
#include "RowBlastApplication.hpp"

#include <iostream>

// Engine includes.
#include "IEngine.hpp"

using namespace Game;

std::unique_ptr<Pht::IApplication> CreateApplication(Pht::IEngine& engine) {
    return std::make_unique<GameApplication>(engine);
}

GameApplication::GameApplication(Pht::IEngine& engine) {}

void GameApplication::OnUpdate() {
    std::cout << "GameApplication::OnUpdate" << std::endl;
}

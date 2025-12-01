#include <iostream>
#include "GameView.h"
#include "GameScene.h"

int main()
{
    std::cout << "=== MARIO GAME START ===" << std::endl;

    try {
        GameView* view = NewGameView(sf::Vector2i(800, 600));
        GameScene* scene = NewGameScene();

        std::cout << "Objects created successfully!" << std::endl;
        std::cout << "Starting game loop..." << std::endl;

        
        EnterGameLoop(*view, UpdateGameScene, DrawGameScene, scene);

        DestroyGameScene(scene);
        DestroyGameView(view);

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "=== PROGRAM FINISHED ===" << std::endl;
    return 0;
}
#include "GameView.h"
#include <iostream>

GameView* NewGameView(const sf::Vector2i& windowSize)
{
    GameView* view = new GameView();
    view->windowSize = windowSize;

    view->window.create(sf::VideoMode(windowSize.x, windowSize.y), "Mario Game");
    view->window.setFramerateLimit(60);

    view->camera.setSize(static_cast<sf::Vector2f>(windowSize) * 0.6f);
    view->camera.setCenter(static_cast<sf::Vector2f>(windowSize) * 0.6f);
    view->window.setView(view->camera);

    return view;
}

void EnterGameLoop(GameView& view, OnUpdate onUpdate, OnDraw onDraw, void* pData)
{
    while (view.window.isOpen())
    {
        sf::Event event;
        while (view.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                view.window.close();
        }

        float deltaSec = view.clock.restart().asSeconds();

        if (onUpdate)
            onUpdate(pData, view, deltaSec);

        view.window.clear(sf::Color::Black);
        if (onDraw)
            onDraw(pData, view);
        view.window.display();
    }
}

void SetCameraCenter(GameView& view, const sf::Vector2f& center)
{
    view.camera.setCenter(center);
    view.window.setView(view.camera);
}

void DestroyGameView(GameView*& pView)
{
    if (pView)
    {
        delete pView;
        pView = nullptr;
    }
}
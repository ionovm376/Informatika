#include "GameScene.h"
#include "GameView.h"
#include <float.h>
#include <math.h>
#include <iostream>

using sf::Keyboard;
using sf::Vector2f;
using sf::Vector2i;

static const float PLAYER_SPEED = 200;

static Vector2f Normalize(const Vector2f& value)
{
    const float length = std::hypotf(value.x, value.y);
    if (length < FLT_EPSILON)
        return Vector2f(0, 0);
    return value / length;
}

static Vector2f GetPlayerDirection()
{
    Vector2f direction;
    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
        direction.x = -1;
    else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
        direction.x = +1;
    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
        direction.y = -1;
    else if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
        direction.y = +1;
    return Normalize(direction);
}

GameScene* NewGameScene()
{
    GameScene* pLogic = new GameScene;
    TmxLevel& level = pLogic->level;

    level.LoadFromFile("res/platformer.tmx");
    pLogic->player = level.GetFirstObject("player");
    pLogic->coins = level.GetAllObjects("coin");
    pLogic->enemies = level.GetAllObjects("enemy");
    pLogic->blocks = level.GetAllObjects("block"); 
    pLogic->startPosition = pLogic->player.sprite.getPosition(); 

    return pLogic;
}

void UpdateGameScene(void* pData, GameView& view, float deltaSec)
{
    GameScene* pLogic = reinterpret_cast<GameScene*>(pData);
    if (!pLogic) return;

    TmxObject& player = pLogic->player;
    const Vector2f movement = GetPlayerDirection() * PLAYER_SPEED * deltaSec;


    sf::Vector2f oldPosition = player.sprite.getPosition();


    player.sprite.move(movement);
    player.rect.left += movement.x;
    player.rect.top += movement.y;

    bool collision = false;
    for (const auto& block : pLogic->blocks)
    {
        sf::FloatRect playerRect = player.sprite.getGlobalBounds();
        sf::FloatRect blockRect = block.rect; 

        if (playerRect.intersects(blockRect))
        {
            collision = true;
            break;
        }
    }


    if (collision)
    {
        player.sprite.setPosition(oldPosition);
        player.rect.left = oldPosition.x;
        player.rect.top = oldPosition.y;
    }


    for (auto it = pLogic->coins.begin(); it != pLogic->coins.end();)
    {
        if (player.sprite.getGlobalBounds().intersects(it->sprite.getGlobalBounds()))
        {
            it = pLogic->coins.erase(it);
        }
        else
        {
            ++it;
        }
    }


    for (const auto& enemy : pLogic->enemies)
    {
        if (player.sprite.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds()))
        {
            player.sprite.setPosition(pLogic->startPosition);
            player.rect.left = pLogic->startPosition.x;
            player.rect.top = pLogic->startPosition.y;
            break;
        }
    }


    SetCameraCenter(view, player.sprite.getPosition());
}

void DrawGameScene(void* pData, GameView& view)
{
    GameScene* pLogic = reinterpret_cast<GameScene*>(pData);
    if (!pLogic) return;

    pLogic->level.Draw(view.window);

    for (const TmxObject& coin : pLogic->coins)
        view.window.draw(coin.sprite);

    for (const TmxObject& enemy : pLogic->enemies)
        view.window.draw(enemy.sprite);

    view.window.draw(pLogic->player.sprite);
}

void DestroyGameScene(GameScene*& pScene)
{
    delete pScene;
    pScene = nullptr;
}

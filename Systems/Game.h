#include <SFML/Graphics.hpp>

#include "../Entities/EntityManager.h"

struct PlayerConfig
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, SMB;
    float S;
};

struct EnemyConfig
{
    int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI, SMIN, SMAX, SEL;
};

struct BulletConfig
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
    float S;
};

#ifndef GAME_H
#define GAME_H
class Game
{
    sf::RenderWindow _window;
    EntityManager _entityManager;
    sf::Font _font;
    sf::Text _text;
    PlayerConfig _playerConfig;
    EnemyConfig _enemyConfig;
    BulletConfig _bulletConfig;
    int _score = 0;
    int _currentFrame = 0;
    int _lastEnemySpawnTime = 0;
    int _lastSpecialMoveTime = 0;
    int _specialMoveCooldownTime = 200;
    bool _paused = false;
    bool _running = true;

    std::shared_ptr<Entity> _player;

    void init(const std::string& config);
    void setPaused(bool paused);  // pause the game

    void sMovement();      // System: Entity position / movement update
    void sUserInput();     // System: User Input
    void sLifespan();      // System: Lifespan
    void sRender();        // System: Render / Drawing
    void sEnemySpawner();  // System: Spawns Enemies
    void sCollision();     // System: Collisions

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

   public:
    Game(const std::string& config);  // constructor, takes in game config

    void run();
};
#endif
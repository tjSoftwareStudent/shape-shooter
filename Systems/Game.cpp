#include "Game.h"

#include <fstream>
#include <sstream>

Game::Game(const std::string& config)
{
    init(config);
}

void Game::init(const std::string& config)
{
    // Read config file
    // Create an ifstream object to open and read from a file
    std::ifstream file(config);

    // Check if the file opened successfully
    if (!file.is_open())
    {
        throw std::invalid_argument("Error: Could not open the file.");
    }

    std::string line;

    // Read each line from the config file
    while (std::getline(file, line))
    {
        // Process each line and create objects
        std::istringstream lineStream(line);
        std::string type;
        lineStream >> type;

        if (type == "Window")
        {
            int width, height, frameRate, isFullscreen;
            lineStream >> width >> height >> frameRate >> isFullscreen;

            auto style =
                isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;

            _window.create(
                sf::VideoMode(width, height), "Shape Shooter", style);
            _window.setFramerateLimit(frameRate);
        }
        else if (type == "Font")
        {
            std::string fontFile;
            int fontSize, r, g, b;
            lineStream >> fontFile >> fontSize >> r >> g >> b;

            if (!_font.loadFromFile(fontFile))
            {
                std::cerr << "Error loading font from file" << std::endl;

                throw std::invalid_argument(fontFile);
            }

            _text.setFont(_font);  // Set the font
            // text.setString(listOfWords[1]);        // Set the text string
            _text.setCharacterSize(fontSize);  // Set the text size
            _text.setFillColor(sf::Color(r, g, b));
        }
        else if (type == "Player")
        {
            lineStream >> _playerConfig.SR >> _playerConfig.CR >>
                _playerConfig.S >> _playerConfig.FR >> _playerConfig.FG >>
                _playerConfig.FB >> _playerConfig.OR >> _playerConfig.OG >>
                _playerConfig.OB >> _playerConfig.OT >> _playerConfig.V;
        }
        else if (type == "Enemy")
        {
            lineStream >> _enemyConfig.SR >> _enemyConfig.CR >>
                _enemyConfig.SMIN >> _enemyConfig.SMAX >> _enemyConfig.OR >>
                _enemyConfig.OG >> _enemyConfig.OB >> _enemyConfig.OT >>
                _enemyConfig.VMIN >> _enemyConfig.VMAX >> _enemyConfig.L >>
                _enemyConfig.SI;
        }
        else if (type == "Bullet")
        {
            lineStream >> _bulletConfig.SR >> _bulletConfig.CR >>
                _bulletConfig.S >> _bulletConfig.FR >> _bulletConfig.FG >>
                _bulletConfig.FB >> _bulletConfig.OR >> _bulletConfig.OG >>
                _bulletConfig.OB >> _bulletConfig.OT >> _bulletConfig.V >>
                _bulletConfig.L;
        }
    }

    // Close the file after reading
    file.close();

    // setup default window parameters

    spawnPlayer();
}

void Game::setPaused(bool paused)
{
    _paused = paused;
}

void Game::spawnPlayer()
{
    // we create every entity by calling EntityManager.addEntity(tag)
    // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
    _player = _entityManager.addEntity(EntityType::PLAYER);

    float x = _window.getSize().x / 2.0f;
    float y = _window.getSize().y / 2.0f;
    // Give the player a Transform so it spawns at center of window with
    // velocity (1,1) and angle 0
    _player->cTransform =
        std::make_shared<CTransform>(Vec2(x, y), Vec2(2.0f, 2.0f), 0.0f);

    // Initialize shape using player config values
    _player->cShape = std::make_shared<CShape>(
        _playerConfig.SR,
        _playerConfig.V,
        sf::Color(_playerConfig.FR, _playerConfig.FG, _playerConfig.FB),
        sf::Color(_playerConfig.OR, _playerConfig.OG, _playerConfig.OB),
        _playerConfig.OT);

    _player->cShape->shape.setPosition(_player->cTransform->pos.x,
                                       _player->cTransform->pos.y);

    // Add an input component to the player so that we can use inputs
    _player->cInput = std::make_shared<CInput>();

    // Add collision component
    _player->cCollision = std::make_shared<CCollision>(_playerConfig.CR);

    // Add score component
    _player->cScore = std::make_shared<CScore>(0);
}

void Game::sMovement()
{
    // TODO: implement all entity movement in this function
    //       you should read the _player->cInput component to determine if the
    //       player is moving

    // Player movement
    _player->cTransform->velocity = {0, 0};

    if (_player->cInput->up)
    {
        _player->cTransform->velocity.y = -5;
    }

    if (_player->cInput->right)
    {
        _player->cTransform->velocity.x = 5;
    }

    if (_player->cInput->down)
    {
        _player->cTransform->velocity.y += 5;
    }

    if (_player->cInput->left)
    {
        _player->cTransform->velocity.x = -5;
    }

    // _player->cShape->shape.setPosition(
    //     sf::Vector2f(_player->cTransform->pos.x,
    //     _player->cTransform->pos.y));
    // _player->cTransform->angle += 1.0f;
    // _player->cShape->shape.setRotation(_player->cTransform->angle);

    // Handle movement for enemies
    for (auto& e : _entityManager.getEntities())
    {
        if (e->cTransform)
        {
            e->cTransform->pos.x += e->cTransform->velocity.x;
            e->cTransform->pos.y += e->cTransform->velocity.y;

            if (e->cTransform->pos.x <= e->cCollision->radius ||
                e->cTransform->pos.x >=
                    (_window.getSize().x - e->cCollision->radius))
            {
                e->cTransform->velocity.x *= -1;
            }
            if (e->cTransform->pos.y <= e->cCollision->radius ||
                e->cTransform->pos.y >=
                    (_window.getSize().y - e->cCollision->radius))
            {
                e->cTransform->velocity.y *= -1;
            }

            e->cShape->shape.setPosition(
                sf::Vector2f(e->cTransform->pos.x, e->cTransform->pos.y));
            // set the rotation of the shape based on the entity's
            // transform->pos
            e->cTransform->angle += 1.0f;
            e->cShape->shape.setRotation(e->cTransform->angle);
        }
    }
}

void Game::sUserInput()
{
    // TODO: handle user input here
    //       note that you should only be setting th eplayer's input component
    //       variables here you should not implement the player's movement logic
    //       here the movement system will read the variables you set in this
    //       function
    sf::Event event;
    if (_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            _window.close();  // Close window if "close" event occurs
            _running = false;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    _player->cInput->up = true;
                    break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    _player->cInput->right = true;
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    _player->cInput->down = true;
                    break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    _player->cInput->left = true;
                    break;
                case sf::Keyboard::P:
                    setPaused(!_paused);
                    break;
                default:
                    break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    _player->cInput->up = false;
                    break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    _player->cInput->right = false;
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    _player->cInput->down = false;
                    break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    _player->cInput->left = false;
                    break;
                default:
                    break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    _player->cInput->up = false;
                    break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    _player->cInput->right = false;
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    _player->cInput->down = false;
                    break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    _player->cInput->left = false;
                    break;
                default:
                    break;
            }
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "Left Mouse Button Clicked at (" << ","
                          << event.mouseButton.y << ")\n";
                // call spawnBullet
                spawnBullet(_player,
                            Vec2(event.mouseButton.x, event.mouseButton.y));
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "Right Mouse Button Clicked at (" << ","
                          << event.mouseButton.y << ")\n";
                // call spawnSpecialWeapon
            }
        }
    }
}

void Game::sLifespan()
{
    for (auto& entity : _entityManager.getEntities())
    {
        if (entity->cLifespan && entity->cLifespan->remaining <= 0)
        {
            entity->destroy();
        }
        else if (entity->cLifespan)
        {
            entity->cLifespan->remaining -= 1;
        }
    }
}

void Game::sRender()
{
    _window.clear();

    for (auto e : _entityManager.getEntities())
    {
        // draw entity
        _window.draw(e->cShape->shape);
    }

    _window.display();
}

void Game::sEnemySpawner()
{
    if ((_currentFrame - _lastEnemySpawnTime) > _enemyConfig.SI)
    {
        spawnEnemy();
    }
}

void Game::sCollision()
{
    // TODO: implement all proper collisions between entities
    //         be sure to use the collision radius, NOT the shape radius
    for (auto& b : _entityManager.getEntities(EntityType::BULLET))
    {
        for (auto& e : _entityManager.getEntities(EntityType::ENEMY))
        {
            // if distance < r1 + r2
            if (e->cTransform->pos.dist(b->cTransform->pos) <=
                (e->cCollision->radius + b->cCollision->radius))
            {
                e->destroy();
                b->destroy();
                // spawnSmallEnemies(e);
            }
        }
    }
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
    auto entity = _entityManager.addEntity(EntityType::ENEMY);

    float x = _enemyConfig.CR +
              (rand() % (_window.getSize().x - (2 * _enemyConfig.CR)));
    float y = _enemyConfig.CR +
              (rand() % (_window.getSize().y - (2 * _enemyConfig.CR)));

    int speed = _enemyConfig.SMIN +
                (rand() % (1 + _enemyConfig.SMAX - _enemyConfig.SMIN));

    // random number from 0,1   - rand % 2 , 0 * 1, 1 * -1
    float velocityX = 1;
    float velocityY = 1;

    if (rand() % 2 == 0)
    {
        velocityX = -1;
    }
    if (rand() % 2 == 0)
    {
        velocityY = -1;
    }

    Vec2 velocity = Vec2(velocityX * rand(), velocityY * rand());
    velocity.normalize();
    velocity = velocity * speed;

    // Give the enemy a Transform so it spawns at random position in window
    // with velocity (1,1) and angle 0
    entity->cTransform =
        std::make_shared<CTransform>(Vec2(x, y), velocity, 0.0f);

    // Initialize shape using player config values
    int numberOfVertices =
        _enemyConfig.VMIN +
        (rand() % (1 + _enemyConfig.VMAX - _enemyConfig.VMIN));

    entity->cShape = std::make_shared<CShape>(
        _enemyConfig.SR,
        numberOfVertices,
        sf::Color(rand() % 256, rand() % 256, rand() % 256),
        sf::Color(_enemyConfig.OR, _enemyConfig.OG, _enemyConfig.OB),
        _enemyConfig.OT);

    entity->cShape->shape.setPosition(entity->cTransform->pos.x,
                                      entity->cTransform->pos.y);

    // Add collision component
    entity->cCollision = std::make_shared<CCollision>(_enemyConfig.CR);

    // Add lifespan component
    // entity->cLifespan = std::make_shared<CLifespan>(_enemyConfig.L);

    // record when the most recent enemy was spawned
    _lastEnemySpawnTime = _currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
    // number of enemies to spawn = number of sides of enemy shape (Points)
    for (int i = 0; i < entity->cShape->shape.getPointCount(); i++)
    {
        // add enity
        // summa cum laude
        auto e = _entityManager.addEntity(EntityType::ENEMY);
        e->cLifespan = std::make_shared<CLifespan>(5);
        e->cShape = std::make_shared<CShape>(
            _enemyConfig.SR / 4,
            entity->cShape->shape.getPointCount(),
            // fillcolor
            entity->cShape->shape.getFillColor(),
            sf::Color(_enemyConfig.OR, _enemyConfig.OG, _enemyConfig.OB),
            _enemyConfig.OT);
    }
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
    auto bullet = _entityManager.addEntity(EntityType::BULLET);
    auto differenceVector = mousePos - entity->cTransform->pos;

    differenceVector.normalize();

    auto velocity = differenceVector * _bulletConfig.S;

    bullet->cTransform =
        std::make_shared<CTransform>(entity->cTransform->pos, velocity, 0);
    bullet->cShape = std::make_shared<CShape>(
        _bulletConfig.SR,
        _bulletConfig.V,
        sf::Color(_bulletConfig.FR, _bulletConfig.FG, _bulletConfig.FB),
        sf::Color(_bulletConfig.OR, _bulletConfig.OG, _bulletConfig.OB),
        _bulletConfig.OT);
    bullet->cLifespan = std::make_shared<CLifespan>(30);
    bullet->cCollision = std::make_shared<CCollision>(_bulletConfig.CR);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}

void Game::run()
{
    while (_running)
    {
        _entityManager.update();
        // Handle input every frame
        sUserInput();

        if (!_paused)
        {
            sEnemySpawner();
            sMovement();
            sCollision();
            sLifespan();
            // increment the current frame
            _currentFrame++;
        }

        sRender();
    }
}
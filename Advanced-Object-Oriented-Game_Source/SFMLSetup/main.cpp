#include "Level.h"

int main()
{
    //Render window
    sf::RenderWindow window(sf::VideoMode({ 1792, 960 }), "SFML works!");
    window.setFramerateLimit(60);

    //Create actors & helper
    Character* Player = new Character(CType_Player);
    Character* Enemy = new Character(CType_Enemy);
    CHelper g_Helper;

    Level NewLevel(100, 100);

    //Load level initialisers
    NewLevel.LoadLevel("Levels/Level1.txt");
    CurrentLevel CurrLevel = Level_1;
    int BoxCount = 0;
    int BoxStart = 0;
    for (int i = 1; i < NewLevel.m_LevelWallColliders.size(); i++)
    {
        if (NewLevel.m_LevelWallColliders[i]->m_TileType == tile_BreakBox)
        {
            BoxCount++;
        }
    }
    BoxStart = BoxCount;
    std::cout << BoxCount << " boxes remain" << std::endl;

    //Actor load
    Player = NewLevel.m_PlaySpawn;
    Player->GetCharShape()->setOutlineColor(sf::Color::Blue);
    Enemy = NewLevel.m_EnemySpawn;
    Enemy->GetCharShape()->setOutlineColor(sf::Color::Red);

    //Bullet load
    sf::RectangleShape Bullet = sf::RectangleShape({ 10, 10 });
    Bullet.setFillColor(sf::Color::Blue);
    Bullet.setPosition({ -20,-20 });

    //Mouse variables
    sf::Vector2f MouseLocation;
    sf::Vector2f BulletRange;
    sf::Vector2f BulletMove;
    sf::Vector2f BulletOrigin;

    //Debug bools
    bool DebugMode = false;
    bool DebugInvincible = false;
    bool DebugHitbox = false;

    //Load bool
    bool LoadNow = false;

    //Bullet variables
    bool BulletFired = false;
    float BulletDirX = 0.0f;
    float BulletDirY = 0.0f;
    float BulletSpeed = 750.0f;

    while (window.isOpen())
    {
        //Mouse code
        MouseLocation = sf::Vector2f(sf::Mouse::getPosition(window));

        sf::Vector2f Offset = MouseLocation - Player->m_CharShape->getPosition();
        sf::Angle Rotation = sf::radians(atan2(Offset.y, Offset.x));


        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            //Mouse/bullet shoot
            if (const auto* MousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    BulletRange = MouseLocation;
                    Bullet.setPosition(Player->m_CharShape->getPosition());
                    Bullet.setRotation(Rotation);
                    BulletOrigin.y = Bullet.getPosition().y;
                    BulletOrigin.x = Bullet.getPosition().x;
                    BulletFired = true;
                }
            }
            //Debug keys
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Grave)
                {
                    if (DebugMode)
                    {
                        Player->m_CharSpeed = 300;
                        DebugMode = false;
                        std::cout << "Debug mode off" << std::endl;
                    }
                    else
                    {
                        DebugMode = true;
                        std::cout << "Debug mode on" << std::endl;
                    }
                }

                //Debug commands
                if (DebugMode)
                {
                    //Level select by the number keys
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
                    {
                        NewLevel.LoadLevel("Levels/Level1.txt");
                        CurrLevel = Level_1;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
                    {
                        NewLevel.LoadLevel("Levels/Level2.txt");
                        CurrLevel = Level_2;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
                    {
                        NewLevel.LoadLevel("Levels/Level3.txt");
                        CurrLevel = Level_3;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::T)
                    {
                        NewLevel.LoadLevel("Levels/LevelTest.txt");
                        CurrLevel = Level_Test;
                    }

                    //Increase player speed
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Equal)
                    {
                        Player->m_CharSpeed += 50;
                        std::cout << "Speed increased" << std::endl;


                    }
                    //Decrease player speed
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Hyphen)
                    {
                        Player->m_CharSpeed -= 50;
                        std::cout << "Speed decreased" << std::endl;
                    }
                }
            }
        }

            //Bullet direction move code
            if (BulletFired)
            {
                if (BulletOrigin.y - BulletRange.y > 0)
                {
                    BulletDirY = -1;
                }
                else if (BulletOrigin.y - BulletRange.y < 0)
                {
                    BulletDirY = 1;
                }

                if (BulletOrigin.x - BulletRange.x > 0)
                {
                    BulletDirX = -1;
                }
                else if (BulletOrigin.x - BulletRange.x < 0)
                {
                    BulletDirX = 1;
                }

                if (BulletOrigin.y - BulletRange.y < 100 && BulletOrigin.y - BulletRange.y > -100)
                {
                    BulletDirY = 0;
                }
                else if (BulletOrigin.x - BulletRange.x < 100 && BulletOrigin.x - BulletRange.x > -100)
                {
                    BulletDirX = 0;
                }

                Bullet.move({ BulletDirX * BulletSpeed * g_Helper.m_DeltaTime,BulletDirY * BulletSpeed * g_Helper.m_DeltaTime });
            }

            //Char input update
            g_Helper.HelperTick();
            Player->CharacterInputUpdate(g_Helper.m_DeltaTime, NewLevel.m_LevelWallColliders, MouseLocation, Bullet, BoxCount);
            window.clear();

            //Load next level
            if (LoadNow)
            {
                switch (CurrLevel)
                {
                case Level_1:
                    NewLevel.LoadLevel("Levels/Level2.txt");
                    CurrLevel = Level_2;
                    LoadNow = false;
                    break;
                case Level_2:
                    NewLevel.LoadLevel("Levels/Level3.txt");
                    CurrLevel = Level_3;
                    LoadNow = false;
                    break;
                case Level_3:
                    NewLevel.LoadLevel("Levels/Level1.txt");
                    CurrLevel = Level_1;
                    LoadNow = false;
                    break;
                case Level_Test:
                    NewLevel.LoadLevel("Levels/LevelTest.txt");
                    CurrLevel = Level_Test;
                    LoadNow = false;
                    break;
                default:
                    break;
                }
                //Resets bullet
                Bullet.setPosition({ -20,-20 });
                BulletFired = false;
                //Set new box count
                BoxCount = 0;
                for (int i = 1; i < NewLevel.m_LevelWallColliders.size(); i++)
                {
                    if (NewLevel.m_LevelWallColliders[i]->m_TileType == tile_BreakBox)
                    {
                        BoxCount++;
                    }
                }
                BoxStart = BoxCount;
                system("cls");
                std::cout << BoxCount << " boxes remain" << std::endl;
            }

            //Level load loops      
            if (Enemy->GetCharShape()->getGlobalBounds().findIntersection(Bullet.getGlobalBounds()))
            {
                if (BoxCount == 0)
                {
                    LoadNow = true;
                }
                else
                {
                    switch (CurrLevel)
                    {
                    case Level_1:
                        NewLevel.LoadLevel("Levels/Level1.txt");
                        break;
                    case Level_2:
                        NewLevel.LoadLevel("Levels/Level2.txt");
                        break;
                    case Level_3:
                        NewLevel.LoadLevel("Levels/Level3.txt");
                        break;
                    case Level_Test:
                        NewLevel.LoadLevel("Levels/LevelTest.txt");
                        break;
                    default:
                        break;
                    }
                    //Reset bullet
                    Bullet.setPosition({ -20,-20 });
                    BulletFired = false;
                    //Box count reset
                    BoxCount = 0;
                    for (int i = 1; i < NewLevel.m_LevelWallColliders.size(); i++)
                    {
                        if (NewLevel.m_LevelWallColliders[i]->m_TileType == tile_BreakBox)
                        {
                            BoxCount++;
                        }
                    }
                    BoxStart = BoxCount;
                    system("cls");
                    std::cout << BoxCount << " boxes remain" << std::endl;
                }
            }
        //Draw game
        for (int i = 0; i < NewLevel.m_LevelTiles.size(); i++)
        {
            window.draw(NewLevel.m_LevelTiles[i]->m_TileShape);
        }

        for (int i = 0; i < NewLevel.m_LevelWallTiles.size(); i++)
        {
           window.draw(NewLevel.m_LevelWallTiles[i]->m_TileShape);
        }

        window.draw(*Player->GetCharShape());
        window.draw(*Enemy->GetCharShape());
        window.draw(Bullet);
        window.display();
    }
}
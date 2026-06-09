#include "Level.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1024, 768 }), "SFML works!");
    window.setFramerateLimit(60);

    Character* Player = new Character(CType_Player);
    Character* Enemy = new Character(CType_Enemy);
    CHelper g_Helper;

    Level NewLevel(100,100);

    NewLevel.LoadLevel("Levels/Level1.txt");
    CurrentLevel CurrLevel = Level_1;

    Player = NewLevel.m_PlaySpawn;
    Player->GetCharShape()->setOutlineColor(sf::Color::Blue);
    Enemy = NewLevel.m_EnemySpawn;
    Enemy->GetCharShape()->setOutlineColor(sf::Color::Red);

    //Debug bools
    bool DebugMode = false;
    bool DebugInvincible = false;
    bool DebugHitbox = false;

    bool LoadNow = false;
    bool EnemyPresent = false;
    bool EnemySpawn = true;
    bool KeyGet = false;

    while (window.isOpen())
    {
        if (EnemySpawn)
        {
            Enemy = new Character(CType_Enemy);
            Enemy = NewLevel.m_EnemySpawn;
            Enemy->GetCharShape()->setOutlineColor(sf::Color::Red);
            EnemySpawn = false;
        }

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Grave)
                {
                    if (DebugMode)
                    {
                        DebugMode = false;
                        DebugInvincible = false;
                        DebugHitbox = false;
                    }
                    else
                    {
                        DebugMode = true;
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
                        EnemySpawn = false;
                        EnemyPresent = false;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
                    {
                        NewLevel.LoadLevel("Levels/Level2.txt");
                        CurrLevel = Level_2;
                        EnemySpawn = false;
                        EnemyPresent = false;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
                    {
                        NewLevel.LoadLevel("Levels/Level3.txt");
                        CurrLevel = Level_3;
                        EnemySpawn = true;
                        EnemyPresent = true;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Num4)
                    {
                        NewLevel.LoadLevel("Levels/Level4.txt");
                        CurrLevel = Level_4;
                        EnemySpawn = false;
                        EnemyPresent = false;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Num5)
                    {
                        NewLevel.LoadLevel("Levels/Level5.txt");
                        CurrLevel = Level_5;
                        EnemySpawn = true;
                        EnemyPresent = true;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::T)
                    {
                        NewLevel.LoadLevel("Levels/LevelTest.txt");
                        CurrLevel = Level_Test;
                        EnemySpawn = true;
                        EnemyPresent = true;
                    }

                    //Toggle invincibility bool
                    if (keyPressed->scancode == sf::Keyboard::Scancode::I)
                    {
                        if (DebugInvincible)
                        {
                            DebugInvincible = false;
                        }
                        else
                        {
                            DebugInvincible = true;
                        }
                    }

                    //Toggle hitbox bool
                    if (keyPressed->scancode == sf::Keyboard::Scancode::H)
                    {
                        if (DebugHitbox)
                        {
                            DebugHitbox = false;
                        }
                        else
                        {
                            DebugHitbox = true;
                        }
                    }
                }
            }
        }

        //Showing hitboxes from debug menu
        if (DebugHitbox)
        {
            Player->GetCharShape()->setOutlineThickness(-1);
            if (EnemyPresent)
            {
                Enemy->GetCharShape()->setOutlineThickness(-1);
            }
        }
        else
        {
            Player->GetCharShape()->setOutlineThickness(0);
            if (EnemyPresent)
            {
                Enemy->GetCharShape()->setOutlineThickness(0);
            }
        }

        g_Helper.HelperTick();

        Player->CharacterInputUpdate(g_Helper.m_DeltaTime, NewLevel.m_LevelWallColliders, KeyGet);
        if(EnemyPresent)
        {
            Enemy->CharacterInputUpdate(g_Helper.m_DeltaTime, NewLevel.m_LevelWallColliders,false);
        }

        window.clear();

        if (LoadNow)
        {
            switch (CurrLevel)
            {
            case Level_1:
                NewLevel.LoadLevel("Levels/Level2.txt");
                CurrLevel = Level_2;
                LoadNow = false;
                EnemyPresent = false;
                EnemySpawn = false;
                break;
            case Level_2:
                NewLevel.LoadLevel("Levels/Level3.txt");
                CurrLevel = Level_3;
                LoadNow = false;
                EnemyPresent = true;
                EnemySpawn = true;
                KeyGet = false;
                break;
            case Level_3:
                NewLevel.LoadLevel("Levels/Level4.txt");
                CurrLevel = Level_4;
                LoadNow = false;
                EnemyPresent = false;
                EnemySpawn = false;
                break;
            case Level_4:
                NewLevel.LoadLevel("Levels/Level5.txt");
                CurrLevel = Level_5;
                LoadNow = false;
                EnemyPresent = true;
                EnemySpawn = true;
                KeyGet = false;
                break;
            case Level_5:
                NewLevel.LoadLevel("Levels/Level1.txt");
                CurrLevel = Level_1;
                LoadNow = false;
                EnemyPresent = false;
                EnemySpawn = false;
                break;
            case Level_Test:
                NewLevel.LoadLevel("Levels/LevelTest.txt");
                CurrLevel = Level_Test;
                EnemyPresent = true;
                EnemySpawn = true;
                KeyGet = false;
                LoadNow = false;
                break;
            default:
                break;
            }
        }

        //Level load loops
        for (int i = 0; i < NewLevel.m_InteractableTiles.size(); i++)
        {
            if (Player->GetCharShape()->getGlobalBounds().findIntersection(NewLevel.m_InteractableTiles[i]->m_TileShape.getGlobalBounds()))
            {
                LoadNow = true;
            }
        }

        if (EnemyPresent)
        {
            if (Player->GetCharShape()->getGlobalBounds().findIntersection(Enemy->GetCharShape()->getGlobalBounds()))
            {
                float Intersect = Player->GetCharShape()->getPosition().y - Enemy->GetCharShape()->getPosition().y;
                if (Intersect <= -9.0 && Intersect >= -14.9 && !DebugInvincible)
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
                        EnemyPresent = true;
                        break;
                    case Level_4:
                        NewLevel.LoadLevel("Levels/Level4.txt");
                        EnemyPresent = false;
                        break;
                    case Level_5:
                        NewLevel.LoadLevel("Levels/Level5.txt");
                        EnemyPresent = true;
                        break;
                    case Level_Test:
                        NewLevel.LoadLevel("Levels/LevelTest.txt");
                        EnemyPresent = true;
                        KeyGet = false;
                        break;
                    default:
                        break;
                    }
                }
                else if (Intersect <= -15.0)
                {
                    EnemyPresent = false;
                    KeyGet = true;
                    delete Enemy;
                }
            }
        }


        for (int i = 0; i < NewLevel.m_LevelTiles.size(); i++)
        {
            window.draw(NewLevel.m_LevelTiles[i]->m_TileShape);
        }

        for (int i = 0; i < NewLevel.m_LevelWallTiles.size(); i++)
        {
            window.draw(NewLevel.m_LevelWallTiles[i]->m_TileShape);
        }

        window.draw(*Player->GetCharShape());
        if (EnemyPresent)
        {
            window.draw(*Enemy->GetCharShape());
        }
        window.display();
    }
}
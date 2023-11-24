// Main.cpp
#include "Ant.h"
#include "Mine.h"
#include "Hive.h"

class World {
public:
    World () {
        initializeAvailablePositions();
        placeHives(MAX_TEAMS);
        placeMines(500);
    }

    std::vector<Vec2i> availablePositions;
    std::vector<Hive*> hives;
    std::vector<Mine*> mines;
    std::vector<Ant*> ants;
    int teamCounter = 0;
    Uint64 startTime = SDL_GetTicks64();
    double deltaTime = 0.0f;

    void initializeAvailablePositions() {
        // Start from GRID_SIZE instead of 0 and go until WINDOW_WIDTH/WINDOW_HEIGHT - GRID_SIZE
        for (int x = GRID_SIZE; x < WINDOW_WIDTH - GRID_SIZE; x += GRID_SIZE) {
            for (int y = GRID_SIZE; y < WINDOW_HEIGHT - GRID_SIZE; y += GRID_SIZE) {
                availablePositions.push_back({ x, y });
            }
        }
    }

    void removeClosePositions(const Vec2i& point) {
        availablePositions.erase(std::remove_if(availablePositions.begin(), availablePositions.end(), [&](const Vec2i& p) {
            return sqrt(pow(p.x - point.x, 2) + pow(p.y - point.y, 2)) < MIN_DISTANCE;
            }), availablePositions.end());
    }

    void placeHives(int numberOfHives) {
        while (!availablePositions.empty() && hives.size() < numberOfHives) {
            int index = rand() % availablePositions.size();
            Vec2i hivePos = availablePositions[index];
            teamCounter += 1;
            Hive* newHive = new Hive(hivePos, teamCounter);
            hives.push_back(newHive);
            removeClosePositions(hivePos);
            if (availablePositions.size() == 0) {
                std::cout << "No more space available for hives." << std::endl;
                break;
            }
        }
    }

    void placeMines(int numberOfMines) {
        while (!availablePositions.empty() && mines.size() < numberOfMines) {
            int index = rand() % availablePositions.size();
            Vec2i minePos = availablePositions[index];
            Mine* newMine = new Mine(minePos);
            mines.push_back(newMine);
            removeClosePositions(minePos);
            if (availablePositions.size() == 0) {
                std::cout << "No more space available for mines." << std::endl;
                break;
            }
        }
    }

    void step(SDL_Window* window, bool windowMoved) {
        Uint64 currentTime = SDL_GetTicks64();
        double deltaTime = (currentTime - startTime) / 1000.0f;
        startTime = currentTime;
        if (!windowMoved) {
            double fps = 1.0 / deltaTime; 
            std::string title = "Ant Hill Battle, FPS: " + std::to_string(fps);
            SDL_SetWindowTitle(window, title.c_str());
            for (Hive* hive : hives) {
                hive->step(ants, deltaTime);
            }
            for (Ant* ant : ants) {
                ant->step(mines, ants, deltaTime);
            }
            for (Mine* mine : mines) {
                mine->step();
            }

            ants.erase(std::remove_if(ants.begin(), ants.end(), [](Ant* ant) {
                return ant->hp <= 0;
                }), ants.end());

            hives.erase(std::remove_if(hives.begin(), hives.end(), [this](Hive* hive) {
                return hive->getPop(ants) == 0 && hive->food < FOOD_COST;
                }), hives.end());
        }
    }

    void draw(SDL_Renderer* renderer, TTF_Font* font) {
        for (Hive* hive : hives) {
            hive->draw(renderer);
            hive->drawTriangle(renderer, hive->teamColor);
            hive->drawText(renderer, font, std::to_string(hive->food), 0, 0);
            hive->drawText(renderer, font, std::to_string(hive->getIdle(ants)) + "/" + std::to_string(hive->getPop(ants)), 0, -15);
        }
        for (Mine* mine : mines) {
            mine->draw(renderer);
            if (mine->occupants.size() != 0) {
                mine->drawText(renderer, font, std::to_string(mine->occupants.size()) + "/" + std::to_string(mine->maxOccupants), 0, -15);
            }
        }
        std::vector<std::reference_wrapper<Ant*>> sortedAnts(ants.begin(), ants.end());
        std::sort(sortedAnts.begin(), sortedAnts.end(), [](const Ant* a, const Ant* b) {
            return a->hp > b->hp;
            });
        for (Ant* ant : sortedAnts) {
            ant->draw(renderer);
            if (ant->state != MINING) {
                ant->drawBar(renderer, ant->hp, 3, 9, 3, 0, -15, { 255, 0, 0, 255 }, { 128, 128, 128, 255 });
                //ant.drawText(renderer, font, std::to_string(ant.hp), 0, -15);
            }
        }
    }
};


class Game {
public:
    Game() {
        initializeSDL();
        world = World();
    }

    ~Game() {
        cleanUp();
    }


    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font;
    bool quit = false;
    World world;


    bool initializeSDL() {
        srand(time(NULL));
        SDL_Init(SDL_INIT_VIDEO);

        window = SDL_CreateWindow("Ant Hill Battle", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return false;
        }

        font = TTF_OpenFont("font.ttf", 12);
        if (font == nullptr) {
            std::cout << "Couldn't open font file" << std::endl;
            return false;
        }
        TTF_SetFontHinting(font, TTF_HINTING_NORMAL);
        return true;
    }

    void runGame() {
        // Event handler
        SDL_Event e;

        // Main loop
        while (!quit) {
            bool windowMoved = false;
            // Handle events on the queue
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                else if (e.type == SDL_WINDOWEVENT) {
                    switch (e.window.event) {
                    case SDL_WINDOWEVENT_MOVED:
                        windowMoved = true;
                        break;

                        // Handle other window events
                    }
                }
            }

            world.step(window, windowMoved);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            world.draw(renderer, font);

            SDL_RenderPresent(renderer);
        }
    }

    void cleanUp() {
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};


int main(int argc, char* args[]) {
    Game game = Game();
    game.runGame();
    return 0;
}

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
struct Opponent {
        double x;
        double y;
        int speed = 100;
        bool does_exist = false;
};
struct Neutralcar {
        double x;
        double y;
        int speed = 100;
        bool does_exist = false;
};
struct Pocisk {
        double x;
        double y;
        int speed = -150;
        bool does_exist = false;
};
struct Powerupp {
        double x;
        double y;
        int speed = 0;
        bool does_exist = false;
};
struct Hearths {
        double x;
        double y;
        int speed = 0;
        bool does_exist = false;
};
struct Colors {
        int czarny;
        int zielony;
        int czerwony;
        int niebieski;
};
const int pociskwidth = 4;
const int pociskheight = 8;
const int carWidth = 44;
const int carHeight = 64;
const int powerupheartWidth = 20;
const int powerupheartHeight = 20;
const int leftroadside = 100;
const int rightroadside = SCREEN_WIDTH - 100;


void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
int SDL_SETUP(int& rc, SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& screen, SDL_Texture*& scrtex, SDL_Surface*& charset, SDL_Surface*& car1, SDL_Surface*& road1, SDL_Surface*& pocisk, SDL_Surface*& car2, SDL_Surface*& car3, SDL_Surface*& road2, SDL_Surface*& road3, SDL_Surface*& road4, SDL_Surface*& powerup, SDL_Surface*& heart);
void Set_colors(Colors& colors, SDL_Surface* screen);
void Draw_screen(SDL_Surface* screen, Colors& colors, double worldTime, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* charset, char* text, int score, int lives);
void SpawnEnemy(double& enemyspawntime, int max_numberof_opponents, int wallright, int wallleft, Opponent* opponents);
void DrawandCalculatePocisks(int max_numberof_bullets, Pocisk* pociski, SDL_Surface* screen, SDL_Surface* pocisk, double delta, int yplayerspeed, int& rangeofshot);
void DrawandcalculateOpponents(int max_numberof_opponents, Opponent* opponents, SDL_Surface* screen, SDL_Surface* car2, double delta, int yplayerspeed, int car1x);
void UserInput(SDL_Event& event, double& car1Speed, int& quit, int max_numberof_bullets, Pocisk* pociski, int car1x, int car1y, bool& pause, int& car1yspeed, int& newGame, int& lives, int& powerupcount);
void UpdateTime(int& t2, double& delta, int& t1, double& worldTime, double& enemyspawntime, double& punishmenTimer, double& fps, int& frames, int& score, double& neutralspawntime, double& roadtime, double& immortal, double& poweruprate, double& heartrate);
void UpdatePlayerpostion(double delta, double car1Speed, double& car1x);
bool Iscollsion(int xleft1, int xleft2, int ybottom1, int ybottom2, int xright1, int xright2, int ytop1, int ytop2);
void checkpociskicollsion(Pocisk* pociski, int max_numberof_bullets, Opponent* opponents, int& score, int max_numberof_opponents);
void Drawroad(SDL_Surface* screen, SDL_Surface* road1, SDL_Surface* road2, int score, SDL_Surface* road3, SDL_Surface* road4, double& roadtime, double roadChangeTime, int yplayerspeed);
void SpawnNeutral(double& neutralspawntime, int max_neutral_cars, int wallright, int wallleft, Neutralcar* neutralcars);
void DrawandcalculateNeutralcar(int max_neutral_cars, Neutralcar* neutralcars, SDL_Surface* screen, SDL_Surface* car3, double delta, int yplayerspeed);
void checkpociskicollsion2(Pocisk* pociski, int max_numberof_bullets, Neutralcar* neutralcars, int& score, int max_neutral_cars, double& punishmenTimer);
void checkplayercollsionwithcars(Neutralcar* neutralcars, int max_neutral_cars, Opponent* opponents, int& score, int max_numberof_opponents, int car1y, int car1x, int& lives, double& punishmenTimer);
void GameOverr(int& quit, SDL_Surface* screen, Colors& colors, SDL_Surface* charset, char* text, SDL_Texture* scrtex, SDL_Renderer* renderer, int& GameOver, int& newGame, int score);
void Immortality(double& immortal, int& lives);
void Drawpowerups(int max_powerupandhearth, Powerupp* powerups, SDL_Surface* screen, SDL_Surface* powerup, double delta, int yplayerspeed);
void SpawnPowerup(double& poweruprate, int max_powerupandhearth, int wallright, int wallleft, Powerupp* powerups);
void DrawHearths(int max_powerupandhearth, Hearths* heearth, SDL_Surface* screen, SDL_Surface* heart, double delta, int yplayerspeed);
void SpawnHearths(double& heartrate, int max_powerupandhearth, int wallright, int wallleft, Hearths* heearth);
void checkplayercollsionwithpowerupandhearth(Hearths* heearth, Powerupp* powerups, int max_powerupandhearth, int car1y, int car1x, int& lives, int& powerupcount);
void Roadside(double& delta, double& punishmenTimer, int car1x, int x1, int x2, int& car1yspped);

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {
        int quit = 0;
        while (quit == 0) {

                srand(time(NULL));
                int t1, t2, frames = 0, rc, score = 0, car1yspped = 50, screenspeed = 50, lives = 3, newGame = 0, GameOver = 1, powerupcount = 0, rangeofshot = 200;
                double delta, worldTime = 0, punishmenTimer = 0, heartrate = 0, immortal = 0, fps = 0, car1Speed = 0, wallleft = 100, wallright = SCREEN_WIDTH - 100, poweruprate = 0;
                bool pause = false;
                SDL_Event event;
                SDL_Surface* screen, * charset;
                double car1x = SCREEN_WIDTH / 2, car1y = SCREEN_HEIGHT - 100;
                SDL_Surface* car1;
                SDL_Surface* car2;
                SDL_Surface* car3;
                SDL_Surface* road1;
                SDL_Surface* road2;
                SDL_Surface* road3;
                SDL_Surface* road4;
                SDL_Surface* powerup;
                SDL_Surface* pocisk;
                SDL_Surface* heart;
                SDL_Texture* scrtex;
                SDL_Window* window;
                SDL_Renderer* renderer;
                // okno konsoli nie jest widoczne, jeżeli chcemy zobaczyć
                // komunikaty wypisywane printf-em trzeba w opcjach:
                // project -> szablon2 properties -> Linker -> System -> Subsystem
                // zmienić na "Console"
                // console window is not visible, to see the printf output
                // the option:
                // project -> szablon2 properties -> Linker -> System -> Subsystem
                // must be changed to "Console"
                printf("wyjscie printfa trafia do tego okienka\n");
                printf("printf output goes here\n");

                if (SDL_SETUP(rc, window, renderer, screen, scrtex, charset, car1, road1, pocisk, car2, car3, road2, road3, road4, powerup, heart) == 1)
                {
                        return 1;
                }

                int max_numberof_bullets = 100;
                Pocisk* pociski = new Pocisk[max_numberof_bullets];
                int max_numberof_opponents = 20;
                double enemyspawntime = 0;
                Opponent* opponents = new Opponent[max_numberof_opponents];
                char text[128];
                Colors colors;
                int max_neutral_cars = 20;
                double neutralspawntime = 0;
                double roadtime = 0;
                double roadChangeTime = 0.01;
                Neutralcar* neutralcars = new Neutralcar[max_neutral_cars];
                int max_powerupandhearth = 5;
                Powerupp* powerups = new Powerupp[max_powerupandhearth];
                Hearths* heearth = new Hearths[max_powerupandhearth];
                Set_colors(colors, screen);


                t1 = SDL_GetTicks();

                while (!quit && !newGame) {
                        if (pause == false) {
                                UpdateTime(t2, delta, t1, worldTime, enemyspawntime, punishmenTimer, fps, frames, score, neutralspawntime, roadtime, immortal, poweruprate, heartrate);
                                SDL_FillRect(screen, NULL, colors.czarny);
                                Drawroad(screen, road1, road2, score, road3, road4, roadtime, roadChangeTime, car1yspped);
                                DrawSurface(screen, car1, car1x, car1y);
                                UpdatePlayerpostion(delta, car1Speed, car1x);
                                if (car1x + 80 <= wallleft || car1x - 80 >= wallright)
                                {
                                        car1x = SCREEN_WIDTH / 2;
                                        lives--;
                                }
                                checkpociskicollsion(pociski, max_numberof_bullets, opponents, score, max_numberof_opponents);
                                checkpociskicollsion2(pociski, max_numberof_bullets, neutralcars, score, max_neutral_cars, punishmenTimer);
                                checkplayercollsionwithcars(neutralcars, max_neutral_cars, opponents, score, max_numberof_opponents, car1y, car1x, lives, punishmenTimer);
                                DrawandCalculatePocisks(max_numberof_bullets, pociski, screen, pocisk, delta, car1yspped, rangeofshot);
                                checkplayercollsionwithpowerupandhearth(heearth, powerups, max_powerupandhearth, car1y, car1x, lives, powerupcount);
                                SpawnEnemy(enemyspawntime, max_numberof_opponents, wallright, wallleft, opponents);
                                SpawnNeutral(neutralspawntime, max_neutral_cars, wallright, wallleft, neutralcars);
                                SpawnHearths(heartrate, max_powerupandhearth, wallright, wallleft, heearth);
                                DrawandcalculateNeutralcar(max_neutral_cars, neutralcars, screen, car3, delta, car1yspped);
                                DrawandcalculateOpponents(max_numberof_opponents, opponents, screen, car2, delta, car1yspped, car1x);
                                DrawHearths(max_powerupandhearth, heearth, screen, heart, delta, car1yspped);
                                SpawnPowerup(poweruprate, max_powerupandhearth, wallright, wallleft, powerups);
                                Drawpowerups(max_powerupandhearth, powerups, screen, powerup, delta, car1yspped);
                                Roadside(delta, punishmenTimer, car1x, leftroadside, rightroadside, car1yspped);
                                Draw_screen(screen, colors, worldTime, scrtex, renderer, charset, text, score, lives);
                                Immortality(immortal, lives);
                                if (powerupcount > 0)
                                {

                                        rangeofshot = 0;
                                }
                                else
                                {
                                        rangeofshot = 200;
                                }
                                frames++;
                        }
                        else
                        {
                                t1 = SDL_GetTicks();
                        }
                        UserInput(event, car1Speed, quit, max_numberof_bullets, pociski, car1x, car1y, pause, car1yspped, newGame, lives, powerupcount);
                        if (lives == 0)
                        {
                                GameOverr(quit, screen, colors, charset, text, scrtex, renderer, GameOver, newGame, score);
                        }
                };

                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_FreeSurface(car1);
                SDL_FreeSurface(car2);
                SDL_FreeSurface(car3);
                SDL_FreeSurface(pocisk);
                SDL_FreeSurface(road1);
                SDL_FreeSurface(road2);
                SDL_FreeSurface(road3);
                SDL_FreeSurface(road4);
                SDL_FreeSurface(powerup);
                SDL_FreeSurface(heart);
                //SDL_FreeSurface(opponent);

                SDL_Quit();
                delete[]pociski;
                delete[]opponents;
                delete[]neutralcars;
                delete[]powerups;
                delete[]heearth;
        }
        return 0;
};

// narysowanie napisu txt na powierzchni screen, zaczynając od punktu (x, y)
// charset to bitmapa 128x128 zawierająca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
        SDL_Surface* charset) {
        int px, py, c;
        SDL_Rect s, d;
        s.w = 8;
        s.h = 8;
        d.w = 8;
        d.h = 8;
        while (*text) {
                c = *text & 255;
                px = (c % 16) * 8;
                py = (c / 16) * 8;
                s.x = px;
                s.y = py;
                d.x = x;
                d.y = y;
                SDL_BlitSurface(charset, &s, screen, &d);
                x += 8;
                text++;
        };
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt środka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
        SDL_Rect dest;
        dest.x = x - sprite->w / 2;
        dest.y = y - sprite->h / 2;
        dest.w = sprite->w;
        dest.h = sprite->h;
        SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
        int bpp = surface->format->BytesPerPixel;
        Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
        *(Uint32*)p = color;
};


// rysowanie linii o długości l w pionie (gdy dx = 0, dy = 1) 
// bądź poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
        for (int i = 0; i < l; i++) {
                DrawPixel(screen, x, y, color);
                x += dx;
                y += dy;
        };
};


// rysowanie prostokąta o długości boków l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
        Uint32 outlineColor, Uint32 fillColor) {
        int i;
        DrawLine(screen, x, y, k, 0, 1, outlineColor);
        DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
        DrawLine(screen, x, y, l, 1, 0, outlineColor);
        DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
        for (i = y + 1; i < y + k - 1; i++)
                DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

int SDL_SETUP(int& rc, SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& screen, SDL_Texture*& scrtex, SDL_Surface*& charset, SDL_Surface*& car1, SDL_Surface*& road1, SDL_Surface*& pocisk, SDL_Surface*& car2, SDL_Surface*& car3, SDL_Surface*& road2, SDL_Surface*& road3, SDL_Surface*& road4, SDL_Surface*& powerup, SDL_Surface*& heart)
{

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                printf("SDL_Init error: %s\n", SDL_GetError());
                return 1;
        }

        // tryb pełnoekranowy / fullscreen mode
//      rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//                                       &window, &renderer);
        rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
                &window, &renderer);
        if (rc != 0) {
                SDL_Quit();
                printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
                return 1;
        };

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_SetWindowTitle(window, "Krzysztof Ostrzycki 193507");


        screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

        scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_STREAMING,
                SCREEN_WIDTH, SCREEN_HEIGHT);


        // wyłączenie widoczności kursora myszy
        SDL_ShowCursor(SDL_DISABLE);

        // wczytanie obrazka cs8x8.bmp
        charset = SDL_LoadBMP("./cs8x8.bmp");
        if (charset == NULL) {
                printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        SDL_SetColorKey(charset, true, 0x000000);
        car1 = SDL_LoadBMP("./carrr.bmp");
        if (car1 == NULL) {
                printf("SDL_LoadBMP(carrr.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        car2 = SDL_LoadBMP("./enemycar.bmp");
        if (car2 == NULL) {
                printf("SDL_LoadBMP(enemycar.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        car3 = SDL_LoadBMP("./neutralcar.bmp");
        if (car3 == NULL) {
                printf("SDL_LoadBMP(neutralcar.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        road1 = SDL_LoadBMP("./road1.bmp");
        if (road1 == NULL) {
                printf("SDL_LoadBMP(road1.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        road2 = SDL_LoadBMP("./road2.bmp");
        if (road2 == NULL) {
                printf("SDL_LoadBMP(road2.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        road3 = SDL_LoadBMP("./road3.bmp");
        if (road3 == NULL) {
                printf("SDL_LoadBMP(road3.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        road4 = SDL_LoadBMP("./road4.bmp");
        if (road4 == NULL) {
                printf("SDL_LoadBMP(road4.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        pocisk = SDL_LoadBMP("./strzal.bmp");
        if (pocisk == NULL) {
                printf("SDL_LoadBMP(strzal.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        powerup = SDL_LoadBMP("./powerup.bmp");
        if (powerup == NULL) {
                printf("SDL_LoadBMP(powerup.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        heart = SDL_LoadBMP("./heart.bmp");
        if (heart == NULL) {
                printf("SDL_LoadBMP(heart.bmp) error: %s\n", SDL_GetError());
                SDL_FreeSurface(charset);
                SDL_FreeSurface(screen);
                SDL_DestroyTexture(scrtex);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
                return 1;
        };
        return 0;
}
void Set_colors(Colors& colors, SDL_Surface* screen)
{
        colors.czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
        colors.zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
        colors.czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
        colors.niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
}
void Draw_screen(SDL_Surface* screen, Colors& colors, double worldTime, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* charset, char* text, int score, int lives)
{
        // tekst informacyjny / info text
        DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 22, colors.czerwony, colors.niebieski);
        //            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
        sprintf(text, "Krzysztof Ostrzycki 193507, czas trwania = %.1lf s lives:%d   score=%d", worldTime, lives, score);
        DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
        DrawRectangle(screen, SCREEN_WIDTH - 98, SCREEN_HEIGHT - 98, 98, 98, colors.czerwony, colors.niebieski);
        sprintf(text, "Esc-quit ");
        DrawString(screen, 547, 385, text, charset);
        sprintf(text, "n-new game ");
        DrawString(screen, 547, 395, text, charset);
        sprintf(text, "p-pause ");
        DrawString(screen, 547, 405, text, charset);
        sprintf(text, "space-shot");
        DrawString(screen, 547, 415, text, charset);
        sprintf(text, "s-save game");
        DrawString(screen, 547, 425, text, charset);
        sprintf(text, "l-load game");
        DrawString(screen, 547, 435, text, charset);
        sprintf(text, "f-end game");
        DrawString(screen, 547, 445, text, charset);
        sprintf(text, "arrows-move");
        DrawString(screen, 547, 455, text, charset);


        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);
}
void Drawroad(SDL_Surface* screen, SDL_Surface* road1, SDL_Surface* road2, int score, SDL_Surface* road3, SDL_Surface* road4, double& roadtime, double roadChangeTime, int yplayerspeed)
{
        static double y = 0;
        if (roadtime > roadChangeTime)
        {
                roadtime = 0;
                y += yplayerspeed / 100.0000;
                if (y >= 236)
                {
                        y = 0;

                }
        }
        DrawSurface(screen, road1, SCREEN_WIDTH / 2, SCREEN_HEIGHT + 195 + y);
        DrawSurface(screen, road2, SCREEN_WIDTH / 2, SCREEN_HEIGHT + 136 + y);
        DrawSurface(screen, road3, SCREEN_WIDTH / 2, SCREEN_HEIGHT + 77 + y);
        DrawSurface(screen, road4, SCREEN_WIDTH / 2, SCREEN_HEIGHT + 18 + y);
        DrawSurface(screen, road1, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 41 + y);
        DrawSurface(screen, road2, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100 + y);
        DrawSurface(screen, road3, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 159 + y);
        DrawSurface(screen, road4, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 218 + y);
        DrawSurface(screen, road1, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 277 + y);
        DrawSurface(screen, road2, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 336 + y);
        DrawSurface(screen, road3, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 395 + y);
        DrawSurface(screen, road4, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 454 + y);
        DrawSurface(screen, road1, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 513 + y);
        DrawSurface(screen, road2, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 572 + y);
        DrawSurface(screen, road3, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 631 + y);
        DrawSurface(screen, road4, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 690 + y);
}
void SpawnEnemy(double& enemyspawntime, int max_numberof_opponents, int wallright, int wallleft, Opponent* opponents)
{
        if (enemyspawntime > 2.9)
        {
                int los = rand() % 2;
                enemyspawntime = 0;
                if (los == 0)
                {
                        for (int i = 0; i < max_numberof_opponents; i++)
                        {
                                if (opponents[i].does_exist == false)
                                {
                                        opponents[i].does_exist = true;
                                        opponents[i].y = 10;
                                        int szerokoscplanszy = wallright - wallleft - 1.5 * (carWidth);
                                        int x = rand() % szerokoscplanszy;
                                        x += wallleft + carWidth;
                                        opponents[i].x = x;;
                                        break;
                                }
                        }
                }
        }
}
void DrawandCalculatePocisks(int max_numberof_bullets, Pocisk* pociski, SDL_Surface* screen, SDL_Surface* pocisk, double delta, int yplayerspeed, int& rangeofshot)
{
        for (int i = 0; i < max_numberof_bullets; i++)
        {
                if (pociski[i].does_exist == true)
                {
                        DrawSurface(screen, pocisk, pociski[i].x, pociski[i].y);
                        double deltay = delta * (pociski[i].speed - yplayerspeed);
                        pociski[i].y += deltay;
                        if (pociski[i].y > SCREEN_HEIGHT || pociski[i].y < rangeofshot)
                        {
                                pociski[i].does_exist = false;
                        }
                }
        }
}
void DrawandcalculateOpponents(int max_numberof_opponents, Opponent* opponents, SDL_Surface* screen, SDL_Surface* car2, double delta, int yplayerspeed, int car1x)
{
        for (int i = 0; i < max_numberof_opponents; i++)
        {
                if (opponents[i].does_exist == true)
                {
                        DrawSurface(screen, car2, opponents[i].x, opponents[i].y);
                        int xspped = 0;
                        if (car1x > opponents[i].x)
                        {
                                xspped = 100;
                        }
                        if (car1x < opponents[i].x)
                        {
                                xspped = -100;
                        }
                        double deltay = delta * (opponents[i].speed + yplayerspeed);
                        double deltax = delta * xspped;
                        opponents[i].y += deltay;
                        opponents[i].x += deltax;

                        if (opponents[i].y > SCREEN_HEIGHT || opponents[i].y < 0)
                        {
                                opponents[i].does_exist = false;
                        }
                }
        }
}
void UserInput(SDL_Event& event, double& car1Speed, int& quit, int max_numberof_bullets, Pocisk* pociski, int car1x, int car1y, bool& pause, int& car1yspeed, int& newGame, int& lives, int& powerupcount)
{

        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                                quit = 1;
                        }
                        else if (event.key.keysym.sym == SDLK_LEFT) car1Speed = -200;
                        else if (event.key.keysym.sym == SDLK_RIGHT) car1Speed = 200;
                        else if (event.key.keysym.sym == SDLK_UP)
                        {
                                car1yspeed += 5;
                                if (car1yspeed > 300)
                                {
                                        car1yspeed = 300;
                                }
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN)
                        {
                                car1yspeed -= 5;
                                if (car1yspeed < 0)
                                {
                                        car1yspeed = 0;
                                }
                        }
                        else if (event.key.keysym.sym == SDLK_f)
                        {
                                lives = 0;
                        }
                        else if (event.key.keysym.sym == SDLK_SPACE)
                        {
                                for (int i = 0; i < max_numberof_bullets; i++)
                                {
                                        if (pociski[i].does_exist == false)
                                        {
                                                pociski[i].does_exist = true;
                                                pociski[i].x = car1x;
                                                pociski[i].y = car1y - 40;
                                                powerupcount--;
                                                if (powerupcount < 0)
                                                {
                                                        powerupcount = 0;
                                                }
                                                break;
                                        }
                                }
                        }
                        else if (event.key.keysym.sym == SDLK_p)
                        {
                                pause = !pause;
                        }
                        else if (event.key.keysym.sym == SDLK_n)
                        {
                                newGame = 1;
                        }
                        break;
                case SDL_KEYUP:
                        car1Speed = 0;
                        break;
                case SDL_QUIT:
                        quit = 1;
                        break;
                };
        };
}
void UpdateTime(int& t2, double& delta, int& t1, double& worldTime, double& enemyspawntime, double& punishmenTimer, double& fps, int& frames, int& score, double& neutralspawntime, double& roadtime, double& immortal, double& poweruprate, double& heartrate)
{
        t2 = SDL_GetTicks();


        delta = (t2 - t1) * 0.001;
        t1 = t2;
        roadtime += delta;
        worldTime += delta;
        immortal += delta;
        enemyspawntime += delta;
        neutralspawntime += delta;
        poweruprate += delta;
        heartrate += delta;
        punishmenTimer += delta;
        if (punishmenTimer > 0.5) {
                fps = frames * 2;
                frames = 0;
                punishmenTimer -= 0.5;
                score += 50;
        };
}
void UpdatePlayerpostion(double delta, double car1Speed, double& car1x)
{
        double deltax = delta * car1Speed;
        car1x += deltax;
}
bool Iscollsion(int xleft1, int xleft2, int ybottom1, int ybottom2, int xright1, int xright2, int ytop1, int ytop2)
{
        if ((xright1 > xleft2 && xright2 > xleft1) && (ybottom1 > ytop2 && ybottom2 > ytop1))
                return true;
        else
                return false;
}
void checkpociskicollsion(Pocisk* pociski, int max_numberof_bullets, Opponent* opponents, int& score, int max_numberof_opponents)
{
        for (int i = 0; i < max_numberof_bullets; i++)
        {
                if (pociski[i].does_exist == true)
                {
                        for (int j = 0; j < max_numberof_opponents; j++)
                        {
                                if (opponents[j].does_exist == true)
                                {
                                        int xleft1 = pociski[i].x - pociskwidth / 2;
                                        int xleft2 = opponents[j].x - carWidth / 2;
                                        int ybottom1 = pociski[i].y + pociskheight / 2;
                                        int ybottom2 = opponents[j].y + carHeight / 2;
                                        int xright1 = pociski[i].x + pociskwidth / 2;
                                        int xright2 = opponents[j].x + carWidth / 2;
                                        int ytop1 = pociski[i].y - pociskheight / 2;
                                        int ytop2 = opponents[j].y - carHeight / 2;
                                        bool collision = Iscollsion(xleft1, xleft2, ybottom1, ybottom2, xright1, xright2, ytop1, ytop2);
                                        if (collision == true)
                                        {
                                                pociski[i].does_exist = false;
                                                opponents[j].does_exist = false;
                                                score += 1000;
                                                break;
                                        }
                                }
                        }
                }
        }
}
void SpawnNeutral(double& neutralspawntime, int max_neutral_cars, int wallright, int wallleft, Neutralcar* neutralcars)
{
        if (neutralspawntime > 1.9)
        {
                int los = rand() % 2;
                neutralspawntime = 0;
                if (los == 0)
                {
                        for (int i = 0; i < max_neutral_cars; i++)
                        {
                                if (neutralcars[i].does_exist == false)
                                {
                                        neutralcars[i].does_exist = true;
                                        neutralcars[i].y = 10;
                                        int sizeofboard = wallright - wallleft - 1.5 * (carWidth);
                                        int x = rand() % sizeofboard;
                                        x += wallleft + carWidth;
                                        neutralcars[i].x = x;;
                                        break;
                                }
                        }
                }
        }
}
void DrawandcalculateNeutralcar(int max_neutral_cars, Neutralcar* neutralcars, SDL_Surface* screen, SDL_Surface* car3, double delta, int yplayerspeed)
{
        for (int i = 0; i < max_neutral_cars; i++)
        {
                if (neutralcars[i].does_exist == true)
                {
                        DrawSurface(screen, car3, neutralcars[i].x, neutralcars[i].y);
                        double deltay = delta * (neutralcars[i].speed + yplayerspeed);
                        neutralcars[i].y += deltay;
                        if (neutralcars[i].y > SCREEN_HEIGHT || neutralcars[i].y < 0)
                        {
                                neutralcars[i].does_exist = false;
                        }
                }
        }
}
void checkpociskicollsion2(Pocisk* pociski, int max_numberof_bullets, Neutralcar* neutralcars, int& score, int max_neutral_cars, double& punishmenTimer)
{
        for (int i = 0; i < max_numberof_bullets; i++)
        {
                if (pociski[i].does_exist == true)
                {
                        for (int j = 0; j < max_neutral_cars; j++)
                        {
                                if (neutralcars[j].does_exist == true)
                                {
                                        int xleft1 = pociski[i].x - pociskwidth / 2;
                                        int xleft2 = neutralcars[j].x - carWidth / 2;
                                        int ybottom1 = pociski[i].y + pociskheight / 2;
                                        int ybottom2 = neutralcars[j].y + carHeight / 2;
                                        int xright1 = pociski[i].x + pociskwidth / 2;
                                        int xright2 = neutralcars[j].x + carWidth / 2;
                                        int ytop1 = pociski[i].y - pociskheight / 2;
                                        int ytop2 = neutralcars[j].y - carHeight / 2;
                                        bool collision = Iscollsion(xleft1, xleft2, ybottom1, ybottom2, xright1, xright2, ytop1, ytop2);
                                        if (collision == true)
                                        {
                                                pociski[i].does_exist = false;
                                                neutralcars[j].does_exist = false;
                                                punishmenTimer = -4, 5;
                                                break;
                                        }
                                }
                        }
                }
        }
}
void checkplayercollsionwithcars(Neutralcar* neutralcars, int max_neutral_cars, Opponent* opponents, int& score, int max_numberof_opponents, int car1y, int car1x, int& lives, double& punishmenTimer)
{
        for (int j = 0; j < max_numberof_opponents; j++)
        {
                if (opponents[j].does_exist == true)
                {
                        int xleft1 = car1x - carWidth / 2;
                        int xleft2 = opponents[j].x - carWidth / 2;
                        int ybottom1 = car1y + carHeight / 2;
                        int ybottom2 = opponents[j].y + carHeight / 2;
                        int xright1 = car1x + carWidth / 2;
                        int xright2 = opponents[j].x + carWidth / 2;
                        int ytop1 = car1y - carHeight / 2;
                        int ytop2 = opponents[j].y - carHeight / 2;
                        bool collision = Iscollsion(xleft1, xleft2, ybottom1, ybottom2, xright1, xright2, ytop1, ytop2);
                        if (collision == true)
                        {
                                opponents[j].does_exist = false;
                                lives--;
                                break;
                        }
                }
        }
        for (int j = 0; j < max_neutral_cars; j++)
        {
                if (neutralcars[j].does_exist == true)
                {
                        int xleft1 = car1x - carWidth / 2;
                        int xleft2 = neutralcars[j].x - carWidth / 2;
                        int ybottom1 = car1y + carHeight / 2;
                        int ybottom2 = neutralcars[j].y + carHeight / 2;
                        int xright1 = car1x + carWidth / 2;
                        int xright2 = neutralcars[j].x + carWidth / 2;
                        int ytop1 = car1y - carHeight / 2;
                        int ytop2 = neutralcars[j].y - carHeight / 2;
                        bool collision = Iscollsion(xleft1, xleft2, ybottom1, ybottom2, xright1, xright2, ytop1, ytop2);
                        if (collision == true)
                        {
                                neutralcars[j].does_exist = false;
                                lives--;
                                punishmenTimer = -4, 5;
                                break;
                        }
                }
        }
}
void GameOverr(int& quit, SDL_Surface* screen, Colors& colors, SDL_Surface* charset, char* text, SDL_Texture* scrtex, SDL_Renderer* renderer, int& GameOver, int& newGame, int score)
{
        DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, colors.czarny, colors.czarny);
        sprintf(text, "Game over");
        DrawString(screen, 300, 100, text, charset);
        sprintf(text, "Your Score");
        DrawString(screen, 300, 200, text, charset);
        sprintf(text, "%d", score);
        DrawString(screen, 330, 210, text, charset);
        sprintf(text, "Press Esc to leave");
        DrawString(screen, 300, 400, text, charset); \
                sprintf(text, "Press N to star a new game");
        DrawString(screen, 300, 410, text, charset);
        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);
        while (GameOver)
        {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                        switch (event.type) {
                        case SDL_QUIT:
                                quit = 1;
                                break;
                        case SDL_KEYDOWN:
                                switch (event.key.keysym.sym)
                                {
                                case SDLK_ESCAPE:
                                        quit = 1;
                                case SDLK_n:
                                        GameOver = 0;
                                }
                        }
                }
        }
        newGame = 1;
}
void Immortality(double& immortal, int& lives)
{
        if (immortal < 6.0)
        {
                lives = 3;
        }

}
void Drawpowerups(int max_powerupandhearth, Powerupp* powerups, SDL_Surface* screen, SDL_Surface* powerup, double delta, int yplayerspeed)
{
        for (int i = 0; i < max_powerupandhearth; i++)
        {
                if (powerups[i].does_exist == true)
                {
                        DrawSurface(screen, powerup, powerups[i].x, powerups[i].y);
                        double deltay = delta * (powerups[i].speed + yplayerspeed);
                        powerups[i].y += deltay;
                        if (powerups[i].y > SCREEN_HEIGHT || powerups[i].y < 0)
                        {
                                powerups[i].does_exist = false;
                        }
                }
        }
}
void SpawnPowerup(double& poweruprate, int max_powerupandhearth, int wallright, int wallleft, Powerupp* powerups)
{
        if (poweruprate > 10)
        {
                int los = rand() % 2;
                poweruprate = 0;
                if (los == 0)
                {
                        for (int i = 0; i < max_powerupandhearth; i++)
                        {
                                if (powerups[i].does_exist == false)
                                {
                                        powerups[i].does_exist = true;
                                        powerups[i].y = 10;
                                        int sizeofboard = wallright - wallleft - 40;
                                        int x = rand() % sizeofboard;
                                        x += wallleft + 40;
                                        powerups[i].x = x;;
                                        break;
                                }
                        }
                }
        }
}
void DrawHearths(int max_powerupandhearth, Hearths* heearth, SDL_Surface* screen, SDL_Surface* heart, double delta, int yplayerspeed)
{
        for (int i = 0; i < max_powerupandhearth; i++)
        {
                if (heearth[i].does_exist == true)
                {
                        DrawSurface(screen, heart, heearth[i].x, heearth[i].y);
                        double deltay = delta * (heearth[i].speed + yplayerspeed);
                        heearth[i].y += deltay;
                        if (heearth[i].y > SCREEN_HEIGHT || heearth[i].y < 0)
                        {
                                heearth[i].does_exist = false;
                        }
                }
        }
}
void SpawnHearths(double& heartrate, int max_powerupandhearth, int wallright, int wallleft, Hearths* heearth)
{
        if (heartrate > 15)
        {
                int los = rand() % 2;
                heartrate = 0;
                if (los == 0)
                {
                        for (int i = 0; i < max_powerupandhearth; i++)
                        {
                                if (heearth[i].does_exist == false)
                                {
                                        heearth[i].does_exist = true;
                                        heearth[i].y = 10;
                                        int sizeofboard = wallright - wallleft - 40;
                                        int x = rand() % sizeofboard;
                                        x += wallleft + 40;
                                        heearth[i].x = x;;
                                        break;
                                }
                        }
                }
        }
}
void checkplayercollsionwithpowerupandhearth(Hearths* heearth, Powerupp* powerups, int max_powerupandhearth, int car1y, int car1x, int& lives, int& powerupcount)
{
        for (int j = 0; j < max_powerupandhearth; j++)
        {
                if (powerups[j].does_exist == true)
                {
                        int xleft1 = car1x - powerupheartWidth / 2;
                        int xleft2 = powerups[j].x - powerupheartWidth / 2;
                        int ybottom1 = car1y + powerupheartHeight / 2;
                        int ybottom2 = powerups[j].y + powerupheartHeight / 2;
                        int xright1 = car1x + powerupheartWidth / 2;
                        int xright2 = powerups[j].x + powerupheartWidth / 2;
                        int ytop1 = car1y - powerupheartHeight / 2;
                        int ytop2 = powerups[j].y - powerupheartHeight / 2;
                        bool collision = Iscollsion(xleft1, xleft2, ybottom1, ybottom2, xright1, xright2, ytop1, ytop2);
                        if (collision == true)
                        {
                                powerups[j].does_exist = false;
                                powerupcount += 15;
                                break;
                        }
                }
        }
        for (int j = 0; j < max_powerupandhearth; j++)
        {
                if (heearth[j].does_exist == true)
                {
                        int xleft1 = car1x - powerupheartWidth / 2;
                        int xleft2 = heearth[j].x - powerupheartWidth / 2;
                        int ybottom1 = car1y + powerupheartHeight / 2;
                        int ybottom2 = heearth[j].y + powerupheartHeight / 2;
                        int xright1 = car1x + powerupheartWidth / 2;
                        int xright2 = heearth[j].x + powerupheartWidth / 2;
                        int ytop1 = car1y - powerupheartHeight / 2;
                        int ytop2 = heearth[j].y - powerupheartHeight / 2;
                        bool collision = Iscollsion(xleft1, xleft2, ybottom1, ybottom2, xright1, xright2, ytop1, ytop2);
                        if (collision == true)
                        {
                                heearth[j].does_exist = false;
                                lives++;
                                break;
                        }
                }
        }
}
void Roadside(double& delta, double& punishmenTimer, int car1x, int x1, int x2, int& car1yspped)
{
        if (car1x<x1 || car1x>x2)
        {
                punishmenTimer -= delta;
                car1yspped = 30;
        }
}
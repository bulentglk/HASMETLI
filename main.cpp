#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <locale.h>

// global değişkenler
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

bool inMainMenu = true;
bool running = true;
bool menuRunning = true;
bool isPaused = false;



int cameraX = 0;
int cameraY = 0;
int score = 0;
const int CAMERA_LEFT_BOUNDARY = SCREEN_WIDTH / 3;
const int CAMERA_RIGHT_BOUNDARY = SCREEN_WIDTH * 2 / 3;

const float PARALLAX_BASE = 0.1f;  // en arka temel renk degrade
const float PARALLAX_BUILDINGS = 0.2f;  // binalar
const float PARALLAX_PROPS = 0.4f;  // ön detay objeler
const float PARALLAX_MIDFOG = 0.7f;  // ortadaki sis
const float PARALLAX_FOG = 1.0f;  // en öndeki sis

int playerX = 0;
int playerY = 0;
const float PLAYER_SPEED = 400.0f;  // piksel/saniye
int health = 3;


SDL_Texture* backgroundBase = nullptr;
SDL_Texture* backgroundBuildings = nullptr;
SDL_Texture* backgroundProps = nullptr;
SDL_Texture* backgroundFog = nullptr;
SDL_Texture* backgroundMidFog = nullptr;
SDL_Texture* spikeTexture = nullptr;
SDL_Texture* deadTexture = nullptr;


SDL_Texture* playerTexture = nullptr;
SDL_Texture* tileTexture = nullptr;

const int TILE_SIZE = 64;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int velocityY = 1;           // düşme/yükselme hızı
bool isJumping = false;      // zıplama durumu kontrolü
const int GRAVITY = 1;       // yerçekimi kuvveti
const int JUMP_POWER = -25;  // zıplama kuvveti (negatif yukarı için)
const int MAX_FALL_SPEED = 3; // maksimum düşme hızı

int frame = 0;

const int mapWidth = 50;
const int mapHeight = 16;

int map[mapHeight][mapWidth] = {
	// 0-10 arası boş
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	// 10.satır platform
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	// 11.satır diken
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	// 12.satır platform
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	// 13.satır normal zemin
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	// 14.satır boş
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	// 15.satır boş
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int main(int argc, char* argv[])
{
	bool isPaused = false;

	int tileUnderPlayer = 0;
	float deltaTime = 0.0f;

	if (TTF_Init() != 0) {
		SDL_Log("TTF init error: %s", TTF_GetError());
		SDL_Quit();
		return 1;
	}

	TTF_Font* font = TTF_OpenFont("assets/fonts/pixel_font.ttf", 48);
	if (!font) {
		SDL_Log("Font load error: %s", TTF_GetError());
		SDL_Quit();
		return 1;
	}

	// sadece pause için daha büyük font
	TTF_Font* pauseFont = TTF_OpenFont("assets/fonts/pixel_font.ttf", 80);
	if (!pauseFont) {
		SDL_Log("Pause font load error: %s", TTF_GetError());
		SDL_Quit();
		return 1;
	}


	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("SDL init error: %s", SDL_GetError());
		return 1;
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_Log("SDL_image init error: %s", IMG_GetError());
		SDL_Quit();
		return 1;
	}

	window = SDL_CreateWindow("Babo Platform Demo",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (!window) {
		SDL_Log("SDL window error: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer) {
		SDL_Log("SDL renderer error: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// player
	SDL_Surface* playerSurface = IMG_Load("assets/player.png");
	if (!playerSurface) {
		SDL_Log("player image error: %s", IMG_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	SDL_SetColorKey(playerSurface, SDL_TRUE, SDL_MapRGB(playerSurface->format, 109, 170, 44));
	playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);

	// base
	SDL_Surface* baseSurface = IMG_Load("assets/backgrounds/Base_Color.png");
	if (!baseSurface) {
		SDL_Log("base background error: %s", IMG_GetError());
		return 1;
	}
	backgroundBase = SDL_CreateTextureFromSurface(renderer, baseSurface);
	SDL_FreeSurface(baseSurface);

	// buildings
	SDL_Surface* buildingsSurface = IMG_Load("assets/backgrounds/Buildings.png");
	if (!buildingsSurface) {
		SDL_Log("buildings background error: %s", IMG_GetError());
		return 1;
	}
	backgroundBuildings = SDL_CreateTextureFromSurface(renderer, buildingsSurface);
	SDL_FreeSurface(buildingsSurface);

	// props
	SDL_Surface* propsSurface = IMG_Load("assets/backgrounds/Background_Props.png");
	if (!propsSurface) {
		SDL_Log("props background error: %s", IMG_GetError());
		return 1;
	}
	backgroundProps = SDL_CreateTextureFromSurface(renderer, propsSurface);
	SDL_FreeSurface(propsSurface);

	// mid fog
	SDL_Surface* midfogSurface = IMG_Load("assets/backgrounds/Mid_Fog.png");
	if (!midfogSurface) {
		SDL_Log("mid fog error: %s", IMG_GetError());
		return 1;
	}
	backgroundMidFog = SDL_CreateTextureFromSurface(renderer, midfogSurface);
	SDL_FreeSurface(midfogSurface);

	// frontal fog
	SDL_Surface* fogSurface = IMG_Load("assets/backgrounds/Frontal_Fog.png");
	if (!fogSurface) {
		SDL_Log("frontal fog error: %s", IMG_GetError());
		return 1;
	}
	backgroundFog = SDL_CreateTextureFromSurface(renderer, fogSurface);
	SDL_FreeSurface(fogSurface);

	// tile
	SDL_Surface* tileSurface = IMG_Load("assets/stone.png");
	if (!tileSurface) {
		SDL_Log("tile image error: %s", IMG_GetError());
		SDL_DestroyTexture(playerTexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
	SDL_FreeSurface(tileSurface);

	SDL_Surface* spikeSurface = IMG_Load("assets/spike.png");
	if (!spikeSurface) {
		SDL_Log("spike image error: %s", IMG_GetError());
		SDL_Quit();
		return 1;
	}
	spikeTexture = SDL_CreateTextureFromSurface(renderer, spikeSurface);
	SDL_FreeSurface(spikeSurface);

	//game over bebi

	SDL_Surface* deadSurface = IMG_Load("assets/oldun.png");
	if (!deadSurface) {
		SDL_Log("öldün image error: %s", IMG_GetError());
		SDL_Quit();
		return 1;
	}
	deadTexture = SDL_CreateTextureFromSurface(renderer, deadSurface);
	SDL_FreeSurface(deadSurface);

	// MENÜ DÖNGÜSÜ
	bool menuRunning = true;
	while (menuRunning)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				menuRunning = false;
				running = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					menuRunning = false;   // menüyü bitir
					inMainMenu = false;    // menü durumunu kapat
					running = true;        // oyunu başlat

					// karakter konumunu sıfırla
					playerX = 200;
					playerY = 100;
					velocityY = 0;
					isJumping = false;
				}

			}
		}
	
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Color white = { 255, 255, 255 };

		// Başlık
		SDL_Surface* titleSurface = TTF_RenderUTF8_Solid(font, "HASMETLI", white);
		if (!titleSurface)
		{
			SDL_Log("titleSurface render hatası: %s", TTF_GetError());
		}
		SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
		SDL_Rect titleRect = {
			SCREEN_WIDTH / 2 - titleSurface->w / 2,
			200,
			titleSurface->w,
			titleSurface->h
		};
		SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
		SDL_FreeSurface(titleSurface);
		SDL_DestroyTexture(titleTexture);

		// Alt yazı
		SDL_Surface* startSurface = TTF_RenderUTF8_Solid(font, "BASLAMAK ICIN BOSLUK TUSUNA BAS", white);
		if (!startSurface)
		{
			SDL_Log("startSurface render hatası: %s", TTF_GetError());
		}
		SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
		SDL_Rect startRect = {
			SCREEN_WIDTH / 2 - startSurface->w / 2,
			400,
			startSurface->w,
			startSurface->h
		};
		SDL_RenderCopy(renderer, startTexture, NULL, &startRect);
		SDL_FreeSurface(startSurface);
		SDL_DestroyTexture(startTexture);

		SDL_RenderPresent(renderer);
	}

	bool running = true;

	SDL_Event e;

	Uint32 lastTime = SDL_GetTicks();

	while (running)
	{
		Uint32 currentTime = SDL_GetTicks();
		deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		// event kontrol
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					isPaused = !isPaused;  // ESC basınca pause aç/kapat
					break;
				case SDLK_UP:
					if (!isPaused && !isJumping) {
						velocityY = JUMP_POWER;
						isJumping = true;
						frame = 3;
					}
					break;
				case SDLK_DOWN:
					if (!isPaused) {
						frame = 0;
					}
					break;
				}
			}
		}
	

		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		if (!isPaused && keystate[SDL_SCANCODE_LEFT]) {
			int nextX = playerX - static_cast<int>(PLAYER_SPEED * deltaTime);
			if (nextX < 0) nextX = 0;
			int leftTileX = nextX / TILE_SIZE;
			int topTileY = playerY / TILE_SIZE;
			int bottomTileY = (playerY + 31) / TILE_SIZE;
			if (map[topTileY][leftTileX] == 0 && map[bottomTileY][leftTileX] == 0) {
				playerX = nextX;
			}
			frame = 1;
		}

		if (!isPaused && keystate[SDL_SCANCODE_RIGHT]) {
			int nextX = playerX + static_cast<int>(PLAYER_SPEED * deltaTime) + 31;
			int maxX = (mapWidth * TILE_SIZE) - SCREEN_WIDTH;
			if (nextX > maxX) nextX = maxX;
			int rightTileX = nextX / TILE_SIZE;
			int topTileY = playerY / TILE_SIZE;
			int bottomTileY = (playerY + 31) / TILE_SIZE;
			if (map[topTileY][rightTileX] == 0 && map[bottomTileY][rightTileX] == 0) {
				playerX = nextX - 31;
			}
			frame = 2;
			score += 1;
		}


		// yerçekimi
		velocityY += GRAVITY;
		if (velocityY > MAX_FALL_SPEED)
			velocityY = MAX_FALL_SPEED;

		int nextY = playerY + velocityY;

		int leftTileX = playerX / TILE_SIZE;
		int rightTileX = (playerX + 31) / TILE_SIZE;
		int bottomTileY = (nextY + 31) / TILE_SIZE;

		if (map[bottomTileY][leftTileX] == 1 || map[bottomTileY][rightTileX] == 1) {
			velocityY = 0;
			isJumping = false;
			playerY = bottomTileY * TILE_SIZE - 32;
		}
		else {
			playerY = nextY;
		}

		// öldün kontrolü - sadece tek yerde
		tileUnderPlayer = map[(playerY + 32) / TILE_SIZE][(playerX + 16) / TILE_SIZE];
		if (tileUnderPlayer == 2) {
			SDL_Log("ÖLDÜN");
			health--;
			if (health <= 0) {
				SDL_Log("GAME OVER");
				score = 0;  // skoru sıfırla
				running = false;
			}
			else {
				// ekrana öldün yazısı göster
				SDL_Rect deadRect = {
					static_cast<int>(SCREEN_WIDTH / 2 - 600 / 2),
					static_cast<int>(SCREEN_HEIGHT / 2 - 300 / 2),
					600, 300
				};
				SDL_RenderCopy(renderer, deadTexture, NULL, &deadRect);
				SDL_RenderPresent(renderer);
				SDL_Delay(1000);
				playerX = 200;
				playerY = 100;
			}
		}

		int topTileY = playerY / TILE_SIZE;
		if (velocityY < 0) {
			int nextTopY = (playerY + velocityY) / TILE_SIZE;
			if (map[nextTopY][leftTileX] == 1 || map[nextTopY][rightTileX] == 1) {
				velocityY = 0;
				playerY = (nextTopY + 1) * TILE_SIZE;
			}
		}

		// kamera
		if (playerX - cameraX < CAMERA_LEFT_BOUNDARY) {
			cameraX = playerX - CAMERA_LEFT_BOUNDARY;
		}
		else if (playerX - cameraX > CAMERA_RIGHT_BOUNDARY) {
			cameraX = playerX - CAMERA_RIGHT_BOUNDARY;
		}
		cameraY = playerY + 16 - SCREEN_HEIGHT / 2;

		if (cameraX < 0) cameraX = 0;
		if (cameraY < 0) cameraY = 0;
		if (cameraX > mapWidth * TILE_SIZE - SCREEN_WIDTH) cameraX = mapWidth * TILE_SIZE - SCREEN_WIDTH;
		if (cameraY > mapHeight * TILE_SIZE - SCREEN_HEIGHT) cameraY = mapHeight * TILE_SIZE - SCREEN_HEIGHT;

		// ekranı temizle
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// background
		SDL_Rect baseRect = { -cameraX * 0.1f, -cameraY * 0.1f, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(renderer, backgroundBase, NULL, &baseRect);
		SDL_Rect buildingsRect = { -cameraX * 0.3f, -cameraY * 0.3f, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(renderer, backgroundBuildings, NULL, &buildingsRect);
		SDL_Rect propsRect = { -cameraX * 0.5f, -cameraY * 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(renderer, backgroundProps, NULL, &propsRect);
		SDL_Rect midfogRect = { -cameraX * 0.8f, -cameraY * 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(renderer, backgroundMidFog, NULL, &midfogRect);
		SDL_Rect fogRect = { -cameraX * 1.0f, -cameraY * 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(renderer, backgroundFog, NULL, &fogRect);

		// harita
		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				SDL_Rect destRect = {
					j * TILE_SIZE - cameraX,
					i * TILE_SIZE - cameraY,
					TILE_SIZE, TILE_SIZE
				};
				if (map[i][j] == 1) SDL_RenderCopy(renderer, tileTexture, NULL, &destRect);
				if (map[i][j] == 2) SDL_RenderCopy(renderer, spikeTexture, NULL, &destRect);
			}
		}

		// karakter
		SDL_Rect srcRect = { frame * 16, 0, 16, 16 };
		SDL_Rect destRect = { playerX - cameraX, playerY - cameraY, 64, 64 };
		SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);

		// skor yaz
		SDL_Color color = { 255,255,255 };
		char scoreText[64];
		sprintf_s(scoreText, "SKOR: %d", score);
		SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, color);
		SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
		SDL_Rect scoreRect = { 20, 20, scoreSurface->w, scoreSurface->h };
		SDL_FreeSurface(scoreSurface);
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
		SDL_DestroyTexture(scoreTexture);

		// can yaz
		char healthText[64];
		sprintf_s(healthText, "CAN: %d", health);
		SDL_Surface* healthSurface = TTF_RenderText_Solid(font, healthText, color);
		SDL_Texture* healthTexture = SDL_CreateTextureFromSurface(renderer, healthSurface);
		SDL_Rect healthRect = { 20, 60, healthSurface->w, healthSurface->h };
		SDL_FreeSurface(healthSurface);
		SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);
		SDL_DestroyTexture(healthTexture);

		//durdurma
		if (isPaused) {
			SDL_Color pauseColor = { 255, 255, 255 };
			SDL_Surface* pauseSurface = TTF_RenderUTF8_Solid(pauseFont, "OYUN DURDURULDU", pauseColor);
			SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
			SDL_Rect pauseRect = {
				SCREEN_WIDTH / 2 - pauseSurface->w / 2,
				SCREEN_HEIGHT / 2 - pauseSurface->h / 2,
				pauseSurface->w,
				pauseSurface->h
			};
			SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);
			SDL_FreeSurface(pauseSurface);
			SDL_DestroyTexture(pauseTexture);
		}


		SDL_RenderPresent(renderer);

	}   // <<< bu while(running) döngüsünün KAPANIŞ parantezi

	// cleanup işlemleri
	SDL_DestroyTexture(tileTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(backgroundBase);
	SDL_DestroyTexture(backgroundBuildings);
	SDL_DestroyTexture(backgroundProps);
	SDL_DestroyTexture(backgroundMidFog);
	SDL_DestroyTexture(backgroundFog);
	SDL_DestroyTexture(deadTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;

}
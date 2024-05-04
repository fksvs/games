#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 800

#define PLAYER_SIZE 20
#define SHOOT_SIZE 10
#define ENEMY_SIZE 15

#define PLAYER_SPEED 5
#define SHOOT_SPEED 6
#define ENEMY_SPEED 4

#define MAX_SHOOTS 50
#define MAX_ENEMIES 50

struct player_s {
	Rectangle block;
	Vector2 direction;
};

struct shoot_s {
	Rectangle block;
	bool active;
};

struct enemy_s {
	Rectangle block;
	bool active;
};

struct player_s player;
struct shoot_s shoot[MAX_SHOOTS];
struct enemy_s enemy[MAX_ENEMIES];

int main()
{
	/* initialize game */
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");

	/* player struct */
	player.block.width = PLAYER_SIZE;
	player.block.height = PLAYER_SIZE;
	player.block.x = SCREEN_WIDTH / 2.0f;
	player.block.y = SCREEN_HEIGHT / 2.0f + 200;

	/* enemy struct */
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemy[i].block.width = ENEMY_SIZE;
		enemy[i].block.height = ENEMY_SIZE;
		enemy[i].block.x = GetRandomValue(0, SCREEN_WIDTH - ENEMY_SIZE);
		enemy[i].block.y = GetRandomValue(0, -2000);
		enemy[i].active = true;
	}

	/* shoot struct */
	for (int i = 0; i < MAX_SHOOTS; i++) {
		shoot[i].block.width = SHOOT_SIZE / 2.0f;
		shoot[i].block.height = SHOOT_SIZE;
		shoot[i].block.x = player.block.x + player.block.width / 4;
		shoot[i].block.y = player.block.y; 
		shoot[i].active = false;
	}

	SetTargetFPS(60);

	int frame = 0;
	while (!WindowShouldClose()) {
		/* update game */

		/* get player movement directions */
		if (IsKeyDown(KEY_RIGHT)) {
			player.direction = (Vector2){ 1, 0 };
		} if (IsKeyDown(KEY_DOWN)) {
			player.direction = (Vector2){ 0, 1 };
		} if (IsKeyDown(KEY_LEFT)) {
			player.direction = (Vector2){ -1, 0 };
		} if (IsKeyDown(KEY_UP)) {
			player.direction = (Vector2){ 0, -1 };
		}

		/* update player position */
		player.block.x += player.direction.x * PLAYER_SPEED;
		player.block.y += player.direction.y * PLAYER_SPEED;
		player.direction = (Vector2){ 0, 0 };

		if (IsKeyDown(KEY_SPACE) && frame % 10 == 0) {
			for (int i = 0; i < MAX_SHOOTS; i++) {
				if (!shoot[i].active) {
					shoot[i].active = true;
					shoot[i].block.x = player.block.x + player.block.width / 4;
					shoot[i].block.y = player.block.y;
					break;
				}
			}
		}

		/* update enemy position */
		for (int i = 0; i < MAX_ENEMIES; i++) {
			enemy[i].block.y += ENEMY_SPEED;
		}

		/* check player - enemy collision */
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (CheckCollisionRecs(player.block, enemy[i].block)) {
				CloseWindow();
				return 0;
			}
		}

		/* reset enemy position if need */
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy[i].block.y > SCREEN_HEIGHT) {
				enemy[i].block.x = GetRandomValue(0, SCREEN_WIDTH - ENEMY_SIZE);
				enemy[i].block.y = GetRandomValue(0, -2000);
			}
		}

		/* update shoot position */
		for (int i = 0; i < MAX_SHOOTS; i++) {
			if (shoot[i].active) {
				shoot[i].block.y -= SHOOT_SPEED;
				if (shoot[i].block.y < 0) {
					shoot[i].active = false;
				}
			}
		}

		/* check enemy hit */
		for (int i = 0; i < MAX_SHOOTS; i++) {
			for (int j = 0; j < MAX_ENEMIES; j++) {
				if (shoot[i].active) {
					if (CheckCollisionRecs(shoot[i].block, enemy[j].block)) {
						enemy[j].block.x = GetRandomValue(0, SCREEN_WIDTH - ENEMY_SIZE);
						enemy[j].block.y = GetRandomValue(0, -2000);
					}
				}
			}
		}

		BeginDrawing();
		ClearBackground(BLACK);

		DrawRectangleRec(player.block, WHITE);

		for (int i = 0; i < MAX_SHOOTS; i++) {
			if (shoot[i].active) {
				DrawRectangleRec(shoot[i].block, RED);
			}
		}

		for (int i = 0; i < MAX_ENEMIES; i++) {
			DrawRectangleRec(enemy[i].block, GRAY);
		}

		EndDrawing();

		frame++;
	}

	CloseWindow();
	return 0;
}

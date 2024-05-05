#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 800

#define PLAYER_SIZE 20
#define BULLET_SIZE 10
#define ENEMY_SIZE 15
#define STAR_SIZE 2

#define PLAYER_SPEED 5
#define BULLET_SPEED 6
#define ENEMY_SPEED 4
#define STAR_SPEED 15

#define MAX_BULLETS 50
#define MAX_ENEMIES 50
#define MAX_STARS 50

struct player_s {
	Rectangle block;
	Vector2 direction;
	bool shoot;
};

struct bullet_s {
	Rectangle block;
	bool active;
};

struct enemy_s {
	Rectangle block;
	bool active;
};

struct player_s player;
struct bullet_s bullet[MAX_BULLETS];
struct enemy_s enemy[MAX_ENEMIES];
Rectangle star[MAX_STARS];

int points = 0;
int frames = 0;
int pause = 0;
int game_over = 0;
int game_win = 0;

void init_game()
{
	points = 0;
	frames = 0;
	pause = 0;
	game_over = 0;
	game_win = 0;

	player.block = (Rectangle){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200,
				PLAYER_SIZE, PLAYER_SIZE};
	player.direction = (Vector2){ 0, 0 };
	player.shoot = false;

	for (int i = 0; i < MAX_BULLETS; i++) {
		bullet[i].block = (Rectangle){ player.block.x + player.block.width / 4,
					player.block.y, BULLET_SIZE / 2, BULLET_SIZE };
		bullet[i].active = false;
	}

	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemy[i].block = (Rectangle){ GetRandomValue(0, SCREEN_WIDTH - ENEMY_SIZE),
					GetRandomValue(0, -2000), ENEMY_SIZE, ENEMY_SIZE };
		enemy[i].active = true;
	}

	for (int i = 0; i < MAX_STARS; i++) {
		star[i] = (Rectangle){ GetRandomValue(0, SCREEN_WIDTH - STAR_SIZE),
			GetRandomValue(SCREEN_HEIGHT, -2000), STAR_SIZE, STAR_SIZE };
	}
}

void update_player()
{
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		player.direction = (Vector2){ 1, 0 };
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
		player.direction = (Vector2){ 0, 1 };
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
		player.direction = (Vector2){ -1, 0 };
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
		player.direction = (Vector2){ 0, -1 };
	if (IsKeyDown(KEY_SPACE))
		player.shoot = true;

	player.block.x += player.direction.x * PLAYER_SPEED;
	player.block.y += player.direction.y * PLAYER_SPEED;
	player.direction = (Vector2){ 0, 0 };

	if (player.block.x < 0)
		player.block.x = 0;
	else if (player.block.x + PLAYER_SIZE > SCREEN_WIDTH)
		player.block.x = SCREEN_WIDTH - PLAYER_SIZE;
	else if (player.block.y < 0)
		player.block.y = 0;
	else if (player.block.y + PLAYER_SIZE > SCREEN_HEIGHT)
		player.block.y = SCREEN_HEIGHT - PLAYER_SIZE;

	if (player.shoot && frames % 10 == 0) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (!bullet[i].active) {
				bullet[i].block.x = player.block.x + player.block.width / 4;
				bullet[i].block.y = player.block.y;
				bullet[i].active = true;
				break;
			}
		}

		player.shoot = false;
	}
}

void random_enemy_pos(struct enemy_s *enemy)
{
	enemy->block.x = GetRandomValue(0, SCREEN_WIDTH - ENEMY_SIZE);
	enemy->block.y = GetRandomValue(0, -2000);
}

void update_enemy()
{
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemy[i].block.y += ENEMY_SPEED;

		if (enemy[i].block.y > SCREEN_HEIGHT)
			random_enemy_pos(&enemy[i]);
	}
}

void update_bullet()
{
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullet[i].active) {
			bullet[i].block.y -= BULLET_SPEED;
			if (bullet[i].block.y < 0)
				bullet[i].active = false;
		}
	}
}

void update_star()
{
	for (int i = 0; i < MAX_STARS; i++) {
		star[i].y += STAR_SPEED;

		if (star[i].y > SCREEN_HEIGHT)
			star[i] = (Rectangle){ GetRandomValue(0, SCREEN_WIDTH - STAR_SIZE),
				GetRandomValue(0, -2000), STAR_SIZE, STAR_SIZE };
	}
}

void check_collision()
{
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (CheckCollisionRecs(player.block, enemy[i].block))
			game_over = 1;
	}
}

void check_hit()
{
	for (int i = 0; i < MAX_BULLETS; i++) {
		for (int j = 0; j < MAX_ENEMIES; j++) {
			if (bullet[i].active && CheckCollisionRecs(bullet[i].block, enemy[j].block)) {
					random_enemy_pos(&enemy[j]);
					bullet[i].active = false;
					points++;
			}
		}
	}

	if (points == 500) game_win = 1; 
}

void draw_frame()
{
	if (!game_over && !game_win) {
		for (int i = 0; i < MAX_STARS; i++)
			DrawRectangleRec(star[i], WHITE);

		DrawRectangleRec(player.block, WHITE);

		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullet[i].active)
				DrawRectangleRec(bullet[i].block, RED);
		}

		for (int i = 0; i < MAX_ENEMIES; i++)
			DrawRectangleRec(enemy[i].block, GRAY);

		if (!pause) DrawText(TextFormat("Points : %d", points), 10, 10, 34, YELLOW);
		else DrawText("Paused", 10, 10, 34, YELLOW);

		if (frames == 1000000)	frames = 0;
		frames++;
	} else if (game_win) {
		DrawText("You Win!", (SCREEN_WIDTH - MeasureText("You Win!", 50)) / 2.0f, SCREEN_HEIGHT / 2.0f - 50, 50, YELLOW);
	} else if (game_over) {
		DrawText("Game Over", (SCREEN_WIDTH - MeasureText("Game Over", 50)) / 2.0f, SCREEN_HEIGHT / 2.0f - 50, 50, RED);
	}

	if (game_over || game_win) {
		DrawText(TextFormat("Points : %d", points), (SCREEN_WIDTH - MeasureText(TextFormat("Points : %d", points), 40)) / 2.0f, SCREEN_HEIGHT / 2.0f, 40, DARKGRAY);
		DrawText("Press Enter to play again", (SCREEN_WIDTH - MeasureText("Press Enter to play again", 30)) / 2.0f, SCREEN_HEIGHT / 2.0f + 50, 30, DARKGRAY);
	}
}

void update_frame()
{
	if (!game_over && !game_win) {
		if (IsKeyPressed(KEY_P)) pause = !pause;

		if (!pause) {
			update_player();
			update_enemy();
			update_bullet();
			update_star();

			check_collision();
			check_hit();
		}
	} else if (IsKeyPressed(KEY_ENTER)){
		init_game();
	}
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");

	init_game();

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		update_frame();

		BeginDrawing();
		ClearBackground(BLACK);

		draw_frame();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

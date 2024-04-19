#include "raylib.h"
#include <stdio.h>

#define SCREEN_SIZE 800
#define BLOCK_SIZE 40
#define MAX_SNAKE_SIZE (((SCREEN_SIZE) / (BLOCK_SIZE)) * ((SCREEN_SIZE) / (BLOCK_SIZE)))

struct snake_s {
	Rectangle block[MAX_SNAKE_SIZE];
	Vector2 direction[MAX_SNAKE_SIZE];
	int snake_size;
};

struct snake_s snake;
Rectangle food;

int frames = 0;
int need_food = 0;
int pause = 0;
int game_over = 0;
int game_win = 0;
int move = 1;

void init_game()
{
	for (int i = 0; i < MAX_SNAKE_SIZE; i++) {
		snake.block[i].width = BLOCK_SIZE;
		snake.block[i].height = BLOCK_SIZE;
		snake.block[i].x = SCREEN_SIZE / 2.0f;
		snake.block[i].y = SCREEN_SIZE / 2.0f;
		snake.direction[i] = (Vector2){ 0, 0 };
	}
	snake.direction[0] = (Vector2){ 1, 0 };
	snake.snake_size = 1;

	food.width = BLOCK_SIZE;
	food.height = BLOCK_SIZE;
	food.x = GetRandomValue(0, SCREEN_SIZE - BLOCK_SIZE);
	food.y = GetRandomValue(0, SCREEN_SIZE - BLOCK_SIZE);

	food.x -= (int)food.x % BLOCK_SIZE;
	food.y -= (int)food.y % BLOCK_SIZE;
}

void update_snake()
{
	Vector2 direction_old[snake.snake_size];

	if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && snake.direction[0].x != -1 && move) {
		snake.direction[0] = (Vector2){ 1, 0 };
		move = 0;
	} else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && snake.direction[0].y != -1 && move) {
		snake.direction[0] = (Vector2){ 0, 1 };
		move = 0;
	} else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && snake.direction[0].x != 1 && move) {
		snake.direction[0] = (Vector2){ -1, 0 };
		move = 0;
	} else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && snake.direction[0].y != 1 && move) {
		snake.direction[0] = (Vector2){ 0, -1 };
		move = 0;
	}

	if (frames % 9 == 0) {
		for (int i = 1; i < snake.snake_size; i++) {
			direction_old[i] = snake.direction[i - 1];
		}

		for (int i = 0; i < snake.snake_size; i++) {
			snake.block[i].x += snake.direction[i].x * BLOCK_SIZE;
			snake.block[i].y += snake.direction[i].y * BLOCK_SIZE;
		}

		for (int i = 1; i < snake.snake_size; i++) {
			snake.direction[i] = direction_old[i];
		}

		move = 1;
	}
}

void check_collision()
{
	if (snake.block[0].x > SCREEN_SIZE - BLOCK_SIZE || snake.block[0].x < 0) {
		game_over = 1;
	}
	if (snake.block[0].y > SCREEN_SIZE - BLOCK_SIZE || snake.block[0].y < 0) {
		game_over = 1;
	}

	for (int i = 1; i < snake.snake_size; i++) {
		if (CheckCollisionRecs(snake.block[0], snake.block[i])) {
			game_over = 1;
		}
	}
}

void generate_food() {
	if (CheckCollisionRecs(snake.block[0], food)) {
		snake.block[snake.snake_size].x = snake.block[snake.snake_size - 1].x - snake.direction[snake.snake_size - 1].x * BLOCK_SIZE;
		snake.block[snake.snake_size].y = snake.block[snake.snake_size - 1].y - snake.direction[snake.snake_size - 1].y * BLOCK_SIZE;
		snake.direction[snake.snake_size] = snake.direction[snake.snake_size -1];

		snake.snake_size++;
		if (snake.snake_size == MAX_SNAKE_SIZE) {
			game_win = 1;
		}
		need_food = 1;
	}

	if (need_food && !game_win) {
		food.x = GetRandomValue(0, SCREEN_SIZE - BLOCK_SIZE);
		food.y = GetRandomValue(0, SCREEN_SIZE - BLOCK_SIZE);

		food.x -= (int)food.x % BLOCK_SIZE;
		food.y -= (int)food.y % BLOCK_SIZE;

		for (int i = 0; i < snake.snake_size; i++) {
			if (food.x == snake.block[i].x && food.y == snake.block[i].y) {
				food.x = GetRandomValue(0, SCREEN_SIZE - BLOCK_SIZE);
				food.y = GetRandomValue(0, SCREEN_SIZE - BLOCK_SIZE);

				food.x -= (int)food.x % BLOCK_SIZE;
				food.y -= (int)food.y % BLOCK_SIZE;

				i = 0;
			}
		}
	}

	need_food = 0;
}

void draw_frame()
{
	if (!game_over && !game_win) {
		for (int i = 0; i < SCREEN_SIZE; i += BLOCK_SIZE) {
			DrawLine(i, 0, i, SCREEN_SIZE, LIGHTGRAY);
			DrawLine(0, i, SCREEN_SIZE, i, LIGHTGRAY);
		}

		DrawRectangleRec(food, RED);

		DrawRectangleRec(snake.block[0], BROWN);
		for (int i = 1; i < snake.snake_size; i++) {
			DrawRectangleRec(snake.block[i], GREEN);
		}

		if (!pause) {
			DrawText(TextFormat("Points : %d", snake.snake_size), 10, 10, 34, DARKGRAY);
		} else {
			DrawText("Pause", 10, 10, 34, GRAY);
		}

		if (frames == 1000) {
			frames = 0;
		}
		frames++;
	} else if (game_win) {
		DrawText("You Win!", (SCREEN_SIZE - MeasureText("You Win!", 50)) / 2.0f, SCREEN_SIZE / 2.0f - 50, 50, GREEN);
	} else if (game_over) {
		DrawText("Game Over", (SCREEN_SIZE - MeasureText("Game Over", 50)) / 2.0f, SCREEN_SIZE / 2.0f - 50, 50, RED);
	}

	if (game_over || game_win) {
		DrawText(TextFormat("Points : %d", snake.snake_size), (SCREEN_SIZE - MeasureText(TextFormat("Points : %d", snake.snake_size), 40)) / 2.0f,
				SCREEN_SIZE / 2.0f, 40, DARKGRAY);
		DrawText("Press Enter to play again", (SCREEN_SIZE - MeasureText("Press Enter to play again", 30)) / 2.0f, SCREEN_SIZE / 2.0f + 50, 30, DARKGRAY);
	}
}

void reset_game()
{
	if (IsKeyPressed(KEY_ENTER)) {
		game_over = 0;
		game_win = 0;
		snake.block[0].x = SCREEN_SIZE / 2.0f;
		snake.block[0].y = SCREEN_SIZE / 2.0f;
		snake.direction[0] = (Vector2){ 1, 0 };
		snake.snake_size = 1;
	}
}

void update_frame()
{
	if (!game_over && !game_win) {
		if (IsKeyPressed(KEY_P)) {
			pause = !pause;
		}

		if (!pause) {
			update_snake();
			check_collision();
			generate_food();
		}
	} else {
		if (IsKeyPressed(KEY_ENTER)) {
			reset_game();
		}
	}
}

int main(int argc, char *argv[])
{
	InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Snake");

	init_game();

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		update_frame();

		BeginDrawing();
		ClearBackground(WHITE);

		draw_frame();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

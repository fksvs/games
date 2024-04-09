#include "raylib.h"

#define MAX_BLOCKS 500

int main(int argc, char *argv[])
{
	const int screen_width = 1000;
	const int screen_height = 800;

	InitWindow(screen_width, screen_height, "Flappy Bird");

	const float player_size = 40;
	const float block_width = 100;
	const float forward_speed = 2.5;
	const float jump_speed = 8;
	const float fall_speed = 4;

	Rectangle terrain_top = { 0, 0, block_width * MAX_BLOCKS, 20 };
	Rectangle terrain_bottom = { 0, 760, block_width * MAX_BLOCKS, 40 };
	
	Rectangle blocks_top[MAX_BLOCKS] = { 0 };
	Rectangle blocks_bottom[MAX_BLOCKS] = { 0 };

	Rectangle player = { screen_width / 2.0f - 300, screen_height / 2.0f,
				player_size, player_size};

	Camera2D camera = { 0 };
	camera.target = (Vector2){ player.x + player_size / 2.0f,
					player.y + player_size / 2.0f};
	camera.offset = (Vector2){ screen_width / 2.0f - 300, screen_height / 2.0f };
	camera.rotation = 0;
	camera.zoom = 1.0f;

	float spacing = 0;
	for (int i = 0; i < MAX_BLOCKS; i++) {
		blocks_top[i].width = block_width;
		blocks_top[i].height = GetRandomValue(200, screen_height / 2.0f);
		blocks_top[i].x = screen_width / 2.0f + spacing;
		blocks_top[i].y = 20;

		blocks_bottom[i].width = block_width;
		blocks_bottom[i].height = screen_height - blocks_top[i].height - 220;
		blocks_bottom[i].x = blocks_top[i].x;
		blocks_bottom[i].y = screen_height - blocks_bottom[i].height - 40;

		spacing += GetRandomValue(250, 350);
	}

	SetTargetFPS(60);

	int points = 0;
	while (!WindowShouldClose()) {
		if (IsKeyDown(KEY_SPACE)) {
			player.y -= jump_speed;
		} else {
			player.y += fall_speed;
		}

		player.x += forward_speed;
		camera.target = (Vector2){ player.x + forward_speed,
					screen_height / 2.0f };


		if (CheckCollisionRecs(terrain_top, player) || 
			CheckCollisionRecs(terrain_bottom, player)) {
			CloseWindow();
			return 0;
		}

		for (int i = 0; i < MAX_BLOCKS; i++) {
			if (CheckCollisionRecs(blocks_top[i], player) ||
				CheckCollisionRecs(blocks_bottom[i], player)) {
				CloseWindow();
				return 0;
			}

			if (player.x > (blocks_top[i].x + block_width)) {
				points = i + 1;
			}
		}

		BeginDrawing();
		ClearBackground(SKYBLUE);

		BeginMode2D(camera);

		DrawRectangleRec(terrain_top, GREEN);
		DrawRectangleRec(terrain_bottom, GREEN);

		for (int i = 0; i < MAX_BLOCKS; i++) {
			DrawRectangleRec(blocks_top[i], BROWN);
			DrawRectangleRec(blocks_bottom[i], BROWN);
		}

		DrawRectangleRec(player, YELLOW);

		EndMode2D();
	
		DrawText(TextFormat("Points : %d", points), 10, 30, 35, WHITE);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

CC = gcc
RAYLIB_FLAGS = -lraylib -lGL -lm -ldl -lrt
CFLAGS = -Wall -Wextra -Werror -Wpedantic
GAMES = snake flappy_bird space_shooter

all : $(GAMES)

snake : src/snake.c
	$(CC) src/snake.c -o snake $(CFLAGS) $(RAYLIB_FLAGS)

flappy_bird : src/flappy_bird.c
	$(CC) src/flappy_bird.c -o flappy_bird $(CFLAGS) $(RAYLIB_FLAGS)

space_shooter : src/space_shooter.c
	$(CC) src/space_shooter.c -o space_shooter $(CFLAGS) $(RAYLIB_FLAGS)

.PHONY : clean
clean : 
	rm -f $(GAMES)

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <math.h>

#define GRID_SIZE 30
void InitializeBlocks();
void MakeNewBlock();

struct Vector2
{
	int x;
	int y;
}typedef Vector2;


struct Block
{
	Vector2 position;
	Vector2 prevPositon;
	Vector2 points[16];
	int rotated;
	int prevRotated;
}typedef Block;

enum GAME_STATE 
{
	GAME_PLAYING,
	GAME_CLEARING,
	GAME_FAILED
}typedef GAME_STATE;

GAME_STATE state;


int grid[GRID_SIZE][GRID_SIZE] = { { 0 } };

Block* blockTypes = NULL;
Block* block = NULL;


int readyToCreateNew = 0;
unsigned long long score = 0;

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(CP_UTF8);
	InitializeBlocks();
	

	state = GAME_PLAYING;

	srand(time(NULL));
	readyToCreateNew = 1;
	block = malloc(sizeof(Block));
	
	int random = rand() % 7;
	for (int i = 0; i < 16; ++i)
	{
		block->points[i] = blockTypes[random].points[i];
	}
	block->position = (struct Vector2){ GRID_SIZE / 2, 0 };
	block->prevPositon = (struct Vector2){ block->position.x, block->position.y };
	block->rotated = 0;
	block->prevRotated = 0;

	int GAME_SHOULD_RUN = 1;
	while (GAME_SHOULD_RUN)
	{
		char c = -1;
		system("cls");
		int clearBottomRow = 0;
		if (_kbhit())
		{
			c = _getch();
		}
		if (c == 'q')
		{
			GAME_SHOULD_RUN = 0;
		}
		if (c == 't')
		{
			clearBottomRow = 1;
		}

		int canMoveRight = 1;
		int canMoveLeft = 1;
		for (int i = 0; i < 4; ++i)
		{
			if (block == NULL || c == -1) break;
			int blockPos = block->position.x + block->points[i].x;
			if (blockPos + 1 > 28 || grid[block->position.y + block->points[i].y][blockPos + 1] == 1) canMoveRight = 0;
			if (blockPos - 1 < 1 || grid[block->position.y + block->points[i].y][blockPos - 1] == 1) canMoveLeft = 0;
		}

		if (c == 'd' && canMoveRight)
		{
			block->position.x += 1;
		}
		if (c == 'a' && canMoveLeft)
		{
			block->position.x -= 1;
		}

		if (c == 'r' && canMoveLeft && canMoveRight)
		{
			block->rotated = (block->rotated + 4) % 16;
		}

		printf("\n");
		for (int i = 0; i < 4; ++i)
		{
			grid[block->prevPositon.y + block->points[block->prevRotated + i].y][block->prevPositon.x + block->points[block->prevRotated + i].x] = 0;

			grid[block->position.y + block->points[block->rotated + i].y][block->position.x + block->points[block->rotated + i].x] = 2;
			int offset = block->position.y + block->points[block->rotated + i].y + 1;
			if (offset <= 28 && grid[offset][block->position.x + block->points[block->rotated + i].x] == 1)
			{
				if (block->position.y == 0) state = GAME_FAILED;
				readyToCreateNew = 1;
			}
		}

		block->prevPositon = (struct Vector2){ block->position.x, block->position.y };
		block->prevRotated = block->rotated;
		if (readyToCreateNew == 0 && block->position.y >= 28)
		{
			readyToCreateNew = 1;
		}
		if (!readyToCreateNew)
		{
			block->position.y += 1;
		}
		else {
			MakeNewBlock();

			int rowsCleared[GRID_SIZE] = { 1 };
			int rowsAmount = 0;
			for (int i = 0; i < GRID_SIZE; ++i)
			{
				for (int y = 0; y < GRID_SIZE; ++y)
				{
					if (grid[i][y] != 1)
					{
						rowsCleared[i] = 0;
						break;
					}
				}
			}
			if (clearBottomRow)
			{
				rowsCleared[GRID_SIZE - 1] = 1;
				rowsCleared[GRID_SIZE - 3] = 1;
			}
			for (int i = 0; i < GRID_SIZE; ++i)
			{
				if (rowsCleared[i] == 1)
				{
					rowsAmount += 1;
					for (int y = 0; y < GRID_SIZE; ++y)
					{
						grid[i][y] = 0;
					}
				}
			}
			for (int i = GRID_SIZE - 1; i > 0; --i)
			{
				if (rowsCleared[i] == 0)continue;
				for (int y = i; y > 0; --y)
				{
					for (int b = 0; b < GRID_SIZE; ++b)
					{
						if (grid[y][b] == 1)
						{
							grid[y][b] = 0;
							grid[y + 1][b] = 1;
						}
						
					}
				}
			}
			score += (rowsAmount * 100 * (GRID_SIZE - 2)) * rowsAmount;


		}
		
		
		for (int i = 2; i < 30; ++i)
		{
			for (int y = 0; y < 30; ++y)
			{
				if ( i == 29 || y == 0 || y == 29)
				{
					printf("▓");
					continue;
				}
				if (grid[i][y])
				{
					printf("█");
					continue;
				}
				printf(" ");
			}
			if (i == 5) printf("  Score: %d", score);
			if (i == 7) printf("  \'a\' za lijevo \'d\' za desno");
			if (i == 8) printf("  \'r\' za rotaciju");
			if (i == 10) printf("  \'q\' za izaci");
			if (i == 14) printf(" -- DEBUG --");
			if (i == 15) printf(" x: %i y: %i", block->position.x, block->position.y);
			printf("\n");
		}
		
		c = 0;
		if (state == GAME_FAILED)
		{
			GAME_SHOULD_RUN = 0;
		}
		clearBottomRow = 0;
		Sleep(50);
	}
	free(block);
	free(blockTypes);
	while (1)
	{
		system("cls");
		system("color 2");
		for (int i = 0; i < 15; ++i)
		{
			if (i == 7) {
				printf("║║║███PROGAMMED BY JAN TESKEREDZIC███║║║\n");
				continue;
			}
			for (int y = 0; y < 40; ++y)
			{
				char i = rand()%2;
				if (i) printf("█");
				else printf("║");
			}
			printf("\n");
		}
		Sleep(50);
	}


	return 0;
}

void MakeNewBlock()
{
	readyToCreateNew = 0;
	score += 50;

	for (int i = 0; i < 4; ++i)
	{
		grid[block->position.y + block->points[block->rotated + i].y][block->position.x + block->points[block->rotated + i].x] = 1;
	}
	block->rotated = 0;
	block->prevRotated = 0;


	int random = rand() % 7;
	for (int i = 0; i < 16; ++i)
	{
		block->points[i] = blockTypes[random].points[i];
	}
	block->position = (struct Vector2){ GRID_SIZE / 2, 0 };
	block->prevPositon = (struct Vector2){ block->position.x, block->position.y };
}

void InitializeBlocks()
{
	//ALLOCATE POINTS
	blockTypes = malloc(sizeof(Block) * 7);

	//O SHAPE
	blockTypes[0].position = (struct Vector2){ 0, 0 };
	blockTypes[0].rotated = 0;
	blockTypes[0].points[0] = (struct Vector2){ 0, 0 };
	blockTypes[0].points[1] = (struct Vector2){ 1, 0 };
	blockTypes[0].points[2] = (struct Vector2){ 0, -1 };
	blockTypes[0].points[3] = (struct Vector2){ 1, -1 };

	blockTypes[0].points[4] = (struct Vector2){ 0, 0 };
	blockTypes[0].points[5] = (struct Vector2){ 1, 0 };
	blockTypes[0].points[6] = (struct Vector2){ 0, -1 };
	blockTypes[0].points[7] = (struct Vector2){ 1, -1 };

	blockTypes[0].points[8] = (struct Vector2){ 0, 0 };
	blockTypes[0].points[9] = (struct Vector2){ 1, 0 };
	blockTypes[0].points[10] = (struct Vector2){ 0, -1 };
	blockTypes[0].points[11] = (struct Vector2){ 1, -1 };

	blockTypes[0].points[12] = (struct Vector2){ 0, 0 };
	blockTypes[0].points[13] = (struct Vector2){ 1, 0 };
	blockTypes[0].points[14] = (struct Vector2){ 0, -1 };
	blockTypes[0].points[15] = (struct Vector2){ 1, -1 };
	
	//I SHAPE
	blockTypes[1].position = (struct Vector2){ 0, 0 };
	blockTypes[1].rotated = 0;

	blockTypes[1].points[0] = (struct Vector2){ 0, 0 };
	blockTypes[1].points[1] = (struct Vector2){ 0, -1 };
	blockTypes[1].points[2] = (struct Vector2){ 0, -2 };
	blockTypes[1].points[3] = (struct Vector2){ 0, -3 };

	blockTypes[1].points[4] = (struct Vector2){ -1, 0 };
	blockTypes[1].points[5] = (struct Vector2){ 0, 0 };
	blockTypes[1].points[6] = (struct Vector2){ 1, 0 };
	blockTypes[1].points[7] = (struct Vector2){ 2, 0 };

	blockTypes[1].points[8] = (struct Vector2){ 0, 0 };
	blockTypes[1].points[9] = (struct Vector2){ 0, -1 };
	blockTypes[1].points[10] = (struct Vector2){ 0, -2 };
	blockTypes[1].points[11] = (struct Vector2){ 0, -3 };

	blockTypes[1].points[12] = (struct Vector2){ -1, 0 };
	blockTypes[1].points[13] = (struct Vector2){ 0, 0 };
	blockTypes[1].points[14] = (struct Vector2){ 1, 0 };
	blockTypes[1].points[15] = (struct Vector2){ 2, 0 };

	//S SHAPE
	blockTypes[2].position = (struct Vector2){ 0, 0 };
	blockTypes[2].rotated = 0;
	blockTypes[2].points[0] = (struct Vector2){ 0, 0 };
	blockTypes[2].points[1] = (struct Vector2){ 1, 0 };
	blockTypes[2].points[2] = (struct Vector2){ 1, -1 };
	blockTypes[2].points[3] = (struct Vector2){ 2, -1 };

	blockTypes[2].points[4] = (struct Vector2){ 0, -1 };
	blockTypes[2].points[5] = (struct Vector2){ 0, -2 };
	blockTypes[2].points[6] = (struct Vector2){ 1, 0  };
	blockTypes[2].points[7] = (struct Vector2){ 1, -1 };

	blockTypes[2].points[8] = (struct Vector2){ 0, 0 };
	blockTypes[2].points[9] = (struct Vector2){ 1, 0 };
	blockTypes[2].points[10] = (struct Vector2){ 1, -1 };
	blockTypes[2].points[11] = (struct Vector2){ 2, -1 };

	blockTypes[2].points[12] = (struct Vector2){ 0, -1 };
	blockTypes[2].points[13] = (struct Vector2){ 0, -2 };
	blockTypes[2].points[14] = (struct Vector2){ 1, 0   };
	blockTypes[2].points[15] = (struct Vector2){ 1, -1 };

	//Z SHAPE
	blockTypes[3].position = (struct Vector2){ 0, 0 };
	blockTypes[3].rotated = 0;
	blockTypes[3].points[0] = (struct Vector2){ 1, 0 };
	blockTypes[3].points[1] = (struct Vector2){ 2, 0 };
	blockTypes[3].points[2] = (struct Vector2){ 0, -1 };
	blockTypes[3].points[3] = (struct Vector2){ 1, -1 };

	blockTypes[3].points[4] = (struct Vector2){ 0, 0 };
	blockTypes[3].points[5] = (struct Vector2){ 0, -1 };
	blockTypes[3].points[6] = (struct Vector2){ 1, -1 };
	blockTypes[3].points[7] = (struct Vector2){ 1, -2 };

	blockTypes[3].points[8] = (struct Vector2){ 1, 0 };
	blockTypes[3].points[9] = (struct Vector2){ 2, 0 };
	blockTypes[3].points[10] = (struct Vector2){ 0, -1 };
	blockTypes[3].points[11] = (struct Vector2){ 1, -1 };

	blockTypes[3].points[12] = (struct Vector2){ 0, 0  };
	blockTypes[3].points[13] = (struct Vector2){ 0, -1 };
	blockTypes[3].points[14] = (struct Vector2){ 1, -1 };
	blockTypes[3].points[15] = (struct Vector2){ 1, -2 };


	//L SHAPE
	blockTypes[4].position = (struct Vector2){ 0, 0 };
	blockTypes[4].rotated = 0;
	blockTypes[4].points[0] = (struct Vector2){ 0, 0 };
	blockTypes[4].points[1] = (struct Vector2){ 1, 0 };
	blockTypes[4].points[2] = (struct Vector2){ 0, -1 };
	blockTypes[4].points[3] = (struct Vector2){ 0, -2 };

	blockTypes[4].points[4] = (struct Vector2){ 0, 0 };
	blockTypes[4].points[5] = (struct Vector2){ 0, -1 };
	blockTypes[4].points[6] = (struct Vector2){ 1, -1 };
	blockTypes[4].points[7] = (struct Vector2){ 2, -1 };

	blockTypes[4].points[8] = (struct Vector2){ 1, 0 };
	blockTypes[4].points[9] = (struct Vector2){ 1, -1 };
	blockTypes[4].points[10] = (struct Vector2){ 1, -2 };
	blockTypes[4].points[11] = (struct Vector2){ 0, -2 };

	blockTypes[4].points[12] = (struct Vector2){ 0, 0  };
	blockTypes[4].points[13] = (struct Vector2){ 0, -1 };
	blockTypes[4].points[14] = (struct Vector2){ -1, 0  };
	blockTypes[4].points[15] = (struct Vector2){ -2, 0  };

	//J SHAPE
	blockTypes[5].position = (struct Vector2){ 0, 0 };
	blockTypes[5].rotated = 0;
	blockTypes[5].points[0] = (struct Vector2){ 0, 0 };
	blockTypes[5].points[1] = (struct Vector2){ 1, 0 };
	blockTypes[5].points[2] = (struct Vector2){ 1, -1 };
	blockTypes[5].points[3] = (struct Vector2){ 1, -2 };

	blockTypes[5].points[4] = (struct Vector2){ 0, 0 };
	blockTypes[5].points[5] = (struct Vector2){ 0, -1 };
	blockTypes[5].points[6] = (struct Vector2){ 1, 0 };
	blockTypes[5].points[7] = (struct Vector2){ 2, 0 };

	blockTypes[5].points[8] = (struct Vector2){ 0, 0 };
	blockTypes[5].points[9] = (struct Vector2){ 0, -1 };
	blockTypes[5].points[10] = (struct Vector2){ 0, -2 };
	blockTypes[5].points[11] = (struct Vector2){ 1, -2 };

	blockTypes[5].points[12] = (struct Vector2){ 0, 0 };
	blockTypes[5].points[13] = (struct Vector2){ 0, -1 };
	blockTypes[5].points[14] = (struct Vector2){ -1, -1 };
	blockTypes[5].points[15] = (struct Vector2){ -2, -1 };

	//T SHAPE
	blockTypes[6].position = (struct Vector2){ 0, 0 };
	blockTypes[6].rotated = 0;
	blockTypes[6].points[0] = (struct Vector2){ -1, 0 };
	blockTypes[6].points[1] = (struct Vector2){ 0, 0 };
	blockTypes[6].points[2] = (struct Vector2){ 1, 0 };
	blockTypes[6].points[3] = (struct Vector2){ 0, -1 };

	blockTypes[6].points[4] = (struct Vector2){ 0, 0 };
	blockTypes[6].points[5] = (struct Vector2){ 0, -1 };
	blockTypes[6].points[6] = (struct Vector2){ 0, -2 };
	blockTypes[6].points[7] = (struct Vector2){ 1, -1 };

	blockTypes[6].points[8] = (struct Vector2){ -1, -1 };
	blockTypes[6].points[9] = (struct Vector2){ 0, -1 };
	blockTypes[6].points[10] = (struct Vector2){ 1, -1 };
	blockTypes[6].points[11] = (struct Vector2){ 0, 0 };

	blockTypes[6].points[12] = (struct Vector2){ -1, -1 };
	blockTypes[6].points[13] = (struct Vector2){ 0, 0 };
	blockTypes[6].points[14] = (struct Vector2){ 0, -1 };
	blockTypes[6].points[15] = (struct Vector2){ 0, -2 };
}
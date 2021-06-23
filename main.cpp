// TikTakToe.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstdlib>

int getRandomNumber(int min, int max)
{
	int tempBuufer = rand();
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);

}

enum Cell {
	CROSS = 'X',
	ZERO = '0',
	EMPTY = ' '
};
enum Progress
{
	IN_PROGRESS,
	WON_HUMAN,
	WON_AI,
	DRAW
};

struct Coord
{
	size_t y;
	size_t x;
};

#pragma pack(push, 1) // Выравнивание полей структуры.
struct Field
{
	Cell** ppFIeld = nullptr;
	const size_t SIZE = 3;
	Cell human = EMPTY;
	Cell ai = EMPTY;
	size_t turn = 0;
	Progress progress = IN_PROGRESS;
};
#pragma pack(pop)

void initGame(Field& inGame) // Инициализация игры
{
	inGame.ppFIeld = new Cell * [inGame.SIZE]; // Выделяем динамическую память в куче
	for (size_t i = 0; i < inGame.SIZE; i++)
	{
		inGame.ppFIeld[i] = new Cell[inGame.SIZE];
	}

	for (size_t y = 0; y < inGame.SIZE; y++) // Инициализируем выделенную ОЗУ 
	{
		for (size_t x = 0; x < inGame.SIZE; x++)
		{
			inGame.ppFIeld[y][x] = EMPTY;
		}
	}

	if (getRandomNumber(0, 1000) > 500)
	{
		inGame.human = CROSS;
		inGame.ai = ZERO;
		inGame.turn = 0;
	}
	else
	{
		inGame.human = ZERO;
		inGame.ai = CROSS;
		inGame.turn = 1;
	}
}

void inline clearScreen()
{
	system("cls");
}

void printField(const Field& inGame)
{
	for (size_t y = 0; y < inGame.SIZE; y++)
	{
		if (y == 0)
		{
			std::cout << " X  1    2    3   Y" << std::endl;
		}
		std::cout << " ****************" << std::endl;
		for (size_t x = 0; x < inGame.SIZE; x++)
		{
			std::cout << " * " << static_cast<char>(inGame.ppFIeld[y][x]) << " ";
			if (x == inGame.SIZE - 1)
			{
				std::cout << " * " << y + 1;
			}
		}
		std::cout << std::endl;
		if (y == inGame.SIZE - 1)
		{
			std::cout << " ****************";
			std::cout << std::endl;
		}

	}
	std::cout << "  Human = " << static_cast<char>(inGame.human) << std::endl;
	std::cout << "     AI = " << static_cast<char>(inGame.ai) << std::endl;
	std::cout << std::endl;
	if (inGame.human == CROSS)
	{
		std::cout << "  Turn = " << inGame.turn << std::endl;
	}
	else
	{
		std::cout << "  Turn = " << inGame.turn - 1 << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Enter coordinate X and Y : " << std::endl;
}

Coord getHumanCoordinate(Field& takeCoord)
{

	return { 0, 0 };
}

Coord getAiCoordinate(Field& takeCoord)
{

	return { 1, 1 };
}

Progress getWon(Field& inGame)
{


	return DRAW;
}

void congrats(Progress progress)
{
	if (progress == WON_HUMAN)
	{
		std::cout << "Human WON! =)" << std::endl;
	}
	else if(progress == WON_AI)
	{
		std::cout << "Computer WON! =(" << std::endl;
	}
	else if (progress == DRAW)
	{
		std::cout << "Draw =/ " << std::endl;
	}
}

void deinitGame(Field& inGame)
{
	for (size_t i = 0; i < inGame.SIZE; i++) // Очищаем выделенную память
	{
		delete[] inGame.ppFIeld[i];
	}

	delete[] inGame.ppFIeld;
	inGame.ppFIeld = nullptr;
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	Field TikTokToe;
	initGame(TikTokToe);

	do
	{
		if (TikTokToe.turn % 2 == 0) // Ходит человек
		{
			Coord cordinate = getHumanCoordinate(TikTokToe);
			TikTokToe.ppFIeld[cordinate.y][cordinate.x] = TikTokToe.human;
		}
		else // Ходит ИИ
		{
			Coord cordinate = getAiCoordinate(TikTokToe);
			TikTokToe.ppFIeld[cordinate.y][cordinate.x] = TikTokToe.ai;
		}


		clearScreen();
		printField(TikTokToe);

		TikTokToe.progress = getWon(TikTokToe);

	} while (TikTokToe.progress == IN_PROGRESS);

	congrats(TikTokToe.progress);

	deinitGame(TikTokToe);
}
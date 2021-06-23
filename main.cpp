#include <iostream>
#include <cstdlib>

#if defined(_WIN32) // Проверка что соберается в ОС Windows

	#define WIN_OS

#endif

#define DEBUG

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
	#ifdef WIN_OS
	system("cls");
	#endif
	#ifndef WIN_OS
	system("clear");
	#endif	
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

Coord getHumanCoordinate(Field& inGame)
{

	Coord buffer;

	do
	{
	std::cout << "Enter x cordinate = ";
	std::cin >> buffer.x;
	std::cout << "Enter y cordinate = ";
	std::cin >> buffer.y;
	if (buffer.x == 0 || buffer.y == 0 ||  buffer.x > 3 || buffer.y > 3 || inGame.ppFIeld[buffer.y - 1][buffer.x -1] != EMPTY)
	{
		std::cout << "Wrong value, or the cell is busy! Pls, try again" << std::endl;
	}
		#ifdef DEBUG
		std::cout << inGame.ppFIeld[buffer.y - 1][buffer.x -1] << std::endl;		
		#endif

	} while ( buffer.x == 0 || buffer.y == 0 ||  buffer.x > 3 || buffer.y > 3 || inGame.ppFIeld[buffer.y - 1][buffer.x -1] != EMPTY );

	

	buffer.x--;
	buffer.y--;

	return buffer;
}


Coord getAiCoordinate(Field& inGame)
{
	if (inGame.ppFIeld[1][1] == EMPTY)
	{
		return { 1, 1 };
	}
	if (inGame.ppFIeld[0][2] == EMPTY)
	{
		return { 0, 2 };
	}
		if (inGame.ppFIeld[2][0] == EMPTY)
	{
		return { 2, 0 };
	}
		if (inGame.ppFIeld[0][0] == EMPTY)
	{
		return { 0, 0 };
	}
		if (inGame.ppFIeld[2][2] == EMPTY)
	{
		return { 2, 2 };
	}

	Coord voidArray[4]{0};
	size_t voidNum = 0;
	if(inGame.ppFIeld[0][1] == EMPTY)
	{
		voidArray[voidNum++] = {0 , 1};
	}
		if(inGame.ppFIeld[1][0] == EMPTY)
	{
		voidArray[voidNum++] = {1 , 0};
	}
		if(inGame.ppFIeld[1][2] == EMPTY)
	{
		voidArray[voidNum++] = {1 , 2};
	}
		if(inGame.ppFIeld[2][1] == EMPTY)
	{
		voidArray[voidNum++] = {2 , 1};
	}

	size_t index = getRandomNumber(0,1000) % voidNum;
	if (voidNum > 0)
	{
		return voidArray[index];
	}
	
}


Progress getWon(Field& inGame)
{
	bool EMPRYcell = false;
	bool rows = false;
	bool colums = false;
	Cell temp;
	
	for (size_t y = 0; y < inGame.SIZE; y++)
	{
		for (size_t x = 0; x < inGame.SIZE; x++)
		{
			if(inGame.ppFIeld[y][x] == EMPTY)
				EMPRYcell = true;
		}	
	}


	#ifdef DEBUG
	if (EMPRYcell)
		std::cout << "EMPRY cell!!!" << std::endl;
	else
		std::cout << "Not EMPRY cell!!!" << std::endl;
	#endif	

	temp = inGame.ppFIeld[1][1];
	if (temp == inGame.ppFIeld[0][0] && temp == inGame.ppFIeld[2][2] )
	{
		if (temp == inGame.human)
			{
				colums = false;
				temp = EMPTY;
				return WON_HUMAN;
			}
			else if(temp == inGame.ai )
			{
				colums = false;
				temp = EMPTY;
				return WON_AI;
			}
	}

	if (temp == inGame.ppFIeld[0][2] && temp == inGame.ppFIeld[2][0] )
	{
		if (temp == inGame.human)
			{
				colums = false;
				temp = EMPTY;
				return WON_HUMAN;
			}
			else if(temp == inGame.ai )
			{
				colums = false;
				temp = EMPTY;
				return WON_AI;
			}
	}

	for (size_t y = 0; y < inGame.SIZE; y++)
	{
		
		for (size_t x = 0; x < inGame.SIZE; x++)
		{
			if(x == 0)
			{
			 temp = inGame.ppFIeld[y][x];
			 continue;
			}
			if (inGame.ppFIeld[y][x] == temp)
				rows = true;
			else 
			{
				rows = false;
				temp = EMPTY;
				continue;
			}
		}	
		if (rows || colums)
		{
			if (temp == inGame.human)
			{
				rows = false;
				temp = EMPTY;
				return WON_HUMAN;
			}
			else if(temp == inGame.ai )
			{
				rows = false;
				temp = EMPTY;
				return WON_AI;
			}
			else if(EMPRYcell)
			{
				rows = false;
				temp = EMPTY;
				return IN_PROGRESS;
			}
			
		}
		
	}

for (size_t x = 0; x < inGame.SIZE; x++)
	{
		
		for (size_t y = 0; y < inGame.SIZE; y++)
		{
			if(x == 0)
			{
			 temp = inGame.ppFIeld[y][x];
			 continue;
			}
			if (inGame.ppFIeld[y][x] == temp)
				colums = true;
			else 
			{
				colums = false;
				temp = EMPTY;
				continue;
			}
		}	
		if (rows || colums)
		{
			if (temp == inGame.human)
			{
				colums = false;
				temp = EMPTY;
				return WON_HUMAN;
			}
			else if(temp == inGame.ai )
			{
				colums = false;
				temp = EMPTY;
				return WON_AI;
			}
			else if(EMPRYcell)
			{
				colums = false;
				temp = EMPTY;
				return IN_PROGRESS;
			}
			
		}
		
	}

	
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
	clearScreen();
	Field TikTokToe;
	initGame(TikTokToe);

	printField(TikTokToe);

	do
	{
		if (TikTokToe.turn % 2 == 0) // Ходит человек
		{
			Coord cord = getHumanCoordinate(TikTokToe);
			TikTokToe.ppFIeld[cord.y][cord.x] = TikTokToe.human;
		}
		else // Ходит ИИ
		{
			Coord cord = getAiCoordinate(TikTokToe);
			TikTokToe.ppFIeld[cord.y][cord.x] = TikTokToe.ai;
		}

		TikTokToe.turn++;

		clearScreen();

		printField(TikTokToe);

		TikTokToe.progress = getWon(TikTokToe);

	} while (TikTokToe.progress == IN_PROGRESS);

	congrats(TikTokToe.progress);

	deinitGame(TikTokToe);
}

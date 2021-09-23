#include "Board.h"

BOARD::BOARD()
{
	//загрузка изображений и создание спрайтов
	Iboard.loadFromFile("images/board.png");
	Tboard.loadFromImage(Iboard);
	Board.setTexture(Tboard);
	Board.setPosition(0, 0);

	Chessman1.loadFromFile("images//chessman_white.png");
	Chessman1.createMaskFromColor(Color(255, 255, 255)); // убираем фон
	Chessman2.loadFromFile("images//chessman.png");
	Chessman2.createMaskFromColor(Color(255, 255, 255));
	chessman1.loadFromImage(Chessman1);
	chessman2.loadFromImage(Chessman2);
	whiteF.setTexture(chessman1);
	blackF.setTexture(chessman2);

	Iend.loadFromFile("images//end.png");
	Tend.loadFromImage(Iend);
	Send.setTexture(Tend);

	Istep.loadFromFile("images//step.png");
	Tstep.loadFromImage(Istep);
	Step.setTexture(Tstep);

	Iwasted.loadFromFile("images//wasted.png");
	Twasted.loadFromImage(Iwasted);
	Wasted.setTexture(Twasted);

	Iwin.loadFromFile("images//win.png");
	Twin.loadFromImage(Iwin);
	Win.setTexture(Twin);

	// края шахматной доски где содержатся цифры и буквы, исключаю из обработки
	for (int i = 0; i <= 9; i++)
	{
		matr[i][0] = -1;
		matr[9 - i][9] = -1;
		matr[0][i] = -1;
		matr[9][9 - i] = -1;
	}

	// начальная расстановка шахмат на доске
	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= 3; j++)
		{
			matr[i][j] = 1;
			matr[9 - i][9 - j] = 2;
		}

	strokeStatus.first = true; // разрешен ли ход
	strokeStatus.second = 0; // счетчик ходов
};


bool BOARD::createFreeCells(Vector2i v, bool flag = true) // создаю ячейки для выбранной фигуры
{
	bool result = false;
	v.x = v.x / h;
	v.y = v.y / h;

	//выбираем свободные места
	if (matr[v.x - 1][v.y] == 0 && flag)
	{
		matr[v.x - 1][v.y] = 4;
		result = true;
	}
	else
		if (matr[v.x - 1][v.y] == 1 || matr[v.x - 1][v.y] == 2)
			if (matr[v.x - 2][v.y] == 0)
			{
				matr[v.x - 2][v.y] = 3;
				result = true;
			}

	if (matr[v.x + 1][v.y] == 0 && flag)
	{
		matr[v.x + 1][v.y] = 4;
		result = true;
	}
	else
		if (matr[v.x + 1][v.y] == 1 || matr[v.x + 1][v.y] == 2)
			if (matr[v.x + 2][v.y] == 0)
			{
				matr[v.x + 2][v.y] = 3;
				result = true;
			}

	if (matr[v.x][v.y - 1] == 0 && flag)
	{
		matr[v.x][v.y - 1] = 4;
		result = true;
	}
	else
		if (matr[v.x][v.y - 1] == 1 || matr[v.x][v.y - 1] == 2)
			if (matr[v.x][v.y - 2] == 0)
			{
				matr[v.x][v.y - 2] = 3;
				result = true;
			}


	if (matr[v.x][v.y + 1] == 0 && flag)
	{
		matr[v.x][v.y + 1] = 4;
		result = true;
	}
	else
		if (matr[v.x][v.y + 1] == 1 || matr[v.x][v.y + 1] == 2)
			if (matr[v.x][v.y + 2] == 0)
			{
				matr[v.x][v.y + 2] = 3;
				result = true;
			}

	return result;
}

void BOARD::deleteFreeCells()
{
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			if (matr[i][j] == 3 || matr[i][j] == 4)
				matr[i][j] = 0;
}

int BOARD::checkWin() // проверяя состояние массива узнаю есть ли победитель
{
	int result = 0;
	int Play = 0, AI = 0;
	// конечная расстановка шахмат на доске
	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= 3; j++)
		{
			if (matr[i][j] == 2)
				Play++;
			if (matr[9 - i][9 - j] == 1)
				AI++;
		}

	if (Play == 9)
		result = 2;
	else
		if (AI == 9)
			result = 1;

	return result;
}

int BOARD::get_cell(Vector2i v)
{
	return matr[v.x / h][v.y / h];
}

void BOARD::set_cell(Vector2i v, int x)
{
	matr[v.x / h][v.y / h] = x;
}

void BOARD::Step_AI() {};

int BOARD::get_h()
{
	return h;
}

Vector2i AI::makeStep(Vector2i v, int p = 1)
{
	Vector2i result;
	int k, m;
	k = p * rand() % 2;
	m = p * abs(k - 1);


	// цепочка выбирающая приоритетное свободное место и выполняющая ход случайным образом выбирая из двух
	if ((matr[v.x + k][v.y + m] == 1 || matr[v.x + k][v.y + m] == 2) && matr[v.x + 2 * k][v.y + 2 * m] == 0)
	{
		matr[v.x + 2 * k][v.y + 2 * m] = 1;
		matr[v.x][v.y] = 0;
		result.x = v.x + 2 * k;
		result.y = v.y + 2 * m;
	}
	else
		if ((matr[v.x + m][v.y + k] == 1 || matr[v.x + m][v.y + k] == 2) && matr[v.x + 2 * m][v.y + 2 * k] == 0)
		{
			matr[v.x + 2 * m][v.y + 2 * k] = 1;
			matr[v.x][v.y] = 0;
			result.x = v.x + 2 * m;
			result.y = v.y + 2 * k;
		}
		else
			if (matr[v.x + k][v.y + m] == 0)
			{
				matr[v.x + k][v.y + m] = 1;
				matr[v.x][v.y] = 0;
				result.x = v.x + k;
				result.y = v.y + m;
			}
			else
				if (matr[v.x + m][v.y + k] == 0)
				{
					matr[v.x + m][v.y + k] = 1;
					matr[v.x][v.y] = 0;
					result.x = v.x + m;
					result.y = v.y + k;
				}

	return result;
}

void AI::Step_AI()
{
	Vector2i vec;

	bool flag = true;
	bool forward = true;
	int i = 1, j = 1;
	int sum = 2;

	while (flag && checkWin() != 1) // работает пока не совершен ход либо партия не выиграна 
	{
		i = 1, j = 1;
		while (i <= 8 && flag)
		{
			while (j <= 8 && flag)
			{

				if ((i < 6 || j < 6) && matr[i][j] == 1) // если координаты меньше необходимых и это фигура AI
				{
					vec;
					vec.x = i;
					vec.y = j;

					// ходы с примерно 0.9 вероятностью направлены в угол противника
					if (rand() % 10 != 0)
						vec = makeStep(vec);
					else
						vec = makeStep(vec, -1);

					// если был выполнен ход завершаем циклы
					if (vec.x != 0)
					{
						flag = false;
					}

				}
				else
				{
					if (matr[i][j] == 1)// для фигур которые дошли на нужное поле, но пока не заняли все угловые позиции
					{
						vec;
						vec.x = i;
						vec.y = j;


						vec = makeStep(vec);

						if (vec.x != 0)
						{
							flag = false;
						}
					}
				}

				j++;
			}
			j = 1;
			i++;
		}
		forward = false;
	}
}
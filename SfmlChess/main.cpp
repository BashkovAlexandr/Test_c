#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include "Board.h"

int main()
{
	RenderWindow window(sf::VideoMode(1200, 1000), "Chess"); //, sf::Style::Fullscreen

	typedef BOARD* Board;
	Board board = new AI; // создаю объект с наследованием

	Vector2i Save_Pos;
	std::pair<bool, Vector2i> isMove;//переменная для щелчка мыши по спрайту (вектор не нуль, когда ход не закончен)
	isMove.first = false;
	isMove.second.x = 0;
	isMove.second.y = 0;

	bool canPlay = true;
	while (window.isOpen())
	{
		window.clear(Color(255, 255, 255));
		window.draw(board->Board); // рисуем доску
		board->Send.setPosition(1020, 120); // кнопка завершения хода
		window.draw(board->Send);
		bool end = false;


		// отрисовка всех шахматных фигур и клеток где разрешен ход на доске
		for (int i = 1; i <= 8; i++)
			for (int j = 1; j <= 8; j++)
			{
				switch (board->matr[i][j])
				{
				case 1:
					board->whiteF.setPosition((i)* board->get_h(), (j)* board->get_h());
					window.draw(board->whiteF);
					break;
				case 2:
					board->blackF.setPosition((i)* board->get_h(), (j)* board->get_h());
					window.draw(board->blackF);
					break;
				case 3:
					board->Step.setPosition((i)* board->get_h(), (j)* board->get_h());
					window.draw(board->Step);
				case 4:
					board->Step.setPosition((i)* board->get_h(), (j)* board->get_h());
					window.draw(board->Step);
				}
			}

		if (!end)// пока игра не завершена
		{
			Vector2i Pos = Mouse::getPosition(window);//забираем коорд курсора для анимации движения

			sf::Event event; // событие

			if (window.pollEvent(event) && canPlay) // если произошло собитие и играет игрок
			{
				if (event.type == sf::Event::Closed)
					window.close();


				// если клавиша зажата
				if (event.type == Event::MouseButtonPressed)
				{
					if (event.key.code == Mouse::Left)// Если ЛКМ
					{
						Vector2i position = Mouse::getPosition(window);

						// если клик по спрайту игрока и ход разрешен и это первый ход фигурой
						if (board->matr[position.x / board->get_h()][position.y / board->get_h()] == 2 && board->strokeStatus.first && isMove.second.x == 0)
						{
							std::cout << "isClicked!\n";//выводим в консоль сообщение об этом

							// сохраняю координаты для работы с координатами после передвижения фигуры
							Save_Pos.x = position.x;
							Save_Pos.y = position.y;

							//вычисляю свободные ячейки для хода, и если это цепочка ходов то запрет соседние клетки
							board->createFreeCells(Save_Pos, board->strokeStatus.second > 0 ? false : true);

							isMove.first = true;//можем двигать спрайт

							//std::cout << "isClicked!\n";
						}
						else // если вектор не равен нулю, ход не первый в цепочке, проверяю что был клик по нужной фигуре и ход разрешен правилами
							if (isMove.second.x != 0 && isMove.second.x == position.x / board->get_h() && isMove.second.y == position.y / board->get_h() && board->strokeStatus.first)
							{
								board->createFreeCells(Save_Pos, board->strokeStatus.second > 0 ? false : true);
								isMove.first = true;//можем двигать спрайт
							}
							else// если ход запрещен либо нажали на кнопку завершения хода
								if ((1020 < position.x && position.x < 1200 && 120 < position.y && position.y < 300) || !board->strokeStatus.first)
								{
									isMove.second.x = 0;
									isMove.second.y = 0;
									isMove.first = false;
									board->strokeStatus.first = false;
									board->strokeStatus.second = 0;
									canPlay = false; //запрет на ход для игрока
									std::cout << "END!\n";
								}
					}
					std::cout << "yes!\n";

				}

				//если отпустили клавишу
				if (event.type == Event::MouseButtonReleased)
					if (event.key.code == Mouse::Left)
					{
						std::cout << "NoClicked!\n";
						Vector2i position = Mouse::getPosition(window);

						if (isMove.first) // если ход был разрешен
						{
							if (board->get_cell(position) == 3) // если сделал перешагивание через другую фигуру
							{
								board->set_cell(position, 2); // меняю позицию на новую
								board->set_cell(Save_Pos, 0);

								board->strokeStatus.second++; // повышаю счетчих ходов

								isMove.second.x = position.x / board->get_h();// сохраняю позицию в цепочке для следующего хода
								isMove.second.y = position.y / board->get_h();

								Save_Pos = position; // сохраняю поцизию для дальнейшего вычисления разрешенных для хода клеток
							}
							else
								if (board->get_cell(position) == 4) // если шагнул на соседнюю клетку
								{
									if (board->strokeStatus.first && board->strokeStatus.second == 0) //проверяю разрешен ли был такой ход
									{
										board->set_cell(position, 2); // меняю позицию фигуры
										board->set_cell(Save_Pos, 0);
										board->strokeStatus.first = false; // запрещаю дальнейший ход после хода на соседнюю клетку
										Save_Pos = position;
									}
									else // если ход не возможет, обновляю статус хода 
									{
										board->strokeStatus.first = false;
										board->strokeStatus.second = 0;
									}
								}


							board->deleteFreeCells(); // удаляю из массива данных о сосоянии доски ячейки с состоянием "разрешен ход"
						}

						isMove.first = false; //не можем двигать спрайт после отпускания клавиши ЛКМ
					}
			}
			else
			{
				if (!canPlay) // если игроку ходить нельзя, очередь AI
				{
					board->Step_AI();
					board->strokeStatus.first = true;
					canPlay = true;
				}
			}


			//если можем двигать фигуру
			if (isMove.first) {

				if (event.type != Event::MouseButtonPressed)
				{
					board->blackF.setPosition(Pos.x - board->get_h() / 2, Pos.y - board->get_h() / 2); // ставлю текущую позицию фигуры, для анимации
					//sprite2.setOrigin(h / 2, h / 2);
					window.draw(board->blackF);
				}
				//Array_board[Pos.x / h][Pos.y / h] = 2;
			}

		}

		// определяю есть ли победитель
		if (board->checkWin() == 1)
		{
			board->Wasted.setPosition(190, 415);
			window.draw(board->Wasted);
			end = true;
		}
		else
			if (board->checkWin() == 2)
			{
				board->Send.setPosition(15, 425);
				window.draw(board->Win);
				end = true;
			}
		//отрисовка окна
		window.display();// показать все это

	}
	return 0;
};
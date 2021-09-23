#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include "Board.h"

int main()
{
	RenderWindow window(sf::VideoMode(1200, 1000), "Chess"); //, sf::Style::Fullscreen

	typedef BOARD* Board;
	Board board = new AI; // ������ ������ � �������������

	Vector2i Save_Pos;
	std::pair<bool, Vector2i> isMove;//���������� ��� ������ ���� �� ������� (������ �� ����, ����� ��� �� ��������)
	isMove.first = false;
	isMove.second.x = 0;
	isMove.second.y = 0;

	bool canPlay = true;
	while (window.isOpen())
	{
		window.clear(Color(255, 255, 255));
		window.draw(board->Board); // ������ �����
		board->Send.setPosition(1020, 120); // ������ ���������� ����
		window.draw(board->Send);
		bool end = false;


		// ��������� ���� ��������� ����� � ������ ��� �������� ��� �� �����
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

		if (!end)// ���� ���� �� ���������
		{
			Vector2i Pos = Mouse::getPosition(window);//�������� ����� ������� ��� �������� ��������

			sf::Event event; // �������

			if (window.pollEvent(event) && canPlay) // ���� ��������� ������� � ������ �����
			{
				if (event.type == sf::Event::Closed)
					window.close();


				// ���� ������� ������
				if (event.type == Event::MouseButtonPressed)
				{
					if (event.key.code == Mouse::Left)// ���� ���
					{
						Vector2i position = Mouse::getPosition(window);

						// ���� ���� �� ������� ������ � ��� �������� � ��� ������ ��� �������
						if (board->matr[position.x / board->get_h()][position.y / board->get_h()] == 2 && board->strokeStatus.first && isMove.second.x == 0)
						{
							std::cout << "isClicked!\n";//������� � ������� ��������� �� ����

							// �������� ���������� ��� ������ � ������������ ����� ������������ ������
							Save_Pos.x = position.x;
							Save_Pos.y = position.y;

							//�������� ��������� ������ ��� ����, � ���� ��� ������� ����� �� ������ �������� ������
							board->createFreeCells(Save_Pos, board->strokeStatus.second > 0 ? false : true);

							isMove.first = true;//����� ������� ������

							//std::cout << "isClicked!\n";
						}
						else // ���� ������ �� ����� ����, ��� �� ������ � �������, �������� ��� ��� ���� �� ������ ������ � ��� �������� ���������
							if (isMove.second.x != 0 && isMove.second.x == position.x / board->get_h() && isMove.second.y == position.y / board->get_h() && board->strokeStatus.first)
							{
								board->createFreeCells(Save_Pos, board->strokeStatus.second > 0 ? false : true);
								isMove.first = true;//����� ������� ������
							}
							else// ���� ��� �������� ���� ������ �� ������ ���������� ����
								if ((1020 < position.x && position.x < 1200 && 120 < position.y && position.y < 300) || !board->strokeStatus.first)
								{
									isMove.second.x = 0;
									isMove.second.y = 0;
									isMove.first = false;
									board->strokeStatus.first = false;
									board->strokeStatus.second = 0;
									canPlay = false; //������ �� ��� ��� ������
									std::cout << "END!\n";
								}
					}
					std::cout << "yes!\n";

				}

				//���� ��������� �������
				if (event.type == Event::MouseButtonReleased)
					if (event.key.code == Mouse::Left)
					{
						std::cout << "NoClicked!\n";
						Vector2i position = Mouse::getPosition(window);

						if (isMove.first) // ���� ��� ��� ��������
						{
							if (board->get_cell(position) == 3) // ���� ������ ������������� ����� ������ ������
							{
								board->set_cell(position, 2); // ����� ������� �� �����
								board->set_cell(Save_Pos, 0);

								board->strokeStatus.second++; // ������� ������� �����

								isMove.second.x = position.x / board->get_h();// �������� ������� � ������� ��� ���������� ����
								isMove.second.y = position.y / board->get_h();

								Save_Pos = position; // �������� ������� ��� ����������� ���������� ����������� ��� ���� ������
							}
							else
								if (board->get_cell(position) == 4) // ���� ������ �� �������� ������
								{
									if (board->strokeStatus.first && board->strokeStatus.second == 0) //�������� �������� �� ��� ����� ���
									{
										board->set_cell(position, 2); // ����� ������� ������
										board->set_cell(Save_Pos, 0);
										board->strokeStatus.first = false; // �������� ���������� ��� ����� ���� �� �������� ������
										Save_Pos = position;
									}
									else // ���� ��� �� ��������, �������� ������ ���� 
									{
										board->strokeStatus.first = false;
										board->strokeStatus.second = 0;
									}
								}


							board->deleteFreeCells(); // ������ �� ������� ������ � �������� ����� ������ � ���������� "�������� ���"
						}

						isMove.first = false; //�� ����� ������� ������ ����� ���������� ������� ���
					}
			}
			else
			{
				if (!canPlay) // ���� ������ ������ ������, ������� AI
				{
					board->Step_AI();
					board->strokeStatus.first = true;
					canPlay = true;
				}
			}


			//���� ����� ������� ������
			if (isMove.first) {

				if (event.type != Event::MouseButtonPressed)
				{
					board->blackF.setPosition(Pos.x - board->get_h() / 2, Pos.y - board->get_h() / 2); // ������ ������� ������� ������, ��� ��������
					//sprite2.setOrigin(h / 2, h / 2);
					window.draw(board->blackF);
				}
				//Array_board[Pos.x / h][Pos.y / h] = 2;
			}

		}

		// ��������� ���� �� ����������
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
		//��������� ����
		window.display();// �������� ��� ���

	}
	return 0;
};
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>

using namespace sf;
using namespace std;

class BOARD
{
private:
	int h = 100; // размер стороны клетки 
	//спрайт дл€ шахматной доски
	Image Iboard;
	Texture Tboard;

	// создание спрайтов дл€ белых и черных фигур
	Image Chessman1, Chessman2;
	Texture chessman1, chessman2;

	// спрайт дл€ кнопки завершени€ хода
	Image Iend;
	Texture Tend;

	// спрайт дл€ выделени€ клетки цветом
	Image Istep;
	Texture Tstep;

	// спрайт дл€ выделени€ клетки цветом
	Image Iwasted;
	Texture Twasted;

	// спрайт дл€ выделени€ клетки цветом
	Image Iwin;
	Texture Twin;

public:
	int matr[10][10] = { 0 }; // у каждой клетки есть состо€ние: "-1" - вне пол€, "0" - пуста€ клетка, "1" - бела€ фигура, "2" - черна€ фигура, ">=3" - разрешен ход.

	pair <bool, int> strokeStatus; // отвечает за состо€ние последовательности ходов

	Sprite Board;
	Sprite whiteF;
	Sprite blackF;
	Sprite Step;
	Sprite Send;
	Sprite Wasted;
	Sprite Win;

	BOARD();

	bool createFreeCells(Vector2i v, bool flag); // создаю €чейки дл€ выбранной фигуры
	
	void deleteFreeCells();

	int checkWin();// провер€€ состо€ние массива узнаю есть ли победитель

	int get_cell(Vector2i v);

	void set_cell(Vector2i v, int x);

	virtual void Step_AI();//виртуальна€ функци€ дл€ работы AI, определ€етс€ в наследнике 

	int get_h();


};

class AI : public BOARD
{
private:
	Vector2i makeStep(Vector2i v, int p);
public:

	virtual void Step_AI();
};
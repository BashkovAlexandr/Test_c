#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>

using namespace sf;
using namespace std;

class BOARD
{
private:
	int h = 100; // ������ ������� ������ 
	//������ ��� ��������� �����
	Image Iboard;
	Texture Tboard;

	// �������� �������� ��� ����� � ������ �����
	Image Chessman1, Chessman2;
	Texture chessman1, chessman2;

	// ������ ��� ������ ���������� ����
	Image Iend;
	Texture Tend;

	// ������ ��� ��������� ������ ������
	Image Istep;
	Texture Tstep;

	// ������ ��� ��������� ������ ������
	Image Iwasted;
	Texture Twasted;

	// ������ ��� ��������� ������ ������
	Image Iwin;
	Texture Twin;

public:
	int matr[10][10] = { 0 }; // � ������ ������ ���� ���������: "-1" - ��� ����, "0" - ������ ������, "1" - ����� ������, "2" - ������ ������, ">=3" - �������� ���.

	pair <bool, int> strokeStatus; // �������� �� ��������� ������������������ �����

	Sprite Board;
	Sprite whiteF;
	Sprite blackF;
	Sprite Step;
	Sprite Send;
	Sprite Wasted;
	Sprite Win;

	BOARD();

	bool createFreeCells(Vector2i v, bool flag); // ������ ������ ��� ��������� ������
	
	void deleteFreeCells();

	int checkWin();// �������� ��������� ������� ����� ���� �� ����������

	int get_cell(Vector2i v);

	void set_cell(Vector2i v, int x);

	virtual void Step_AI();//����������� ������� ��� ������ AI, ������������ � ���������� 

	int get_h();


};

class AI : public BOARD
{
private:
	Vector2i makeStep(Vector2i v, int p);
public:

	virtual void Step_AI();
};
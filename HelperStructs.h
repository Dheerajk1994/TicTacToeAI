#pragma once
#include <vector>

enum SquareOccupied {
	EMPTY = 0, PLAYER_X = 1, PLAYER_O = 2
};

enum WinState {
	TIE = 0, X_WINS, O_WINS, CONTINUE
};

struct Vector2 {
	int x = 0;
	int y = 0;
public:
	Vector2() { }
	Vector2(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

struct Square {
	Vector2 topLeftCorner, bottomRightCorner;
	SquareOccupied squareStatus = SquareOccupied::EMPTY;
public:
	Square() { }
	Square(Vector2 _topLeftCorner, Vector2 _bottomRightCorner) {
		topLeftCorner = _topLeftCorner;
		bottomRightCorner = _bottomRightCorner;

	}
};



#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "AIPlayer.h"
#include <cstring>

class MyApplication : public olc::PixelGameEngine {
	int playingField[9];
	const int SQUARE_SIZE = 50;
	const int PADDING = 20;
	const std::string CIRCLE_ICON = "O";
	const std::string CROSS_ICON = "X";
	const int ICON_SCALE_VALUE = 3;//the size of the placed player icons
	bool playersTurn = true;

	AIPlayer computer;

	Square* squares;

public:
	MyApplication() {
		sAppName = "TicTacToe With C++ AI";
	}

public:
	bool OnUserCreate() override {
		squares = new Square[9];
		DrawPlayArea();
		UpdatePlayArea();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if (GetMouse(0).bReleased && playersTurn) {
			PlaceIconInSquare(CheckWhichSquareWasClicked(Vector2(GetMouseX(), GetMouseY())));
		} 
		else if (GetKey(olc::R).bPressed) {
			RestartGame();
		}
		return true;
	}
private:

	void GameOver(WinState state) {
		UpdatePlayArea();
		switch (state)
		{
		case TIE:
			PrintTurnInfo("Its a tie.\n\nPress R to restart.");
			break;
		case X_WINS:
			PrintTurnInfo("You won!\n\nPress R to restart.");
			break;
		case O_WINS:
			PrintTurnInfo("The computer won!\n\nPress R to restart.");
			break;
		case CONTINUE:
			break;
		default:
			break;
		}
		playersTurn = false;
	}

	void RestartGame() {
		delete squares;
		squares = new Square[9];
		DrawPlayArea();
		UpdatePlayArea();
		playersTurn = true;
	}

	void PrintTurnInfo(std::string info) {
		DrawString(20, 190, info, olc::WHITE, 1);
	}

	void PlaceIconInSquare(int squareId) {
		if (squareId == -1 || squares[squareId].squareStatus != SquareOccupied::EMPTY) return;
		squares[squareId].squareStatus = SquareOccupied::PLAYER_X;
		WinState currentState = computer.GetWinState(SquareOccupied::PLAYER_X, squares);
		if (currentState != WinState::CONTINUE) {
			GameOver(currentState);
		}
		else {
			playersTurn = false;
			int id = computer.MyTurn(squares);
			squares[id].squareStatus = SquareOccupied::PLAYER_O;
			currentState = computer.GetWinState(SquareOccupied::PLAYER_O, squares);
			if (currentState != WinState::CONTINUE) {
				GameOver(currentState);
			}
			else {
				playersTurn = true;
				UpdatePlayArea();
			}
		}
	}

	void DrawIcon(std::string icon, int squareId) {
		DrawString(squares[squareId].topLeftCorner.x + SQUARE_SIZE / 3, squares[squareId].topLeftCorner.y + SQUARE_SIZE / 3, icon, olc::BLACK, ICON_SCALE_VALUE);
	}


	void DrawPlayArea() {
		for (int x = 0; x < 3; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				FillRect(x * SQUARE_SIZE + PADDING, y * SQUARE_SIZE + PADDING, SQUARE_SIZE, SQUARE_SIZE, olc::WHITE);
				DrawRect(x * SQUARE_SIZE + PADDING, y * SQUARE_SIZE + PADDING, SQUARE_SIZE, SQUARE_SIZE, olc::DARK_RED);
				squares[x + y * 3] = Square(
					Vector2(x * SQUARE_SIZE + PADDING, y * SQUARE_SIZE + PADDING),
					Vector2(x * SQUARE_SIZE + PADDING + SQUARE_SIZE, y * SQUARE_SIZE + PADDING + SQUARE_SIZE)
				);
			}
		}
	}

	void UpdatePlayArea() {
		Clear(olc::BLACK);
		for (int x = 0; x < 3; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				FillRect(x * SQUARE_SIZE + PADDING, y * SQUARE_SIZE + PADDING, SQUARE_SIZE, SQUARE_SIZE, olc::WHITE);
				DrawRect(x * SQUARE_SIZE + PADDING, y * SQUARE_SIZE + PADDING, SQUARE_SIZE, SQUARE_SIZE, olc::DARK_RED);
				if (squares[x + y * 3].squareStatus == SquareOccupied::PLAYER_X) {
					DrawIcon(CROSS_ICON, x + y * 3);
				}
				else if((squares[x + y * 3].squareStatus == SquareOccupied::PLAYER_O)) {
					DrawIcon(CIRCLE_ICON, x + y * 3);
				}
			}
		}
		//PrintTurnInfo();
	}

	int CheckWhichSquareWasClicked(const Vector2 mousePos) {
		if (mousePos.x < PADDING || mousePos.x > PADDING + SQUARE_SIZE * 3 || mousePos.y < PADDING || mousePos.y > PADDING + SQUARE_SIZE * 3) {
			return -1;//out of bounds
		}
		int x = std::floor((mousePos.x - PADDING) / SQUARE_SIZE);
		int y = std::floor((mousePos.y - PADDING) / SQUARE_SIZE);
		
		return (x + y * 3);
	}
	   	 

};

int main(int argc, char* argv[]) {

	MyApplication application;
	if (application.Construct(190, 250, 2, 2)) {
		application.Start();
	}
	return 0;
}

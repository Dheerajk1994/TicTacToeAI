#ifndef AI_PLAYER
#define AI_PLAYER
#include "HelperStructs.h"

class AIPlayer {

public:
	int MyTurn(const Square playArea[9]) {
		std::vector<int> idsToCheck = GetEmptySquareIDs(playArea);
		WinState* bestResult;
		bestResult = new WinState[idsToCheck.size()] ;
		for (int i = 0; i < idsToCheck.size(); ++i) {
			Square newArea[9];
			for (int copyID = 0; copyID < 9; ++copyID) {
				newArea[copyID] = playArea[copyID];
			}
			newArea[idsToCheck[i]].squareStatus = SquareOccupied::PLAYER_O;
			bestResult[i] = GetBestResult(PLAYER_O, newArea);
			std::cout << "at index: " << idsToCheck[i] << " win status: " << bestResult[i] << std::endl;
		}

		for (int i = 0; i < idsToCheck.size(); ++i) {
			if (bestResult[i] == WinState::O_WINS) {
				return idsToCheck[i];;
			}
		}
		for (int i = 0; i < idsToCheck.size(); ++i) {
			if (bestResult[i] == WinState::TIE) {
				return idsToCheck[i];
			}
		}
		delete bestResult;
		return idsToCheck[0];
	}

	WinState GetBestResult(SquareOccupied currentPlayer, const Square playArea[9]) {
		//check if the current state is winning
		if (SeeIfThisPlayerWon(currentPlayer, playArea)) {
			return static_cast<WinState>(currentPlayer);
		}
		//check if the current state is a tie
		else if (GetEmptySquaresAmount(playArea) == 0) {
			return WinState::TIE;
		}
		else {
			SquareOccupied playerToCheck = (currentPlayer == SquareOccupied::PLAYER_O) ? SquareOccupied::PLAYER_X : PLAYER_O;
			std::vector<int> idsToCheck = GetEmptySquareIDs(playArea);
			WinState* bestResult;
			bestResult = new WinState[idsToCheck.size()];
			for (int i = 0; i < idsToCheck.size(); ++i) {
				Square newArea[9];
				for (int copyID = 0; copyID < 9; ++copyID) {
					newArea[copyID] = playArea[copyID];
				}
				newArea[idsToCheck[i]].squareStatus = playerToCheck;
				bestResult[i] = GetBestResult(playerToCheck, newArea);
			}


			//current player is computer
			if (playerToCheck == PLAYER_O) {
				for (int i = 0; i < idsToCheck.size(); ++i) {
					if (bestResult[i] == O_WINS) return O_WINS;
				}
				for (int i = 0; i < idsToCheck.size(); ++i) {
					if (bestResult[i] == TIE) return TIE;
				}
				return X_WINS;
			}
			//current player is human
			else {
				for (int i = 0; i < idsToCheck.size(); ++i) {
					if (bestResult[i] == X_WINS) return X_WINS;
				}
				for (int i = 0; i < idsToCheck.size(); ++i) {
					if (bestResult[i] == TIE) return TIE;
				}
				return O_WINS;
			}
		}
	}


	//LOGIC CONTROLS

	WinState GetWinState(SquareOccupied playerLastPlaced, Square playArea[9]) {
		if (SeeIfThisPlayerWon(playerLastPlaced, playArea))
		{
			return static_cast<WinState>(playerLastPlaced);
		}
		else if (GetEmptySquaresAmount(playArea) == 0)
		{
			return WinState::TIE;
		}
		else
		{
			return WinState::CONTINUE;
		}
	}

	bool SeeIfThisPlayerWon(SquareOccupied playerToLookFor, const Square playArea[9]) {
		for (int x = 0; x < 3; ++x) {
			if (CheckVertical(playerToLookFor, playArea, x)) return true;
		}
		for (int y = 0; y < 3; ++y) {
			if (CheckHorizontal(playerToLookFor, playArea, y)) return true;
		}
		return CheckDiagonal(playerToLookFor, playArea);
	}

	int GetEmptySquaresAmount(const Square playArea[9]) {
		int amount = 0;
		for (int i = 0; i < 9; ++i) {
			if (playArea[i].squareStatus == SquareOccupied::EMPTY) {
				amount++;
			}
		}
		return amount;
	}

	std::vector<int> GetEmptySquareIDs(const Square playArea[9]) {
		std::vector<int> ids;
		for (int i = 0; i < 9; ++i) {
			if (playArea[i].squareStatus == SquareOccupied::EMPTY) {
				ids.emplace_back(i);
			}
		}
		return ids;
	}

	bool CheckHorizontal(SquareOccupied playerToLookFor, const Square playArea[9], int yID) {
		for (int x = 0; x < 3; ++x) {
			if (playArea[x + yID * 3].squareStatus != playerToLookFor) {
				return false;
			}
		}
		return true;
	}

	bool CheckVertical(SquareOccupied playerToLookFor, const Square playArea[9], int xID) {
		for (int y = 0; y < 3; ++y) {
			if (playArea[xID + y * 3].squareStatus != playerToLookFor) {
				return false;
			}
		}
		return true;
	}

	bool CheckDiagonal(SquareOccupied playerToLookFor, const Square playArea[9]) {
		bool firstDiagonalStatus = true;
		for (int x = 0; x < 3; ++x) {
			int y = x;
			if (playArea[x + y * 3].squareStatus != playerToLookFor) {
				firstDiagonalStatus = false;
			}
		}
		if (firstDiagonalStatus) return true;
		for (int x = 2; x >= 0; --x) {
			int y = 2 - x;
			if (playArea[x + y * 3].squareStatus != playerToLookFor) {
				return false;
			}
		}
		return true;
	}




};

#endif // !AI_PLAYER

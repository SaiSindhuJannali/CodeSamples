/* AMAZONS GAME */

#include<stdlib.h>
#include<iostream>
#include<string>
#include<filesystem>
#include<vector>
#include<tuple>

using namespace std;
#define ROW_COUNT 10
#define COL_COUNT 10

enum PLAYER
{
	LEFT = 1,
	RIGHT
};

// Move --> <Player,player pos,tomove pos, cross pos, Before matrix, after matrix>
typedef struct Move_Variables
{
	int player;
	int playerX, playerY;
	int to_PlayerX, to_PlayerY;
	int x_X, x_Y;
	int before[10][10];
	int after[10][10];
}Move;

class Board
{
	
public:
	bool isEnd = false;
	int playerTurn = LEFT;
	int nextPlayer = RIGHT;
	//int boardState[ROW_COUNT][COL_COUNT];
	int board[ROW_COUNT][COL_COUNT];
	void printBoad();
	void play(bool);
	int checkPossibleMoves(int, int boardState[ROW_COUNT][COL_COUNT]);
	vector<Move> listMoves(int boardState[ROW_COUNT][COL_COUNT]);
	Move nextMove();
	Move GenerateMove(int, int, int, int, int, int);
	bool validateMove(Move move);
	//vector<Move> checkForArrowPositions(int x0, int y0, int x1, int y1);
};

void Board::printBoad()
{
	cout << "Board state after move by player: "<< playerTurn << endl;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; j++)
		{
			switch (board[i][j])
			{
			case 0:
				cout << "- ";
				break;

			case 1:
				cout << "B ";
				break;

			case 2:
				cout << "W ";
				break;

			case 3:
				cout << "X ";
				break;

			default:
				break;
			}
		}
		cout << endl;
	}
}



Move Board::GenerateMove(int x0, int y0, int x1, int y1, int cx, int cy)
{
	Move move;
	move.player = playerTurn;
	move.playerX = x0;
	move.playerY = y0;
	move.to_PlayerX = x1;
	move.to_PlayerY = y1;
	move.x_X = cx;
	move.x_Y = cy;
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int j = 0; j < COL_COUNT; j++)
		{
			move.before[i][j] = board[i][j];
			move.after[i][j] = board[i][j];
		}
	}
	move.after[x0][y0] = 0;
	move.after[x1][y1] = playerTurn;
	move.after[cx][cy] = 3;

	return move;
}

void Board::play(bool selfPlay)
{

	while (!isEnd)
	{
		
		int scope1 = checkPossibleMoves(playerTurn, board);
		int scope2 = checkPossibleMoves(nextPlayer, board);

		if (scope1 > 0)
		{
			Move bestMove;
			if (selfPlay && playerTurn == 1)
			{
				bool validMove = false;
				do
				{
					Move playerMove;
					int x0, y0, x1, y1, x2, y2;
					//get the values from the player
					cout << "Enter which coin you want to move: (in x <enter key> y format)";
					cin >> x0 >> y0;
					cout << endl << "Enter where to move the coin (in x <enter key> y format)";
					cin >> x1 >> y1;
					cout << endl << "Enter where to place the cross(X) (in x <enter key> y format)";
					cin >> x2 >> y2;

					playerMove = GenerateMove(x0, y0, x1, y1, x2, y2);
					validMove = validateMove(playerMove);
					if (!validMove)
						cout << endl << "Your move is Invalid! Re enter..." << endl;
					else
						bestMove = playerMove;
				} while (!validMove);
			}
			else
			{
				 bestMove = nextMove();
			}
			
			for (int i = 0; i < ROW_COUNT; i++)
			{
				for (int j = 0; j < COL_COUNT; j++)
				{
					board[i][j] = bestMove.after[i][j];
				}
			}
			printBoad();

			int temp = playerTurn;
			playerTurn = nextPlayer;
			nextPlayer = temp;
		}
		else
		{
			isEnd = true;
		}
	}
}

bool Board::validateMove(Move move)
{
	if (move.before[move.to_PlayerX][move.to_PlayerY] != 0 || 
		move.before[move.playerX][move.playerY] != move.player)
	{
		return false;
	}
	else
	{
		int x_offset = 0, y_offset = 0;
		if (move.before[move.x_X][move.x_Y] == 0 || (move.x_X == move.playerX && move.x_Y == move.playerY))
		{
			//check if there are any hurdles midway from start pos to end pos
			if (move.playerX > move.to_PlayerX)
			{
				x_offset = -1;
			}
			else
			{
				x_offset = 1;
			}

			if (move.playerY > move.to_PlayerX)
			{
				y_offset = -1;
			}
			else
			{
				y_offset = 1;
			}

			for (int i = move.playerX + x_offset, j = move.playerY + y_offset;
				(i >= 0 && i < ROW_COUNT && j >= 0 && j < COL_COUNT);
				i += x_offset, j += y_offset)
			{
				if (move.before[i][j] != 0)
					return false;
			}
		}
			
		else
			return false;
	}
}

int Board::checkPossibleMoves(int player,int boardState[ROW_COUNT][COL_COUNT])
{
	int retVal = 0;
	int x_offset = 0, y_offset = 0;


	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int j = 0; j < COL_COUNT; j++)
		{
			//Check all 8 directions
			/*
			up : same y inc x
			down : same y dec x
			Left : same x dec y
			Right: same x inc y
			Diag:
				inc x and y
				dec x and y
				inc x and dec y
				dec x and inc y
			*/
			if (boardState[i][j] == player)
			{

				//  up 
				x_offset = -1;
				for (int x = i+x_offset; x < ROW_COUNT && x >= 0; x += x_offset)
				{
					if (boardState[x][j] == 0)
					{
						retVal++;
					}
					else break;
				}

				x_offset = 1;//down
				for (int x = i+x_offset; x < ROW_COUNT && x >= 0; x += x_offset)
				{
					if (boardState[x][j] == 0)
					{
						retVal++;
					}
					else break;
				}

				y_offset = -1;//left
				for (int x = j+y_offset; x < COL_COUNT && x >= 0; x += y_offset)
				{
					if (boardState[i][x] == 0)
					{
						retVal++;
					}
					else break;
				}

				y_offset = 1;//right
				for (int x = j+y_offset; x < COL_COUNT && x >= 0; x += y_offset)
				{
					if (boardState[i][x] == 0)
					{
						retVal++;
					}
					else break;
				}

				x_offset = 1; y_offset = -1; //down-left
				for (int x = i+x_offset, y = j+y_offset; (x < COL_COUNT && x >= 0) && (y < COL_COUNT && y >= 0); x += x_offset, y += y_offset)
				{
					if (boardState[x][y] == 0)
					{
						retVal++;
					}
					else break;
				}

				x_offset = 1; y_offset = 1;//down-right
				for (int x = i+x_offset, y = j+y_offset; (x < ROW_COUNT && x >= 0) && (y < COL_COUNT && y >= 0); x += x_offset, y += y_offset)
				{
					if (boardState[x][y] == 0)
					{
						retVal++;
					}
					else break;
				}

				x_offset = -1; y_offset = 1;//up-right
				for (int x = i+x_offset, y = j+y_offset; (x < ROW_COUNT && x >= 0) && (y < COL_COUNT && y >= 0); x += x_offset, y += y_offset)
				{
					if (boardState[x][y] == 0)
					{
						retVal++;
					}
					else break;
				}

				x_offset = -1; y_offset = -1;//upleft
				for (int x = i + x_offset, y = j + y_offset; (x < ROW_COUNT && x >= 0) && (y < COL_COUNT && y >= 0); x += x_offset, y += y_offset)
				{
					if (boardState[x][y] == 0)
					{
						retVal++;
					}
					else break;
				}
			}
		}
	}

	return retVal;
}


vector<Move> Board::listMoves(int boardState[ROW_COUNT][COL_COUNT])
{
	vector<Move> retVal;
	int x_offset = 0, y_offset = 0;
	int cx_offset = 0, cy_offset = 0;
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int j = 0; j < COL_COUNT; j++)
		{
			if (boardState[i][j] == playerTurn)
			{
				x_offset = -1; y_offset = 0;
				for (int x = i + x_offset, y = j + y_offset; boardState[x][y] == 0 && x < ROW_COUNT && x >= 0; x += x_offset)
				{
					//do a recursive check for 8 directions for arrows and push each possible move to the list
					//up
					cx_offset = -1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx >= 0; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx < ROW_COUNT; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy < COL_COUNT; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy >= 0; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
					cx_offset = 1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
				}

				x_offset = 1; y_offset = 0;
				for (int x = i + x_offset, y = j + y_offset; boardState[x][y] == 0 && x < ROW_COUNT && x >= 0; x += x_offset)
				{
					//do a recursive check for 8 directions for arrows and push each possible move to the list
					//up
					cx_offset = -1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx >= 0; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx < ROW_COUNT; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy < COL_COUNT; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy >= 0; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
					cx_offset = 1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
				}

				x_offset = 0; y_offset = -1;
				for (int x = i + x_offset, y = j + y_offset; boardState[x][y] == 0 && y < ROW_COUNT && y >= 0; y += y_offset)
				{
					//do a recursive check for 8 directions for arrows and push each possible move to the list
					//up
					cx_offset = -1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx >= 0; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx < ROW_COUNT; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy < COL_COUNT; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy >= 0; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
					cx_offset = 1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
				}

				x_offset = 0; y_offset = 1;
				for (int x = i + x_offset, y = j + y_offset; boardState[x][y] == 0 && y < ROW_COUNT && y >= 0; y += y_offset)
				{
					//do a recursive check for 8 directions for arrows and push each possible move to the list
					//up
					cx_offset = -1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx >= 0; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx < ROW_COUNT; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy < COL_COUNT; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy >= 0; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
					cx_offset = 1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
				}


				x_offset = 1; y_offset = -1;
				for (int x = i + x_offset, y = j + y_offset;
					boardState[x][y] == 0 && x < ROW_COUNT && x >= 0 && y < COL_COUNT && y >= 0;
					x += x_offset, y += y_offset)
				{
					//do a recursive check for 8 directions for arrows and push each possible move to the list
					//up
					cx_offset = -1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx >= 0; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx < ROW_COUNT; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy < COL_COUNT; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy >= 0; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
					cx_offset = 1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
				}


				x_offset = 1; y_offset = 1;
				for (int x = i + x_offset, y = j + y_offset;
					boardState[x][y] == 0 && x < ROW_COUNT && x >= 0 && y < COL_COUNT && y >= 0;
					x += x_offset, y += y_offset)
				{
					//do a recursive check for 8 directions for arrows and push each possible move to the list
					//up
					cx_offset = -1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx >= 0; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx < ROW_COUNT; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy < COL_COUNT; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy >= 0; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
					cx_offset = 1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
				}


				x_offset = -1; y_offset = 1;
				for (int x = i + x_offset, y = j + y_offset;
					boardState[x][y] == 0 && x < ROW_COUNT && x >= 0 && y < COL_COUNT && y >= 0;
					x += x_offset, y += y_offset)
				{
					//do a recursive check for 8 directions for arrows and push each possible move to the list
					//up
					cx_offset = -1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx >= 0; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx < ROW_COUNT; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy < COL_COUNT; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy >= 0; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
					cx_offset = 1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
				}

				x_offset = -1; y_offset = -1;
				for (int x = i + x_offset, y = j + y_offset;
					boardState[x][y] == 0 && x < ROW_COUNT && x >= 0 && y < COL_COUNT && y >= 0;
					x += x_offset, y += y_offset)
				{
					//do a recursive check for 8 directions for arrows and push each possible move to the list
					//up
					cx_offset = -1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx >= 0; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 0;
					for (int cx = x + cx_offset, cy = y + cy_offset; cx < ROW_COUNT; cx += cx_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy < COL_COUNT; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 0; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset; cy >= 0; cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = -1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx >= 0 && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = 1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy < COL_COUNT;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}

					cx_offset = 1; cy_offset = -1;
					for (int cx = x + cx_offset, cy = y + cy_offset;
						cx < ROW_COUNT && cy >= 0;
						cx += cx_offset, cy += cy_offset)
					{
						if (boardState[cx][cy] == 0 || (cy == j && cx == i))
						{
							retVal.push_back(GenerateMove(i, j, x, y, cx, cy));
						}
						else break;
					}
				}
			}
		}
	}
	return retVal;
}

Move Board::nextMove()
{
	//for each move calculate the scope and select the move with best scope
	vector<int> scope, maxscope;
	vector<Move> possible_moves = listMoves(board);
	int numMoves = possible_moves.size();
	for (Move state : possible_moves)
	{
		scope.push_back(checkPossibleMoves(nextPlayer,state.after));
	}

	//get the moves that have min scope for next player
	int minScope = *min_element(scope.begin(), scope.end());

	vector<Move> best_Moves_possible;

	for (int i = 0; i < numMoves; i++)
	{
		if (scope[i] == minScope)
		{
			best_Moves_possible.push_back(possible_moves[i]);
		}
	}

	//get  the best move in the min moves 
	//i.e., the move that maximises the scope for current player

	for (Move state : best_Moves_possible)
	{
		maxscope.push_back(checkPossibleMoves(playerTurn,state.after));
	}
	int maxScope = *max_element(maxscope.begin(), maxscope.end());

	for (int i = 0; i < maxscope.size(); i++)
	{
		if (maxscope[i] == maxScope)
		{
			Move next = best_Moves_possible[i];
			return next;
		}
	}
}

#include <cstdio> /* sscanf */
int main()
{
	string filename;
	FILE *read_file, *write_file;
	filename = "StartBoard.txt";
	cout << "Enter the initial board : ";
	cin >> filename;
	Board b;

	//read board data from file
	fopen_s(&read_file, filename.c_str(), "r");
	fopen_s(&write_file, "EndBoard.txt", "w");

	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int j = 0; j < COL_COUNT; j++)
		{
			fscanf_s(read_file, "%i", &b.board[i][j]);
		}
	}

	char selfPlay = 'Y';
	cout << "Do you want to the computer to play the game?";
	cin >> selfPlay;

	//Play game
	if (selfPlay == 'N' || selfPlay == 'n')
		b.play(true);
	else
		b.play(false);

	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int j = 0; j < COL_COUNT; j++)
		{
			fprintf_s(write_file, "%i ", b.board[i][j]);
		}
		fprintf_s(write_file, "\n");
	}

	fclose(read_file);
	fclose(write_file);

	return 0;
}

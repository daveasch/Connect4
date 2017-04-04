#include "board.h"


Board::Board() {
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			data[i][j] = Empty;
		}
	}
	nextPlayer_to_move = Player1;
}


Board::Board(const string &fen) {
	int k = 0;
	int oneASCII = 49;
	int sevenASCII = 55;
	char diffASCIIandNum = 48;
	int j = 0;
	int i = 0;			
	while (i < NUM_ROWS) {
		while (j < NUM_COLS) {
			if (fen[k] == '/'){
				i++;
				j = 0;
			}
			else if (fen[k] == PLAYER1_TOKEN){
				data[i][j] = Player1;
				j++;
			}
			else if (fen[k] == PLAYER2_TOKEN){
				data[i][j] = Player2;
				j++;
			}
			else if (fen[k] >= oneASCII && fen[k] <= sevenASCII){
				int empties = (int)fen[k] - diffASCIIandNum;
				for (int l = j; l < (j + empties); l++){
					data[i][l] = Empty;
				}
				j = j + empties;
			}
			k++;
		}
	}					
}

void Board::printBoard_as_FENstring(ostream &os) const {
	int count = 0;
	for (int i = 0; i < NUM_ROWS; i++){	
		for (int j = 0; j < NUM_COLS; j++){	
			if (data[i][j] == Player1) {
				if (count > 0){
					os << count;
				}
				os << PLAYER1_TOKEN;
				int count = 0;
			}
			if (data[i][j] == Player2){
				if (count > 0){
					os << count;
				}
				os << PLAYER2_TOKEN;
				int count = 0;
				
			}
			if (data[i][j] != Player1 && data[i][j] != Player2){ 		
				count ++;
			}
		}
	os << '/';
	}
	os << ' ';
	if (nextPlayer_to_move == Player1){
		os << PLAYER1_TOKEN;
	}
	else if (nextPlayer_to_move == Player2){
		os << PLAYER2_TOKEN;
	}
}



PieceType Board::atLocation(int row, int col) {
    return data[row][col];
}


void Board::prettyPrintBoard(ostream &os) const {
	os << endl;
    for (int row = NUM_ROWS - 1; row >= 0; row--)  {
        os << "     +---+---+---+---+---+---+---+"
           << endl
           << "    ";
        
        for (int col = 0; col < NUM_COLS; col++)   {
            os << " | " ;
            if ( data[row][col] == Player1)
                os <<  PLAYER1_TOKEN;
            else if (data[row][col] == Player2)
                os << PLAYER2_TOKEN;
            else
                os << EMPTY_TOKEN;
        }
        os << " |" << endl;
    }
    os << "     +---+---+---+---+---+---+---+" << endl
       <<"  col  1   2   3   4   5   6   7" << endl;
    return;
}


int Board::toMove() const {
    return nextPlayer_to_move;
}

Result Board::makeMove(int col) {
	//checks if the row is valid
	if(getFirstFreeRow(col) == NUM_ROWS) {
		return IllegalMove;
	}
    if (col < 0 || col >= NUM_COLS){
        return IllegalMove;
    }
	
	int row = getFirstFreeRow(col);
	
	
	//updateToMove returns the current player and updates the player after
	//this is a very convinient function. Great idea! :)
	data[row][col] = updateToMove(); 	
	
	if(isBoardFull()){
		return Draw;
	}
	if(isWin(row, col)){
		return Win;
	}
	
    return NoResult;
}


int Board::getFirstFreeRow(int col) const {
    for (int i= 0; i < NUM_ROWS; i++){		
	    if (data[i][col] == Empty){
		return i;
	    }
    }
    return NUM_ROWS;
}


PieceType Board::updateToMove() {
    if (nextPlayer_to_move == Player1){
	    nextPlayer_to_move = Player2;
	    return Player1;
    }
    if (nextPlayer_to_move == Player2){
	    nextPlayer_to_move = Player1;
	    return Player2;
    }
}


bool Board::isBoardFull() const {
    for (int i = 0; i < NUM_ROWS; i++){			//same i problem in the for loop. Doesn't really matter here though
	    for (int j = 0; j < NUM_COLS; j++){
		    if ((data[i][j] == Player1)		    //I think you mean if data[i][j] != EMPTY return false
		    || (data[i][j] == Player2)){
			    return false;
		    }
	    }
    }
     return true;
}


bool Board::inBounds(int row, int col) const {
    if ((row < NUM_ROWS && row >= 0)
	&& (col < NUM_COLS && col >= 0)){
	    return true;
    }
    return false;
}

int Board::piecesInDirection(int row, int col, int dRow, int dCol) const {	
	int howManyInARow = 0;
	while(inBounds(row, col)){
		if(data[row][col] == data[row+dRow][col+dCol]){
			howManyInARow++;
		}
		//update the row and column. The row is increased by dRow
		// and the column is increased by dCol
		row = row + dRow;
		col = col + dCol;
	}
    return howManyInARow;
}

bool Board::isWin(int row, int col) const {
	//vertical win
	if(piecesInDirection(row, col, 1, 0) == NUM_FOR_WIN || piecesInDirection(row, col, -1, 0) == NUM_FOR_WIN) {
		return true;
	}
	//horizontal win
	if(piecesInDirection(row, col, 0, 1) == NUM_FOR_WIN || piecesInDirection(row, col, 0, -1) == NUM_FOR_WIN) {
		return true;
	}
	//diagonal win
	if(piecesInDirection(row, col, 1, 1) == NUM_FOR_WIN || piecesInDirection(row, col, -1, -1) == NUM_FOR_WIN) {
		return true;
	}
    return false;
}



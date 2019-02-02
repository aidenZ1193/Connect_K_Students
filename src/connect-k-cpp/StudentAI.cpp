#include "StudentAI.h"
#include <stdlib.h>

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int k,int g)
	:AI(col, row, k, g)
{
	my_board = Board(col, row, k, g);
	min_col = col/2-2;
	max_col = col/2+2;
	min_row = row/2-2;
	max_row = row/2+2;
}

Move StudentAI::GetMove(Move board)
{
	if (this->g == 0)
		return Move(rand() % (col - 1), rand() % (row - 1));
	else
		return Move(rand() % (col - 1), 0);
}

// @return: board's evaluation score
// @usage: emulate AI's move and the following moves
int StudentAI::max_move(pair<int, int> &spot, int depth){

}

// @return: evaluation score of the line
// @usage：based on the line'e empty spots and number of same color pieces
int StudentAI::score_dict(int empty, int pieces, int k){
	if(pieces >= k) return pow(10, k);
	k--;
	while(k > 0){
		if(pieces == k){
			if(empty >= 2) return pow(10, k);
			else if(empty == 1) return pow(10, k-1);
		} 
		k--;
	}
	return 0;
}

// @usage: find all valid moves in the board (within constrains)
void StudentAI::find_empty(vector<pair<int, int> > &valid){
	for(int i = min_row; i < max_row; i++){
		for(int j = min_col; j < max_col; j++ ){
			if(my_board.board[i][j] == 0) valid.push_back(make_pair(i, j));
		}
	}
}



// @return: score for the whole board
int StudentAI::evaluate_board(){

}
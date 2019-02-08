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

// oponent: 2
// AI: 1
// this function works as the first round's max
Move StudentAI::GetMove(Move board)
{
	/*
	if (this->g == 0)
		return Move(rand() % (col - 1), rand() % (row - 1));
	else
		return Move(rand() % (col - 1), 0);
	*/
	Move m;
	// 1. eveluate board, put move in board
	if(board.col == -1 && board.row == -1){
		// start first: return the spot in the middle
		my_board.board[my_board.row/2][my_board.col/2] = 1;
		m.row = my_board.row/2;
		m.col = my_board.col/2;
		return m;
	}

	// 2. generate all points
	// stores all valid spots
	vector<pair<int, int> > valid;
	// stores all usable spots
	vector<pair<int, int> > usable;
	find_empty(valid);
	int len = valid.size();
	int max_score = 0, depth = 4;	// max - min - max- min - max
	for(int i = 0; i < len; i++){
		// change color in the spot to 1, which is the AI's move
		my_board.board[valid[i].first][valid[i].second] = 1;

		// perform min-max in here for each possible move
		int temp = min_move(valid[i], depth-1);
		// check the score for valid[i]
		if(temp > max_score ){
			usable.clear();
			usable.push_back(valid[i]);
			max_score = temp;
		}else if(temp == max_score)
			usable.push_back(valid[i]);

		// change it back
		my_board.board[valid[i].first][valid[i].second] = 0;
	}
	int i = 0;
	if(usable.size() > 1){
		int i = (int)(rand ()% usable.size());
	}else
		i = 0;
	m.row = usable[i].first;
	m.col = usable[i].second;

	my_board.board[usable[i].first][usable[i].second] = 1;
	
	return m;

}

// @return: board's evalutation score
// @usage: emulate component's move and the following moves
int StudentAI::min_move(pair<int, int> &spot, int depth){
	int score = evaluate_board(2);
	int win = my_board.IsWin();
	if(win != 0 || depth == 0){
		return score;
	}

	vector<pair<int, int> > valid;
	vector<pair<int, int> > usable;
	find_empty(valid);

	int len = valid.size();
	int min_score = pow(10, my_board.k+1);
	for(int i = 0; i < len; i++){
		my_board.board[valid[i].first][valid[i].second] = 2;
		int temp = max_move(valid[i], depth-1);
		if(temp < min_score){
			usable.clear();
			usable.push_back(valid[i]);
			min_score = temp;
		}else if(temp == min_score)
			usable.push_back(valid[i]);

		my_board.board[valid[i].first][valid[i].second] = 0;
	}
	return min_score;
}


// @return: board's evaluation score
// @usage: emulate AI's move and the following moves
int StudentAI::max_move(pair<int, int> &spot, int depth){

	// check for end game
	int score = evaluate_board(1);	// i = AI's turn
	int win = my_board.IsWin();
	// tie/someone wins/reached max depth
	if(win != 0 || depth == 0){
		return score;
	}

	vector<pair<int, int> > valid;
	vector<pair<int, int> > usable;
	find_empty(valid);

	int len = valid.size();
	int max_score = 0;
	// from all points in valid[][], perform min-max search with depth
	for(int i = 0; i < len; i++){
		my_board.board[valid[i].first][valid[i].second] = 1;
		int temp = min_move(valid[i], depth);
		if(temp > max_score){
			usable.clear();
			usable.push_back(valid[i]);
			max_score = temp;
		}else if(temp == max_score)
			usable.push_back(valid[i]);

		my_board.board[valid[i].first][valid[i].second] = 0;		
	}
	return max_score;
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
	// disable gravity
	if(my_board.g == 0){
		for(int i = min_row; i < max_row; i++){
			for(int j = min_col; j < max_col; j++ ){
				if(my_board.board[i][j] == 0) 
					valid.push_back(make_pair(i, j));
			}
		}
	}else{
		// enabled gravity, pick from lowest in each column
		for(int i = min_col; i < max_col; i++ ){
			for(int j = 0; i < max_row; j--){
				if(my_board.board[j][i] == 0){
					// pick only lowest one in each row
					valid.push_back(make_pair(j, i));
					break;
				}
			}
		}
	}
}

// @return true for break, false for continue
// helper function for evaluate_single_space
int StudentAI::count_piece(int &empty, int &same, int turn, int i, int j){
	//bool res = false;
	if(my_board.board[i][j] == 0)
		empty++;
	else if(my_board.board[i][j] == turn)
		same++;
	else return true;
	return false;
}

// @return evaluation score for the empty spot valid[i][j]
// @usage: evaluate all four directions for the space
// need to go through all eight directions, stop when seeing another color's piece
int StudentAI::evaluate_single_space(int i, int j, int turn){
	// evaluation score for the single space
	int sum = 0; 	
	// empty spots and pieces with same color
	int empty = 0, same = 0;

	// 1. horizontal, left and right
	for(int y = j; y < max_col; y++){
		if(count_piece(empty, same, turn, i, y))
			break;
	}
	for(int y = j; y > min_col; y--){
		if(count_piece(empty, same, turn, i, y))
			break;
	}
	sum += score_dict(empty, same, my_board.k);
	empty = same = 0;

	// 2. vertical, up and down
	for(int x = i; x < max_row; x++){
		if(count_piece(empty, same, turn, x, j))
			break;
	}
	for(int x = i; x > min_row; x--){
		if(count_piece(empty, same, turn, x, j))
			break;
	}
	sum += score_dict(empty, same, my_board.k);
	empty = same = 0;

	// 3. 45 degree left to right, bottom to top
	for(int x = i, y = j; x > min_row && y < max_col; x --, y++){
		if(count_piece(empty, same, turn, x, y))
			break;
	}
	for(int x = i, y = j; x < max_row && y > min_col; x++, y--){
		if(count_piece(empty, same, turn, x, y))
			break;
	}
	sum += score_dict(empty, same, my_board.k);
	empty = same = 0;

	// 4. 135 degree left to right, top to bottom
	for(int x = i, y = j; x > min_row && y > min_col; x--, y--){
		if(count_piece(empty, same, turn, x, y))
			break;
	}
	for(int x = i, y = j; x < max_row && y < max_col; x++, y++){
		if(count_piece(empty, same, turn, x, y))
			break;
	}
	sum += score_dict(empty, same, my_board.k);
	return sum;
}

// @return: score for the whole board
int StudentAI::evaluate_board(int turn){
	int sum = 0;

	for(int i = 0; i < my_board.row; i++){
		for(int j = 0; j < my_board.col; j++){
			sum += evaluate_single_space(i, j, turn);
		}
	}
	return sum;
}
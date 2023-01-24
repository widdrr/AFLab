#include <fstream>
#include <vector>

using namespace std;

/*
 *
 *		Bonus 2) X Si O
 *
 *			Parcurgand DFS graphul tuturor starilor jocului, se poate determina cine castiga astfel:
 *		Pentru o stare:
 *		1.Daca este randul lui x:
 *		  -daca oricare din starile vecine(toate miscarile posibile) duce la victorie, atunci este victorie
 *		  -daca cel putin una duce la egalitate si niciuna nu duce la victorie, este egalitate
 *		  -daca toate starile duc la infrangere, este infrangere
 *		2.Daca este randul lui o este la fel dar inversat, deoarece
 *		  rezultatele sunt din perspectiva lui X
 *
 *			De asemenea, putem stoca pentru fiecare stare care este rezultatul deoarece acesta nu se va schimba.
 *		Pentru asta aplicam un hash pe stare (numar in baza 3) si il folosim drept indice.
 *
 *			Deoarece este suficient sa facem o singura parcurgere completa (care poate fi si preprocesata)
 *		Complexitatea e practic O(1). Parcurgerea in sine e O(N + M), cat sunt M si N?.. nu e un calcul usor 
 *
 */

enum outcomes{
	win,
	lose,
	draw,
	invalid
};

vector<outcomes> cache(60000,invalid);

struct state {
	vector<bool> x_pos;
	vector<bool> o_pos;
	int moves;
	int id;

	state() : x_pos(9), o_pos(9), moves(),id() {}
	state(const state& st) : x_pos(st.x_pos), o_pos(st.o_pos), moves(st.moves),id() {}
	void recomputeId()
	{
		id = 0;
		for(int i=0; i < 9; ++i)
		{
			id *= 3;
			if (x_pos[i])
				id += 1;
			if (o_pos[i])
				id += 2;
		}
	}
};

bool victory(const vector<bool>& pos){
	return  (pos[0] && pos[1] && pos[2]) ||  //line1
			(pos[3] && pos[4] && pos[5]) ||  //line2
			(pos[6] && pos[7] && pos[8]) ||  //line3
			(pos[0] && pos[3] && pos[6]) ||  //column1
			(pos[1] && pos[4] && pos[7]) ||  //column2
			(pos[2] && pos[5] && pos[8]) ||  //column3
			(pos[0] && pos[4] && pos[8]) ||  //diagonal1
			(pos[2] && pos[4] && pos[6]);    //diagonal2
}

outcomes simulate_game(const state& s){

	if (cache[s.id] != invalid)
		return cache[s.id];


	if (victory(s.x_pos)) {
		cache[s.id] = win;
		return win;
	}

	if (victory(s.o_pos)){
		cache[s.id] = lose;
		return lose;
	}

	if (s.moves == 9){
		cache[s.id] = draw;
		return draw;
	}

	if (s.moves % 2 == 0) {
		cache[s.id] = lose;

		for (int pos = 0; pos < 9; ++pos) {
			if (!s.x_pos[pos] && !s.o_pos[pos]) {

				state new_state = s;
				new_state.x_pos[pos] = true;
				++new_state.moves;
				new_state.recomputeId();

				const auto pot_res = simulate_game(new_state);

				if (pot_res == win) {
					cache[s.id] = win;
					//return win;
					//for some reason returning early breaks it... still don't know why
				}

				if (cache[s.id] == lose && pot_res == draw) {
					cache[s.id] = draw;
				}
			}
		}
	}
	else{
		cache[s.id] = win;

		for (int pos = 0; pos < 9; ++pos) {
			if (!s.x_pos[pos] && !s.o_pos[pos]) {

				state new_state = s;
				new_state.o_pos[pos] = true;
				++new_state.moves;
				new_state.recomputeId();

				const auto pot_res = simulate_game(new_state);

				if (pot_res == lose) {
					cache[s.id] = lose;
					//return lose;
					//see above comment
				}

				if (cache[s.id] == win && pot_res == draw) {
					cache[s.id] = draw;
				}
			}
		}
	}
	return cache[s.id];

}

// int main(){
// 	ifstream in("xsizero.in");
// 	ofstream out("xsizero.out");
// 	char token;
// 	int pos = 0;
// 	int x_cnt = 0;
// 	int o_cnt = 0;
// 	int play_count = 0;
// 	state board;
//
// 	while(in>>token){
//
// 		switch(token){
//
// 			case 'X':
// 				board.x_pos[pos] = true;
// 				board.id = board.id * 3 + 1;
// 				++x_cnt;
// 				break;
// 			case '0':
// 				board.o_pos[pos] = true;
// 				board.id = board.id * 3 + 2;
// 				++o_cnt;
// 				break;
// 			case '.':
// 				board.x_pos[pos] = board.o_pos[pos] = false;
// 				board.id = board.id * 3;
// 				break;
// 			default:
// 				break;
// 		}
// 		++pos;
// 		if (pos == 9) {
// 			++play_count;
// 			board.moves = x_cnt + o_cnt;
// 			out << "Testul #" << play_count << ": ";
//
// 			if(x_cnt - o_cnt == 1 || x_cnt-o_cnt == 0){
//
// 				auto result = simulate_game(board);
// 				switch(result){
// 					case draw:
// 						out << "draw\n";
// 						break;
// 					case win:
// 						out << "win\n";
// 						break;
// 					case lose:
// 						out << "lose\n";
// 						break;
// 					case invalid:
// 						//never gets here
// 						break;
// 				}
// 			}
// 			else{
// 				out << "invalid\n";
// 			}
// 			board.id = 0;
// 			pos = 0;
// 			x_cnt = 0;
// 			o_cnt = 0;
// 		}
// 	}
// 	out.close();
// 	in.close();
//
// 	return 0;
// }
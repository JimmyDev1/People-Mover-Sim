#include <string>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

#define TRACKS 2
#define SEG_PER_TRACK 10
#define NTRAIN 4
#define SIMTIME 50

void init_tracks(struct Segment* segments[TRACKS][SEG_PER_TRACK]); // intializes each segment array of the track with 0 and places the trains randomly
void connect(struct Segment* segments[TRACKS][SEG_PER_TRACK]); //sets each segment iteration in the array to hold the past and future train locations
void move_train(struct Segment* segments[TRACKS][SEG_PER_TRACK]); // moves train ccw or cw and changes track to avoid crash 
void print_tracks(struct Segment* segments[TRACKS][SEG_PER_TRACK]); // prints the tracks


// this structure holds the current train location and past and future locations
struct Segment {

	int train; //-1 ccw 0 none 1 cw

	Segment* next;
	Segment* prev;
	Segment* next_adj;
	Segment* prev_adj;




};

int main() {

	struct Segment* segments[TRACKS][SEG_PER_TRACK]; // creating the segments of the track in an array

	init_tracks(segments);
	connect(segments);


	for(int i = 0; i < SIMTIME; i++) {

		print_tracks(segments);
		move_train(segments);

	}

	return 0;

}


void init_tracks(struct Segment* segments[TRACKS][SEG_PER_TRACK] ){

	for (int i = 0; i < TRACKS; i++) {

		for (int j = 0; j < SEG_PER_TRACK; j++) {

			segments[i][j] = new Segment;


		}

	}
	
	srand(time(0)); 

	for (int i = 0; i < TRACKS; i++) {

		for (int j = 0; j < SEG_PER_TRACK; j++) {

			segments[i][j]-> train = 0;


		}

	}

	int n = 0;

	while (n < NTRAIN) {

		int t, s, d; // t = track s = segment d = direction

		t = rand() % TRACKS;        //rand() % 2  generates -> 0 or 1 for which track
		s = rand() % SEG_PER_TRACK; //rand() % 10  generates -> 0 - 9 for which segment 
		d = ((rand() % 2) * 2 )- 1; // ( (rand() % 2) * 2 ) - 1 generates -1 or 1 for which direction 

		if (segments[t][s] -> train == 0) {

			segments[t][s] -> train = d;

			n++;

		}


	}


}

void connect(struct Segment* segments[TRACKS][SEG_PER_TRACK]) {

	for (int i = 0; i < TRACKS; i++) {

		for (int j = 0; j < SEG_PER_TRACK; j++) {

			int knext, kprev;

			knext = (j + 1) % SEG_PER_TRACK;
			
			kprev = (j - 1 + SEG_PER_TRACK) % SEG_PER_TRACK;

			segments[i][j]-> next = segments[i][knext];
			segments[i][j]-> prev = segments[i][kprev];

			segments[i][j] -> next_adj = segments[TRACKS - 1 - i][knext];
			segments[i][j] -> prev_adj = segments[TRACKS - 1 - i][kprev];



		}

	}


}


void move_train(struct Segment* segments[TRACKS][SEG_PER_TRACK]) {


	for (int i = 0; i < TRACKS; i++) {

		for (int j = 0; j < SEG_PER_TRACK; j++) {

			struct Segment* t_seg = segments[i][j];

			if (t_seg -> train == 1) { // cw

				if (t_seg -> next -> train == 0) {

					t_seg -> train  = 0;

					t_seg->next->train = 2;

				}
				else if (t_seg -> next_adj -> train == 0) {

					t_seg->train = 0;

					t_seg->next_adj->train = 2;

				}


			}
			else if (t_seg->train == -1) {


				if (t_seg->prev->train == 0) {

					t_seg->train = 0;

					t_seg->prev->train = -2;

				}
				else if (t_seg->prev_adj->train == 0) {

					t_seg->train = 0;

					t_seg->prev_adj->train = -2;


				}


			}


		}

	}


	for (int i = 0; i < TRACKS; i++) {

		for (int j = 0; j < SEG_PER_TRACK; j++) {

			if (segments[i][j]-> train == 2 || segments[i][j]->train == -2) {

				segments[i][j]->train /= 2;

			}


		}

	}


}



void print_tracks(struct Segment* segments[TRACKS][SEG_PER_TRACK]) {


	cout << "+---"; // upper left corner 

	for (int i = 0; i < SEG_PER_TRACK / 2; i++) {

		if (segments[0][i]->train == 0) {

			cout << "---";

		}
		else if (segments[0][i]->train == 1) {

			cout << "->-";

		}
		else {

			cout << "-<-";

		}


	}

	cout << "---+" << endl;

	cout << "|    x  x  x  x  x    |" << endl;

	cout << "|   ";

	for (int i = 0; i < SEG_PER_TRACK / 2; i++) {

		if (segments[1][i]->train == 0) {

			cout << "---";

		}
		else if (segments[1][i]->train == 1) {

			cout << "->-";

		}
		else {

			cout << "-<-";

		}


	}

	cout << "   |" << endl;

	cout << "|   |             |   |" << endl;

	cout << "|   ";

	for (int i = SEG_PER_TRACK - 1; i >= SEG_PER_TRACK / 2; i--) {

		if (segments[1][i]->train == 0) {

			cout << "---";

		}
		else if (segments[1][i]->train == 1) {

			cout << "-<-";

		}
		else {

			cout << "->-";

		}

	}

	cout << "   |" << endl;

	cout << "|    x  x  x  x  x    |" << endl;

	cout << "+---"; // bottom left corner


	// i wrote the code below backwards so that the train makes a little more sense it makes actual circles 
	/*
	 it travels like so 
	 
	 >>>>>>>>  01234
	 |      |
	 <<<<<<<<  98765
	  
	 instead of like this 

	 >>>>>>>>  01234
	 |      |
	 >>>>>>>>  56789
	
	*/

	for (int i = SEG_PER_TRACK - 1; i >= SEG_PER_TRACK / 2; i--) {
	

		if (segments[0][i]->train == 0) {

			cout << "---";

		}
		else if (segments[0][i]->train == 1) {

			cout << "-<-";

		}
		else {

			cout << "->-";

		}
	
	
	}

	cout << "---+" << endl;

}
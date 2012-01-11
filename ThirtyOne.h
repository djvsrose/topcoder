#ifndef THIRTY_ONE_H_INCLUDED
#define THIRTY_ONE_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

class ThirtyOne
{
	enum FACE_VALUE
	{
		CARD_2, CARD_3, CARD_4, CARD_5, CARD_6, CARD_7, CARD_8, CARD_9, CARD_10,
		CARD_J, CARD_Q, CARD_K, CARD_A
	};

	struct Card
	{
		FACE_VALUE value;	

		bool operator< (const Card& other) const
		{
			return value != CARD_A && other.value == CARD_A;
		}
	};

	struct Hand
	{
		Hand(Card c0, Card c1, Card c2) 
		{
			cards[0] = c0;
			cards[1] = c1;
			cards[2] = c2;
		}
		Card cards[3];
	};

private:
	Hand SortCard(const string& hand)
	{
		istringstream iss(hand);
		string fv[3];
		Card cards[3];
		iss >> fv[0] >> fv[1] >> fv[2];

		for (int i = 0; i < 3; ++i) {
			string facevalue = fv[i];
			if (facevalue == "2")
				cards[i].value = CARD_2;
			else if (facevalue == "3")
				cards[i].value = CARD_3;
			else if (facevalue == "4")
				cards[i].value = CARD_4;
			else if (facevalue == "5")
				cards[i].value = CARD_5;
			else if (facevalue == "6")
				cards[i].value = CARD_6;
			else if (facevalue == "7")
				cards[i].value = CARD_7;
			else if (facevalue == "8")
				cards[i].value = CARD_8;
			else if (facevalue == "9")
				cards[i].value = CARD_9;
			else if (facevalue == "10")
				cards[i].value = CARD_10;
			else if (facevalue == "J")
				cards[i].value = CARD_J;
			else if (facevalue == "Q")
				cards[i].value = CARD_Q;
			else if (facevalue == "K")
				cards[i].value = CARD_K;
			else if (facevalue == "A")
				cards[i].value = CARD_A;
		};
		sort(cards, cards+3);
		return Hand(cards[0], cards[1], cards[2]);
	}

	float ParsePoint(const string& hand)
	{
		// first, find out whether we have 'A'
		// Have to treat 'A' in the end because it can be 1 or 11
		Hand h = SortCard(hand);

		if (h.cards[0].value == h.cards[1].value && h.cards[1].value == h.cards[2].value) {
			return 30.5;
		}

		int points = 0;
		for (int i = 0; i < 3; ++i) {
			switch (h.cards[i].value) {
			case CARD_2:
				points += 2;
				break;
			case CARD_3:
				points += 3;
				break;
			case CARD_4:
				points += 4;
				break;
			case CARD_5:
				points += 5;
				break;
			case CARD_6:
				points += 6;
				break;
			case CARD_7:
				points += 7;
				break;
			case CARD_8:
				points += 8;
				break;
			case CARD_9:
				points += 9;
				break;
			case CARD_10:
			case CARD_J:
			case CARD_Q:
			case CARD_K:
				points += 10;
				break;
			case CARD_A:
				points += (points <= 20) ? 11 : 1;
				break;
			};
		}
		return points;
	}

public:
	int findWinner(vector<string> hands)
	{
		int winnerId = -1;
		float maxPoints = 0.f;

		for (unsigned int i = 0; i < hands.size(); ++i) {
			float pnts = ParsePoint(hands[i]);
			if (pnts <= 31 && pnts > maxPoints) {
				winnerId = i;
				maxPoints = pnts;
			}	
		}

		return winnerId;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
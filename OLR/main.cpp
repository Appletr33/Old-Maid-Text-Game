#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <string>

void ClearScreen()
{
	if (system("CLS")) system("clear");
}

void Print_MainMenu()
{
ClearScreen();

printf(" ::::::::       :::             :::::::::  \n");
std::this_thread::sleep_for(std::chrono::milliseconds(500));
printf(":+:    :+:      :+:             :+:    :+: \n");
std::this_thread::sleep_for(std::chrono::milliseconds(500));
printf("+:+    +:+      +:+             +:+    +:+ \n");
std::this_thread::sleep_for(std::chrono::milliseconds(500));
printf("+#+    +:+      +#+             +#++:++#:  \n");
std::this_thread::sleep_for(std::chrono::milliseconds(500));
printf("+#+    +#+      +#+             +#+    +#+ \n");
std::this_thread::sleep_for(std::chrono::milliseconds(500));
printf("#+#    #+#      #+#             #+#    #+# \n");
std::this_thread::sleep_for(std::chrono::milliseconds(500));
printf(" ########       ##########      ###    ### \n");
std::this_thread::sleep_for(std::chrono::milliseconds(3000));

ClearScreen();
}

void Print_Premise()
{
	int garbage;
	std::cout << "OLR is a game similar to the game old maid. The GOAL of this game is to be the first player to have 0 cards. Each player is given a random assortment of cards at the begining of each round. To remove cards from your deck, either another player picks one, or match two cards of the same color and type. To add cards to your card pile, you choose a random card from the next player. DO NOT be the last person with the red card at the end of the game otherwise you lose.";
	std::cout << "\n\n\nTL;DR\n1. Don't be that last one with the red card.\n2. Try and match all your cards.\n3. Each player draws a random card from the next player's deck.";
	std::cout << "\n\n\n";
	std::cout << "Enter \"1\" to continue... ";
	std::cin >> garbage;
	ClearScreen();
	sleep(1);
}

void Dynamic_Type(std::string sentence, int milli = 70)
{
	std::vector <char> letters_vector(sentence.begin(), sentence.end());

	for (int i = 0; i < letters_vector.size(); i++)
	{
		std::cout << letters_vector[i];
		std::cout.flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(milli));
	}
}

struct stats
{
	std::string player_name;
	int matches;

	stats(std::string name, int points)
	{
		player_name = name;
		matches = points;
	}
};

struct card
{
	std::string name;
	int color;
	std::string style; 

	card(std::string card_name, int card_color, std::string card_style)
	{
		name = card_name;
		color = card_color;
		style = card_style;
	}
};

class Player
{
private:
	int points;
	std::map<std::string, int> matches;
	std::vector<card> cards;

public:
	//Constructer
	Player()
	{
		points = 0;
	}

	//Name
	std::string name;
	void Change_Name(std::string);
	//Points
	int Get_Points();
	void Add_Points(int);
	//Cards
	void Remove_Cards(int);
	void Add_Cards(card);
	void Add_Cards_Random(card);
	std::vector<card> Get_Cards();
	//Matches
	void Add_Match(std::string, int);
	std::map<std::string, int> Get_Matches();
};

int Player::Get_Points()
{
	return points;
}

void Player::Add_Points(int point)
{
	points += point;
}

void Player::Remove_Cards(int index)
{
	cards.erase(cards.begin() + index);
}

void Player::Add_Cards(card add_card)
{
	cards.push_back(add_card);
}

void Player::Add_Cards_Random(card add_card)
{
	cards.insert(cards.begin() + rand() % cards.size(), add_card);
}

std::vector<card> Player::Get_Cards()
{
	return cards;
}

std::map<std::string, int> Player::Get_Matches()
{
	return matches;
}

void Player::Add_Match(std::string match, int number)
{
	matches[match] += number; //make += number
}

void Player::Change_Name(std::string player_name)
{
	name = player_name;
}


void Deal_Cards(std::vector<Player> &players, std::vector<std::string> card_names, std::vector<std::string> card_styles)
{
	int num_of_cards = 50; //50

	
	std::map<std::string, int> card_types;
	
	card_types["green"] = 32;
	card_types["cyan"] = 36;
	card_types["yellow"] = 33;
	card_types["blue"] = 34;
	card_types["magenta"] = 35;
	
	
	
	//red = 31
	
	//For players
	int i = 0;


	while (!(num_of_cards == 0))
	{
		int card_name = rand() % card_names.size();
		int card_style = rand() % card_styles.size();
		card rand_card(card_names[card_name], card_types.find(card_names[card_name])->second, card_styles[card_style]);

		players[i].Add_Cards(rand_card);
		players[rand() % players.size()].Add_Cards(rand_card);
		
		if (i + 1 == players.size()) // idk + 1
		{
			i = 0;
		}
			
		else
		{
			i++;
		}
			
		num_of_cards -= 2;
		
	}

	card red("red", 31, "R");
	players[rand() % players.size()].Add_Cards(red);

}

void Check_Matches(std::vector<Player> &players, int player_index, std::string card_name, std::string card_style)
{
	int matches = 0;

	for(int i = 0; i < players[player_index].Get_Cards().size(); i++)
	{
		if (players[player_index].Get_Cards()[i].name == card_name && players[player_index].Get_Cards()[i].style == card_style)
		{
			matches ++;
		}
	}

	if (matches > 1)
	{
		int temp = 0;
		bool found = false;
		int index = 0;

		// Iterate over all elements in Vector
		for (auto & elem : players[player_index].Get_Cards())
		{
			if (elem.name == card_name && elem.style == card_style)
			{
				if (temp >= 1 || matches % 2 != 1)
					found = true;
				temp++;
				index = index; //posibly -1
			}

			if(found)
				{
					//std::cout << players[player_index].Get_Cards()[index].name <<" -->"; //REMOVE LATER
					players[player_index].Remove_Cards(index);
					found = false;
					//std::cout << players[player_index].Get_Cards()[index].name << "\n"; //REMOVE LATER
				}

				else
					index ++;
			}
			players[player_index].Add_Match(card_style + " "+ card_name, ((matches - matches % 2) / 2));
			players[player_index].Add_Points(((matches - matches % 2) / 2));
			
		}	
		//int index; //REMOVE LATER
		//std::cin >> index; //REMOVE LATER
}

void Draw_Player_Menu(std::vector<Player> players, bool choose)
{
	ClearScreen();

	std::cout << "\033[1;31m";
	std::cout << ("_________________________________\n");
	std::cout << ("      __         _         ____  \n");
	std::cout << ("    /    )       /         /    )\n");
	std::cout << ("---/----/-------/---------/___ /-\n");
	std::cout << ("  /    /       /         /    |  \n");
	std::cout << ("_(____/_______/____/____/_____|__\n");
	std::cout << "\033[0m" << "\n";
	std::cout << "Type the number of your desired action bellow:\n" << std::endl;

	std::cout << "1. Match Cards" << std::endl;
	std::cout << "2. Make A Funny Face" << std::endl;
	std::cout << "3. Try and peak at another player's cards" << std::endl;

	if (choose)
		std::cout << "4. End Turn" << std::endl;

	else if (players.size() > 1)
		std::cout << "4. Pick A Card From " << players[1].name << std::endl;

	else
		std::cout << "4. End Turn" << std::endl;

	std::cout << "\n";

}

void Draw_Player_Cards(std::vector<Player> players, int player_index, bool indexs = false)
{
	// DRAW CARDS
	
	if (player_index == 0)
		std::cout << "\n" << "Your Cards:\n";
	else
		std::cout << "\n" << players[player_index].name <<"'s Cards:\n";

	
	int card_index = 0;
	std::string index_draw = "";

	if (players[player_index].Get_Cards().size() >= 5)
	{
		int remainder = players[player_index].Get_Cards().size() % 5;

		for (int i = 0; i < (players[player_index].Get_Cards().size() - remainder) / 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (player_index == 0)
				{
					std::cout << "\033[1;" << players[player_index].Get_Cards()[card_index].color << "m🂱" + players[player_index].Get_Cards()[card_index].style + "\033[0m ";
				}
				else
				{
					if (indexs)
						index_draw = std::to_string(card_index + 1) + ":";
					std::cout << "\033[1;" << "37" << "m" + index_draw + "🂱?\033[0m ";
				}
				card_index++;
			}
			std::cout << std::endl;
		}
	}

	for (int i = card_index; i < players[player_index].Get_Cards().size(); i++)
	{
		if (player_index == 0)
			{
				std::cout << "\033[1;" << players[player_index].Get_Cards()[card_index].color << "m🂱" + players[player_index].Get_Cards()[card_index].style + "\033[0m ";
			}
			else
			{
				if (indexs)
						index_draw = std::to_string(card_index + 1) + ":";
					std::cout << "\033[1;" << "37" << "m" + index_draw + "🂱?\033[0m ";
			}
			card_index++;
	}
	std::cout << std::endl;
	std::cout << "TOTAL: " <<  players[player_index].Get_Cards().size() << std::endl;

	// END DRAW CARDS
	if (indexs == 0)
	{

	std::cout << "TOTAL MATCHES: " <<  players[player_index].Get_Points() << std::endl;
	}
}

bool Menu_Logic(std::vector<Player> &players, std::vector<std::string> card_names, std::vector<std::string> card_styles, bool &in_game, std::vector<stats> &end_stats)
{
	int choice;
	std::cout << std::endl << "Enter Number: ";
	std::cin >> choice;

	if (choice == 1)
	{
		for (int j = 0; j < card_styles.size(); j++)
		{
			for (int i = 0; i < card_names.size(); i++)
			{
				Check_Matches(players, 0, card_names[i], card_styles[j]);	
			}
		}
		
		ClearScreen(); //REMOVE LATER
		Dynamic_Type("MATCHES: ");
		std::cout << "\n\n";

		int total_matches = 0;
		for( auto const& [key, val] : players[0].Get_Matches() )
		{
			Dynamic_Type(key + ": " + std::to_string(val), 20);
			total_matches += val;
			std::cout << std::endl;
		}
		std::cout << "\nTOTAL: " << total_matches;
		std::cout << "\n\nEnter \"1\" to continue: ";
		int temp;
		std::cin >> temp;
		return false;
	}
	else if (choice == 2)
	{
		int cpu;
		do
		{
			cpu = rand() % players.size();
		} while (cpu == 0);

		std::vector<std::string> phrases;
		phrases.push_back("laughed.");
		phrases.push_back("made a funny face too.");
		phrases.push_back("is intrigued.");
		phrases.push_back("smiles back.");

		ClearScreen();
		Dynamic_Type("You made a funny face...");
		std::cout.flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "\n\n";
		Dynamic_Type(players[cpu].name + " " + phrases[rand() % phrases.size()]);

		std::cout << "\n\nEnter \"1\" to continue: ";
		int temp;
		std::cin >> temp;
		return false;
	}
	else if (choice == 3)
	{
		int cpu;
		do
		{
			cpu = rand() % players.size();
		} while (cpu == 0);

		ClearScreen();

		int success = rand() % 5;

		if (success == 1)
		{
			Dynamic_Type("You Tried to Peak at " + players[cpu].name + "'s cards, but you got caught.");

			success = rand() % 5;

			if (success == 1)
			{
				Dynamic_Type("\n\n" + players[cpu].name + " got upset and kicked you out of the game. You gave your cards to random players.  :<");
				in_game = false;
				for (int i = 0; i < players[0].Get_Cards().size(); i++)
				{
					int player_add_index;
					do
					{
						player_add_index = rand() % players.size();
					} while (player_add_index == 0);
					players[player_add_index].Add_Cards(players[0].Get_Cards()[i]);
				}
				 //CHANGE
				//stats stat(players[0].name, players[0].Get_Points());
				//end_stats.push_back(stat);
				players.erase(players.begin());
			}

			else
			{
				Dynamic_Type("\n" + players[cpu].name + " got upset and quit the game giving their cards to random players.  :<");
				for (int i = 0; i < players[cpu].Get_Cards().size(); i++)
				{
					int player_add_index;
					do
					{
						player_add_index = rand() % players.size();
					} while (player_add_index == cpu);
					players[player_add_index].Add_Cards(players[cpu].Get_Cards()[i]);
				}
				//stats stat(players[cpu].name, players[cpu].Get_Points());
				//end_stats.push_back(stat);
				players.erase(players.begin() + cpu); 
			}
				
		}
		else
		{
			Dynamic_Type("You Successfully peaked at " + players[cpu].name + "'s cards.\n\n");

			Dynamic_Type("You Saw that " + players[cpu].name + " had a\t");
			
			int card_index = rand() % players[cpu].Get_Cards().size();
			std::cout << "\033[1;" << players[cpu].Get_Cards()[card_index].color << "m🂱" + players[cpu].Get_Cards()[card_index].style + "\033[0m ";
		}
		std::cout << "\n\nEnter \"1\" to continue: ";
		int temp;
		std::cin >> temp;
			
		return false;
	}
	else if (choice == 4)
	{
		return true;
	}

	else
		return false;
}

void Choose_Card(int player, std::vector<Player> &players)
{
	
	if (players[player].name == "player")
	{
		bool check = true;
		
		while (check) 
		{
			ClearScreen();
			Dynamic_Type("Choose a CARD from " + players[1].name + "\n\n\n");
			
			Draw_Player_Cards(players, 1, true);
			int choice;
			std::cout << "\n\n Enter Corresponding Number: ";
			std::cin >> choice;
			choice --;

			if (choice > - 1 && choice <= players[1].Get_Cards().size())
			{
				check = false;
				players[0].Add_Cards(players[1].Get_Cards()[choice]);
				players[1].Remove_Cards(choice);
			}
			
		}
		ClearScreen();
	}
	else
	{
		int random_card;

		if (player + 1 != players.size())
		{
			Dynamic_Type(players[player].name + " is choosing from " + players[player + 1].name, 30);
			random_card = rand() % players[player + 1].Get_Cards().size();
			players[player].Add_Cards_Random(players[player + 1].Get_Cards()[random_card]);
			players[player + 1].Remove_Cards(random_card);
		}

		else
		{
			if (players[0].name == "player")
				Dynamic_Type(players[player].name + " is choosing from you" , 30);
			else
				Dynamic_Type(players[player].name + " is choosing from " + players[0].name , 30);
			random_card = rand() % players[0].Get_Cards().size();
			players[player].Add_Cards_Random(players[0].Get_Cards()[random_card]);
			players[0].Remove_Cards(random_card);
		}

		for (int i = 0; i < 3; i++)
			Dynamic_Type(".", 100);
		std::cout << " ✓\n\n";
	}
}

bool Game_Stats(std::vector<stats> &end_stats)
{
	ClearScreen();
	std::cout << "\t\t\t\t\t\t\tGAME OVER\n\n\n";
	std::cout << "GAME STATS:\n\n";
	for (int i = 0; i < end_stats.size(); i++)
	{
		Dynamic_Type(end_stats[i].player_name + ":\n");
		Dynamic_Type("PLACEMENT ON LEADER BOARD: " + std::to_string(i + 1) + "\n");
		Dynamic_Type("MATCHES: " + std::to_string(end_stats[i].matches));
		std::cout << "\n\n";
	}
	std::cout << "\n\nEnter \"1\" to play again: ";
	int temp;
	std::cin >> temp;
	if (temp == 1)
		return true;
	else
		return false;
}

void Shuffle(std::vector<Player> &players, int player_index)
{
	std::vector<card> shuffle;
	int rand_num;

	for (int i = 0; i < players[player_index].Get_Cards().size(); i++)
	{
		shuffle.push_back(players[player_index].Get_Cards()[i]);
		players[player_index].Remove_Cards(i);
	}

	
	//SIZE IS FIXED!
	int i = 0;
	int j = shuffle.size();
	while (i < j)
	{
		rand_num = rand() % shuffle.size();
		players[player_index].Add_Cards(shuffle[rand_num]);
		shuffle.erase(shuffle.begin() + rand_num);
		i++;
		j = shuffle.size();
	} 
}

void CheckWin(std::vector<Player> &players, std::vector<stats> &end_stats, bool game_over = false)
{
	for (int i = 0; i < players.size(); i ++)
	{
		if (players[i].Get_Cards().size() == 0 || game_over)
		{
			stats stat(players[i].name, players[i].Get_Points());
			end_stats.push_back(stat);
			players.erase(players.begin() + i); 
		}
	}
}

bool Game()
{
	int num_players;
	std::vector<Player> players;
	std::vector<stats> end_stats;
	
	std::vector<std::string> card_names;
	card_names.push_back("green");
	card_names.push_back("cyan");
	card_names.push_back("yellow");
	card_names.push_back("blue");
	card_names.push_back("magenta");

	std::vector<std::string> card_styles;
	card_styles.push_back("#");
	card_styles.push_back("$");
	card_styles.push_back("&");

	do
	{
		ClearScreen();
		std::cout << "How Many CPUs? (1-11 Players): ";
		std::cin >> num_players;
		ClearScreen();
	} while(!(num_players <= 11 && num_players >= 1));
	
	Player player;
	player.Change_Name("player");
	players.push_back(player);

	for(int i = 0; i < num_players; i++)
	{
		std::string name = "CPU";
		name += std::to_string(i + 1);

		Player CPU;
		CPU.name = name;
		players.push_back(CPU);
	}

	
	Deal_Cards(players, card_names, card_styles);

	bool turn = false;
	bool in_game = true;
	bool starting = true;
	bool choosen = false;
	int times_looped = 0;

	do 
	{
		if (players[0].name == "player")
		{
			while (times_looped < 2)
			{
				Draw_Player_Menu(players, choosen);
				Draw_Player_Cards(players, 0);

				if (players.size() > 1)
					Draw_Player_Cards(players, 1);

				turn = Menu_Logic(players, card_names, card_styles, in_game, end_stats);
				
				if (turn && !(choosen))
				{
					if (starting)
					{
						for (int i = 0; i < players.size(); i++)
						{
							if (players[i].name != "player")
							{
								for (int j = 0; j < card_styles.size(); j++)
								{
									for (int x = 0; x < card_names.size(); x++)
									{
										Check_Matches(players, i, card_names[x], card_styles[j]);	
									}
								}
							}	
						}
						starting = false;
					}
					//Shuffle(players, 1);
					CheckWin(players, end_stats);
					if (players.size() <= 1)
					{
						CheckWin(players, end_stats, true);
						break;
					}
					Choose_Card(0, players);
					choosen = true;
					times_looped ++;
				}

				else if (turn && choosen)
				{
					choosen = false;
					times_looped++;
				}

				if (!(in_game))
				{
					break;
				}
			}
		}
		
		CheckWin(players, end_stats);
		if (players.size() <= 1)
		{
			CheckWin(players, end_stats, true);
			break;
		}

		times_looped = 0;

		ClearScreen();
		for (int i = 0; i < players.size(); i++)
		{
			if (players[i].name != "player")
				{
					Choose_Card(i, players);
					for (int j = 0; j < card_styles.size(); j++)
					{
						for (int x = 0; x < card_names.size(); x++)
						{
							CheckWin(players, end_stats);
							Check_Matches(players, i, card_names[x], card_styles[j]);	
						}
					}
				}
		}
		CheckWin(players, end_stats);
		if (players.size() <= 1)
		{
			CheckWin(players, end_stats, true);
			break;
		}

		std::cout << "\n\n\nFinished Players:\n" << std::endl;
		for (int i = 0; i < end_stats.size(); i++)
		{
			std::cout << end_stats[i].player_name << std::endl;
			std::cout << "Number Of Matches: " <<end_stats[i].matches << std::endl;
			std::cout << std::endl;
		}

		std::cout << "\n\nEnter \"1\" to continue: ";
		int temp;
		std::cin >> temp;

	} while (true);
	
	if (Game_Stats(end_stats))
		return true;
	else
		return false;
}

int main() {
	srand(time(NULL));
	Print_MainMenu();
	Print_Premise();
	bool game = true;

	while (game)
	{
		game = Game();
	}

	return 0;
}


/*
	for(int i = 0; i < players.size(); i++)
	{
		std::cout << players[i].name << std::endl;
	}


	//check matches
	Check_Matches(players, 1, "green");	


red:31
blue:34
yellow:33
yellow:33
cyan:36
cyan:36
magenta:35
yellow:33

int num_cards = 0;
	for (int i = 0; i < players.size(); i++)
	{
		std::cout << players[i].name << ":"<<std::endl;
		for (int j = 0; j < players[i].Get_Cards().size(); j++)
		{
			std::cout << players[i].Get_Cards()[j].name << ":" << players[i].Get_Cards()[j].color << std::endl;
			num_cards++;
		}
		std::cout << std::endl;
	}
	std::cout << "TOTAL CARDS: " << num_cards;


	//get matches
	std::cout << std::endl << "MATCHES: ";
	for( auto const& [key, val] : players[player_index].Get_Matches() )
	{
		std::cout << key << ":" <<val;
	}



_________________________________
      __         _         ____  
    /    )       /         /    )
---/----/-------/---------/___ /-
  /    /       /         /    |  
_(____/_______/____/____/_____|__
                                 
       "OLR"                          




//add 3 diffrent types of cards, fix matches - something is busted :[bruh] change OLR in the menu printing to something cool THATS RED!!...
//problem was actually the draw function ;{bruh}

//Change matches to accept styles?

//SHUFFLE WORDS :)!!!!



//FIX DEALING :(

//IMPLEMENT statements to check if players have 0 cards or something

//GET THE CPUS TO GO REALLY FAST IF PLAYER WINS/GETS OUT!!


*/
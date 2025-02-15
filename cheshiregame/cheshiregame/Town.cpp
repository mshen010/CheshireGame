#include "stdafx.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Town.h"
#include "Store.h"
/*
Town fileformat will be as follows:
   
   <townname>
   <store_filename>
   <# of people>
   <person 1's name>
   <person 1 dialog>
   <person 2's name>
   <person 2's dialog>
   <continued>
*/

void Town::fill_town(const std::string &text_file) {
    std::string townname;
    std::string store_filename;
    int people_count;

    std::ifstream inFS;
    inFS.open(text_file.c_str());
    if (!inFS.is_open()) {
        std::cerr << "Error: Cannot open Town file" << std::endl;
        return;
    }
    std::getline(inFS, townname);
    this->townname = townname;
    std::getline(inFS, store_filename);
    Store s;
    // TODO: Make a sample store?
    s.fillStore(store_filename);
    this->s = s;
    string p;
    std::getline(inFS, p);
    stringstream ss(p);
    ss >> people_count;
    //inFS >> people_count;
    //std::cout << store_filename << std::endl;
    std::string name;
    std::string dialog;
    for (int i = 0; i < people_count; ++i) {
    //    std::cout << i << std::endl;
        std::getline(inFS, name);
        std::getline(inFS, dialog);
    //    std::cout << name << ": " << dialog << std::endl;
        people.push_back(
            std::make_pair(name, dialog));
    }
    inFS.close();
}

Town::Town() {
	//dungeon1 = new Dungeon("demo_dun_1.txt");
	//dungeon2 = new Dungeon("demo_dun_2.txt");
   // fill_town("sample_town.txt");
}

Town::Town(std::string filename) {
    //fill_town(filename);
}

void Town::run(player* p) {
    // Main function? I guess.
    //std::cout << "Welcome to " << townname << std::endl;
	int choice = -1;

    while (choice != 0) //Fix for exit
    {
        std::cout << "What would you like to do?" << std::endl;
        std::cout << "1. Visit Inn" << std::endl
            << "2. Go shopping" << std::endl
            << "3. Go to the blacksmith" << std::endl
            << "4. Go to the tavern" << std::endl
            << "5. Go talk to the townspeople" << std::endl
            << "6. Go to a dungeon" << std::endl
            << "7. Quit" << std::endl
            << "0. Save Game" << std::endl;

        std::cout << "> ";
        cin >> choice;
        if (cin.good()) {

            if (choice == 1) {
                inn(p);
            }
            else if (choice == 2) {
                store(p);
            }
            else if (choice == 3) {
                blacksmith(p);
            }
            else if (choice == 4) {
                tavern();
            }
            else if (choice == 5) {
                talk();
            }
            else if (choice == 6) {
                dungeon_select(p);
            }
            else if (choice == 7) {
                return;
            }
            else if (choice == 0) {
                savesystem.p = *p;
                savesystem.saveSave();
            }
            else {
                cout << "bad input, try again" << std::endl;
            }
        }
        cin.clear();
        cin.ignore(INT_MAX, '\n');
	}
    return;
}

void Town::inn(player* p) {

	int choice = -1;
	std::string input;
    
    std::cout << ">";
	while (choice != 3)
	{
		std::cout << "Welcome to the inn." << std::endl;
		std::cout << "1. Rest" << std::endl;
		std::cout << "2. Recruit" << std::endl;
		std::cout << "3. Exit" << std::endl;
		std::cin >> choice;
        if (cin.good()) {
            if (choice == 1) {
                std::cout << "It will cost you 50 gold. Are you sure?" << std::endl;
				std::cout << "You have " << p->getmoney() << " gold." << std::endl;
                std::cout << "1. Yes" << std::endl;
                std::cout << "2. No" << std::endl;
                std::cin >> choice;
                if (choice == 1)
                {
                    if (p->getmoney() < 50)
                    {
                        std::cout << "What do you think this is, a charity house? Get lost!" << std::endl;
                        return;
                    }
                    else
                    {
                        p->setmoney(p->getmoney() - 50);
                        p->setHP(p->getMaxHP());
                        std::cout << "Hope you enjoyed your stay!" << std::endl;
                        return;
                    }
                }
                else if (choice == 2)
                {
                    return;
                }
            }
            else if (choice == 2) {
                // Recruit party members
                std::cout << "Recruiting not implemented." << std::endl;
                return;
            }
            else if (choice == 3) {
                return; 
            }
        }
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Bad input, try again." << endl;
	}
    return;
}

void Town::blacksmith(player* p) {
    
	int choice = -1;

	std::cout << "Welcome to the blacksmith." << std::endl;
	std::cout << "1. Upgrade Weapon." << std::endl;
	std::cout << "2. Exit." << std::endl;
	while (choice != 2)
	{
		std::cin >> choice; 
		if (cin.good())
		{
			if (choice == 1)
			{
				std::string wep;

				std::cout << "I can refine your weapon for a price. However, I may fail." << std::endl;
				std::cout << "The price to refine your weapon is 100 gold." << std::endl;
				if (p->getmoney() < 100)
				{
					std::cout << "Sorry pal, you don't have enough gold." << std::endl;
					return;
				}
				std::cout << "Which weapon would you like me to refine? (Type q to quit)" << std::endl;
				std::cin.ignore();
				getline(std::cin, wep);
				if (wep == "q")
				{
					return;
				}
				while (p->wep_search(wep) == NULL)
				{
					std::cout << "You do not have this weapon. Please enter it again." << std::endl;
					getline(std::cin, wep);
					if (wep == "q")
					{
						return;
					}
				}
				std::cout << "Alright, here goes!" << std::endl;
				if ((rand() % 3 + 1) == 1)
				{
					std::cout << "The blacksmiths hammers down your weapon, tempering it with fine skill." << std::endl;
					std::cout << "\"Here you go.\" The blacksmith hands you your weapon, looking better than ever." << std::endl;
					p->wep_search(wep)->add_dmg((rand() % 10 + 1));
					return;
				}
				else
				{
					std::cout << "The blacksmith clanks your weapon with his hammer, but breaks with his next strike." << std::endl;
					std::cout << "\"Curses! My hand slipped.\" He frowns, and apologetically hands you your broken weapon." << std::endl;
					p->remove_wep(wep);
					return;
				}

			}
			if (choice == 2)
			{
				return;
			}
		}
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Bad input, try again." << endl;
	}

    return;
}

void Town::tavern() {
    // TODO: Implement tavern
    std::cout << "Unimplemented." << std::endl;
    return;
}

void Town::store(player* p) {
	std::cout << "Unimplemented." << std::endl;
	return;
    //s.run(p);
    return;
}

void Town::talk() {
	std::cout << "Unimplemented." << std::endl;
	return;
    srand(time(NULL));
    int random = rand() % this->people.size();
    std::cout << people.at(random).first << " says: " <<
    people.at(random).second << std::endl;
    return;
}

void Town::dungeon_select(player* p)
{
	int input;

	std::cout << "Which dungeon would you like to go to?" << std::endl;
	std::cout << "1. Ben's Dungeon" << std::endl;
	std::cout << "2. Devil's Lair" << std::endl;
	std::cout << "3. The Lonely Dungeon :'(" << std::endl;
	//Output dungeons														// Tony's super cool lazy fix!
	std::cin >> input;
	//if (input == 1)
	//{
		std::string map_name = "demo_map.txt";
		map test_map(5, map_name);
        std::string dungeon_name = "demo_dun_1.txt";
		Dungeon* d = new Dungeon(dungeon_name, map_name);
		Combat_System cs(p);
		//test_map.run(p, d, cs);
		Scene s;
		s.scene_005();
		cs.runBattle(d->get_boss());
		s.scene_006();
	//}
	/*FOR DEMO ONLY*/
		
}

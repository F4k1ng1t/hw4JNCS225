//Joshua Nichols

#include <iostream>
#include <string>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;



class Color
{
public:
	static const string red;
	static const string green;
	static const string white;
	static const string blue;

	static inline void print(string text, string color = white, string reset = white, bool newLine = true)
	{
		cout << color << text << reset;
		if(newLine)
			cout << endl;
	}
	static inline void change(string color = white)
	{
		cout << color;
	}
	static string input(string color, string reset = white)
	{
		string input = "";
		change(color);
		getline(cin, input);
		change(reset);
		return input;
	}
	static void type(string text, int delay = 100, string color = white, string reset = white)
	{
		for(char c : text)
		{
			string s(1, c);
			print(s, color, reset, false);
			cout.flush(); // force immediate output
			this_thread::sleep_for(milliseconds(delay));
		}
		print("", color, reset, true);
	}
};

const string Color::red = "\033[31m";
const string Color::green = "\033[32m";
const string Color::white = "\033[0m";
const string Color::blue = "\033[34m";

class ErrorHandler
{
private:
	Color c;
public:
	static int GetIntegerInput()
	{
		int value;
		while (!(cin >> value))
		{
			cin.clear();               
			cin.ignore(INT_MAX, '\n');
			cout << "Error, enter a valid integer: ";
		}
		return value;
	}
	static string AskForOption(string option1, string option2)
	{
		string input = "";
		cin >> input;
		while(input != option1 and input != option2)
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << option1 << " or " << option2 << "?" << endl;
			cin >> input;
		}
		return input;
	}
};

class Hero
{
private:
	string _name;
	int _health;
	Color c;
public:
	inline void setName(string Name)
	{
		_name = Name;
	}
	inline string getName()
	{
		return _name;
	}
	inline void setHealth(int newHealth)
	{
		_health = newHealth;
	}
	inline int getHealth()
	{
		return _health;
	}

	Hero()
	{
		c.print("Enter a name for your hero.", c.green);
		setName(c.input(c.blue));
		c.print("The hero's name is " +  getName(), c.red);
		setHealth(100);
		c.print(getName() + " has " + to_string(getHealth()) + " health.", c.green);
	}
};

class Monster
{
private:
	string _name;
	int _health;
	Color c;
public:
	inline void setName(string Name)
	{
		_name = Name;
	}
	inline string getName()
	{
		return _name;
	}
	inline void setHealth(int newHealth)
	{
		_health = newHealth;
	}
	inline int getHealth()
	{
		return _health;
	}
	Monster()
	{
		//seed number generator
		string monsterNames[5] = {"Orc","Goblin","Demon","Dragon","Skeleton"};
		setName(monsterNames[rand() % 5]);
		setHealth(1 + rand() % 20);
		//c.print(getName(), c.red);
		//c.print(to_string(getHealth()));
	}
};

void Adventure(Hero& h, Monster m[], int length);
bool Fight(Hero& h, Monster& m);
bool Flee(Hero& h, Monster& m);

int main()
{
	srand(time(0));
	Color c;
	Hero hero;
	c.print("How many Monsters will " + hero.getName() + " fight?", c.blue);
	unsigned int length = ErrorHandler::GetIntegerInput();
	Monster monsters[length];
	Adventure(hero, monsters, length);
	return 0;
}

bool Fight(Hero& h, Monster& m)
{
	Color c;
	int damageChance = 1 + rand() % 10;
	if(damageChance <= 5)
	{
		h.setHealth(h.getHealth() - m.getHealth());
		c.type("OUCH!!! You took " + to_string(m.getHealth()) + " damage! " + h.getName() + " now has " + to_string(h.getHealth()) + " health.", 100, c.red);
		return false;
	}
	else
	{
		c.type("You killed the " + m.getName() + "!!", 100, c.green);
		return true;
	}
}

bool Flee(Hero& h, Monster& m)
{
	Color c;
	int failChance = 1 + rand() % 20;
	if(failChance != 1)
	{
		c.type("You got away!", 100, c.blue);
		return true;
	}
	else
	{
		c.type("OUCH!!! You took " + to_string(m.getHealth()) + " damage! " + h.getName() + " now has " + to_string(h.getHealth()) + " health.", 100, c.red);
		return false;
	}
}

void Adventure(Hero& h,  Monster m[], int length)
{
	Color c;
	int monstersKilled = 0;
	for(int i = 0; i < length; i++)
	{
		c.print("A " + m[i].getName() + " with " + to_string(m[i].getHealth()) + " health appears!", c.red);
		c.type("Would you like to fight or flee?");
		string option = ErrorHandler::AskForOption("fight", "flee");
		if(option == "fight")
		{
			if(Fight(h, m[i]))
			{
				monstersKilled++;
			}
			else
			{
				i--; //Fight the same monster again
			}
		}
		else if(option == "flee")
		{
			if(!Flee(h, m[i]))
			{
				i--; //Fight the same monster again
			}
		}
		if(h.getHealth() <= 0)
		{
			c.type(h.getName() + " died", 500, c.red);
		}
	}
	c.type(h.getName() + " defeated " + to_string(monstersKilled) + " monsters", 100, c.green);
}

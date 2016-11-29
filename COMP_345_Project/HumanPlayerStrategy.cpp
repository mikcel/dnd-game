#include "HumanPlayerStrategy.h"

#include <string> 

/**
* Obtains the map representation of the current character
* @return the symbol that will represent the curretn character on the map
*/
std::string HumanPlayerStrategy::print() const
{
	return "\xFE";
}

/**
* Obtains a human-readable name for the strategy
* @return a string that represents the type of the strategy
*/
std::string HumanPlayerStrategy::getStrategyName()
{
	return "Human Player";
}

/**
* Executes a game turn on the HumanPlayerStrategy
* @param map the map on which the game is played
* @param mo the map observer
* @param meo the map elements toggler
* @return true if the players wants to continue to play, false otherwise.
*/
bool HumanPlayerStrategy::executeMovementTurn(Map& map, MapObserver& mo, MapElementsToggler& meo)
{
	int remainingSteps = NUMBER_OF_STEPS;

	mo.setPrintCharacterInfo(getStrategyName(), characterElement->getCharacter().getName());
	mo.setPrintTurnInfo(TurnType::HUMAN_PLAYER_MOVE);
	mo.setPrintMovementInfo(remainingSteps);
	mo.printMap();

	bool isPlaying = true;

	while (remainingSteps > 0)
	{
		char c = _getch();
		if (isPlaying)
		{
			switch (c)
			{
			case KEY_UP: //Move the character up
				if (map.moveElement(0, -1, *characterElement))
				{
					mo.setPrintMovementInfo(--remainingSteps);
					mo.printMap();
				}
				break;
			case KEY_DOWN: //Move the character down
				if (map.moveElement(0, 1, *characterElement))
				{
					mo.setPrintMovementInfo(--remainingSteps);
					mo.printMap();
				}
				break;
			case KEY_LEFT: //Move the character left
				if (map.moveElement(-1, 0, *characterElement))
				{
					mo.setPrintMovementInfo(--remainingSteps);
					mo.printMap();
				}
				break;
			case KEY_RIGHT: //Move the character right
				if (map.moveElement(1, 0, *characterElement))
				{
					mo.setPrintMovementInfo(--remainingSteps);
					mo.printMap();
				}
				break;
			case 'q': //Quit the application
			case 'Q': //Quit the application
				return false;
				break;
			case 'T': //Toggles the view of the map elements
			case 't': //Toggles the view of the map elements
				meo.show();
				isPlaying = false;
				break;
			case 'p': //Toggles the view of the map elements
				meo.showPlayer();
				isPlaying = false;
				break;
			case 'e': //Toggles the view of the map elements
				meo.showEnemies();
				isPlaying = false;
				break;
			case 'c': //Toggles the view of the map elements
				meo.showChests();
				isPlaying = false;
				break;
			case KEY_ENTER:
				return true;
				break;
			}
		}
		else if (c != 224 && c != KEY_UP && c != KEY_DOWN && c != KEY_RIGHT && c != KEY_LEFT)
		{
			if (system("CLS")) system("clear");
			mo.printMap();
			isPlaying = true;
		}
	}

	return true;
}

/**
* Executes the attack phase of the character turn
* @param map the map on which the game is played
* @param mo the map observer
* @param meo the map elements toggler
* @return true if the player wants to continue to play, false otherwise
*/
bool HumanPlayerStrategy::executeAttack(Map& map, MapObserver& mo, MapElementsToggler& meo)
{
	mo.setPrintTurnInfo(TurnType::HUMAN_PLAYER_ATTACK);
	mo.printMap();

	bool isPlaying = true;

	while (true)
	{
		char c = _getch();
		if (isPlaying)
		{
			switch (c)
			{
			case 'a':
			case 'A':
			{
				CharacterElement* characterToAttack = chooseAttackTarget(map, mo, meo);
				if (characterToAttack != nullptr)
				{
					this->characterElement->attack(*characterToAttack);
				}
				cout << endl;
				system("pause");
				return true;
			}
			break;
			case 'q': //Quit the application
			case 'Q': //Quit the application
				return false;
				break;
			case 'T': //Toggles the view of the map elements
			case 't': //Toggles the view of the map elements
				meo.show();
				isPlaying = false;
				break;
			case 'p': //Toggles the view of the map elements
				meo.showPlayer();
				isPlaying = false;
				break;
			case 'e': //Toggles the view of the map elements
				meo.showEnemies();
				isPlaying = false;
				break;
			case 'c': //Toggles the view of the map elements
				meo.showChests();
				isPlaying = false;
				break;
			case KEY_ENTER:
				return true;
				break;
			}
		}
		else if (c != 224 && c != KEY_UP && c != KEY_DOWN && c != KEY_RIGHT && c != KEY_LEFT)
		{
			if (system("CLS")) system("clear");
			mo.printMap();
			isPlaying = true;
		}
	}

	return true;
}

/**
* Chooses an attack target
* The player will be prompt to choose the character that he wants to attack
* from the characters that are near him
* @param map the map on which the game is played
* @param mo the map observer
* @param meo the map elements toggler
* @return true if the start point was succesfully set
*/
CharacterElement* HumanPlayerStrategy::chooseAttackTarget(Map& map, MapObserver& mo, MapElementsToggler& meo)
{
	std::vector<CharacterElement*> attackableChracters;
	for (Element* element : map.getElements())
	{
		CharacterElement* currentCharacter = dynamic_cast<CharacterElement*>(element);		

		if (currentCharacter)
		{
			Position characterPosition = characterElement->getPosition();
			Position currentCharacterPosition = currentCharacter->getPosition();
			if (isTileNextTo(characterPosition.x, characterPosition.y, currentCharacterPosition.x, currentCharacterPosition.y))
			{
				attackableChracters.push_back(currentCharacter);
			}			
		}
	}

	if (attackableChracters.size() > 1)
	{
		while (true)
		{
			cout << endl << "Here is a list of all possible characters to attack." << endl;
			cout << "Please enter the ID of the character that you want to attack:" << endl;
			for (int i = 0; i < attackableChracters.size(); i++)
			{
				cout << i + 1 << ") " << attackableChracters[i]->getCharacterStrategy()->getStrategyName() 
					<< " named \"" << attackableChracters[i]->getCharacter().getName() << "\"" << endl;				
			}

			string enteredIdString;
			cin >> enteredIdString;

			if (enteredIdString.find_first_not_of("0123456789") == string::npos)
			{
				int enteredId = atoi(enteredIdString.c_str());
				if (enteredId > 0 || enteredId <= attackableChracters.size())
				{
					return attackableChracters[enteredId - 1];
				}
				else
				{
					cout << "Invalid ID" << endl;
				}
			}
			else
			{
				cout << "Invalid ID" << endl;
			}
		}
	}
	else if (attackableChracters.size() == 1)
	{
		while (true)
		{
			cout << endl << "The only attackable character is" << attackableChracters[0]->getCharacterStrategy()->getStrategyName() 
				<< " named \"" << attackableChracters[0]->getCharacter().getName() << "\"" << endl;
			cout << "Do you want to proceed attacking? (Y/N)" << endl;

			string answer;
			cin >> answer;
			if (answer == "Y" || answer == "y")
			{
				return attackableChracters[0];
			}
			else if (answer == "N" || answer == "n")
			{
				cout << endl << "Player decided to not to attack anyone" << endl;
				return nullptr;
			}
		}
	}
	else
	{
		cout << endl << "No characters are near, so there is no one to attack" << endl;
		return nullptr;
	}
}
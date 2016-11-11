
#include "CharacterController.h"


CharacterController::CharacterController()
{
	
}

CharacterController::CharacterController(Character *currentCharacter){

	this->currentCharacter = currentCharacter;

}

CharacterController::~CharacterController()
{
	delete currentCharacter;
}

Character* CharacterController::getCurrentCharacter(){
	return currentCharacter;
}

void CharacterController::setCurrentCharacter(Character *character){
	currentCharacter = new Character(*character);
}

void CharacterController::createCharacter(){
	
	cout << "==== Character creation ====" << endl << endl;
	bool choiceCorrect = false; //Flag for correct choice between character and fighter creation 
	int choice = 0; //! Choice of user

	cout << "Please choose between creating a simple Character(1) or a Fighter(2) \nEnter the respective number: ";

	//! Keep asking if choice is not correct
	while (!choiceCorrect){
		
		cin >> choice;
		if (!cin){
			cin.clear();
			cin.ignore();
		}

		if (choice == 1){ //! If choice is Character
			cout << "\nTime to create a new Character!" << "\nLet's gets started.";
			choiceCorrect = true;
		}
		else if (choice == 2){ //! If choice is Fighter
			cout << "\nTime to create a new Fighter!" << "\nLet's gets started.";
			choiceCorrect = true;
		}
		else{ //! Asks again if incorrect choice
			cout << "\nIncorrect choice. Please enter only 1 or 2 depending on the character type you want to create.\n";
			cout << "1 - Character OR 2 - Fighter. Please choose correctly: ";
		}
	}

	bool characterCorrect = false; //! Flag for correct Character

	string name; //! Player's name
	int level; //! Player's level
	int size; //! Player's size
	int fightStyle; //! Fighter's fighting style

	//! While character not correct keep asking
	while (!characterCorrect){

		cout << "\nEnter your player's name: ";
		cin.ignore();
		getline(cin, name); //! Get name even with spaces

		cout << "Enter your level: ";
		cin >> level; //! Get level

		cout << "Enter choose size from the list below:\n";
		//! Display all the sizes possible
		displayCharacterSize();
		cin >> size; //! Get size

		if (choice == 2){ //! If fighter
			cout << "Enter your Fighter Style from the list below:\n";
			//! Displays all the fighting style possible
			displayFighterStyle();
			cout << "Please choose: ";
			cin >> fightStyle; //! Get fighting style if fighter
		}

		if (!cin){ //! Error while inputting
			cin.clear();
			cin.ignore();
			cout << "\nIncorrect Input. Please enter again all data.";
		}
		else if (level<1){ //! Check level
			while (level < 1){ //! if level is still not good keep asking
				cout << "\nIncorrect Level entered. Please enter level again: ";
				cin >> level;
				if (!cin){
					cin.clear();
					cin.ignore();
				}
			}
		}
		else if (size<0 || size>3){ //! Check size
			while (size<0 || size>3){ //! If size is still not good check size
				cout << "\nIncorrect Size entered. Please enter Size again: ";
				displayCharacterSize();
				cin >> size;
				if (!cin){
					cin.clear();
					cin.ignore();
				}
			}
		}
		else if (choice == 2 && (fightStyle<0 || fightStyle>2)){
			while (fightStyle<0 || fightStyle>2){ //! if fighting style is still not good ask again
				cout << "\nIncorrect Fighting Style entered. Please enter style again from the list below: \n";
				displayFighterStyle();
				cin >> fightStyle;
				if (!cin){
					cin.clear();
					cin.ignore();
				}
			}
		}

		//! Display information entered for now and ask if want to continue
		cout << "\n\nHere are the data you entered for your character: ";
		cout << "\nName: " << name << "\nLevel: " << level << "\nSize: " << (CharacterSize)size;

		if (choice == 2)
			cout << "\nFighting Style: " << (FightStyle)fightStyle;

		string changeData = "";
		bool changeDataCorrect = false; //! Flag to check if continue input is correct
		cout << "\nDo you want to change anything? (Y/N) ";
		while (!changeDataCorrect){
			cin >> changeData;
			if (changeData == "N"){
				characterCorrect = true;
				changeDataCorrect = true;
			}
			else if (changeData == "Y"){
				changeDataCorrect = true;
			}
			else
			{
				cout << "Please enter Y or N: ";
			}

		}

	}

	//! Generates the ability score
	cout << "\n\nTime to generate the ability scores...";

	srand(time(0)); //! Give a starting point to the random function

	int arrRand[Character::NO_ABILITY]; //Create an array with the no of abilities a character should have
	int *ptr = arrRand; //! Pointer that points to newly created array

	int arrAbilityScores[Character::NO_ABILITY];//! Creates a second array of ability scores that will be used to passed to the Character constructor

	//! For loop to generate random numbers between 3 and 18 and but them in the first created array
	cout << "\n\nThe random scores are: ";
	for (int i = 0; i < Character::NO_ABILITY; i++){
		arrRand[i] = rand() % 16 + 3;
		cout << arrRand[i] << " ";
	}

	//! Variable use for input
	int chosenScr = 0;

	//! Make user chooses which number generated he wants to assign to each score.
	cout << "\nPlease choose a score from the numbers obtained for the following: \n";
	for (int i = 0; i < Character::NO_ABILITY - 1; i++){

		//! Outputs the number that are left ot be chosen
		cout << "The numbers left are: ";
		for (int i = 0; i < Character::NO_ABILITY; i++){
			if (arrRand[i] != 0)
				cout << arrRand[i] << " ";
		}

		//! For each ability, (using the enumrated type), choose score
		cout << "\n" << CharacterAbility(i) << ": ";
		cin >> chosenScr;

		//! If incorrect input, program dies
		if (!cin){
			cerr << "Program dies";
			exit(1);
		}

		//! Check if number entered is in random generated array. if not ask user to enter again until number is valid
		while (!checkScr(ptr, chosenScr)){
			cout << "Please choose a correct no.: ";
			cin >> chosenScr;
		}

		//! Initialise the number chosen to the respective ability score
		arrAbilityScores[i] = chosenScr;
		cout << "\n";
	}

	//! Add remaining ability score
	for (int i = 0; i < Character::NO_ABILITY; i++){
		if (arrRand[i] != 0){
			arrAbilityScores[Character::NO_ABILITY - 1] = arrRand[i];
			break;
		}
	}



	if (choice == 1){
		//! Create Character Object
		currentCharacter = new Character(name, "1d10", arrAbilityScores, level, (CharacterSize)size);
		if (!currentCharacter->validateNewCharacter()){ //! Validate
			//! If invalid character
			cout << "Incorrect Character. Character will not be saved";
			system("pause");
			currentCharacter = NULL;
			return;
		}
	}
	else{
		//! Create fighter
		currentCharacter = new Fighter(name, arrAbilityScores, (FightStyle)fightStyle, level, (CharacterSize)size);
		if (!currentCharacter->validateNewCharacter()){ //! Validate{
			//! If incorrect Fighter end program
			cout << "Incorrect Fighter.  Character will not be saved";
			system("pause");
			currentCharacter = NULL;
			return;
		}
	}

	bool flagCorrectChoice = false;
	string itemChoice = "";
	cout << "Do you want to add items to your backpack? (Y/N) ";
	while (!flagCorrectChoice){
		cin >> itemChoice;
		if (itemChoice == "Y" || itemChoice == "N")
			flagCorrectChoice = true;
		else
		{
			cout << "Incorrect choice. Please enter Y or N: ";
		}
	}

	if (itemChoice == "Y"){
		addItem();
	}

	if (choice == 1){
		cout << "Character Created!\nThe Stats are:\n" << *currentCharacter;
	}
	else{
		cout << "Fighter Created!\nThe Stats are:\n" << *currentCharacter;
	}

	saveCharacter();

	system("pause");

}

void CharacterController::editCharacter(){
	cout << "====  Character Edition ====" << endl << endl;

	vector<string> chrFiles = getFilesInsideFolderNoExtension("SaveFiles/Characters/");

	if (chrFiles.size() == 0) {
		cout << "No Character has yet been created.\n";
		system("pause");
		return;
	}
	cout << "The saved files available are: \n";
	for (auto i : chrFiles) {
		cout << i << "\n";
	}
	string fileName = "";
	bool fileNameCorrect = false;
	cout << "\nPlease enter the name of the Character you want to load: ";
	while (!fileNameCorrect) {
		cin >> fileName;
		if (find(chrFiles.begin(), chrFiles.end(), fileName) != chrFiles.end()) {
			currentCharacter = new Character(*readCharacterFile("SaveFiles/Character", fileName));
			fileNameCorrect = true;
		}
		else {
			cout << "File name does not exist. Try again: ";
		}
	}

	system("cls");
	if (dynamic_cast<Fighter*>(currentCharacter))
		cout << *(dynamic_cast<Fighter*>(currentCharacter));
	else
		cout << *currentCharacter;

	int menuChoice = 0;
	string menuChoiceStr;

	while (menuChoice!=10){
		cout << "Currently edited character: " << currentCharacter->getName() << endl << endl;
		cout << "Please choose from the following:\n"
			<< "1 - Save Character\n"
			<< "2 - Edit Name\n"
			<< "3 - Edit Level\n"
			<< "4 - Edit Size\n"
			<< "5 - Add Item\n"
			<< "6 - Remove Item\n"
			<< "7 - Equip Item\n"
			<< "8 - Unequip Item\n"
			<< "9 - Edit Fighting Style\n"
			<< "10 - Return\n";
		cout << "Please choose: ";
		cin >> menuChoiceStr;
		try {
			menuChoice = stoi(menuChoiceStr);
		}
		catch (...) {
			cout << "Enter a valid input." << endl;
			continue;
		}
		system("cls");

		bool styleChangeCorrect = true;
		switch (menuChoice){
			case 1:
				saveCharacter();
				break;
			case 2:
			{
					  string newName = "";
					  cout << "Please enter the new Character name: ";
					  cin >> newName;
					  currentCharacter->setName(newName);
					  cout << "Name set.\n";
					  cout << *currentCharacter;
					  break;
			}
			case 3:
			{
					  int level = 0;
					  cout << "Please enter the new Character level (>1): ";
					  while (level < 1){
						cin >> level;
						if (!cin){
							cin.clear();
							cin.ignore();
						}
						if (level < 1){
							cout << "Incorrect Level. Try again (-1 to stop): ";
						}
						if (level == -1){
							break;
						}
					  }
					  if (level != -1){
						currentCharacter->setLevel(level);
						cout << "Level changed.";
					  }
					  else{
						  cout << "Level not changed.";
					  }
					  break;
			}
			case 4:{
		
					   int size = -1;
					   cout << "Please enter the new Character Size by selecting from the list below:\n";
					   displayCharacterSize();
					   while (size < 0 || size>3){
						   cin >> size;
						   if (!cin){
							   cin.clear();
							   cin.ignore();
						   }
						   if (size < 0 || size>3){
							   cout << "Incorrect Size. Try again (-1 to stop): ";
						   }
						   if (size == -1){
							   break;
						   }
					   }
					   if (size != -1){
						   currentCharacter->setSize((CharacterSize)size);
						   cout << "Size changed.";
					   }
					   else{
						   cout << "Size not changed.";
					   }
					   
					   break;
			}
			case 5:{

					   addItem();
					   break;
			}
			case 6:{
					   vector <Item*> backItem;

						backItem = currentCharacter->getBackpackContents();

					   if (backItem.size() == 0)
						   cout << "\n:( There are no item in the backpack!!!";
					   else{

						   int itemID = 0;
						   bool flagCorrect = false;
						   cout << "\nHere are the items that are in your backpack: ";
						   for (int i = 0; i < backItem.size(); i++){
							   cout << "\nItem ID: " << i << "\n" << *backItem[i];
						   }

						   cout << "\nPlease enter the correct item ID: ";
						   //! Ask user to choose item
						   while (itemID != -1 && !flagCorrect){

							   cin >> itemID;
							   if (!cin){
								   cin.clear();
								   cin.ignore();
							   }

							   //! Check id entered
							   if (itemID >= 0 && itemID<backItem.size()){

									  //! If Wearing item was ok
									   if (!currentCharacter->removeItemBack(backItem[itemID])){
										   cout << "\nError. Try Again or else enter -1 to exit: ";
									   }
									   else{
										   cout << "Item Removed\n";
										   flagCorrect = true;
									   }

							   }
							   else{
								   //! Asks id for item again since incorrectly entered
								   cout << "\nIncorrect Item Id entered. Try Again or else enter -1 to exit: ";

							   }
						   }

					   }

					   break;
			}
			case 7:
			{
					  vector<Item*> backItem; //! Get backpack contents
						 backItem = currentCharacter->getBackpackContents();
					  if (backItem.size() == 0) //! Check if there are items in backpack
						  cout << "\n:( There are no Item that you can equip with!!! Try adding a new item first.\n";

					  else{

						  int itemID = 0; //! Identify each item in backpack
						  bool flagCorrect = false;
						  cout << "\nHere are the item(s) in your backpack that you can equip yourself with: ";

						  //! output backcpack contents with id
						  for (int i = 0; i < backItem.size(); i++){
							  cout << "\nITEM ID: " << i << "\n" << *backItem[i];
						  }

						  cout << "\nPlease enter the correct item ID: ";
						  //! Ask user to choose item
						  while (itemID != -1 && !flagCorrect){

							  cin >> itemID;
							  if (!cin){
								  cin.clear();
								  cin.ignore();
							  }

							  //! Check id entered
							  if (itemID >= 0 && itemID<backItem.size()){
									//! If Wearing item was ok
									if (!currentCharacter->wearItem(backItem[itemID])){
										cout << "\nError. Try Again or else enter -1 to exit: ";
									}
									else{
										cout << "Item Equiped\n";
										flagCorrect = true;
									}

							  }
							  else{
								  //! Asks id for item again since incorrectly entered
								  cout << "\nIncorrect Item Id entered. Try Again or else enter -1 to exit: ";

							  }
						  }
					  }

					  break;
			}
			case 8:{
					   //! Get contents of currently worn item
					   vector<Item*> holdItem;
						holdItem = currentCharacter->getCurrentWornItems();
					   //! check if there are items
					   bool wearingItem = false;
					   for (auto i : holdItem){
						   if (i->getItemTypes() != ItemType::UNSPECIFIED)
							   wearingItem = true;
					   }

					   if (!wearingItem)
						   //! Telle user there are no items currently wearing
						   cout << "\n:( You are wearing no items!!! Add a new one or equip yourself first.\n";

					   else{

						   int itemID = 0;
						   bool flagCorrect = false;
						   cout << "\nHere are the item(s) that you are currently wearing:";
						   //! output all currently worn items with an id
						   for (int i = 0; i < holdItem.size(); i++){
							   if (holdItem[i]->getItemTypes() != ItemType::UNSPECIFIED){
								   cout << "\nITEM ID: " << i << "\n" << *holdItem[i];
							   }
						   }

						   cout << "\nPlease enter the correct item ID: ";
						   //! ask user to enter id
						   while (itemID != -1 && !flagCorrect){

							   cin >> itemID;
							   if (!cin){
								   cin.clear();
								   cin.ignore();
							   }

							   //! Check id
							   if (holdItem[itemID]->getItemTypes() != ItemType::UNSPECIFIED){

									   //! Take off item
									   if (!currentCharacter->takeOffItem(holdItem[itemID])){
										   cout << "\nError. Try Again or else enter -1 to exit: ";
									   }
									   else{
										   cout << "Item Unequiped \n";
										   flagCorrect = true;
									   }

							   }
							   else{
								   //! Ask to reenter id if incorretly entered
								   cout << "\nIncorrect Item Id entered. Try Again or else enter -1 to exit: ";

							   }
						   }
					   }

					   break;
			}
			case 9:{
					   if (!dynamic_cast<Fighter*>(currentCharacter)){
						   cout << "Not a fighter. Cannot modify.";
						   styleChangeCorrect = false;
						   break;
					   }

					   int style = -1;
					   cout << "Please enter the new Fighter style selecting from the list below:\n";
					   displayFighterStyle();
					   while (style < 0 || style>3){
						   cin >> style;
						   if (!cin){
							   cin.clear();
							   cin.ignore();
						   }
						   if (style < 0 || style>3){
							   cout << "Incorrect Style. Try again (-1 to stop): ";
						   }
						   if (style == -1){
							   break;
						   }
					   }
					   if (style != -1){

						   dynamic_cast<Fighter*>(currentCharacter)->setStyle((FightStyle)style);
						   cout << "Style changed.";
					   }
					   else{
						   cout << "Style not changed.";
					   }

					   break;
			}
			case 10:
				break;
			
			default:
				cout << "Incorrect Menu choice. Try Again." << endl;
				continue;			
		}

		if (menuChoice != 10){
			if (menuChoice != 1 && styleChangeCorrect)
				saveCharacter();
			system("pause");
			system("cls");
		}

	}
	
}

void CharacterController::saveCharacter(){

	cout << "Saving the character...\n";

	vector<string> chrFiles = getFilesInsideFolderNoExtension("SaveFiles/Characters/");

	bool foundFile = false;
	for (auto i : chrFiles){
		if (i == currentCharacter->getName()){
			foundFile = true;
			break;
		}
	}

	if (chrFiles.size() ==0 || !foundFile){
		currentCharacter->saveCharacter();
		cout << "Saved Successfully!\n";
	}
	else{
		string choiceOverw = "";
		cout << "A file with the same name already exists. Do you want to overwrite it? (Y/N) ";
		cin >> choiceOverw;
		while (choiceOverw != "Y" && choiceOverw != "N"){
			cout << "Incorrect choice. Please enter again: ";
			cin >> choiceOverw;
		}
		if (choiceOverw == "Y")
		{
			currentCharacter->saveCharacter();
			cout << "Overwritten Successfully!\n";
		}
		else{
			cout << "File not overwritten.\n";
		}
	}

}

Character* readCharacterFile(string charFileLocation, string charName){

	ifstream inStream("SaveFiles/Characters/" + charName + ".txt", ios::in);

	if (!inStream){
		return nullptr;
	}

	Character * tempCharacter = nullptr;

	string chrType = "";
	string strSize = "";
	string name;
	string hitDice;
	int level;
	int fightStyle;

	inStream >> chrType >> name >> hitDice >> level >> strSize >> fightStyle;

	int abilityScr[Character::NO_ABILITY];
	for (int i = 0; i < Character::NO_ABILITY; i++){
		inStream >> abilityScr[i];
	}

	if (chrType == "character")
		tempCharacter = new Character(name, hitDice, abilityScr, level, (CharacterSize)stoi(strSize));
	else
		tempCharacter = new Fighter(name, abilityScr, (FightStyle)fightStyle, level, (CharacterSize)stoi(strSize));

	string itemName;
	inStream.ignore();
	getline(inStream, itemName);
	getline(inStream, itemName);

	Item *itmPoint = nullptr;
	
	while (itemName!="wornItem"){

		itmPoint = readItemFile(itemName);
		tempCharacter->storeItem(itmPoint);
		getline(inStream, itemName);

	}

	for (int i = 0; i < 7; i++){
		getline(inStream, itemName);
		if (itemName!="UNSPECIFIED"){
			itmPoint = readItemFile(itemName);
			tempCharacter->storeItem(itmPoint);
			tempCharacter->wearItem(itmPoint);
		}
	}

	delete itmPoint;

	inStream.close();

	return tempCharacter;
}

void CharacterController::displayCharacterSize(){

	cout << (int)CharacterSize::FINE << " - FINE\n"
		<< (int)CharacterSize::TINY << " - TINY\n"
		<< (int)CharacterSize::HUGE << " - HUGE\n"
		<< (int)CharacterSize::COLOSSAL << " - COLOSSAL\nPlease choose: ";

}

void CharacterController::displayFighterStyle(){

	cout << (int)FightStyle::ARCHERY << " - " << FightStyle::ARCHERY << "\n"
		<< (int)FightStyle::DEFENSE << " - " << FightStyle::DEFENSE << "\n"
		<< (int)FightStyle::DUELING << " - " << FightStyle::DUELING << "\n";

}


void CharacterController::addItem(){

		vector<string> filesInFolder = getFilesInsideFolderNoExtension("SaveFiles/Items");

		if (filesInFolder.size() == 0){
			cout << "There are no items files available. Please create items and then edit the character to add the items.\n";
			cin.ignore();
			system("pause");
		}
		else{
			Item* itmPoint = NULL;
			int itemID = 0;

			while (itemID != -1 && filesInFolder.size() != 0){

				cout << "The item template files available are:\n";
				for (int i = 0; i < filesInFolder.size(); i++){
					cout << i << " - " << filesInFolder[i] << "\n";
				}

				cout << "\nPlease enter the Item file name (-1 to stop adding): ";
				cin >> itemID;
				if (itemID >= 0 && itemID<filesInFolder.size()){

					bool itemFound = false;
					for (auto i : currentCharacter->getBackpackContents()){
						if (i->getItemName() == filesInFolder[itemID])
							itemFound = true;
					}

					if (!itemFound){
						itmPoint = readItemFile(filesInFolder[itemID]);
						currentCharacter->storeItem(itmPoint);
						cout << "Item Added.\n";
						filesInFolder.erase(filesInFolder.begin() + itemID);
					}
					else{
						cout << "Item already in backpack. Cannot add again.\n";
						filesInFolder.erase(filesInFolder.begin() + itemID);
					}
				}
				else if (itemID != -1){
					cout << "Incorrect No. Please try again (-1 to stop): ";
				}

			}

			delete itmPoint;

		}
	

}

//! function to check if a number is in an array
//! @param array pointer - array to check
//! @param number - number to check if in array
//! @return true - if number is in array, false - if not
bool CharacterController::checkScr(int *arr, int scr){

	//! If number is 0 return false (no. already used in array/element does not exist)
	if (scr == 0)
		return false;

	//! for loop to check for number in each element
	for (int i = 0; i < Character::NO_ABILITY; i++){
		if (*(arr + i) == scr) //! Use pointer to look through array
		{
			*(arr + i) = NULL; //! Fag that number has been used
			return true; //! Return true if found
		}
	}
	return false; //! Return false if not found

}

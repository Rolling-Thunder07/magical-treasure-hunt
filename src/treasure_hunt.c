/* 🧙‍♂ MAGICAL TREASURE HUNT GAME 🏆
 * A text-based adventure where you explore a castle,
 * answer wizards' riddles, and find the hidden treasure!
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// COLOR CODING FOR TEXT - SIMPLIFIED PALETTE
#define RED "\033[1;31m"     // For warnings, errors, walls
#define GREEN "\033[1;32m"   // For success messages and valid paths
#define YELLOW "\033[1;33m"  // For important highlights and treasures
#define BLUE "\033[1;34m"    // For titles and headings
#define NORMAL "\033[0m"     // Reset to default terminal color
#define BOLD "\033[1m"       // Bold text for emphasis

// 📝 Function Prototype Declarations
int get_player_choice(int);
void DisplayMap(int);
void setupQuestions();
void divider();
void Introduction();
void PushLocation(int);
int checkForTreasure(int);
int getIntegerInput(int);

// 🌍 Global Variables
int steps=1;        // Steps counter to track player's journey
int stackLength = 0; // How many rooms discovered
int lives=2;        // ❤ ❤ Player's lives

// 🏰 Location Structure Definition
struct Location{
  int id;
  char name[50];        // Location name
  char details[200];    // Location description
  int north;            // IDs of adjacent locations (-1 if no connection)
  int south;
  int east;
  int west;
  int hasWizard;        // ID of wizard at this location (-1 if none)
  int visited;          // Whether player has visited before
};
typedef struct Location Location;
Location locations[10]; // 10 magical locations to explore

// 🧙‍♂ Creates the castle with all its magical locations
void Fill_Locations_Details(){
  // Initialize all locations with default values
  for(int i=0;i<10;i++){
    locations[i].id=i;
    locations[i].north = -1;
    locations[i].south = -1;
    locations[i].east = -1;
    locations[i].west = -1;
    locations[i].hasWizard = -1;
    locations[i].visited = 0;
  }
  
  // 🚪 Location 0: Castle Entrance
  strcpy(locations[0].name, "Castle Entrance");
  strcpy(locations[0].details, "You stand before an ancient castle. The massive wooden doors creak in the wind.");
  locations[0].north=8;
  locations[0].east=3;
  
  // 📚 Location 1: Ancient Library
  strcpy(locations[1].name, "Ancient Library");
  strcpy(locations[1].details,"Dusty bookshelves line the walls. Arcane symbols are etched into the floor.");
  locations[1].hasWizard = 2;
  locations[1].north=6;
  locations[1].south=3;
  locations[1].east=5;
  locations[1].west=8;
  
  // ⚰ Location 2: Ancient Crypt
  strcpy(locations[2].name, "Ancient Crypt");
  strcpy(locations[2].details,"Stone sarcophagi line the walls of this cold, eerie chamber.");
  locations[2].hasWizard = 1;
  locations[2].west = 5;
  locations[2].north = 7;
  
  // 🌿 Location 3: Eastern Garden
  strcpy(locations[3].name,"Eastern Garden");
  strcpy(locations[3].details,"An overgrown garden with strange glowing plants and stone statues.");
  locations[3].north = 1;
  locations[3].west = 0;
  
  // 🗼 Location 4: Tower Base
  strcpy(locations[4].name, "Tower Base");
  strcpy(locations[4].details,"The base of a crumbling stone tower. A spiral staircase leads upward.");
  locations[4].hasWizard = 0;
  locations[4].south = 8;
  locations[4].east = 6;
  
  // 🔗 Location 5: Dungeon
  strcpy(locations[5].name, "Dungeon");
  strcpy(locations[5].details,"A damp, dark dungeon with rusted chains hanging from the walls.");
  locations[5].hasWizard = 3;
  locations[5].north = 9;
  locations[5].east = 2;
  locations[5].west = 1;
  
  // 🧪 Location 6: Alchemist's Laboratory
  strcpy(locations[6].name, "Alchemist's Laboratory");
  strcpy(locations[6].details,"Bubbling potions and strange apparatus fill this room with odd smells.");
  locations[6].south = 1;
  locations[6].east = 9;
  locations[6].west = 4;
  
  // 💎 Location 7: Treasure Room
  strcpy(locations[7].name, "Treasure Room");
  strcpy(locations[7].details,"A glittering chamber filled with gold, gems, and ancient artifacts.");
  // locations[7].hasTreasure= 4;
  locations[7].south = 2;
  locations[7].west = 9;
  
  // 🏛 Location 8: Great Hall
  strcpy(locations[8].name, "Great Hall");
  strcpy(locations[8].details,"A vast hall with faded tapestries depicting ancient battles.");
  locations[8].north = 4;
  locations[8].south = 0;
  locations[8].east = 1;
  
  // 🔮 Location 9: Secret Chamber
  strcpy(locations[9].name, "Secret Chamber");
  strcpy(locations[9].details,"A hidden room with arcane symbols glowing on the walls.");
  locations[9].hasWizard = 4;
  locations[9].south = 5;
  locations[9].east = 7;
  locations[9].west = 6;
}

// 🧩 Question Structure for Wizards' Riddles
struct Question{
  char text[200];
  char options[4][100];
  int correctAnswer; // stores the index of correct option(0-3)
};
struct Question typedef Question;

// 🧙‍♂ Wizard Structure Definition
struct Wizard{
  int id;
  char name[50];
  Question question;
  int answered; // 1 if player answered correctly otherwise 0
};
struct Wizard typedef Wizard;

// 🧙‍♂ Global variables for wizards
Wizard wizards[5]; // 5 powerful wizards with unique riddles

// 🔮 Setup the riddles for each wizard
void setupQuestions(){

  // 🧙‍♂ Wizard 0: Albus Dumbledore
  wizards[0].id = 0;
  strcpy(wizards[0].name,"Albus Dumbeldore");
  strcpy(wizards[0].question.text,"What charm is used to make objects levitate?");
  strcpy(wizards[0].question.options[0],"Alohomora");
  strcpy(wizards[0].question.options[1],"Wingardium Leviosa");
  strcpy(wizards[0].question.options[2],"Accio");
  strcpy(wizards[0].question.options[3],"Expelliarmus");
  wizards[0].question.correctAnswer = 1; //Wingardium Leviosa
  wizards[0].answered = 0;

  // 🧙‍♂ Wizard 1: Severus Snape
  wizards[1].id = 1;
  strcpy(wizards[1].name,"Severus Snape");
  strcpy(wizards[1].question.text,"Which of these is NOT a real Horcrux made by Voldemort?");
  strcpy(wizards[1].question.options[0],"Nagini");
  strcpy(wizards[1].question.options[1],"The Sorting Hat");
  strcpy(wizards[1].question.options[2],"Hufflepuff's Cup");
  strcpy(wizards[1].question.options[3],"Ravenclaw's Diadem");
  wizards[1].question.correctAnswer = 1; //The Sorting Hat
  wizards[1].answered = 0;

  // 🧙‍♀ Wizard 2: Minerva McGonagall
  wizards[2].id = 2;
  strcpy(wizards[2].name,"Minerva McGonagall");
  strcpy(wizards[2].question.text,"What can travel around the world while staying in a corner?");
  strcpy(wizards[2].question.options[0],"A stamp");
  strcpy(wizards[2].question.options[1],"The sun");
  strcpy(wizards[2].question.options[2],"A thought");
  strcpy(wizards[2].question.options[3],"A spider");
  wizards[2].question.correctAnswer = 0; //A stamp
  wizards[2].answered = 0;

  // 🧔 Wizard 3: Rubeus Hagrid
  wizards[3].id = 3;
  strcpy(wizards[3].name,"Rubeus Hagrid");
  strcpy(wizards[3].question.text,"Which of these creatures is known to sing before it dies?");
  strcpy(wizards[3].question.options[0],"Thestral");
  strcpy(wizards[3].question.options[1],"Hippogriff");
  strcpy(wizards[3].question.options[2],"Phoenix");
  strcpy(wizards[3].question.options[3],"Niffler");
  wizards[3].question.correctAnswer = 3; //Phoenix
  wizards[3].answered = 0;

  // 🧙‍♂ Wizard 4: Lord Voldemort
  wizards[4].id = 4;
  strcpy(wizards[4].name,"Lord Voldemort");
  strcpy(wizards[4].question.text,"Which spell is used to kill instantly, without causing physical harm?");
  strcpy(wizards[4].question.options[0],"Crucio");
  strcpy(wizards[4].question.options[1],"Expelliarmus");
  strcpy(wizards[4].question.options[2],"Imperio");
  strcpy(wizards[4].question.options[3],"Avada Kedavra");
  wizards[4].question.correctAnswer = 3; //Avada Kedavra
  wizards[4].answered = 0;
}

// 📜 Displays the introduction story to the player
void Introduction(){
  printf("%s✨✨✨ Welcome, brave adventurer! ✨✨✨%s\n\n", YELLOW, NORMAL);
  printf("You stand before an ancient castle rumored to hide a legendary treasure.\n");
  printf("To find it, you must navigate through mysterious rooms, encounter wizards,\n");
  printf("and answer their riddles to open new pathways.\n\n");
  printf("%sYour goal is to find the treasure using the shortest path possible.\n", YELLOW);
  printf("Choose your route wisely!%s\n\n", NORMAL);
  printf("Press Enter to begin your adventure... 🏰 ");
  getchar();
}

// 🧭 Display available directions from current room
void directions_of_room(int id){
  printf("%s",BLUE);
  printf("🧭 Exits in this Location : \n");
  printf("%s",NORMAL);

  if(locations[id].north==-1){
    printf("%s⬆ North : Wall%s\n", RED, NORMAL);}
  else {
    printf("⬆ North : %s\n",locations[locations[id].north].name);}
  if(locations[id].south==-1){
    printf("%s⬇ South : Wall%s\n", RED, NORMAL);}
  else {
    printf("⬇ South : %s\n",locations[locations[id].south].name);}
  if(locations[id].east==-1){
    printf("%s➡ East : Wall%s\n", RED, NORMAL);}
  else {
    printf("➡ East : %s\n",locations[locations[id].east].name);}
  if(locations[id].west==-1){
    printf("%s⬅ West : Wall%s\n\n", RED, NORMAL);}
  else {
    printf("⬅ West : %s\n\n",locations[locations[id].west].name);}
} 

// 🔍 Display room details and handle wizard encounters
int display_room_details(int id){
  printf("\n");
  divider();
  printf("%s",BLUE);
  printf("                                                       🏰 %s",locations[id].name);
  printf("%s",NORMAL);
  divider();
  printf("%s\n",locations[id].details);
  locations[id].visited=1;
  steps++;

  directions_of_room(id);

  // 🧙‍♂ Handle wizard encounter if one exists in this room
  if(locations[id].hasWizard!=-1 && !wizards[locations[id].hasWizard].answered){
    int playerAnswer;

    printf("%s",BLUE);
    printf("\n✨ A wizard appears before you! ✨\n");
    printf("🧙 %s: \"Answer my riddle to proceed, traveler.\"\n\n",wizards[locations[id].hasWizard].name);
    printf("%s",NORMAL);

    printf("%s❓ %s%s\n", YELLOW, wizards[locations[id].hasWizard].question.text, NORMAL);
    printf("1️⃣ %s\n",wizards[locations[id].hasWizard].question.options[0]);
    printf("2️⃣ %s\n",wizards[locations[id].hasWizard].question.options[1]);
    printf("3️⃣ %s\n",wizards[locations[id].hasWizard].question.options[2]);
    printf("4️⃣ %s\n",wizards[locations[id].hasWizard].question.options[3]);
    printf("\nYour answer (1-4): ");
    scanf("%d",&playerAnswer);

    if(playerAnswer<=0 || playerAnswer>4){
      getchar(); // Clear the newline character
      printf("%s 😵‍💫 That's not even close, champ. Try a number between 1 and 4, pretty please. %s\n", RED, NORMAL);
      playerAnswer=getIntegerInput(4);
    }
    playerAnswer--;
    
    if(playerAnswer!=wizards[locations[id].hasWizard].question.correctAnswer){
      lives--;
      printf("%s🧙 %s: Wrong Anwer!%s\n%s❤ You have %d lives remaining.%s\n", RED, wizards[locations[id].hasWizard].name, NORMAL, RED, lives, NORMAL);
      if(lives==0){
        printf("%s💀 Better luck next time, champ! Maybe Google the answers first? %s", RED, NORMAL);
        return -1;
      }
    }
    else{
      printf("%s🧙 %s: Correct! You may proceed on your quest.%s\n", GREEN, wizards[locations[id].hasWizard].name, NORMAL);
      wizards[locations[id].hasWizard].answered=1;
    }
  }
  int t=get_player_choice(id);
  return t;
}

// 🎮 Get player movement choice
int get_player_choice(int id){
  int choice;

  do{
    printf("\n%s🎮 What would you do?%s\n", BLUE, NORMAL);
    printf("1. Go North ⬆\n");
    printf("2. Go South ⬇\n");
    printf("3. Go East ➡\n");
    printf("4. Go West ⬅\n");
    printf("5. Map 🗺\n");
    printf("6. Exit 🚪\n");
    printf("Enter Choice:  ");
    scanf("%d",&choice);

    int x;
    do{
      x=-2;
      switch (choice){
        case 1:
          x=locations[id].north;
          break;
        case 2:
          x=locations[id].south;
          break;
        case 3:
          x=locations[id].east;
          break;
        case 4:
          x=locations[id].west;
          break;
        }

      if(x==-1)
        printf("%s🧱 You hit the wall!%s\n", RED, NORMAL);
      
      if((choice<1 || choice>6) || (x==-1) )
        choice=getIntegerInput(6);
        
    }while((choice<1 || choice>6) || (x==-1));
    
      switch (choice){
        case 1:
          return locations[id].north;
          break;
        case 2:
          return locations[id].south;
          break;
        case 3:
          return locations[id].east;
          break;
        case 4:
          return locations[id].west;
          break;
        case 5:
          DisplayMap(id);
          divider();
          directions_of_room(id);
          break;
        case 6:
          printf("\n👋 Thank you for playing!\n");
          return -1;
          break;
        }
  }while(choice==5);
}

// 🔢 Get and validate integer input from player
int getIntegerInput(int range){
  int number,status;
  do{
    printf("Enter an integer between 1 to %d: ", range);
    status=scanf("%d",&number);

    if(status!=1){
      while(getchar()!='\n');
      printf("%s⚠ Integers only, please! This isn't rocket science! %s\n", RED, NORMAL);
    }
    else{
      while(getchar()!='\n');
      return number;
    }
  }while(1);
}

// 💎 Check if player has found the treasure
int checkForTreasure(int id){
  if(id==7){
    printf("%s",YELLOW);
    printf("\n🎉🎉🎉 *** CONGRATULATIONS! *** 🎉🎉🎉\n");
    printf("💎 You have found the treasure and completed your quest! 💎\n\n");
    printf("%s",NORMAL);
    printf("%sRemaining lives: ❤ %d %s\n", GREEN, lives, NORMAL);
    PushLocation(id);
    DisplayMap(id);
    printf("\n🚶 You completed the adventure in total %d steps!\n", steps);
    printf("🧭 And you discovered %d rooms\n", stackLength);
    printf("\n👋 Thank you for playing!\n");
    return -1;
  }
  return -2;
}

// 📋 Stack implementation for Player's Path
typedef struct node{
  int id,count;
  struct node *next;
}PathStack;
PathStack *head=NULL,*tail=NULL;
PathStack Path;

// 📝 Add location to player's path history
void PushLocation(int currentLocation){
  PathStack *ptr=head;
  PathStack *temp = (PathStack *)malloc(sizeof(PathStack));
  temp->id = currentLocation;
  temp->next = NULL;
  if(head==NULL){
    head=temp;
    tail=temp;
  }
  else{
    while(ptr!=NULL){
      if (ptr->id==currentLocation)
      return;
      ptr = ptr->next;
    }
    temp->next = head;
    head = temp;
  }
  stackLength++;
  head->count = stackLength;
}

// 📏 Print divider line for UI formatting
void divider(){
  printf("\n=======================================================================================================================\n");
}

// 🗺 Display the map of discovered locations
void DisplayMap(int currentLocation){
  printf("\n");
  divider();
  printf("%s                                                          🗺 MAP%s", BLUE, NORMAL);
  divider();
  int i=0;
  PathStack *ptr=head;

  if(head==NULL){
    printf("%sYou haven't explored anywhere yet, brave explorer! %s\n", RED, NORMAL);
  }
  else{
    while(ptr!=NULL){
      if (ptr->id==currentLocation){
      printf("%s%d. %s ----> 👉 YOU ARE HERE NOW%s\n", GREEN, ptr->count, locations[ptr->id].name, NORMAL);}
      else{
        printf("%d. %s\n", ptr->count, locations[ptr->id].name);
      }
      ptr = ptr->next;
    }
  }
}

// 🎮 Main game function
int main(){
  time_t start=time(NULL), end;
  srand((unsigned)time(&start));
  char *string=ctime(&start);

  int currentLocation = 0;
  Fill_Locations_Details();
  setupQuestions();

  printf("\n");
  printf("%s==============================================\n", BLUE);
  printf("||                                          ||\n");
  printf("||      ✨ MAGICAL TREASURE HUNT ✨         ||\n");
  printf("||                                          ||\n");
  printf("==============================================\n%s", NORMAL);
  printf("\n");

  Introduction();
  printf("\n%s-----🏰 You stand before the Castle Entrance! 🏰-----%s\n", YELLOW, NORMAL);
  printf("\n------Press ENTER to enter in the Castle ! 🚪-----");
  getchar();

  int gameover = 0;
  while(gameover!=-1){
    PushLocation(currentLocation);
    if (currentLocation>=0 && currentLocation<10){
    currentLocation = display_room_details(currentLocation);}
    gameover = currentLocation;
    if(gameover!=-1)
      gameover = checkForTreasure(currentLocation);
  }

  end=time(NULL);
  printf("⏱ Total time taken %d sec\n", end-start);
  printf("🕒 %s", string);
  return 0;
}
/* 
 * File:   Project2.cpp
 * Author: Ryan Ramos
 * Created on January 31, 2023, 2:04 PM
 * Purpose:  Project 2
 */

//System Libraries
#include <iostream>  //Input Output Library
#include <iomanip>
#include <random>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

//User Libraries

//Global Constants not Variables 
//Science, Math, Conversions, Dimensions

//Function Prototypes
void assign(const string[], const string[], string[], int[], int = 2);
void reassign(const string[], const string[], string[], int[], int = 2);
void displayCards(const string[], const string[], const int[], const int, const string[], const int[]);
void displayTotals(int [], int [], int &, int &, int = 2);
void displayRoundWinner(const int, const int, int [][2], int &);
void postGame(const int [][2], ofstream &);
void promptAce(int[], string, int = 2);
void promptAgain(char, const int, bool &);

bool compare(const string[], const int[], const string[], const int[], int = 2);
bool isAce(const int[], string &, int &, int = 2);

void Menu();
void GameTitle();
void fillDeck(vector<int> , const int); //FILL THE DECK
void orderDeck(vector <int>, const int, int); //SELECTION SORT
void orderDeck(vector <int>, const int, string); //BUBBLE SORT

//Execution begins here at main
int main(int argc, char** argv) {
    //Set random number seed
    srand(time(0));
    
    //Declare Variables
    ofstream Record ("record.txt");
    const int deckSz = 52;
    const int playerCount = 2;
    int playerIdentifier = -1;
    int enemyIdentifier  = -2;
    
    string suits[] = {"of Spades", "of Hearts", "of Diamonds", "of Clubs"};
    string cards[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                        "Jack", "Queen", "King"};
    
    vector<int> Deck(deckSz);
    
    //PLAYER CARDS
    int faceVal[deckSz];
    string whichSt[deckSz];
    faceVal[50] = playerIdentifier;
    
    //ENEMY CARDS
    int EfaceVal[deckSz];
    string EwhichSt[deckSz];
    EfaceVal[50] = enemyIdentifier;
    
    //COLLECTED CARDS
    int Collected[playerCount][playerCount]; //ROW & COLUMN 1 = PLAYER, ROW & COLUMN 2 = ENEMY
    Collected[0][0] = 0;
    Collected[1][1] = 0;
    
    //WIN/LOSS/TIE COUNTERS
    int winCount = 0, //AMOUNT OF WINS THE PLAYER HAS OBTAINED, ADDS TO A TEXT FILE
        tieCount = 0, //AMOUNT OF TIMES THE PLAYER AND ENEMY HAS TIED.
        enemyWinCount = 0; //AMOUNT OF WINS THE ENEMY HAS OBTAINED, ADDS TO A TEXT FILE
        
    //OTHER
    bool gameOver = false; //BOOL TO CHECK IF GAME IS OVER
    
    string who;
    string player = "Player";
    string enemy  = "Enemy";
    
    char again;
    
    int playerTotal, enemyTotal;
    int deckRem = 52;
    int ruleSet = 2;
    int index;
    
    float roundCount = static_cast<float>(ruleSet);22
    
                            //MENU AND TITLE
    Menu();
    GameTitle();
    
                            //THE GAME
    do{
        //ASSIGN PLAYERS VALUES
        assign(suits, cards, whichSt, faceVal, ruleSet);
        //ASSIGN ENEMY VALUES 
        assign(suits, cards, EwhichSt, EfaceVal, ruleSet);
        
        //COMPARE CARDS TO SEE IF THEY'RE EQUAL, IF THEY ARE, CHANGE THEM
        bool matched = compare(whichSt, faceVal, EwhichSt, EfaceVal, ruleSet);
        
        while(matched){
            reassign(suits, cards, whichSt,  faceVal,  ruleSet);
            reassign(suits, cards, EwhichSt, EfaceVal, ruleSet);
        }
            
        //DISPLAY PLAYER AND ENEMY CARDS
        displayCards(cards, whichSt, faceVal, ruleSet, EwhichSt, EfaceVal);
        
        bool pHasAce = isAce(faceVal,  who, index, ruleSet);
        bool eHasAce = isAce(EfaceVal, who, index, ruleSet);
        
        //CHECK IF ANYBODY GOT ACES
        if(pHasAce)
            promptAce(faceVal, player);
        
        if(eHasAce)
            promptAce(EfaceVal, enemy);
        
        //DISPLAY CARD VALUE TOTALS
        displayTotals(faceVal, EfaceVal, playerTotal, enemyTotal, ruleSet);
        
        //DISPLAY WINNER
        displayRoundWinner(playerTotal, enemyTotal, Collected, deckRem);
        
        //CHECK IF GAME IS OVER, ASK TO GO AGAIN
        promptAgain(again, deckRem, gameOver);
        
    }while(!gameOver);
    
                            //DISPLAY POST GAME STATS
    postGame(Collected, Record);
}

//ASSIGN CARDS VALUES
void assign(const string suits[], const string cards[], string whichSt[], int faceVal[], int ruleSet){ 
    //FOR THE AMOUNT OF CARDS,
    for(int i = 0; i < ruleSet; i++){
        int randCrd = rand()%13+1;
        int randSt  = rand()%4+1;
        
        faceVal[i] = randCrd - 1; // ASSIGN FACE VALUE
        whichSt[i] = suits[randSt - 1]; // ASSIGN SUIT
    }
}

//REASSIGN CARDS VALUE
void reassign(const string suits[], const string cards[], string whichSt[], int faceVal[], int ruleSet){ 
    //FOR THE AMOUNT OF CARDS,
    for(int i = 0; i < ruleSet; i++){
        int randCrd = rand()%13+1;
        int randSt  = rand()%4+1;
        
        faceVal[i] = randCrd - 1; // ASSIGN FACE VALUE
        whichSt[i] = suits[randSt - 1]; // ASSIGN SUIT
    }
}

//DISPLAY PLAYER AND ENEMY CARDS
void displayCards(const string cards[], const string whichSt[], const int faceVal[], const int ruleSet, const string EwhichSt[], const int EfaceVal[]){
    int playerTotal = 0;
    int enemyTotal = 0;
    cout << "Your cards are " << fixed << setw(82) << "The enemy's cards are\n--------------             ---------------------" << '\n';
    
    for(int i = 0; i < ruleSet; i++){
        int num = faceVal[i];
        int eNum = EfaceVal[i];
        cout << cards[num]  << " " << whichSt[i] << setw(21) <<
                cards[eNum] << " " << EwhichSt[i] << '\n';
    }
            
}

//COMPARE CARDS TO SEE IF THEYRE EQUAL
bool compare(const string whichSt[], const int faceVal[], const string EwhichSt[], const int EfaceVal[], int ruleSet){
    bool matched = false;
    
    //CHECK PLAYER AND ENEMY CARDS TO MAKE SURE THEY'RE NOT IDENTICAL
    for(int i = 0; i < ruleSet; i++){
        if( (whichSt[i] == whichSt[i + 1]) && (faceVal[i] == faceVal[i + 1]) ) // IF PLAYER CARDS ARE IDENTICAL TO EACH OTHER
            return true;
        else
            matched = false;
        
        if( (EwhichSt[i] == EwhichSt[i + 1]) && (EfaceVal[i] == EfaceVal[i + 1]) ) // IF ENEMY CARDS ARE IDENTICAL TO EACH OTHER
            return true;
        else
            matched = false;
        
        if( (whichSt[i] == EwhichSt[i]) && (faceVal[i] == EfaceVal[i]) ) // IF PLAYER AND ENEMY CARDS ARE IDENTICAL TO EACH OTHER
            return true;
        else
            matched = false;
    }
    return matched;
}

//CHECK IF ANYBODY HAS AN ACE USING BINARY SEARCH
bool isAce(const int faceVal[], string &who, int &index, const int ruleSet){
    int playerIdentifier = -1;
    int enemyIdentifier  = -2;
    int val = 0;
    bool hasAce = false;
    
    //BINARY SEARCH FOR ACE
    int first = 0,
        middle,
        last = ruleSet - 1,
        position = -1;
    
    while(!hasAce && first <= last){
        middle = (first + last) / 2;
        
        if(faceVal[middle] == val){
            index = middle;
            faceVal[50] == playerIdentifier ? who = "Player" : who = "Enemy";
            hasAce = true;
        }else if(faceVal[middle] > val)
            last = middle - 1;
        else
            first = middle + 1;
    }
    
    return hasAce;
}

//IF SOMEBODY HAS ACE, ENTER THIS PROMPT
void promptAce(int faceVal[], string who, int ruleSet){
    string pick;
    bool isValid = false;
    
    if(who == "Player"){
        cout << "\nYou got an Ace, which means you get to pick if the value of this card" <<
                " will be 1, or 11. Please input 1 or 11 to choose.\n";
        cin >> pick;
        
        //ENSURES THE USER INPUTS A VALID DECISION
        while(isValid == false){
            
            if(pick == "1"){
                for(int i = 0; i < ruleSet; i++){
                    if(faceVal[i] == 0)
                        faceVal[i] = 1;
                }
                isValid = true;
            }else if(pick == "11"){
                for(int i = 0; i < ruleSet; i++){
                    if(faceVal[i] == 0)
                        faceVal[i] = 11;
                }
                isValid = true;
            }
            //IF THE USER INPUTTED AN INVALID RESPONSE
            else if(pick != "11" && pick != "1"){
                cout << "Invalid input. Please pick 1 or 11.\n";
                cin >> pick;
            }
        }
    }
    
    if(who == "Enemy"){
        int eChoice;
        int val;
        cout << "\nThe enemy got an Ace, which means they will get to pick if the value of this card" <<
                " will be 1, or 11.\n";
        eChoice = rand()%2+1; //PSEUDO AI
        
        eChoice == 1 ? val = 1 : val = 11;
        
        for(int i = 0; i < ruleSet; i++){ // SET ENEMY ACE VALUE
            if(faceVal[i] == 0)
                faceVal[i] = val;
        }
        
        cout << "The enemy has chosen this Ace to have a value of " << val << '\n';
    }
}

//DISPLAY CARD VALUE TOTALS
void displayTotals(int faceVal[], int EfaceVal[], int &playerTotal, int &enemyTotal, int ruleSet){
    playerTotal = 0;
    enemyTotal  = 0;
    int maxVal = 10;
    
    cout << "\n     TOTALS\n-----------------\n";
    
    //CALCULATE TOTALS
    for(int i = 0; i < ruleSet; i++){
        //IF VALUE IS ABOVE 10, SET IT TO 10
        if(faceVal[i] > maxVal && faceVal[i] != 11)
            faceVal[i] = maxVal;
        
        if(EfaceVal[i] > maxVal && EfaceVal[i] != 11)
            EfaceVal[i] = maxVal;
        
        //ACCOUNT FOR ARRAY VS. VALUE DIFFERENCE
        if(faceVal[i] < maxVal && faceVal[i] != 1)
            faceVal[i] = faceVal[i] + 1;
        
        if(EfaceVal[i] < maxVal && EfaceVal[i] != 1)
            EfaceVal[i] = EfaceVal[i] + 1;
        
        playerTotal += faceVal[i];
        enemyTotal  += EfaceVal[i];
    }
    
    if(playerTotal > 21)
        cout << "Your total was " << playerTotal << " . Which was greater than 21.\n" <<
                "The enemy's total was " << enemyTotal << '\n';
    else if(playerTotal == 21)
        cout << "Your total was equal to 21. A perfect score!\n"  <<
                "The enemy's total was " << enemyTotal << '\n';
    else{
        cout << "Your total was " << playerTotal << ".\n" <<
                "The enemy's total was " << enemyTotal << "." << '\n';;
    }
            
}

//DETERMINE ROUND WINNER
void displayRoundWinner(const int playerTotal, const int enemyTotal, int Collected[][2], int &deckRem){
    int player = 0;
    int enemy  = 1;
    int bounds = 21;
    static int pile = 4;
    cout << fixed << setprecision(0);
    
    //CALCULATE WINNER
    if(playerTotal > enemyTotal && playerTotal <= bounds){ //PLAYER WON
        cout << "You won this round. " << pile << " cards have been added to your deck.\n\n";
        Collected[player][player] += pile;
        deckRem -= pile;
        
    }else if(enemyTotal > playerTotal && enemyTotal <= bounds){ //ENEMY WON
        cout << "You have lost this round. " << pile << " cards have been added to the enemy's deck.\n\n";
        Collected[enemy][enemy] += pile;
        deckRem -= pile;
        
    }else if(enemyTotal == playerTotal){ //TIE
        cout << "You have tied with the enemy. The stakes will now be doubled.\n\n";
        pile = pile + pile;
        deckRem -= pile;
    }
    
    else if(enemyTotal > bounds && playerTotal > bounds){ //BOTH WENT ABOVE MAX VALUE
        cout << "Your totals both went above the 21 and therefore you have tied with the enemy. The stakes will now be doubled.\n\n";
        pile = pile + pile;
        deckRem -= pile;
        
    }else if(enemyTotal > playerTotal && enemyTotal > bounds && playerTotal < bounds){ //IF ENEMY WENT OVER VALUE AND YOU DIDNT
        cout << "You won this round. " << pile << " cards have been added to your deck.\n\n";
        Collected[player][player] += pile;
        deckRem -= pile;
        
    }else if(playerTotal > enemyTotal && playerTotal > bounds && enemyTotal < bounds){ //IF YOU WENT OVER VALUE AND ENEMY DIDNT
        cout << "You have lost this round. " << pile << " cards have been added to the enemy's deck.\n\n";
        Collected[enemy][enemy] += pile;
        deckRem -= pile;
    }
    
    if(deckRem < 0)
        deckRem = 0;
    //PRINT HOW MUCH CARDS ARE IN BOTH PILES AND HOW MUCH ARE REMAINING IN THE MAIN DECK
    cout << "Your Deck: " << Collected[player][player] << "     "
            "Enemy Deck: " << Collected[enemy][enemy] << "     "
            "Remaining Cards: " << deckRem << endl;
}

//ASK PLAYER TO GO AGAIN OR QUIT PLAYING
void promptAgain(char again, const int deckRem, bool &gameOver){
    bool isValid = false;
    
    if(deckRem > 0){ //PROMPT THE USER TO GO AGAIN, OR QUIT PLAYING
        cout << "Enter R when you are ready for the next round.\nEnter N to stop playing.\n";
        cin >> again;

        while(isValid == false){ //ENSURE USER PICKS A VALID RESPONSE
            
        if(again == 'R'){ //USER WANTS TO CONTINUE TO NEXT ROUND
            cout << "\n\n\n\n               NEXT ROUND\n";
            isValid = true;
            
        }
        
        else if(again == 'N'){ //USER WANTS TO STOP PLAYING
            isValid = true;
            cout << "Exiting game.";
            exit(0);
            
        }else{ //USER INPUTTED INVALID INPUT
            cout << "Invalid input. Please enter R to keep playing, or N to stop playing.\n";
            cin >> again;
            }
        }
    }else
        gameOver = true;
}

//POST GAME, DISPLAY WINNERS
void postGame(const int Collected[][2], ofstream &Record){
    static int winCount = 0,
               enemyWinCount = 0,
               tieCount = 0;
    int player = 0;
    int enemy  = 1;
    
    //POST GAME
    if(Collected[player][player] > Collected[enemy][enemy]){      //PLAYER WON
        cout << "               YOU WIN!\n           CONGRATULATIONS!";
        winCount += 1;
    }
    else if(Collected[enemy][enemy] > Collected[player][player]){ // ENEMY WON
        cout << "               YOU LOST!\n          BETTER LUCK NEXT TIME!";
        enemyWinCount += 1;
    }
    else if(Collected[player][player] == Collected[enemy][enemy]){ // TIED
        cout << "               YOU TIED!\n          WHAT ARE THE CHANCES!";
        tieCount += 1;
    }
    
    Record << "PLAYER WIN COUNT: " << winCount << endl << 
              "ENEMY WIN COUNT: " << enemyWinCount << endl << 
              "TIE COUNT: " << tieCount;
}

//MENU
void Menu(){
    char choiceM;
    //WELCOME MESSAGE & MENU (WILL BE REPLACED WITH A FUNCTION IN PROJECT 2)
    cout << "Welcome to 21: The Card Game\n----------------------------\n" <<
            "Enter '1' to PLAY\n" <<
            "Enter '2' for HELP\n" <<
            "Enter '3' to EXIT\n";
    cin >> choiceM;

    bool isValid = false;
    
    do{
    switch(choiceM){
        case '0': //WELCOME MESSAGE & MENU (WILL BE REPLACED WITH A FUNCTION IN PROJECT 2)
            isValid = true;
            cout <<
            "Welcome to 21: The Card Game\n----------------------------\n" <<
            "Enter '1' to PLAY\n" <<
            "Enter '2' for HELP\n" <<
            "Enter '3' to EXIT\n";
            cin >> choiceM; 
            if(choiceM != '1' || choiceM != '2' || choiceM != '3' || choiceM != '0') 
                isValid = false; break;
        case '1': /* GAME */ isValid = true; break; // EVENTUAL FUNCTION IN P2
        case '2': /* INSTRUCTIONS */ isValid = true;
                cout << "------------\nINSTRUCTIONS\n------------\n\n\n" <<
                "21 is a card game where you are given two cards every round. Each card has a value." <<
                "Number cards retain the value of their number. EX: The 3 of Spades has a value of 3." <<
                "Face cards retain a value of 10. EX: The Queen of Hearts has a value of 10." << 
                "An Ace leaves you with the decision to choose its value of 1 or 11." <<
                "You win a round by having a greater total value than your opponent, but having a value less than 22." <<
                "If your total value goes past 21, you will automatically lose the round." <<
                "The goal of this game is to win as many rounds as possible. Winning a round grants you with the cards in the pile, and at play that round." <<
                "If you tie, the cards you just played with will be added to the pile and you will continue rounds until somebody wins." <<
                "The game is over when there are no more remaining cards in the deck." << 
                "The player with the highest amount of cards collected at the end of the game wins.\n" <<
                "TO RETURN TO MENU ENTER 0, TO PLAY ENTER 1, TO EXIT ENTER 3.\n";
                cin >> choiceM; 
                if(choiceM != '1' || choiceM != '2' || choiceM != '3' || choiceM != '0') 
                    isValid = false; break;
        case '3': cout << "Exiting game."; exit(0); break;
        default: cout << "Invalid input please enter 0 to return to the Main Menu, " <<
                              "enter 1 to play the game, enter 2 for help, or enter 3 to exit.\n";
                isValid = false;
                cin >> choiceM;
        }
    }while(isValid == false);
}

//GAME TITLE
void GameTitle(){
                             //TITLE
    cout << "               GAME STARTED                \n" <<
            "The cards will now be shuffled & distributed.\n\n";
}

void fillDeck(vector<int> cards, const int deckSz){
    int cardAmt = 13;
    int cardMax = 4;
    int cardSt = rand()%4+1;
    string suit;
    
    for(int i = 0; i < cardAmt; i++){
        for(int j = 0; j < cardMax; j++){
            int randVal = rand()%13+1;
            cards.push_back(randVal);
        }
    }
    
    switch(cardSt){
        case 4: suit = "of Spades"; break;
        case 3: suit = "of Hearts"; break;
        case 2: suit = "of Diamonds"; break;
        case 1: suit = "of Clubs"; break;
    }
}

//ORDER THE DECK USING VECTORS, AND SELECTION SORT
void orderDeck(vector<int> cards, const int deckSz, int sel){
    int minIndex, minValue;
    
    //SELECTION SORT
    for(int i = 0; i < (deckSz - 1); i++){
        minIndex = i;
        minValue = cards.at(i);
        
        for(int index = i + 1; index < deckSz; index++){
            if(cards.at(index) < minValue){
                minValue = cards.at(index);
                minIndex = index;
            }
        }
        swap(cards.at(minIndex),cards.at(i));
    }
}

//ORDER THE DECK USING VECTORS, AND BUBBLE SORT
void orderDeck(vector<int> cards, const int deckSz, string sel){
    int minIndex, minValue;
    
    //BUBBLE SORT
    for(int max = deckSz - 1; max > 0; max --){
        for(int index = 0; index < max; index++){
            if(cards.at(index) > cards.at(index + 1))
                swap(cards.at(index), cards.at(index + 1));
        }
    }
}
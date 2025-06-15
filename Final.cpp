#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>          // For time-related operations
#include <limits>          // For numeric limits (used in input validation)
#include <cstdlib>         // For system() function calls
#include <stdexcept>       // For exception handling
#include <cmath>           // For mathematical functions
#include <algorithm>       // For algorithms like max, min
using namespace std;
using namespace std::chrono; // Use chrono namespace for time operations

const int MAX_NAME_LENGTH = 50;
const int MAX_USERS = 10;

//Class to store username and ID
class User
{
public:

    //variables are public to be accessed by UserManager
    int ID_no; //UserID
    string name; //User Name

    //Default Constructor for guest user
    User() :ID_no(0)
    {
        name="Guest";
    }

    //Parameterised Constructor to accept input ID and name
    User(int ID_number, string a_name)
    {
        ID_no=ID_number;
        name=a_name;
    }

};

// class to manage functions to be used by UserApp
class UserManager
{
public:
    //public variables for easy access in UserApp
    User users[MAX_USERS];
    int no_of_user;

    //Default Constructor to initialise no of users to 0;
    UserManager(): no_of_user(0) {}

    //function to add new users
    void addUser(int ID_no, string name)
    {
        if(no_of_user<MAX_USERS)
        {
           users[no_of_user]=User(ID_no,name); //User Input is passed into users array
           no_of_user++;
           cout<<"New User Added!"<<endl<<endl;;
        }
        else
        {
            cout<<"Maximum Capacity Reached. Cannot Add New Users."<<endl;
        }
    }

    //Function to show all Users
    void displayUsers()
    {
        if(no_of_user==0)
        {
            cout<<"No Users Added/To Display.Try Loading Users file"<<endl<<endl;
            return;
        }

        for(int i=0; i<no_of_user;i++)
        {
            cout<<"ID No:"<<users[i].ID_no<<endl;
            cout<<"Name:"<<users[i].name<<endl;
            cout<<endl;

        }
    }

    //Function to get a specific User using their ID
    //pointer is used to enable use of nullptr for execption(easier method)
    User *getUserByIDno(int ID_num)
    {
        for(int i=0;i<no_of_user;i++)
        {
            if(users[i].ID_no==ID_num)
            {
                return &users[i];
            }
        }
        return nullptr;
    }

    // function to check for overlapping users
    bool userExistence (int ID_no)
    {
        return getUserByIDno(ID_no)!=nullptr;// calls getUserById to compare with nullptr
    }
};

//Class to run User Manager and manage operations in main body
class UserApp
{
private:
    UserManager usermanagers;
    string filename;

public:
    //default constructor to create file with name userdata
    UserApp(const string &file ="userdata.txt")
    {
        filename=file;
    }

    //get function to acess usermanagers object to allow access for Flashcardapp
    UserManager &getUserManager()
    {
        return usermanagers;
    }

    //Function to call to add User
    void addNewUser()
    {
        int ID;

    string name;

    cout<<"Enter ID No:";
    cin>>ID;
    cin.ignore(); // Used to ignore newline character and makes sure the next line for getline works

    //This conditional is done first to make sure no overlapping users with same id appears(names may repeat)
    if(usermanagers.userExistence(ID))
    {
        cout<<endl<<"User with this ID already exists"<<endl<<endl;
        return;

    }

    cout<<"Enter Username:";
    getline(cin,name);

    usermanagers.addUser(ID,name); //calls function from obj usermanager to add a new user
    }

    void saveData()
    {
        ofstream wf(filename);
        if(!wf)
        {
            cout<<"Unable to open file for writing!"<<endl;
            return;
        }

        wf<<usermanagers.no_of_user<<endl;

        for(int i=0;i<usermanagers.no_of_user;i++)
        {
            wf<<usermanagers.users[i].ID_no<<endl; //ID no from obj usermanagers that has user obj is written into txt file
            wf<<usermanagers.users[i].name<<endl; //name of user from obj usermanagers that has user obj is written into txt file
        }

        wf.close();
        cout<<"User Data Saving Operation Done!"<<endl<<endl;
    }

    void loadData()
    {
        ifstream rf(filename);
        if(!rf)
        {
            cout<<"Unable to read from file!"<<endl;
            return;
        }

        rf>>usermanagers.no_of_user;
        rf.ignore(); //similar to cin.(ignore) it is used to disregard the \newline(enter key) line

        for(int i=0;i<usermanagers.no_of_user;i++)
        {
            rf>>usermanagers.users[i].ID_no; //ID no from obj usermanagers that has user obj is read from txt file
            rf.ignore();
            rf>>usermanagers.users[i].name; //name of user from obj usermanagers that has user obj is read from txt file
        }

        rf.close();
        cout<<"Reading User Data Process Complete"<<endl<<endl;
    }

    //Function to show all user data in current session/loaded data
    void displayUserData()
    {
        cout<<"User Data...."<<endl;
        usermanagers.displayUsers();
    }

    //Function  for selection of User Profile for more personalised review experience
    int selectUser()
    {
        if(usermanagers.no_of_user==0)
        {
            cout<<"No Users! Add Users first!"<<endl;

        }

        cout<<endl<<"Select User Profile"<<endl;
        cout<<"Added Users:"<<endl;

        for(int i=0;i<usermanagers.no_of_user;i++)
        {
            cout<<usermanagers.users[i].ID_no<<"."<<usermanagers.users[i].name<<endl;
        }

        int selectedno;
        cout<<"Enter ID No to select your profile:";
        cin>>selectedno;

        //crosscheck input and user ID
        if(usermanagers.userExistence(selectedno))
        {
            User *selecteduser=usermanagers.getUserByIDno(selectedno); //getUserbyIDno is called and user data stored in a pointer
            cout<<"Selected User Profile:"<<selecteduser->name<<endl;
            return selectedno;

        }else
        {
            cout<<"User ID doesn't exist"<<endl;
            return -1;
        }

    }

    //Menu Handling
    void Menu()
    {
        int choice=0;

        while(choice!=5)
        {
            cout<<"User App Management UI"<<endl;
            cout<<"1.Display All User Profiles"<<endl;
            cout<<"2.Add New User"<<endl;
            cout<<"3.Save User Profile"<<endl;
            cout<<"4.Load User Profile"<<endl;
            cout<<"5.Enter Flashcard Menu"<<endl;
            cout<<"Selected Choice:"; cin>>choice;


            system("CLS"); //will not work in Linux

            switch(choice)
            {
                case 1:
                    displayUserData();
                    break;
                case 2:
                    addNewUser();
                    break;
                case 3:
                    saveData();
                    break;
                case 4:
                    loadData();
                    break;
                case 5:
                    break;
                default:
                    //if input is not 1-5 or is character
                    if(cin.fail())
                    {
                        cin.clear();
                        // used to ignore the largest possible value of characters until newline is hit
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        cout<<"Invalid choice! Please enter valid choice!"<<endl;
                        continue;

                    }
            }
        }

    }




};

// class to initialise and store the flashcard contents and functions
class Flashcards
{
private:
    int card_no;
    string ques;
    string ans;
    int difficulty;
    int achievement;
    int userOwner; // no for users who own the right to a flashcard
    //system_clock::time_point provides a real time wall clock that takes timepoint as a timestamp
    //and NextReviewSession works as a variable name
    system_clock::time_point NextReviewSession;

public:
    //default constructor
    Flashcards():card_no(0),ques(" "),ans(" "),difficulty(1),
    NextReviewSession(system_clock::now()),achievement(0),userOwner(0)
    {}

    //Parameterised Constructor
    Flashcards(int cardno,string a_ques,string a_ans,int a_difficulty,int a_achievement,
               int a_userOwner,system_clock::time_point a_NextReviewSession):
                card_no(cardno),ques(a_ques),ans(a_ans),difficulty(a_difficulty),
                NextReviewSession(a_NextReviewSession),achievement(a_achievement),userOwner(a_userOwner)
                {}

    //Get Functions to be used by FlashcardApp to access private members
    int getCardNo() const{return card_no;}
    string getQues() const {return ques;}
    string getAns() const{return ans;}
    int getDifficulty() const{return difficulty;}
    int getUserOwner() const{return userOwner;};
    system_clock::time_point getNextReviewSession() {return NextReviewSession;}
    int getAchievement() const{return achievement;}

     //function to update difficulty of question between 1-5(lower being easier and vice versa)
    void updateDifficulty(bool correct)
    {
        if(correct)
        {
            difficulty=max(1,difficulty-1); //returns the bigger value , 1 is the min value returned
        }
        else
        {
            difficulty=min(5,difficulty+1);//return the smaller value, 5 is the max value
        }
    }

    //will determine how long the interval for the flashcard based on difficulty
    void updateNextReviewSession()
    {
        int intervalseconds;
        switch(difficulty)
        {
            case 1:
            intervalseconds=60;
            break;

            case 2:
            intervalseconds=30;
            break;

            case 3:
            intervalseconds=20;
            break;

            case 4:
            intervalseconds= 15;
            break;

            case 5:
            intervalseconds=5;
            break;

            default:
            intervalseconds=30;
            break;

        }

        //review session time is updated by using real time clock with time interval
        NextReviewSession=system_clock::now()+seconds(intervalseconds);
    }

    //Function to make sure its time for session review
    bool DueCardTime()
    {
        //compares current time to calculated next session time
        //if comparison is same, the time is returned
        return system_clock::now()>=NextReviewSession;
    }

    //Function to update user achievement score
    void updateAchievement()
    {
        achievement++;
    }

    //function to change system time to one single data variable,time_t to be stored
    time_t getNextReviewTime()
    const{
            return system_clock::to_time_t(NextReviewSession);
            //to_time_t is a lib function that converts system time to time of lower precision

        }

    //Function to read time_t and convert into system time
    void setNextReviewFromTimeT(time_t T)
    {
        NextReviewSession=system_clock::from_time_t(T);
    }

};

//class to put together the flashcards into a deck and allow access to the cards for FlashcardApp
class Deck
{
private:
    vector<Flashcards>cards; //Vector to enable dynamic storing

public:

    //Function to add cards to deck
    void addCard(const Flashcards &card)
    {
        cards.push_back(card);
    }

    //Function to remove a card by index
    void removeCard (int index)
    {
        if(index>=0 && index<cards.size())
        {
            cards.erase(cards.begin()+index);
        }
        else
        {
            cerr<<"Invalid Index.\n";
        }

    }

        //Function to get Cards Due for review for specific user
    vector<Flashcards> getDueReviewCards(int userOwner)
    {
        vector<Flashcards>DueReviewCards;
        for(int i=0;i<cards.size();i++)
        {   //first part compares if current user has the cards
            //second part determines if a card is due for review using NextReviewSession as time
            if(cards[i].getUserOwner()==userOwner&&cards[i].DueCardTime())
            {
                DueReviewCards.push_back(cards[i]); //adda card to DueReviewDeck
            }

        }
        return DueReviewCards;
    }

    //Function to get All Cards for specific user
    vector<Flashcards>getAllUserCards(int userOwner)
    {
        vector<Flashcards>AllUserCards;
        for(int i=0;i<cards.size();i++)
        {
            if(cards[i].getUserOwner()==userOwner)
            {
                AllUserCards.push_back(cards[i]); //adds all cards to AllUserCardsDeck
            }
        }
        return AllUserCards;
    }

    Flashcards &getCard(int cardno)
    {
        for (int i = 0; i < cards.size(); i++) {
            if (cards[i].getCardNo() == cardno)
                return cards[i];  // Return reference to found card
        }
        static Flashcards dummy;  // Static dummy card if not found
        return dummy;
    }

    //function to return size of vector
    int size() const
    {
        return static_cast<int>(cards.size());
    }

};

//class to manage Deck in the main program and interface with UserApp
class FlashcardApp
{
private:
    Deck deck;
    int cardno; //used for making cards when reading from file
    string filename;
    int currentUserNo;
    UserManager * usermanager1;

//Following Functions are placed under private to prevent accidental data overwrite

    //Function to Return to UI/clear the inputs
    void ReturnToMain()
    {
        cout<<"Press Enter To Continue";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignores all except enter key
    }

    //Function to Show Score
    void showScore()
    {
        system("cls");
        vector<Flashcards>usercards=deck.getAllUserCards(currentUserNo);

        if(usercards.size()==0)
        {
            cout<<"No flashcards created yet!"<<endl<<endl;
            ReturnToMain();
            return;
        }
        User *currentuser =usermanager1->getUserByIDno(currentUserNo);
        cout<<"FlashCard Scores for Spaced Repetition:"<<endl<<endl;

        for(int i=0; i<usercards.size();i++)
        {
            cout<<" Card "<<(i+1)<<endl;
            cout<<"Question:"<<usercards[i].getQues()<<endl;
            cout<<"Answer:"<<usercards[i].getAns()<<endl;
            cout<<"Difficulty Level:"<<usercards[i].getDifficulty()<<endl;
            cout<<"Achievement Score :"<<usercards[i].getAchievement()<<endl<<endl;

        }
        ReturnToMain();

    }

    //Function to Start Spaced Repetition Session
    void startSpacedRepetition()
    {
        system("cls");
        //Retrieves Due Cards
        vector<Flashcards>DueReviewCards=deck.getDueReviewCards(currentUserNo);

        if(DueReviewCards.size()==0)
        {
            cout<<"No Cards For Spaced Repetition Yet!"<<endl;
            ReturnToMain();
            return;

        }

        User *currentuser =usermanager1->getUserByIDno(currentUserNo);
        cout<<"Spaced Repetition Session for:"<<currentuser->name<<endl;
        cout << "Found " << DueReviewCards.size() << " cards due for spaced repetition review!\n\n";

        for(int i=0; i<DueReviewCards.size();i++)
        {
            system("cls");
            cout<<"Question:"<<DueReviewCards[i].getQues()<<endl;
            cout<<endl<<"Press Enter to show back of Card"<<endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Answer:"<<DueReviewCards[i].getAns()<<endl<<endl;

        int difficultyresponse;
        do
        {
            cout<<"Press 1 if you answered correctly"<<endl;
            cout<<"Press 0 if you answered wrongly"<<endl;
            cout<<"Seleccted Choice:";
            cin>>difficultyresponse;

                if(difficultyresponse==1)
                {
                   deck.getCard(DueReviewCards[i].getCardNo()).updateDifficulty(true);
                   deck.getCard(DueReviewCards[i].getCardNo()).updateAchievement();
                   deck.getCard(DueReviewCards[i].getCardNo()).updateNextReviewSession();

                   break;
                }
                else if(difficultyresponse==0)
                {
                    deck.getCard(DueReviewCards[i].getCardNo()).updateDifficulty(false);
                    deck.getCard(DueReviewCards[i].getCardNo()).updateNextReviewSession();

                    break;
                }
                else
                {
                    cout<<"Please Input 1 or 0"<<endl;
                    continue;
                }


        }while(true);

        cout << "\nPress enter to continue";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout<<endl<<"Spaced Repetition Session Complete"<<endl;

    }


    //Function for Reviewing All Flashcards
    void Review()
    {
        system("cls");
        //assigns all usercards vector to allcards as local variable
        vector<Flashcards> AllCards=deck.getAllUserCards(currentUserNo);

        if(AllCards.size()==0)
        {
            cout<<"No Flashcards added for this user!"<<endl;
            ReturnToMain();
            return;
        }

        User *currentuser =usermanager1->getUserByIDno(currentUserNo);
        cout<<"Normal Review Session Selection"<<endl;
        cout<<"1.Review All Cards"<<endl;
        cout<<"2.Review By Difficulty"<<endl;

        int SelectedSession;
        cin>>SelectedSession;
        cin.ignore();
        vector<Flashcards>Selectedcards;

        if(SelectedSession==1)
        {
            Selectedcards=AllCards;

        }
        else if(SelectedSession==2)
        {
            cout<<"Select Difficulty Level (1-5)";
            int chosenDifficulty;
            cin>>chosenDifficulty;
            cin.ignore();

            for(int i=0;i<AllCards.size();i++)
            {
                if(AllCards[i].getDifficulty()==chosenDifficulty)
                {   //retrieves cards of chosen difficulty
                    Selectedcards.push_back(AllCards[i]);
                }
            }
            if(Selectedcards.empty())
            {
                cout<<"No Cards found with chosen difficulty"<<" "<<chosenDifficulty<<endl;
                ReturnToMain();
                return;
            }

        }
        else
            {
                cout<<"Invalid Choice"<<endl;
                ReturnToMain();
                return;
            }

        //Review start

        int correctcounter=0;
        for(int i=0;i<Selectedcards.size();i++)
        {
            system("cls");
            cout<<"Normal Review Session"<<endl;
            cout<<"Question:"<<Selectedcards[i].getQues()<<endl;
            cout<<endl<<"Press Enter to show back of Card"<<endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Answer:"<<Selectedcards[i].getAns()<<endl<<endl;

            int correctresponse;
            do
            {
                cout<<"Did you answer correctly?"<<endl;
                cout<<"1-Yes"<<endl;
                cout<<"0-No"<<endl;
                cout<<"Selected Choice:";
                cin>>correctresponse;

                if(correctresponse==1)
                {   //tracks user progress
                    correctcounter++;
                    cout<<"Correct!Progress Tracked"<<endl;
                    break;
                }
                else if(correctresponse==0)
                {
                    cout<<"Try Again Later"<<endl;
                    break;
                }
                else
                {
                    cout<<"Invalid Input.Please enter 1 or 0"<<endl;
                }
            }while(true);
        }
        cout<<"Press Enter To Continue";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignores all except enter key

        //Results for Review Session
        system("cls");
        cout<<"No of cards reviewed:"<<Selectedcards.size()<<endl;
        cout<<"No of Correct Answers:"<<correctcounter<<endl;
        cout<<"Accuracy:"<<(correctcounter*100/Selectedcards.size())<<"%"<<endl;
        ReturnToMain();

}

public:
    //Parameterized constructor
    FlashcardApp(UserManager *a_usermanager,const string &file="flashcard_data.txt")
    : usermanager1(a_usermanager),currentUserNo(0),filename(file){}

    //function to set current user
    void setCurrentUser(int ID_no)
    {
        currentUserNo=ID_no;
    }

    //function to save flashcard data into txt file
    void saveData()
    {
        ofstream wf(filename);
        if(!wf)
        {
            cout<<"Unable to open file for writing!"<<endl;
            return;

        }

        //writes flashcard data into file as long as there are flashcards
        for (int i=0; i<deck.size();i++)
        { Flashcards cards=deck.getCard(i);
            wf<<cards.getCardNo()<<endl;
            wf<<cards.getQues()<<endl;
            wf<<cards.getAns()<<endl;
            wf<<cards.getDifficulty()<<endl;
            wf<<cards.getNextReviewTime()<<endl;
            wf<<cards.getUserOwner()<<endl;
        }
        wf.close();
        cout<<endl<<"Flashcard data saved!"<<endl;
    }

    //function to read flashcard data from txt file and create cards
    void loadData()
    {
        ifstream rf(filename);
        if(!rf)
        {
            cout<<"No flashcard data found!"<<endl;
            cardno=0;
            return;
        }

        deck=Deck();//reset deck by calling default constructor(as no default constructor is written
                     //it will initialised as empty vector
        cardno=0;

        //local variables to store read data
        int cardnum,difficulty,userowner1,achievement1=0;
        string question,answer;
        time_t nextReviewSesh;

        while(rf>>cardnum)
        {
            rf.ignore();
            getline(rf,question);
            getline(rf,answer);
            rf>>difficulty>>nextReviewSesh>>userowner1;
            rf.ignore();

        // create flashcard after reading data
      Flashcards card(cardnum,question,answer,difficulty,achievement1,userowner1,system_clock::now());
      card.setNextReviewFromTimeT(nextReviewSesh);// set review time

      deck.addCard(card); //add cards to deck
      cardno=max(cardno,cardnum);
        }

        rf.close();
        cout<<"Loaded"<<deck.size()<<"flashcards successfully!"<<endl;
    }

    //function to create new card
    void createNewCard()
    {
        if (currentUserNo==-1)
        {
            cout<<"No User Selected"<<endl;
            ReturnToMain();
            return;
        }

        system("cls");
        string question,answer;
        //get current user through ID no
        User *currentuser =usermanager1->getUserByIDno(currentUserNo);

        //get answer and question
        cout<<"Create New Flashcard for"<<" "<<currentuser->name<<endl;
        cout<<"Enter question:";
        getline(cin,question);
        cout<<"Enter answer:";
        getline(cin,answer);


        //Validation to make sure ques and ans are not blank
        if (question.empty()||answer.empty())
        {
            cout<<endl<<"Question and answer must not be empty!"<<endl<<endl;
            ReturnToMain();
            return;
        }

        //actual process of creating and adding flashcard
        deck.addCard(Flashcards(cardno++,question,answer,1,0,currentUserNo,system_clock::now()));
        cout<<"Flashcard Added "<<endl<<endl;
        cout<<"Card ID:"<<cardno<<endl<<endl;
        cout<<"Assigned to User:"<<currentuser->name<<endl<<endl;;

        ReturnToMain();
    }

    // Main menu for flashcard app
    void run()
    {
        loadData();

        if(currentUserNo==-1)
        {
            cout<<"Please Select a User Profile!"<<endl;
            return;
        }

        int choice;
        User *currentuser=usermanager1->getUserByIDno(currentUserNo);

        do
        {
            system("cls");
            cout<<"Flashcard System for:"<<" "<<currentuser->name<<endl;
            cout<<"Cards Due for Spaced Repetition:"<<deck.getDueReviewCards(currentUserNo).size()<<endl;
            cout<< "1) Create new flashcard" << endl;
            cout<< "2) Spaced repetition review (due cards only)" << endl;
            cout<< "3) Normal flashcard review (all cards)" << endl;
            cout<< "4) Switch user profile" << endl;
            cout<< "5) View Statistics for Each Card in Spaced Repetition"<<endl;
            cout<< "6) Save and Exit" << endl;
            cout<< "Selected Choice:";
            cin >> choice;
            cin.ignore();

            switch(choice)
            {
                case 1:createNewCard();break;
                case 2:startSpacedRepetition();break;
                case 3:Review();break;
                case 4:
                    {
                        UserApp userselector;
                        userselector.getUserManager()=*usermanager1;
                        int newUser=userselector.selectUser();
                        if(newUser!=-1)
                        {
                            setCurrentUser(newUser);
                            currentuser=usermanager1->getUserByIDno(currentUserNo);

                        }
                        break;
                    }
                case 5: showScore();break;
                case 6:
                    saveData();
                    cout<<"Exiting Program!"<<endl;
                    break;
                default:
                    cout<<"Invalid Selection!"<<endl;
                    ReturnToMain();
            }

        } while (choice != 6);                 // Continue until user chooses to exit
    }
};


int main()
{
    //userapp obj created to invoke menu
    UserApp app1;
    app1.Menu();

    //Select User
    int selecteduser=app1.selectUser();
    if(selecteduser==-1)
    {
        cout<<"Invalid User"<<endl;
        return 1;
    }

    //Flashcard app is created and usermanager is passed through variable through obj app1
    FlashcardApp app2(&app1.getUserManager());
    app2.setCurrentUser(selecteduser);  //Set selected user
    app2.run(); //Main UI is run

    return 0;
}

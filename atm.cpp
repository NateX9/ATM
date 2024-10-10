#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

class Account {
    private:
        double accountbal = 0.0;
        //account balance starts at 0
        string accountname, accountpass;
        //initialize name and pass to be assigned later
        vector<string> accountHistory;
        //create a vector of strings that records account history
    public:

        double getBalance() {
            return accountbal; //getter method to safely pull private variables globally
        }

        string getName(){
            return accountname;
        }

        string getPass(){
            return accountpass;
        }

        void setBalance(double newBalance){
            accountbal = newBalance;
        }
        //setter methods to pull from files
        void setName(string newName){
            accountname = newName;
        }

        void setPass(string newPass){
            accountpass = newPass;
        }

        friend ostream &operator<<(ostream &os, const Account &account) {
            os << "Account Name: " << account.accountname << "\n";
            os << "Balance: $" << fixed << setprecision(2) << account.accountbal << "\n";
            return os;
        }
        //The point of this is to allow cout to print objects of the "Account" class. I do this by overloading the "<<" operator, telling it what to print out when it reads an account object.

        void depositMoney(double additionalmoney){
            ostringstream stream;
            //this same variable is also found in withdrawMoney(). It takes from the <sstream> header and works similar to cout (but it doesn't print to the screen)
            //Its purpose is to round the numbers pushed to the accountHistory vector to 2 decimal places.
            accountbal = accountbal + additionalmoney;
            cout << fixed << setprecision(2) << "You now have $" << accountbal << " dollars in your account!" << endl;
            stream.str(""); //This and the following line clears stream so multiple numbers dont print next to eachother.
            stream.clear();
            stream << fixed << setprecision(2) << additionalmoney;
            accountHistory.push_back("Deposited $" + stream.str());
            //setprecision(2) rounds to 2 decimals. like money.
        }

        void withdrawMoney(double subtractedMoney){
            ostringstream stream;
            if(subtractedMoney > accountbal){
                cout << "You can't withdraw that much, your account only holds $" << accountbal << " dollars." << endl;
            }else{
                accountbal = accountbal - subtractedMoney;
                cout << fixed << setprecision(2) << "You now have $" << accountbal << " dollars in your account!" << endl;
                stream.str("");
                stream.clear();
                stream << fixed << setprecision(2) << subtractedMoney;
                accountHistory.push_back("Withdrew $" + stream.str());
            }
        }

        void checkBalance(){
            cout << fixed << setprecision(2) << "You have $" << accountbal << " dollars in your account currently." << endl;
        }

        void checkHistory(){
            cout << "Your account history: " << endl;
            for(int i = 0; i<accountHistory.size(); i++){
                cout << accountHistory[i] << endl << endl;
                //looks at every item in the account history vector and prints each individually.
            }
        }

        bool successfulLogin = false; //correct user and pass?
        bool isSessionActive = true; //does the user want to be in this account?

        void accountLogin(){
            string pass, user;
            //local pass and user to be compared to true pass and user in private section of class
            int allowedLoginAttempts = 3;
            //3 attempts allowed
            while(!successfulLogin && isSessionActive && allowedLoginAttempts > 0){
                cout << "Enter Password below." << endl;
                cin >> pass;
                if(pass == accountpass){
                    successfulLogin = true; //breaks out of while loop
                    cout << " " << endl;
                    cout << "Successful Login." << endl;
                }else{
                    allowedLoginAttempts--;
                    cout << "Incorrect password. Try Again. You have " << allowedLoginAttempts << " attempts left. " << endl;
                    cout << " " << endl;
                }
            }
            if(allowedLoginAttempts <= 0){
                cout << "Too many incorrect login attempts. Going back to menu. ";
            }
        }

        friend struct ATM;
};

enum userChoice { BALANCE, WITHDRAW, DEPOSIT, EXIT, HISTORY, INVALID, DELETE, CREATE, VIEW, LOGIN };

userChoice getChoice(string input){
    if(input == "bal") return BALANCE;
    if(input == "wit") return WITHDRAW;
    if(input == "dep") return DEPOSIT;
    if(input == "hist") return HISTORY;
    if(input == "exit") return EXIT;
    return INVALID;
}

userChoice getDecision(string input){
    if(input == "view") return VIEW;
    if(input == "create") return CREATE;
    if(input == "delete") return DELETE;
    if(input == "login") return LOGIN;
    if(input == "leave") return EXIT;
    return INVALID;
}

double getAmount(){ //gets used in the performWithdrawal() and performDeposit() functions
    double amount;
    while(true){
        cin >> amount;
        if(cin.fail() || amount < 0){ //Checks if input is allowed (double has to be inputted) or if neg number. if neither, ignore if statement.
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid amount (positive number). " << endl;
        }else{
            return amount;
        }
    }
}

void performWithdrawal(Account &currentATM){ //has to pass by reference because otherwise it will create another object
    cout << "How much would you like to withdraw? " << endl;
    double amount = getAmount();
    if(amount > currentATM.getBalance()){
        cout << "You can only withdraw $" << currentATM.getBalance() << "." << endl;
    }else{
        currentATM.withdrawMoney(amount);
    }
}

void performDeposit(Account &currentATM){
    cout << "How much would you like to deposit? " << endl;
    currentATM.depositMoney(getAmount());
}

struct ATM {
    vector<Account> accountsOnFile;
    //keeps track of all accounts created
    void accountCreation(Account &accountName){
        string username, password;
        char choice; //whether they want that specific username and password (maybe they mistyped?)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "What is your name? ";
        getline(cin, username);

        cout << "\n Create a password. ";
        getline(cin, password);

        cout << "\n Are you sure about the username and pass? Y for yes, n for no (case sensitive): ";
        cin >> choice;

        bool accountFound = false;
        //Look through every account in the vector, return true if account with same name (if two accounts have the same name, the whole program will get confused)
        for(int i = 0; i<accountsOnFile.size(); i++){ //O(n)
            if(accountsOnFile[i].getName() == username){
                accountFound = true;
                break;
            }
        }

        if(accountFound){
            cout << "There is already an account with that name" << endl;
        }else if(cin.fail() || !(choice == 'Y' || choice == 'n')){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Assuming no." << endl;
        }else if(choice == 'Y' && !accountFound){ //&& !accountFound is technically uneccessary but I added it for redundancy.
            accountName.accountname = username;

            accountName.accountpass = password;

            accountsOnFile.push_back(accountName);
            cout << "Your account has been created!" << endl;

        }else{
            cout << "Okay. Returning you to main screen...";
        }
    }

    void showAccounts(){
        cout << "Accounts on ATM: " << endl;
        for(int i = 0; i<accountsOnFile.size(); i++){
            cout << accountsOnFile[i] << endl << endl;
        }
    }

    void accountDeletion(string accountName){
        bool success = false; //going to check whether the account was found (true) or if theres no account with that name (false)
        char choice;

        for(int i = 0; i<accountsOnFile.size(); i++){ //O(n)
            if (accountsOnFile[i].accountname == accountName){
                cout << "Do you want to delete the account named " << accountsOnFile[i].accountname << "? Y for yes, n for no (case sensitive) ";
                cin >> choice;
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Assuming no." << endl;
                }else if(choice == 'Y'){
                    accountsOnFile.erase(accountsOnFile.begin() + i); //deletes the account i(for loop iteration num) away from the beginning of the vector
                    success = true;
                    break;
                }else{
                    cout << "Okay. Returning you to main screen...";
                }
            }
        }
        if(success){
            cout << "Account has been deleted." << endl;
        }
        if(!success){
            cout << "There is no account with that name or an error has occurred. Try again." << endl;
        }
    }

    void userSession(Account &accountName){
        while(accountName.successfulLogin && accountName.isSessionActive){
            string choice;
            cout << " " << endl;

            cout << "What would you like to do in your account? " << endl;

            cout << " " << endl;

            cout << "Note: You can view your balance by typing 'bal', withdraw money using 'wit' and deposit money using 'dep'." << endl;
            cout << "You can also check account history with 'hist' or leave the instance with 'exit'." << endl;

            cout << " " << endl;

            cin >> choice;

            userChoice action = getChoice(choice); //assigns a variable to global enum created before

            switch(action){ //Switches between the enum created earlier and assigned to variable "action"
                case BALANCE: {
                    accountName.checkBalance();
                    break;
                }
                case WITHDRAW: {
                    performWithdrawal(accountName);
                    break;
                }
                case DEPOSIT: {
                    performDeposit(accountName);
                    break;
                }
                case HISTORY: {
                    accountName.checkHistory();
                    break;
                }
                case EXIT: {
                    cout << "Goodbye!" << endl;
                    accountName.successfulLogin = false;
                    accountName.isSessionActive = false;
                    break;
                }
                default: {
                    cout << "Invalid input. Try again. ";
                }
            }
        }
    }

    void saveAccountsToFile(){
        ofstream toFile("history.txt"); //opens file "history.txt" or creates it | soon to be written to
        for(auto &account : accountsOnFile){ //loops through every account on accountsOnFile, passing them by reference
            toFile << account.getName() << ";" << account.getBalance() << ";" << account.getPass() << "\n"; //writes to file
            for(auto &transaction : account.accountHistory){ //loops through all transactions on accountHistory, passing by reference
                toFile << transaction << "\n";
            }
            toFile << "---" << "\n"; //used to split each Account object in the txt file
        }
        toFile.close();
    }

    void loadFromFile(){
        ifstream fromFile("history.txt"); //opens file "history.txt" to be taken from
        string line;
        while(getline(fromFile, line)){ //goes down every line in the file, mapping each line to the string variable line
            if(line == "---"){
                continue; //go to the next line in the txt file
            }
            Account newAccount;
            istringstream fin(line); //converts line to stream so it can be parsed easier
            string name, pass;
            double balance;
            getline(fin, name, ';');
            fin >> balance; //extracts account name bal and pass from line
            fin.ignore(1);
            getline(fin, pass, '\n');
            newAccount.setName(name);
            //sets account name balance and pass to the extracted ones
            newAccount.setBalance(balance);
            newAccount.setPass(pass);
            while(getline(fromFile, line) && line != "---"){
                newAccount.accountHistory.push_back(line); //for every line that isn't "---", write to the history vector of the Account object in question
            }
            accountsOnFile.push_back(newAccount); //pushes back the object to the Account vector in ATM struct
        }
        fromFile.close();
    }
};

int main(){
    ATM myATM;
    //loads history.txt
    myATM.loadFromFile();
    bool globalSessionActive = true;
    string decision;

    while(globalSessionActive){
        cout << "Welcome to the ATM! What would you like to do?" << endl;
        cout << "You can log into an existing account by typing login." << endl;
        cout << "You can look at all the accounts stored on this machine by typing 'view'." << endl;
        cout << "You can also create your own account by typing 'create' or delete an account by typing 'delete'." << endl;
        cout << "Finally, you can leave this session by typing 'leave'.";
        cout << endl << "Keep in mind that these are case sensitive." << endl;
        cin >> decision;

        userChoice enumeratedDecision = getDecision(decision);

        switch(enumeratedDecision){
            case VIEW: {
                myATM.showAccounts();
                break;
            }
            case CREATE: {
                Account newAccount;
                myATM.accountCreation(newAccount);
                break;
            }
            case DELETE: {
                string accountName;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter the name of the account you want to delete. ";
                getline(cin, accountName);
                myATM.accountDeletion(accountName);
                break;
            }
            case EXIT: {
                //save before exit
                myATM.saveAccountsToFile();
                globalSessionActive = false;
                cout << "Goodbye!";
                break;
            }
            case LOGIN: {
                string accountName;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter the name of the account you want to login to. ";
                getline(cin, accountName);
                bool accountFound = false;

                for(int i = 0; i<myATM.accountsOnFile.size(); i++){
                    if(myATM.accountsOnFile[i].getName() == accountName){
                        myATM.accountsOnFile[i].isSessionActive = true;
                        myATM.accountsOnFile[i].accountLogin();
                        if(myATM.accountsOnFile[i].successfulLogin){
                            myATM.userSession(myATM.accountsOnFile[i]);
                        }
                        accountFound = true;
                        break;
                    }
                }
                //checks all accounts in account vector, checks if they have the same name as the entered name, starts login method, if login successful, starts account session.

                if(!accountFound){
                    cout << "No account found with name " << accountName << endl;
                }
                break;
            }
            default: {
                cout << "Invalid input. Please try again." << endl;
            }
        }
    }
    return 0;
}
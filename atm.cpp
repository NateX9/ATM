#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

class Account {
    private:

        double accountbal = 0.0;
        //account balance starts at 0
        string accountname, accountpass;
        //initialize name and pass to be assigned later
        vector<string> accountHistory;
    public:

        double getBalance() {
            return accountbal;
        }

        string getName(){
            return accountname;
        }

        friend ostream& operator<<(ostream& os, const Account& account) { //operator overloading for couting classes
            os << "Account Name: " << account.accountname << "\n";
            os << "Balance: $" << fixed << setprecision(2) << account.accountbal << "\n";
            return os;
        }

        void depositMoney(double additionalmoney){
            ostringstream stream;
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
            }
        }

        bool successfulLogin = false;
        bool isSessionActive = true;

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
                    cout << "Incorrect password. Try Again." << endl;
                    cout << " " << endl;
                }
            }
            if(allowedLoginAttempts <= 0){
                cout << "Too many incorrect login attempts. Going back to menu. ";
            }
        }

        friend struct ATM;
};

enum accountChoice { BALANCE, WITHDRAW, DEPOSIT, EXIT, HISTORY, INVALID };

accountChoice getChoice(string input){
    if(input == "bal") return BALANCE;
    if(input == "wit") return WITHDRAW;
    if(input == "dep") return DEPOSIT;
    if(input == "hist") return HISTORY;
    if(input == "exit") return EXIT;
    return INVALID;
}
//Kinda confusing, but since strings aren't native to c++, switch statements can't switch through them. I work around this by switching through an enum which assigns
// a return value for each string input, and in the case of an unknown input, returns INVALID.

enum ATMchoice { DELETE, CREATE, VIEW, LEAVE, LOGIN, UNKNOWN };

ATMchoice getDecision(string input){
    if(input == "view") return VIEW;
    if(input == "create") return CREATE;
    if(input == "delete") return DELETE;
    if(input == "login") return LOGIN;
    if(input == "leave") return LEAVE;
    return UNKNOWN;
}

double getAmount(){
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

        void accountCreation(Account &accountName){
            string username, password;
            char choice;
            cout << "What is your name? ";
            cin >> username;

            cout << "\n Create a password. ";
            cin >> password;

            cout << "\n Are you sure about the username and pass? Y for yes, n for no (case sensitive): ";
            cin >> choice;

            bool accountFound = false;

            for(int i = 0; i<accountsOnFile.size(); i++){
                if(accountsOnFile[i].getName() == username){
                    accountFound = true;
                    break;
                }
            }

            if(accountFound){
                cout << "There is already an account with that name" << endl;
            }else if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Assuming no." << endl;
            }else if(choice == 'Y' && !accountFound){ //&& !accountFound is technically uneccessary but I added it to make myself feel better.
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
            bool success = false;
            char choice;

            for(int i = 0; i<accountsOnFile.size(); i++){
                if (accountsOnFile[i].accountname == accountName){
                    cout << "Do you want to delete the account named " << accountsOnFile[i].accountname << "? Y for yes, n for no (case sensitive) ";
                    cin >> choice;
                    if(cin.fail()){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Assuming no." << endl;
                    }else if(choice == 'Y'){
                        accountsOnFile.erase(accountsOnFile.begin() + i);
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

                accountChoice action = getChoice(choice);

                switch(action){ //big switch statement
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
};

int main(){
    ATM myATM;
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

        ATMchoice enumeratedDecision = getDecision(decision);

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
                cout << "Enter the name of the account you want to delete. ";
                cin >> accountName;
                myATM.accountDeletion(accountName);
                break;
            }
            case LEAVE: {
                globalSessionActive = false;
                cout << "Goodbye!";
                break;
            }
            case LOGIN: {
                string accountName;
                cout << "Enter the name of the account you want to login to. ";
                cin >> accountName;
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
}
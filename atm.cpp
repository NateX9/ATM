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
        ostringstream stream; //used to round floats that get pushed to the history vector

        void depositMoney(double additionalmoney){
            accountbal = accountbal + additionalmoney;
            cout << fixed << setprecision(2) << "You now have $" << accountbal << " dollars in your account!" << endl;
            stream.str(""); //This and the following line clears stream so multiple numbers dont print next to eachother.
            stream.clear();
            stream << fixed << setprecision(2) << additionalmoney;
            accountHistory.push_back("Deposited $" + stream.str());
            //setprecision(2) rounds to 2 decimals. like money.
        }

        void withdrawMoney(double subtractedMoney){
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

        void accountCreation(){
            cout << "Create a username. These are case sensitive. " << endl;
            cin >> accountname;
            cout << " " << endl;
            cout << "Create a password. These are case sensitive. " << endl;
            cin >> accountpass;
            cout << " " << endl;
            cout << "You're all set! You can now sign in." << endl;
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
                cout << "Enter Username below." << endl;
                cin >> user;
                cout << "Enter Password below." << endl;
                cin >> pass;
                if(user == accountname && pass == accountpass){
                    successfulLogin = true; //breaks out of while loop
                    cout << " " << endl;
                    cout << "You're in!" << endl;
                }else{
                    allowedLoginAttempts--;
                    cout << "Incorrect username or password. Try Again." << endl;
                    cout << " " << endl;
                }
            }
            if(allowedLoginAttempts <= 0){
                cout << "Too many incorrect login attempts. Shutting Down. ";
            }
        }

        double &balancereference = accountbal; //So that accountbal (private) can be referenced and changed outside of class
};


enum userChoice { BALANCE, WITHDRAW, DEPOSIT, EXIT, HISTORY, INVALID };

userChoice getChoice(string input){
    if(input == "bal") return BALANCE;
    if(input == "wit") return WITHDRAW;
    if(input == "dep") return DEPOSIT;
    if(input == "hist") return HISTORY;
    if(input == "exit") return EXIT;
    return INVALID;
}
//Kinda confusing, but since strings aren't native to c++, switch statements can't switch through them. I work around this by switching through an enum which assigns
// a return value for each string input, and in the case of an unknown input, returns INVALID.
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
    if(amount > currentATM.balancereference){
        cout << "You can only withdraw $" << currentATM.balancereference << "." << endl;
    }else{
        currentATM.withdrawMoney(amount);
    }
}

void performDeposit(Account &currentATM){
    cout << "How much would you like to deposit? " << endl;
    currentATM.depositMoney(getAmount());
}

int main(){
    Account atm1;

    atm1.accountCreation(); //runs through both methods
    atm1.accountLogin();

    while(atm1.successfulLogin && atm1.isSessionActive){
        string choice;
        cout << " " << endl;

        cout << "What would you like to do in your account? " << endl;

        cout << " " << endl;

        cout << "Note: You can view your balance by typing 'bal', withdraw money using 'wit' and deposit money using 'dep'." << endl;
        cout << "You can also check account history with 'hist' or leave the instance with 'exit'." << endl;

        cout << " " << endl;

        cin >> choice;

        userChoice action = getChoice(choice);

        switch(action){ //big switch statement
            case BALANCE: {
                atm1.checkBalance();
                break;
            }
            case WITHDRAW: {
                performWithdrawal(atm1);
                break;
            }
            case DEPOSIT: {
                performDeposit(atm1);
                break;
            }
            case HISTORY: {
                atm1.checkHistory();
                break;
            }
            case EXIT: {
                cout << "Goodbye!" << endl;
                atm1.isSessionActive = false;
                break;
            }
            default: {
                cout << "Invalid input. Try again. ";
            }
        }
    }
}
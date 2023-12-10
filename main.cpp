/*
Authors: Paul Gleason, David Thomsen, and James Cangelosi.
Class: CSI-140-06
Assignment: Final Project
Due Date: 12/10/23
Description:
This is our banking program. It allows the user to login as a teller and perform various actions on customer accounts. 
Certification of Authenticity:
I certify that this is entirely my own work, except where I have given
fully-documented references to the work of others. I understand the
definition and consequences of plagiarism and acknowledge that the assessor
of this assignment may, for the purpose of assessing this assignment:
- Reproduce this assignment and provide a copy to another member of
academic staff; and/or
- Communicate a copy of this assignment to a plagiarism checking
service (which may then retain a copy of this assignment on its
database for the purpose of future plagiarism checking)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_TELLERS = 10;
const int MAX_ACCOUNTS = 10000;

// Teller data
string tellerIds[MAX_TELLERS];
string tellerPasswords[MAX_TELLERS];

// Account data
string accountNumbers[MAX_ACCOUNTS];
string accountSSNs[MAX_ACCOUNTS];
string accountNames[MAX_ACCOUNTS];
string accountAddresses[MAX_ACCOUNTS];
string accountPhoneNumbers[MAX_ACCOUNTS];

// Initialize and declare the total tellers and accounts
int totalTellers = 0;
int totalAccounts = 0;

// Stores the total amount of tellers within totalTellers
void loadTellers() {
    // Sets the input file as tellers.dat
    ifstream file("tellers.dat");
    // Adds +1 to totalTellers for every teller account if it does not exceed the maximum
    while (file >> tellerIds[totalTellers] >> tellerPasswords[totalTellers] && totalTellers < MAX_TELLERS) {
        totalTellers++;
    }
    // Close the file
    file.close();
}

// Stores the total amount of customer accounts within totalAccounts
void loadAccounts() {
    // Sets the input file as accounts.dat
    ifstream file("accounts.dat");
    // Variable index is to target the first address of an array
    int index = 0;
    // Gets the account details at the address specified by variable index
    while (getline(file, accountNumbers[index]) && index < MAX_ACCOUNTS) {
        getline(file, accountSSNs[index]);
        getline(file, accountNames[index]);
        getline(file, accountAddresses[index]);
        getline(file, accountPhoneNumbers[index]);
        // Adds +1 to index so the loop can advance to the next account
        index++;
    }
    // Stores the value of index after the loop as a counter for the number of accounts
    totalAccounts = index;
    // Close the file
    file.close();
}

// Function to format a phone number as (XXX) XXX-XXXX
string formatPhoneNumber(const string& phoneNumber) {
    if (phoneNumber.substr(0,1) == "(") {
        return phoneNumber;
    }
    if (phoneNumber.length() != 10) {
        return "Invalid Phone Number";
    }
    return "(" + phoneNumber.substr(0, 3) + ")-" + phoneNumber.substr(3, 3) + "-" + phoneNumber.substr(6, 4);
}

// Function to format an SSN as XXX-XX-XXXX
string formatSSN(const string& ssn) {
    if (ssn.substr(3,1) == "-" && ssn.substr(6,1) == "-") {
        return ssn;
    }
    if (ssn.length() != 9) {
        return "Invalid SSN";
    }
    return ssn.substr(0, 3) + "-" + ssn.substr(3, 2) + "-" + ssn.substr(5, 4);
}

// Allows the teller to update customer account information
void updateAccountInformation() {
    string accountNumber;
    // Teller enters account information
    cout << "Enter account number you want to update: ";
    cin >> accountNumber;

    bool accountFound = false;
    string tempAccountNumber, tempSsn, tempName, tempAddress, tempPhoneNumber;

    // Sets the input file as accounts.dat
    ifstream inFile("accounts.dat");
    // Sets the output file as temp.dat
    ofstream outFile("temp.dat");

    // Copies the acount data to a temp file to be manipulated
    while (getline(inFile, tempAccountNumber)) {
        // Gets the data from the input file and stores it in temp strings
        getline(inFile, tempSsn);
        getline(inFile, tempName);
        getline(inFile, tempAddress);
        getline(inFile, tempPhoneNumber);

        // Checks to see if the account number pulled from the file is the same as the requested
        if (tempAccountNumber == accountNumber) {
            accountFound = true;
            cout << "Updating information for account " << accountNumber <<
            ". Leave blank to keep the old value.\n";

            string updatedInfo;

            // For all changes - If user input is nothing the value is unchanged

            // Changes the SSN
            cout << "SSN (" << tempSsn << "): ";
            cin.ignore(); // clear newline buffer
            getline(cin, updatedInfo);
            tempSsn = updatedInfo.empty() ? tempSsn : updatedInfo;

            // Changes the name
            cout << "Name (" << tempName << "): ";
            getline(cin, updatedInfo);
            tempName = updatedInfo.empty() ? tempName : updatedInfo;

            // Changes the address
            cout << "Address (" << tempAddress << "): ";
            getline(cin, updatedInfo);
            tempAddress = updatedInfo.empty() ? tempAddress : updatedInfo;

            // Changes the phone number
            cout << "Phone number (" << tempPhoneNumber << "): ";
            getline(cin, updatedInfo);
            tempPhoneNumber = updatedInfo.empty() ? tempPhoneNumber : updatedInfo;
        }

        // Stores the new account details in the temp file
        outFile << tempAccountNumber << "\n";
        outFile << formatSSN(tempSsn) << "\n";
        outFile << tempName << "\n";
        outFile << tempAddress << "\n";
        outFile << formatPhoneNumber(tempPhoneNumber) << "\n";
    }

    // Closes both files
    inFile.close();
    outFile.close();

    // Deletes outdated accounts.dat
    remove("accounts.dat");
    // Renames temp.dat to accounts.dat
    rename("temp.dat", "accounts.dat");

    // Produces an error if the account number was incorrect
    if (!accountFound) {
        cout << "Error: Account number " << accountNumber << " doesn't exist.\n";
    }
    // Verifies to the user that changes were made successfully
    else {
        cout << "Account information updated successfully!\n";
    }
}

// Allows the teller to login
bool verifyTellerLogin(string id, string password) {
    // Checks the teller's input with the teller file
    for (int i = 0; i < totalTellers; i++) {
        if (tellerIds[i] == id && tellerPasswords[i] == password) {
            return true;
        }
    }
    return false;
}

// Sets the account balance for a customer's account
void setBalance(string accountNumber, double amount) {
    string filename = accountNumber + ".dat";
    ofstream file("data/" + filename);
    // Teller inputs the amount they wish to set
    if (file) {
        // If the account number exists it is updated
        file << amount;
        cout << "Balance for account " << accountNumber << " updated to: $" << fixed << setprecision(2) << amount << "\n";
    } else {
        // If the account doesn't exist it provides the teller with an error
        cout << "Failed to update balance for account " << accountNumber << ".\n";
    }
    file.close();
}

// Prints the balance of a customer's account
double getBalance(string accountNumber) {
    string filename = accountNumber + ".dat";
    ifstream file("data/" + filename);
    double balance = 0;
    if (file) {
        file >> balance;
    }
    file.close();
    return balance;
}

// Deposits funds into a customer's account
void deposit(string accountNumber, double amount) {
    double balance = getBalance(accountNumber);
    setBalance(accountNumber, balance + amount);
}

// Withdraws funds from a customer's account
void withdraw(string accountNumber, double amount) {
    double balance = getBalance(accountNumber);                                                                                                                 
    // Checks to see if the account has enough funds to withdraw
    if (balance < 0){
        cout << "Error: Account is overdrafted by: $" << balance << ".\n";
        return;
    }else{
        if (balance < amount) {
            cout << "Error: Insufficient funds.\n";
            cout << "A $15 fee will be charged.\n";                                                                                 
            // If the account doesn't have enough funds it is charged a $15 fee
            setBalance(accountNumber, (balance - amount) - 15);
            return;
        }
        else if (amount < 0) {
            cout << "Error: Cannot withdraw a negative amount.\n";
            return;
        }
        else if (amount == 0) {
            cout << "Error: Cannot withdraw $0.\n";
            return;
        }
        else {
            setBalance(accountNumber, balance - amount);
        }
    }

}

// Allows the teller to search for a customer account based on attributes
void searchAccounts() {
    string attribute, query;

    // Asks the teller which attribute they want to search
    cout << "Which attribute do you want to search by? (accountNumber, ssn, name, address, phoneNumber): ";
    cin >> attribute;

    // Teller inputs the value in the format they specified
    cin.ignore(); // Clearing the newline buffer
    cout << "Enter the value to search for: ";
    getline(cin, query);

    bool found = false;

    // Sets input file as accounts.dat and declares temp strings
    ifstream file("accounts.dat");
    string tempAccountNumber, tempSsn, tempName, tempAddress, tempPhoneNumber;

    // Loop to search every account in the file
    while (getline(file, tempAccountNumber)) {
        getline(file, tempSsn);
        getline(file, tempName);
        getline(file, tempAddress);
        getline(file, tempPhoneNumber);

        // Matches the teller's input to the temp strings of the file
        if ((attribute == "accountNumber" && tempAccountNumber == query) ||
            (attribute == "ssn" && tempSsn == formatSSN(query)) ||
            (attribute == "name" && tempName == query) ||
            (attribute == "address" && tempAddress == query) ||
            (attribute == "phoneNumber" && tempPhoneNumber == formatPhoneNumber(query))) {
            
            cout << "Account found:\n";
            cout << "Account Number: " << tempAccountNumber << "\n";
            cout << "SSN: " << tempSsn << "\n";
            cout << "Name: " << tempName << "\n";
            cout << "Address: " << tempAddress << "\n";
            cout << "Phone Number: " << tempPhoneNumber << "\n\n";
            found = true;
        }
    }

    file.close();

    // Error if search failed
    if (!found) {
        cout << "No accounts found with the given attribute value.\n";
    }
}

// Delete customer account
void deleteAccount(string accountNumber) {
    // Creates a temp file output to store the new file contents of accounts.dat
    ifstream infile("accounts.dat");
    ofstream tempFile("temp.dat");

    string line;
    bool skipAccount = false;

    // If the account number of an account doesn't match the requested it is skipped
    while (getline(infile, line)) {
        if (line == accountNumber) {
            skipAccount = true;
        }
        
        if (skipAccount) {
            for (int i = 0; i < 4; i++) {
                getline(infile, line); 
            }
            skipAccount = false; 
        } else {
            tempFile << line << "\n";
        }
    }

    infile.close();
    tempFile.close();

    // Remove account from accountNumbers array
    for (int i = 0; i < totalAccounts; i++) {
        if (accountNumbers[i] == accountNumber) {
            for (int j = i; j < totalAccounts - 1; j++) {
                accountNumbers[j] = accountNumbers[j + 1];
                accountSSNs[j] = accountSSNs[j + 1];
                accountNames[j] = accountNames[j + 1];
                accountAddresses[j] = accountAddresses[j + 1];
                accountPhoneNumbers[j] = accountPhoneNumbers[j + 1];
            }
            totalAccounts--;
            break;
        }
    }

    // Deletes accounts.dat and replaces it with the temp file
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    // Removes the file associated with the specified account
    string filename = "data/" + accountNumber + ".dat";
    remove(filename.c_str());
    
    cout << "Account deleted successfully!\n";
}

// Adds a teller account
void addTeller() {
    ofstream file("tellers.dat", ios::app);
    string id, password;

    // Teller inputs the account information
    cout << "Enter teller ID: ";
    cin >> id;

    // Check if the teller ID already exists
    bool tellerExists = false;
    for (int i = 0; i < totalTellers; i++) {
        if (tellerIds[i] == id) {
            tellerExists = true;
            break;
        }
    }
    if (tellerExists) {
        cout << "Error: A teller with the same ID already exists.\n";
    } else {
        // Teller inputs the password
        cout << "Enter teller password: ";
        cin >> password;
            // Prints the data into the file
        file << id << " " << password << "\n";
        file.close();

        cout << "Teller added successfully!\n";
    }
}

// Adds a customer account
void addAccount() {
    ofstream file("accounts.dat", ios::app);
    string accountNumber, ssn, name, address, phoneNumber;

    // Teller inputs customer information
    cout << "Enter account number: ";
    cin >> accountNumber;


    // Error if the account number already exists
    bool accountExists = false;
    for (int i = 0; i < totalAccounts; i++) {
        if (accountNumbers[i] == accountNumber) {
            accountExists = true;
            break;
        }
    }
    if (accountExists) {
        cout << "Error: An account with the same account number already exists.\n";
    } else {
        // Collect the rest of the information if the account number is unique
        cout << "Enter SSN: ";
        cin >> ssn;
        cin.ignore();
        cout << "Enter name: ";
        getline(cin, name);
        cout << "Enter address: ";
        getline(cin, address);
        cout << "Enter phone number: ";
        getline(cin, phoneNumber);

        // Format phone number and SSN
        string formattedPhoneNumber = formatPhoneNumber(phoneNumber);
        string formattedSSN = formatSSN(ssn);

        if (formattedPhoneNumber == "Invalid Phone Number") {
            cout << "Error: Invalid phone number.\n";
            cout << "Enter phone number: ";
            getline(cin, phoneNumber);
            formattedPhoneNumber = formatPhoneNumber(phoneNumber);
        } 

        if (formattedSSN == "Invalid SSN") {
            cout << "Error: Invalid SSN.\n";
            cout << "Enter SSN: ";
            cin >> ssn;
            formattedSSN = formatSSN(ssn);
        }
        
        // Prints the data to the file
        file << accountNumber << "\n";
        file << formattedSSN << "\n";
        file << name << "\n";
        file << address << "\n";
        file << formattedPhoneNumber << "\n";
        file.close();

        // Creates a file for the account balance
        string filename = "data/" + accountNumber + ".dat";
        ofstream balanceFile(filename);

        // Teller enters the initial balance of the account
        cout << "Enter initial balance: ";
        double balance;
        cin >> balance;
        if (balance < 0) {
            cout << "Error: Cannot deposit a negative amount.\n";
            cout << "Enter initial balance: ";
            double balance;
            cin >> balance;
            deposit(accountNumber, balance);
        }else{
            deposit(accountNumber, balance);
        }

        cout << "Account added successfully!\n";
    }
}

int main() {
    loadTellers();
    loadAccounts();

    // Teller inputs the login command
    string input;
    cout << "Enter command l=login: ";
    cin >> input;

    // Teller inputs their credentials
    if(input == "l") {
        string id, password;
        cout << "Enter teller ID: ";
        cin >> id;
        cout << "Enter password: ";
        cin >> password;

        // Program verifies teller credentials
        if (verifyTellerLogin(id, password)) {
            cout << "Login successful!\n";
            // Teller selects a command
            while (true){
                string teller_input;
                cout << "Enter command (b=balance, d=deposit, w=withdraw, s=search, at=add teller, aa=add account, da=delete account, ua=update account) or 'leave' to exit : ";
                cin >> teller_input;

                // Exits the program
                if (teller_input == "leave") {
                    break;
                }

                // Check the balance of a specified account
                if(teller_input == "b") {
                    string accountNumber;
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    cout << "Account " << accountNumber << " has $" << fixed << setprecision(2) << getBalance(accountNumber) << endl;
                }

                // Deposits funds into specified account
                else if(teller_input == "d") {
                    string accountNumber;
                    double amount;
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    cout << "Enter amount: ";
                    cin >> amount;
                    deposit(accountNumber, amount);
                }

                // Withdraws funds from a specified account
                else if(teller_input == "w"){
                    string accountNumber;
                    double amount;
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    cout << "Enter amount: ";
                    cin >> amount;
                    withdraw(accountNumber, amount);
                }

                // Searches for accounts based on customer attributes
                else if(teller_input == "s"){
                    searchAccounts();
                }

                // Adds a teller account
                else if(teller_input == "at") {
                    addTeller();
                }

                // Adds a customer account
                else if(teller_input == "aa") {
                    addAccount();
                }

                // Deletes customer account
                else if(teller_input == "da"){
                    string accountNumber;
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    deleteAccount(accountNumber);
                }

                // Updates customer account
                else if(teller_input == "ua"){
                    updateAccountInformation();
                }

                // Error if the command isn't recognized
                else {
                    cout << "Unknown command.\n";
                }
            }
        }
        // If the teller credentials are not verified
        else {
            cout << "Invalid ID or password.\n";
        }
    }
    else {
        cout << "Unknown command.\n";
    }
    return 0;
}

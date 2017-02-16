/*
	-----------------------------------------------
	Berry Bank
	-----------------------------------------------
	Simple bank program
	
	Reads account data from files, allows deposits,
	withdrawls and opening new accounts.
	The data for the accounts is stored encrypted
	to ensure privacy of account holders using 
	custom encryption algorithm.
	The accounts use a user generated 6 letter key
	for decrypting account data, which also serves
	as a password.
		
	-----------------------------------------------
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <cstring>
#include <ctype.h>
#include <math.h>
#include "rlutil.h"

typedef long long ll;

using namespace std;
using namespace rlutil;

class Account{
	public:
		Account(void){              // Default constructor
            strcpy(holder, "NoHolder");
            strcpy(key, "aaaaaa");
            bal = 0;
		};
		Account(char *, char *);    // Initialising constructor

		friend void init();         // Gets data from files
		friend void wrap();			// Writes changed data to files
		friend void prt(string);	// Prints inputted string to the center of the screen
		string getname();           // Returns name of account holder
		unsigned getacc();          // Returns account number
		ll getbal();                // Returns balance
		void trans(long long);      // Alters balance for deposit/withdrawl
		int login(char *);          // Decrypts name and balance as per given key
		void encrypt();				// Re-encrypts data for storage
		
		void operator = (Account & a){ 
		/* 
        Overloaded '=' operator
        Takes an 'Account' reference, copies data from it
        and pastes it into the Lvalue reference given
        */
			strcpy(holder, a.holder);
			strcpy(key, a.key);
			bal = a.bal;
			acc_num = a.acc_num;
		}

	private:
		char holder[30];             // Account holder name [ENCRYPTED]
		unsigned acc_num;            // Account number
		ll bal;                      // Current account balance [ENCRYPTED]
		char key[7];                 // Holds encryption key for account(basically, password)
};

Account account[100];				 // Store all account data, supporting a maximum of 100

int max_acc = 0;                   	 // Currently opened accounts(total)

int h,w;							 // Height and width of screen

int inp_acc;			    	  	 // Account number of acocunt being accessed

void init();
void wrap();
void prt(string);
void frame();

int main(){

	init();

	h = rlutil::trows();
	w = rlutil::tcols();

	int inp;

	start:

	rlutil::cls();
    rlutil::setColor(WHITE);

	frame();
	gotoxy(0,8);
	cout << endl;
	prt("Welcome to ABC Bank!"); cout << endl;
	prt("Please choose desired option");
	
	gotoxy(((w - strlen("Please choose desired option"))/2+1), 11);
	cout << "1. Access account";
	
	gotoxy(((w - strlen("Please choose desired option"))/2+1), 12);
	cout << "2. Open account";
	
	gotoxy(((w - strlen("Please choose desired option"))/2+1), 13);
	cout << "3. Quit" << endl << endl;
	prt("Enter choice : ");

	inp = getch();

	switch((int) inp){
		case 49:
			goto access;
		case 50:
			goto open;
		case 51:
			goto exit;
		default:
			rlutil::setColor(LIGHTRED);
			prt("Invalid choice!!");
			getch();
			goto start;
	}

	
	open:
	{
		char holder[30], key[7];	// User inputted data for new account

		rlutil::cls();
		frame();
		gotoxy(0,9);
		prt("Opening new account"); cout << endl;
		prt("Enter account holder name(max 30 letters)");
		gotoxy(w/2 - 12, 11);
		int counter = 0;
		for(int i=0; i<999; i++){
			if(counter>28){
				break;
			}
			char ch = getch();
			if(ch=='\n' || ch=='\r'){
				if(counter > 0){
					break;
				}
			}
			if(ch=='\b'){
				cout << '\b';
				cout << ' ';
				cout << '\b';
				counter--;
				continue;
			}
			if(isalpha(ch) || ch == ' '){
				// Ignores non-alphabetic input, accepts only spaces
				holder[counter++] = ch;
				cout << ch;
			}
			else{
				continue;
			}
		}

		rlutil::cls();
		frame();
		gotoxy(0,9);
		prt("Opening new account"); cout << endl;
		prt("Enter account key(6 letters)"); cout << endl;

		counter = 0;
		for(int j=0; j<999; j++){
			if(counter>5){
				char b[7];
				for(int k=0; k<6; k++){
					b[k] = key[k];
				}
				// Filters possible garbage captured from input buffer
				b[6] = (char) 0;
				Account acc(holder, b);
				// Uses overloaded '=' operator to copy account data
				account[max_acc - 1] = acc;
				// Encrypt data of new account for storage
				account[max_acc - 1].encrypt();
				rlutil::setColor(WHITE);
				cout << endl << endl;
				prt("Account opened with account number ");
				rlutil::setColor(WHITE);
				cout << acc.getacc();
				getch();
				break;
			}
			char ch = getch();
			if(ch=='\b'){
				cout << '\b';
				cout << ' ';
				cout << '\b';
				counter--;
				continue;
			}
			else if(ch==(char) 127){
                cout << (char) 127;
                cout << ' ';
                cout << (char) 127;
                counter--;
                continue;
			}
			if(isalpha(ch)){
				// Ignores non-alphabetic input
				key[counter++] = ch;
				rlutil::setColor(YELLOW);
				cout << '*';
			}
			else{
				continue;
			}
		}

		goto start;
	}

	access:
	{
		char key[7];		// User inputted 6 letter password
		rlutil::cls();
		frame();
		gotoxy(0,9);
		prt("Accessing existing account"); cout << endl;
		prt("Enter account number for your account"); cout << endl << endl;

		while(1)
		{
			cin >> inp_acc;
			if(cin.fail()){
				rlutil::setColor(LIGHTRED);
				prt("Invalid input!!");
				getch();
				cin.clear();
				cin.ignore(100, '\n');
				goto access;
			}
			if(inp_acc>0 && inp_acc<=max_acc){
				break;
			}
			else{
				rlutil::setColor(LIGHTRED);
				prt("Invalid input!!");
				getch();
				goto access;
			}
		}

		pass:

		rlutil::cls();
		frame();
		gotoxy(0,9);
		prt("Enter key for account"); cout << endl;
		int counter = 0;
		for(int i=0; i<9999; i++){
			if(counter > 5){
                cout << endl << endl;
				if(account[inp_acc-1].login(key) == 0){
					rlutil::setColor(LIGHTGREEN);
					prt("Login success!!");
					getch();
					goto acc_success;
				}
				else{
					rlutil::setColor(LIGHTRED);
					prt("Invalid key!! Please retry, enter ~ to quit");
					getch();
					goto pass;
				}
			}
			char ch = getch();
			if(ch=='~'){
				goto exit;
			}
			if(ch=='\b'){
				cout << '\b';
				cout << ' ';
				cout << '\b';
				counter--;
				continue;
			}
			if(isalpha(ch)){
				// Ignores non-alphabetic input
				key[counter] = ch;
				rlutil::setColor(YELLOW);
				cout << '*';
				counter++;
			}
			else{
				continue;
			}

		}

	}

	acc_success:
	{
		ll amount;

		rlutil::cls();
		frame();
		gotoxy(0,2);
		prt("Accessing account #");
		account[inp_acc-1].getacc(); cout << endl;
		prt("Account Holder"); cout << endl;
		rlutil::setColor(WHITE);
		prt(account[inp_acc-1].getname()); cout << endl;
		rlutil::setColor(LIGHTBLUE);
        string bal_prt = "Balance Rs. ";
		bal_prt = bal_prt.append(to_string(account[inp_acc-1].getbal()));
		prt(bal_prt);

		cout << endl << endl;
		prt("Choose desired task");
		gotoxy(((w - strlen("Choose desired task"))/2+1), 8);
		cout << "1. Deposit money";
		gotoxy(((w - strlen("Choose desired task"))/2+1), 9);
		cout << "2. Withdraw money";
		gotoxy(((w - strlen("Choose desired task"))/2+1), 10);
		cout << "3. Log out";

		int inp2 = getch();

		switch(inp2){
		case 49:
		    cout << endl << endl;
			prt("Enter amount ");
			cin >> amount;	cin.ignore(100, '\n');
			account[inp_acc-1].trans(abs(amount));
			getch();
			goto acc_success;
		case 51:
			account[inp_acc-1].encrypt();
			goto start;
		case 50:
		    cout << endl << endl;
			prt("Enter amount ");
			cin >> amount;	cin.ignore(100, '\n');
			account[inp_acc-1].trans(-abs(amount));
			getch();
			goto acc_success;
		default:
		    cout << endl << endl;
			rlutil::setColor(LIGHTRED);
			prt("Invalid choice!!");
			getch();
			goto acc_success;
		}
	}

	exit:
	wrap();
	return 0;
}


/*
	*****************************************
			   Function definitions
	*****************************************
*/

void init(){                     
	/*
	Initializes the program, obtains account data from files
	Opens files 
		holder.txt		Account holder names
		pass.txt		Account keys
		balance.txt 	Account balances
	Reads data and stores them into account[] objects
	Terminates read if default holder 'NoHolder' is reached
	*/
	
    ifstream hld ("holder.txt", ios_base::in);
    ifstream pass ("pass.txt", ios_base::in);
    ifstream balance ("balance.txt", ios_base::in);

    for(int i=0; i<100; i++){
        string h,k,b;
        getline(hld, h);
        getline(pass, k);
        getline(balance, b);

        if(strncmp(h.c_str(), "NoHolder", 8) != 0){
            strcpy(account[i].holder, h.c_str());
            strcpy(account[i].key, k.c_str());
            char *pEnd;
            account[i].bal = strtoll(b.c_str(), &pEnd , 10);
            ++max_acc;
        }
    }

/*
	strcpy(account[0].holder, "Sankalp Gambhir");
	strcpy(account[0].key, "abcdef");
	account[0].acc_num = 1;
	account[0].bal = 10000;
	max_acc = 1;
	account[0].encrypt();
*/

}

void frame(){                         
	/* 
		Generates a frame at the edges of the screen
	*/
	
	gotoxy(1,1);
	for(int i = 0; i < w; i++)
	{
		cout << '*';
	}

	for(int i = 2; i <= h - 2; i++)
	{
		gotoxy(1, i); cout << '*';
		gotoxy(w, i); cout << '*';
	}

	gotoxy(1, h - 1);
	for(int i = 0; i < w; i++)
	{
		cout << '*';
	}
}

void prt(string str){                 
	/* 
	Prints given string at the center of the current line
	Uses preset color using setColor() function of rlutil.h
	Resets color to WHITE for further processing
	*/
	
	int width = rlutil::tcols();

    cout << '*' << string( (width-str.size())/2, ' ' ) << str;

    rlutil::setColor(WHITE);
}

void wrap(){                       
	/* 
	Ends the program, writes data back into files
	Opens files 
		holder.txt		Account holder names
		pass.txt		Account keys
		balance.txt 	Account balances
	Truncates already present data
	To the empty file, all data is written, even for empty accounts
	Holder name and balance are stored encrypted
	*/
	
    ofstream hld ("holder.txt", ios_base::trunc);
    ofstream pass ("pass.txt", ios_base::trunc);
    ofstream balance ("balance.txt", ios_base::trunc);

    for(int i=0; i<100; i++){
        hld << string (account[i].holder) << '\n';
        pass << string (account[i].key) << '\n';
        balance << account[i].bal << '\n';
    }

    hld.close();
    pass.close();
    balance.close();
}


/*
	*****************************************
	Definitions of functions of class Account
	*****************************************
*/

Account::Account(char * hld, char * pass){
	/*
    Creates an instance of the class Account (constructor)
    Initializes holder name and password as per given values
    Initializes account balance as 0
    Initializes account number as one more than the number of maximum accounts
    while incrementing the maximum number of accounts
    */
	
	for(int i=0; i<30; i++){
		holder[i] = hld[i];
	}
	for(int j=0; j<7; j++){
		key[j] = pass[j];
	}
	acc_num = ++max_acc;
	bal = 0;
}

string Account::getname(){      // Returns account holder name(private entity)
	return string (holder);
}

unsigned Account::getacc(){     // Returns account number(private entity)
	return acc_num;
}

ll Account::getbal(){           // Returns account balance(private entity)
	return bal;
}

void Account::trans(ll a){        
	/* 
    Carries out transactions, takes amount as input and 
	an integer, positive for deposit, negative for withdrawl
	Prints 'Withdrawl failure' if withdrawl is opted & balance
	is less than given amount
	*/
	
    cout << endl << endl;
	if(a<0){
		if(bal<abs(a)){
			rlutil::setColor(LIGHTRED);
			prt("Withdrawl fail! Invalid amount!");
			getch();
			goto ex;
		}
	}
	bal += a;
	if(a>0){
		rlutil::setColor(LIGHTGREEN);
		prt("Deposit successful!");
	}
	else if(a<0){
		rlutil::setColor(LIGHTGREEN);
		prt("Withdrawl successful!");
	}

	cout << endl << endl;
	{
	    string s = "New balance is Rs. ";
        s = s.append(to_string(bal));
        prt(s);
	}
	ex:
	    ;
}

int Account::login(char * a){
    /* 
	Checks given password against account key
	Uses strncmp(char *, char *, int) to compare only 6 letters
	Ignores garbage data that might have been captured from stream
	by ignoring anything after the 6 letters of key
	Returns 0 on success, 1 on failure
	*/
	
	int match = 0;
	int i,j;
	char b[6];
	for(int k=0; k<6; k++){
		b[k] = a[k];
	}
	b[6] = (char) 0;

	if(strncmp(b, key, 6) == 0){
		match++;
	}

	if(match == 1){
		goto decrypt;
	}
	else{
		return 1;
	}

	decrypt:
			for(j=0; j<30; j++){
				int m = j%7;
				char d = key[m];
				holder[j] -= (int) d;
				bal -= (int) d;
			}
			return 0;
}

void Account::encrypt(){              
	/* 
	Encrypts account holder name and balance for storage
	Uses the user-defined key
	It adds the (int) value of a letter of the key
	to both holder name and balance
	In the holder name(char *), each letter is altered
	In the balance(double), the effect compounds with each loop
	*/
	
	for(int j=0; j<30; j++){
		int m = j%7;
		char d = key[m];
		holder[j] += (int) d;
		bal += (int) d;
	}
}

//end-of-file

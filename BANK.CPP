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

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

typedef unsigned long ll;

class Account{
	public:
		Account(void){            // Default constructor
			strcpy(holder, "NoHolder");
			strcpy(key, "aaaaaa");
			bal = 0;
		};
		Account(char *, char *);  // Initialising constructor

		friend void init(void);   // Gets encrypted data from files
		friend void wrap(void);	  // Writes changed data to files
		friend void prt(char *);  // Prints inputted string to the center of the screen
		char * getname(void);     // Returns name of account holder
		unsigned getacc(void);    // Returns account number
		ll getbal(void);          // Returns balance
		void trans(ll, int);      // Alters balance for deposit/withdrawl
		int login(char *);        // Decrypts name and balance as per given key
		void encrypt(void);		  // Re-encrypts data for storage
    
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
		char holder[30];    // Account holder name [ENCRYPTED]
		unsigned acc_num;   // Account number
		ll bal;             // Current account balance [ENCRYPTED]
		char key[7];        // Holds encryption key for account(basically, password)
};

Account account[100];       // Store all account data, supporting a maximum of 100

int max_acc = 0;            // Currently opened accounts(total)

int h,w;					// Height and width of screen

int inp_acc;			    // Account number of account being accessed

void init();
void wrap();
void prt(char *);
void frame();

int main(){

	init();

	struct text_info window;
	gettextinfo(&window);
	h = window.screenheight - 2;
	w = window.screenwidth - 2;

	int inp;

	start:

	clrscr();
	textcolor(WHITE);

	frame();
	gotoxy((w-strlen("Welcome to Berry Bank!"))/2, 9);
	prt("Welcome to ABC Bank!");
	
	gotoxy((w-strlen("Please choose desired option"))/2, wherey()+1);
	prt("Please choose desired option");
	
	gotoxy(((w - strlen("Please choose desired option"))/2+1), wherey()+1);
	cprintf("%s", "1. Access account");
	
	gotoxy(((w - strlen("Please choose desired option"))/2+1), wherey()+1);
	cprintf("%s", "2. Open account");
	
	gotoxy(((w - strlen("Please choose desired option"))/2+1), wherey()+1);
	cprintf("%s", "3. Quit");
	
	gotoxy((w-strlen("Enter choice : "))/2, wherey()+2);
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
			textcolor(LIGHTRED);
			gotoxy((w-strlen("Invalid choice!!"))/2, wherey()+2);
			prt("Invalid choice!!");
			
			getch();
			goto start;
	}

    open:
	{
		char holder[30], key[7];       // User inputted details for new account

		clrscr();
		frame();
    
		gotoxy((w-strlen("Opening new account"))/2, 9);
		prt("Opening new account");
		
		gotoxy((w-strlen("Enter account holder name(max 30 letters)"))/2, wherey()+1);
		prt("Enter account holder name(max 30 letters)");
		
		gotoxy(w/2 - 12, wherey()+1);
    
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
			else if(ch==(char) 127){
				cout << (char) 127;
				cout << ' ';
				cout << (char) 127;
				counter--;
				continue;
			}
			if(isalpha(ch) || ch == ' '){                
				// Ignores non-alphabetic inputs, accepts only spaces
				holder[counter++] = ch;
				cout << ch;
			}
			else{
				continue;
			}
		}

		clrscr();
		frame();
		
		gotoxy((w-strlen("Opening new account"))/2, 9);
		prt("Opening new account");
		
		gotoxy((w-strlen("Enter account key(6 letters)"))/2, wherey()+1);
		prt("Enter account key(6 letters)");

		counter = 0;
		gotoxy((w-6)/2, wherey()+1);
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
				
				textcolor(WHITE);
				gotoxy((w-strlen("Account opened with account number    "))/2, wherey()+2);
				prt("Account opened with account number    ");
				textcolor(WHITE);
				cout << "\b\b\b";
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
				textcolor(YELLOW);
				cprintf("%c", '*');
			}
			else{
				continue;
			}
		}

		goto start;
	}

	access:
	{
		char key[7];           // User inputted 6-letter password
    
		clrscr();
		frame();
    
		gotoxy((w-strlen("Accessing existing account"))/2, 9);
		prt("Accessing existing account");
		gotoxy((w-strlen("Enter account number for your account"))/2, wherey()+1);
		prt("Enter account number for your account");

		gotoxy((w-2)/2, wherey()+1);
		while(1)
		{
			cin >> inp_acc;
			if(cin.fail()){
				textcolor(LIGHTRED);
				gotoxy((w-strlen("Invalid input!!"))/2, wherey()+2);
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
				textcolor(LIGHTRED);
				gotoxy((w-strlen("Invalid input!!"))/2, wherey()+2);
				prt("Invalid input!!");
				getch();
				goto access;
			}
		}

		pass:

		clrscr();
		frame();
    
		gotoxy((w-strlen("Enter key for account"))/2, 9);
		prt("Enter key for account");
		
		gotoxy((w-6)/2, wherey()+1);
		int counter = 0;
		for(int i=0; i<9999; i++){
			if(counter > 5){
				if(account[inp_acc-1].login(key) == 0){
					textcolor(LIGHTGREEN);
					gotoxy((w-strlen("Login success!!"))/2, wherey()+2);
					prt("Login success!!");
					getch();
					goto acc_success;
				}
				else{
					textcolor(LIGHTRED);
					gotoxy((w-strlen("Invalid key!! Please retry, enter ~ to quit"))/2, wherey()+2);
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
			else if(ch==(char) 127){
				cout << (char) 127;
				cout << ' ';
				cout << (char) 127;
				counter--;
				continue;
			}
			if(isalpha(ch)){              
				// Ignores non-alphabetic input
				key[counter] = ch;  
				textcolor(YELLOW);
				cprintf("%c", '*');
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

		clrscr();
		frame();
    
		gotoxy((w-strlen("Accessing account #   "))/2, 4);
		prt("Accessing account #   ");
		cout << "\b\b\b" << account[inp_acc-1].getacc();
		
		gotoxy((w-strlen("Account Holder"))/2, wherey()+1);
		prt("Account Holder");
		
		textcolor(WHITE);
		gotoxy((w-strlen(account[inp_acc-1].getname()))/2, wherey()+1);
		prt(account[inp_acc-1].getname());
		
		gotoxy((w-strlen("Balance Rs.     "))/2, wherey()+1);
		prt("Balance Rs.     ");
		cout << "\b\b\b\b" << account[inp_acc-1].getbal();

		gotoxy((w-strlen("Choose desired task"))/2, wherey()+2);
		prt("Choose desired task");
		
		gotoxy(((w - strlen("Choose desired task"))/2+1), wherey()+1);
		cprintf("%s", "1. Deposit money");
		
		gotoxy(((w - strlen("Choose desired task"))/2+1), wherey()+1);
		cprintf("%s", "2. Withdraw money");
		
		gotoxy(((w - strlen("Choose desired task"))/2+1), wherey()+1);
		cprintf("%s", "3. Log out");

		int inp2 = getch();

		switch(inp2){
		case 49:
			gotoxy((w-strlen("Enter amount "))/2, wherey()+2);
			prt("Enter amount ");
			cin >> amount;	cin.ignore(100, '\n');
			account[inp_acc-1].trans(amount, 1);
			getch();
			goto acc_success;
		case 51:
			account[inp_acc-1].encrypt();
			goto start;
		case 50:
			gotoxy((w-strlen("Enter amount "))/2, wherey()+2);
			prt("Enter amount ");
			cin >> amount;	cin.ignore(100, '\n');
			account[inp_acc-1].trans(amount, -1);
			getch();
			goto acc_success;
		default:
			textcolor(LIGHTRED);
			gotoxy((w-strlen("Invalid choice!!"))/2, wherey()+2);
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
	
	ifstream holder_file ("holder.txt", ios::in);
	ifstream key_file ("pass.txt", ios::in);
	ifstream balance_file ("balance.txt", ios::in);

	for(int i=0; i<100; i++){
		char h[30],k[7];
		ll b;
		holder_file >> h;
		key_file >> k;
		balance_file >> b;

		if(strncmp(h, "NoHolder", 8) != 0){
			strcpy(account[i].holder, h);
			strcpy(account[i].key, k);
			account[i].bal = b;
			account[i].acc_num = ++max_acc;
		}
	}

	holder_file.close();
	key_file.close();
	balance_file.close();
}

void frame(){                            
	/* 
		Generates a frame at the edges of the screen
	*/
	struct text_info window;
	gettextinfo(&window);
	int height = window.screenheight;
	int width = window.screenwidth;

	gotoxy(1,2); cprintf("%c", (char) 201);
	gotoxy(1,height); cprintf("%c", (char) 200);
	gotoxy(width,2); cprintf("%c", (char) 187);
	gotoxy(width,height); cprintf("%c", (char) 188);

	for(int i = 2; i<width; i++){
		gotoxy(i,1); cprintf("%c", (char) 205);
		gotoxy(i,height-1); cprintf("%c", (char) 205);
	}

	for(i = 2; i<(height-1); i++){
		gotoxy(1,i); cprintf("%c", (char) 186);
		gotoxy(width,i); cprintf("%c", (char) 186);
	}
}

void prt(char * str){                   
	/* 
	Prints given string at the center of the current line
	Uses preset color using textcolor() function of conio.h
	Resets color to WHITE for further processing
	*/
	
	gotoxy((w-strlen(str))/2,wherey());
	cprintf("%s", str);
	textcolor(WHITE);
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
	
	ofstream holder_file ("holder.txt", ios::trunc);
	ofstream key_file ("pass.txt", ios::trunc);
	ofstream balance_file ("balance.txt", ios::trunc);

	for(int i=0; i<100; i++){
		holder_file << account[i].holder << '\n';
		key_file << account[i].key << '\n';
		balance_file << account[i].bal << '\n';
	}

	holder_file.close();
	key_file.close();
	balance_file.close();
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

char * Account::getname(){     // Returns account holder name(private entity)
	return holder;
}

unsigned Account::getacc(){    // Returns account number(private entity)
	return acc_num;
}

ll Account::getbal(){          // Returns account balance(private entity)
	return bal;
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
	char b[6];
	for(int k=0; k<6; k++){
		b[k] = a[k];
	}
	b[6] = (char) 0;

	if(strncmp(b, key, 6) == 0){
		match++;
	}

	if(match == 1){
        for(int j=0; j<30; j++){
            int m = j%7;
            char d = key[m];
            holder[j] -= (int) d;
            bal -= (int) d;
        }
        return 0;
	}
	else{
		return 1;
	}
}

void Account::trans(ll a, int b){           
	/* 
    Carries out transactions, takes amount as input and 
	an integer, positive for deposit, negative for withdrawl
	Prints 'Withdrawl failure' if withdrawl is opted & balance
	is less than given amount
	*/
	if(b<0){
		if(bal<a){
			textcolor(LIGHTRED);
			gotoxy((w-strlen("Withdrawl fail! Invalid amount!"))/2, wherey()+2);
			prt("Withdrawl fail! Invalid amount!");
			getch();
			goto ex;
		}
	}
	bal += a;
	if(b>0){
		textcolor(LIGHTGREEN);
		gotoxy((w-strlen("Deposit successful!"))/2, wherey()+2);
		prt("Deposit successful!");
	}
	else if(b<0){
		textcolor(LIGHTGREEN);
		gotoxy((w-strlen("Withdrawl successful!"))/2, wherey()+2);
		prt("Withdrawl successful!");
	}

	gotoxy((w-strlen("New balance is Rs.     "))/2, wherey()+1);
	prt("New balance is Rs.     ");
	cout << "\b\b\b\b" << bal;

	ex:
		;
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

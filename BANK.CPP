#include <iostream.h>
#include <fstream.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>


class Account{
	public:
		Account(void){};            // Default constructor
		Account(char *, char *);    // Initialising constructor

		friend void init();         // gets data from files
		friend void wrap();			// writes changed data to files
		friend void prt(const char *, int);
		void getname();             // Prints name of account holder
		void getacc();              // Prints account number
		void getbal();              // Prints balance
		void trans(int);            // Alters balance for deposit/withdrawl
		void encrypt();				// Re-encrypts data for storage
		int login(char *);          // Decrypts name and balance as per given key
		void operator = (Account & a){
			strcpy(holder, a.holder);
			strcpy(key, a.key);
			bal = a.bal;
			acc_num = a.acc_num;
		}

	private:
		char holder[30];            // ENCRYPTED till login()
		int acc_num;                // Account number
		double bal;                 // Current account balance [ENCRYPTED]
		char key[7];                // Holds encryption key for account(basically, password)
};

Account account[100];

int max_acc = 0, max_acc_prev;      // Currently opened accounts(total)

int h,w;							// Height and width of screen

int inp_acc;			    		// accessed account number

int main(){

	void frame();
	init();

	struct text_info info;
	gettextinfo(&info);

	h = ( (int) info.screenheight) - 2;
	w = ( (int) info.screenwidth) - 2;

	int inp;

	start:

	clrscr();
	textcolor(LIGHTGRAY);

	frame();
	prt("Welcome to ABC Bank!", 9);init();
	prt("Please choose desired option", wherey()+1);
	gotoxy(((w - strlen("Please choose desired option"))/2+1), wherey()+1);
	cprintf("%s", "1. Access account");
	gotoxy(((w - strlen("Please choose desired option"))/2+1), wherey()+1);
	cprintf("%s", "2. Open account");
	gotoxy(((w - strlen("Please choose desired option"))/2+1), wherey()+1);
	cprintf("%s", "3. Quit");
	prt("Enter choice : ", wherey()+2);

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
			prt("Invalid choice!!", wherey()+2);
			getch();
			goto start;
	}

	access:
	{
		char key[7];
		clrscr();
		frame();
		prt("Accessing existing account", 9);
		prt("Enter account number for your account", wherey()+1);
		gotoxy(w/2 - 1, wherey()+1);

		while(1)
		{
			cin >> inp_acc;
			if(cin.fail()){
				textcolor(LIGHTRED);
				prt("Invalid input!!", wherey()+1);
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
				prt("Invalid input!!", wherey()+1);
				getch();
				goto access;
			}
		}

		pass:

		clrscr();
		frame();
		prt("Enter key for account", 9);
		gotoxy(w/2 - 3, wherey()+1);
		int counter = 0;
		for(int i=0; i<9999; i++){
			if(counter > 5){
				if(account[inp_acc-1].login(key) == 0){
					textcolor(LIGHTGREEN);
					prt("Login success!!", wherey()+2);
					getch();
					goto acc_success;
				}
				else{
					textcolor(LIGHTRED);
					prt("Invalid key!! Please retry, enter ~ to quit", wherey()+2);
					getch();
					goto pass;
				}
			}
			char ch = getch();
			if(ch=='~'){
				goto exit;
			}
			if(ch=='\b'){
				cprintf("%c", '\b');
				counter--;
				continue;
			}
			if(isalpha(ch)){
				key[counter] = ch;
				textcolor(YELLOW);
				cprintf("%c",'*');
				counter++;
			}
			else{
				continue;
			}

		}

	}

	open:
	{
		char holder[30], key[7];

		clrscr();
		frame();
		prt("Opening new account", 9);
		prt("Enter account holder name(max 30 letters)", wherey()+1);
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
				cprintf("%c", '\b');
				counter--;
				continue;
			}
			if(isalpha(ch) || ch == ' '){
				holder[counter++] = ch;
				cprintf("%c", ch);
			}
			else{
				continue;
			}
		}

		clrscr();
		frame();
		prt("Opening new account", 9);
		prt("Enter account key(6 letters)", wherey()+1);
		gotoxy(w/2 - 3, wherey()+1);
		counter = 0;
		for(i=0; i<999; i++){
			if(counter>5){
				char b[7];
				for(int k=0; k<6; k++){
					b[k] = key[k];
				}
				b[6] = (char) 0;
				Account acc(holder, b);
				account[max_acc - 1] = acc;
				textcolor(LIGHTGRAY);
				prt("Account opened with account number ", wherey()+2);
				textcolor(WHITE);
				acc.getacc();
				getch();
				break;
			}
			char ch = getch();
			if(ch=='\b'){
				cprintf("%c", '\b');
				counter--;
				continue;
			}
			if(ch>=65 && ch<=122){
				key[counter++] = ch;
				textcolor(YELLOW);
				cprintf("%c",'*');
			}
			else{
				continue;
			}
		}

		goto start;
	}

	acc_success:
	{
		long int amount;

		clrscr();
		frame();
		prt("Accessing account #", 2);
		account[inp_acc-1].getacc();
		prt("Account Holder", wherey()+1);
		gotoxy(w/2 - 8, wherey()+1);
		textcolor(WHITE);
		account[inp_acc-1].getname();
		gotoxy(w/2 - 17, wherey()+1);
		cprintf("%s", "Balance  Rs. ");
		account[inp_acc-1].getbal();

		prt("Choose desired task", wherey()+2);
		gotoxy(((w - strlen("Choose desired task"))/2+1), wherey()+1);
		cprintf("%s", "1. Deposit money");
		gotoxy(((w - strlen("Choose desired task"))/2+1), wherey()+1);
		cprintf("%s", "2. Withdraw money");
		gotoxy(((w - strlen("Choose desired task"))/2+1), wherey()+1);
		cprintf("%s", "3. Log out");

		int inp2 = getch();

		switch(inp2){
		case 49:
			prt("Enter amount", wherey()+2);
			gotoxy(w/2 - 3, wherey()+1);
			cin >> amount;	cin.ignore(100, '\n');
			account[inp_acc-1].trans(abs(amount));
			getch();
			goto acc_success;
		case 51:
			account[inp_acc-1].encrypt();
			goto start;
		case 50:
			prt("Enter amount", wherey()+2);
			gotoxy(w/2 - 3, wherey()+1);
			cin >> amount;	cin.ignore(100, '\n');
			account[inp_acc-1].trans(-abs(amount));
			getch();
			goto acc_success;
		default:
			textcolor(LIGHTRED);
			prt("Invalid choice!!", wherey()+2);
			getch();
			goto acc_success;
		}
	}

	exit:
	wrap();
	return 0;
}

void Account::Account(char * hld, char * pass){
	for(int i=0; i<30; i++){
		holder[i] = hld[i];
	}
	for(i=0; i<7; i++){
		key[i] = pass[i];
	}
	acc_num = ++max_acc;
	bal = 0;
}

void Account::trans(int a){
	if(a<0){
		if(bal<abs(a)){
			textcolor(LIGHTRED);
			prt("Withdrawl fail! Invalid amount!", wherey() + 2);
			getch();
			goto ex;
		}
	}
	bal += a;
	if(a>0){
		textcolor(LIGHTGREEN);
		prt("Deposit successful!", wherey() + 2);
	}
	else if(a<0){
		textcolor(LIGHTGREEN);
		prt("Withdrawl successful!", wherey() + 2);
	}

	prt("New balance is", wherey()+2);
	gotoxy((w/2 - 8), wherey()+1);
	cout << "Rs. ";
	//setw(9);
	cout << bal;
	ex:
}

void Account::getname(){
	cprintf("%s", holder);
}

void Account::getacc(){
	cout << acc_num;
}

void Account::getbal(){
	cout << bal;
}

int Account::login(char * a){
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
	for(int j=0; j<30; j++){
		int m = j%7;
		char d = key[m];
		holder[j] += (int) d;
		bal += (int) d;
	}
}

void init(){

	strcpy(account[0].holder, "Sankalp Gambhir");
	strcpy(account[0].key, "abcdef");
	account[0].acc_num = 1;
	account[0].bal = 10000;
	max_acc = 1;
	account[0].encrypt();
}

void wrap(){
	int hld_file, key_file, bal_file;

	if ((hld_file = open("HOLDER.TXT", O_RDONLY | O_APPEND,
					  S_IREAD | S_IWRITE)) == -1)
	{
		printf("Error opening file.\n");
	}
	if ((key_file = open("KEY.TXT", O_RDONLY | O_APPEND,
					  S_IREAD | S_IWRITE)) == -1)
	{
		printf("Error opening file.\n");
	}
	if ((bal_file = open("BAL.TXT", O_RDONLY | O_APPEND,
					  S_IREAD | S_IWRITE)) == -1)
	{
	  printf("Error opening file.\n");
	}

	write(hld_file, account[max_acc-1].holder, strlen(account[max_acc-1].holder));
	write(key_file, account[max_acc-1].key, strlen(account[max_acc-1].key));
	char buff[65];
	itoa(account[max_acc-1].bal, buff, 10);
	write(bal_file, buff, strlen(buff));

}

void prt(const char * a, int b){
	gotoxy((w-strlen(a))/2, b);
	cprintf("%s", a);
	textcolor(LIGHTGRAY);
}

void frame()
{
	gotoxy(1,1);
	for(int i = 0; i < w; i++)
	{
		cout << '*';
	}

	for(i = 2; i <= h - 2; i++)
	{
		gotoxy(1, i); cout << '*';
		gotoxy(w, i); cout << '*';
	}

	gotoxy(1, h - 1);
	for(i = 0; i < w; i++)
	{
		cout << '*';
	}
}
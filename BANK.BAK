#include <iostream.h>
#include <fstream.h>
#include <conio.h>
#include <iomanip.h>
#include <string.h>
#include <ctype.h>


class Account{
	public:
		Account(void){};            // Default constructor
		Account(char *, char *);    // New Account constructor
		Account(Account *);         // Copy constructor

		friend void init();
		void getname();             // Prints name of account holder
		void getacc();              // Prints account number
		void getbal();              // Prints balance
		void trans(int);            // Alters balance for deposit/withdrawl
		void encrypt();				// Re-encrypts data for storage
		int login(char *);          // Decrypts name and balance as per given key

	private:
		char holder[100];           // ENCRYPTED till login()
		int acc_num;                // Account number
		unsigned long int bal;      // Current account balance [ENCRYPTED]
		char key[7];                // Holds encryption key for account(basically, password)
};

int max_acc;                        // Currently opened accounts(num)

int h,w;							// Height and width of screen

int main(){

	void frame();

	struct text_info info;
	gettextinfo(&info);

	h = ( (int) info.screenheight) - 2;
	w = ( (int) info.screenwidth) - 2;

	clrscr();

	frame();


	getch();
	return 0;
}

Account::Account(char * hld, char * pass){  	// Holder name & password
	strcpy(holder, hld);
	strcpy(key, pass);
	acc_num = ++max_acc;
	bal = 0;
	encrypt();
}

ofstream& operater=(Account * a, Account * b){
	strcopy(b::holder, a::holder);
	strcopy(b::key, a::holder);
	b::bal = a::bal;
	b::acc_num = a::acc_num;
}

void Account::trans(int a){
	bal += a;
	if(a>0){
		gotoxy((w - strlen("Deposit successful!"))/2,(wherey()+2));
		textcolor(2);
		cprintf("%s","Desposit successful!");
		textcolor(7);
	}
	else if(a>0){
		gotoxy((w - strlen("Withdrawl successful!"))/2,(wherey()+2));
		textcolor(2);
		cprintf("%s","Withdrawl successful!");
		textcolor(7);
	}

	gotoxy((w - strlen("New balance is"))/2,(wherey()+2));
	cout << "New balance is";
	gotoxy((w/2 - 12), wherey()+1);
	cout << "Rs. ";
	setw(9);
	cout << bal;
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
	int match_counter = 0;
	int i,j,d;
	for(i=0; i<7; i++){
		if(a[i] == key[i]){
			match_counter++;
		}
	}
	if(match_counter == 7){
		goto decrypt;
	}
	else{
		goto fail;
	}

	decrypt:
			for(j=0; j<100; j++){
				d = (int) key[j%7];
				holder[j] -= d;
				bal += d;
			}
			return 0;

	fail:
			return 1;
}

void Account::encrypt(){
	for(int j=0; j<100; j++){
		int d = (int) key[j%7];
		holder[j] += d;
		bal -= d;
	}
}

void init(){

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
		gotoxy(width, i); cout << '*';
	}

	gotoxy(1, h - 1);
	for(i = 0; i < w; i++)
	{
		cout << '*';
	}
}
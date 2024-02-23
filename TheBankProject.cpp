#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
const string ClientsFileName = "Clientss.txt";
const string UsersFileName = "Users.txt";
void ShowMainMenue();
void ShowTransActionsScreen();
void ManageUsersScreen();
stUser CurrentUser;

enum enMainMenuOptions {Show = 1,Add = 2, Delete = 3,Update = 4,Find = 5,TransActions = 6,ManageUsers = 7,LogOut = 8};

enum enMangeUsersOptions {ListUsers = 1,AddUsers = 2,DeleteUsers = 3,UpdateUsers = 4,FindUsers = 5,MainMenu = 6};

enum enPermitions {eAll = -1,pListClients = 1,pAddNewClient = 2,pDeleteClient = 4,pUpdateClient = 8, pFindClient = 16,pTransActions = 32,pMangeUsers = 64 };

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

struct stUser
{
	string UserName;
	string Password;
	int Permition;
	bool MarkForDelete = false;
};

vector <string> SplitString(string St,string Dialm = "#//#")
{
	vector <string> vString;
	short Pos = 0;
	string SWord;

	while ((Pos = St.find(Dialm)) != string::npos)
	{
		SWord = St.substr(0,Pos);

		if (SWord != "")
		{
			vString.push_back(SWord);
		}

		St.erase(0,Pos + Dialm.length());
	}
	if (St != "")
	{
		vString.push_back(St);
	}

	return vString;
}

stClient ConvertLineToRecord(string Line,string Seprator = "#//#")
{
	stClient Client;
	vector <string> vLine = SplitString(Line);

	Client.AccountNumber = vLine[0];
	Client.PinCode = vLine[1];
	Client.Name = vLine[2];
	Client.Phone = vLine[3];
	Client.AccountBalance = stod(vLine[4]);

	return Client;
}

string ConvertRecordToLine(stClient Client,string Seprator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seprator;
	stClientRecord += Client.PinCode + Seprator;
	stClientRecord += Client.Name + Seprator;
	stClientRecord += Client.Phone + Seprator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

vector <stClient> LoadDataFromFile(string FileName)
{
	vector <stClient> vClients;

	fstream MyFile;
	MyFile.open(FileName,ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile,Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";

	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code     : " << Client.PinCode;
	cout << "\nName         : " << Client.Name;
	cout << "\nPhone        : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber,vector <stClient> vClients,stClient &Client)
{

	for (stClient C : vClients)
	{
		if (AccountNumber == C.AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool MarkClientFoDeleteByAccountNumber(string AccountNumber,vector <stClient> &vClients)
{

	for (stClient &C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector <stClient> SaveClientsDataToFile(string FileName,vector <stClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName,ios::out);

	string DataLine;

	if (MyFile.is_open())
	{
		for (stClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber,vector<stClient>& vClients)
{
	stClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber,vClients,Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre You sure you want delete this client? y/n ? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			MarkClientFoDeleteByAccountNumber(AccountNumber,vClients);
			SaveClientsDataToFile(ClientsFileName,vClients);

			vClients = LoadDataFromFile(ClientsFileName);

			cout << "\n\nClient Delted Succesfully.";
			return true;
		}


	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}

}

bool ClientExistsByAccountNumber(string AccountNumber,string FileName)
{
	vector <stClient> vClients;
	fstream MyFile;
	MyFile.open(FileName,ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

stClient ReadNewClient()
{
	stClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws,Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber,ClientsFileName))
	{
		cout << "\nClient With [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws,Client.AccountNumber);
	} 

	cout << "Enter PinCode? ";
	getline(cin,Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

void AddDataLineToFile(string FileName,string Line)
{
	fstream MyFile;
	MyFile.open(FileName,ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << Line << '\n';
		MyFile.close();
	}


}

void AddNewClient()
{
	stClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName,ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMore = 'Y';
	do
	{
		cout << "Adding New Client:\n\n";

		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more Client? Y/N ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 7]? ";
	short Option;
	cin >> Option;
	return Option;
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin,Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber,vector <stClient>&vClients)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber,vClients,Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want update this client? y/n ? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (stClient &C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName,vClients);

			cout << "\n\nClient Updated Successfully.";
			return true;
		}

	}
	else 
	{
	cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
	return false;
	}
}

void PrintClientRecordLine(stClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

enum enTransActionsMenu {Deposit = 1,WithDraw = 2,TotalBalances = 3,BackToMainMenue = 4};

void PrintClientRecordBalanceLine(stClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalances()
{
	vector <stClient> vClients = LoadDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name"; 
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	double TotalBalances = 0;

	if (vClients.size() == 0)
		cout << "\t\t\tNo Clients Available In the System!";
	else
		for (stClient Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			TotalBalances += Client.AccountBalance;

			cout << endl;
		}

	cout << "\n_________________________________________";
	cout << "__________________________________________\n" << endl;
	cout << "\t\t\t\t\t Total Balances = " << TotalBalances;
}

void GoBackToTransActionsMenue()
{
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	ShowTransActionsScreen();
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber,double Amount,vector <stClient> &vClients)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want perform this transaction? y/n? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (stClient &C: vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName,vClients);
				cout << "\n\nDone Successfully. Now Balance is: " << C.AccountBalance;

				return true;
			}
		}
		return false;
	}
}

void ShowDepositScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n---------------------------------------\n";

	stClient Client;

	vector <stClient> vClients = LoadDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;


	DepositBalanceToClientByAccountNumber(AccountNumber,Amount,vClients);
}

void ShowWithDrawScreen()
{
	cout << "\n----------------------------------\n";
	cout << "\tWithDrawScreen";
	cout << "\n----------------------------------\n";

	stClient Client;

	vector <stClient> vClients = LoadDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	
	while (!FindClientByAccountNumber(AccountNumber,vClients,Client))
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist.";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter Withdraw amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, tou can withdraw up to : " << Client.AccountBalance << '\n';
		cout << "Please enter another amount? ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber,Amount * -1,vClients);

}

short ReadTransAction()
{
	short i;
	cout << "Choose What Do You want to do? [1 to 4]? ";
	cin >> i;
	return i;
}

void PerformTransActions(enTransActionsMenu TransActionsMenue)
{
	switch (TransActionsMenue)
	{
	case  enTransActionsMenu::Deposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransActionsMenue();
		break;
	case enTransActionsMenu::WithDraw:
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransActionsMenue();
		break;
	case enTransActionsMenu::TotalBalances:
		system("cls");
		ShowTotalBalances();
		GoBackToTransActionsMenue();
		break;
	case enTransActionsMenu::BackToMainMenue:
		ShowMainMenue();
	}
}

void ShowTransActionsScreen()
{
	system("cls");
	cout << "===================================================\n";
	cout << "\t\tTransActions Menue Screen\n";
	cout << "===================================================\n";

	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";

	cout << "===================================================\n";

	PerformTransActions((enTransActionsMenu)ReadTransAction());
}

void ShowDeleteClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n------------------------------------\n";

	vector <stClient> vClients = LoadDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(AccountNumber,vClients);
}

void ShowAddNewClientsScreen()
{
	cout << "\n----------------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n----------------------------------------\n";

	AddNewClients();
}

void ShowUpdateClientScreen()
{
	cout << "\n-------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-------------------------------\n";

	vector <stClient> vClients = LoadDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
	cout << "\n----------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n----------------------------\n";

	vector <stClient> vClients = LoadDataFromFile(ClientsFileName);
	stClient Client;
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
}

void ShowAllClientsScreen()
{
	vector <stClient> vClients = LoadDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else

		for (stClient C : vClients)
		{
			PrintClientRecordLine(C);
			cout << endl;
		}
	cout << "\n________________________________________________________";
	cout << "__________________________________________________\n" << endl;
}

void ShowEndScreen()
{
	cout << "\n-------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-------------------------\n";
}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to main menue...";
	system("pause>0");
	ShowMainMenue();
}

string ConvertRecordToLine(stUser User, string Dialm = "#//#")
{
	string st;

	st = User.UserName + Dialm;
	st += User.Password + Dialm;
	st += to_string(User.Permition);

	return st;
}

stUser ConvertLineToUserRecord(string Line, string Dialm = "#//#")
{
	stUser User;
	vector <string> vString = SplitString(Line,Dialm);

	User.UserName = vString[0];
	User.Password = vString[1];
	User.Permition = stoi(vString[2]);

	return User;
}

bool LoadUserInfo(string FileName, string UserName, string Pass)
{

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		while (getline(MyFile, Line))
		{
			stUser User = ConvertLineToUserRecord(Line);

			if (User.UserName == UserName && User.Password == Pass)
			{
				MyFile.close();
				return true;
			}

		}
		MyFile.close();
	}

	return false;
}

vector <stUser>UsersVector(string FileName)
{
	vector <stUser> VUsers;

	fstream MyFile;
	MyFile.open(FileName,ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stUser User;
		while (getline(MyFile,Line))
		{
			User = ConvertLineToUserRecord(Line);

			VUsers.push_back(User);
		}
		MyFile.close();
	}
	return VUsers;
}

void PrintUserRecordLine(stUser User)
{
	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(40) << left << User.Permition;
}

void ShowUsersListScreen()
{
	vector <stUser> vUsers = UsersVector(UsersFileName);
	
	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(10) << "Password"; 
	cout << "| " << left << setw(40) << "Permissions";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vUsers.size() == 0)
		cout << "\t\t\t\tNo Users Available In the System!";
	else
	{
		for (stUser User : vUsers)
		{
			PrintUserRecordLine(User);
			cout << '\n';
		}
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "Press Any Key To Back to Mnage Users Screen....\n";
	system("pause>0");
}

bool UserExistsByUserName(string FileName,string UserName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stUser user;

		while (getline(MyFile, Line))
		{
			user = ConvertLineToUserRecord(Line);
			if (user.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
			
		}
		MyFile.close();
	}
	return false;
}

int ReadPermissionsToSet()
{
	int Permissions = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access? y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
		return -1;

	cout << "\nDo you want to give access to : \n";

	cout << "\nShow Client List? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enPermitions::pListClients;

	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
		Permissions += enPermitions::pAddNewClient;
	
	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enPermitions::pDeleteClient;

	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enPermitions::pUpdateClient;

	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enPermitions::pFindClient;

	cout << "\nTransactions? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enPermitions::pTransActions;
	
	cout << "\nMange Users? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enPermitions::pMangeUsers;
}

stUser ReadUserToAdd()
{
	stUser User;

	cout << "Enter Username? ";
	getline(cin >> ws,User.UserName);

	while (UserExistsByUserName(UsersFileName, User.UserName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password? ";
	getline(cin, User.Password);

	User.Permition = ReadPermissionsToSet();

	return User;
}

void AddUserToFile(string FileName)
{
	stUser User = ReadUserToAdd();
	AddDataLineToFile(FileName,ConvertRecordToLine(User));
}

void AddMoreUsers()
{
	char Answer = 'n';
	do
	{
		AddUserToFile(UsersFileName);

		cout << "\nDo U Want Add User More? y/n? ";
		cin >> Answer;

	} while (Answer == 'y' || Answer == 'Y');
	cout << "\n Click any button to back to mange users screen....";
}

void ShowAddNewUserScreen()
{

	cout << "\n-------------------------------------\n";
	cout << "\tAdd New User Screen";
	cout << "\n-------------------------------------\n";

	cout << "Adding New User:\n\n";
	AddMoreUsers();
	system("pause>0");
}

short ReadMangeUsersOption()
{
	

	cout << "Choose What do you want to do? [1 to 6]? ";
	short Num;
	cin >> Num;
	return Num;
}

void GoBackToMangeScreen()
{
	system("cls");
	ManageUsersScreen();
}

void PerformMangeUsers(enMangeUsersOptions MangeUsers)
{
	switch (MangeUsers)
	{
	case enMangeUsersOptions::ListUsers:
		system("cls");
		ShowUsersListScreen();
		GoBackToMangeScreen();
		break;
	case enMangeUsersOptions::AddUsers:
		system("cls");
		ShowAddNewUserScreen();
		GoBackToMangeScreen();
		break;
	}

}



void ManageUsersScreen()
{
	system("cls");
	cout << "=============================================\n";
	cout << "\t\tMange Users Menue Screen\n";
	cout << "=============================================\n";

	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";

	cout << "=============================================\n";
	PerformMangeUsers((enMangeUsersOptions)ReadMangeUsersOption());

}

void PerformMainMenueOption(enMainMenuOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
	case enMainMenuOptions::Show:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenuOptions::Add:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenuOptions::Delete:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenuOptions::Update:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenuOptions::Find:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenuOptions::TransActions:
		system("cls");
		ShowTransActionsScreen();
		break;
	case enMainMenuOptions::ManageUsers:
		system("cls");
		ManageUsersScreen();
		break;
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "====================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "====================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] TrasnActions Minue.\n";
	cout << "\t[7] Mange Users.\n";
	cout << "\t[8] Log Out.\n";
	cout << "====================================\n";
	PerformMainMenueOption((enMainMenuOptions)ReadMainMenueOption());
}

void LogIn()
{
	bool LogInFailed = false;

	string UserName,Password;

	do
	{
		system("cls");
		cout << "\n--------------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n--------------------------------------\n";

		if (LogInFailed)
		{
			cout << "Invlaid Username/Password!\n";
		}

		cout << "Enter User name? ";
		cin >> UserName;

		cout << "Enter Password? ";
		cin >> Password;

		LogInFailed = !LoadUserInfo(UsersFileName,UserName,Password);

	} while (LogInFailed);

	ShowMainMenue();

}

int main()
{
	LogIn();


	system("pause>0");
	return 0;
}

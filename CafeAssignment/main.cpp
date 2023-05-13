#include "pch.h"

using namespace std;

void Cafe::InitializeCafe()
{
#if defined DEBUG
	cout << "Initializing cafe..." << endl;
#endif

	ifstream ItemList("items_for_sale.txt");
	bool ReadingName = true;
	string StringInput;
	string DoubleInput;

	if (ItemList.is_open())
	{
		while (ItemList)
		{
			if (ReadingName)
			{
				getline(ItemList, StringInput);
			}
			else
			{
				getline(ItemList, DoubleInput);
				this->Items.push_back(CafeItem(StringInput, stod(DoubleInput)));

#if defined DEBUG
				cout << "Added a " << this->Items.back().Name << " to the item list, which will be sold for $" << this->Items.back().Cost << "." << endl;
#endif
			}

			ReadingName = !ReadingName;
		}
	}

	ReadingName = true;
	ifstream CurrencyList("valid_currency.txt");

	if (CurrencyList.is_open())
	{
		while (CurrencyList)
		{
			if (ReadingName)
			{
				getline(CurrencyList, StringInput);
			}
			else
			{
				getline(CurrencyList, DoubleInput);
				this->CashRegister.push_back(Currency(StringInput, stod(DoubleInput)));

#if defined DEBUG
				cout << "Added a " << this->CashRegister.back().Name << " to the cash register, which is worth $" << this->CashRegister.back().Value << "." << endl;
#endif
			}

			ReadingName = !ReadingName;
		}
	}
}

void Cafe::Welcome()
{
	ClearScreen();

	cout << "Welcome to Cafe++! We only accept full dollar payments. What would you like to order?" << endl << endl;

	for (int i = 0; i < Items.size(); i++)
	{
		cout << i + 1 << ". " << Items[i].Name << " ($" << fixed << showpoint << setprecision(2) << Items[i].Cost << ")" << endl;
	}

	cout << Items.size() + 1 << ". Leave";

	TakeOrder();
}

void Cafe::TakeOrder()
{
	cout << endl;

	string input;
	int order;
	cin >> input;

	if (!IsValidInput(input))
	{
		order = -1;
	}
	else
	{
		order = stoi(input);
		order--;
	}

	if (order == Items.size())
	{
		cout << "Have a good day." << endl;
		exit(0);
	}
	else if (order < 0 || order > Items.size())
	{
		cout << "Sorry, we do not have that item. Please try again." << endl << endl;
		system("pause");
		Welcome();
	}
	else
	{
		double cost = Items[order].Cost;
		double offered = 0;
		cout << "The " << Items[order].Name << " will cost you $" << fixed << showpoint << setprecision(2) << cost << ". Please render payment: ";
		offered = GetPayment(cost);

		while (offered < cost)
		{
			cout << "That won't be enough to cover the cost of your " << Items[order].Name << ". Please offer at least $" << fixed << showpoint << setprecision(2) << cost << ": ";
			offered = GetPayment(cost);
		}

		MakeChange(Items[order].Name, offered, cost);
	}
}

double Cafe::GetPayment(double cost)
{
	string input;
	double offered = cost - 1;

	cin >> input;
	if (IsValidInput(input))
	{
		offered = stod(input);
	}

	return offered;
}

void Cafe::MakeChange(string ItemName, double offered, double cost)
{
	ClearScreen();

	cout << "You purchased the " << ItemName << ", with a retail price of $" << fixed << showpoint << setprecision(2) << cost << ", for $" << offered << "." << endl << endl;

	double difference = offered - cost;
	if (difference > 0)
	{
		cout << "Cafe++ owes you $" << fixed << showpoint << setprecision(2) << difference << " in change, which is given to you in the form of:" << endl << endl;

		vector<Currency> Returned = vector<Currency>();
		while (difference > 0)
		{
			Currency Largest = GetLargestCurrency(difference);

			int slot = FindSlotOfCurrency(Returned, Largest);
			if (slot == -1)
			{
				Returned.push_back(Largest);
				slot = Returned.size() - 1;
			}

			Returned[slot].Quantity++;
			difference -= Returned[slot].Value;
		}

		for (int i = 0; i < Returned.size(); i++)
		{
			cout << Returned[i].Quantity << "x " << Returned[i].Name << "(s), totaling $" << fixed << showpoint << setprecision(2) << Returned[i].Quantity * Returned[i].Value << "." << endl;
		}
	}
	else
	{
		cout << "You did not receive any change." << endl;
	}

	cout << endl;
	system("pause");
	Welcome();
}

Currency Cafe::GetLargestCurrency(double max)
{
	Currency Largest = Currency("null", 0.00);

	for (int i = 0; i < CashRegister.size(); i++)
	{
		if (CashRegister[i].Value > Largest.Value && CashRegister[i].Value <= max)
		{
			Largest = CashRegister[i];
		}
	}

	return Largest;
}

int Cafe::FindSlotOfCurrency(vector<Currency> Returned, Currency check)
{
	int ExistingSlot = -1;

	for (int i = 0; i < Returned.size() && ExistingSlot == -1; i++)
	{
		if (Returned[i].Name == check.Name)
		{
			ExistingSlot = i;
		}
	}

	return ExistingSlot;
}

void Cafe::ClearScreen()
{
	cout << "\x1B[2J\x1B[0;0f";
}

bool Cafe::IsValidInput(string input)
{
	for (char character : input)
	{
		if (isdigit(character) == 0)
		{
			return false;
		}
	}

	return true;
}

int main()
{
	Cafe cafe = Cafe();
}

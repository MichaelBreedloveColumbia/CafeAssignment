#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>

//#define DEBUG

using namespace std;

class Currency
{
	public:string Name;
	public:double Value;
	public:int Quantity = 0;

	public:
	Currency(string name, double value)
	{
		Name = name;
		Value = value;
	}
};

class CafeItem
{
	public:string Name;
	public:double Cost;

	public:
	CafeItem(string name, double cost)
	{
		Name = name;
		Cost = cost;
	}
};

class Cafe
{
	vector<CafeItem> Items = vector<CafeItem>();
	vector<Currency> CashRegister = vector<Currency>();

	public:
	Cafe()
	{
		InitializeCafe();
		Welcome();
	}

	void InitializeCafe();
	void Welcome();
	void TakeOrder();
	void MakeChange(string ItemName, double offered, double cost);
	Currency GetLargestCurrency(double max);
	int FindSlotOfCurrency(vector<Currency> Returned, Currency check);
	void ClearScreen();
	bool IsValidInput(string input);
	double GetPayment(double cost);
};
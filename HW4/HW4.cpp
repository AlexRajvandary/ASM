#include <iostream>
#include <vector>
#include <string>
#include <omp.h>


using namespace std;

void Enter(vector<double>& childrenProp, vector<double>& childrenSum, double& totalSum)
{

	string line;
	do {
		for (int i = 0; i < 1; i++) {
			cout << "Введите общую сумму денег (больше нуля)" << endl;
			cin >> line;

			try {
				totalSum = stoi(line);
			}
			catch (const exception&) {
				i--;
			}
		}
	} while (totalSum <= 0);

	double currentFraction = 0;

	for (int i = 0; i < 8; i++) {

		cout << "Введите долю сына №" << i + 1 << "  (Число от 0 до 1)" << endl;

		cin >> line;

		try {
			currentFraction = stod(line);
		}
		catch (const exception&) {
			i--;
		}
		if (currentFraction > 1 || currentFraction < 0) {
			cout << "Доля не может быть больше 1 или меньше 0!" << endl;
			i--;
			currentFraction = 0;
		}
		else {
			childrenProp.push_back(currentFraction);
			currentFraction = 0;
		}
	}

	int sum = 0;

	for (int i = 0; i < 8; i++) {

		cout << "Введиет сумму, которую получил сын №" << i + 1 << endl;

		cin >> line;

		try {

			sum = stod(line);
		}
		catch (const exception&) {
			i--;
		}
		if (sum < 0) {
			cout << "Сумма должна быть не меньше нуля!" << endl;
			i--;
			sum = 0;
		}
		else {
			childrenSum.push_back(sum);
			sum = 0;
		}
	}
}

int main()
{
	setlocale(LC_CTYPE, "rus");

	//Доли правнуков
	vector<double> GreatGrandSonsFractions;
	//Деньги, которые получили правнуки
	vector<double> GreatGrandSonsSums;

	bool check = true;

	//Все доли
	double AllFractions = 0;
	//Общая сумма, полученных денег
	double total = 0;

	//Пользовательский ввод
	Enter(GreatGrandSonsFractions, GreatGrandSonsSums, total);

#pragma omp parallel for 
	for (int i = 0; i < 8; i++)
	{
		//Суммируем все доли 
		AllFractions += GreatGrandSonsFractions[i];
	}

	if (AllFractions > 1) {

		//Если сумма всех долей больше единицы, была допущена ошибка
		cout << "Сумма всех долей не может быть больше единицы (Правнуки не могут получить больше денег, чем было всего у Дона Энрике!)" << endl;
		return 0;
	}



#pragma omp parallel for

	for (int j = 0; j < 8; j++)
	{
		if (GreatGrandSonsFractions[j] * GreatGrandSonsSums[j] != total) {
			cout << "Адвокат ошибся" << endl;
			cout << "Правнук № " << j + 1 << " должен был получить " << GreatGrandSonsFractions[j] * total << ", а получил: " << GreatGrandSonsSums[j] << endl;	
		}
		else {
			cout << "Правнук № " << j + 1 << " получил правильную сумму!" << endl;
		}
	}



	return 0;
}

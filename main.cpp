#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <algorithm>
/*Задача полягає у знаходженні найдешевшого та найшвидшого шляхів представляє з себе Задачу Комівояжера*/
using namespace std;

int END[5][2];

//Функція вирішення Здачі Комівояжера
double travllingSalesmanProblem(double graph[][6], int s)
{
	// Зберігання усіх вершин графу
	vector<int> vertex;
	vector<int> WEND(6);
	for (int i = 0; i < 6; i++)
		if (i != s)
			vertex.push_back(i);
	
	// Зберігання мінімальної ваги Циклу
	double min_path = INT_MAX;
	do {
		
		// Зберігання поточної ваги
		double current_pathweight = 0;

		// знаходження ваги для конурктного перебору
		int k = s;
		for (int i = 0; i < vertex.size(); i++) {
			current_pathweight += graph[k][vertex[i]];
			WEND[i] = k;
			k = vertex[i];
		}
		//current_pathweight += graph[(int)k][s]; // Повертання з останньої станціі на першу

		// Оновлюємо значення мінімуму та зберігаємо індекси у графі
		if (min_path > current_pathweight) {
			min_path = current_pathweight;
			for (int i = 0; i < vertex.size(); i++) {
				END[i][1] = vertex[i];
				END[i][0] = WEND[i];
			}
			
		}
		

		// Зміна порядку вершин
	} while (next_permutation(vertex.begin(), vertex.end()));
	
	return min_path;
}


// Переведення данних часу з таблиці у секунди
double stringTime(string time) {
	double sec = 0;
	sec = ((time[0] - 48) * 10 + (time[1] - 48)) * 3600 + ((time[3] - 48) * 10 + (time[4] - 48)) * 60;
	return sec;
}


// Структура для зберігання данних файлу
struct ticket
{
	int Number = 0;
	int depSta = 0; 
	int arrSta = 0;
	double cost = 0;
	string depTime = "";
	string arrTime = "";
	double sec;
	bool used = false;
	
};

int main()
{   
	SetConsoleOutputCP(1251);

	string fname = "test_task_data.csv";
	string line, word;
	ifstream file(fname, ios::in);
	int strCount = 0;
	bool choice = 0;

	// Відкриття файлу, зчитування та збереження данних у масив структур
	if (file.is_open())
	{
		while (!file.eof()) {
			getline(file, word);
			strCount++;
		}
	}
	else
	{
		cout << "Could not open the file\n";
	}
	file.close();


	file.open(fname);
	ticket *tick = new ticket[strCount];
	int i = 0;
	if (file.is_open())
	{
			
		while (!file.eof()) {
		
			getline(file, word, ';');
			for (int j = 0; j < word.size(); j++) {
				tick[i].Number = tick[i].Number * 10 + word[j] - 48;
			}
			
			getline(file, word, ';');
			
			for (int j = 0; j < word.size(); j++) {
				tick[i].depSta = tick[i].depSta * 10 + word[j] - 48;
			}
			getline(file, word, ';');
			for (int j = 0; j < word.size(); j++) {
				tick[i].arrSta = tick[i].arrSta * 10 + word[j] - 48;
			}
			getline(file, word, ';');
			
			tick[i].cost = atof(word.c_str());
			
			getline(file, word, ';');
			tick[i].depTime = word;
			getline(file, word);
			tick[i].arrTime = word;

			i++;
		}
	}
	else {
		cout << "Could not open the file\n";
	}


	cout << "Оберіть Крітерій для пошуку шляху: 1 - Ціна, 0 - Час: ";
	cin >> choice;

	
	int base[6] = { 1909, 1929, 1902, 1937, 1981, 1921}; // Перераховані вершини отриманого графу
	vector<vector <int>> answers(7, vector<int>(7));

	// Ціна
	if (choice) {

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				answers[i][j] = 0;
			}
		}

		// Знаходження найменьших значеннь квитків з поміж однаковіх маршрутів
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				double min = 3.402823466e+38;
				for (int f = 0; f < strCount; f++) {
					if (base[i] == tick[f].depSta && base[j] == tick[f].arrSta) {
						if (tick[f].cost <= min) {
							min = tick[f].cost;
							answers[i][j] = f;

						}
					}
				}
			}
		}

		// Створення та запись цін у граф для подальшого його розвьязання
		int s = 0;
		double graph[6][6];
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				if (answers[i][j] != 0) {
					graph[i][j] = tick[answers[i][j]].cost;
				}
				else {
					graph[i][j] = INT_MAX;
				}
			}
		}

		cout << endl << endl;
		// Вивод найменьшої ціни маршруту
		cout << "Найменьша ціна маршруту по станціях: " << travllingSalesmanProblem(graph, s) << endl;

		// Вивод шляху по станціях
		cout << "Вивод шляху по станціях: " << endl;
		for (int i = 0; i < 6; i++) {
			if (i < 5) {
				cout << base[END[i][0]] << " ————> ";          
			}
			else {
				cout << base[END[i - 1][1]];
			}
		}
		cout << endl;

		cout << "Вивод шляху по квитках: " << endl;
		// Вивід Можливих квитків з однаковою ціною та маршрутом станцій
		for (int i = 0; i < 5; i++) {
			cout << "( ";
			for (int j = 0; j < strCount; j++) {
				
				if (tick[j].depSta == base[END[i][0]] && tick[j].arrSta == base[END[i][1]] && tick[j].cost == graph[END[i][0]][END[i][1]]) {
					cout << tick[j].Number << " ";
				}
			}
			if (i != 4) {
				cout << ") " << "————> ";
			}
			else {
				cout << ") ";
			}
		}
		cout << endl << endl;
	}


	// Час
	else {
		for (int i = 0; i < strCount; i++) {
			tick[i].sec = abs(stringTime(tick[i].depTime) - stringTime(tick[i].arrTime));
		}
		
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				answers[i][j] = 0;
			}
		}

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				double min = 3.402823466e+38;
				for (int f = 0; f < strCount; f++) {
					if (base[i] == tick[f].depSta && base[j] == tick[f].arrSta) {
						if (tick[f].sec <= min) {
							min = tick[f].sec;
							answers[i][j] = f;

						}
					}
				}
			}
		}
		int s = 0;
		double graph[6][6];
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				if (answers[i][j] != 0) {
					graph[i][j] = tick[answers[i][j]].sec;
				}
				else {
					graph[i][j] = INT_MAX;
				}
			}
		}


		int h, m, seconds;
		seconds = travllingSalesmanProblem(graph, s);
		h = seconds/3600;
		m = (seconds - h*3600)/ 60;
		seconds = seconds - h*3600 - m*60;

		cout << endl << endl;
		// Вивід мінімального час
		cout << "Найменьшого час маршруту потягів: " << h << "г. " << m << "хв. " << s << "сек. " << endl;	

		// Вивод шляху по станціях
		cout << "Вивод шляху по станціях: " << endl;
		for (int i = 0; i < 6; i++) {
			if (i < 5) {
				cout << base[END[i][0]] << " ————> ";          
			}
			else {
				cout << base[END[i - 1][1]];
			}
		}
		cout << endl;

		// Вивід Можливих квитків з однаковим часом та маршрутом станцій
		cout << "Вивод шляху по квитках: " << endl;
		for (int i = 0; i < 5; i++) {
			cout << "( ";
			for (int j = 0; j < strCount; j++) {
				if (tick[j].depSta == base[END[i][0]] && tick[j].arrSta == base[END[i][1]] && tick[j].sec == graph[END[i][0]][END[i][1]]) {
					cout << tick[j].Number << " ";
				}
			}
			if (i != 4) {
				cout << ") " << "————> ";
			}
			else {
				cout << ") ";
			}
		}
		cout << endl << endl;
	}
	
	return 0;
}


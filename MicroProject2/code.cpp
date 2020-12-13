
#include <iostream>
#include <mutex>
#include <atomic>
#include <vector>
#include <thread>



using namespace std;


const int  maxNumberOfPictures = 5;//Количесвто картин
const int  K = 50;//Максимальное количесвто посетителей в галлерее одновременно
const int  MaxCurrentVisitorsOfPicture = 10;//Максимальное количесвто посетителей у одной картины
const int  MAX = 50; // Максимальное количество посещений, чтобы программа остановилась

static atomic_int maxNumOfVisitor(0); // максимальный номер посетителя
static atomic_int numOfVisitors(0); // количество посетителей в галерее
static int Pictures[maxNumberOfPictures]; //Массив картин, где каждая картина хранит количесвто посетителей



mutex wmtx, pmtx;
// функция посетителя
void visitor(int num) {

	int m, numOfPicture = -1; 

	
	for (;;)
	{
		unique_lock<mutex> lock(wmtx);
		this_thread::sleep_for(chrono::milliseconds(700));
		lock.unlock();
		srand(time(0));
		int isVisitorWantsToGoHome = rand() % 2;

		// проверка количества поситетелей
		if (numOfVisitors + 1 < K ) {
			numOfVisitors++; // Добавляем нового посетителя

			
			//Проверяем не захотел ли посетитель покинуть галерею
			if (isVisitorWantsToGoHome == 0) {
				//Выбор картины
			for (;;) {
				srand(time(0));
				if (numOfPicture == -1)
					numOfPicture = rand() % maxNumberOfPictures + 1;
				else
				{
					m = rand() % maxNumberOfPictures;
					if (m != numOfPicture) {

						unique_lock<mutex> plock(pmtx);

						Pictures[numOfPicture]--;

						numOfPicture = m;

						plock.unlock();
					}
					else
					{
						continue;
					}

				}
				
			
				if (numOfPicture <= maxNumberOfPictures)
				{
					unique_lock<mutex> plock(pmtx);

					//Если у картины еще есть свободное место, то посетитель может ее посмотреть
					if (Pictures[numOfPicture] < MaxCurrentVisitorsOfPicture) {
						cout << "Посетитель " << num << " смотрит картину " << numOfPicture + 1 << endl;

						Pictures[numOfPicture]++;

						this_thread::sleep_for(chrono::milliseconds(500));

						plock.unlock();
					}
					else // Иначе посетитель ждет очереди
					{
						cout << "Посетитель " << num << " ожидает очереди к картине " << numOfPicture + 1 << endl;

						this_thread::sleep_for(chrono::milliseconds(500));

						plock.unlock();

						while (true) {
							this_thread::sleep_for(chrono::milliseconds(500));

							unique_lock<mutex> plock(pmtx);

							if (Pictures[numOfPicture] < MaxCurrentVisitorsOfPicture) // смотрим после ожидания
							{
								cout << "Посетитель " << num << " дождался очереди" << numOfPicture + 1 << endl;

								Pictures[numOfPicture]++;

								this_thread::sleep_for(chrono::milliseconds(500));

								break;
							}
							plock.unlock();
						}

					}
				}
			}
			}
			else {
				unique_lock<mutex> plock(pmtx);
				cout.width(2);

				cout << "Посетитель " << num << " решил покинуть галерею" << endl;

				cout << "Общее количество посетителей галереи за все время " << ++maxNumOfVisitor << endl;

				numOfVisitors--;

				this_thread::sleep_for(chrono::milliseconds(500));

				plock.unlock();

				return;
			}
			
		}
		else {
			unique_lock<mutex> lock(wmtx);
			cout << "Вахтер не впустил посетителя " << endl;
			lock.unlock();
		}

	}
}

//Инициализируем посетителей галереи
void Galary()
{
	int num = 0;

	vector<thread> persons;

	// Создание потоков посетителей
	for (int i = 0; i < MAX; i++) {
		thread vis(visitor, ++num);
		persons.push_back(move(vis));
	}


	for (auto& person : persons) {
		person.join();
	}
}

int main() {

	setlocale(LC_ALL, "RUS");


	for (int i = 0; i < maxNumberOfPictures; i++) {
		Pictures[i] = 0;
	}

	cout << "Максимальное кол-во посещений 50" << endl;


	thread start(Galary);
	start.join();

	cout << "Галерея закрыта!" << endl;
	return 0;
}



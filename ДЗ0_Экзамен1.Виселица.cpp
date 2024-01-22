#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;

class Game {
	class Data {
		int tryCount = 0;
		int errors = 0;
		unsigned char userChars[24] = {};
	public:
		void showUserChars() {
			cout << "\nИспользованные буквы: | ";
			for (int i = 0; i < tryCount; i++)
				cout << userChars[i] << " | ";
			cout << endl;
		}
		bool setUserChar(unsigned char c) {
			for (int i = 0; i < tryCount; i++) {
				if (c == userChars[i]) {
					cout << "Такая буква уже есть\n";
					return false;
				}
			}
			if (c < 192) {
				cout << "\n!Неверный ввод. Используйте буквы кириллицы.\n";
				return false;
			}
			userChars[tryCount] = c;
			return true;
		}
		int getErrors() { return errors; }
		int getTryCount() { return tryCount; }
		char getLastChar() { return userChars[tryCount]; }
		void increaseTryCount() { tryCount++; }
		void increaseErrors() { errors++; }
		void setStartState() {
			tryCount = 0;
			errors = 0;
			for (int i = 0; i < 24; i++)
				userChars[i] = 0;
		}
	};

	class Board {
		string boardPath = "board.txt";
		string boardBuf = "";
	public:
		string getBoardData()
		{
			ifstream iFile;
			string line = "";
			iFile.open("board.txt");
			for (int i = 0; !iFile.eof(); i++) {
				getline(iFile, line);
				boardBuf.append(line + '\n');
			}
			iFile.close();
			return boardBuf;
		}
		void printBoard(int er) {
			for (int i = er * 48; i < er * 48 + 48; i++)	//изображени виселицы содержит 48 символов (8 строчек по 6 знаков)
				cout << boardBuf[i];
		}
	};

	class Word {
		string wordsPath = "words.txt";
		string word;
		string encriptedWord;
		int wordsCount = 0;
	public:
		int getWordsCount() {
			string temp;
			ifstream iFile;
			iFile.open(wordsPath);
			for (int i = 0; !iFile.eof(); i++) {
				iFile >> temp;
				wordsCount++;
			}
			iFile.close();
			return wordsCount;
		}
		void encriptTheWord() {
			for (int i = 0; i < word.size(); i++)
				encriptedWord.append("_");
		}
		void chooseWord() {
			getWordsCount();
			int randomWordValue = rand() % wordsCount + 0;
			ifstream iFile;
			iFile.open(wordsPath);
			for (int i = 0; i <= randomWordValue; i++)
				iFile >> word;
			encriptTheWord();
			iFile.close();
		}
		string getWord() { return word; }
		string getEncriptedWord() { return encriptedWord; }
		void showEncriptedWord() { cout << "\nСлово: " << encriptedWord << endl; }
		bool searchChar(unsigned char lastChar) {
			char tempLower = (char)tolower(lastChar);
			char tempUpper = (char)toupper(lastChar);
			if (word.find(tempLower) == word.npos && word.find(tempUpper) == word.npos)	//если таких букв в слове нет, сразу выходим из метода с false
				return false;
			for (int i = 0; i < word.size(); i++) {
				if (word[i] == tempLower || word[i] == tempUpper) {
					encriptedWord[i] = lastChar;
				}
			}
			return true;
		}
		void setStartState() {
			encriptedWord = "";
		}
	};

	class Time {
		string gameTime;
		int timeStart;
		int timeEnd;
		struct tm newtime;
	public:
		int getCurTime()
		{
			time_t now = time(0);
			localtime_s(&newtime, &now);
			return  newtime.tm_min * 60 + newtime.tm_sec;
		}
		void setStartTime(int t) { timeStart = t; }
		void setEndTime(int t) { timeEnd = t; }
		int getStartTime() { return timeStart; }
		int getEndTime() { return timeEnd; }
		string getGameTime() {
			int temp = timeEnd - timeStart;
			int mins = temp / 60;
			gameTime = to_string(mins) + " : " + to_string(temp - mins * 60) + "\n";
			return gameTime;
		}
		void setStartState() {
			timeStart = 0;
			timeEnd = 0;
		}
	};
	Data* pData = new Data;
	Board* pBoard = new Board;
	Word* pWord = new Word;
	Time* pTime = new Time;
	unsigned char tempChar;
public:
	void start() {
		pTime->setStartTime(pTime->getCurTime());
		pWord->chooseWord();
		pBoard->getBoardData();
		while (!win() ^ lose()) { //пока не выйграл или не проиграл (при возврате одного true из двух - выход из цикла
			doTurn();
		}
		system("cls");
		cout << "Игра окончена!\n\n";
		pTime->setEndTime(pTime->getCurTime());
		if (win()) {
			cout << "Поздравляю с победой!\n\n";
		}
		if (lose()) {
			pBoard->printBoard(pData->getErrors());
			cout << "\nК сожалению Вы проиграли. В следующий раз повезёт больше!\n";
		}
		cout << "Загаданное слово: " << pWord->getWord() << endl;
		cout << "Количество попыток: " << pData->getTryCount() << endl;
		pData->showUserChars();
		cout << "Время игры: " << pTime->getGameTime() << endl;
		system("pause");
		pData->setStartState();
		pWord->setStartState();
		pTime->setStartState();
	}
	void doTurn() {
		/*
		-Отобразить поле! (проверить как работает новая схема)
		-Отобразить текущее состояние слова!!
		-Отобразить уже использованные буквы!!
		-Предложить ввести букву!!
		-Выполнить поиск буквы в слове!!
			-поиск буквы с проверкой на уже введённые!!
			-занесение буквы в статистику!!
		-Сообщить результат наличия буквы в слове - отобразить букву в encryptedWord!!
		-Проверка на условия поражения или победы!!
		-Если игра окончена, вывести результаты: время, слово, кол-во попыток, буквы игрока!!
		-По окончанию игры обнулить данные!!
		*/
		system("cls");
		pBoard->printBoard(pData->getErrors());
		pWord->showEncriptedWord();
		pData->showUserChars();
		do {
			cout << "\n\nВведите букву: ";
			cin >> tempChar;
		} while (!pData->setUserChar(tempChar));
		if (!pWord->searchChar(pData->getLastChar()))
			pData->increaseErrors();
		pData->increaseTryCount();
	}
	bool lose() {
		return pData->getErrors() == 7;
	}
	bool win() {
		return (pWord->getEncriptedWord().find("_") == pWord->getEncriptedWord().npos);
	}
	~Game() {
		delete pWord;
		delete pData;
		delete pBoard;
		delete pTime;
	}
};

int main()
{
	//..........................................ИГРА "ВИСЕЛИЦА".............................................

	setlocale(0, "");
	srand(time(NULL));
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Game game;
	int menuChoose = 1;
	enum menu { EXIT, PLAY };
	while (menuChoose != EXIT) {
		system("cls");
		cout << "Введите цифру, соответствующую пункту меню:\n\n"
			<< " 1. Новая игра;\n 0. Выход.\n-------------------------------------------"
			<< "\nВаш выбор: ";
		cin >> menuChoose;
		switch (menuChoose) {
		case PLAY:
			game.start();
			break;
		case EXIT:
			cout << "\nДо свидания!\n";
			break;
		}
	}
	return 0;
}
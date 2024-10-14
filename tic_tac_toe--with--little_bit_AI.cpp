#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//глобальные константы
const char X = 'X'; //крестик
const char O = 'O'; //нолик
const char EMPTY = ' '; //пустое поле
const char TIE = 'T'; //ничья 
const char NO_ONE = 'N'; //никто не победил

//прототипы
void instructions();
char askYesNo(string question);
int askNumber(string question, int high, int low = 0);
char humanPiece();
char opponent(char piece);
void displayBoard(const vector<char>& board);
char winner(const vector<char>& board);
bool isLegal(const vector<char>& board, int move);
int humanMove(const vector<char>& board, char human);
int computerMove(const vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);

int main()
{
	setlocale(LC_ALL, "Russian");

	int move;
	const int NUM_SQUARES = 9;

	//Создаем пустое поле (символьный вектор ("доска" 9 квадратов, которые разделяются пробелами))
	vector<char> board(NUM_SQUARES, EMPTY);

	//Вывод правил на экран
	instructions();

	//Определяем кто ходит первым (крестики первые)
	char human = humanPiece();
	char computer = opponent(human);
	char turn = X;

	//отображаем доску 
	displayBoard(board);

	//Пока никто не победил и не наступила ничья 
	while (winner(board) == NO_ONE)
	{
		//Если сейчас ход пользователя, то получить его ход
		if (turn == human)
		{
			move = humanMove(board, human);
			board[move] = computer;
		}
		//Иначе вычислить ход компьютера
		else
		{
			move = computerMove(board, computer);
			board[move] = human;
		}

		//Отображаем поле 
		displayBoard(board);

		//Передаем ход сопернику
		turn = opponent(turn);
	}

	//анонс победителя
	announceWinner(winner(board), computer, human);

	return 0;
}

//приветствие
void instructions()
{
	cout << "Приветстую тебя! Человек.. Это крестики-нолики против компьютера - Меня!\n";
	cout << "Обозначай свой ход числом, как на таблице ниже.\n";
	cout << "И не пытайся меня обмануть, у тебя ничего не выйдет! >:)\n\n";

	cout << " 0 | 1 | 2\n";
	cout << " ---------\n";
	cout << " 3 | 4 | 5\n";
	cout << " ---------\n";
	cout << " 6 | 7 | 8\n\n";

	cout << "Удачи, она тебе понадобится.\n\n";
}

//да или нет(для функции humanPiece())
char askYesNo(string question)
{
	char responce;
	
	do
	{
		cout << question << "(y/n)";
		cin >> responce;
	} while (responce != 'y' && responce != 'n');

	return responce;
}

//принимает макс. и мин. числа, возращает число из этого диап.(нужно для след. функций)
int askNumber(string question, int hight, int low)
{
	int number;

	do
	{
		cout << question << " (" << low << " - " << hight << "): ";
		cin >> number;
	} while (number > hight || number < low);

	return number;
}

//спрашивает пользователя, хочет ли он ходить первым
char humanPiece()
{
	char first = askYesNo("Ты хочешь ходить первым?");
	if (first == 'y')
	{
		cout<<"\nДам тебе фору, делай первый шаг.\n";
		return X;
	}
	else
	{
		cout << "\nНу.. как хочешь. Ты слишком самоуверенный!\n";
		return O;
	}
}

//определяет фигуру пользователя и возвращает не используемую
char opponent(char piece)
{
	if (piece == X)
		return O;
	else
		return X;
}

//отображение игрового поля 
void displayBoard(const vector<char>& board)
{
	cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8]<<"\n\n";
}

//получает игровое поле и возвращает победителя
char winner(const vector<char>& board)
{
	//выйгрышные ряды
	const int WINNING_ROWS[8][3] =
	{
		{0, 1, 2},	// верхняя строчка
		{3, 4, 5},	// средняя строчка
		{6, 7, 8},	// нижняя строчка

		{0, 3, 6},	// левый столбец
		{1, 4, 7},	// средний столбец
		{2, 5, 8},	// правый столбец

		{0, 4, 8},	// диагональ слева на право
		{2, 4, 6}	// диагональ справа на лево
	};

	//проверка, не победил ли уже кто-то
	const int TOTAL_ROWS = 8;

	for (int row = 0; row < TOTAL_ROWS; ++row)
	{
		if ((board[WINNING_ROWS[row][0]] != EMPTY) &&
			(board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
			(board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
		{
			return board[WINNING_ROWS[row][0]];
		}
	}

	//если нет победителя, проверяем на ничью
	//проверяем остались ли пустые клетки
	if (count(board.begin(), board.end(), EMPTY) == 0)
		return TIE;

	//если нет победителя, и нет ничьей, то продолжаем игру
	return NO_ONE;
}

//получает поле и проверяет правильно ли сделан ход(нужно для след. функций)
bool isLegal(const vector<char>& board, int move)
{
	return (board[move] == EMPTY);
}

//твой ход
//получает поле и фигуру, возвращает номер клетки хода
int humanMove(const vector<char>& board, char human)
{
	int move = askNumber("Куда будешь ходить? ", (board.size() - 1));

	while (!isLegal(board, move))
	{
		cout << "\nЧел ты... Занято!\n";
		move = askNumber("Куда будешь ходить? ", (board.size() - 1));
	}
	cout << "Отлично.\n";
	return move;
}

//ход компьютера
//получает поле и фигуру, возвращает номер клетки хода
//тут передача поля по значению, т.к. далее нужно будет работать и изменять копию поля
//работая с копией, исходный вектор останентся без изменений 
int computerMove(vector<char> board, char computer)
{
	//реализуем немного искусnвенного интелекта
	/*
		Шаг 1
		Если у компьютера есть возможность сделать ход,
		который принесет ему победу - сделать этот ход.
	*/
	unsigned move = 0;
	bool found = false;

	while (!found && move < board.size())
	{
		if (isLegal(board, move))
		{
			board[move] = computer;
			found = winner(board) == computer;
			board[move] = EMPTY;
		}

		if (!found)
			++move;
	}

	/*
		Шаг 2
		Иначе, если человек может победить 
		следующим ходом, блокировать этот ход.
	*/
	//ставим фигуру пользователя и проверяем приводит ли его ход к победе
	//затем отменяем ход пользователя, чтобы клетка стала пустой
	//если ход пользователя не приводит к победе, то ищем дальше
	//если приводит, то нашли клетку, в которую должен сходить компьютер
	if (!found)
	{
		move = 0;
		char human = opponent(computer);

		while (!found && move < board.size())
		{
			if (isLegal(board, move))
			{
				board[move] = human;
				found = winner(board) == human;
				board[move] = EMPTY;
			}

			if (!found)
				++move;
		}
	}

	//если пока не найден выйгрышный ход, занимаем оптимальную позицию
	/*
		Шаг 3
		Иначе занять оптимальную позцию из оставшихся клеток.
		Самая лучшая клетка в центре, менее ценны угловые, 
		остальные клетки не очень ценятся 
	*/
	if (!found)
	{
		const int BEST_MOVES[] = { 4, 0, 2, 6, 8, 1, 3, 5, 7 };
		move = 0;
		unsigned i = 0;

		while (!found && i < board.size())
		{
			move = BEST_MOVES[i];

			if (isLegal(board, move))
				found = true;

			++i;
		}
	}

	//объявление хода компьютера и возврат выбранной клетки
	cout << "...\nПусть будет " << move << endl;
	return move;
}

//Получает победителя, фигуру, которой играл комьютер и пользователь
void announceWinner(char winner, char computer, char human)
{
	if (winner == computer)
	{
		cout << winner << "'s победил!1\n";
		cout << "Не может быть... Требую реванш!1\n";
	}

	else if (winner == human)
	{
		cout << winner << "'s победил!1\n";
		cout << "Ты и вправду думал что победишь? Не смеши...";
	}

	else
	{
		cout << "Ничья...\nНеплохо, может еще разок?";
	}
}

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

const int WORDS_COUNT = 10;

string easyWords[WORDS_COUNT] = {
    "кіт", "дім", "сир", "ліс", "дах",
    "ніж", "рак", "дуб", "жук", "м'яч"};
string easyHints[WORDS_COUNT] = {
    "Хатня тварина", "Там, де живуть", "Миші це полюбляють", "Там багато дерев", "Верхня частина будинку",
    "Ним ріжуть хліб", "Тварина з клешнями", "Велике міцне дерево", "Маленька комаха", "Ним грають у футбол"
};


string medWords[WORDS_COUNT] = {
    "собака", "дерево", "космос", "яблуко", "машина",
    "гітара", "квітка", "дракон", "золото", "колесо"};
string medHints[WORDS_COUNT] = {
    "Друг людини", "Росте на вулиці", "Там знаходяться зорі", "Фрукт, що впав на Ньютона", "Транспортний засіб",
    "Музичний інструмент з 6 струнами", "Росте в саду і гарно пахне", "Міфічна істота, що дихає вогнем", "Дорогоцінний жовтий метал", "Кругла деталь автомобіля"
};


string hardWords[WORDS_COUNT] = {
    "програмування", "препроцесор", "компілятор", "алгоритм", "архітектура",
    "бібліотека", "шифрування", "оптимізація", "навігація", "ідентифікатор"};
string hardHints[WORDS_COUNT] = {
    "Процес написання коду", "Обробляє макроси до компіляції", "Перекладає код для комп'ютера", "Послідовність дій для розв'язання задачі", "Побудова або структура системи",
    "Набір готових функцій у коді", "Захист даних від прочитання", "Покращення швидкодії програми", "Визначення місцезнаходження", "Унікальне ім'я змінної"
};

void DrawHangman(int mistakes) {

    cout << "\n  +---+\n";
    cout << "  |   |\n";

    if (mistakes >= 1) cout << "  O   |\n";
    else cout << "      |\n";

    if (mistakes == 2) cout << "  |   |\n";
    else if (mistakes == 3) cout << " /|   |\n";
    else if (mistakes >= 4) cout << " /|\\  |\n";
    else cout << "      |\n";

    if (mistakes == 5) cout << " /    |\n";
    else if (mistakes >= 6) cout << " / \\  |\n";
    else cout << "      |\n";

    cout << "      |\n";
    cout << "=========\n\n";
}


void SaveResult(string mode, string word, bool isWon, int mistakes) {

    FILE* out;

    if (fopen_s(&out, "game_log.txt", "a") == 0) {
        fprintf(out, "Режим: %s\n", mode.c_str());
        fprintf(out, "Слово: %s\n", word.c_str());

        if (isWon) {
            fprintf(out, "Результат: Перемога\n");
        }
        else {
            fprintf(out, "Результат: Поразка\n");
        }

        fprintf(out, "Кількість помилок: %d/6\n", mistakes);
        fprintf(out, "-----------------------------------\n");

        fclose(out);
    }
    else {
        cout << "Помилка! Не вдалося відкрити файл для збереження результатів.\n";
    }
}


void PlayRound(string word, string hint, string modeName, int& coins) {

    int mistakes = 0;
    string guessed= "";
    for (int i = 0; i < word.length(); i++) {
        guessed += "_";
    }

    string usedLetters = "";
    bool gameWon = false;

    string alertMessage = "";

    while (mistakes < 6) {
        system("cls");

        if (alertMessage != "") {
            cout << ">>> УВАГА: " << alertMessage << " <<<\n";
            alertMessage = "";
        }

        cout << "    ГРА ПОЧАЛАСЯ    \n";
        cout << "Ваш баланс: " << coins << " монет\n";
        DrawHangman(mistakes);

        cout << "Підказка: " << hint << "\n";
        cout << "Слово: ";
        for (int i = 0; i < guessed.length(); i++) {
            cout << guessed[i] << " ";
        }

        cout << "\n\nВикористані літери: " << usedLetters << "\n";

        if (guessed == word) {
            gameWon = true;
            break;
        }

        cout << "\nВведіть маленьку літеру (або '1', щоб купити підказку за 15 монет): ";
        char choice;
        cin >> choice;

        if (choice == '1') {
            if (coins >= 15) {
                coins -= 15;
                int randIndex;
                do {
                    randIndex = rand() % word.length();
                } while (guessed[randIndex] != '_');

                guessed[randIndex] = word[randIndex];
            }
            else {
                alertMessage = "Недостатньо монет для купівлі підказки!";
            }
            continue;
        }

        bool used = false;
        for (int i = 0; i < usedLetters.length(); i++) {
            if (usedLetters[i] == choice) used = true;
        }
        if (used) {
            alertMessage = "Ви вже вводили цю літеру!";
            continue;
        }

        usedLetters += choice;
        usedLetters += " ";

        bool found = false;
        for (int i = 0; i < word.length(); i++) {
            if (word[i] == choice) {
                guessed[i] = choice;
                found = true;
            }
        }

        if (!found) {
            mistakes++;
        }
    }

    system("cls");
    DrawHangman(mistakes);
    if (gameWon) {
        cout << "\nВітаємо! Ви вгадали слово: " << word << "!\n";
        cout << "+ 10 монет!\n";
        coins += 10;
    }
    else {
        cout << "\nВи програли! Загадане слово було: " << word << "\n";
    }

    SaveResult(modeName, word, gameWon, mistakes);
    cout << "\nРезультати гри збережено у файл game_log.txt!\n";
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(NULL));

    int coins = 0;
    int choice = 0;

    system("cls");

    while (choice != 3) {

        cout << "\n           ГРА ШИБЕНИЦЯ             \n";
        cout << "Ваш баланс: " << coins << " монет.\n\n";
        cout << "1. Грати з комп'ютером\n";
        cout << "2. Грати з другом\n";
        cout << "3. Вийти з гри\n";
        cout << "Оберіть дію: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\nОберіть рівень складності:\n";
            cout << "1 - Легкий\n";
            cout << "2 - Середній\n";
            cout << "3 - Складний\n";
            cout << "Вибір: ";
            int ch;
            cin >> ch;

            int randomIndex = rand() % WORDS_COUNT;
            string word_g, hint_g, mode_g;

            switch (ch) {
            case 1:
                word_g = easyWords[randomIndex];
                hint_g = easyHints[randomIndex];
                mode_g = "Проти ПК (Легко)";
                break;
            case 2:
                word_g = medWords[randomIndex];
                hint_g = medHints[randomIndex];
                mode_g = "Проти ПК (Середньо)";
                break;
            case 3:
                word_g = hardWords[randomIndex];
                hint_g = hardHints[randomIndex];
                mode_g = "Проти ПК (Складно)";
                break;
            default:
                cout << "Невірний вибір!\n";
                continue;
            }
            PlayRound(word_g, hint_g, mode_g, coins);

        }
        else if (choice == 2) {
            string customWord, customHint;
            cout << "\nГРАВЕЦЬ 1, введіть слово (у нижньому регістрі): ";
            cin >> customWord;

            cout << "ГРАВЕЦЬ 1, введіть підказку (одне або кілька слів): ";
            cin.ignore();
            getline(cin, customHint);

            PlayRound(customWord, customHint, "Гравець 1 проти Гравця 2", coins);
        }
    }

    cout << "\nДякуємо за гру!\n";
    return 0;
}
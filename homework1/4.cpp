/*
На старости лет разум начал подводить пожилого архитектора, но строить дома он не бросил.
Пытаясь доказать своим коллегам, что он ещё достоин, архитектор построил ещё одно здание...
То был его opus magnum - здание невероятных размеров, безумной сложности, по-своему прекрасное.
Но безумие архитектора просочилось через стены здания... Его строение оказалось специфическим.
В здании есть множество подъездов, но в подъездах оказалось разное количество этажей.
На каждом этаже есть квартиры - но количество квартир на этажах тоже почти не совпадало.
А каждая квартира - практически уникальна. Площади квартир оказались совсем уж разными.

Благо, это были хотя бы целые (но не обязательно натуральные) числа. Безумие и могущество
архитектора оказались столь великими, что он умудрился создать квартиры отрицательной площади
(на отрицательные номера подъездов и этажей его мощи не хватило).

На вход программы подаётся число p (0 < p < 10^3) - число подъездов.
Затем на вход подаётся p раз информация о подъездах:
подаётся число f (0 < f < 10^3) - число этажей.
Затем программа получает f раз информацию о квартирах на этаже:
подаётся число c (0 < c < 10^3) - число квартир, затем c площадей этих квартир.
Каждая площадь помещается в int. Всего в здании не больше 100000 квартир.

В прошлом архитектор был питонистом, что заметно сказалось на его рассудке, к слову.
Важно то, что из-за его прошлого все подъезды, этажи и квартиры он нумерует с 0.

Архитектор ещё не завершил своё творение и всё старается сделать его ещё лучше.
Чтобы не потеряться в собственном здании, ему понадобилась программа для навигации.
После ввода данных мы находимся на уровне здания.
Программа должна отвечать на запросы:

enter n - войти в подъезд n, если мы на уровне здания, или на этаж n, если мы в подъезде.
          С уровня этажа спускаться дальше нельзя.
back    - вернуться с этажа в подъезд или из подъезда на уровень здания.
list    - вывести содержимое текущего уровня, каждый пункт на отдельной строке:
          для здания - номер подъезда и число этажей;
          для подъезда - номер этажа и число квартир;
          для этажа - номер квартиры и её площадь.
          Два числа в строке разделяются пробелом.
sum     - вывести суммарную площадь квартир текущего этажа с учётом отрицательных площадей.
          На других уровнях запрос недоступен.
quit    - завершить работу.

Каждый запрос подаётся на отдельной строке, аргумент enter - целое число.
Если номер не существует, переход невозможен или запрос неизвестен, вывести error.
На запрос sum вне этажа тоже вывести error. Положение при ошибке не меняется.
Успешные enter и back ничего не выводят. Конец ввода также завершает работу.
*/

#include <iostream>

int main() {
  // int*** нужен для цепочки: подъезд -> этаж -> площадь квартиры.
  // Размеры подъездов и этажей разные. Указатели связывают нужные участки
  // массивов, сохраняя обращение areas[подъезд][этаж][квартира].
  // void* позволяет current хранить int***, int** или int* в зависимости
  // от уровня. В path лежат указатели на предыдущие уровни разных типов.
  // Так для навигации хватает одного current и одного массива path.
  // Правильный тип указателя при обращении к данным определяет level.

  constexpr int LIMIT = 1000;
  constexpr int MAX_FLATS = 100000;

  // Площади, указатели на этажи и размеры этажей.
  static int storage[MAX_FLATS];
  static int* floors[MAX_FLATS];
  static int sizes[MAX_FLATS];

  int** entrances[LIMIT];
  int floorCounts[LIMIT];
  int* flatCounts[LIMIT];
  int totalFloors = 0;
  int totalFlats = 0;

  // int*** связывает подъезды, этажи и площади.
  int*** areas = entrances;

  int p;
  if (!(std::cin >> p) || p <= 0 || p >= LIMIT) {
    std::cout << "error\n";
    return 1;
  }

  for (int i = 0; i < p; ++i) {
    if (!(std::cin >> floorCounts[i]) ||
        floorCounts[i] <= 0 || floorCounts[i] >= LIMIT ||
        floorCounts[i] > MAX_FLATS - totalFloors) {
      std::cout << "error\n";
      return 1;
    }

    // Отдаём подъезду свой участок массивов.
    areas[i] = floors + totalFloors;
    flatCounts[i] = sizes + totalFloors;
    totalFloors += floorCounts[i];

    for (int j = 0; j < floorCounts[i]; ++j) {
      if (!(std::cin >> flatCounts[i][j]) ||
          flatCounts[i][j] <= 0 || flatCounts[i][j] >= LIMIT ||
          flatCounts[i][j] > MAX_FLATS - totalFlats) {
        std::cout << "error\n";
        return 1;
      }

      // Отдаём этажу свой участок площадей.
      areas[i][j] = storage + totalFlats;
      totalFlats += flatCounts[i][j];
      for (int k = 0; k < flatCounts[i][j]; ++k) {
        if (!(std::cin >> areas[i][j][k])) {
          std::cout << "error\n";
          return 1;
        }
      }
    }
  }

  const char* commands[] = {"enter", "back", "list", "sum", "quit"};
  char command[16];

  // void*: текущее место и путь назад.
  void* current = areas;
  void* path[2]{};
  // 0 - здание, 1 - подъезд, 2 - этаж.
  int level = 0;
  int entrance = 0;
  int floor = 0;

  while (true) {
    std::cin.width(sizeof(command));
    if (!(std::cin >> command)) {
      break;
    }
    int action = 0;
    for (; action < 5; ++action) {
      int i = 0;
      while (commands[action][i] != '\0' &&
             commands[action][i] == command[i]) {
        ++i;
      }
      if (commands[action][i] == command[i]) {
        break;
      }
    }

    if (action == 0) {
      int index;
      if (!(std::cin >> index)) {
        std::cout << "error\n";
        break;
      }
      int count = level == 0 ? p : floorCounts[entrance];
      if (level == 2 || index < 0 || index >= count) {
        std::cout << "error\n";
        continue;
      }
      path[level] = current;
      if (level == 0) {
        entrance = index;
        current = static_cast<int***>(current)[index];
      } else {
        floor = index;
        current = static_cast<int**>(current)[index];
      }
      ++level;
    } else if (action == 1) {
      if (level == 0) {
        std::cout << "error\n";
        continue;
      }
      current = path[--level];
    } else if (action == 2) {
      int count = p;
      if (level == 1) {
        count = floorCounts[entrance];
      } else if (level == 2) {
        count = flatCounts[entrance][floor];
      }
      for (int i = 0; i < count; ++i) {
        std::cout << i << " ";
        if (level == 0) {
          std::cout << floorCounts[i];
        } else if (level == 1) {
          std::cout << flatCounts[entrance][i];
        } else {
          std::cout << static_cast<int*>(current)[i];
        }
        std::cout << "\n";
      }
    } else if (action == 3) {
      if (level != 2) {
        std::cout << "error\n";
        continue;
      }
      long long sum = 0;
      for (int i = 0; i < flatCounts[entrance][floor]; ++i) {
        sum += static_cast<int*>(current)[i];
      }
      std::cout << sum << "\n";
    } else if (action == 4) {
      break;
    } else {
      std::cout << "error\n";
      char c;
      while (std::cin.get(c) && c != '\n') {
      }
    }
  }

  return 0;
}


/*
P.s. условия вполне можно было сформировать более кратко и понятно, 
не реши я его писать столь иносказательно
Просьба не судить строго)
*/
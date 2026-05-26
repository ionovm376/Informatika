#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <set>
#include <map>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>

using namespace std;
using namespace sf;

// глобальные константы для настройки игрового поля

const int GRID_SIZE = 30; // количество клеток по горизонтали и вертикали
const int CELL_SIZE = 25; // размер одной стороны клетки в пикселях
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE; // итоговый размер окна

const int MIN_V = 1; // минимально возможный вес клетки
const int MAX_V = 10; // максимально возможный вес клетки

// типы состояний, в которых может находиться клетка

enum CellType {
    EMPTY, // проходимая пустая клетка
    OBSTACLE, // препятствие
    START, // точка начала пути
    END, // конечная точка
    PATH // часть найденного маршрута
};

// объект клетки на поле

struct Cell {
    CellType type; // текущий тип
    int weight; // вес клетки
    Cell() : type(EMPTY), weight(1) {} // конструктор: по умолчанию клетка пустая с минимальным весом
    bool is_barrier() const { return type == OBSTACLE; } // метод проверки на препятствие
    bool is_start()   const { return type == START; } // метод проверки на старт
    bool is_end()     const { return type == END; } // метод проверки на финиш

    // методы изменения состояния
    void make_start() { type = START; } // делает точку стартом
    void make_end() { type = END; } // делает точку финишем
    void make_barrier() { type = OBSTACLE; } // делает точку препятствием
    void make_path() { if (type != START && type != END) type = PATH; } // делает точку фрагментом пути, если она не стартовая и не финишная

    // сброс клетки до пустой
    void reset() {
        if (type == PATH) type = EMPTY;
    }
};

// поиск в глубину: используется для проверки наличия пути от старта до финиша

void dfs(int x, int y, const vector<vector<Cell>>& grid, vector<vector<bool>>& visited) {
    if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) // проверка выхода за границы или столкновения со стеной/уже посещенной клеткой
        return;
    if (visited[x][y])
        return;
    if (grid[x][y].type == OBSTACLE)
        return;
    visited[x][y] = true; // помечаем клетку как достижимую
    // рекурсивно идем во все 4 стороны
    dfs(x + 1, y, grid, visited);
    dfs(x - 1, y, grid, visited);
    dfs(x, y + 1, grid, visited);
    dfs(x, y - 1, grid, visited);
}

// узел, хранящий координаты и веса для приоритетной очереди

struct Node {
    int x, y, g, h; // координаты узла (x,y), g - стоимость пути от старта до клетки, h - стоимость от клетки до финиша
    int f() const {
        return g + h; // f(n) = g(n) + h(n)
    }
    Node(int x, int y, int g = 0, int h = 0) : x(x), y(y), g(g), h(h) {
    }
    bool operator>(const Node& other) const { // оператор сравнения: возвращает true, если текущий узел дороже другого
        if (f() != other.f()) return f() > other.f();
        return h > other.h; // если f равны, выбираем тот, у которого меньше h
    }
};

// манхэттенское расстояние

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

vector<pair<int, int>> a_star(const vector<vector<Cell>>& grid, pair<int, int> start, pair<int, int> goal) {
    priority_queue<Node, vector<Node>, greater<Node>> open_set; // приоритетная очередь для хранения узлов, которые нужно исследовать
    set<pair<int, int>> open_set_hash; // хэш-таблица для быстрой проверки: находится ли уже клетка в очереди на исследование
    map<pair<int, int>, pair<int, int>> came_from; // карта ,которая хранит для каждой клетки координаты её предыдущий шаг.
    map<pair<int, int>, int> g_score; // карта стоимостей пути.
    int h_start = manhattan_distance(start.first, start.second, goal.first, goal.second); // вычисляем эвристическое расстояние от старта до цели
    open_set.push(Node(start.first, start.second, 0, h_start)); // помещаем стартовый узел в очередь
    open_set_hash.insert(start);
    g_score[start] = 0; // стоимость достижения старта равна нулю.

    while (!open_set.empty()) {
        Node current = open_set.top(); // берем узел с минимальным показателем f(n)
        open_set.pop();
        open_set_hash.erase({ current.x, current.y });

        if (current.x == goal.first && current.y == goal.second) { // проверка, если текущие координаты совпадают с целью
            vector<pair<int, int>> path;
            pair<int, int> current_pos = goal;
            while (came_from.count(current_pos)) { // реконструкция пути
                path.push_back(current_pos);
                current_pos = came_from[current_pos];
            }
            path.push_back(start); // добавляем в начало сам старт
            reverse(path.begin(), path.end()); // разворачиваем, чтобы путь шел от старта к цели
            return path;
        }

        for (auto [dx, dy] : vector<pair<int, int>>{ {-1, 0}, {1, 0}, {0, -1}, {0, 1} }) {
            int nx = current.x + dx;
            int ny = current.y + dy;

            if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) { // проверка границ сетки.
                if (grid[nx][ny].is_barrier()) continue; // проверка проходимости

                pair<int, int> neighbor = { nx, ny };
                int tentative_g = g_score[{current.x, current.y}] + grid[nx][ny].weight; // g(current) + вес клетки соседа.

                if (!g_score.count(neighbor) || tentative_g < g_score[neighbor]) { // если этот путь до соседа дешевле, чем найденный ранее
                    came_from[neighbor] = { current.x, current.y }; // в эту клетку выгоднее приходить из 'current'
                    g_score[neighbor] = tentative_g;
                    int h = manhattan_distance(nx, ny, goal.first, goal.second); // вычисляем эвристику для соседа.
                    if (!open_set_hash.count(neighbor)) { // если соседа еще нет в очереди на проверку — добавляем его
                        open_set.push(Node(nx, ny, tentative_g, h));
                        open_set_hash.insert(neighbor);
                    }
                }
            }
        }
    }
    return {}; // путь не найден
}

// отрисовка всех клеток на экран

void draw_grid(RenderWindow& window, vector<vector<Cell>>& grid) {
    for (int x = 0; x < GRID_SIZE; ++x) { // проходим циклом по всем клеткам нашей сетки
        for (int y = 0; y < GRID_SIZE; ++y) {
            RectangleShape shape(Vector2f((float)CELL_SIZE - 1.f, (float)CELL_SIZE - 1.f)); // создаем прямоугольник размером чуть меньше клетки
            shape.setPosition((float)x * CELL_SIZE, (float)y * CELL_SIZE); // устанавливаем положение квадрата в окне
            float weight = (float)grid[x][y].weight; // присваим клетке вес
            int colorValue = 40 + static_cast<int>(((weight - MIN_V) / (float)(MAX_V - MIN_V)) * 215); // рассчитываем цвет для пустых клеток на основе их веса
            switch (grid[x][y].type) { // присваиваем цвет в зависимости от типа клетки
            case EMPTY: shape.setFillColor(Color(0, 0, colorValue)); break; // пустая - определяется весом клетки
            case OBSTACLE: shape.setFillColor(Color::Black); break; // препятствие - чёрный
            case START: shape.setFillColor(Color(255, 165, 0)); break; // старт - оранжевый
            case END: shape.setFillColor(Color(64, 224, 208)); break; // конец - бирюзовый
            case PATH: shape.setFillColor(Color(139, 0, 255)); break; // путь - фиолетовый
            }
            window.draw(shape); // отрисовка клетки
        }
    }
}

// генерация случайной карты с гарантированной проходимостью

void generate_random_map(vector<vector<Cell>>& grid) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> wall_dist(0, 3); // 25% шанс генерации препятствия (если выпадет 0)
    uniform_int_distribution<> weight_dist(MIN_V, MAX_V); // случайный вес для клетки от 1 до 10
    bool valid_map = false; // флаг: стала ли карта пригодной для использования
    while (!valid_map) { // цикл будет работать до тех пор, пока не сгенерируется карта, которую можно пройти
        for (int x = 0; x < GRID_SIZE; ++x) { // заполнение сетки случайными данными
            for (int y = 0; y < GRID_SIZE; ++y) {
                grid[x][y].type = EMPTY; // сначала делаем клетку пустой
                grid[x][y].weight = weight_dist(gen); // задаем ей случайную стоимость прохода
                if (wall_dist(gen) == 0) // с шансом 25% превращаем клетку в препятствие
                    grid[x][y].type = OBSTACLE;
            }
        }
        grid[0][0].type = START; // гарантируем, что старт и финиш — не стены
        grid[GRID_SIZE - 1][GRID_SIZE - 1].type = END;
        vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false)); // создаем вспомогательную матрицу посещенных клеток для dfs
        dfs(0, 0, grid, visited); // запускаем поиск в глубину из верхней левой точки
        valid_map = true; // предполагаем, что карта проходимая
        for (int x = 0; x < GRID_SIZE; ++x) {
            for (int y = 0; y < GRID_SIZE; ++y) {
                if (grid[x][y].type != OBSTACLE && !visited[x][y]) { // если клетка — не стена, но dfs до неё не дотянулся
                    valid_map = false; // помечаем карту как непроходимую
                    break;
                }
            }
            if (!valid_map)
                break;
        } // если valid_map остался true — цикл while завершится и карта станет проходимой
    }
}

int main() {
    vector<vector<Cell>> grid(GRID_SIZE, vector<Cell>(GRID_SIZE)); // создаем сетку объектов Cell размером 30x30
    pair<int, int> startPos = { 0, 0 }; // устанавливаем координаты старта и финиша
    pair<int, int> goalPos = { GRID_SIZE - 1, GRID_SIZE - 1 };
    generate_random_map(grid); // генерируем начальную карту со случайными весами и стенами
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "алгоритм A*"); // создаем окно размером 750x750 пикселей
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) { // опрос событий (нажатия клавиш, закрытия окна)
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) { // обработка нажатий клавиш
                if (event.key.code == Keyboard::Space) { // Нажатие пробела — запуск поиска пути
                    for (auto& row : grid) { // сначала проходим по всей сетке и сбрасываем старые пути
                        for (auto& cell : row) cell.reset();
                    }
                    auto path = a_star(grid, startPos, goalPos); // вызываем алгоритм A* и получаем вектор координат пути
                    if (!path.empty()) { // если путь найден
                        int total_weight = 0; // переменная для подсчета общей стоимости пути
                        for (auto [x, y] : path) {
                            total_weight += grid[x][y].weight; // суммируем веса клеток
                            grid[x][y].make_path(); // красим клетку в цвет пути
                        }
                        cout << "Кратчайшее расстояние: " << total_weight << endl; // Выводим результат в консоль
                    }
                }
                if (event.key.code == Keyboard::R) { // нажатие клавиши r — полная перегенерация мира
                    generate_random_map(grid); // создаем новую карту
                    grid[startPos.first][startPos.second].make_start(); // ставим старт и финиш на их места
                    grid[goalPos.first][goalPos.second].make_end();
                }
            }
        }
        window.clear(); // очищаем экран
        draw_grid(window, grid); // вызываем функцию отрисовки нашей сетки
        window.display(); // выводим содержимое буфера на экран
    }
    return 0;
}
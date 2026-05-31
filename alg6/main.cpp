#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <set>
#include <map>
#include <random>
#include <algorithm>

using namespace std;
using namespace sf;

const int GRID_SIZE = 30; // размер сетки 30x30 клеток
const int CELL_SIZE = 25; // размер одной клетки в пикселях
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE; // размер окна 750x750 пикселей

const int MIN_WEIGHT = 1; // минимальный вес клетки (самый дешёвый путь)
const int MAX_WEIGHT = 10; // максимальный вес клетки (самый дорогой путь)

enum CellType { // перечисление типов клеток
    EMPTY, // пустая клетка (проходимая)
    OBSTACLE, // препятствие (стена)
    START, // стартовая клетка
    END, // конечная клетка
    IN_QUEUE, // клетка в очереди на рассмотрение (зелёная)
    VISITED, // посещённая клетка (красная)
    PATH // клетка входящая в найденный путь (фиолетовая)
};

struct Cell { // структура для представления клетки
    int x, y; // координаты клетки на сетке
    CellType type; // тип клетки (из перечисления выше)
    int weight; // вес клетки (стоимость прохождения через неё)
    int g, h, f; // параметры для a*: g - путь от старта, h - эвристика, f = g + h
    Cell(int x = 0, int y = 0) : x(x), y(y), type(EMPTY), weight(1), g(0), h(0), f(0) {} // конструктор
    void set_start() { type = START; } // устанавливает тип "старт"
    void set_end() { type = END; } // устанавливает тип "финиш"
    void set_wall() { type = OBSTACLE; } // устанавливает тип "стена"
    void set_path() { // устанавливает тип "путь"
        if (type != START && type != END) type = PATH;
    }
    void set_visited() { // устанавливает тип "посещённая"
        if (type != START && type != END && type != IN_QUEUE) type = VISITED;
    }
    void set_in_queue() { // устанавливает тип "в очереди"
        if (type != START && type != END && type != VISITED) type = IN_QUEUE;
    }
    void reset() { // сбрасывает временные типы обратно в empty
        if (type == PATH || type == VISITED || type == IN_QUEUE) type = EMPTY;
    }
    bool is_wall() const { // проверяет, является ли клетка стеной
        return type == OBSTACLE;
    }
};

int heuristic(const Cell& a, const Cell& b) { // функция эвристики (манхэттенское расстояние)
    return abs(a.x - b.x) + abs(a.y - b.y); // сумма абсолютных разностей координат
}

bool in_bounds(int x, int y) { // проверка, находятся ли координаты в пределах сетки
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

void dfs(int x, int y, const vector<vector<Cell>>& grid, vector<vector<bool>>& visited) { // обход в глубину
    if (!in_bounds(x, y)) return; // если вышли за границы - возвращаемся
    if (visited[x][y]) return; // если уже посещали эту клетку - возвращаемся
    if (grid[x][y].is_wall()) return; // если клетка - стена - возвращаемся
    visited[x][y] = true; // отмечаем клетку как посещённую
    dfs(x + 1, y, grid, visited); // рекурсивно проверяем соседа справа
    dfs(x - 1, y, grid, visited); // рекурсивно проверяем соседа слева
    dfs(x, y + 1, grid, visited); // рекурсивно проверяем соседа сверху
    dfs(x, y - 1, grid, visited); // рекурсивно проверяем соседа снизу
}

void generate_map(vector<vector<Cell>>& grid, int wall_probability = 25) { // генерация случайной карты
    random_device rd; // устройство для генерации случайных чисел
    mt19937 gen(rd()); // генератор mersenne twister
    uniform_int_distribution<> wall_dist(0, 99); // распределение для стен (0-99)
    uniform_int_distribution<> weight_dist(MIN_WEIGHT, MAX_WEIGHT); // распределение для весов
    bool is_valid = false; // флаг, что карта проходима
    while (!is_valid) { // генерируем карту, пока не получим проходимую
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                grid[i][j] = Cell(i, j); // создаём новую клетку с координатами
                grid[i][j].weight = weight_dist(gen); // присваиваем случайный вес
                if (wall_dist(gen) < wall_probability) { // если выпало число меньше вероятности
                    grid[i][j].type = OBSTACLE; // делаем клетку стеной
                }
            }
        }
        grid[0][0].type = START; // старт в клетке (0,0)
        grid[GRID_SIZE - 1][GRID_SIZE - 1].type = END; // финиш в клетке (29,29)
        vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false)); // матрица посещённых
        dfs(0, 0, grid, visited); // запускаем dfs из старта
        is_valid = true; // предполагаем, что карта проходима
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (grid[i][j].type != OBSTACLE && !visited[i][j]) { // если клетка не стена и не достигнута
                    is_valid = false; // карта непроходима
                    break;
                }
            }
            if (!is_valid) break;
        }
    }
}

void clear_path(vector<vector<Cell>>& grid) { // сброс визуализации пути
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j].reset(); // сбрасываем временные типы клеток
        }
    }
    grid[0][0].type = START; // восстанавливаем старт
    grid[GRID_SIZE - 1][GRID_SIZE - 1].type = END; // восстанавливаем финиш
}

vector<pair<int, int>> a_star(vector<vector<Cell>>& grid, pair<int, int> start, pair<int, int> goal) { // алгоритм a*
    auto cmp = [](const tuple<int, int, int>& a, const tuple<int, int, int>& b) { // компаратор для очереди
        return get<0>(a) > get<0>(b); // чем меньше f, тем выше приоритет
        };
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, decltype(cmp)> pq(cmp); // приоритетная очередь
    set<pair<int, int>> open_set; // множество клеток в очереди
    set<pair<int, int>> closed_set; // множество обработанных клеток
    map<pair<int, int>, int> g_score; // лучшая известная стоимость g для каждой клетки
    map<pair<int, int>, pair<int, int>> came_from; // предыдущая клетка для восстановления пути
    int start_h = heuristic(grid[start.first][start.second], grid[goal.first][goal.second]); // эвристика для старта
    pq.push({ start_h, start.first, start.second }); // добавляем старт в очередь
    open_set.insert({ start.first, start.second }); // отмечаем в множестве очереди
    g_score[{start.first, start.second}] = 0; // стоимость пути до старта = 0
    vector<pair<int, int>> dirs = { {1,0}, {-1,0}, {0,1}, {0,-1} }; // 4 направления движения
    while (!pq.empty()) { // пока очередь не пуста
        auto [f_val, x, y] = pq.top(); // берём клетку с минимальным f
        pq.pop(); // удаляем её из очереди
        pair<int, int> current = { x, y }; // текущая клетка
        open_set.erase(current); // удаляем из множества очереди
        if (closed_set.count(current)) continue; // если уже обработана - пропускаем
        closed_set.insert(current); // отмечаем как обработанную
        if (grid[x][y].type != START && grid[x][y].type != END) { // если не старт и не финиш
            grid[x][y].set_visited(); // отмечаем клетку как посещённую
        }
        if (current == goal) { // достигли цели
            vector<pair<int, int>> path; // вектор для хранения пути
            pair<int, int> step = goal; // начинаем с финиша
            while (came_from.count(step)) { // пока есть предыдущая клетка
                path.push_back(step); // добавляем текущую клетку
                step = came_from[step]; // переходим к предыдущей
            }
            path.push_back(start); // добавляем старт
            reverse(path.begin(), path.end()); // разворачиваем путь
            int total_cost = 0; // общая стоимость пути
            for (auto [px, py] : path) { // проходим по всем клеткам пути
                total_cost += grid[px][py].weight; // суммируем веса клеток
                if (grid[px][py].type != START && grid[px][py].type != END) {
                    grid[px][py].set_path(); // отмечаем клетки пути
                }
            }
            cout << "Кратчайшее расстояние: " << total_cost << endl; // выводим стоимость
            return path; // возвращаем найденный путь
        }
        for (auto [dx, dy] : dirs) { // для каждого направления
            int nx = x + dx; // координата x соседа
            int ny = y + dy; // координата y соседа
            pair<int, int> neighbor = { nx, ny }; // пара координат соседа
            if (!in_bounds(nx, ny)) continue; // если за границами - пропускаем
            if (grid[nx][ny].is_wall()) continue; // если стена - пропускаем
            if (closed_set.count(neighbor)) continue; // если уже обработан - пропускаем
            int move_cost = grid[nx][ny].weight; // стоимость входа в клетку
            int tentative_g = g_score[current] + move_cost; // новая стоимость пути до соседа
            if (!g_score.count(neighbor) || tentative_g < g_score[neighbor]) { // если нашли более дешёвый путь
                g_score[neighbor] = tentative_g; // обновляем лучшую стоимость
                came_from[neighbor] = current; // запоминаем, откуда пришли
                int h_val = heuristic(grid[nx][ny], grid[goal.first][goal.second]); // вычисляем эвристику
                int new_f = tentative_g + h_val; // вычисляем новую полную стоимость f
                pq.push({ new_f, nx, ny }); // добавляем соседа в очередь
                open_set.insert(neighbor); // отмечаем в множестве очереди
                if (grid[nx][ny].type != START && grid[nx][ny].type != END) {
                    grid[nx][ny].set_in_queue(); // отмечаем клетку как находящуюся в очереди
                }
            }
        }
    }
    return {}; // возвращаем пустой вектор (путь не найден)
}

void render_grid(RenderWindow& window, vector<vector<Cell>>& grid) { // отрисовка сетки
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            RectangleShape rect(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1)); // прямоугольник с зазором 1 пиксель
            rect.setPosition(i * CELL_SIZE, j * CELL_SIZE); // устанавливаем позицию
            switch (grid[i][j].type) { // выбираем цвет в зависимости от типа клетки
            case EMPTY: {
                float ratio = (grid[i][j].weight - MIN_WEIGHT) / (float)(MAX_WEIGHT - MIN_WEIGHT); // отношение веса
                int shade = 100 + (int)(155 * ratio); // оттенок от 100 до 255
                rect.setFillColor(Color(shade, shade, shade)); // оттенки серого
                break;
            }
            case OBSTACLE: rect.setFillColor(Color::Black); break; // стена - чёрный
            case START: rect.setFillColor(Color(255, 165, 0)); break; // старт - оранжевый
            case END: rect.setFillColor(Color(64, 224, 208)); break; // финиш - бирюзовый
            case IN_QUEUE: rect.setFillColor(Color::Green); break; // в очереди - зелёный
            case VISITED: rect.setFillColor(Color::Red); break; // посещённая - красный
            case PATH: rect.setFillColor(Color(139, 0, 255)); break; // путь - фиолетовый
            }
            window.draw(rect); // отрисовываем прямоугольник
        }
    }
}

int main() {
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "A* algoritm"); // создаём окно
    vector<vector<Cell>> grid(GRID_SIZE, vector<Cell>(GRID_SIZE)); // создаём сетку клеток
    generate_map(grid, 25); // генерируем начальную карту с 25% стен
    pair<int, int> start_pos = { 0, 0 }; // координаты старта (левый верхний угол)
    pair<int, int> end_pos = { GRID_SIZE - 1, GRID_SIZE - 1 }; // координаты финиша (правый нижний угол)
    while (window.isOpen()) { // главный игровой цикл
        Event event; // объект для хранения событий
        while (window.pollEvent(event)) { // обработка всех событий
            if (event.type == Event::Closed) { // если нажата кнопка закрытия
                window.close(); // закрываем окно
            }
            if (event.type == Event::KeyPressed) { // если нажата клавиша
                if (event.key.code == Keyboard::Space) { // если пробел
                    clear_path(grid);        // сбрасываем предыдущий путь
                    a_star(grid, start_pos, end_pos); // запускаем поиск пути
                }
                if (event.key.code == Keyboard::R) { // если клавиша r
                    generate_map(grid, 25);  // генерируем новую карту
                    clear_path(grid);        // сбрасываем путь
                }
            }
        }
        window.clear(Color::White); // очищаем окно белым цветом
        render_grid(window, grid); // отрисовываем сетку
        window.display(); // отображаем нарисованное на экране
    }
    return 0;
}
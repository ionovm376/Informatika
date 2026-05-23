#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>
#include <windows.h>
#include <chrono>


#include "tinyxml2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using std::string;
using std::vector;
using std::map;
using std::tuple;
using std::pair;
using std::make_pair;
using std::make_tuple;
using std::cout;
using std::endl;

typedef pair<double, double> Coord;
typedef tuple<Coord, Coord, string> EdgeItem;
typedef map<Coord, vector<pair<Coord, double>>> Graph;


double haversine(const Coord& coord1, const Coord& coord2) {
    double lon1 = coord1.first, lat1 = coord1.second;
    double lon2 = coord2.first, lat2 = coord2.second;
    double R = 6371.0; 

    double phi1 = std::atan2(std::sin(lat1 * M_PI / 180.0), std::cos(lat1 * M_PI / 180.0));
    double phi2 = std::atan2(std::sin(lat2 * M_PI / 180.0), std::cos(lat2 * M_PI / 180.0));
    phi1 = lat1 * M_PI / 180.0;
    phi2 = lat2 * M_PI / 180.0;
    double dphi = (lat2 - lat1) * M_PI / 180.0;
    double dlambda = (lon2 - lon1) * M_PI / 180.0;

    double a = std::sin(dphi / 2.0) * std::sin(dphi / 2.0) +
        std::cos(phi1) * std::cos(phi2) *
        std::sin(dlambda / 2.0) * std::sin(dlambda / 2.0);
    return 2 * R * std::atan2(std::sqrt(a), std::sqrt(1 - a));
}


tuple<vector<Coord>, double, vector<string>> dijkstra(const Graph& graph, const Coord& start, const Coord& end) {
    typedef pair<double, Coord> QueueItem;
    auto cmp = [](const QueueItem& left, const QueueItem& right) {
        return left.first > right.first;
        };
    std::priority_queue<QueueItem, vector<QueueItem>, decltype(cmp)> priority_queue(cmp);
    priority_queue.push(make_pair(0.0, start));
    
    map<Coord, pair<Coord, double>> dict_shortdist;
    dict_shortdist[start] = make_pair(start, 0.0);

    vector<Coord> visited;

    while (!priority_queue.empty()) {
        double current_distance = priority_queue.top().first;

        Coord current_node = priority_queue.top().second;
        priority_queue.pop();

        if (current_node == end) {
            break;
        }

        if (std::find(visited.begin(), visited.end(), current_node) != visited.end()) {
            continue;
        }

        visited.push_back(current_node);

        auto it = graph.find(current_node);
        if (it != graph.end()) {
            for (auto& neighbor_pair : it->second) {
                Coord neighbor = neighbor_pair.first;
                double distance = neighbor_pair.second;
                double total_distance = current_distance + distance;

                if (dict_shortdist.find(neighbor) == dict_shortdist.end() ||
                    total_distance < dict_shortdist[neighbor].second) {
                    dict_shortdist[neighbor] = make_pair(current_node, total_distance);
                    priority_queue.push(make_pair(total_distance, neighbor));
                }
            }
        }
    }
    vector<Coord> path;
    vector<string> street_names;
    if (dict_shortdist.find(end) == dict_shortdist.end()) {
        return make_tuple(path, std::numeric_limits<double>::infinity(), street_names);
    }
    Coord current_node = end;
    while (true) {
        path.push_back(current_node);
        auto pred = dict_shortdist[current_node].first;
        if (current_node == start) {
            break;
        }
        current_node = pred;
    }
    std::reverse(path.begin(), path.end());

    double total_distance = dict_shortdist[end].second;
    return make_tuple(path, total_distance, street_names);
}

Graph build_graph(const vector<EdgeItem>& edges) {
    Graph graph;
    for (const auto& edge : edges) {
        Coord start, end;
        string street;
        std::tie(start, end, street) = edge;
        double dist = haversine(start, end);
        graph[start].push_back(make_pair(end, dist));
        graph[end].push_back(make_pair(start, dist));  
    }
    return graph;
}

std::string utf8_to_cp1251(const std::string& utf8_str) {
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wide_len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_len);
    int cp1251_len = WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string cp1251_str(cp1251_len, '\0');
    WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, &cp1251_str[0], cp1251_len, nullptr, nullptr);
    return cp1251_str;
}

tuple<map<string, Coord>, vector<EdgeItem>> read_graphml(const string& file_path) {
    map<string, Coord> nodes;
    vector<EdgeItem> edges;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << file_path << endl;
        return make_tuple(nodes, edges);
    }

    tinyxml2::XMLElement* graphml = doc.FirstChildElement("graphml");
    if (!graphml) {
        std::cerr << "Нет файла" << endl;
        return make_tuple(nodes, edges);
    }

    tinyxml2::XMLElement* graph = graphml->FirstChildElement("graph");
    if (!graph) {
        std::cerr << "Нет элементов графа" << endl;
        return make_tuple(nodes, edges);
    }

    for (tinyxml2::XMLElement* node = graph->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {
        const char* id = node->Attribute("id");
        if (!id) continue;

        double x = 0, y = 0;
        bool has_coords = false;

        for (tinyxml2::XMLElement* data = node->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (!key) continue;

            if (strcmp(key, "d5") == 0) {
                x = atof(data->GetText());
                has_coords = true;
            }
            else if (strcmp(key, "d4") == 0) {
                y = atof(data->GetText());
                has_coords = true;
            }
        }

        if (has_coords) {
            nodes[id] = make_pair(x, y);
        }
    }

    for (tinyxml2::XMLElement* edge = graph->FirstChildElement("edge"); edge; edge = edge->NextSiblingElement("edge")) {
        const char* source_id = edge->Attribute("source");
        const char* target_id = edge->Attribute("target");
        if (!source_id || !target_id) continue;

        auto source_it = nodes.find(source_id);
        auto target_it = nodes.find(target_id);
        if (source_it == nodes.end() || target_it == nodes.end()) continue;

        string street_name;
        for (tinyxml2::XMLElement* data = edge->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (!key) continue;

            if (strcmp(key, "d15") == 0 && data->GetText()) { 
                street_name = data->GetText(); 
            }
        }
        edges.emplace_back(source_it->second, target_it->second, street_name);
    }

    cout << "Количество вершин " << nodes.size() << " ребер " << edges.size() << endl;

    return make_tuple(nodes, edges);
}

std::string convert_cp1251_to_utf8(const std::string& input) {
    int wlen = MultiByteToWideChar(1251, 0, input.c_str(), -1, nullptr, 0);
    if (wlen <= 0) return "";
    std::wstring wstr(wlen, L'\0');
    MultiByteToWideChar(1251, 0, input.c_str(), -1, &wstr[0], wlen);
    int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (ulen <= 0) return "";
    std::string utf8_str(ulen, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8_str[0], ulen, nullptr, nullptr);
    return utf8_str;
}

pair<int, string> find_street_index(const vector<EdgeItem>& edges, const string& street_name_query) {
    for (size_t i = 0; i < edges.size(); i++) {
        string name = std::get<2>(edges[i]);
        if (name == street_name_query) {
            return make_pair(static_cast<int>(i), name);
        }
    }
    return make_pair(-1, "");
}

void visualize_path_with_network(const map<string, Coord>& nodes,
    const vector<EdgeItem>& edges,
    const vector<Coord>& path,
    const vector<string>& street_names = vector<string>(),
    const sf::Vector2u& figsize = sf::Vector2u(800, 800)) {

    sf::RenderWindow window(sf::VideoMode(figsize.x, figsize.y), "Кратчайший маршрут");

    double minX = std::numeric_limits<double>::max(), maxX = -std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max(), maxY = -std::numeric_limits<double>::max();
    for (auto& node : nodes) {
        double x = node.second.first;
        double y = node.second.second;
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    auto transform_coord = [=](const Coord& c) -> sf::Vector2f {
        double scaleX = (figsize.x - 40) / (maxX - minX + 1e-6);
        double scaleY = (figsize.y - 40) / (maxY - minY + 1e-6);
        float x = static_cast<float>((c.first - minX) * scaleX + 20);
        float y = static_cast<float>(figsize.y - ((c.second - minY) * scaleY + 20));
        return sf::Vector2f(x, y);
        };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        for (const auto& edge : edges) {
            Coord start, end;
            string dummy;
            std::tie(start, end, dummy) = edge;
            sf::Vertex line[] =
            {
                sf::Vertex(transform_coord(start), sf::Color(128,128,128)),
                sf::Vertex(transform_coord(end), sf::Color(128,128,128))
            };
            window.draw(line, 2, sf::Lines);
        }

        if (!path.empty() && path.size() > 1) {
            for (size_t i = 0; i < path.size() - 1; i++) {
                sf::Vertex line[] =
                {
                    sf::Vertex(transform_coord(path[i]), sf::Color::Red),
                    sf::Vertex(transform_coord(path[i + 1]), sf::Color::Red)
                };
                window.draw(line, 2, sf::Lines);
                if (!street_names.empty() && i < street_names.size() && !street_names[i].empty()) {
                    sf::Font font;
                    if (!font.loadFromFile("arial.ttf")) {
                    }
                    else {
                        sf::Text text;
                        text.setFont(font);
                        text.setString(street_names[i]);
                        text.setCharacterSize(12);
                        text.setFillColor(sf::Color::Blue);
                        float midX = (transform_coord(path[i]).x + transform_coord(path[i + 1]).x) / 2;
                        float midY = (transform_coord(path[i]).y + transform_coord(path[i + 1]).y) / 2;
                        text.setPosition(midX, midY);
                        window.draw(text);
                    }
                }
            }
        }

        window.display();
    }
}

void visualize_path_with_network_duplicate(const map<string, Coord>& nodes,
    const vector<EdgeItem>& edges,
    const vector<Coord>& path,
    const vector<string>& street_names = vector<string>(),
    const sf::Vector2u& figsize = sf::Vector2u(800, 800)) {
    visualize_path_with_network(nodes, edges, path, street_names, figsize);
}

void visualize_only_path(const vector<Coord>& path, const sf::Vector2u& figsize = sf::Vector2u(400, 400)) {
    if (path.empty() || path.size() < 2) {
        std::cout << "Маршрут слишком короткий или отсутствует." << std::endl;
        return;
    }

    sf::RenderWindow window(sf::VideoMode(figsize.x, figsize.y), "Кратчайший маршрут (только путь)");

    double minX = std::numeric_limits<double>::max(), maxX = -std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max(), maxY = -std::numeric_limits<double>::max();
    for (const auto& c : path) {
        if (c.first < minX) minX = c.first;
        if (c.first > maxX) maxX = c.first;
        if (c.second < minY) minY = c.second;
        if (c.second > maxY) maxY = c.second;
    }
    auto transform_coord = [=](const Coord& c) -> sf::Vector2f {
        double scaleX = (figsize.x - 40) / (maxX - minX + 1e-6);
        double scaleY = (figsize.y - 40) / (maxY - minY + 1e-6);
        float x = static_cast<float>((c.first - minX) * scaleX + 20);
        float y = static_cast<float>(figsize.y - ((c.second - minY) * scaleY + 20));
        return sf::Vector2f(x, y);
        };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::White);

        for (size_t i = 0; i < path.size() - 1; i++) {
            sf::Vertex line[] =
            {
                sf::Vertex(transform_coord(path[i]), sf::Color::Red),
                sf::Vertex(transform_coord(path[i + 1]), sf::Color::Red)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
}

void save_visualization(const string& filename, int dpi = 300) {
    sf::RenderTexture renderTexture;
    sf::Vector2u size(800, 800);
    if (!renderTexture.create(size.x, size.y)) {
        std::cerr << "Не удалось создать RenderTexture" << std::endl;
        return;
    }
    renderTexture.clear(sf::Color::White);
    renderTexture.display();

    sf::Texture texture = renderTexture.getTexture();
    sf::Image screenshot = texture.copyToImage();
    if (!screenshot.saveToFile(filename)) {
        std::cerr << "Не удалось сохранить изображение в файл " << filename << std::endl;
    }
}

void print_edges(const std::vector<EdgeItem>& edges) {
    for (const auto& [start, end, name] : edges) {
        if (!name.empty()) {
            std::cout << "ул." << name;
        }
        std::cout << std::endl;
    }
}

template <typename Func, typename... Args>
auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now(); 
    auto result = func(std::forward<Args>(args)...);  
    auto end_time = std::chrono::high_resolution_clock::now();  

    auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    double duration_ms = duration_ns / 1000000.0;
    return std::make_pair(result, duration_ms);
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    map<string, Coord> nodes;
    vector<EdgeItem> edges;
    std::tie(nodes, edges) = read_graphml("budapest_road_network.graphml");
    string start_street_query = "Maros utca";
    string end_street_query = "Angeli utca";

    pair<int, string> start_result = find_street_index(edges, start_street_query);
    std::cout << start_result.second << std::endl;
    pair<int, string> end_result = find_street_index(edges, end_street_query);
    std::cout << end_result.second << std::endl;
    int start_index = start_result.first;
    int end_index = end_result.first;
    string start_street = start_result.second;
    string end_street = end_result.second;

    if (start_index == -1 || end_index == -1) {
        std::cout << "Не удалось найти заданную улицу для начала или конца маршрута" << std::endl;
    }
    else {
        
        Coord start_node = std::get<0>(edges[start_index]);
        Coord end_node = std::get<1>(edges[end_index]);
        Graph graph = build_graph(edges);
        vector<Coord> path;
        double distance;
        vector<string> street_names;
        std::tie(path, distance, street_names) = dijkstra(graph, start_node, end_node);

        auto [path_result, time_ms] = measure_execution_time(
            [&](const Graph& grapher, const Coord& starter, const Coord& ender) -> vector<Coord> {
                vector<Coord> mii;
                double ga;
                vector<string> ik;
                std::tie(mii, ga, ik) = dijkstra(grapher, starter, ender);
                return mii;
            },
            graph, start_node, end_node
        );

        cout << "Время работы алгоритма Дейкстры: " << time_ms << " мс" << endl;

        if (path.empty()) {
            std::cout << "Путь не найден" << std::endl;
        }
        else {
            std::cout << "Найден путь длиной " << distance << " км" << std::endl;
            bool first = true;
            for (const auto& s : street_names) {
                if (!s.empty()) {
                    if (!first)
                        std::cout << ", ";
                    std::cout << s;
                    first = false;
                }
            }
            std::cout << std::endl;

            visualize_path_with_network(nodes, edges, path, street_names);
        }
    }
    return 0;
}
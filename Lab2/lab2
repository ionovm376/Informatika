#include <SFML/Graphics.hpp>
#include <functional> 
#include <cmath> 
#include <iostream>

// ������� ��� ��������� �������
void drawGraph(sf::RenderWindow& window, std::function<float(float)> func, float xMin, float xMax, float scaleX, float scaleY, sf::Color color) {
    sf::VertexArray graph(sf::LinesStrip);

    for (float x = xMin; x <= xMax; x += 0.1f) {
        float y = func(x); // ���������� �������� �������

        // �������������� ��������� � ��������
        float screenX = 400 + x * scaleX;
        float screenY = 300 - y * scaleY;

        // ���������� ����� � ������ ������
        graph.append(sf::Vertex(sf::Vector2f(screenX, screenY), color));
    }

    window.draw(graph);
}

// ������� y = -x
float f1(float x) {
    return -x;
}

// (f2 �� ������������ ��� ������ �����, �.�. ������ ����� � ������������ x = -3)
float f2(float x) {
    return 0.0f;
}

int main() {
    // �������� ����
    sf::RenderWindow window(sf::VideoMode(800, 600), "Application for displaying graphs");

    // ���������� ��� �������� ���������������� �����
    sf::CircleShape userPoint(3);
    userPoint.setFillColor(sf::Color::Red);
    bool userPointExists = false;

    // 1 _ �������� ������ (�������� ���)
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }
    // 2 _ ����� ��� ����������� ��������� ����� (�������� ���)
    sf::Text coordinatesText;
    coordinatesText.setFont(font);
    coordinatesText.setCharacterSize(20);
    coordinatesText.setFillColor(sf::Color::White);
    coordinatesText.setPosition(10, 10);

    // ��� X � Y
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(50, 300);
    xAxis[0].color = sf::Color::White;
    xAxis[1].position = sf::Vector2f(750, 300);
    xAxis[1].color = sf::Color::White;

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 50);
    yAxis[0].color = sf::Color::White;
    yAxis[1].position = sf::Vector2f(400, 550);
    yAxis[1].color = sf::Color::White;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // �������� ����� �����
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // ��������� ������� �����
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // �������������� �������� ��������� � "��������������"
                    float mathX = (mousePos.x - 400) / 10.0f; // ������� �� X
                    float mathY = -(mousePos.y - 300) / 10.0f; // ������� �� Y

                    // ��������� ����� ���������������� �����
                    userPoint.setPosition(mousePos.x - userPoint.getRadius(),
                        mousePos.y - userPoint.getRadius());
                    userPointExists = true;

                    // 3 _ ������ �������� ����� �� ���������� mathX � mathY
                    std::string pos = "Border";  // �� ��������� � �������

                    const float eps = 1e-4f;
                    bool onLine1 = std::fabs(mathY - f1(mathX)) < eps;        // ����� �� y = -x
                    bool onLine2 = std::fabs(mathX - (-3.0f)) < eps;           // ����� �� x = -3

                    if (onLine1 || onLine2) {
                        pos = "Border";
                    }
                    else if (mathX < -3.0f && mathY > f1(mathX)) {
                        pos = "2";
                    }
                    else if (mathX < -3.0f && mathY < f1(mathX)) {
                        pos = "3";
                    }
                    else if (mathX > -3.0f && mathY < f1(mathX)) {
                        pos = "4";
                    }
                    else if (mathX > -3.0f && mathY > f1(mathX)) {
                        pos = "1";
                    }
                    // ����� ������� "Border"

                    // ���������� ������ � ������������ �����
                    coordinatesText.setString("Coordinates: (" +
                        std::to_string(mathX) + ", " +
                        std::to_string(mathY) + ")\nPosition: " + pos);
                }
            }
        }

        // 4 _ ������� ������ (�������� ���)
        window.clear();

        // ��������� ����
        window.draw(xAxis);
        window.draw(yAxis);

        // 5 _ ��������� ������� y1 = x*x (�������� �� y = -x)
        drawGraph(window, [](float x) { return -x; }, -10, 10, 10, 10, sf::Color::Blue);

        // 5 _ ��������� ������� y2 = x + 9 (�������� �� ������������ x = -3)
        {
            float xConst = -3.0f;
            float screenX = 400 + xConst * 10.0f;
            float screenY1 = 300 - (-10.0f) * 10.0f;
            float screenY2 = 300 - (10.0f) * 10.0f;
            sf::VertexArray line(sf::Lines, 2);
            line[0] = sf::Vertex(sf::Vector2f(screenX, screenY1), sf::Color::Red);
            line[1] = sf::Vertex(sf::Vector2f(screenX, screenY2), sf::Color::Red);
            window.draw(line);
        }

        // ��������� ���������������� ����� � ������
        if (userPointExists) {
            window.draw(userPoint);
            window.draw(coordinatesText);
        }

        // ����������� ������ �����
        window.display();
    }

    return 0;
}


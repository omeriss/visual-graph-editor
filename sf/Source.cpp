#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <math.h>
#include <Windows.h>
#include <string.h>
#include "Gvertex.h"
#include "Node.h"
#include "UnionFind.h"
using namespace std;


#define R 50
typedef sf::Vertex line[2];


int startPoint = -1, endPoint = -1;
int lastLineP1 = -1, lastLineP2 = -1;
sf::Font font;
sf::RenderWindow* pWindow;
vector<Gvertex> nodes;
vector<line*> lines;
vector<vector<pair<int, int>>> edjs;
vector<sf::Text> lineTxt;

#pragma region WindowFuncs
void refWind(bool readTxt = false) {
    sf::Event event;
    while ((*pWindow).pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            (*pWindow).close();
        if (event.type == sf::Event::TextEntered && readTxt) {
            if (event.text.unicode < 128) {
                if (lastLineP2 != -1 && lastLineP1 != -1) {
                    if (event.text.unicode == 8) {
                        lineTxt[lineTxt.size() - 1].setString(to_string(stoi(lineTxt[lineTxt.size() - 1].getString().toAnsiString()) / 10));
                        edjs[lastLineP1][edjs[lastLineP1].size() - 1].second /= 10;
                        edjs[lastLineP2][edjs[lastLineP2].size() - 1].second /= 10;
                    }
                    else if (event.text.unicode <= '9' && event.text.unicode >= '0') {
                        lineTxt[lineTxt.size() - 1].setString(to_string(stoi(lineTxt[lineTxt.size() - 1].getString().toAnsiString()) * 10 + event.text.unicode - '0'));
                        edjs[lastLineP1][edjs[lastLineP1].size() - 1].second = edjs[lastLineP1][edjs[lastLineP1].size() - 1].second * 10 + event.text.unicode - '0';
                        edjs[lastLineP2][edjs[lastLineP2].size() - 1].second = edjs[lastLineP2][edjs[lastLineP2].size() - 1].second * 10 + event.text.unicode - '0';
                    }
                }
            }
        }
    }
}

void WaitFor(sf::Keyboard::Key key) {
    bool enterP = true;
    while (!sf::Keyboard::isKeyPressed(key) || enterP)
    {
        refWind();
        if (!sf::Keyboard::isKeyPressed(key))
            enterP = false;
        Sleep(10);
    }
}

void PrintScreen(bool doClear = true, bool doDisplay = true) {
    if(doClear)
        pWindow->clear();
    for (auto lineT : lines) {
        pWindow->draw(*lineT, 2, sf::Lines);
    }
    for (auto s : nodes) {
        if(s.isActive)
            pWindow->draw(s.GetShape());
    }
    for (auto txtt : lineTxt) {
        pWindow->draw(txtt);
    }
    if(doDisplay)
        pWindow->display();
}
#pragma endregion


#pragma region Dijksra

Node* dj() {
    bool* done = new bool[nodes.size()];
    memset(done, 0, nodes.size() * sizeof(bool));
    priority_queue<pair<int, pair<int, Node*>>> q;
    q.push({ 0, {endPoint, new Node(endPoint, NULL)} });
    while (!q.empty()) {
        int x = q.top().second.first;
        int d = -q.top().first;
        //cout << x << endl;
        Node* nt = q.top().second.second;
        q.pop();
        if (done[x])
            continue;
        done[x] = true;
        if (x == startPoint) {
            cout << d << endl;
            return nt;
        }
        for (auto u : edjs[x]) {
            if (!done[u.first]) {
                Node* ndt = new Node(u.first, nt);
                q.push({ -d - u.second ,{u.first, ndt} });
            }
        }
    }
    return NULL;
}

void djStaps() {
    vector<sf::Text> nodeDisTxt;
    bool* done = new bool[nodes.size()];
    memset(done, 0, nodes.size() * sizeof(bool));
    priority_queue<pair<int, pair<int, Node*>>> q;
    q.push({ 0, {startPoint, new Node(startPoint, NULL)} });
    while (!q.empty()) {
        int x = q.top().second.first;
        int d = -q.top().first;
        Node* nt = q.top().second.second;
        q.pop();
        if (done[x])
            continue;
        done[x] = true;
        
        for (auto u : edjs[x]) {
            if (!done[u.first]) {
                Node* ndt = new Node(u.first, nt);
                q.push({ -d - u.second ,{u.first, ndt} });
            }
        }

        WaitFor(sf::Keyboard::Key::Enter);
        nodes[x].dot.setFillColor(sf::Color::Blue);
        sf::Text ttemp(to_string(d), font);
        ttemp.setFillColor(sf::Color::White);
        ttemp.setPosition(nodes[x].x, nodes[x].y);
        nodeDisTxt.push_back(ttemp);
        (*pWindow).clear();
        PrintScreen(0, 0);
        for (auto txtt : nodeDisTxt) {
            (*pWindow).draw(txtt);
        }
        pWindow->display();
    }

    WaitFor(sf::Keyboard::Key::Enter);
    int lens = nodes.size();
    for (int i = 0; i < lens; i++) {
        nodes[i].dot.setFillColor(sf::Color::White);
    }

    startPoint = -1;
    endPoint = -1;

}

#pragma endregion

#pragma region MinSpanningTree

void FindConnected(int x, bool* inTree) {
    if (inTree[x])
        return;
    inTree[x] = true;
    for (auto u : edjs[x]) {
        FindConnected(u.first, inTree);
    }

}

void MinSpanning(int x, bool sbs) {
    int tempLen = nodes.size();
    bool* inTree = new bool[tempLen];
    UnionFind uni(tempLen);
    vector <pair<int, pair<int, int>>> tempEdj;
    vector<sf::Text> TlineTxt;
    vector<line*> Tlines;

    for (int i = 0; i < tempLen; i++) {
        inTree[i] = 0;
    }
    FindConnected(x, inTree);
    int it = 0;
    for (auto u1 : edjs) {
        for (auto u2 : u1) {
            tempEdj.push_back({ u2.second , {it, u2.first} });
        }
        it++;
    }
    sort(tempEdj.begin(), tempEdj.end());
    if(sbs)
        WaitFor(sf::Keyboard::Enter);
    for (auto u : tempEdj) {
        if (uni.Sfind(u.second.first) != uni.Sfind(u.second.second) && inTree[u.second.first] && inTree[u.second.second]) {
            uni.Sunion(u.second.first, u.second.second);
            line* lt = (line*)malloc(sizeof(line));;
            lt[0][0] = sf::Vertex(sf::Vector2f(nodes[u.second.first].x, nodes[u.second.first].y));
            lt[0][1] = sf::Vertex(sf::Vector2f(nodes[u.second.second].x, nodes[u.second.second].y));
            Tlines.push_back(lt);
            int sqa = (int)sqrt((nodes[u.second.first].x - nodes[u.second.second].x) * (nodes[u.second.first].x - nodes[u.second.second].x) + (nodes[u.second.first].y - nodes[u.second.second].y) * (nodes[u.second.first].y - nodes[u.second.second].y));
            //sf::Text ttemp(to_string(sqa), font);
            sf::Text ttemp(to_string(u.first), font);
            ttemp.setFillColor(sf::Color::Color(255, 128, 0, 255));
            ttemp.setPosition((nodes[u.second.first].x + nodes[u.second.second].x) / 2, (nodes[u.second.first].y + nodes[u.second.second].y) / 2);
            TlineTxt.push_back(ttemp);
            if (sbs) {
                PrintScreen(1, 0);
                for (auto lineT : Tlines) {
                    lineT[0][0].color = sf::Color::Green;
                    lineT[0][1].color = sf::Color::Green;
                    pWindow->draw(*lineT, 2, sf::Lines);
                    lineT[0][0].color = sf::Color::White;
                    lineT[0][1].color = sf::Color::White;
                }
                pWindow->display();
                WaitFor(sf::Keyboard::Enter);
            }
        }
    }
    if (!sbs) {
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            refWind();
            pWindow->clear();
            for (auto lineT : Tlines) {
                pWindow->draw(*lineT, 2, sf::Lines);
            }
            int it = 0;
            for (auto s : nodes) {
                if (s.isActive && inTree[it])
                    pWindow->draw(s.GetShape());
                it++;
            }
            for (auto txtt : TlineTxt) {
                pWindow->draw(txtt);
            }
            pWindow->display();
        }
    }
    for (auto lineT : Tlines) {
        free(lineT);
    }
}

#pragma endregion

#pragma region Input Funcs

int findVer(int xx, int yy) {
    int ii = 0;
    for (auto s : nodes) {
        if ((xx - s.x) * (xx - s.x) + (yy - s.y) * (yy - s.y) < R * R && s.isActive) {
            break;
        }
        ii++;
    }
    return ii;
}

void LClick() {
    int mindis = INT_MAX;
    int xx = sf::Mouse::getPosition(*pWindow).x;
    int yy = sf::Mouse::getPosition(*pWindow).y;
    for (auto s : nodes) {
        if (s.isActive)
            mindis = min(mindis, (xx - s.x) * (xx - s.x) + (yy - s.y) * (yy - s.y));
    }

    if (mindis > R * R * 4) {
        sf::CircleShape shape(R);
        shape.setFillColor(sf::Color::White);
        shape.setOrigin(R, R);
        shape.setPosition(sf::Mouse::getPosition(*pWindow).x, sf::Mouse::getPosition(*pWindow).y);
        cout << sf::Mouse::getPosition(*pWindow).x << " " << sf::Mouse::getPosition(*pWindow).y << endl;
        Gvertex e(sf::Mouse::getPosition(*pWindow).x, sf::Mouse::getPosition(*pWindow).y, shape);
        nodes.push_back(e);
        edjs.push_back(vector<pair<int, int>>());
    }
}

void RClick(int& lastRClick, int& lastX, int& lastY, bool& rDown) {
    int ii = 0;
    int xx = sf::Mouse::getPosition(*pWindow).x;
    int yy = sf::Mouse::getPosition(*pWindow).y;
    if (xx != lastX || yy != lastY || !rDown) {
        lastX = xx;
        lastY = yy;
        ii = findVer(xx, yy);

        if (ii != nodes.size() && ii != lastRClick) {
            cout << ii << endl;
            if (lastRClick == -1)
                lastRClick = ii;
            else {
                line* lt = (line*)malloc(sizeof(line));
                lt[0][0] = sf::Vertex(sf::Vector2f(nodes[ii].x, nodes[ii].y));
                lt[0][1] = sf::Vertex(sf::Vector2f(nodes[lastRClick].x, nodes[lastRClick].y));
                lines.push_back(lt);
                edjs[ii].push_back({ lastRClick, sqrt((nodes[ii].x - nodes[lastRClick].x) * (nodes[ii].x - nodes[lastRClick].x) + (nodes[ii].y - nodes[lastRClick].y) * (nodes[ii].y - nodes[lastRClick].y)) });
                edjs[lastRClick].push_back({ ii, sqrt((nodes[ii].x - nodes[lastRClick].x) * (nodes[ii].x - nodes[lastRClick].x) + (nodes[ii].y - nodes[lastRClick].y) * (nodes[ii].y - nodes[lastRClick].y)) });
                int sqa = (int)sqrt((nodes[ii].x - nodes[lastRClick].x) * (nodes[ii].x - nodes[lastRClick].x) + (nodes[ii].y - nodes[lastRClick].y) * (nodes[ii].y - nodes[lastRClick].y));


                sf::Text ttemp(to_string(sqa), font);
                ttemp.setFillColor(sf::Color::Color(255, 128, 0, 255));
                ttemp.setPosition((nodes[ii].x + nodes[lastRClick].x) / 2, (nodes[ii].y + nodes[lastRClick].y) / 2);
                lineTxt.push_back(ttemp);


                lastLineP1 = lastRClick;
                lastLineP2 = ii;

                lastRClick = -1;
            }
        }
    }
}

void DelNode() {
    int xx = sf::Mouse::getPosition(*pWindow).x;
    int yy = sf::Mouse::getPosition(*pWindow).y;
    int ii = findVer(xx, yy);
    if (ii != nodes.size()) {

        int lene = edjs.size();
        for (int j = 0; j < lene; j++) {
            int len = edjs[j].size();
            for (int i = 0; i < len; i++) {
                if (edjs[j][i].first == ii) {
                    edjs[j].erase(edjs[j].begin() + i);
                    len--;
                    i--;
                }
            }
        }

        int len = lines.size();

        for (int i = 0; i < len; i++) {
            if (lines[i][0][0].position == nodes[ii].GetShape().getPosition() || lines[i][0][1].position == nodes[ii].GetShape().getPosition()) {
                lines.erase(lines.begin() + i);
                lineTxt.erase(lineTxt.begin() + i);
                i--;
                len--;
            }
        }

        nodes[ii].isActive = false;

        if (startPoint == ii)
            startPoint = -1;
        if (endPoint == ii)
            endPoint = -1;
    }
}

void SetNodeAs(int& p, sf::Color color) {
    int xx = sf::Mouse::getPosition(*pWindow).x;
    int yy = sf::Mouse::getPosition(*pWindow).y;
    int ii = findVer(xx, yy);

    if (ii != nodes.size() && ii != startPoint && ii != endPoint) {
        if (p != -1) {
            nodes[p].dot.setFillColor(sf::Color::White);
        }
        p = ii;
        nodes[p].dot.setFillColor(color);
    }
}

void djAnimation() {
    Node* djTree = dj();
    int firstNodeT, lastNodeT;
    if (djTree) {
        firstNodeT = djTree->val;
        lastNodeT = firstNodeT;


        while (djTree)
        {
            nodes[djTree->val].dot.setFillColor(sf::Color::Blue);
            PrintScreen();
            Sleep(1000);
            refWind();
            lastNodeT = djTree->val;
            djTree = djTree->next;
        }

        int lens = nodes.size();
        for (int i = 0; i < lens; i++) {
            nodes[i].dot.setFillColor(sf::Color::White);
        }

        nodes[firstNodeT].dot.setFillColor(sf::Color::Green);
        nodes[lastNodeT].dot.setFillColor(sf::Color::Red);
    }
}

#pragma endregion

int main()
{

    int lastRClick = -1;
    int lastX = -1, lastY = -1;
    bool rDown = false;
    
    if (!font.loadFromFile("arial.ttf")) {
        cout << "cant load font";
    }

    sf::RenderWindow window(sf::VideoMode(1500, 1500), "graph");
    pWindow = &window;


    while (window.isOpen())
    {
        try {
      
            refWind(true);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                LClick();
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                RClick(lastRClick, lastX, lastY, rDown);
                rDown = true;
            }
            else {
                rDown = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                SetNodeAs(startPoint, sf::Color::Green);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
                SetNodeAs(endPoint, sf::Color::Red);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))) {
                if (startPoint != -1)
                    djStaps();
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && startPoint != -1 && endPoint != -1) {
                djAnimation();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                int xx = sf::Mouse::getPosition(window).x;
                int yy = sf::Mouse::getPosition(window).y;
                int ii = findVer(xx, yy);
                if (ii != nodes.size()) {
                    MinSpanning(ii, sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                DelNode();
            }

            PrintScreen();
        }
        catch (const std::exception& e) {
            std::cout << e.what();
        }
    }
    
    return 0;
}
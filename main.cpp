#include <iostream>
#include <ncurses.h>
#include <experimental/filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>
#define err(x) std::cerr<<x<<std::endl;endwin();exit(-1);
std::string getLevelPath(int n) {
    return "./levels/" + std::to_string(n) + ".txt";
}
std::vector<std::string> readLevel(int n) {
    std::vector<std::string> level;
    std::ifstream infile(getLevelPath(n));
    std::string current;
    while (std::getline(infile, current))
        if (current.size() > 0)
            level.push_back(current);
    infile.close();
    return level;
}
struct Pos {
    int y, x;
};
Pos findInLevel(char val, const std::vector<std::string>& level) {
    for (int y = 0; y < level.size(); y++) {
        for (int x = 0; x < level[y].size(); x++) {
            if (level[y][x] == val) {
                return Pos {y, x};
            }
        }
    }
    err("Character not found in level data");
}
bool done = false;
Pos calculatePos(int key, Pos curPos, const std::vector<std::string>& level) {
    Pos newPos = curPos;
    char curTile = NULL;
    Pos prevPos = newPos;
    while (true) {
        prevPos = newPos;
        switch (key) {
            case KEY_UP:
                newPos.y--;
                break;
            case KEY_DOWN:
                newPos.y++;
                break;
            case KEY_LEFT:
                newPos.x--;
                break;
            case KEY_RIGHT:
                newPos.x++;
                break;
            default: break;
        }
        curTile = level[newPos.y][newPos.x];
        if (curTile == '#') break;
        if (curTile == '@') {
            done = true;
            break;
        }
    }
    return prevPos;
}
void drawScreen(Pos playerPos, const std::vector<std::string>& level) {
    clear();
    for (int y = 0; y < level.size(); y++) {
        for (int x = 0; x < level[y].size(); x++) {
            char curTile = level[y][x];
            if (curTile == 'S') continue;
            if (curTile == '@') {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, curTile);
                attroff(COLOR_PAIR(2));
                continue;
            }
            mvaddch(y, x, curTile);
        }
    }
    attron(COLOR_PAIR(1));
    mvaddch(playerPos.y, playerPos.x, '*');
    attroff(COLOR_PAIR(1));
    refresh();
}
int main() {
    initscr();
    keypad(stdscr, true);
    curs_set(false);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    int level = 1;
    std::string levelPath = getLevelPath(level);
    while (std::experimental::filesystem::exists(levelPath)) {
        std::vector<std::string> currentLevel = readLevel(level);
        Pos playerPos = findInLevel('S', currentLevel);
        Pos endPos = findInLevel('@', currentLevel);
        done = false;
        while (!done) {
            drawScreen(playerPos, currentLevel);
            int input = getch();
            switch (input) {
                case KEY_UP:
                case KEY_DOWN:
                case KEY_LEFT:
                case KEY_RIGHT:
                    playerPos = calculatePos(input, playerPos, currentLevel);
                    break;
                case (int)'r':
                    level--;
                    done = true;
                    break;
                default:
                    break;
            }
        }
        level++;
    }
    endwin();
}

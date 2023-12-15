#include "Maze.h"
#include <iostream>
#include <fstream>

Point::Point(int a, int b)
{
    set(a, b);
}

void Point::set(int a, int b) {
    x = a;
    y = b;
}

Maze::Maze(std::string path, int w, int h)
    : width{w}, height{h}
{
    cells.reserve(w * h);
    fill_from_file(path);
}

void Maze::fill_from_file(std::string file_path)
{
    std::ifstream maze_file(file_path);
    std::string maze_string = "";
    std::string file_line = "";

    if (maze_file.is_open())
    {
        while (getline(maze_file, file_line))
           maze_string = maze_string + file_line;
        maze_file.close();
        
        if (maze_string.length() != width * height)
            throw InvalidMazeException{};
        for (int i = 0; i < height; i++)
        {
            std::vector<char> row;
            cells.push_back(row);
            for (int j = 0; j < width; j++)
                cells.at(i).push_back(maze_string[i*width+j]);
        }
    }
    else
        throw FileNotFoundException{};
}

char Maze::get(Point p) const
{
    return cells.at(p.y).at(p.x);
}

Point Maze::get_start() const
{
    Point start = {-1, -1};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (cells.at(i).at(j) == 'S')
                start = {j, i};
        }
    }
    return start;
}

Point Maze::get_exit() const
{
    Point exit = {-1, -1};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (cells.at(i).at(j) == 'E')
                exit = {j, i};
        }
    }
    return exit;
}

bool Maze::is_wall(Point p) const
{
    if (is_inside(p))
        return cells.at(p.y).at(p.x) == '*';
    return true;    // se è fuori è come se fosse muro: non ci può andare
}

bool Maze::is_exit(Point p) const
{
    if (is_inside(p))
        return cells.at(p.y).at(p.x) == 'E';
    return false;   // se è fuori non è l'uscita valida
}

bool Maze::is_inside(Point p) const
{
    return 0 <= p.x && p.x < width && 0 <= p.y && p.y < height;
}

void Maze::display() const
{
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            std::cout << cells.at(i).at(j) << ' ';
        }
        std::cout << '\n';
    }
}

void Maze::display_with_robot(const Point &r, char c) const
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << ((r.x == j && r.y == i) ? c : cells.at(i).at(j)) << ' ';
        }
        std::cout << '\n';
    }
}

bool Point::operator==(Point p)
{
    return x == p.x && y == p.y;
}

const Point& Point::operator=(const Point &p) {
    x = p.x;
    y = p.y;
    return *this;
}
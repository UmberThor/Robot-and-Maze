#ifndef MAZE_H
#define MAZE_H

#include <string>
#include <vector>

struct Point
{
  int x;
  int y;

  Point(int a = -1, int b = -1);
  Point(const Point& p);

  void set(int a, int b);

  bool operator==(Point p);
  const Point& operator=(const Point& p);
};

class Maze {
  public:
    Maze(std::string path, int w, int h);

    char get(Point p) const;
    Point get_start() const;
    Point get_exit() const;

    bool is_wall(Point p) const;
    bool is_exit(Point p) const;
    bool is_inside(Point p) const;

    void display() const;

    void display_with_robot(const Point &r, char d) const;

  private:
    int width, height;
    std::vector<std::vector<char>> cells;
    void fill_from_file(std::string file_path);

    class FileNotFoundException {};
    class InvalidMazeException {};
};

#endif

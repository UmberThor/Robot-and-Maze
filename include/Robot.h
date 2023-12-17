#ifndef ROBOT_H
#define ROBOT_H

#include "Maze.h"

class Robot {
  public:
    Robot(const Point& s);
    Robot();

    Point get_position() const;
    Point set_position(const Point& p);

    virtual void move(const Maze& maze) = 0;

    class StuckRobotException{};
  protected:
    Point current;
};

class RandomRobot : public Robot {
  public:
    RandomRobot(const Point& s);
    RandomRobot();

    void move(const Maze &maze) override;
};

class RightHandRuleRobot : public Robot {
  public:
    RightHandRuleRobot(const Point &s, char d = 'R');
    RightHandRuleRobot();

    char get_direction();

    void move(const Maze &maze) override;
  private:
    char direction; // U up, R right, D down, L left
    bool init_move_fw = false, init_rotate = false; // queste variabili verificano che il robot sia arrivato in una posizione valida per iniziare a muoversi tenendo il muro a destra
    char rotate(char dir, char offset);

    // queste cose servono per far si che il robot capisca quando è bloccato in un loop
    struct Step_record
    {
      Point cell;
      char dir;
      bool operator==(const Step_record& sr);
    };
    std::vector<Step_record> past_steps;
};

std::vector<Point> calculate_points(Point cur, char dir);
Point calculate_fw(Point cur, char dir);
Point calculate_rg(Point cur, char dir);
Point calculate_bh(Point cur, char dir);
Point calculate_lf(Point cur, char dir);

#endif

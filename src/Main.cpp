// UN MAZE ESISTE ANCHE SENZA ROBOT, MA UN ROBOT HA BISOGNO DI UN MAZE PER CAPIRE DOVE SI TROVA, COME MUOVERSI ECC
// PER QUESTO ROBOT INLUDE MAZE E LO RICEVE SPESSO COME PARAMETRO

#include <iostream>
#include <thread>
#include <chrono>
#include "Maze.h"
#include "Robot.h"

void tests(const Maze &maze);
void random_robot(const Maze &maze);
void right_robot(const Maze &maze);

int main()
{

    Maze maze("../mazes/maze3.txt", 9, 9);

    //tests(maze);
    right_robot(maze);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    random_robot(maze);

    return 0;
}

void random_robot(const Maze& maze)
{
    RandomRobot rand_robot{maze.get_start()};
    int count = 0;
    while (!maze.is_exit(rand_robot.get_position()))
    {
        system("clear");
        try
        {
            rand_robot.move(maze);
        }
        catch (Robot::StuckRobotException)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return;
        }
        maze.display_with_robot(rand_robot.get_position(), 'R');
        count++;
        std::cout << count << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
    std::cout << "Required steps: " << count << "\n";
}

void right_robot (const Maze& maze)
{
    RightHandRuleRobot right_robot{};
    right_robot.set_position(maze.get_start());
    int count = 0;
    char c = ' ';

    while (!maze.is_exit(right_robot.get_position()))
    {
        system("clear");

        try
        {
            right_robot.move(maze);
        }
        catch (Robot::StuckRobotException)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return;
        }

        switch (right_robot.get_direction())
        {
            case 'U': c = '^'; break;
            case 'R': c = '>'; break;
            case 'D': c = 'v'; break;
            case 'L': c = '<'; break;
            default: c = 'X'; break;
        }
        maze.display_with_robot(right_robot.get_position(), c);
        count++;
        std::cout << count << '\n';

        //char x;
        //std::cin >> x;

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
    std::cout << "Required steps: " << count << "\n";
}

void tests(const Maze &maze)
{
    std::cout << "SOME TESTS" << "\n";

    maze.display();

    std::cout << "S:\t" << maze.get_start().x << "," << maze.get_start().y << "\n";
    std::cout << "E:\t" << maze.get_exit().x << "," << maze.get_exit().y << "\n";
    std::cout << "[4,6]:\t" << maze.get(Point {4, 6}) << "\n";
    std::cout << "[6,8]:\t" << maze.get(Point{6, 8}) << "\n";
    std::cout << "[6,7]:\t" << maze.get(Point{6, 7}) << "\n";
    std::cout << "[6,8] wall:\t" << maze.is_wall(Point{6, 8}) << "\n";
    std::cout << "[6,7] wall:\t" << maze.is_wall(Point{6, 7}) << "\n";
    std::cout << "[2,0] exit:\t" << maze.is_exit(Point{2, 0}) << "\n";
    std::cout << "[1,0] exit:\t" << maze.is_exit(Point{1, 0}) << "\n";
}

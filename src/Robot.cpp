#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include "Robot.h"

Robot::Robot(const Point& s)
    : current{s}
{}

Robot::Robot()
    : current{Point{-1,-1}}
{}

Point Robot::get_position() const
{
    return current;
}

Point Robot::set_position(const Point &p)
{
    current = p;
}

RandomRobot::RandomRobot(const Point &s)
    : Robot{s}
{
    srand(time(NULL));
}

RandomRobot::RandomRobot()
    : Robot{}
{
    srand(time(NULL));
}


void RandomRobot::move(const Maze& maze)
{
    int dX = rand() % 3 - 1; // numero tra 0 e 2 -> -1 così diventa numero tra -1 e 1;
    int dY = rand() % 3 - 1;
    Point expected = {current.x + dX, current.y + dY};
    // deve rifare se: sta fermo, va addosso a un muro, esce dal range

    if (maze.is_wall(Point(current.x + 1, current.y)) && maze.is_wall(Point(current.x -1, current.y)) && maze.is_wall(Point(current.x, current.y+1)) && maze.is_wall(Point(current.x, current.y-1)))
    {
        maze.display_with_robot(current, 'X');
        std::cout << "I'm stuck!" << '\n';
        throw StuckRobotException{};
    }

    Point cross_x = {current.x + dX, current.y};
    Point cross_y = {current.x, current.y + dY};

    if (!maze.is_inside(expected) || expected == current || maze.is_wall(expected)) // ordine importante! se è fuori allora rifare (cortocircuito), senza controllare se è un muro
        move(maze);
    else if (maze.is_wall(cross_x) && maze.is_wall(cross_y)) // per evitare gli attraversamenti tipo :        *   ->     R*
        move(maze);                                          //                                              *R          *
    else
    {
        current.x += dX;
        current.y += dY;
        }
}

RightHandRuleRobot::RightHandRuleRobot(const Point &s, char d)
    : Robot{s}, direction{d}
{}

RightHandRuleRobot::RightHandRuleRobot()
    : Robot{}, direction{'R'}
{}

char RightHandRuleRobot::get_direction()
{
    return direction;
}

void RightHandRuleRobot::move(const Maze& maze)
{
    if (!(init_move_fw && init_rotate)) // se non ha ancora sistemato la sua condizione iniziale
    {
        Point rg = calculate_rg(current, direction);
        if (maze.is_wall(rg)) {
            init_move_fw = true;
            init_rotate = true;
            move(maze);
            //std::cout << "init completato" << "\n";
        }
        else {
            Point expected_fw = calculate_fw(current, direction);
            if (!init_move_fw)
            {
                if (!maze.is_wall(expected_fw)) // si deve muovere ancora avanti
                    current = expected_fw;
                else
                {
                    init_move_fw = true;
                    move(maze);
                }
            }
            else
            {
                direction = rotate(direction, 1);
                move(maze);
            }
        }
    }
    else // se ha gia sistemato la sua condizione iniziale
    {
        Point fw = calculate_fw(current, direction);
        Point rg = calculate_rg(current, direction);
        Point bh = calculate_bh(current, direction);
        Point lf = calculate_lf(current, direction);
        // se a destra è libero, ruota la tua direzione verso destra
        if (!maze.is_wall(rg))
            direction = rotate(direction, 1);
        // altrimenti se davanti è libero, rimani con la stessa posizione
        else if (!maze.is_wall(fw))
            ;
        // altrimenti se a sinistra è libero, ruota la tua direzione verso sinistra
        else if (!maze.is_wall(lf))
            direction = rotate(direction, 3);
        // altrimenti se dietro è libero, ruota la tua direzione verso indietro
        else if (!maze.is_wall(bh))
            direction = rotate(direction, 2);
        // altrimenti sei bloccato: errore!
        else
        {
            maze.display_with_robot(current, 'X');
            std::cout << "I can't exit!" << '\n';
            throw StuckRobotException{};
        }
        fw = calculate_fw(current, direction);
        current = fw;

        Step_record step{current, direction};
        past_steps.push_back(step);
        for (int i = 0; i < past_steps.size() - 1; i++)
        {
            if (step == past_steps[i])
            {
                maze.display_with_robot(current, 'X');
                std::cout << "I'm stuck in a loop!" << '\n';
                throw StuckRobotException{};
            }
        }
    }
}

char RightHandRuleRobot::rotate(char dir, char offset)
{
    char directions[] = {'U', 'R', 'D', 'L'};
    int dir_i = -1;
    for (int i = 0; i < 4; i++) {
        if (dir == directions[i])
            dir_i = i;
    }
    if (dir_i != -1)
        return directions[(dir_i + offset) % 4];
    else
        return 'X';
}

std::vector<Point> calculate_points(Point cur, char dir)
{
    // relativamente alla sua direzione:
    Point rg{}; // cella che gli sta a destra
    Point lf{}; // cella che gli sta a sinistra
    Point fw{}; // cella che gli sta davanti
    Point bh{}; // cella che gli sta dietro
    switch (dir)
    {
    case 'U': // up
        rg.set(cur.x + 1, cur.y);
        lf.set(cur.x - 1, cur.y);
        fw.set(cur.x, cur.y - 1);
        bh.set(cur.x, cur.y + 1);
        break;
    case 'R': // right
        rg.set(cur.x, cur.y + 1);
        lf.set(cur.x, cur.y - 1);
        fw.set(cur.x + 1, cur.y);
        bh.set(cur.x - 1, cur.y);
        break;
    case 'D': // down
        rg.set(cur.x - 1, cur.y);
        lf.set(cur.x + 1, cur.y);
        fw.set(cur.x, cur.y + 1);
        bh.set(cur.x, cur.y - 1);
        break;
    case 'L': // left
        rg.set(cur.x, cur.y - 1);
        lf.set(cur.x, cur.y + 1);
        fw.set(cur.x - 1, cur.y);
        bh.set(cur.x + 1, cur.y);
        break;
    default:
        break;
    }
    std::vector<Point> points = {fw, rg, bh, lf};
    return points;
}

Point calculate_fw(Point cur, char d) {
    return calculate_points(cur, d)[0];
}

Point calculate_rg(Point cur, char d)
{
    return calculate_points(cur, d)[1];
}

Point calculate_bh(Point cur, char d)
{
    return calculate_points(cur, d)[2];
}

Point calculate_lf(Point cur, char d)
{
    return calculate_points(cur, d)[3];
}

bool RightHandRuleRobot::Step_record::operator==(const Step_record &sr)
{
    return cell == sr.cell && dir == sr.dir;
}
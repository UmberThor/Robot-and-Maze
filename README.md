# Progetto Labirinto-Robot

Questo progetto implementa un simulatore di labirinto e due tipi di robot che cercano di risolverlo. Il codice è scritto in C++ e comprende diverse classi per rappresentare il labirinto, il robot e le eccezioni correlate.

## Contenuti

- [**Robot.cpp**](Robot.cpp): Implementazione delle classi `Robot`, `RandomRobot`, e `RightHandRuleRobot`.
- [**Maze.cpp**](Maze.cpp): Implementazione della classe `Maze`.
- [**Main.cpp**](Main.cpp): Funzioni principali per eseguire test e simulazioni.
- [**Robot.h**](Robot.h): Dichiarazioni delle classi e delle funzioni relative ai robot.
- [**Maze.h**](Maze.h): Dichiarazione della classe `Maze` e la struttura `Point`.

## Uso

Il progetto simula il movimento di due tipi di robot all'interno di un labirinto. Il labirinto viene caricato da un file di testo e i robot cercano di raggiungere l'uscita seguendo diverse strategie.

Per eseguire il programma, compila i file sorgente utilizzando CMake:

```bash
/build$ cmake ..
/build$ cmake --build .
```
Esegui quindi il programma con
```bash
/build$ ./SolveMaze
```

## Descrizione delle Classi

### `Robot`

La classe `Robot` rappresenta la base per i robot nel simulatore del labirinto. Ogni robot ha una posizione corrente nel labirinto.

#### Metodi

- `Robot(const Point& s)`: Costruttore che inizializza il robot con una posizione specifica.
- `Robot()`: Costruttore di default.
- `Point get_position() const`: Restituisce la posizione corrente del robot.
- `Point set_position(const Point& p)`: Imposta la posizione del robot.
- `virtual void move(const Maze& maze) = 0`: Metodo virtuale puro che rappresenta il movimento del robot nel labirinto.
- `class StuckRobotException{}`: Eccezione che viene lanciata quando il robot si blocca nel labirinto.

### `RandomRobot`

La classe `RandomRobot` è una derivata della classe `Robot` e rappresenta un robot che si muove in modo casuale nel labirinto.

#### Metodi

- `RandomRobot(const Point& s)`: Costruttore che inizializza il robot con una posizione specifica e imposta il seed del generatore casuale.
- `RandomRobot()`: Costruttore di default.
- `void move(const Maze &maze) override`: Implementazione del movimento casuale del robot.

### `RightHandRuleRobot`

La classe `RightHandRuleRobot` è una derivata della classe `Robot` e rappresenta un robot che segue la regola "tieni il muro sulla destra" nel labirinto.

#### Metodi

- `RightHandRuleRobot(const Point &s, char d = 'R')`: Costruttore che inizializza il robot con una posizione specifica e una direzione di default ('R' per destra).
- `RightHandRuleRobot()`: Costruttore di default.
- `char get_direction()`: Restituisce la direzione corrente del robot.
- `void move(const Maze &maze) override`: Implementazione del movimento del robot seguendo la regola "tieni il muro sulla destra".
- `char rotate(char dir, char offset)`: Ruota la direzione del robot.
- `struct Step_record`: Struttura che registra i passi effettuati dal robot.

### `Maze`

La classe `Maze` rappresenta il labirinto nel simulatore, caricando il layout da un file di testo.

#### Metodi

- `Maze(std::string path, int w, int h)`: Costruttore che inizializza il labirinto leggendolo da un file di testo.
- `char get(Point p) const`: Restituisce il contenuto della cella del labirinto alla posizione specificata.
- `Point get_start() const`: Restituisce la posizione di partenza nel labirinto.
- `Point get_exit() const`: Restituisce la posizione di uscita nel labirinto.
- `bool is_wall(Point p) const`: Verifica se la cella nel labirinto alla posizione specificata è un muro.
- `bool is_exit(Point p) const`: Verifica se la cella nel labirinto alla posizione specificata è l'uscita.
- `bool is_inside(Point p) const`: Verifica se la posizione specificata è all'interno dei limiti del labirinto.
- `void display() const`: Visualizza il layout del labirinto.
- `void display_with_robot(const Point &r, char d) const`: Visualizza il layout del labirinto con la posizione del robot.

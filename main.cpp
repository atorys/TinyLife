//
// Created by atory on 26.03.2022.
//

#include <iostream>
#include <ctime>
#include <unistd.h>

#define WIDTH	100
#define HEIGHT	25
#define ALIVE	"⬣ "
#define DEAD	"⎔ "

#define	COLOR_5		"\033[38;2;218;217;244m\033[48;2;30;30;30m"
#define	COLOR_3		"\033[38;2;107;183;186m\033[48;2;30;30;30m"
#define	COLOR_2		"\033[38;2;135;102;215m\033[48;2;30;30;30m"
#define	COLOR_DEAD	"\033[38;2;25;25;25m\033[48;2;30;30;30m"

typedef struct s_cell	{
	bool			alive;
	int				neighbors;
	bool 			changed_condition;
	std::string		color;
} cell;

void	getMap(cell cellsMap[HEIGHT][WIDTH]);
void	startLife();
bool	getLifeConditions(cell cellsMap[HEIGHT][WIDTH]);
bool	is_alive(const cell& current);
void	evolution(cell cellsMap[HEIGHT][WIDTH]);
int		getNeighbors(cell cellsMap[HEIGHT][WIDTH], int i, int j);
int		offset(int coordinate, int maxVal, int minVal, int off);
void	output(cell cellsMap[HEIGHT][WIDTH]);


int main()
{
	startLife();
	return 0;
}

//--Life-Cycle-----------------------------------------------------------------
//
// Life continues as long as at least one cell is alive AND the previous map
// configuration doesn't match the current one
//
// A cell is born if it has 3 living neighbors, it stays alive if it has 2 or 3
// living neighbors
// Otherwise a cell dies from loneliness or overpopulation
//
//-----------------------------------------------------------------------------
void	startLife()
{
	cell	cellsMap[HEIGHT][WIDTH];

	srand(time(0));

	getMap(cellsMap);
	while (getLifeConditions(cellsMap)) {
		system("clear");
		evolution(cellsMap);
		output(cellsMap);
		usleep(200000);
	}
}


bool	is_alive(const cell& current) {
	return (!current.alive && current.neighbors == 3) ||
			(current.alive && (current.neighbors == 3 || current.neighbors == 2));
}

bool	getLifeConditions(cell cellsMap[HEIGHT][WIDTH])
{
	bool	all_alive = false;
	bool	all_changed = false;

	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j) {
			if (cellsMap[i][j].alive)
				all_alive = true;
			if (cellsMap[i][j].changed_condition)
				all_changed = true;
		}

	return all_alive && all_changed;
}

void	evolution(cell cellsMap[HEIGHT][WIDTH])
{
	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j)
			cellsMap[i][j].neighbors = getNeighbors(cellsMap, i, j);

	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j) {
			if (cellsMap[i][j].alive != is_alive(cellsMap[i][j])) {
				cellsMap[i][j].alive = !cellsMap[i][j].alive;
				cellsMap[i][j].changed_condition = true;
			}
			else
				cellsMap[i][j].changed_condition = false;
		}

	int count;
	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j)
		{
			count = -1;
			if (cellsMap[i][j].alive)
				count = getNeighbors(cellsMap, i, j);
			(count > 5) ? cellsMap[i][j].color = COLOR_5 :
			(count > 3) ? cellsMap[i][j].color = COLOR_3 :
			(count >= 0) ? cellsMap[i][j].color = COLOR_2 :
						  cellsMap[i][j].color = COLOR_DEAD ;
		}
}

int	offset(int coordinate, int maxVal, int minVal, int off) {
	return (coordinate + off < minVal) ? (maxVal + off) :
		   (coordinate + off >= maxVal) ? (minVal) : coordinate + off;
}

int	getNeighbors(cell cellsMap[HEIGHT][WIDTH], int i, int j)
{
	return (cellsMap[offset(i, HEIGHT, 0, +1)][j].alive
			+ cellsMap[offset(i, HEIGHT, 0, +1)][offset(j, WIDTH, 0, +1)].alive
			+ cellsMap[offset(i, HEIGHT, 0, +1)][offset(j, WIDTH, 0, -1)].alive
			+ cellsMap[offset(i, HEIGHT, 0, -1)][j].alive
			+ cellsMap[offset(i, HEIGHT, 0, -1)][offset(j, WIDTH, 0, +1)].alive
			+ cellsMap[offset(i, HEIGHT, 0, -1)][offset(j, WIDTH, 0, -1)].alive
			+ cellsMap[i][offset(j, WIDTH, 0, +1)].alive
			+ cellsMap[i][offset(j, WIDTH, 0, -1)].alive);
}

void	output(cell cellsMap[HEIGHT][WIDTH])
{
	std::string		out;

	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j) {
			out += cellsMap[i][j].color;
			if (cellsMap[i][j].alive)
				out += ALIVE;
			else
				out += DEAD;
			if (j == WIDTH - 1)
				out += "\n";
		}
	out += "\n";
	std::cout << out;
}

void	getMap(cell cellsMap[HEIGHT][WIDTH])
{
//	std::string answer;
//	std::cout << "Fill map randomly? (yes/no)\n";
//	std::cin >> answer;
//	if (answer.empty() || answer == "yes") {
	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j) {
			cellsMap[i][j].alive = rand() % 5 == 0;
			cellsMap[i][j].changed_condition = true;
		}
//	}
}

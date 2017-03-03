#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	int	x = 0,
		y = 0,
		maxX = 0,
		maxY = 0,
		nextX = 0,
		nextY = 0,
		dirX = 1,
		dirY = 1;
	
	initscr();
	noecho();
	curs_set(FALSE);
	
	getmaxyx(stdscr, maxY, maxX);
	
	while(true) {
		clear();
		usleep(15000);
		mvprintw(y,x, "@");
		refresh();
		nextX = x + dirX;
		nextY = y + dirY;
		
		if(nextX >= maxX || nextX < 0)
			dirX *= -1;
		
		else
			x += dirX;
		
		if(nextY >= maxY || nextY < 0)
			dirY *= -1;
		
		else
			y += dirY;
		
	}
	
	endwin();
	return 0;
}

		
		

#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int	WINSIZE_CHANGE	=	0;
int	PADDLE_SIZE	=	6;
	
// Window resize callback
void winResize(int dummyvar) {
	
	// Set flag to indicate window size changed
	WINSIZE_CHANGE = 1;
	
	struct winsize size;
	if(ioctl(fileno(stdout), TIOCGWINSZ, &size) == 0) {
		resize_term(size.ws_row, size.ws_col);
	}

}

void drawPaddle(int ypos, int xpos) {
	for(int i=0; i<=PADDLE_SIZE; i++)
		mvprintw(ypos+i, xpos, "|");
}



int main(int argc, char** argv) {

	// position, screen boundaries, direction constants
	int	x = 11,
		y = 11,
		maxX = 0,
		maxY = 0,
		nextX = 0,
		nextY = 0,
		dirX = 1,
		dirY = 1,
		padX = 10,
		padY = 10;
	char	kbinput;
		
	
	// Initialize screen
	// Use non-blocking getch()
	WINDOW* w = initscr();
	cbreak();
	nodelay(w, TRUE);
	noecho();
	curs_set(FALSE);
	
	// Get max Y and X values for terminal
	getmaxyx(stdscr, maxY, maxX);
	
	// Set up signal handler in case user resizes terminal
	signal(SIGWINCH, winResize);
	
	while(true) {

		// Our window size has changed
		// TODO: Recalculate relative positions
		if(WINSIZE_CHANGE) {
			sleep(1);
			getmaxyx(stdscr, maxY, maxX);
			x = maxX/2;
			y = maxY/2;
			WINSIZE_CHANGE = 0;
		}			
			
		// Wait, then redraw at new position
		usleep(30000);
		clear();
		printw("Window Size Y=%d X=%d\n", maxY, maxX);
		printw("Position Y=%d X=%d\n", y, x);
		mvprintw(y,x, "@");
		kbinput = getch();

		// Very basic keyboard handling
		// Use '['  and   ']' to move paddle
		if(kbinput != ERR) {
			
			if(kbinput == '[') 
				padY -= 2;
			if(kbinput == ']') 
				padY += 2;

		}

		drawPaddle(padY,padX);
		refresh();

		// Handle bounce off of the walls
		nextX = x + dirX;
		nextY = y + dirY;
		
		// Very basic paddle bounce
		if(	nextX	== 10	&&
			dirX	== -1	&&
			padY	<= y	&&
			padY+6	>= y
		)
			dirX *= -1;
		
		// Hit X boundary, change direction
		if(nextX >= maxX || nextX < 0)
			dirX *= -1;		
		else
			x += dirX;
		
		// Hit Y boundary, change direction
		if(nextY >= maxY || nextY < 0)
			dirY *= -1;
		else
			y += dirY;
	}
	
	endwin();
	return 0;

}

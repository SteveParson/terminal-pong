#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <math.h>

int WINSIZE_CHANGE = 0;
int PADDLE_SIZE = 6;

// Window resize callback
void winResize(int dummyvar) {
    // Set flag to indicate window size changed
    WINSIZE_CHANGE = 1;

    // Update terminal data so that getmaxyx uses the
    // right data
    struct winsize size;
    if(ioctl(fileno(stdout), TIOCGWINSZ, &size) == 0) {
        resize_term(size.ws_row, size.ws_col);
    }
}

// Draw the paddle
void drawPaddle(int ypos, int xpos) {
    for(int i = 0; i <= PADDLE_SIZE; i++) {
        mvprintw(ypos + i, xpos, "*");
    }
}

int main(int argc, char **argv) {
    // position, screen boundaries, direction constants
    int x     = 11, y     = 11,
        maxX  = 0,  maxY = 0,
        nextX = 0,  nextY = 0,
        dirX  = 1,  dirY  = 1,
        padX  = 10, padY  = 10;
    
    double relposbX = 0,
           relpospY = 0,
           relposbY = 0;
    
    char kbinput;

    // Initialize screen
    // Use non-blocking getch()
    WINDOW *w = initscr();
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
        // Recalculate relative positions
        if(WINSIZE_CHANGE) {
            usleep(1500000);
            getmaxyx(stdscr, maxY, maxX);
            
            x    = round(maxX * relposbX);
            y    = round(maxY * relposbY);
            padY = round(maxY * relpospY);
            
            WINSIZE_CHANGE = 0;
        } else {
            // Wait, then redraw at new position
            usleep(50000);
            clear();
            printw("[ and ] to move paddle.  Q to quit\n");

            mvprintw(y, x, "O");
            kbinput = getch();

            // Very basic keyboard handling
            // Use '['  and   ']' to move paddle
            if(kbinput != ERR) {
                
                if(kbinput == '[' && padY >= 2) {
                    padY -= 2;
                }
                
                if(kbinput == ']'
                   && padY < (maxY - PADDLE_SIZE - 2)) {
                    
                    padY += 2;
                }
                
                if(kbinput == 'q' || kbinput == 'Q') {
                    endwin();
                    printf("Thanks for playing!\n");
                    return 0;
                }
            }

            drawPaddle(padY, padX);
            refresh();

            // Handle bounce off of the walls
            nextX = x + dirX;
            nextY = y + dirY;

            // Very basic paddle bounce
            if(nextX == 10
               && dirX == -1
               && padY <= nextY
               && (padY + PADDLE_SIZE) >= nextY) {
                dirX *= -1;
            }
            
            if(nextX == 9
               && dirX == 1
               && padY <= nextY
               && (padY + PADDLE_SIZE) >= nextY) {
                    dirX *= -1;
            }

            // Hit X boundary, change direction
            if(nextX >= maxX || nextX < 0) {
                dirX *= -1;
            }
            else {
                x += dirX;
            }

            // Hit Y boundary, change direction
            if(nextY >= maxY || nextY < 0) {
                dirY *= -1;
            }
            else {
                y += dirY;
            }

            if(maxX > 0 && maxY > 0) {
                relposbX = (double) x / maxX;
                relposbY = (double) y / maxY;
                relpospY = (double) padY / maxY;
            }
        }
    }

    endwin();
    return 0;
}

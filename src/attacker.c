// Standard headers
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "attacker.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

static position_t get_position_from_string(char string[100]){
    
    char delim[2];
    delim[0] = ' ';
    delim[1] = 0;
    position_t result;

    char* token = strtok(string, delim);

    result.i = atoi(token);
    token = strtok(0, delim);
    result.j = atoi(token);

    return result;
}

static direction_t get_direction_from_string(char string[100]){
    
    char delim[2];
    delim[0] = ' ';
    delim[1] = 0;
    direction_t result;

    char* token = strtok(string, delim);

    result.i = atoi(token);
    token = strtok(0, delim);
    result.j = atoi(token);

    return result;
}

static int get_last_line_turno() {
	FILE *fd;                           // File pointer
	char filename[] = "round_counter_attacker.txt";       // file to read
	static const long max_len = 5;  // define the max length of the line to read
	char buff[max_len + 1];             // define the buffer and allocate the length

	if ((fd = fopen(filename, "r")) != NULL)  {      // open file. I omit error checks

		while( fgets(buff, sizeof(buff), fd)!=NULL );
		fclose(fd);
    	return atoi(buff);

	}
	return 0;
}

static int get_last_turno(int turno, position_t current_position, position_t last_position){
    if (abs((int)current_position.i - (int)last_position.i) > 2 || abs((int)current_position.j - (int)last_position.j) > 2){
        return 0;
    }else{
        return turno;
    }
}

position_t get_last_attacker_position(){
    FILE *fd;                           // File pointer
	char filename[] = "attacker_last_position.txt";       // file to read
	static const long max_len = 100;  // define the max length of the line to read
	char buff[max_len + 1];             // define the buffer and allocate the length

	if ((fd = fopen(filename, "r")) != NULL)  {      // open file. I omit error checks

		while( fgets(buff, sizeof(buff), fd)!=NULL );
		fclose(fd);
    	return get_position_from_string(buff);
	}
	return (position_t) {0, 0};
}

static void save_defender_position(position_t defender_position){
    FILE *fd;                           // File pointer
	char filename[] = "last_defender_position_spy.txt";       // file to read
    fd = fopen(filename, "a+");
    fprintf(fd, "%ld %ld\n", defender_position.i, defender_position.j);
    fclose(fd);
}


position_t get_defender_position(){
    FILE *fd;                           // File pointer
	char filename[] = "last_defender_position_spy.txt";       // file to read
	static const long max_len = 100;  // define the max length of the line to read
	char buff[max_len + 1];             // define the buffer and allocate the length

	if ((fd = fopen(filename, "r")) != NULL)  {      // open file. I omit error checks

		while( fgets(buff, sizeof(buff), fd)!=NULL );
		fclose(fd);
    	return get_position_from_string(buff);

	}
	return (position_t) {0, 0};
}


static void save_turno(int turno){
    FILE *fd;                           // File pointer
	char filename[] = "round_counter_attacker.txt";       // file to read
    fd = fopen(filename, "a+");
    fprintf(fd, "%d\n", turno);
    fclose(fd);

}

static direction_t get_last_move(int made_move){
    if(!made_move)
        return (direction_t) {0, 0};
    FILE *fd;                           // File pointer
	char filename[] = "last_attacker_move.txt";       // file to read
	static const long max_len = 100;  // define the max length of the line to read
	char buff[max_len + 1];             // define the buffer and allocate the length

	if ((fd = fopen(filename, "r")) != NULL)  {      // open file. I omit error checks

		while( fgets(buff, sizeof(buff), fd)!=NULL );
		fclose(fd);
    	return get_direction_from_string(buff);

	}
	return (direction_t) {0, 0};
}

void save_last_move(direction_t move){
    FILE *fd;                           // File pointer
	char filename[] = "last_attacker_move.txt";       // file to read
    fd = fopen(filename, "a+");
    fprintf(fd, "%d %d\n", move.i, move.j);
    fclose(fd);
}

static int made_move_last_turn(position_t current_position){
    position_t last_position = get_last_attacker_position();
    if (current_position.i != last_position.i || current_position.j != last_position.j){
        return 1;
    } else {
        return 0;
    }
}

static void save_attacker_position(position_t attacker_position){
    FILE *fd;                           // File pointer
	char filename[] = "attacker_last_position.txt";       // file to read
    fd = fopen(filename, "a+");
    fprintf(fd, "%ld %ld\n", attacker_position.i, attacker_position.j);
    fclose(fd);
}

int compare_direction(direction_t a, direction_t b){
    if (a.i == b.i && a.j == b.j){
        return 1;
    }
    return 0;
}

direction_t decide_direction_attacker(int turno, int made_move, position_t attacker_position, position_t defender_position, direction_t last_move){
    //printf("Decisão: turno:%d, made_move:%d, last_move.i:%d, last_move.j:%d, diferença.i:%d\n", turno, made_move, last_move.i, last_move.j, abs((int)attacker_position.i - (int)defender_position.i));
    if (turno < 7){
        return (direction_t) {0, 0};
    }
    else if (turno < 12){
        if (made_move && compare_direction(last_move, (direction_t) DIR_UP)){
            return (direction_t) DIR_UP;
        }else if (made_move && compare_direction(last_move, (direction_t) DIR_RIGHT)) {
            return (direction_t) DIR_RIGHT;
        }else if (!made_move && compare_direction(last_move, (direction_t) DIR_UP)){
            return (direction_t) DIR_RIGHT;
        }else if (!made_move && compare_direction(last_move, (direction_t) DIR_RIGHT)){
            return (direction_t) DIR_UP;
        } else {
            return (direction_t) DIR_UP;
        }
    }
    else if (turno >= 12){
        if (abs((int)attacker_position.i - (int)defender_position.i) <= 3){
            if ((made_move && compare_direction(last_move, (direction_t) DIR_DOWN)) || turno == 12){
                return (direction_t) DIR_DOWN;
            }else if (made_move && compare_direction(last_move, (direction_t) DIR_RIGHT)) {
                return (direction_t) DIR_RIGHT;
            }else if (!made_move && compare_direction(last_move, (direction_t) DIR_DOWN)){
                return (direction_t) DIR_RIGHT;
            }else if (!made_move && compare_direction(last_move, (direction_t) DIR_RIGHT)){
                return (direction_t) DIR_DOWN;
            } else {
                return (direction_t) DIR_DOWN;
            }
        } else {
            if (made_move && compare_direction(last_move, (direction_t) DIR_UP)){
                return (direction_t) DIR_UP;
            }else if (made_move && compare_direction(last_move, (direction_t) DIR_RIGHT)) {
                return (direction_t) DIR_RIGHT;
            }else if (!made_move && compare_direction(last_move, (direction_t) DIR_UP)){
                return (direction_t) DIR_RIGHT;
            }else if (!made_move && compare_direction(last_move, (direction_t) DIR_RIGHT)){
                return (direction_t) DIR_UP;
            } else {
                return (direction_t) DIR_RIGHT;
            }
        }
    }
    return (direction_t) {0, 0};
}


direction_t execute_attacker_strategy(
    position_t attacker_position, Spy defender_spy) {
    int turno = get_last_line_turno();
    position_t last_position = get_last_attacker_position(attacker_position);
    turno = get_last_turno(turno, attacker_position, last_position);
    int made_move = made_move_last_turn(attacker_position);
    direction_t movement_direction;
    direction_t last_move = get_last_move(made_move);
    position_t defender_position;
    turno++;
    
    if (turno == 12){
        defender_position = get_spy_position(defender_spy);
        save_defender_position(defender_position);
    } else if (turno > 12){
        defender_position = get_defender_position();
    } else {
        defender_position = (position_t) {0, 0};
    }
    
    movement_direction = decide_direction_attacker(turno, made_move, attacker_position, defender_position, last_move);
    save_last_move(movement_direction);
    save_attacker_position(attacker_position);
    save_turno(turno);

    return movement_direction;
}

/*----------------------------------------------------------------------------*/

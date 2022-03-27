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
#include "defender.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

position_t get_position_from_string(char string[100]){
    
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

int get_last_line_turno() {
	FILE *fd;                           // File pointer
	char filename[] = "round_counter_defender.txt";       // file to read
	static const long max_len = 5;  // define the max length of the line to read
	char buff[max_len + 1];             // define the buffer and allocate the length

	if ((fd = fopen(filename, "r")) != NULL)  {      // open file. I omit error checks

		while( fgets(buff, sizeof(buff), fd)!=NULL );
		fclose(fd);
    	return atoi(buff);

	}
	return 0;
}

int get_last_turno(int turno, position_t current_position, position_t last_position){
    if (abs((int)current_position.i - (int)last_position.i) > 2 || abs((int)current_position.j - (int)last_position.j) > 2){
        return 0;
    }else{
        return turno;
    }
}

position_t get_last_defender_position(){
    FILE *fd;                           // File pointer
	char filename[] = "defender_last_position.txt";       // file to read
	static const long max_len = 100;  // define the max length of the line to read
	char buff[max_len + 1];             // define the buffer and allocate the length

	if ((fd = fopen(filename, "r")) != NULL)  {      // open file. I omit error checks

		while( fgets(buff, sizeof(buff), fd)!=NULL );
		fclose(fd);
    	return get_position_from_string(buff);

	}
	return (position_t) {0, 0};
}

void save_attacker_position(position_t attacker_position){
    FILE *fd;                           // File pointer
	char filename[] = "last_attacker_position_spy.txt";       // file to read
    fd = fopen(filename, "a+");
    fprintf(fd, "%ld %ld\n", attacker_position.i, attacker_position.j);
    fclose(fd);
}


position_t get_attacker_position(){
    FILE *fd;                           // File pointer
	char filename[] = "last_attacker_position_spy.txt";       // file to read
	static const long max_len = 100;  // define the max length of the line to read
	char buff[max_len + 1];             // define the buffer and allocate the length

	if ((fd = fopen(filename, "r")) != NULL)  {      // open file. I omit error checks

		while( fgets(buff, sizeof(buff), fd)!=NULL );
		fclose(fd);
    	return get_position_from_string(buff);

	}
	return (position_t) {0, 0};
}


void save_turno(int turno){
    FILE *fd;                           // File pointer
	char filename[] = "round_counter_defender.txt";       // file to read
    fd = fopen(filename, "a+");
    fprintf(fd, "%d\n", turno);
    fclose(fd);

}

int made_move_last_turn(position_t current_position){
    position_t last_position = get_last_defender_position();
    if (current_position.i != last_position.i || current_position.j != last_position.j){
        return 1;
    } else {
        return 0;
    }
}

void save_defender_position(position_t defender_position){
    FILE *fd;                           // File pointer
	char filename[] = "defender_last_position.txt";       // file to read
    fd = fopen(filename, "a+");
    fprintf(fd, "%ld %ld\n", defender_position.i, defender_position.j);
    fclose(fd);
}

direction_t decide_direction_defender(int turno, int made_move, position_t attacker_position, position_t defender_position){
    if (turno < 8){
        return (direction_t) {0, 0};
    }
    else if (turno >= 8){
        if (made_move || turno == 8){
            if (attacker_position.i > defender_position.i){
                return (direction_t) DIR_DOWN;
            }else {
                return (direction_t) DIR_UP;
            }
        }else {
            return (direction_t) {0, 0};
        }
    }
    return (direction_t) {0, 0};
}


direction_t execute_defender_strategy(
    position_t defender_position, Spy attacker_spy) {
  	
	int turno = get_last_line_turno();
    position_t last_position = get_last_defender_position(defender_position);
    turno = get_last_turno(turno, defender_position, last_position);
    
    direction_t movement_direction;
    position_t attacker_position;
    turno++;
    
    if (turno == 8){
        attacker_position = get_spy_position(attacker_spy);
        save_attacker_position(attacker_position);
    } else if (turno > 8){
        attacker_position = get_attacker_position();
    } else {
        attacker_position = (position_t) {0, 0};
    }
    int made_move = made_move_last_turn(defender_position);
    movement_direction = decide_direction_defender(turno, made_move, attacker_position, defender_position);

    save_defender_position(defender_position);
    save_turno(turno);

    return movement_direction;
}

/*----------------------------------------------------------------------------*/

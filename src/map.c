/**
 * @author Yasser Faress (yasser.faress @ ecole.ensicaen.fr)
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/memory.h"
#include "../include/map.h"

/**
 * @brief This function creates a map for the simulation. For now it isn't random.
 * @param map a pointer to the map shared memory
 */
void create_map(map_t* map){
    map->rows = map->columns = 7;
    for (int i = 0; i < 7; i++){
        for (int j = 0; j < 7; j++){
            map->cells[i][j].type = WASTELAND;
            map->cells[i][j].row = i;
            map->cells[i][j].column = j;
        }
    }
    // Residential Buildings
    map->cells[1][0].type = map->cells[1][5].type = map->cells[2][1].type = map->cells[2][3].type = map->cells[2][6].type = map->cells[4][1].type = map->cells[4][6].type =  map->cells[5][4].type = map->cells[6][0].type = map->cells[6][4].type = map->cells[6][6].type = RESIDENTIAL_BUILDING;
    map->cells[1][0].nb_of_characters = map->cells[1][5].nb_of_characters = map->cells[2][1].nb_of_characters = map->cells[2][3].nb_of_characters = map->cells[2][6].nb_of_characters = map->cells[4][1].nb_of_characters = map->cells[4][6].nb_of_characters =  map->cells[5][4].nb_of_characters = map->cells[6][0].nb_of_characters = map->cells[6][4].nb_of_characters = map->cells[6][6].nb_of_characters = 15;
    
    // Companies
    map->cells[0][1].type = map->cells[0][3].type = map->cells[0][6].type = map->cells[3][4].type = map->cells[4][2].type = map->cells[5][2].type = map->cells[5][6].type = map->cells[6][1].type = COMPANY;
    map->cells[0][1].nb_of_characters = map->cells[0][3].nb_of_characters = map->cells[0][6].nb_of_characters = map->cells[3][4].nb_of_characters = map->cells[4][2].nb_of_characters = map->cells[5][2].nb_of_characters = map->cells[5][6].nb_of_characters = map->cells[6][1].nb_of_characters = 50;

    // Supermarkets
    map->cells[1][2].type = map->cells[4][3].type = SUPERMARKET;
    map->cells[1][2].nb_of_characters = map->cells[4][3].nb_of_characters = 30;

    // City Hall
    map->cells[3][3].type = CITY_HALL;
    map->cells[3][3].nb_of_characters = 20;
}

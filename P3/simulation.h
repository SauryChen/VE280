#ifndef __SIMULATION_H__
#define __SIMULATION_H__
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cassert>

#include "world_type.h"

using namespace std;

bool initWorld(world_t &world, const string &speciesFile, const string &creaturesFile, species_t (&species)[MAXSPECIES], creature_t (&creature)[MAXCREATURES]);
// MODIFIES: world
// EFFECTS: Initialize "world" given the species summary file "speciesFile" and the world description file
//          "creaturesFile". This initializes all the components of "world".
//          Returns true is initialization is successful.

bool load_species(const string &speciesFile, const string &worldFile, species_t (&species)[MAXSPECIES]);
// EFFECTS: Read all the species from the "species" file mentioned in world file.

void printGrid(const grid_t &grid);
// MODIFIES: cout.
// EFFECTS: print a grid representation of the creature world.

point_t adjacentPoint (point_t pt, direction_t dir);
// EFFECTS: Returns a point that results from moving squares in the direction "dir" from the point "pt".

direction_t leftFrom(direction_t dir);
// EFFECTS: Returns the direction that results from turning left from the given direction "dir".

direction_t rightFrom(direction_t dir);
// EFFECTS: Returns the direction that results from turning right from the given direction "dir".

instruction_t getInstruction(const creature_t &creature);
// EFFECTS: Returns the current instruction of "creature".

void hop(creature_t &creature, grid_t &grid);
// EFFECTS: The creature moves forward as long as the square it is facing is empty.
//          If moving forward would put the creature outside the boundaries of the grid or would cause it to land
//          on top of another creature, the hop instruction does nothing.

void left(creature_t &creature);
// EFFECTS: The creature turns left 90 degrees to face in a new direction;

void right(creature_t &creature);
// EFFECTS: The creature turns right 90 degrees to face in a new direction;

void infect (creature_t &creature_come, grid_t &grid);
// EFFECTS: Occupied creature will become the same species as the come creature if they are different.
//          Only changes the program and program ID starts from 1.
//          If the square immediately in fromt of the come_creature is empty, outside the grid, or occupied by a creature of the same species,
//          the instruction does nothing.

void ifempty(creature_t &creature, grid_t &grid);
// EFFECTS: If the square in front of the creature is inside the grid boundary and unoccupied, jump to step n of the program.
//          Otherwise, go on with the next instruction in sequence.

void ifwall(creature_t &creature, grid_t &grid);
// EFFECTS: If the creature is facing the border of the grid, jump to step n of the program;
//          otherwise, go on with the next instruction in sequence.

void ifsame(creature_t &creature, grid_t &grid);
// EFFECTS: If the square the creature is facing is occupied by a creature of the same species,
//          jump to step n; otherwise, go on with the next instruction.

void ifenemy(creature_t &creature, grid_t &grid);
// EFFECTS: If the square the creature is facing is occupied by a creature of an enemy species,
//          jump to step n; otherwise, go on with the next instruction.

void go(creature_t &creature);
// EFFECTS: This instruction always jumps to step n, independent of any condition.

void simulateCreature(creature_t &creature, grid_t &grid, bool verbose);
// REQUIRES: creature is inside the grid.
// MODIFIES: creture, grid, cout.
// EFFECTS: simulate one turn of "creature" and update the creature, the infected creature and the grid if necessary.
//          The creature program ID is always updated. The function also prints to the stdout the procedure.
//          If verbose is true, it prints more information.

#endif


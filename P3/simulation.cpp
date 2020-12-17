#include "simulation.h"

using namespace std;

bool load_species(const string &speciesFile, const string &worldFile, species_t (&species)[MAXSPECIES]){
// EFFECTS: Read all the species and their instructions in creature/x direction mentioned in a world file.
    ifstream iFile_species_summary;
    ifstream iFile_single_species;
    ifstream iFile_creature_in_world;

    string directory; // the folder contains all the species
    string species_name; // the name of each species
    string species_name_line; // the line containing the name, direction, location of a species.
    unsigned int species_number = 0; // count the number of species
    unsigned int instruction_number = 0; //count the instruction number in each species file
    istringstream read_instructions; // read instructions in each species file and separate them
    string instructions;
    string operation;
    unsigned int address;

    iFile_creature_in_world.open(worldFile);
    if(!iFile_creature_in_world){
        cout << "Error: Cannot open file " << worldFile << "!" << endl;
        return false;
    }

    iFile_species_summary.open(speciesFile);
    if(!iFile_species_summary){
        cout << "Error: Cannot open file " << speciesFile << "!" << endl;
        return false;
    }
    else{
        getline(iFile_species_summary,directory);
        string useless_row;
        string useless_coloum;
        getline(iFile_creature_in_world, useless_row);
        getline(iFile_creature_in_world, useless_coloum);

        string read_species_in_world;
        string species_in_world[MAXSPECIES];
        for (unsigned int i = 0; i < MAXSPECIES; i++){
            getline(iFile_species_summary,read_species_in_world);
            species_in_world[i] = read_species_in_world;
        }

        while(getline(iFile_creature_in_world,species_name_line)){
            istringstream species_in_world_file;
            species_in_world_file.str(species_name_line);
            species_in_world_file >> species_name;

            bool judge = false;
            for (unsigned int i = 0; i < MAXSPECIES; i++){
                if (species_name == species_in_world[i]){
                    judge = true;
                    break;
                }
            }
            if (judge == false){
                cout << "Error: Species " << species_name << " not found!" << endl;
                return false;
            }

            iFile_single_species.open(directory+"/"+species_name);
            if(!iFile_single_species){
                cout << "Error: Cannot open file " << directory << "/" << species_name << "!" << endl;
                return false;
            }
            else{
                    species[species_number].name = species_name;

                    while(getline(iFile_single_species,instructions) && (instructions.length() != 0)){
                        read_instructions.str(instructions);
                        read_instructions >> operation >> address;

                        int count = 0; // judge whether the operation is in opName[n]. If count = n+1, then cannot find.
                        for (unsigned int i = 0; i <= (sizeof(opName)/sizeof(opName[0])); i++){
                            if (operation == opName[i]){
                                species[species_number].program[instruction_number].op = opcode_t(i);
                                species[species_number].program[instruction_number].address = address;
                                break;
                            }
                            count ++;
                            if (count == sizeof(opName)/sizeof(opName[0])){
                                cout << "Error: Instruction " << operation << " is not recognized!" << endl;
                                return false;
                            }
                        }

                        instruction_number += 1;
                        if (instruction_number > MAXPROGRAM){
                            cout << "Error: Too many instructions for species " << species[species_number].name << "!" << endl;
                            cout << "Maximal number of instruction is " << MAXPROGRAM << "." << endl;
                            return false;
                        }
                        read_instructions.clear();
                        operation.clear();
                        species_name.clear();
                        address = 0;
                    }
                    species[species_number].programSize = instruction_number;

                    species_number += 1;
                    instruction_number = 0;
                    iFile_single_species.clear();
                    iFile_single_species.close();
            }
        }
        iFile_species_summary.close();
        iFile_creature_in_world.close();
        return true;
    }
}

bool initWorld(world_t &world, const string &speciesFile, const string &creaturesFile, species_t (&species)[MAXSPECIES], creature_t (&creature)[MAXCREATURES]){
// MODIFIES: world
// EFFECTS: Initialize "world" given the species summary file "speciesFile" and the world description file
//          "creaturesFile". This initializes all the components of "world".
//          Returns true is initialization is successful.
    ifstream iFile_species_summary; // species summary file
    ifstream iFile_world;   // creatures file
    ifstream iFile_species; // species file

    // Error checking 4: whether the number of species listed in the species summary file exceeds MAXSPECIES.
    // Error checking 5: whether the number of instructions exceeds MAXPROGRAM.
    iFile_species_summary.open(speciesFile);
    string species_in_summary[MAXSPECIES + 1];
    if(!iFile_species_summary){
        cout << "Error: Cannot open file " << speciesFile << "!" << endl;
        return false;
    }
    else{
        string count_species;
        string directory;
        getline(iFile_species_summary,directory);
        unsigned int count_species_number = 0;
        while(getline(iFile_species_summary,count_species)){
            species_in_summary[count_species_number] = count_species;
            iFile_species.open(directory+"/"+count_species);
            if(!iFile_species){
                cout << "Error: Cannot open file " << directory << "/" << count_species << "!" << endl;
                return false;
            }
            else{
                string get_instruction;
                unsigned int count_instruction = 0;
                while(getline(iFile_species,get_instruction)){
                    count_instruction ++;
                    if (count_instruction > MAXPROGRAM){
                        cout << "Error: Too many instructions for species " << count_species << "!" << endl;
                        cout << "Maximal number of instructions is " << MAXPROGRAM << "." << endl;
                        return false;
                    }
                }
                iFile_species.close();
            }
            count_species_number ++;
            count_species.clear();
            if (count_species_number > MAXSPECIES){
                cout << "Error: Too many species!" << endl;
                cout << "Maximal number of species is " << MAXSPECIES << "." << endl;
                return false;
            }
        }
    }

    iFile_species_summary.close();

    // read the row and column of the world
    iFile_world.open(creaturesFile);
    if(!iFile_world){
        cout << "Error: Cannot open file " << creaturesFile << "!" << endl;
        return false;
    }
    else{
        string world_row_number; // height of the world
        string world_coloum_number; // width of the world
        getline(iFile_world, world_row_number);
        getline(iFile_world, world_coloum_number);
        // convert the type to unsigned int and save them into the world.grid.
        istringstream iStream;
        iStream.str(world_row_number);
        iStream >> world.grid.height;
        // Error checking 10. Grid height
        if ((world.grid.height<1) || (world.grid.height > MAXHEIGHT)){
            cout << "Error: The grid height is illegal!" << endl;
            return false;
        }
        iStream.clear();
        iStream.str(world_coloum_number);
        iStream >> world.grid.width;
        //Error checking 11. Grid width
        if ((world.grid.width < 1) || (world.grid.width > MAXWIDTH)){
            cout << "Error: The grid width is illegal!" << endl;
            return false;
        }
        iStream.clear();
    }

    // initialize the creatures (include the species, direction, location);

    string creature_whole_line;
    istringstream creature_line;
    string creature_name;
    string creature_direction;
    unsigned int creature_row;
    unsigned int creature_column;
    unsigned int creature_number = 0;
    while(getline(iFile_world,creature_whole_line)) {
        creature_line.str(creature_whole_line);
        creature_line >> creature_name >> creature_direction >> creature_row >> creature_column;

        // Error checking 8, unknown species.
        bool check_unknown = false;
        for (unsigned int i = 0; i < MAXSPECIES; i++){
            //cout << species_in_summary[i] << endl;
            if (creature_name == species_in_summary[i]){
                check_unknown = true;
                break;
            }
        }
        if (check_unknown == false){
            cout << "Error: Species " << creature_name << " not found!" << endl;
            return check_unknown;
        }

        creature[creature_number].location.c = creature_column;
        creature[creature_number].location.r = creature_row;
        //cout << species << endl;
        // Error check 12, check whether the creature is inside the boundary of the grid.
        if ((creature_column >= world.grid.width) || (creature_row >= world.grid.height) || (creature_column < 0) || (creature_row < 0)){
            cout << "Error: Creature (" << creature_name << " " << creature_direction << " " << creature_row << " " << creature_column << ") is out of bound!" << endl;
            cout << "The grid size is " << world.grid.height << "-by-" << world.grid.width << "." << endl;
            return false;
        }

        creature[creature_number].species = &species[creature_number];

        for (unsigned int i = 0; i <= (sizeof(directName) / sizeof(directName[0])); i++) {
            if (creature_direction == directName[i]) {
                creature[creature_number].direction = direction_t(i);
                break;
            } else if (i == (sizeof(directName) / sizeof(directName[0]))) {
                cout << "Error: Direction " << creature_direction << " is not recognized!" << endl;
                return false;
            }
        }
        creature[creature_number].programID = 1;

        creature_number++;
        if (creature_number > MAXCREATURES){
            cout << "Error: Too many creatures!" << endl;
            cout << "Maximal number of creatures is " << MAXCREATURES << "." << endl;
            return false;
        }
        creature_line.clear();
        creature_whole_line.clear();
        creature_name.clear();
        creature_direction.clear();
    }

    // fill in the numCreatures of the world
    world.numCreatures = creature_number ;

    // fill in the creature_t array in the world
    for (unsigned int i = 0; i < creature_number; i++){
        world.creatures[i] = creature[i];
    }

    // fill in the species_t array in the world, the element in the array should be distinct.
    unsigned int species_number = 0;
    for (unsigned int i = 0; i < creature_number; i++){
        int count = 0;
        for (unsigned int j = 0; j < i ; j++){
            if (world.species[j].name == world.creatures[i].species->name) {
                count++;
            }
        }
        if (count == 0){
            world.species[species_number] = *(creature[i].species);
            species_number ++;
        }
    }

    // count the species number in the world
    world.numSpecies = species_number;

    // fill in world.grid.creature_t *squares[MAXHEIGHT][MAXWIDTH] and NULL pointers
    for (unsigned int height = 0; height < MAXHEIGHT; height ++){
        for (unsigned int width = 0; width < MAXWIDTH; width ++){
            world.grid.squares[height][width] = nullptr;
        }
    }
    for (unsigned int creature_num = 0; creature_num < creature_number; creature_num ++){
        unsigned int coloum_in_world = world.creatures[creature_num].location.c;
        unsigned int row_in_world = world.creatures[creature_num].location.r;
        // Error checking 13: whether each square in the grid is occupied by at most one creature.
        if (world.grid.squares[row_in_world][coloum_in_world] == nullptr){
            world.grid.squares[row_in_world][coloum_in_world] = &world.creatures[creature_num];
        }
        else {
            cout << "Error: Creature (" << creature[creature_num].species->name << " " << directName[creature[creature_num].direction] << " " << creature[creature_num].location.r << " " <<
                 creature[creature_num].location.c << ") overlaps with creature ("<< world.grid.squares[row_in_world][coloum_in_world]->species->name << " " << directName[world.grid.squares[row_in_world][coloum_in_world]->direction]
                 << " " << world.grid.squares[row_in_world][coloum_in_world]->location.r << " " << world.grid.squares[row_in_world][coloum_in_world]->location.c << ")!" << endl;
            return false;
        }
    }

    iFile_species_summary.close();
    iFile_species.close();
    iFile_world.close();
    return true;
}

void printGrid(const grid_t &grid){
    // MODIFIES: cout.
    // EFFECTS: print a grid representation of the creature world.
    for (unsigned int i = 0; i < grid.height; i++){
        for (unsigned int j = 0 ; j < grid.width; j++){
            if (grid.squares[i][j] == nullptr){
                cout << "____ ";
            }
            else{
                cout << grid.squares[i][j]->species->name[0] << grid.squares[i][j]->species->name[1] << "_" << directShortName[grid.squares[i][j]->direction] << " ";
            }
        }
        cout << endl;
    }
}

point_t adjacentPoint (point_t pt, direction_t dir){
    // EFFECTS: Returns a point that results from moving squares in the direction dir from the point "pt".
    // enum direction_t { EAST, SOUTH, WEST, NORTH };
    if (dir == EAST){
        pt.c++;
        return pt;
    }
    else if (dir == SOUTH){
        pt.r++;
        return pt;
    }
    else if (dir == WEST){
        pt.c--;
        return pt;
    }
    else{
        pt.r--;
        return pt;
    }
}

direction_t leftFrom(direction_t dir){
    // EFFECTS: Returns the direction that results from turning left from the given direction "dir".
    // enum direction_t { EAST, SOUTH, WEST, NORTH };
    if (dir == EAST){
        dir = NORTH;
        return dir;
    }
    else {
        dir = direction_t(dir - 1);
        return dir;
    }
}

direction_t rightFrom(direction_t dir){
    // EFFECTS: Returns the direction that results from turning right from the given direction "dir".
    // enum direction_t { EAST, SOUTH, WEST, NORTH };
    if (dir == NORTH){
        dir = EAST;
        return dir;
    }
    else{
        dir = direction_t(dir + 1);
        return dir;
    }
}

instruction_t getInstruction(const creature_t &creature){
    // EFFECTS: Returns the current instruction of "creature".
    instruction_t program = creature.species->program[creature.programID-1];
    return program;
}

void hop(creature_t &creature, grid_t &grid){
    // EFFECTS: The creature moves forward as long as the square it is facing is empty.
    //          If moving forward would put the creature outside the boundaries of the grid or would cause it to land
    //          on top of another creature, the hop instruction does nothing. Change the program ID.
    // enum direction_t { EAST, SOUTH, WEST, NORTH };
    unsigned int row = creature.location.r;
    unsigned int column = creature.location.c;
    if ((creature.direction == EAST) && (column < (grid.width - 1)) && (grid.squares[row][column + 1] == nullptr)){
        grid.squares[row][column + 1] = &creature;
        grid.squares[row][column] = nullptr;
        creature.location = adjacentPoint(creature.location, creature.direction);
    }
    else if (creature.direction == SOUTH && (row < (grid.height - 1)) && (grid.squares[row + 1][column] == nullptr)){
        grid.squares[row + 1][column] = &creature;
        grid.squares[row][column] = nullptr;
        creature.location = adjacentPoint(creature.location, creature.direction);
    }
    else if (creature.direction == WEST && (column >= 1) && (grid.squares[row][column - 1] == nullptr)){
        grid.squares[row][column - 1] = &creature;
        grid.squares[row][column] = nullptr;
        creature.location = adjacentPoint(creature.location, creature.direction);
    }
    else {
        if (creature.direction == NORTH && (row >= 1) && (grid.squares[row - 1][column] == nullptr)){
            grid.squares[row - 1][column] = &creature;
            grid.squares[row][column] = nullptr;
            creature.location = adjacentPoint(creature.location, creature.direction);
        }
    }
    creature.programID ++;
}

void left(creature_t &creature){
    // EFFECTS: The creature turns left 90 degrees to face in a new direction.
    //          Change the program ID.
    creature.direction = leftFrom(creature.direction);
    creature.programID ++;
}

void right(creature_t &creature){
    // EFFECTS: The creature turns right 90 degrees to face in a new direction.
    //          Change the program ID.
    creature.direction = rightFrom(creature.direction);
    creature.programID ++;
}

void infect (creature_t &creature_come, grid_t &grid){
    // EFFECTS: Occupied creature will become the same species as the come creature if they are different.
    //          Only changes the program and program ID starts from 1.
    //          If the square immediately in front of the come_creature is empty, outside the grid, or occupied by a creature of the same species,
    //          the instruction does nothing. Change the program ID.
    // enum direction_t { EAST, SOUTH, WEST, NORTH };
    unsigned int row = creature_come.location.r;
    unsigned int colomn = creature_come.location.c;
    if (creature_come.direction == EAST && (grid.squares[row][colomn + 1] != nullptr) && (colomn < grid.width - 1) &&  (grid.squares[row][colomn + 1]->species->name != grid.squares[row][colomn]->species->name)){
            grid.squares[row][colomn + 1]->species = grid.squares[row][colomn]->species;
            grid.squares[row][colomn + 1]->programID = 1;
    }
    else if (creature_come.direction == SOUTH && (grid.squares[row + 1][colomn] != nullptr) && (row < grid.height - 1)  && (grid.squares[row + 1][colomn]->species->name != grid.squares[row][colomn]->species->name)){
            grid.squares[row + 1][colomn]->species = grid.squares[row][colomn]->species;
            grid.squares[row + 1][colomn]->programID = 1;
    }
    else if (creature_come.direction == WEST && (grid.squares[row][colomn - 1] != nullptr) && (colomn >= 1) && (grid.squares[row][colomn - 1]->species->name != grid.squares[row][colomn]->species->name)){
            grid.squares[row][colomn - 1]->species = grid.squares[row][colomn]->species;
            grid.squares[row][colomn - 1]->programID = 1;
    }
    else {
        if (creature_come.direction == NORTH && (grid.squares[row - 1][colomn] != nullptr) && (row >= 1) && (grid.squares[row - 1][colomn]->species->name != grid.squares[row][colomn]->species->name)){
                grid.squares[row - 1][colomn]->species = grid.squares[row][colomn]->species;
                grid.squares[row - 1][colomn]->programID = 1;
        }
    }
    creature_come.programID ++;
}

void ifempty(creature_t &creature, grid_t &grid){
    // EFFECTS: If the square in front of the creature is inside the grid boundary and unoccupied, jump to step n of the program.
    //          Otherwise, go on with the next instruction in sequence.
    unsigned int row = creature.location.r;
    unsigned int colomn = creature.location.c;
    bool empty = false;

    if (((creature.direction == EAST) && (colomn < grid.width - 1) && (grid.squares[row][colomn + 1] == nullptr))
    || ((creature.direction == SOUTH) && (row < grid.height - 1) && (grid.squares[row + 1][colomn] == nullptr))
    || ((creature.direction == WEST) && (colomn >= 1) && (grid.squares[row][colomn - 1] == nullptr))
    || ((creature.direction == NORTH) && (row >= 1) && (grid.squares[row - 1][colomn] == nullptr))){
        empty = true;
    }
    if (empty){
        creature.programID = creature.species->program[creature.programID - 1].address;
    }
    else{
        if (creature.programID < creature.species->programSize){
            creature.programID ++;
        }
        else{
            creature.programID = 1;
        }
    }
}

void ifwall(creature_t &creature, grid_t &grid){
    // EFFECTS: If the creature is facing the border of the grid, jump to step n of the program;
    //          otherwise, go on with the next instruction in sequence.
    unsigned int row = creature.location.r;
    unsigned int colomn = creature.location.c;
    bool wall = false;

    if (((creature.direction == EAST) && (colomn == grid.width - 1))
        || ((creature.direction == SOUTH) && (row == grid.height - 1))
        || ((creature.direction == WEST) && (colomn == 0 ))
        || ((creature.direction == NORTH) && (row == 0))){
        wall = true;
    }
    if (wall){
        creature.programID = creature.species->program[creature.programID - 1].address;
    }
    else{
        if (creature.programID < creature.species->programSize){
            creature.programID ++;
        }
        else{
            creature.programID = 1;
        }
    }
}

void ifsame(creature_t &creature, grid_t &grid){
// EFFECTS: If the square the creature is facing is occupied by a creature of the same species,
//          jump to step n; otherwise, go on with the next instruction.
//          Have no idea why "!=nullptr" needs to be added, otherwise, it will exit 11.
    unsigned int row = creature.location.r;
    unsigned int colomn = creature.location.c;
    std::string name = grid.squares[row][colomn]->species->name;
    bool is_same = false;

    if (((creature.direction == EAST) && (colomn < (grid.width - 1)) && (grid.squares[row][colomn + 1] != nullptr) && (grid.squares[row][colomn + 1]->species->name == name))
        || ((creature.direction == SOUTH) && (row < (grid.height - 1)) && (grid.squares[row + 1][colomn] != nullptr) && (grid.squares[row + 1][colomn]->species->name == name))
        || ((creature.direction == WEST) && (colomn >= 1) && (grid.squares[row][colomn - 1] != nullptr) && (grid.squares[row][colomn - 1]->species->name == name))
        || ((creature.direction == NORTH) && (row >= 1) && (grid.squares[row - 1][colomn] != nullptr) && (grid.squares[row - 1][colomn]->species->name == name))){
        is_same = true;
    }
    if (is_same){
        creature.programID = creature.species->program[creature.programID - 1].address;
    }
    else{
        if (creature.programID < creature.species->programSize){
            creature.programID ++;
        }
        else{
            creature.programID = 1;
        }
    }
}

void ifenemy(creature_t &creature, grid_t &grid){
// EFFECTS: If the square the creature is facing is occupied by a creature of an enemy species,
//          jump to step n; otherwise, go on with the next instruction.
    unsigned int row = creature.location.r;
    unsigned int colomn = creature.location.c;
    std::string name = grid.squares[row][colomn]->species->name;
    bool is_enemy = false;

    if (((creature.direction == EAST) && (colomn < (grid.width - 1)) && (grid.squares[row][colomn + 1] != nullptr) && (grid.squares[row][colomn + 1]->species->name != name))
        || ((creature.direction == SOUTH) && (row < (grid.height - 1)) && (grid.squares[row + 1][colomn] != nullptr) &&(grid.squares[row + 1][colomn]->species->name != name))
        || ((creature.direction == WEST) && (colomn >= 1) && (grid.squares[row][colomn - 1] != nullptr) && (grid.squares[row][colomn - 1]->species->name != name))
        || ((creature.direction == NORTH) && (row >= 1) && (grid.squares[row - 1][colomn] != nullptr) && (grid.squares[row - 1][colomn]->species->name != name))){
        is_enemy = true;
    }
    if (is_enemy){
        creature.programID = creature.species->program[creature.programID - 1].address;
    }
    else{
        if (creature.programID < creature.species->programSize){
            creature.programID ++;
        }
        else{
            creature.programID = 1;
        }
    }
}

void go(creature_t &creature){
    // EFFECTS: This instruction always jumps to step n, independent of any condition.
    creature.programID = creature.species->program[creature.programID - 1].address;
}

void simulateCreature(creature_t &creature, grid_t &grid, bool verbose) {
    // REQUIRES: creature is inside the grid.
    // MODIFIES: creature, grid, cout.
    // EFFECTS: simulate one turn of "creature" and update the creature, the infected creature and the grid if necessary.
    //          The creature program ID is always updated. The function also prints to the stdout the procedure.
    //          If verbose is true, it prints more information.
    // hop, left, right, infect print the grid in verbose. if empty, ifwall, ifsame, ifenemy, go don't.

    cout << "Creature (" << creature.species->name << " " << directName[creature.direction] << " "
         << creature.location.r << " " << creature.location.c << ") takes action:";
    if (verbose){
        cout << endl;
    }
    else{
        cout << " ";
    }
    while ((getInstruction(creature).op == IFEMPTY) || (getInstruction(creature).op == IFWALL) ||
           (getInstruction(creature).op == IFSAME) || (getInstruction(creature).op == IFENEMY) ||
           (getInstruction(creature).op == GO)) {
        switch (getInstruction(creature).op) {
            case IFEMPTY:
                if (verbose) {
                    cout << "Instruction " << creature.programID << ": ";
                    cout << "ifempty" << " " << getInstruction(creature).address << endl;
                }
                ifempty(creature, grid);
                break;
            case IFWALL:
                if (verbose) {
                    cout << "Instruction " << creature.programID << ": ";
                    cout << "ifwall" << " " << getInstruction(creature).address << endl;
                }
                ifwall(creature, grid);
                break;
            case IFSAME:
                if (verbose) {
                    cout << "Instruction " << creature.programID << ": ";
                    cout << "ifsame" << " " << getInstruction(creature).address << endl;
                }
                ifsame(creature, grid);
                break;
            case IFENEMY:
                if (verbose) {
                    cout << "Instruction " << creature.programID << ": ";
                    cout << "ifenemy" << " " << getInstruction(creature).address << endl;
                }
                ifenemy(creature, grid);
                break;
            case GO:
                if (verbose) {
                    cout << "Instruction " << creature.programID << ": ";
                    cout << "go" << " " << getInstruction(creature).address << endl;
                }
                go(creature);
                break;
            default:
                break;
        }
    }
    switch (getInstruction(creature).op) {
        case HOP:
            if (verbose) {
                cout << "Instruction " << creature.programID << ": ";
            }
            cout << "hop" << endl;
            hop(creature, grid);
            break;
        case LEFT:
            if (verbose) {
                cout << "Instruction " << creature.programID << ": ";
            }
            cout << "left" << endl;
            left(creature);
            break;
        case RIGHT:
            if (verbose) {
                cout << "Instruction " << creature.programID << ": ";
            }
            cout << "right" << endl;
            right(creature);
            break;
        case INFECT:
            if (verbose) {
                cout << "Instruction " << creature.programID << ": ";
            }
            cout << "infect" << endl;
            infect(creature, grid);
            break;
        default:
            break;
    }
}

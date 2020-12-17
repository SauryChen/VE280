#include "simulation.h"
using namespace std;

int main(int argc, char *argv[])
{
    //cout << argc << endl;
    // Error checking 1, whether the #arguments is less than three.
    // ./p3 <species-summary> <world-file> <rounds> [v|verbose] (argc = 5).
    if (argc < 4){
        cout << "Error: Missing arguments!" << endl;
        cout << "Usage: ./p3 <species-summary> <world-file> <rounds> [v|verbose]" << endl;
        return 0;
    }

    // Error checking 2, whether the rounds is negative.
    int round = atoi(argv[3]);
    if (round < 0){
        cout << "Error: Number of simulation rounds is negative!" << endl;
        return 0;
    }

    world_t world;
    species_t species[MAXSPECIES];
    creature_t creature[MAXCREATURES];

    const string speciesfile = argv[1];
    const string creaturefile = argv[2];

    bool bool_species = load_species(speciesfile,creaturefile,species);
    if (!bool_species){
        return 0;
    }

    bool bool_initWorld = initWorld(world, speciesfile, creaturefile, species, creature);
    if (!bool_initWorld){
        return 0;
    }


    if (argc > 4) {
        if ((string(argv[4]) == "v") || (string(argv[4]) == "verbose")) {
            cout << "Initial state" << endl;
            printGrid(world.grid);
            for (int count = 1; count <= round; count++) {
                cout << "Round " << count << endl;
                for (unsigned int creature_count = 0; creature_count < world.numCreatures; creature_count++) {
                    simulateCreature(world.creatures[creature_count], world.grid, true);
                    printGrid(world.grid);
                }
            }
        } else {
            cout << "Initial state" << endl;
            printGrid(world.grid);
            for (int count = 1; count <= round; count++) {
                cout << "Round " << count << endl;
                for (unsigned int creature_count = 0; creature_count < world.numCreatures; creature_count++) {
                    simulateCreature(world.creatures[creature_count], world.grid, false);
                }
                printGrid(world.grid);
            }
        }
    }

    else if (argc == 4){
        cout << "Initial state" << endl;
        printGrid(world.grid);
        for (int count = 1; count <= round; count ++){
            cout << "Round " << count << endl;
            for (unsigned int creature_count = 0; creature_count < world.numCreatures; creature_count ++){
                simulateCreature(world.creatures[creature_count], world.grid, false);
            }
            printGrid(world.grid);
        }
    }
    return 0;
}


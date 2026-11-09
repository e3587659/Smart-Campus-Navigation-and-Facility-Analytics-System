Smart Campus Navigation and Facility Analytics System
CSCE 2110 - Project 2 (Milestone 1)

PURPOSE
A C++ program that loads campus data from four text files and lets the
user view and search facilities (custom hash table), explore building
connections (undirected graph with BFS), and manage the academic
department hierarchy (general tree).

BUILD (from the project's top folder)
    g++ -std=c++17 -Wall -Iinclude src/*.cpp -o campus

RUN
    ./campus                         uses the default data/ folder
    ./campus testdata/collision      uses another data folder (optional argument)

Files are loaded in this order: buildings.txt, connections.txt,
facilities.txt, departments.txt. Paths are relative to the folder the
program is run from.

DIRECTORY LAYOUT
    include/            header files (.h)
    src/                source files (.cpp)
        main.cpp              loads files and runs the menu (shared)
        FileHelp.cpp          trim, case, and split helpers (shared)
        FacilityHashTable.cpp custom ID hash table (Member A)
        FacilityManager.cpp   facility loading, display, ID search (Member A)
        CampusGraph.cpp       campus graph and BFS (Member B)
        DepartmentTree.cpp    department general tree (Member C)
    data/               instructor-provided data files (unchanged)
    testdata/           our own test data folders
        collision/      IDs that collide in the hash table
        invalid/        bad records, duplicates, Windows line endings
        missing/        missing data files and an empty department tree
    README.txt, design_document.txt, test_results.txt, CONTRIBUTIONS.txt

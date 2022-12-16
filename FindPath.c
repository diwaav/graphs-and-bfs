/********************************************************************************* 
* Diwa Ashwini Vittala, dashwini
* 2022 Spring CSE101 PA2
* FindPath.c
* Main file for graphing algorithm
**********************************************************************************/

#include "List.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // There must be an input and output file
    if (!argv[1]) {
        fprintf(stderr, "Error: Missing input or ouptput files.\n");
        exit(EXIT_FAILURE);
    }
    if (argv[3]) {
        fprintf(stderr, "Error: Too many inputs given!\n");
        exit(EXIT_FAILURE);
    }

    // open input and output file 
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL) {
        fprintf(stderr, "Error: no such input file exists.\n");
        exit(EXIT_FAILURE);
    }
    FILE *outfile = fopen(argv[2], "w");

    // read in total # of vertices
    int numberOfVertices = 0;
    fscanf(infile, "%d\n", &numberOfVertices);

    Graph A = newGraph(numberOfVertices);

    // read in edges
    int u, v;
    fscanf(infile, "%d %d\n", &u, &v);
    addEdge(A, u, v);
    while (true) {
        fscanf(infile, "%d %d\n", &u, &v);
        if ((u == 0) && (v == 0)) {
          break;
        }
        addEdge(A, u, v);
    }

    // print adj list
    printGraph(outfile, A);
    fprintf(outfile, "\n");

    // read source and destination
    int s, d;
    while (true) {
        fscanf(infile, "%d %d\n", &s, &d);
        if ((s == 0) && (d == 0)) {
            break;
        }

        // perform search
        BFS(A, s);

        // find distance
        int distance = getDist(A, d);
        if (distance == INF) {
            fprintf(outfile, "The distance from %d to %d is infinity\n", s, d);
        } else {
            fprintf(outfile, "The distance from %d to %d is %d\n", s, d, getDist(A, d));
        }

        // find shortest path for each source/destination pair
        List list = newList();
        getPath(list, A, d);
        moveFront(list);
        if (get(list) == 0) {
            fprintf(outfile, "No %d-%d path exists\n", s, d);
        } else {
            fprintf(outfile, "A shortest %d-%d path is: ", s, d);
            printList(outfile, list);
        }
        fprintf(outfile, "\n");
        freeList(&list);
    }

    freeGraph(&A);
    fclose(infile);
    fclose(outfile);
    return 0;
}






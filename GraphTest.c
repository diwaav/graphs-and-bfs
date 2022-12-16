/********************************************************************************* 
* Diwa Ashwini Vittala, dashwini
* 2022 Spring CSE101 PA2
* GraphTest.c
* A program to test Graph.c
**********************************************************************************/

#include "Graph.h"
#include "List.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int num = 20;
    Graph G = newGraph(num);

    // connecting some vertices
    addEdge(G, 2, 4);
    addEdge(G, 3, 6);
    addEdge(G, 3, 4);
    addEdge(G, 1, 3);
    addEdge(G, 4, 5);
    addEdge(G, 9, 5);
    addEdge(G, 3, 3);
    // Make sure everything was added properly
    printf("Generated graph's adj list:\n");
    printGraph(stdout, G);

    // get a path to somewhere from many different sources
    int somewhere = 4;
    for (int source = 1; source <= num; source += 1) {
        BFS(G, source);
        printf("The distance from vertex %d to %d is %d\n", somewhere, source, getDist(G, source));
        List list = newList();
        getPath(list, G, somewhere);
        moveFront(list);
        if (get(list) == 0) {
            printf("No %d-%d path exists\n", somewhere, source);
        } else {
            printf(" -> There is a path between %d and %d like this: ", somewhere, source);
            printList(stdout, list);
        }
        printf("\n");
        freeList(&list);
    }

    // free memory
    freeGraph(&G);
    return 0;
}


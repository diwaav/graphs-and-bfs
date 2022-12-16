/********************************************************************************* 
* Diwa Ashwini Vittala, dashwini
* 2022 Spring CSE101 PA2
* Graph.c
* Graph ADT 
**********************************************************************************/

#include "Graph.h"
#include "List.h"

// A graph structure
typedef struct GraphObj {
    int size; // number of arcs or edges
    int vertices; // total number of vertices in the graph
    int source; // source vertex
    char* color; // determine if a vertex has been visted or not 
                 // (w = undiscovered, g = discorvered but not neighbors, b = done)
    List* neighbors; // the graph, made up of lists
    int* parent; // contains all the parents of each vertex
    int* distance; // the distance to the most recent source to vertex i
} GraphObj;

/*** Constructors-Destructors ***/
// newGraph(): create a new graph
Graph newGraph(int n) {
    Graph G = malloc(sizeof(GraphObj));
    G->vertices = n;
    G->source = NIL;
    G->size = 0;

    // start with color = w, distance = inf, parent = nil
    G->color = (char *) calloc((n + 1), sizeof(int));
    for (int i = 0; i <= n; i += 1) {
        G->color[i] = 'w';
    }

    G->neighbors = (List *) calloc((n + 1), sizeof(List));
    for (int i = 0; i <= n; i += 1) {
        G->neighbors[i] = newList();
    }

    G->parent = (int *) calloc((n + 1), sizeof(int));
    for (int i = 0; i <= n; i += 1) {
        G->parent[i] = NIL;
    }

    G->distance = (int *) calloc((n + 1), sizeof(int));
    for (int i = 0; i <= n; i += 1) {
        G->distance[i] = INF;
    }
    return G;
}

// freeGraph(): destroys a graph
void freeGraph(Graph* pG) {
    if (pG && *pG) {
        for (int i = 0; i <= (*pG)->vertices; i += 1) {
            freeList(&(((*pG)->neighbors)[i]));
        }
        free(((*pG)->neighbors));
        if ((*pG)->parent) {
            free((*pG)->parent);
            (*pG)->parent = NULL;
        }
        if ((*pG)->distance) {
            free((*pG)->distance);
            (*pG)->distance = NULL;
        }
        if ((*pG)->color) {
            free((*pG)->color);
            (*pG)->color = NULL;
        }
        free(*pG);
        *pG = NULL;
    }
    return;
}

/*** Access functions ***/
// getOrder(): returns number of vertices in graph
int getOrder(Graph G) {
    if (G) {
        return G->vertices;
    } else {
        printf("Graph error: calling getOrder() on non existent graph.\n");
        exit(EXIT_FAILURE);
    }
}

// getSize(): returns number of edges or arcs in graph
int getSize(Graph G) {
    if (G) {
        return G->size;
    } else {
        printf("Graph error: calling getSize() on non existent graph.\n");
        exit(EXIT_FAILURE);
    }
}

// getSource(): returns source vertex of the graph
int getSource(Graph G) {
    return G->source;
}

// getParent(): returns the parent of a vertex
int getParent(Graph G, int u) {
    if ((u >= 1) && (u <= getOrder(G))) {
        return G->parent[u];
    } else {
        printf("Graph error: calling getParent() on u not in range.\n");
        exit(EXIT_FAILURE);
    }
}

// getDist(): returns distance travelled while BFS
int getDist(Graph G, int u) {
    if ((u >= 1) && (u <= getOrder(G))) {
        return G->distance[u];
    } else {
        printf("Graph error: calling getDist() on u not in range.\n");
        exit(EXIT_FAILURE);
    }
}

// getPath(): turns List L into a path from the source to u
void getPath(List L, Graph G, int u) {
    if ((G->source != 0) && ((u >= 1) && (u <= getOrder(G)))) {
        if (u == G->source) {
            append(L, G->source);
        } else if (G->parent[u] == NIL) {
            append(L, NIL);
        } else {
            getPath(L, G, G->parent[u]);
            append(L, u);
        }
    }
    return;
}

/*** Manipulation procedures ***/
// makeNull(): resets graph
void makeNull(Graph G) {
    if (G) {
        for (int i = 0; i <= G->vertices; i += 1) {
            clear((G->neighbors)[i]);
            G->parent[i] = NIL;
            G->distance[i] = INF;
            G->color[i] = 'w';
        }
    }
    return;
}

// place(): like append(), but adds while sorting from least to greatest
void place (Graph G, List list, int i) {
    if (length(list) == 0) {
        append(list, i);
        moveFront(list);
    } else if (i <= get(list)) { // if num <= cursor
        if (get(list) == front(list)) {
            prepend(list, i); // if cursor was at front
            return;
        }
        movePrev(list); 
        if (i < get(list)) {
            place(G, list, i);
        } else {
            insertAfter(list, i);
        }
    } else {
        if (get(list) == back(list)) {
            append(list, i); // if cursor was at back
            return;
        }
        moveNext(list);
        if (i > get(list)) {
            place(G, list, i);
        } else {
            insertBefore(list, i);
        }
    }
    return;
}


// addEdge(): adds an edge from vertex u to v
void addEdge(Graph G, int u, int v) {
    if (G && ((u >= 1) && (u <= getOrder(G))) && ((v >= 1) && (v <= getOrder(G)))) {
        if (G->neighbors[u]) {
            addArc(G, v, u);
            addArc(G, u, v);
            G->size -= 1;
        } else {
            printf("Graph error: calling addEdge() to a vertex that does not exist or not in range.\n");
            exit(EXIT_FAILURE);
        }
    }
    return;
}
// addArc(): adds an arc from u to v
void addArc(Graph G, int u, int v) {
    if (G && ((u >= 1) && (u <= getOrder(G))) && ((v >= 1) && (v <= getOrder(G)))) {
        if (G->neighbors[u]) {
            place(G, G->neighbors[u], v);
            G->size += 1;
        } else {
            printf("Graph error: calling addArc() to a vertex that does not exist or not in range.\n");
            exit(EXIT_FAILURE);
        }
    }
    return;
}

// BFS(): performs a Breadth First Search on a graph given source vertex
void BFS(Graph G, int s) {
    // if queue is empty and we are starting at the source
    G->source = s;
    for (int i = 0; i <= getOrder(G); i += 1) {
        G->color[i] = 'w';
        G->distance[i] = INF;
        G->parent[i] = NIL;
    }
    G->color[G->source] = 'g';
    G->distance[s] = 0;
    G->parent[s] = NIL;
    List Q = newList();
    append(Q, s);
    int x;
    while (length(Q) > 0) {
        x = front(Q);
        deleteFront(Q);
        moveFront(G->neighbors[x]);
        int y;
        while (length(G->neighbors[x]) > 0 && (index(G->neighbors[x]) >= 0)) {
            y = get(G->neighbors[x]);
            if (G->color[y] == 'w') {
                G->color[y] = 'g';
                G->distance[y] = G->distance[x] + 1;
                G->parent[y] = x;
                append(Q, y);
            }
            moveNext(G->neighbors[x]);
        }
        G->color[x] = 'b';
    }
    freeList(&Q);
    return;
}

/*** Other operations ***/
// printGraph(): prints adjacency list of graph
void printGraph(FILE* out, Graph G) {
    if (G) {
        for (int i = 1; i <= G->vertices; i += 1) {
            fprintf(out, "%d: ", i);
            if (length(G->neighbors[i]) > 0) {
                printList(out, G->neighbors[i]);
            } else {
                fprintf(out, "\n");
            }
        }
    }
    return;
}




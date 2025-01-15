// MARALOI IOANA 313CC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INFINITY 10000
// structuri pentru taskul 1
typedef struct node {
  char *cityName;
  // retin numele orasului destinatie
  float *degradation;
  float *oldDegradation;
  // am 2 degradari
  int numberTrons;
  struct node *next;
} node;
typedef struct list {
  node *head;
  int len;
} list;
typedef struct graph {
  int nr_nodes;
  list **adjlists;
  // in cities retin orasele, cand citesc un oras,
  // verific daca il am in cities. daca il am returnez pozitia
  // la care se afla si inserez un nou nod la lista cu pozitia respectiva
  char **cities;
} graph;
// structuri pentru taskul 2
typedef struct node2 {
  char *name2;
  int cost;
  struct node2 *next;
} node2;
typedef struct list2 {
  node2 *head;
  int len;
} list2;
typedef struct graph2 {
  int numberNodes2;
  list2 **adjLists2;
  char **cities;
  int k;
  int m;
} graph2;
typedef struct nodeHeap {
  char *nameCity;
  int cost;
} nodeHeap;
typedef struct Heap {
  nodeHeap *vector;
  int size;
  int capacity;
} Heap;
graph *initGraph(int nr_nodes) {
  // initializez graful cu 2*nr_muchii, acesta fiind
  // nr maxim de noduri posibile
  graph *g = calloc(1, sizeof(graph));
  if (g == NULL) {
    printf("eroare la alocare graf\n");
    return NULL;
  }
  int i;
  g->nr_nodes = 0;
  g->cities = calloc(nr_nodes, sizeof(char *));
  // initializez un vector de 2*nr_muchii orase si liste de adiacenta
  if (g->cities == NULL) {
    printf("eroare la alocare orase\n");
    return NULL;
  }
  g->adjlists = (list **)calloc(nr_nodes, sizeof(list *));
  if (g->adjlists == NULL) {
    printf("eroare la alocare liste\n");
    return NULL;
  }
  for (i = 0; i < nr_nodes; i++) {
    g->adjlists[i] = NULL;
    g->cities[i] = NULL;
  }
  return g;
}

float *reverse(float *damage, int nrelem) {
  // functie pentru inversarea unui sir de numere
  // inversez gradele de degradare pt a adauga vectorul pe dos
  // in nodul destinatie
  int i;
  float *reversed = calloc(nrelem, sizeof(float));
  if (reversed == NULL) {
    printf("eroare la reverse\n");
    return NULL;
  }
  for (i = 0; i < nrelem; i++) reversed[i] = damage[nrelem - i - 1];
  return reversed;
}
int checkCity(char *city, graph *g) {
  // functie care verifica daca am deja orasul citit din fiser
  // in graful meu
  int i;
  for (i = 0; i < g->nr_nodes; i++) {
    // daca il am deja, returnez pozitia la care se afla in
    // vectorul de orase
    if (strcmp(g->cities[i], city) == 0) {
      return i;
    }
  }
  // daca nu il am, returnez -1
  return -1;
}
node *add_list(int pos, char *city) {
  // adaug orasul city in lista cu numarul pos
  // returnez noul nod
  node *new = calloc(1, sizeof(node));
  if (new == NULL) {
    printf("eroare la alocare nod lista\n");
    return NULL;
  }
  // aloc si copiez numele orasului in nod
  new->cityName = calloc(100, sizeof(char));
  if (new->cityName == NULL) {
    printf("eroare la alocare nume oras\n");
    return NULL;
  }
  strcpy(new->cityName, city);
  new->next = NULL;
  return new;
}
void add_edge(int source, int destination, float *damage, graph *g, char *city1,
              char *city2, int nrelem) {
  // adaug o muchie intre 2 noduri
  // adaug un nod
  node *new = add_list(destination, city2);
  new->numberTrons = nrelem;
  // daca lista e goala, acel nod est head
  if (g->adjlists[source]->len == 0) {
    g->adjlists[source]->head = new;
  } else {
    // daca lista nu este goala, pun acel nod pe prima pozitie
    new->next = g->adjlists[source]->head;
    g->adjlists[source]->head = new;
  }
  // cresc nr de noduri din lista
  g->adjlists[source]->len++;
  // aloc memorie pentru a retine gradul de degrdare pt fiecare tronson
  new->degradation = calloc(nrelem, sizeof(float));
  if (new->degradation == NULL) {
    printf("eroare la alocare degradation\n");
    return;
  }
  new->oldDegradation = calloc(nrelem, sizeof(float));
  if (new->oldDegradation == NULL) {
    printf("eroare la alocare oldDegradation\n");
    return;
  }
  int i;
  // copiez gradul de degradare in ndegradation si olddegradation
  for (i = 0; i < nrelem; i++) {
    new->degradation[i] = damage[i];
    new->oldDegradation[i] = new->degradation[i];
  }
}
void add_node(char *city, graph *g, int cnt) {
  // daca nu am orasul city in graf, apelez aceasta functie
  // initializez orasul de la poziita cnt si lista de adiacenta
  // adaug la pozitia cnt din vectorul de orase numele orasului
  g->nr_nodes++;
  g->cities[cnt] = calloc(100, sizeof(char));
  if (g->cities[cnt] == NULL) {
    printf("eroare la alocare nod\n");
    return;
  }
  strcpy(g->cities[cnt], city);
  if (g->adjlists[cnt] == NULL) {
    g->adjlists[cnt] = calloc(1, sizeof(list));
    g->adjlists[cnt]->head = NULL;
  }
}
float getMax(float *v, int len) {
  // returnez maximul dintr-un vector
  int i;
  float MAX = -1;
  for (i = 0; i < len; i++) {
    if (v[i] > MAX) MAX = v[i];
  }
  return MAX;
}
void updateNewDegr(graph *g) {
  // dupa fiecare parcurgere a grafului, copiez in
  // olddegradation numerele din vectorul de degradation
  int i, j;
  for (i = 0; i < g->nr_nodes; i++) {
    node *aux = g->adjlists[i]->head;
    while (aux != NULL) {
      for (j = 0; j < aux->numberTrons; j++) {
        aux->oldDegradation[j] = aux->degradation[j];
      }
      aux = aux->next;
    }
  }
}
float updateFirstSector(int pos, graph *g, char *destination) {
  int i;
  // aloc un vector de lungime egala cu numarul de noduri\
  // aflate in lista sursei
  float *v = malloc(g->adjlists[pos]->len * sizeof(float));
  for (i = 0; i < g->adjlists[pos]->len; i++) v[i] = 0;  // initializez cu 0
  node *aux = g->adjlists[pos]->head;
  i = 0;
  int j;
  // parcurg toata lista cu vecinii sursei
  while (aux != NULL) {
    // daca nodul curent este destinatia
    if (strcmp(aux->cityName, destination) == 0) {
      if (aux->numberTrons == 1)
        // daca destinatia are un singur tronson, pun zero
        v[i] = 0;
      else
        // daca are mai multe tronsoane, copiez in v tronsonul urmator
        v[i] = aux->oldDegradation[1];
    } else
      // daca nu e destinatia, copiez degradaera corespunzatoare primului
      // tronson
      v[i] = aux->oldDegradation[0];
    i++;
    aux = aux->next;
  }
  // aflu maximul si returnez
  float returnat = getMax(v, g->adjlists[pos]->len);
  free(v);
  return returnat / 2;
}
float updateLastSector(int pos2, graph *g, char *source) {
  int i;
  // aloc un vector de lungime egala cu nr de noduri din lista destinatiei
  float *v = calloc(g->adjlists[pos2]->len, sizeof(float));
  for (i = 0; i < g->adjlists[pos2]->len; i++) v[i] = 0;
  node *aux = g->adjlists[pos2]->head;
  i = 0;
  // parcurg lista destinatiei
  while (aux != NULL) {
    // daca nodul e sursa, verific cate tronsoane are
    if (strcmp(aux->cityName, source) == 0) {
      if (aux->numberTrons == 1)
        v[i] = 0;
      else
        v[i] = aux->oldDegradation[1];
    } else
      // daca nodul nu e sursa, atunci pun direct prima degradare
      v[i] = aux->oldDegradation[0];
    i++;
    aux = aux->next;
  }
  // returnez maximul/2
  float returnat = getMax(v, g->adjlists[pos2]->len);
  free(v);
  return returnat / 2;
}
void updatePaths(graph *g, int pos, int pos2) {
  char *source = g->cities[pos];        // numele orasului sursa
  char *destination = g->cities[pos2];  // numele orasului destinatie
  node *aux1 = g->adjlists[pos]->head;
  // parcurg lista orasului sursa in cautarea destinatiei
  while (aux1 != NULL) {
    if (strcmp(aux1->cityName, destination) == 0)
      break;
    else
      aux1 = aux1->next;
  }
  node *aux2 = g->adjlists[pos2]->head;
  // parcurg lista orasului destinatie in cautarea sursei
  while (aux2 != NULL) {
    if (strcmp(aux2->cityName, source) == 0)
      break;
    else
      aux2 = aux2->next;
  }
  int i;
  // cand le-am gasit pe amandoua, copiez degradarile din
  // lista sursa in lista destinatiei doar ca pe dos
  for (i = 0; i < aux1->numberTrons; i++) {
    aux2->degradation[i] = aux1->degradation[aux1->numberTrons - 1 - i];
  }
}
void decay(graph *g, int pos, int pos2) {
  int i;
  // parcurg lista lui pos pana gasesc orasul
  // corespunzator lui pos2
  if (g->adjlists[pos] != NULL) {
    char *source = g->cities[pos];        // numele orasului sursa
    char *destination = g->cities[pos2];  // numele orasului destinatie
    node *city1 = g->adjlists[pos]->head;
    node *city2 = g->adjlists[pos2]->head;
    while (city1 != NULL) {
      // caut orasul destinatie in lista sursei
      if (strcmp(city1->cityName, destination) == 0) {
        // am gasit destinatia in lista sursei
        for (i = 0; i < city1->numberTrons; i++) {
          if (city1->degradation[i] != 0) {
            // daca tronsonul nu are degradarea 0
            if (city1->degradation[i] < 50)
              // daca e mai mic decat 50, dublez
              city1->degradation[i] = 2 * city1->degradation[i];
            else if (city1->degradation[i] >= 50)
              // daca e mai mare, pun 100
              city1->degradation[i] = 100;
          } else {
            // daca gradul de degradare e zero, verific pe ce pozitie
            // se afla tronsonul
            if (i > 0 && i < city1->numberTrons - 1) {
              // daca e la mijloc, aflu maximul dintre tronsoanele vecine si /2
              if (city1->oldDegradation[i - 1] > city1->oldDegradation[i + 1])
                city1->degradation[i] = city1->oldDegradation[i - 1] / 2;
              else
                city1->degradation[i] = city1->oldDegradation[i + 1] / 2;
            }
            if (i == 0)  // daca primul tronson e zero
              city1->degradation[i] = updateFirstSector(pos, g, destination);
            if (i == city1->numberTrons - 1)  // daca ultimul tronson e zero
              city1->degradation[i] = updateLastSector(pos2, g, source);
          }
        }
        // updatez degradarea in lista destinatiei, la nodul corespunzator
        // sursei
        updatePaths(g, pos, pos2);
        return;
      } else
        city1 = city1->next;
    }
  }
}
void free_graph(graph **g) {
  // eliberez memoria pentru graf
  int i;
  for (i = 0; i < (*g)->nr_nodes; i++) {
    if ((*g)->adjlists[i] != NULL) {
      node *curr = (*g)->adjlists[i]->head;
      // eliberez memoria pt fiecare nod din fiecare lsita
      while (curr != NULL) {
        node *temp = curr;
        curr = curr->next;
        free(temp->cityName);
        free(temp->degradation);
        free(temp->oldDegradation);
        free(temp);
      }
    }
    free((*g)->adjlists[i]);  // eiberez memoria pt fiecare lista
    if ((*g)->cities[i] != NULL) free((*g)->cities[i]);
  }
  free((*g)->cities);
  free((*g)->adjlists);
  free(*g);
}
void displayRoutes(graph *g, char ***rutele, FILE *file_out, int r, int l) {
  // functia de scriere a rezultatului in fiserul de out
  int i, j;
  float sum = 0;
  // initializez un vector de lungime nr_rute, pun -1 peste tot
  int *keptRoutes = malloc(r * sizeof(int));
  // parcurg toate rutele si le afisez
  for (i = 0; i < r; i++) {
    keptRoutes[i] = -1;
    fprintf(file_out, "%s %s ", rutele[i][0], rutele[i][1]);
    int cntSource = checkCity(rutele[i][0], g);
    // caut in lista sursei, orasul destinatie
    node *aux = g->adjlists[cntSource]->head;
    while (aux != NULL) {
      if (strcmp(aux->cityName, rutele[i][1]) == 0) break;
      aux = aux->next;
    }
    // afisez nr de tronsoane si degradarile
    fprintf(file_out, "%d", aux->numberTrons);
    sum = 0;
    for (j = 0; j < aux->numberTrons; j++) {
      // fac suma degradarilor
      sum = sum + aux->degradation[j];
      fprintf(file_out, " %.2f", aux->degradation[j]);
    }
    sum = sum / aux->numberTrons;
    // daca su,a e mai mica decat degradarea max admisa
    if (sum < l)
      keptRoutes[i] = i + 1;  // retin pozitia rutei
    else
      keptRoutes[i] = -1;  // nu retin pozitia rutei
    fprintf(file_out, "\n");
  }
  // daca am retinut pozitia rutei, o afisez
  for (i = 0; i < r; i++) {
    if (keptRoutes[i] != -1) fprintf(file_out, "%d ", keptRoutes[i]);
  }
  free(keptRoutes);
}
void task1(FILE *file_in, FILE *file_out) {
  int r, k, l, nrtrons;
  // citesc din fisier r, k, l
  fscanf(file_in, "%d", &r);
  fscanf(file_in, "%d", &k);
  fscanf(file_in, "%d", &l);
  int i, j;
  int poz1 = 0, poz2 = 0;
  char *city1 = malloc(20 * sizeof(char));
  if (city1 == NULL) {
    printf("eroare alocare city1\n");
    return;
  }
  char *city2 = malloc(20 * sizeof(char));
  if (city2 == NULL) {
    printf("eroare alocare city2\n");
    return;
  }
  // initializez graful cu 2*nr_rute
  // deoarece acesta e nr maxim de noduri pe care
  // pot sa il am
  graph *g = initGraph(2 * r);
  char ***routes = calloc(r, sizeof(char **));
  // in routes pastrez un vector de perechi de orase
  // in ordinea in care sunt in fisierul de intrare
  if (routes == NULL) {
    printf("eroare alocare rute\n");
    return;
  }
  // citesc r rute din fisier
  for (i = 0; i < r; i++) {
    routes[i] = malloc(2 * sizeof(char *));
    if (routes[i] == NULL) {
      printf("eroare alocare rute\n");
      return;
    }
    routes[i][0] = malloc(20 * sizeof(char));
    if (routes[i][0] == NULL) {
      printf("eroare alocare rute\n");
      return;
    }
    routes[i][1] = malloc(20 * sizeof(char));
    if (routes[i][1] == NULL) {
      printf("eroare alocare rute\n");
      return;
    }
    // citesc primul oras si il caut in graf
    fscanf(file_in, "%s", city1);
    strcpy(routes[i][0], city1);
    if (g->nr_nodes == 0) {
      // daca nu am niciun nod in graf, il adaug pe acesta
      poz1 = 0;
      add_node(city1, g, g->nr_nodes);
    } else {
      // daca am noduri in graf, verific daca il am pe acesta
      poz1 = checkCity(city1, g);
      if (poz1 == -1) {
        // daca nu am orasul in graf
        // adaug orasul la vectorul de orase din graf
        // so initializez lista de adiacenta a orasului
        add_node(city1, g, g->nr_nodes);
        poz1 = g->nr_nodes - 1;
      }
    }
    // cotesc al doilea oras si il caut in graf
    fscanf(file_in, "%s", city2);
    strcpy(routes[i][1], city2);
    poz2 = checkCity(city2, g);
    if (poz2 == -1) {
      // daca nu am orasul in graf, il adaug la vectorul
      // de orase si initializez lista de adiacenta
      add_node(city2, g, g->nr_nodes);
      poz2 = g->nr_nodes - 1;
    }
    fscanf(file_in, "%d", &nrtrons);
    float *grad = malloc(nrtrons * sizeof(float));
    if (grad == NULL) {
      printf("eroare alocare grad\n");
    }
    // citesc gradele de degradare
    for (j = 0; j < nrtrons; j++) {
      fscanf(file_in, "%f", &grad[j]);
    }
    // dupa ce am citit un rand din fisier, aloc muchia in graf
    // adaug in lista de adiacenta a orasului1, orasul2 si gradele
    // de degradare asa cum le-am citit din fisier
    add_edge(poz1, poz2, grad, g, city1, city2, nrtrons);
    float *reversed = reverse(grad, nrtrons);
    // adaug la lista de adiacenta a orasului2, orasul1 si gradele
    // de degradere inversate
    add_edge(poz2, poz1, reversed, g, city2, city1, nrtrons);
    free(grad);
    free(reversed);
  }
  // am terminat de citit din fisier
  int posCity1, posCity2;
  // parcurg de k ori tot graful( k ani )
  for (j = 0; j < k; j++) {
    // parcurg toate rutele din graf
    for (i = 0; i < r; i++) {
      // iau rand cu rand orasele, asa cum le-am citit din fisier
      // verific la ce pozitie se afla in graful meu
      // pozitia returnata indica numele orasului si numarul listei
      posCity1 = checkCity(routes[i][0], g);
      posCity2 = checkCity(routes[i][1], g);
      // degradez muchia dintre cele 2 orase si actualizez drumul in sens invers
      decay(g, posCity1, posCity2);
      updatePaths(g, posCity1, posCity2);
    }
    updateNewDegr(g);
  }
  // afisez in fisierul de out si eliberez memoria
  displayRoutes(g, routes, file_out, r, l);
  for (i = 0; i < r; i++) {
    for (j = 0; j < 2; j++) free(routes[i][j]);
    free(routes[i]);
  }
  free(routes);
  free(city1);
  free(city2);
  free_graph(&g);
}
// functii pentru taskul 2
graph2 *initGraph2(int NumberNodes) {
  // initializez graful cu nr max de moduri
  // similar initGraph
  graph2 *g2 = calloc(1, sizeof(graph2));
  if (g2 == NULL) {
    printf("eroare initialilzare graf2\n");
    return NULL;
  }
  int i;
  g2->numberNodes2 = 0;
  g2->cities = calloc(NumberNodes, sizeof(char *));
  if (g2->cities == NULL) {
    printf("eroare initializare cities\n");
    return NULL;
  }
  g2->adjLists2 = (list2 **)calloc(NumberNodes, sizeof(list2 *));
  if (g2->adjLists2 == NULL) {
    printf("eroare initializare liste\n");
    return NULL;
  }
  for (i = 0; i < NumberNodes; i++) {
    g2->adjLists2[i] = NULL;
    g2->cities[i] = NULL;
  }
  return g2;
}
void freeGraph2(graph2 **g2) {
  // eliberez memoria pt graf
  int i;
  for (i = 0; i < (*g2)->numberNodes2; i++) {
    if ((*g2)->adjLists2 != NULL) {
      node2 *curr = (*g2)->adjLists2[i]->head;
      // eliberez memoria pt fiecare nod din lista
      while (curr != NULL) {
        node2 *temp = curr;
        curr = curr->next;
        free(temp->name2);
        free(temp);
      }
    }
    // eliberez memoria pt fiecare lista
    free((*g2)->adjLists2[i]);
    if ((*g2)->cities[i] != NULL) free((*g2)->cities[i]);
  }

  free((*g2)->adjLists2);
  free((*g2)->cities);
  free(*g2);
}
Heap *initHeap(int capacity) {
  // initializez heapul cu nr de noduri = nr noduri graf
  Heap *h = malloc(sizeof(Heap));
  if (h == NULL) {
    printf("eroare initializare heap\n");
    return NULL;
  }
  h->size = 0;
  h->capacity = capacity;
  h->vector = malloc(capacity * sizeof(nodeHeap));
  if (h->vector == NULL) {
    printf("eorare initializare heap vector\n");
    return NULL;
  }
  return h;
}
Heap *moveUp(Heap *h, int pos) {
  // cat timp tatal nodului de la pozitia pos
  // are costul mai mic decat nodul pos
  // il mut mai in sus
  while (pos > 0 && (h->vector[(pos - 1) / 2].cost >= h->vector[pos].cost)) {
    nodeHeap aux = h->vector[(pos - 1) / 2];
    h->vector[(pos - 1) / 2] = h->vector[pos];
    h->vector[pos] = aux;
    pos = (pos - 1) / 2;
  }
  return h;
}
Heap *insertHeap(Heap *h, char *destination, int cost) {
  // aloc memorie si mai adaug un oras in vectorul de elemente al heapului
  h->vector[h->size].nameCity = malloc(100 * sizeof(char));
  if (h->vector[h->size].nameCity == NULL) {
    printf("eroare insert\n");
    return NULL;
  }
  h->vector[h->size].cost = cost;
  // copiez numele orasului
  strcpy(h->vector[h->size].nameCity, destination);
  // daca heapul are mai multe elemente, incerc sa mut
  // nodul acesta in sus( deoarece l-am adaugat pe ultima poz)
  if (h->size != 0) h = moveUp(h, h->size);
  h->size++;
  return h;
}
void freeHeap(Heap *h) {
  if (h != NULL) {
    free(h->vector);
  }
  free(h);
}
int searchHeap(Heap *h, char *city) {
  int i;
  // verific daca am in heap un anume oras
  for (i = 0; i < h->size; i++) {
    if (strcmp(h->vector[i].nameCity, city) == 0) {
      return i;
      // daca il am, returnez pozitia la care se gaseste
    }
  }
  return -1;
}
Heap *moveDown(Heap *h, int index) {
  // retin pozitiile nodului si al copiilor lui
  int maxIndex = index;
  int left = index * 2 + 1;
  int right = index * 2 + 2;
  // verific daca copilul din stanga exista si daca e mai mic
  // decat parintele
  if (left < h->size && h->vector[left].cost < h->vector[maxIndex].cost)
    maxIndex = left;
  // verific daca copilul din dreapta exista si daca e mai mic
  // decat parintele
  if (right < h->size && h->vector[right].cost < h->vector[maxIndex].cost)
    maxIndex = right;
  // daca unul dintre copii e mai mic decat parintele
  if (maxIndex != index) {
    // interschimb copilul cu parintele
    nodeHeap aux = h->vector[index];
    h->vector[index] = h->vector[maxIndex];
    h->vector[maxIndex] = aux;
    h = moveDown(h, maxIndex);
  }
  return h;
}
nodeHeap extractMin(Heap *h) {
  // extrag primul element din vector
  nodeHeap min = h->vector[0];
  // mut pe prima pozitie cel mai mare elem
  h->vector[0] = h->vector[h->size - 1];
  h->size--;
  h = moveDown(h, 0);
  // returnez cea mai mica pozitie
  return min;
}
int returnPos(graph2 *g2, char *city) {
  // similar cu check_city
  int i;
  for (i = 0; i < g2->numberNodes2; i++) {
    // daca am orasul, returnez pozitia la care se afla
    if (strcmp(g2->cities[i], city) == 0) return i;
  }
  return -1;
}
void addNode2(graph2 *g2, char *city1) {
  // adaug un nou nod in graf
  // aloc memorie pt sirul de caractere si copiez
  g2->cities[g2->numberNodes2] = calloc(100, sizeof(char));
  if (g2->cities[g2->numberNodes2] == NULL) {
    printf("eroare addNode2\n");
    return;
  }
  // aloc memorie pt lista cu vecini a orasului city1
  strcpy(g2->cities[g2->numberNodes2], city1);
  if (g2->adjLists2[g2->numberNodes2] == NULL) {
    g2->adjLists2[g2->numberNodes2] = calloc(1, sizeof(list2));
    if (g2->adjLists2[g2->numberNodes2] == NULL) {
      printf("eroare addNode2 la lista\n");
      return;
    }
    g2->adjLists2[g2->numberNodes2]->head = NULL;
  }
  g2->numberNodes2++;
}
void AddEdge(graph2 *g2, char *city1, char *city2, int cost) {
  int pos = returnPos(g2, city1);
  node2 *new = malloc(sizeof(node2));
  if (new == NULL) {
    printf("eroare AddEdge\n");
    return;
  }
  new->name2 = malloc(100 * sizeof(char));
  if (new->name2 == NULL) {
    printf("eroare AddEdge nume\n");
    return;
  }
  new->cost = cost;
  strcpy(new->name2, city2);
  if (g2->adjLists2[pos]->len == 0) {
    g2->adjLists2[pos]->head = new;
    new->next = NULL;
  } else {
    new->next = g2->adjLists2[pos]->head;
    g2->adjLists2[pos]->head = new;
  }
  g2->adjLists2[pos]->len++;
}
void algorithm(graph2 *g2, char *source, char **parent, int *v, int *keep,
               char ***routes) {
  int i;
  int *visited = malloc(g2->numberNodes2 * sizeof(int));
  if (visited == NULL) {
    printf("eroare algorithm\n");
    return;
  }
  for (i = 0; i < g2->numberNodes2; i++) {
    keep[i] = 0;
    parent[i] = calloc(50, sizeof(char));
    visited[i] = 0;
    // daca nodul nu este nodul sursa, distanta pana la el e infinit
    if (strcmp(g2->cities[i], source) == 0)
      v[i] = 0;
    else
      v[i] = INFINITY;
  }
  int cnt = 0;
  Heap *h = initHeap(g2->numberNodes2);
  int posSource = returnPos(g2, source);
  visited[posSource] = 1;
  strcpy(parent[posSource], source);
  node2 *aux = g2->adjLists2[posSource]->head;
  // pun toti vecinii sursei in heap
  while (aux != NULL) {
    // le modific costul
    v[returnPos(g2, aux->name2)] = aux->cost;
    // le modific parintele
    strcpy(parent[returnPos(g2, aux->name2)], source);
    insertHeap(h, aux->name2, aux->cost);
    aux = aux->next;
  }
  while (h->size > 0) {
    // extrag minimul si ii aflu pozitia din vectorul de orase
    nodeHeap minim = extractMin(h);
    char *numeMin = minim.nameCity;
    int posMin = returnPos(g2, numeMin);
    visited[posMin] = 1;             // il marchez ca fiind vizitat
    if (cnt < g2->k) {               // daca mai pot sa pastrez drumuri
      for (i = 0; i < g2->m; i++) {  // parcurg vectorul de perechi de orase
        // in cautarea perechii formate din orasul extras din heap si parintele
        if (strcmp(parent[posMin], routes[i][0]) == 0 &&
            strcmp(numeMin, routes[i][1]) == 0) {
          keep[i] =
              1;  // daca am gasit perechea, marchez perechea cu 1 pt a o afisa
          cnt++;
          break;
        }
        if (strcmp(numeMin, routes[i][0]) == 0 &&
            strcmp(parent[posMin], routes[i][1]) == 0) {
          keep[i] = 1;
          cnt++;
          break;
        }
      }
    }
    // parcurg toata lista orasului scos din heap
    node2 *auxiliar = g2->adjLists2[posMin]->head;
    while (auxiliar != NULL) {
      int auxiliarPos = returnPos(g2, auxiliar->name2);
      // daca drumul pana la auxiliar e mai scurt prin nodul
      // scos din heap si auxiliarul nu a fost vizitat
      if (visited[auxiliarPos] == 0 &&
          v[auxiliarPos] > v[posMin] + auxiliar->cost) {
        // actualizez distanta si parintele
        v[auxiliarPos] = v[posMin] + auxiliar->cost;
        strcpy(parent[auxiliarPos], g2->cities[posMin]);
        // verific daca nodul se afla in heap
        int found = searchHeap(h, auxiliar->name2);
        // daca nu se afla, il adaug
        if (found == -1)
          insertHeap(h, auxiliar->name2, v[auxiliarPos]);
        else {
          // daca nu se afla, actualizez distanta si il mut mai in sus
          h->vector[found].cost = v[auxiliarPos];
          h = moveUp(h, found);
        }
      }
      auxiliar = auxiliar->next;
    }
    free(numeMin);
  }
  freeHeap(h);
  free(visited);
}
void printOutput(graph2 *g2, char **parent, char ***routes, FILE *file_out,
                 int m, int k, int *v, int *keep) {
  int i;
  int routesKept = 0;
  // calculez cate rute vreau sa pastrez
  for (i = 0; i < g2->m; i++) {
    if (keep[i] == 1) routesKept++;
  }
  // afisez cate rute vreau sa pastrez
  fprintf(file_out, "%d\n", routesKept);
  for (i = 0; i < m; i++) {
    // iau din vectorul de perechi de orase cate o pereche
    char *city1 = routes[i][0];
    char *city2 = routes[i][1];
    // aflu pozitiile celor 2 orase
    int pos1 = returnPos(g2, city1);
    int pos2 = returnPos(g2, city2);
    // daca doresc sa pastrez aceasta pereche, o afisez
    if (keep[i] == 1) {
      if (strcmp(parent[pos1], city2) == 0) {
        fprintf(file_out, "%s %s\n", city1, city2);
      }
      if (strcmp(parent[pos2], city1) == 0) {
        fprintf(file_out, "%s %s\n", city1, city2);
      }
    }
  }
}
void task2(FILE *file_in, FILE *file_out) {
  // retin orasul de start
  char *startCity = malloc(20 * sizeof(char));
  if (startCity == NULL) {
    printf("eroare startCity\n");
    return;
  }
  fscanf(file_in, "%s", startCity);
  int k, m, cost;
  fscanf(file_in, "%d", &k);
  fscanf(file_in, "%d", &m);
  int i;
  char *city1 = malloc(20 * sizeof(char));
  if (city1 == NULL) {
    printf("eroare city1\n");
    return;
  }
  char *city2 = malloc(20 * sizeof(char));
  if (city2 == NULL) {
    printf("eroare city2\n");
    return;
  }
  // retin in routes toate perechile de orase
  // in ordinea in care le am citit din fisier
  char ***routes = malloc(m * sizeof(char **));
  if (routes == NULL) {
    printf("eroare routes\n");
    return;
  }
  // initializez graful cu 2*nr_muchii noduri
  graph2 *g2 = initGraph2(2 * m);
  g2->k = k;
  g2->m = m;
  for (i = 0; i < m; i++) {
    // aloc memorie pt routes si copiez cele 2 orase
    routes[i] = malloc(2 * sizeof(char *));
    if (routes[i] == NULL) {
      printf("eroare routes\n");
      return;
    }
    routes[i][0] = malloc(100 * sizeof(char));
    if (routes[i][0] == NULL) {
      printf("eroare routes\n");
      return;
    }
    routes[i][1] = malloc(100 * sizeof(char));
    if (routes[i][1] == NULL) {
      printf("eroare routes\n");
      return;
    }
    fscanf(file_in, "%s %s", city1, city2);
    fscanf(file_in, "%d", &cost);
    strcpy(routes[i][0], city1);
    strcpy(routes[i][1], city2);
    // verific daca am deja orasul in graf
    // daca nu il am, il adaug
    if (returnPos(g2, city1) == -1) addNode2(g2, city1);
    if (returnPos(g2, city2) == -1) addNode2(g2, city2);
    AddEdge(g2, city1, city2, cost);
    AddEdge(g2, city2, city1, cost);
  }
  int *keep = calloc(g2->m, sizeof(int));
  if (keep == NULL) {
    printf("eroare keep\n");
    return;
  }
  char **parent = malloc(g2->numberNodes2 * sizeof(char *));
  if (parent == NULL) {
    printf("eroare parent\n");
    return;
  }
  int *v = malloc(g2->numberNodes2 * sizeof(int));
  if (v == NULL) {
    printf("eroare v\n");
    return;
  }
  algorithm(g2, startCity, parent, v, keep, routes);
  // afisez rezultatul
  printOutput(g2, parent, routes, file_out, m, k, v, keep);
  // eliberez memoria
  free(v);
  free(city1);
  free(city2);
  free(startCity);
  for (i = 0; i < g2->numberNodes2; i++) free(parent[i]);
  for (i = 0; i < m; i++) {
    free(routes[i][0]);
    free(routes[i][1]);
    free(routes[i]);
  }
  free(keep);
  free(routes);
  free(parent);
  freeGraph2(&g2);
}
int main(int argc, char *argv[]) {
  FILE *file_in;
  FILE *file_out;
  file_in = fopen("tema3.in", "r");
  if (file_in == NULL) {
    printf("eroare deschidere fisier\n");
    return 0;
  }
  file_out = fopen("tema3.out", "w");
  if (file_out == NULL) {
    printf("eroare deschidere fisier\n");
    return 0;
  }
  if (strcmp(argv[1], "1") == 0) task1(file_in, file_out);
  if (strcmp(argv[1], "2") == 0) task2(file_in, file_out);
  fclose(file_in);
  fclose(file_out);
}
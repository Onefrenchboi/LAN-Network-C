#include "algos.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

size_t degre(graphe const *g, sommet s) {
    size_t count = 0;

    for (size_t i = 0; i < g->nb_aretes; i++) {
        arete a = g->aretes[i];
        if (a.s1 == s) {
            count++;
        } else if (a.s2 == s) {
            count++;
        }
    }
    return count;
}

bool est_regulier(graphe const *g) {

    if (g->nb_aretes == 0) {
        return true; //graphe vide = régulier
    }
    size_t degre_ref = degre(g, 0); //on prends le degré du premier sommet
    for (size_t i = 1; i < ordre(g); i++) {
        if (degre(g, i) != degre_ref) {
            return false; //Si c'est diff on dit non
        }
    }
    return true; //sinn oui
}

void afficher(graphe const *g) {
    printf("# Sommets = %zu\n", g->ordre);
    printf("# Aretes = %zu\n", g->nb_aretes);

    printf("--SOMMETS--\n");
    for (size_t i = 0; i < ordre(g); i++) {
        printf("%zu (degré: %zu) <-> ", i, degre(g, i));
        sommet sa[10];
        size_t nb_sommets = sommets_adjacents(g, i, sa);
        for (size_t j = 0; j < nb_sommets; j++) {
            //print sommets adjacents
            printf("%zu ", sa[j]);
        }
        
        printf("\n");
        
    }
    printf("--ARETES--\n");
    for (size_t i = 0; i < g->nb_aretes; i++) {
        arete a = g->aretes[i];
        printf("%zu - %zu \n", a.s1, a.s2);
    }



}

void generer_complet(graphe *g, size_t ordre) {
    for (size_t i = 0; i < ordre; i++) {
        ajouter_sommet(g); //on ajoute les sommets
    }
    for (size_t i = 0; i < ordre; i++) {
        for (size_t j = i + 1; j < ordre; j++) {
            arete a = {i, j}; //on crée l'arete
            ajouter_arete(g, a); //on l'ajoute
        }
    }
}

void visite_composante_connexe(graphe const *g, sommet s, bool *visite) {
    visite[s] = true; //bah le premier sommet est visité
    for (size_t i = 0; i < g->nb_aretes; i++) { //on boulce tsais
        arete a = g->aretes[i]; //on prend la premiere arete
        if (a.s1 == s && !visite[a.s2]) { //si le sommet c'est le bon et qu'on a pas visité l'autre
            visite_composante_connexe(g, a.s2, visite); //bah on lance la visite mais sur le deuxieme
        } else if (a.s2 == s && !visite[a.s1]) { // bon la faut aps faire chier t'as compris 
            visite_composante_connexe(g, a.s1, visite); 
        }
    }
}

uint32_t nb_composantes_connexes(graphe const *g) {
    bool *visite = malloc(sizeof(bool) * ordre(g)); //on cree le tableau histoire d'utilser le truc au dsus la
    for (size_t i = 0; i < ordre(g); i++){
        visite[i] = false;
    }

    uint32_t nb_de_con = 0; //oui j'ai 4 ans et alors
    for (size_t i = 0; i < ordre(g); i++) {
        if (!visite[i]){ //si on a pas visité
            nb_de_con++; //on increment et on visite  
            visite_composante_connexe(g, i, visite);
        }
    }
   
    free(visite);
    visite = NULL;
    return nb_de_con;
}

bool sont_connectes(graphe const *g, sommet s1, sommet s2) {
    bool *visite = malloc(sizeof(bool) * ordre(g)); //on cree le tableau histoire d'utilser le truc au dsus du dsus la
    for (size_t i = 0; i < ordre(g); i++){
        visite[i] = false;
    }
    visite_composante_connexe(g, s1, visite);
    bool connected = visite[s2];
    free(visite);
    visite=NULL;
    return connected;

}

void coloriage_glouton(graphe const *g, uint8_t *couleur_sommet) {
    for (size_t i = 0; i<ordre(g);i++){
        couleur_sommet[i]=255;
    } //on initialize tt a 255


    uint8_t couleur_max = 0;
       

    for (size_t i = 0; i < ordre(g); i++) {
        //malloc du truc pour les couleurs used
        uint8_t *used_colors = malloc(sizeof(uint8_t) * (couleur_max + 1)); 
        

        //on mets tout a 0
        for (size_t j = 0; j < ordre(g); j++) {
            used_colors[j] = 0;
        }
        

        //on va lire les sommets adjacents, donc on fait un tab pour ca
        sommet sa[degre(g, i)];
        size_t nb_sommets = sommets_adjacents(g, i, sa);
        for (size_t j = 0; j < nb_sommets; j++) {
            //on prend le sommets, et sa couleur
            sommet v = sa[j];
            uint8_t c = couleur_sommet[v];
            //si c'est pas 255 (donc déja used), on dit que la couleur est déjà used
            if (c != 255) {
                used_colors[c] = 1;
            }
        }


        size_t couleur = 0;
        //tant que la couleur est pas used bah on monte
        while (used_colors[couleur]) {
            couleur++;
        }
        //on la pose au sommet
        couleur_sommet[i] = couleur;
        //si c'est plus gros, on monte la couleur max
        if (couleur > couleur_max) {
            couleur_max = couleur;
        }
        //on free
        free(used_colors);
    }

}

void appliquer_permutation(graphe const *src, graphe *dst, size_t const *permutation) {
    //on fout autant de sommets
    for (size_t k = 0; k < ordre(src);k++){
        ajouter_sommet(dst);
    }

    //douvle boucle pour verif si un existe arete, et si oui bah on la remet mais avec les trucs permutés
    for (size_t i = 0; i < ordre(src); i++) {
        for (size_t j = i + 1; j < ordre(src); j++) {
            arete a = {i, j};
            if (existe_arete(src, a)) {
                arete b = {permutation[i], permutation[j]};
                ajouter_arete(dst, b);
            }
        }
    }
}

uint32_t estimation_nb_chromatique(graphe const *g, uint32_t n) {
    return 0;
}

void generer_aleatoire(graphe *g, size_t ordre, uint32_t k) {}


void dijkstra(graphe const *g, sommet s, double const *poids_arete, double *distance_sommet) {
    
    //on commence par toutes les dist = inf  (ici dblmax)
    for (size_t i = 0; i < ordre(g); i++) {
        distance_sommet[i]= DBL_MAX;
    }

    //la distance du sommet duquel on part bah c 0
    distance_sommet[s]=0;

    //tab des visites
    bool *visite = malloc(sizeof(bool) * ordre(g)); 
    for (size_t i = 0; i < ordre(g); i++){
        visite[i] = false;
    }

    for (size_t i = 0; i < ordre(g); i++) {
        //on trouve le sommet pas visité le plus piti
        double distmin = DBL_MAX;
        sommet first = 0;
        for (size_t i = 0; i < ordre(g); i++) {
            if (!visite[i] && distance_sommet[i] < distmin) { //si pas visité et la distance vers lui est plus ptite, alors on le prend
                distmin = distance_sommet[i];
                first = i;
            }
        }
        visite[first] = true; //on l'a visité (on y est ("ca y est, nous y est" type shi))


        //on chope les voisins
        sommet sa[degre(g, first)];
        size_t nb_sommets = sommets_adjacents(g, first, sa); 

        for (size_t j = 0; j < nb_sommets; j++) {
            //pr tt les voisins, on prends un des sommets, on crée une arete avec
            sommet second = sa[j];
            arete a = {first, second};
            //on prend son poid correspondant 
            double poids = poids_arete[index_arete(g, a)];
            //si son poid est pas inf et que la dist vers le premier + le poid est plus ptite que la distance du second
            //(en gros on verif que c pas plus rapide de faire le détour)
            if (poids != DBL_MAX && distance_sommet[first] + poids < distance_sommet[second]) {
                distance_sommet[second] = distance_sommet[first] + poids;
            }
        }

    }
    free(visite);
    visite=NULL;
}

void trier_aretes(arete *aretes_triees, graphe const *g, double const *poids_arete) {
    //bon c'est un tri par insertion hein fais pas chier
    for (size_t i = 0; i < nb_aretes(g); i++) {
        aretes_triees[i]=g->aretes[i];        
    }
    for (size_t i = 1; i < nb_aretes(g); i++) {
        arete a = g->aretes[i];
        double poids = poids_arete[index_arete(g, a)];
        int j = i - 1;

        while (j >= 0 && poids_arete[index_arete(g, aretes_triees[j])] > poids) {
            aretes_triees[j + 1] = aretes_triees[j];
            j--;
        }

        aretes_triees[j + 1] = a;
    }

}

bool est_arbre_couvrant(graphe const *g) {
    //on fait ca parce que grosse flemme de mettre tout ca dns krukral ca fait pas bo
    size_t nb_sommets = ordre(g);
    size_t nb_aretes = g->nb_aretes;

    if (nb_aretes != nb_sommets - 1) {
        return false; //pas assez de trucs
    }
    if (nb_composantes_connexes(g) != 1) {
        return false; //pas connexe
    }
    return true;
}

void kruskal(graphe const *g, double const *poids_arete, graphe *acm) {

    //on mets autant de sommets
    for (size_t i = 0; i < ordre(g); i++) {
        ajouter_sommet(acm);
    }    

    //on trie les aretes par poids
    arete* aretes_triees = malloc(sizeof(arete) * nb_aretes(g)); 
    trier_aretes(aretes_triees, g, poids_arete);

    size_t i = 0;   
    //tant que l'arbre est pas couvrant (la fonction utilise juste nb_composantes_connexe et compte le nb d'aretes)
    while (!est_arbre_couvrant(acm) && i < nb_aretes(g)) {
        arete a = aretes_triees[i];
        //si ca crée pas de cycle, on l'ajoute
        if (!sont_connectes(acm,a.s1,a.s2)){
            ajouter_arete(acm, a);
        }
        i++;
    }
    free(aretes_triees);
    aretes_triees=NULL;
}
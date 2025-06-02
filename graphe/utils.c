#include "utils.h"

void generer_permutation(size_t *permutation, size_t n)
{
    //on gen le tableau de 0 a n
    for (size_t i = 0; i < n; i++) {
        permutation[i] = i;
    }


    //et en gros blabla boucle qui va gen un random entre 0 et taille-1, et va swap avec un autre
    for (size_t i = 0 ; i < n; i++) {
        size_t j = rand() % n;
        size_t temp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = temp;
    }
}

void init_aleatoire(double *val, size_t n, double max){
    for (size_t i = 0; i<n; i++){
        //on prend 2 randoms, la premiere crée un truc entre 0 et 1 qu'on cast en double, puis on le 
        //mult par max pour l'avoir dans l'intervalle qu'on veut
        val[i] = ((double)rand() / (double)RAND_MAX) * max; 
    }

}
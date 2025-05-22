#include "algos.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

size_t degre(graphe const *g, sommet s)
{
  if (index_sommet(g, s) == UNKNOWN_INDEX)
  {
    return -1;
  }
  size_t degre = 0;
  for (size_t i = 0; i < ordre(g); i++)
  {
    arete a;
    a.s1 = s;
    a.s2 = i;
    if (existe_arete(g, a))
    {
      degre += 1;
    }
  }
  return degre;
}

bool est_regulier(graphe const *g)
{
  size_t d = degre(g, 0);
  for (size_t i = 1; i < ordre(g); i++)
  {
    if (degre(g, i) != d)
    {
      return false;
    }
  }
  return true;
}

void afficher(graphe const *g)
{
  /* 
  #sommets = 5
 #arêtes =5--SOMMETS-
0(degré: 2)<->23
 1(degré: 1)<->4
 2(degré: 2)<->03
 3(degré: 3)<->024
 4(degré: 2)<->13--ARÊTES-
0-2
 0-3
 1-4
 2-3
 3-4
 */
  printf("# sommets = %zu\n", ordre(g));
  printf("# arêtes = %zu\n", nb_aretes(g));
  printf("--SOMMETS--\n");
  for (size_t i = 0; i < ordre(g); i++)
  {
    printf("%zu (degré: %zu) <-> ", i, degre(g, i));
    for (size_t j = 0; j < ordre(g); j++)
    {
      arete a;
      a.s1 = i;
      a.s2 = j;
      if (existe_arete(g, a))
      {
        printf("%zu ", j);
      }
    }
    printf("\n");
  }
  printf("--ARÊTES--\n");
  for (size_t i = 0; i < ordre(g); i++)
  {
    for (size_t j = i + 1; j < ordre(g); j++)
    {
      arete a;
      a.s1 = i;
      a.s2 = j;
      if (existe_arete(g, a))
      {
        printf("%zu-%zu\n", a.s1, a.s2);
      }
    }
  }
  printf("\n");
}

void generer_complet(graphe *g, size_t ordre)
{
    for (size_t i = 0; i < ordre; i++)
    {
      ajouter_sommet(g);
    }
    for (size_t i = 0; i < ordre; i++)
    {
      for (size_t j = 0; j < ordre; j++)
      {
        arete a;
        a.s1 = i;
        a.s2 = j;
        ajouter_arete(g, a);
      }
    }
}

void visite_composante_connexe(graphe const *g, sommet s, bool *visite)
{
  visite[s] = true;
  for (size_t i = 0; i < ordre(g); i++)
  {
    arete a;
    a.s1 = s;
    a.s2 = i;
    if (existe_arete(g, a) && !visite[i])
    {
      visite_composante_connexe(g, i, visite);
    }
  }
}

uint32_t nb_composantes_connexes(graphe const *g)
{
    bool *visite = malloc(sizeof(bool) * ordre(g));
    for (size_t i = 0; i < ordre(g); i++)
    {
      visite[i] = false;
    }
    size_t n = 0;
    for (size_t i = 0; i < ordre(g); i++)
    {
      if (!visite[i])
      {
        visite_composante_connexe(g, i, visite);
        n++;
      }
    }
    free(visite);
    visite = NULL;
    return n;
}

bool sont_connectes(graphe const *g, sommet s1, sommet s2)
{
    bool *visite = malloc(sizeof(bool) * ordre(g));
    for (size_t i = 0; i < ordre(g); i++)
    {
      visite[i] = false;
    }
    visite_composante_connexe(g, s1, visite);
    if (visite[s2])
    {
      return true;
    }
    else
    {
      return false;
    }
    free(visite);
    visite = NULL;
    return false;
}

void couleur_max_voisins(graphe const *g, uint8_t *couleur_sommet, sommet s, uint8_t *couleur_max)
{
    for (size_t i = 0; i < ordre(g); i++)
    {
      if (sont_connectes(g, s, i))
      {
        if (couleur_sommet[i] == 255)
        {
          return;
        }
        
      }
    }
}

void coloriage_glouton(graphe const *g, uint8_t *couleur_sommet)
{
    for (size_t i = 1; i < ordre(g); i++)
    {
      couleur_sommet[i] = 255;
    }
    uint8_t couleur_max = 0;
    uint8_t *nb_couleurs = malloc(sizeof(uint8_t) * couleur_max + 1);
    for (size_t i = 0; i < ordre(g); i++)
    {
    }
}

void appliquer_permutation(graphe const *src, graphe *dst, size_t const *permutation)
{
}

uint32_t estimation_nb_chromatique(graphe const *g, uint32_t n)
{
    return 0;
}

void generer_aleatoire(graphe *g, size_t ordre, uint32_t k)
{
}

void dijkstra(graphe const *g, sommet s, double const *poids_arete, double *distance_sommet)
{
}

void trier_aretes(arete *aretes_triees, graphe const *g, double const *poids_arete)
{
}

void kruskal(graphe const *g, double const *poids_arete, graphe *acm)
{
}

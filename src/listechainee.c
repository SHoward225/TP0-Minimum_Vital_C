/**
   Copyright (C) 2016 by Gregory Mounie

   This file is part of RappelDeC

   RappelDeC is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.


   RappelDeC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct elem {
  long unsigned int val;
  struct elem *next;
};

/* ================ Affiche les éléments de la liste passée en paramètre sur la sortie standard.================*/

void affichage_liste(struct elem const *const liste) {
  const struct elem *current=liste; // Parcourir la liste et afficher chaque élément

  while (current != NULL){
    printf("%lu -> ", current->val); //Affiche la valeur de l'élément
    current = current->next; // Passer à l'élément suivant
  }

  printf("NULL\n"); //Fin de la liste

}

/** /*================ Libère toute la mémoire associée à la liste passée en paramètre.  ================

   @param liste head list, pointer on first element, not NULL
*/

void destruction_liste(struct elem liste[static 1]) {

  struct elem *current = liste;   //Pointeur pour parcourir la liste
  struct elem *next_elem;         //Pointeur pour stocker l'élément suivant

  while (current != NULL) {
    next_elem = current->next;               // Sauvegarde l'élément suivant
    free(current);                           // Libère l'élément actuel
    current = next_elem;                     // Passe à l'élément suivant
  }
}

/*================ Crée une liste simplement chainée à partir des nb_elems éléments du tableau valeurs.  ======================*/
struct elem *creation_liste(size_t nb_elems, long unsigned int const valeurs[nb_elems]) {
  
  if(nb_elems ==0){
    return NULL; // Si aucun élément, retourne NULL
  }

  // Allouer la tête de la liste
  struct elem *head = malloc(sizeof(struct elem));
  if (!head){
    return NULL; // Vérifier si l'allocation a réussi
  }

  head->val = valeurs[0];
  head->next = NULL;

  // Pointeur pour construire la liste
  struct elem *current = head;
  
  // Boucle pour ajouter les autres élements
  for (size_t i=1; i<nb_elems; i++){
    struct elem *new_elem = malloc(sizeof(struct elem));
    if (!new_elem) {
      // Si l'allocation échoue, libérer la mémoire déjà allouée
      destruction_liste(head);
      return NULL;
    }
    new_elem->val = valeurs[i];  // Assigner la valeur
    new_elem->next = NULL;       // Initialiser à NULL
    
    current->next = new_elem;    // Relier l'élément précédent au nouveau
    current = new_elem;          // Avancer au nouvel élément
  }

  return head;  // Retourner la tête de la liste

}

/**================================================================================================================
 * Inverse la liste simplement chainée passée en paramètre. Le
 * paramètre liste contient l'adresse du pointeur sur la tête de liste
 * à inverser.
 * @param liste head list address, not NULL
 * ================================================================================================================
 */
void inversion_liste(struct elem *liste[static 1]) {

  struct elem *prev = NULL;      // Pointeur vers l'élément précédent (au début NULL)
  struct elem *current = *liste; // Pointeur vers l'élément courant (initialisé à la tête)
  struct elem *next = NULL;      // Pointeur vers l'élément suivant
  
  while (current != NULL){
    next = current->next;   // Sauvegarder l'élément suivant
    current->next = prev;   // Inverser le lien : l'élément courant pointe vers le précédent
    prev = current;         // Avancer le pointeur prev à l'élément courant
    current = next;         // Avancer current à l'élément suivant
  }

  *liste = prev;  // Mise à jour de la tête de liste (qui devient le dernier élément)

}


/* ============================= Fonction de verification d'inversion ================================ */

void debug_inversion(struct elem *h1, struct elem *h2) {
    printf("head1= %p, head2= %p\n", h1, h2);  // Affiche les adresses des têtes de liste
    affichage_liste(h1);  // Affiche la première liste
    affichage_liste(h2);  // Affiche la deuxième liste
}


const long unsigned int TAILLE = 100;
const long unsigned int TAILLE_GRANDE = 1000000;



int main(void) {
  /* Test d'affichage d'une liste créée à la main. */
  struct elem e1, e2, e3;
  e1.val = 0;
  e2.val = 1;
  e3.val = 2;
  e1.next = &e2;
  e2.next = &e3;
  e3.next = NULL;
  affichage_liste(&e1);

  long unsigned int valeurs[TAILLE];
  for (size_t i = 0; i < TAILLE; i++) {
    valeurs[i] = i;
  }

  struct elem *tab_elem = creation_liste(TAILLE, valeurs);
  assert(tab_elem != NULL);

  /* Test que la liste dans l'ordre */
  struct elem *tmp;
  long unsigned int idx;
  for (tmp = tab_elem, idx = 0; tmp != NULL; idx++, tmp = tmp->next) {
    assert(tmp->val == idx);
  }
  /* Test que la liste est complète (à la suite du test sur l'ordre) */
  assert(idx == TAILLE);

  printf("Liste créée:\n");
  affichage_liste(tab_elem);
  inversion_liste(&tab_elem);
  assert(tab_elem != NULL);
  printf("Liste inversée:\n");
  affichage_liste(tab_elem);

  /* Vérifie que la liste est inversée et complète */
  for (tmp = tab_elem, idx = TAILLE - 1; tmp != NULL; --idx, tmp = tmp->next) {
    assert(tmp->val == idx);
  }
  assert(idx == (long unsigned int)(0UL - 1));

  /* Pour tester le bon fonctionnement de la fonction suivante, on
   * lancera valgrind sur le programme pour vérifier qu'aucune fuite
   * mémoire n'est présente. */
  destruction_liste(tab_elem);

  /* vérification avec une liste très longue (1M) élément pour faire
   * exploser les implémentations récursives (taille de la pile == 8Mo) */
  long unsigned int *valeurs_nombreuses = malloc(sizeof(long unsigned int[TAILLE_GRANDE]));
  for (size_t i = 0; i < TAILLE_GRANDE; i++) {
    valeurs_nombreuses[i] = i;
  }

  tab_elem = creation_liste(TAILLE, valeurs_nombreuses);
  assert(tab_elem != NULL);
  destruction_liste(tab_elem);

  // Libérer la mémoire allouée pour le tableau valeurs_nombreuses
  free(valeurs_nombreuses);
  
  return EXIT_SUCCESS;
}

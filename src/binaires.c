#include <stdio.h>
#include <stdlib.h>

static unsigned long long X = 123456ULL;

unsigned char crand48(void) {
  const unsigned long long a = 0x5DEECE66DULL;  // Coefficient multiplicatif
  const unsigned long long c = 0xBULL;          // Terme constant
  const unsigned long long m = 1ULL << 48;      // m = 2^48

  // Calcul de Xn+1
  X = (a * X + c) % m;

  // Extraire les bits 32 à 39 (en partant du bit de poids faible) et retourner le résultat
  return (X >> 32) & 0xFF;  // Décalage de 32 bits vers la droite et masquage des 8 bits (0xFF)
}

int main(void) {
  printf("%hhu\n", crand48());
  printf("%hhu\n", crand48());
  printf("%hhu\n", crand48());

  return EXIT_SUCCESS;
}

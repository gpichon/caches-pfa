23/10/2013
1re rencontre -> D. Barthou

Explications sur les caches.
Attentes du produit.


==> Pour la prochaine fois :

- utiliser CacheGrind/Hwloc/MAQAO
- rafraîchissement sur les caches
- rencontre le 4 novembre
  14h30; envoyer mail

==> Notre outil à développer

- outil par forcément pour les experts
- Nous on se place au niveau de la simulation.
- utilisation de MAQAO
- outil d'analyse des traces
- dans un 1er temps, on doit pouvoir compter les cache-miss
  (d'abord avec un seul cache/coeur)

- module de lecture de la trace
- module de gestion de l'architecture des caches
- module de gestion des boucles ? optimisation 
  pour éviter d'éxuter 1000 fois une itération


==> Hwloc

Décrit l'architecture des caches.
API pour le C.

==> MAQAO

Collecte les traces d'éxécutions et "compresse" les données.

==> CacheGrind (simulation)

Exécute le code sur une machine virtuelle par coeur.
Il vérifie/indique essentiellement les cache-hit/miss.
Problème, basé sur valgrind qui ne gère pas le multi-thread :
chaque trhead est émulé l'un APRES l'autre.

==> Cashesim (simulation)

Pas très évident à gérer, car il faut spécifier son architecture

==> Exécution directe = compteur hardware (+ VTune pour Intel)
Permet de définir des compteurs (de cache-miss) par exemple
mais ces compteurs sont très variés selon les architectures.

PAPI : utilisation gratuite pendant un certain temps
VTUNE : //


==> Exemple de code très mauvais :
1er thread : for(i) A[2i]
2me thread : for(i) B[2i+1]

==> Benchmarks (fournissent des codes)

- NAS//Benchmark
- Polybench
- Parsec benchmark

==> OpenMP
#pragma omp parallel for
for(i,N)

Permet de parallèliser du code

==> HexaScale (ortho ?)
Possibilité de stage ...(versailles)
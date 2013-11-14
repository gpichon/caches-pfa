programmation :
- base en c
- algorithme en lua (tout ce qui peut changer entre 2 exécutions)

32/64 bits :
coder pour différentes tailles d'adresses (pointeurs)

Parallélisme :
/!\ intercroisement des threads
combinatoire simple au début : 1/2

Faire un prototype rapidement
- se baser sur le prototype pour réfléchir sur l'architecture
- ne rien optimiser, mettre des choses en dur pour aller plus vite
- à tester sur des petits programmes puis sur un pluis grand pour
savoir où et comment améliorer le code

Séparer le travail mais pas trop en profondeur :
- modélisation des caches
- analyse des traces de maqao
- opérations sur les caches
- exécuter des fichiers lua

N'optimiser le code que dans l'ordre d'importance
(ie le code qui prend le plus de temps en premier)

Peer programming ?
Une personne code, une autre vérifie ce que fait celle qui code.
Cela permet d'éviter des erreurs bête (tiens cette fonction existait
déjà) et d'avoir une meilleure vision de l'ensemble du code.
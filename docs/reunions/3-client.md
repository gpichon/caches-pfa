18 nov 2013 -- 3me rencontre Barthou

------- Réponses aux questions ---------

0) réutiliser le format d'entrée de HWLoc (en XML, par le biais de libXML)

1) lignes de caches partout identiques, passée en paramètre

2) on va jamais voir dans les caches voisins de même niveau (cela ne change rien en terme de miss/hit car cf 3).)

3) on suppose que le dernier cache (L3) est toujours inclusif

4) tant mieux si on fait une architecture pas symétrique (mais argumenter si on peut pas le faire ...)

5) on dépose pas les données dans les caches non inclusifs

6) cohérence : faire MSI ou MESI

7) le write-through/back est un paramètre pour chaque niveau de caches voire chaque cache

8) politique de remplacement différente sur chaque cache/niveau : en option


------- Exemples de traces ----------

Rappel : une trace par thread

------- Métrique ---------

Rappel : nb de hit/miss par cache/thread

------- Test de notre programme --------

Par de la mesure hardware (si le code est assez long en temps)
 avec l'outil PAPI par exemple.

------ Nouveau problème --------------

Comment est-ce qu'on lit les traces maqao ?
Par scriptage lua (déroulage des boucles) ou alors par interprétation directe des boucles.

----- Prochain rdv ------

Lundi 25/11 à 14h INRIA

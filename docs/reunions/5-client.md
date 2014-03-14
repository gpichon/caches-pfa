Réunion du 16/12/2013

Dans le cahier des charges, il faudrait ajouter:
- ex de trace
- ex de fichier xml de hwloc
- ex de fichier xml transformé

Préciser les différentes politiques d'entrelacement des threads:
- 1 par un
- thread par thread
- x d'un thread, y d'un autre etc...

Une execution doit pouvoir être rejouée.
Si introduction d'aléatoire -> plutot du pseudo aléatoire reproductible.

Quand un L1 fait un miss:
      soit snooping sur le bus avec les autres L1
      soit L2 demande aux autres L1 et on continue de meme avec le L3

L1i pas génant car utile seulement au début de l'execution du programme (code compact)

Un cache inclusif l'est pour tous les caches en dessous.

Quand on écrit, on remet à jour dans les caches de niveau supérieur. Pour nous, on change juste le flag modified et les flages update.

Avec un cache non inclusif, faire du WB n'est plus obligatoire -> intéret de l'état owned.

L'état F de MESIF est utile au niveau des bus, il ne sert donc à rien de le considérer dans notre étude.


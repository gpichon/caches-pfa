Compte rendu séance 27/11/2013

Utiliser le format .md pour les comptes rendus

Faire des "override": notion d'include
On inclut dans notre fichier de config, les confs de hwloc

Si valeur dans xml on prend valeur, sinon dans config par défault, sinon en dur

Définir un "contrat" pour les objets, les pointeurs de fonction, comme flags, flags_new_line...

Flags_new_line -> set_flags_new_line: pour plus de clarté dans le code

Rajouter des fonctions, par ex à la place de current->cache->writes_back++
Avec des macros cela peut etre efficace
Cela évite de faire des setters incompréhensibles partout

Générateur d'automates: ragel
Génére des fichiers .c
Permet de mettre des actions sur des transitions / des états...
Permet d'abstraire un peu plus

Ajouter un pointeur de fonction dans share_level à la place de modify

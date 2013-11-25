Réunion 25/11/2013

On ne va pas gérer les architectures hétérogènes.

Gérer les threads en lua?
Une trace par thread/processeur
Trace = acces aux coeurs
Configurer l'ordenancement des traces par coeur

L1d et L1i?
On ne fait rien sur le L1i

Caches inclusifs ou non?
Sur intel64, L3 inclusifs dans L2 et L1
L2 pas inclusif par rapport au L1

En cas de miss, on ne laisse pas la donnée sur le cache non inclusif (en l'occurence L2)

Implementation en mode snooping bus ne sert pas à grand chose.
On peut supposer que le L3 contient toutes les données

Protocoles de coherence:
MESI, MSI
MESIF, MUESI
Intel sandy bridge: inclusif -> L3 inclus L1
      	    	    une donnée ramenée dans le L1 est aussi dans le L3 avec une info sur la localisation

Les dernières données sont dans le cache L1.

Quand on supprime des lignes de caches (avec dirty bit à 1: donnée modifiée):
      On met à jour (recopie) en mémoire: c'est le seul moment où il y a du trafic mémoire

Selon le comportement des caches:
      Si L3 inclusif: la recopie en mémoire se fait quand on supprime du L3 pas du L1
      On peut recopier la ligne de cache L1 supprimée dans L2 (alors pas inclusif)


Fonctionnement entre les coeurs:
Si une meme ligne dans 2 L1 de coeurs différents: coherence mécanique lors d'une écriture

Protocole de cohérence: MESI
	  Chaque ligne de cache est dans un état différent: M, E, S ou I
	  	 M: Modified donnée modifiée
		 E: Exclusif dans un seul cache L1: evite le broadcast avant une eventuelle ecriture
		 S: Sharded partagée entre plusieurs coeurs
		 I: Invalid il ne faut pas utiliser la donnée
Serialization au niveau des bus.

Sur la plupart des machines: caches misses non bloquant: les instructions peuvent continuer à s'executer

-> Base: une trace mémoire par coeur

-> Base à faire absolument

1 cache: taille, associativité, LRU      
  	 lire donnée, écrire donnée

hierarchie de caches: caches inclusifs ou non, politque de remplacement

caches multicoeurs: ajout d'un protocole de cohérence (MESI Wikipedia)
       		    comment tournent les coeurs? ordre

Suppostion: programme multicoeurs sans bug
	    neglige les interruptions: core0 plus long (souvent)
	    on suppose qu'il n'y a pas de synchronisations: pas d'info sur comment s'entrelacent les load/store
	    metrique simple: hit/miss par niveau de cache

I) Réflechir
   entrelacement codes par coeur
   representation des donnees

II) Qu'est ce qui sort de l'implémentation
    hit/miss par niveau de cache
    	     par instruction
    visualisation des métriques: graphique?

III) Plus de sport: optimisation
     la simulation va prendre du temps
     actuellement, tout le monde deroule les données
     ensembles d'adresses parcourues
     

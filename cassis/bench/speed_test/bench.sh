#!/bin/bash

echo "Sans snooping, tous les caches inclusifs"
./../../bin/cassis -f ../architectures/architest.cassis_1.xml -t 4 -v 4 -r ../../MAQAO

echo "Sans snooping, avec un L2 exclusif"
./../../bin/cassis -f ../architectures/architest.cassis_2.xml -t 4 -v 4 -r ../../MAQAO

echo "Sans snooping, avec un L2 non inclusif"
./../../bin/cassis -f ../architectures/architest.cassis_3.xml -t 4 -v 4 -r ../../MAQAO

echo "Sans snooping, avec un L3 exclusif"
./../../bin/cassis -f ../architectures/architest.cassis_4.xml -t 4 -v 4 -r ../../MAQAO

echo "Avec snooping niveau L1, niveau L2 non inclusif"
./../../bin/cassis -f ../architectures/architest.cassis_6.xml -t 4 -v 4 -r ../../MAQAO

echo "Avec snooping niveau L2, niveau L2 non inclusif"
./../../bin/cassis -f ../architectures/architest.cassis_7.xml -t 4 -v 4 -r ../../MAQAO

echo "Avec snooping tous les niveaux, niveaux L2 non inclusif"
./../../bin/cassis -f ../architectures/architest.cassis_8.xml -t 4 -v 4 -r ../../MAQAO

echo "Avec snooping tous les niveaux et L2 exclusif"
./../../bin/cassis -f ../architectures/architest.cassis_9.xml -t 4 -v 4 -r ../../MAQAO

echo "Avec snooping tous les niveaux et L2 oriente exclusif"
./../../bin/cassis -f ../architectures/architest.cassis_10.xml -t 4 -v 4 -r ../../MAQAO


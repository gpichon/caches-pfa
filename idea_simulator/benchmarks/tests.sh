#!/bin/bash
cd ..
make

echo "Sans snooping, tous les caches inclusifs"
./bin/simulator benchmarks/architectures/architest.cassis_1.xml

echo "Sans snooping, avec un L2 exclusif"
./bin/simulator benchmarks/architectures/architest.cassis_2.xml

#echo "Sans snooping, avec un L2 non inclusif"
#./bin/simulator benchmarks/architectures/architest.cassis_3.xml
#
#echo "Sans snooping, avec un L3 exclusif"
#./bin/simulator benchmarks/architectures/architest.cassis_4.xml
#
#echo "Sans snooping, avec un L3 non inclusif"
#./bin/simulator benchmarks/architectures/architest.cassis_5.xml

echo "Avec snooping niveau L1, niveau L2 non inclusif"
./bin/simulator benchmarks/architectures/architest.cassis_6.xml

echo "Avec snooping niveau L2, niveau L2 non inclusif"
./bin/simulator benchmarks/architectures/architest.cassis_7.xml

#echo "Avec snooping tous les niveaux, niveaux L2 non inclusif"
#./bin/simulator benchmarks/architectures/architest.cassis_8.xml

echo "Avec snooping tous les niveaux et L2 exclusif"
./bin/simulator benchmarks/architectures/architest.cassis_9.xml

#echo "Avec snooping tous les niveaux et L2 oriente exclusif"
#./bin/simulator benchmarks/architectures/architest.cassis_10.xml

cd benchmarks

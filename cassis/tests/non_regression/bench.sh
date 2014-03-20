#!/bin/bash
cd ../..

echo "Sans snooping, tous les caches inclusifs"
./bin/simulator -f tests/architectures/architest.cassis_1.xml -t 4 -v 4 -r tests/non_regression

echo "Sans snooping, avec un L2 exclusif"
./bin/simulator -f tests/architectures/architest.cassis_2.xml -t 4 -v 4 -r tests/non_regression

echo "Sans snooping, avec un L2 non inclusif"
./bin/simulator -f tests/architectures/architest.cassis_3.xml -t 4 -v 4 -r tests/non_regression

echo "Sans snooping, avec un L3 exclusif"
./bin/simulator -f tests/architectures/architest.cassis_4.xml -t 4 -v 4 -r tests/non_regression

echo "Sans snooping, avec un L3 non inclusif"
./bin/simulator -f tests/architectures/architest.cassis_5.xml -t 4 -v 4 -r tests/non_regression

echo "Avec snooping niveau L1, niveau L2 non inclusif"
./bin/simulator -f tests/architectures/architest.cassis_6.xml -t 4 -v 4 -r tests/non_regression

echo "Avec snooping niveau L2, niveau L2 non inclusif"
./bin/simulator -f tests/architectures/architest.cassis_7.xml -t 4 -v 4 -r tests/non_regression

echo "Avec snooping tous les niveaux, niveaux L2 non inclusif"
./bin/simulator -f tests/architectures/architest.cassis_8.xml -t 4 -v 4 -r tests/non_regression

echo "Avec snooping tous les niveaux et L2 exclusif"
./bin/simulator -f tests/architectures/architest.cassis_9.xml -t 4 -v 4 -r tests/non_regression

echo "Avec snooping tous les niveaux et L2 oriente exclusif"
./bin/simulator -f tests/architectures/architest.cassis_10.xml -t 4 -v 4 -r tests/non_regression

cd tests/non_regression

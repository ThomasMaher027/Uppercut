# Uppercut
Voici le code pour une carte [OpenRB-150 de Robotis](https://emanual.robotis.com/docs/en/parts/controller/openrb-150/) pour contrôler les moteurs Dynamixels. L'ensemble est configuré pour être utilisé avec l'IDE d'Arduino.

Le code principale est situé dans [control_moteur.ino](https://github.com/ThomasMaher027/Uppercut/blob/main/Arduino/ArduinoIDE/controle_moteur/controle_moteur.ino).

Il y a 2 modes de fonctionnement : temps réel et choix de mouvement. Pour choisir le mode, commenter/décommenter la fonction loop() appropriée.

## Personnalisation
Certaines variables du programme qui peuvent/doivent être modifiées selon le contexte d'utilisation. En voici la liste :
- nb_moteur (controle_moteur.ino, mouv_prenregistres.cpp et real_time.cpp)
- DXL_ID : identidiant des moteurs Dynamixel. Faire attention à l'ordre dans lequel les moteurs sont initiés (controle_moteur.ino, mouv_prenregistres.cpp et real_time.cpp)
- vit_max (real_time.cpp)
- vit_min (real_time.cpp)
- nb_mouvement : indique le nombre de mouvements préenregistrés totaux (mouv_prenregistres.cpp)
- combinaison : combinaison de mouvement à faire (controle_moteur.ino)

## Temps réel
Le temps utilise le serialCommunication.h pour lire le port série et extraire l'information de message. Le message reçu par l'Arduino est une chaine de caractère de la forme suivante : "<cible1,cible2,cible3,cible4>. Les cibles sont extraite et enregistrer dans le tableau _mgs_donnees_.

## Choix mouvement
Pour le choix de mouvement, l'Arduino lit le port série pour 1 chiffre. Ensuite, fait correspondre le chiffre lu à un mouvement préenregistré.

### Pour ajouter des mouvements
La structure _structStance_ contient les informations de chaque mouvement. Il contient : 
- Un identifiant
- Un nombre de sous-mouvement
- Un tableau de cibles

Pour ajouter un mouvement indiquer le nombre de sous-mouvement on souhaite diviser le mouvement complet. Ensuite, le tableau de cibles fonction de la façon suivante : pour n moteur, les n premières valeurs correspondent aux cibles du 1er sous-mouvement. Les n prochaines correspondent aux prochaines cibles du 2e sous-mouvement. Le tableau de cible a une taille de k*n, k étant le nombre de sous-mouvement. Les cibles sont ordonnés selon l'ordre dans lequel les identifiants des moteurs a été défini.


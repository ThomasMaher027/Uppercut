# Uppercut
Voici le code pour une carte [OpenRB-150 de Robotis](https://emanual.robotis.com/docs/en/parts/controller/openrb-150/) pour contrôler les moteurs Dynamixels. L'ensemble est configuré pour être utilisé avec Platform.IO

Le code principale est situé dans [srs/main.cpp](https://github.com/ThomasMaher027/Uppercut/tree/main/Arduino/src).

## Personnalisation
Certaines variables du main.cpp peuvent/doivent être modifiées selon le contexte d'utilisation. En voici la liste :
- nb_motor
- nb_movement : indique le nombre de mouvements préenregistrés totaux
- DXL_ID : identidiant des moteurs Dynamixel. Faire attention à l'ordre dans lequel les moteurs sont initiés.
- upper_speed_limit
- lower_speed_limit

### Pour ajouter des mouvements
La structure _structStance_ contient les informations de chaque mouvement. Il contient : 
- Un identifiant
- Un nombre de sous-mouvement
- Un tableau de cibles

Pour ajouter un mouvement indiquer le nombre de sous-mouvement on souhaite diviser le mouvement complet. Ensuite, le tableau de cibles fonction de la façon suivante : pour n moteur, les n premières valeurs correspondent aux cibles du 1er sous-mouvement. Les n prochaines correspondent aux prochaines cibles du 2e sous-mouvement. Le tableau de cible a une taille de k*n, k étant le nombre de sous-mouvement. Les cibles sont ordonnés selon l'ordre dans lequel les identifiants des moteurs a été défini.


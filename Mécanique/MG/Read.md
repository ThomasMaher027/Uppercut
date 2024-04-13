# Dossier
Dans ce dossier les fichiers servent à vérifier les comportements de notre système avec motionGenesis.

# Fichier
le DCL suivant représente notre système de manière simple.

![image du DCL](/assets/images/DCL.PNG)

L'épaule à été défini en un rigidFrame (E) pour la rotation nx et un rigidBody (B) pour la rotation ey. 
le rigidbody Av sert à repérésenter la rotation du coude et le rigidbody M représente la main.
Les particules MotE, MotC et MotM servent à représenter le moteur pour la 2e rotation de l'épaule, le moteur pour le coude 
et le moteur pour ouvrir et fermer les doigts de la main.

Les constantes mMotp et mMotg servent à définir la masse des particules moteurs.
mMotp est pour les XL430 
mMotg est pour les XM430 

Les constantes Lb, LAv et LM représentent la longueur du bicep, la longueur de l'avant bras et la longueur de la main.
Les constantes avec cmx, cmy et cmz sevent à positionner le centre de masse des différents corps.
Les constantes dMotM et dMotE sevent à positionner le moteur pour la 2e rotation de l'épaule et le moteur pour les doigts de la main.

## StatiqueBras.txt


## DynamiqueBras.txt

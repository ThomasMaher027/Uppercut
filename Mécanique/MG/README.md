# Dossier
Dans ce dossier les fichiers servent à vérifier les comportements de notre système avec motionGenesis.

# Fichier
le DCL suivant représente notre système de manière simple.

![image du DCL](https://github.com/ThomasMaher027/Uppercut/blob/main/M%C3%A9canique/MG/DCL.png)

L'épaule à été défini en un rigidFrame (E) pour la rotation nx et un rigidBody (B) pour la rotation ey. 
le rigidbody Av sert à repérésenter la rotation du coude et le rigidbody M représente la main.
Les particules MotE, MotC et MotM servent à représenter le moteur pour la 2e rotation de l'épaule, le moteur pour le coude et le moteur pour ouvrir et fermer les doigts de la main.

Les constantes mMotp et mMotg servent à définir la masse des particules moteurs.
mMotp est pour les XL430 
mMotg est pour les XM430 

Les constantes Lb, LAv et LM représentent la longueur du bicep, la longueur de l'avant bras et la longueur de la main.
Les constantes avec cmx, cmy et cmz sevent à positionner le centre de masse des différents corps.
Les constantes dMotM et dMotE servent à positionner le moteur pour la 2e rotation de l'épaule et le moteur pour les doigts de la main.

## StatiqueBras.txt
Les variables Te, Tb et Tc sont inconnues et elles servent à savoir quels couples les moteurs pour l'épaule et le coude doivent développer pour garder une position statique déterminer avec les angles qe, qb et qc.

notre analyse se fait dans 2 positions pour l'instant :
```
%Constant qe = 0 deg, qb = 0 deg, qc = 0 deg %bras droit a coter (demi Tpose)
Constant qe = -90 deg, qb = 90 deg, qc = 0 deg %lever devant (pointer)
```
Dans les deux position, le bras est à l'horizontale et compètement droit. La 1ere, le bras est parallèle à la poitrine. Dans la 2e, le bras est perpendiculaire à la poitrine.


## DynamiqueBras.txt
Pour faire une bonne analyse dynamique, il faut ajouter les inerties des membres. Elles ont été calculés avec solidwork et les assemblages dans la section 3D CAD.

Dans cette analyse, le coude est toujours déplier (qc = 0 deg). L'anaylse est faite sur un tour, 360 deg. 
Si c'est le 1er moteur qui travail à 2.95 Nm on regarde son acceleration (qe'') et les couples au coude et à l'autre moteur de l'épaule. Le bras est complètement droit, qb est à 0 deg. 
Si c'est le 2e moteur qui travail à 2.95 Nm on regarde son acceleration (qb'') et les couples au coude et au 1er moteur de l'épaule. Le bras est complètement droit, qe est à -90 deg

Selon le moteur regarder, on commence dans la position statique y faisant référence. 
moteur 1 = 1ere position statique au départ. le tour se fait autour de nx.
moteur 2 = 2e position statique au départ. le tour se fait autour de ey et ressemble à un lancer de balle donnée.

```
B.SetInertia(Bcm, IBxx = 0.00136096 kg*m^2, IByy = 0.00105899 kg*m^2, IBzz = 0.00064248 kg*m^2,  IBxy = -0.00000034 kg*m^2, IByz =  -0.00001995 kg*m^2, IBzx = -0.00012044 kg*m^2)
Av.SetInertia(Avcm, IAvxx = 0.00018089 kg*m^2, IAvyy = 0.00011084 kg*m^2, IAvzz = 0.00007775 kg*m^2,  IAvxy = 0 kg*m^2, IAvyz =  0 kg*m^2, IAvzx = 0.00000058 kg*m^2)
M.SetInertia(Mcm, IMxx = 0.00016081 kg*m^2, IMyy = 0.00012716 kg*m^2, IMzz = 0.00003633 kg*m^2,  IMxy = 0.00000009 kg*m^2, IMyz =  -0.00000214 kg*m^2, IMzx = -0.00000098 kg*m^2)
```

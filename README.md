# Projet S4GRO 2024 - Uppercut
Le Uppercut est un projet ayant pour but de concevoir un bras robotique de type *Shadow boxer*, inspiré du film [*Real Steel*](https://youtu.be/wD0goULgueE?si=wrtzg6bM562aRAEw).
Le projet s'est divisé en 3 objectifs : concevoir un bras robotique à 4 axes, atteindre des positions cibles avec précision et utiliser une caméra pour contrôler le robot. 

![image du bras robotique](https://github.com/ThomasMaher027/Uppercut/blob/main/Images/bras1.PNG)


##  Guide du répertoire
Voici un guide de l'organisation des dossiers :
- Le dossier [*Arduino*](https://github.com/ThomasMaher027/Uppercut/tree/main/Arduino) contient le code utilisé sur la carte Arduino pour contrôler les moteurs.
- Le dossier [*Informatique*](https://github.com/ThomasMaher027/Uppercut/tree/main/Informatique) contient le code Python pour analyser l'information de la caméra.
- Le dossier [*Mécanique*](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique) contient l'analyse dynamique du système et des CAD du robot.


## Procédure de lancement
Voici la procédure de fonctionnement du robot une fois qu'il est assemblé.
1. Télécharger les fichiers de code pour la caméra se trouvant dans le dossier [main/Informatique/Projet_s4/code_final_utiliser](https://github.com/ThomasMaher027/Uppercut/tree/main/Informatique/Projet_s4/code_final_utiliser)
2. Télécharger les fichiers de code pour contrôler les moteurs dans le dossier [main/Arduino/ArduinoIDE/controle_moteur](https://github.com/ThomasMaher027/Uppercut/tree/main/Arduino/ArduinoIDE/controle_moteur)
3. Suivre le [guide pour l'utilisation de la carte OpenRB-150](https://youtu.be/RaNzGhQzlu4?si=twsLND9ONWHosGwk)
4. Téléverser le code [controle_moteur.ino](https://github.com/ThomasMaher027/Uppercut/blob/main/Arduino/ArduinoIDE/controle_moteur/controle_moteur.ino) dans la carte OpenRB-150 à partir de [l'IDE d'Arduino](https://www.arduino.cc/en/software)
5. Laisser l'ordinateur connecté à la carte OpenRB avec le câble USB et ne pas ouvrir le moniteur série dans l'IDE d'Arduino.
6. Lancer le code [Holistic_TempsReel_Angle_communicationSerie.py](https://github.com/ThomasMaher027/Uppercut/blob/main/Informatique/Projet_s4/code_final_utiliser/Holistic_TempsReel_Angle_communicationSerie.py) dans un IDE Python (par exemple : [PyCharm](https://www.jetbrains.com/pycharm/)) sur un ordinateur branché à une caméra
7. Pour arrêter le programme, appuyer sur la touche "q" quand la fenêtre de la caméra est ouverte
8. S'assurer d'appuyer sur le bouton "*reset*" de la carte OpenRB avant de relancer le code Python


## Vidéos de présentation
Voici une vidéo du robot contrôlé en temps réel

https://github.com/ThomasMaher027/Uppercut/assets/114596560/83d8d1ad-10cf-46f6-bbb1-c36d5d543623


Et ici, une vidéo du robot qui éxécute une séquence prédéfini de mouvement

https://github.com/ThomasMaher027/Uppercut/assets/114596560/a4b2e40c-ad0c-4941-9826-d2be24af7056


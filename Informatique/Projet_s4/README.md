# Système de Vision 

## Début et recherche utile pour comprendre le cheminement du côté de la vision
le fichier suivant présente comment mettre en place l'environnement de programmation et qu'elle vidéo ont aider le choix des librairies et le choix du language de programmation utilisé :
[doc_vision](https://github.com/ThomasMaher027/Uppercut/blob/main/Informatique/Projet_s4/Projet_S4_recherche_Camera_Python.docx)

## Temps Réel
À l'aide des librairies OpenCv et MediaPipe, le flux vidéo de la caméra est analyser et des angles sont calculés. 
Les angles calculés sont ceux du coude et de l'épaule. Pour l'épaule, il y a deux angles de calculés. Le 1er est celui fait entre le vecteur (épaule droite à épaule gauche) et le vecteur (épaule droite à coude droit). Le 2e est celui fait entre le vecteur (épaule droite à hanche droite) et le vecteur (épaule droite à coude droit). Il y a aussi une analyse de fait pour savoir si la main est fermé ou non. Les 3 angles filtrés et le resultats de l'analyse de la main sont envoyés à l'openRB par port série.

## Mouvements prédéfinis
Le programme de vision permet d'analyser des mouvements prédéfinie du bras gauche d'un utilisateur pour envoyer les mouvements lus à un arduino par communication port série. 
Les 4 mouvements sont:
- flexion du coude
- abduction/adduction du bras parallèle au corps
- abduction/adduction du bras perpendiculaire au corps
- Léver du bras au-dessus de l'épaule ( position pour poser une question)
Pour des informations supplémentaires sur les fonctions du programme, consulter [MediaPipe](https://developers.google.com/mediapipe) sur internet.

## Classe de filtrage
La classe dataAngle située dans [filteDonnees](https://github.com/ThomasMaher027/Uppercut/blob/main/Informatique/Projet_s4/filtreDonnees.py).
Cette classe enregistre les valeurs d'angle calculées par la caméra et applique un filtre sur les données.

3 filtres sont disponibles : 
- Une moyenne mobile
- Un filtre RIf passe-bas
- Un filtre RII passe-bas (Butterworth)

# Sytème de Vision
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

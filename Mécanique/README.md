# ASSEMBLAGE MÉCANIQUE
Toutes les impressions sont faites à 15% infill en fast (0,2mm).
## ASSEMBLAGE DE L'ÉPAULE

Il faut d'abord imprimer les éléments suivant :
- **1x** [0001.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Épaule_test_2)
- **1x** [0002.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Épaule_test_2)
- **1x** [0003.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Épaule_test_2)
- **2x** [Adapteur_boost.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Coude_VF)
    - À imprimer avec plus de précison, par exemple : Fine, 80% infill.

Il faut ensuite se procurer les éléments suivant :
- **2x** [Shaft 1/4 de longueur 180mm et 135mm](https://ca.robotshop.com/products/actobotics-12x1-4-precision-d-shaft)
- **2x** [moteur dynamixel XM430-W350-T](https://emanual.robotis.com/docs/en/dxl/x/xm430-w350/)
	
Pour faire l'assemblage, il faut fixer 0001 et 0002 à l'aide d'un shaft. Ensuite, il faut visser l'adaptateur à un moteur et positionner celui-ci du côté de son support. Il ne reste qu'à visser le moteur à sa base. 

Il faut ensuite refaire ces manipulations pour la pièce 0002 et 0003. Vous pouvez vous fier à la figure 1 pour visualiser le modèle correctement.Il faut faire attention à la bonne orientation des pièces. 

![Figure 1 : Modèle 3D de l'épaule](https://github.com/ThomasMaher027/Uppercut/blob/main/Mécanique/figure_1.png)

*Figure 1 : Modèle 3D de l'épaule*

## ASSEMBLAGE DU BICEP
Il faut d'abord imprimer les éléments suivant :
- **2x** [Bicep_base.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Coude_VF)
- **1x** [Fixation_moteur.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Coude_VF)
- **1x** [Fixation_bicep.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Coude_VF)
- **1x** [Adaptateur.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Coude_VF)
    - À imprimer avec plus de précison, par exemple : Fine, 80% infill.

Il faut ensuite se procurer les éléments suivant :
- **2x** [Shaft 1/4 de longueur 100mm et 50mm](https://ca.robotshop.com/products/actobotics-12x1-4-precision-d-shaft)
- **1x** [moteur dynamixel XL430-W250-T](https://emanual.robotis.com/docs/en/dxl/x/xl430-w250/)
- **2x** [32T, 0.250" (1/4) Bore 32P Shaft Mount Pinion Gear](https://www.servocity.com/32t-0-250-1-4-bore-32p-shaft-mount-pinion-gear/)

Pour faire l'assemblage, il faut d'abord fixer les deux Bicep_base et Fixation_moteur à l'aide de la quincaillerie. Ensuite, il faut insérer la Fixation du moteur de la même orientation que la figure 2.
	
Il faut visser l'adaptateur sur le moteur et insérer un premier shaft au bout de Bicep_base. Avant de l'insérer au complet, il faut ajouter dans l'ordre une partie de l'avant-bras, un engrenage 16T et l'autre partie de l'avant-bras.
	
Enfin, il faut insérer le deuxième shaft comme sur la figure 2, y ajouter l'engrenage 32T et positionner le moteur vis-à-vis les 4 trous de sa fixation. Il ne reste qu'à bien fixer le moteur et aligner les deux engrenages, en s'étant assuré de ne pas oublier de brancher les fils au moteur avant de le fixer.  

![Figure 2 : Modèle 3D du biceps](https://github.com/ThomasMaher027/Uppercut/blob/main/Mécanique/figure_2.png)

*Figure 2 : Modèle 3D du biceps*

## ASSEMBLAGE DE L'AVANT-BRAS
Pour faire l'avant-bras, il faut simplement fixer le rouleau_doigts au moteur et fixer le moteur à son support. Ensuite il faut assembler les 2 pièces d'Avant_Bras sur les côtés et les fixer au biceps avec le shaft ainsi qu'à la main à l'autre extrémité.
	
Il faut d'abord imprimer les éléments suivant :
- **2x** [Avant_Bras.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Avant_Bras)
- **1x** [Support_Avant_Bras_moteur.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Support)
- **1x** [Support_files_main.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Avant_Bras)
- **1x** [Poulie_1.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Avant_Bras)
- **1x** [Poulie_2.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Avant_Bras)

Il faut ensuite se procurer l'élément suivant :
- **1x** [moteur dynamixel XL430-W250-T](https://emanual.robotis.com/docs/en/dxl/x/xl430-w250/)

![Figure 3 : Modèle 3D de l'avant-bras](https://github.com/ThomasMaher027/Uppercut/blob/main/Mécanique/figure_3.png)

*Figure 3 : Modèle 3D de l'avant-bras*

## ASSEMBLAGE DE LA MAINMAIN
Il faut d'abord imprimer les éléments suivant :
- **9x** [Doigt1.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/doigt)
    - À imprimer avec plus de précison, par exemple : Fine (0.1mm), 20% infill.
- **5x** [Doigt2.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/doigt)
    - À imprimer avec plus de précison, par exemple : Fine (0.1mm), 20% infill.
- **1x** [paume_V3.SLDPRT](https://github.com/ThomasMaher027/Uppercut/tree/main/Mécanique/3D%20CAD/En%20cours/Paume)

Il faut ensuite se procurer l'élément suivant :
- **1x** Environ 30 cm de PLA 2,85mm de diamètre pour assembler les doigts
	
Pour assembler la main il faut faire 4 doigts et 1 pouce. Les doigt sont fait en connectant 2 Doigt1 et 1 Doigt2. Le pouce est fait en connectant 1 Doigt1 et 1 Doigt2. Pour faire la connexion il faut passer un bout de filament PLA dans les connecteurs rond puis faire fondre les bouts pour le garder en place.
	
Après, il faut passer le fil de pêche dans les doigts et l'attacher au bout du doigt. Il peut aussi être intéressant d'utiliser de la colle.
	
Ensuite il faut connecter les doigts à la paume de la même façon que les segments de doigt se connectent ensemble.

![Figure 4 : Modèle 3D de la main](https://github.com/ThomasMaher027/Uppercut/blob/main/Mécanique/figure_4.png)

*Figure 4 : Modèle 3D de la main*

## ASSEMBLAGE COMPLET
	
Il faut fixer le biceps à l'épaule à l'aide de 4 vis M4 25LG, 4 écrous et 4 rondelles. Il faut ensuite fixer le bras à sa base à l'aide de 4 vis M6 50LG, 4 écrous et 4 rondelles et de même pour la main, en s'assurant que les fils de pêche puisse bien s'enrouler autour de la poulie de l'avant-bras et passer dans les trous correspondant. Il faut les enrouler de manière opposé afin qu'il ouvre et/ou ferme la main.

![Figure 5 : Modèle 3D du bras entier](https://github.com/ThomasMaher027/Uppercut/blob/main/Mécanique/figure_5.png)

*Figure 5 : Modèle 3D du bras entier*
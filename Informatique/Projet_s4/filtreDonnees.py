# source : https://stackoverflow.com/questions/22583391/peak-signal-detection-in-realtime-timeseries-data
import numpy as np
import matplotlib.pyplot as plt
import csv
#from functions import clamp, arduinoMap
from scipy import signal 
#import detectionHauteFreq

class dataAngle():
    def __init__(self, Fe, Fc, delai, NB_moteur):
        """
        Description
        -----------
        Cette classe permet d'enregistrer les valeurs d'angles calculées par la caméra et de les filtrer avant de les envoyer à l'Arduino.
        3 choix de filtre :
            - Une moyenne mobile
            - Un FIR passe-bas
            - Un IIR passe-bas de type Butterworth (le meilleur selon nos test)
        Pour choisir le filtre à appliquer, appeler la méthode appropriée (dans le main/autre bout de code) avant d'envoyer les données
        
        Parameters
        ----------
        Fe : int
            Fréquence d'échantillonnage.
        Fc : int
            Fréquence de coupure des filtres.
        delai : int
            Ordre de la moyenne mobile.
        NB_moteur : itn
            Nombre de moteur sur le robot (comprend le moteur de la main. Cependant, les valeurs d'angle de la main ne sont pas filtrées).

        Returns
        -------
        None.

        """
        
        self.lag = delai # Nombre de données utilisées dans la moyenne mobile
        self.nb_moteur = NB_moteur
        
        self.val_angles = {} # Contient les valeurs d'angle bruttes pour les moteurs
        self.val_moy = {} # Contient les résultats des moyennes mobiles
        self.filt_RIF = {} # Contient les angles filtrés par le FIR
        self.filt_RII = {} # Contient les angles filtrés par IIR
        for ii in range(NB_moteur):
            # Les moteurs sont numérotés de 0 à nb_moteur-1
            self.val_angles[f"moteur_{ii}"] = []
            self.val_moy[f"moteur_{ii}"] = []
            self.filt_RIF[f"moteur_{ii}"] = []
            self.filt_RII[f"moteur_{ii}"] = []

        
        self.ordre = 2 # filtre RIF/RII ordre 4
        self.fc = 2 # fréquence de coupure = 2 Hz
        self.fe = Fe # fréquence d'échantillonnage
        self.te = 1/self.fe # période d'échantillonnage
        self.b_RIF = signal.firwin(self.ordre, self.fc, fs=self.fe) # Coefficients du filtre RIF
        self.b_RII, self.a_RII = signal.butter(self.ordre, self.fc, 'low', False, 'ba', fs=self.fe) # Coefficient du filtre RII
        
        
    """============================================================================"""   

     
    def setData(self, IN1, IN2, IN3, IN4):
        """
        Description
        ----------
        Enregistre toutes les valeurs d'angle calculées par la caméra
        
        Parameters
        ----------
        IN1 : int
            Valeur de l'angle calculé par la caméra du moteur 1.
        IN2 : int
            Valeur de l'angle calculé par la caméra du moteur 2.
        IN3 : int
            Valeur de l'angle calculé par la caméra du moteur 3.
        IN4 : int
            Valeur de l'angle calculé par la caméra du moteur 4.

        Returns
        -------
        None.

        """
        IN = [IN1, IN2, IN3, IN4] 
        for ii in range(self.nb_moteur):
            self.val_angles[f"moteur_{ii}"].append(IN[ii])
 
    
    """============================================================================"""   
    
    
    def moy(self):
        """
        Desciption
        ----------
        Calcul la moyenne mobile sur les self.lag données

        Returns
        -------
        None.

        """
        for ii in range(self.nb_moteur-1):
            if len(self.val_angles[f"moteur_{ii}"]) >= self.lag: # S'il y a assez de données enregistrées, on peut calculer la moyenne. Sinon, on passe la dernière mesure d'angle
                moy = int(np.mean(self.val_angles[f"moteur_{ii}"][-self.lag:])) 
            else:
                moy = int(self.val_angles[f"moteur_{ii}"][-1])
                
            self.val_moy[f"moteur_{ii}"].append(moy)


    """============================================================================"""   
    
    
    def filtreRIF(self):
        """
        Description
        ----------
        Applique un filtre RIF aux valeurs d'angle lus

        Returns
        -------
        None.

        """

        for ii in range(self.nb_moteur-1):
            out = 0
            if len(self.val_angles[f"moteur_{ii}"]) >= len(self.b_RIF): # S'il y a assez de valeurs d'angle qui ont été lues applique le filtre. Sinon, on passe la dernière mesure d'angle
                for jj in range(len(self.b_RIF)):
                    out += self.b_RIF[jj]*self.val_angles[f"moteur_{ii}"][-1-jj]
            else:
                out = self.val_angles[f"moteur_{ii}"][-1]
            self.filt_RIF[f"moteur_{ii}"].append(int(out))
    
    
    """============================================================================"""   
    
    
    def calculRII(self, x, y):
        """
        Description
        -----------
        Calcul pour le filtre RII. Basé sur une équation de différente

        Parameters
        ----------
        x : liste de int/float
            Valeurs d'angle non filtrés.
        y : liste de int
            Valeurs d'angle déjà filtrés par le RII (rétroaction).

        Returns
        -------
        int
            Angle filtré (retourne une seule valeur).

        """
        out = 0
        
        # Application du filtre (cas général)
        if (len(y) >= len(self.b_RII)-1):
            for ii in range(len(self.b_RII)):
                out += self.b_RII[ii]*x[-1-ii]
            for ii in range(1,len(self.a_RII)):
                out -= self.a_RII[ii]*y[-ii]  
        
        # Cas d'application dans les cas où la rétroaction n'est pas encore assez grande
        elif (len(y) == 0):
            out = self.b_RII[0]*x[-1]
            
        elif (len(y) == 1):
            out = self.b_RII[0]*x[-1] + self.b_RII[1]*x[-2] - self.a_RII[1]*y[-1]
            
        return int(out)
    

    """============================================================================"""  
    
                
    def filtreRII(self):
        """
        Description
        -----------
        Applique un filtre RII aux valeurs d'angle lus

        Returns
        -------
        None.

        """
        for ii in range(self.nb_moteur-1):
            out_RII = self.calculRII(self.val_angles[f"moteur_{ii}"], self.filt_RII[f"moteur_{ii}"])
            self.filt_RII[f"moteur_{ii}"].append(out_RII)
        
    
    """============================================================================"""  
    
    
    def graph(self, data1 = [], data2 = [], leg1 = ' ', leg2 = ' ', xlabel = ' ', ylabel = ' ', title = ' '):
        """
        Description
        ----------
        Affiche un grapique pour comparer les différentes méthodes de filtrage des données

        Parameters
        ----------
        data1 : liste
            1er ensemble de données.
        data2 : liste
            2e ensemble de données.
        leg1 : str
            légende pour data1.
        leg2 : str
            légende pour data2.
        xlabel : str
            Titre de l'axe x.
        ylabel : str
            Titre de l'axe y.
        title : str
            Titre du graphique.

        Returns
        -------
        None.

        """
        fig, ax = plt.subplots()
        ax.plot(np.linspace(0,len(data1),len(data1))*self.te, data1)
        ax.plot(np.linspace(0,len(data2),len(data2))*self.te, data2, '--')
        ax.legend([leg1,leg2])
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        plt.title(title)
        plt.show()
        
        
    def outputData(self):
        """
        Description
        ----------
        Sort les valeurs en CSV

        Returns
        -------
        None.

        """
        data =  [self.data1, self.data2, self.data3, self.moy1, self.moy2, self.moy3]
        filename = 'outputData.csv'
        with open(filename, 'w', newline='') as csvfile:
            csvwriter = csv.writer(csvfile, delimiter='|')
            csvwriter.writerows(data)
    

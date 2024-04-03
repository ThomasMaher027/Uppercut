# source : https://stackoverflow.com/questions/22583391/peak-signal-detection-in-realtime-timeseries-data
import numpy as np
import matplotlib.pyplot as plt
import csv
from functions import clamp, arduinoMap
from scipy import signal 

class data():
    def __init__(self, sr):
        """

        Parameters
        ----------
        sr : int
            Fréquence d'échantillonnage des angles.

        Returns
        -------
        None.

        """
        
        self.lag = 5 # Nombre de données utilisées dans la moyenne mobile
        
        # Contient toutes les valeurs d'angle bruttes pour les moteurs - Utilisé pour le filtre RIF
        self.data1 = []
        self.data2 = []
        self.data3 = []
        
        # Contient les {lag} dernières valeurs d'angle
        self.temp_data1 = []
        self.temp_data2 = []
        self.temp_data3 = []
        
        # Réusltats des moyennes mobiles
        self.moy1 = []
        self.moy2 = []
        self.moy3 = []
        
        # Réusltats des filtres RIF
        self.filt1 = []
        self.filt2 = []
        self.filt3 = []
        
        # Réusltats des filtres IIR
        self.filt_IIR1 = []
        self.filt_IIR2 = []
        self.filt_IIR3 = []
        
        self.ordre = 2 # filtre FIR/IIR ordre 4
        self.fc = 2 # fréquence de coupure = 2 Hz
        self.fe = sr # fréquence d'échantillonnage
        self.te = 1/self.fe # période d'échantillonnage
        self.b_FIR = signal.firwin(self.ordre, self.fc, fs=self.fe) # Coefficients du filtre RIF
        self.b_IIR, self.a_IIR = signal.butter(self.ordre, self.fc, 'low', False, 'ba', fs=self.fe) # Coefficient du filtre IIR
        
        
    def setData(self, IN1, IN2, IN3):
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

        Returns
        -------
        None.

        """
        self.data1.append(IN1)
        self.data2.append(IN2)
        self.data3.append(IN3)
        
        
    def setTempData(self, IN1, IN2, IN3):
        """
        Description
        ----------
        Enregistre les {lag} valeurs d'angle calculées par la caméra

        Parameters
        ----------
        IN1 : int
            Valeur de l'angle calculé par la caméra du moteur 1.
        IN2 : int
            Valeur de l'angle calculé par la caméra du moteur 2.
        IN3 : int
            Valeur de l'angle calculé par la caméra du moteur 3.

        Returns
        -------
        None.

        """
        if IN1 > 90:
            IN1 = 90
        if ((len(self.temp_data1) >= self.lag) and (len(self.temp_data2) >= self.lag) and (len(self.temp_data3) >= self.lag)):
            self.temp_data1.pop(0)
            self.temp_data2.pop(0)
            self.temp_data3.pop(0)  
        self.temp_data1.append(IN1)
        self.temp_data2.append(IN2)
        self.temp_data3.append(IN3)

        
    def moy(self):
        self.moy1.append(int(np.mean(self.temp_data1)))
        self.moy2.append(int(np.mean(self.temp_data2)))
        self.moy3.append(int(np.mean(self.temp_data3)))
        
    def filterSignal(self):
        """
        Description
        ----------
        Applique un filtre RIF (ou IIR, mais n'a pas l'air de fonctionner) aux valeurs d'angle lus

        Returns
        -------
        None.

        """
        out1 = 0
        out2 = 0 
        out3 = 0
        
        if (len(self.data1)>=len(self.b_FIR) and len(self.data2)>=len(self.b_FIR) and len(self.data3)>=len(self.b_FIR)):
            for ii in range(len(self.b_FIR)):
                out1 += self.b_FIR[ii]*self.data1[-1-ii]
                out2 += self.b_FIR[ii]*self.data2[-1-ii]
                out3 += self.b_FIR[ii]*self.data3[-1-ii]
                """
                if (len(self.filt_IIR1)>=len(self.b_IIR)+1):
                    out4 += self.b_IIR[ii]*self.data1[-1-ii] - self.a_IIR[ii+1]*self.filt_IIR1[-1-ii]
                    out5 += self.b_IIR[ii]*self.data2[-1-ii] - self.a_IIR[ii]*self.filt_IIR2[-1-ii]
                    out6 += self.b_IIR[ii]*self.data3[-1-ii] - self.a_IIR[ii]*self.filt_IIR3[-1-ii]
                else:
                    out4 = self.data1[-1]
                    out5 = self.data2[-1]
                    out6 = self.data3[-1]"""
                    
                
        else:
            out1 = self.data1[-1]
            out2 = self.data2[-1]
            out3 = self.data3[-1]
            
        self.filt1.append(out1)
        self.filt2.append(out2)
        self.filt3.append(out3)
        
        
        
    
    def iirFilter(self, x, y):
        out = 0
        if (len(y) >= len(self.b_IIR)-1):
            for ii in range(len(self.b_IIR)):
                out += self.b_IIR[ii]*x[-1-ii]
            for ii in range(1,len(self.a_IIR)):
                out -= self.a_IIR[ii]*y[-1-ii+1]  
        elif (len(y) == 0):
            out = self.b_IIR[0]*x[-1]
        elif (len(y) == 1):
            out = self.b_IIR[0]*x[-1] + self.b_IIR[1]*x[-2] - self.a_IIR[1]*y[-1]
        return int(out)
    
                
    def callIIRFilter(self):
        """
        if (len(self.filt_IIR1) >= len(self.b_IIR)):
            self.filt_IIR1.append(self.iirFilter(self.data1, self.filt_IIR1))
        else:
            self.filt_IIR1.append(int(self.data1[-1]))
            
        if (len(self.filt_IIR2) >= len(self.b_IIR)):
            self.filt_IIR2.append(self.iirFilter(self.data2, self.filt_IIR2))
        else:
            self.filt_IIR2.append(int(self.data2[-1]))
        """
            
        """
        if (len(self.filt_IIR3) >= len(self.b_IIR)-1):
            self.filt_IIR3.append(self.iirFilter(self.data3, self.filt_IIR3))
        elif (len(self.filt_IIR3) == 0):
            self.filt_IIR3.append(int(self.b_IIR[0]*self.data3[-1]))
        elif (len(self.filt_IIR3) == 1):
            self.filt_IIR3.append(int(self.b_IIR[0]*self.data3[-1] + self.b_IIR[1]*self.data3[-2] - self.a_IIR[1]*self.filt_IIR3[-1]))
            """
        self.filt_IIR1.append(self.iirFilter(self.data1, self.filt_IIR1))
        self.filt_IIR2.append(self.iirFilter(self.data2, self.filt_IIR2))    
        self.filt_IIR3.append(self.iirFilter(self.data3, self.filt_IIR3))
        
        
    def getData(self):
        return self.data1, self.data2, self.data3
    
    def getMoy(self):
        return self.moy1, self.moy2, self.moy3
    
    def graph(self, data1, data2, data3, leg1, leg2, leg3, xlabel, ylabel, title):
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
        data3 : liste
            3e ensemble de données.
        leg1 : str
            légende pour data1.
        leg2 : str
            légende pour data2.
        leg3 : str
            légende pour data3.
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
        ax.plot(np.linspace(0,len(data3),len(data3))*self.te, data3, '-.')
        ax.legend([leg1,leg2,leg3])
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
    

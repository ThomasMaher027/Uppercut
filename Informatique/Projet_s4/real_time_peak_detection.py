# source : https://stackoverflow.com/questions/22583391/peak-signal-detection-in-realtime-timeseries-data
import numpy as np
import matplotlib.pyplot as plt
import csv
from functions import clamp, arduinoMap
from scipy import signal 

class data():
    def __init__(self, sr):
        self.lag = 5
        
        self.data1 = []
        self.data2 = []
        self.data3 = []
        
        self.temp_data1 = []
        self.temp_data2 = []
        self.temp_data3 = []
        
        self.moy1 = []
        self.moy2 = []
        self.moy3 = []
        
        self.filt1 = []
        self.filt2 = []
        self.filt3 = []
        
        self.filt_IIR1 = []
        self.filt_IIR2 = []
        self.filt_IIR3 = []
        
        self.trad_moy1 = []
        self.trad_moy2 = []
        self.trad_moy3 = []
        
        self.ordre = 4 #filtre FIR/IIR ordre 4
        self.fc = 2 #fréquence de coupure = 2 Hz
        self.fe = sr #fréquence d'échantillonnage
        self.te = 1/self.fe
        self.b_FIR = signal.firwin(self.ordre, self.fc, fs=self.fe)
        self.b_IIR, self.a_IIR = signal.butter(self.ordre, self.fc, 'low', False, 'ba', fs=self.fe)
        
    def setData(self, IN1, IN2, IN3):
        self.data1.append(IN1)
        self.data2.append(IN2)
        self.data3.append(IN3)
        
    def setTempData(self, IN1, IN2, IN3):
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
        out1 = 0
        out2 = 0 
        out3 = 0
        out4 = 0
        out5 = 0
        out6 = 0
        
        if (len(self.data1)>=len(self.b_FIR) and len(self.data2)>=len(self.b_FIR) and len(self.data3)>=len(self.b_FIR)):
            for ii in range(len(self.b_FIR)):
                out1 += self.b_FIR[ii]*self.data1[-1-ii]
                out2 += self.b_FIR[ii]*self.data2[-1-ii]
                out3 += self.b_FIR[ii]*self.data3[-1-ii]
                
                if (len(self.filt_IIR1)>=len(self.b_IIR)+1):
                    out4 += self.b_IIR[ii]*self.data1[-1-ii] + self.a_IIR[ii]*self.filt_IIR1[-1-ii]
                    out5 += self.b_IIR[ii]*self.data2[-1-ii] + self.a_IIR[ii]*self.filt_IIR2[-1-ii]
                    out6 += self.b_IIR[ii]*self.data3[-1-ii] + self.a_IIR[ii]*self.filt_IIR3[-1-ii]
                else:
                    out4 = self.data1[-1]
                    out5 = self.data2[-1]
                    out6 = self.data3[-1]
                
        else:
            out1 = self.data1[-1]
            out2 = self.data2[-1]
            out3 = self.data3[-1]
            
        self.filt1.append(out1)
        self.filt2.append(out2)
        self.filt3.append(out3)
        
        self.filt_IIR1.append(out4)
        self.filt_IIR2.append(out5)
        self.filt_IIR3.append(out6)
        
        
    def getData(self):
        return self.data1, self.data2, self.data3
    
    def getMoy(self):
        return self.moy1, self.moy2, self.moy3
    
    def graph(self, data1, data2, data3, leg1, leg2, leg3, xlabel, ylabel, title):
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
        data =  [self.data1, self.data2, self.data3, self.moy1, self.moy2, self.moy3]
        filename = 'outputData.csv'
        with open(filename, 'w', newline='') as csvfile:
            csvwriter = csv.writer(csvfile, delimiter='|')
            csvwriter.writerows(data)
    
    def tradData(self):
        for x in self.moy1:
            floor = 0
            ceil = 90
            x = clamp(x,floor,ceil)
            x = 90 - x
            self.trad_moy1.append(arduinoMap(x, floor, ceil, 200, 262))
            
        for x in self.moy3:
            floor = 0
            ceil = 180
            x = clamp(x,floor,ceil)
            self.trad_moy3.append(arduinoMap(x, floor, ceil, 43, 150))
# source : https://stackoverflow.com/questions/22583391/peak-signal-detection-in-realtime-timeseries-data
import numpy as np
import matplotlib.pyplot as plt
import csv
from functions import clamp, arduinoMap

class data():
    def __init__(self):
        self.lag = 10
        
        self.data1 = []
        self.data2 = []
        self.data3 = []
        
        self.temp_data1 = []
        self.temp_data2 = []
        self.temp_data3 = []
        
        self.moy1 = []
        self.moy2 = []
        self.moy3 = []
        
        self.trad_moy1 = []
        self.trad_moy2 = []
        self.trad_moy3 = []
        
    def setData(self, IN1, IN2, IN3):
        self.data1.append(IN1)
        self.data2.append(IN2)
        self.data3.append(IN3)
        
    def setTempData(self, IN1, IN2, IN3):
        if IN1 > 90:
            IN1 = 90
        self.temp_data1.append(IN1)
        self.temp_data2.append(IN2)
        self.temp_data3.append(IN3)
        if ((len(self.temp_data1) >= self.lag) and (len(self.temp_data2) >= self.lag) and (len(self.temp_data3) >= self.lag)):
            self.temp_data1.pop(0)
            self.temp_data2.pop(0)
            self.temp_data3.pop(0)
        
    def moy(self):
        self.moy1.append(np.mean(self.temp_data1))
        self.moy2.append(np.mean(self.temp_data2))
        self.moy3.append(np.mean(self.temp_data3))
        
    def getData(self):
        return self.data1, self.data2, self.data3
    
    def getMoy(self):
        return self.moy1, self.moy2, self.moy3
    
    def graph(self, data1, data2, leg1, leg2, xlabel, ylabel, title):
        fig, ax = plt.subplots()
        ax.plot(np.linspace(0,len(data1),len(data1))*0.1, data1)
        ax.plot(np.linspace(0,len(data2),len(data2))*0.1,data2)
        ax.legend([leg1,leg2])
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        plt.title(title)
        plt.show()
        """
        fig, ax = plt.subplots()
        ax.plot(self.data2)
        ax.plot(self.moy2)
        ax.legend(['Angles calculés','Moyenne mobile'])
        plt.show()
        
        fig, ax = plt.subplots()
        ax.plot(self.data3)
        ax.plot(self.moy3)
        ax.legend(['Angles calculés','Moyenne mobile'])
        plt.show()
        """
        """
        fig, ax = plt.subplots()
        ax.plot(self.trad_moy1)
        plt.show()
        
        fig, ax = plt.subplots()
        ax.plot(self.trad_moy3)
        plt.show()
        """
        
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
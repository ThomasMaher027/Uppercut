#import time

"""
Voici les imports nécessaire pour que le programme soit fonctionnel
"""
import mediapipe as mp
import numpy as np
import cv2
import serial
import time
from threading import Timer
import filtreDonnees
import matplotlib.pyplot as plt



"""
class qui permet de faire l'envoie des données pendant la lecture des angles
"""
class RepeatedTimer(object):
    def __init__(self, interval, function, *args, **kwargs):
        """

        Parameters
        ----------
        interval : période
        function : fonction exécuter
        args : argument de la fonction
        kwargs : argument de la fonction
        """
        self._timer     = None
        self.interval   = interval
        self.function   = function
        self.args       = args
        self.kwargs     = kwargs
        self.is_running = False
        self.start()


    def _run(self):
        """
        roule le programme
        Returns
        -------

        """
        self.is_running = False
        self.start()
        self.function(*self.args, **self.kwargs)

    def start(self):
        if not self.is_running:
            self._timer = Timer(self.interval, self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        self._timer.cancel()
        self.is_running = False



"""
le frame utiliser pour analyser le corps
"""

mp_drawing = mp.solutions.drawing_utils
mp_holistic = mp.solutions.holistic

"""
communication par port serie
"""
arduino = serial.Serial(port='COM5',   baudrate=115200, timeout=.1) # TODO

# VIDEO FEED
cap = cv2.VideoCapture(0)
#initiate holistic model

# curl counter
counter = 0
stage = None

# shoulder counter overhead
counterB = 0
stageB = None

#shoulder counter perpendicular
counterC = 0
stageC = None

#shoulder counter épaule droite, épaule gauche, coude parallèle
counterD = 0
stageD = None

"""
initialisation des angles et la position de la main
"""
defVal = -900
angleA = defVal
angleB = defVal 
angleC = defVal
angleD = defVal
pos_main = 0
stage_main = None

fs = 20
ts = 1/fs
pres_t = time.time()
last_t = 0

"""
fonction permettant la communication par port série
"""
def write_read(x): # TODO
    """

    Parameters
    ----------
    x : données envoyées

    Returns
    -------

    """
    arduino.write(bytes(x,   'utf-8'))
    #time.sleep(0.05)
    #data = arduino.readline()
    #print(data)

def communication():

    """
    applique les filtre sur les données et les envoient dans le port serie
    Returns
    -------

    """
    
    
    if((angleC!=defVal) and (angleD!=defVal) and (angleA!=defVal)):
        dataAngle.setData(IN1=angleC, IN2=angleD, IN3=angleA, IN4=pos_main)
        dataAngle.filtreRII()
        dataAngle.moy()
        angle_moteur_0 = dataAngle.filt_RII['moteur_0'][-1]
        angle_moteur_1 = dataAngle.filt_RII['moteur_1'][-1]
        angle_moteur_2 = dataAngle.filt_RII['moteur_2'][-1]
        angle_moteur_3 = dataAngle.val_moy['moteur_3'][-1]
        write_read(f"<{angle_moteur_0}, {angle_moteur_1}, {angle_moteur_2}, {angle_moteur_3}>")
        
        #print(f"a' IN : <{dataAngle.moy1}, {dataAngle.moy2}, {dataAngle.moy3}, {pos_main}>")


def calculate_angle(a, b, c):

    """

    Parameters
    ----------
    a : position articulation 1
    b : position articulation 2
    c : position articulation 3

    Returns
    -------

    """
    a = np.array(a)
    b = np.array(b)
    c = np.array(c)

    radians = np.arctan2(c[1]-b[1], c[0]-b[0]) - np.arctan2(a[1]-b[1], a[0]-b[0])
    angle = np.abs(radians*180.0/np.pi)

    if angle >180.0:
        angle = 360-angle

    return int(angle)

def main(a, b, c, d, e, f, g, h, i, j):
    """

    Parameters
    ----------
    a : position phalange 1 index
    b : position phalange 2 index
    c : position phalange 1 majeur
    d : position phalange 2 majeur
    e : position phalange 1 annulaire
    f : position phalange 2 annulaire
    g : position phalange 1 auriculaire
    h : position phalange 2 auriculaire
    i : position poignet
    j : position pouce

    Returns
    -------

    """
    a = np.array(a)
    b = np.array(b)
    c = np.array(c)
    d = np.array(d)
    e = np.array(e)
    f = np.array(f)
    g = np.array(g)
    h = np.array(h)
    i = np.array(i)
    j = np.array(j)

    if i[1]<a[1]<b[1] and i[1]<c[1]<d[1] and i[1]<e[1]<f[1] and i[1]<g[1]<h[1] and b[0]<j[0]<h[0]:
        stage_m = 1

    elif b[1]<a[1]<i[1] and d[1]<c[1]<i[1] and f[1]<e[1]<i[1] and h[1]<g[1]<i[1] and h[0]<j[0]<b[0]:
        stage_m = 1

    else:
        stage_m = 0
        
    return stage_m



dataAngle = filtreDonnees.dataAngle(fs, 2, 10, 4)

"""
ouverture de la caméra avec le modèle avec son modèle (holistic)
"""

message = RepeatedTimer(ts, communication)

try:
    with (mp_holistic.Holistic(min_detection_confidence=0.9, min_tracking_confidence=0.9) as holistic):
        while cap.isOpened():
            ret, frame = cap.read()
            #recolor image to rgb
            image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            image.flags.writeable = False
            #make detection
            results = holistic.process(image)
            #recolor back to RGB
            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    
            #Extract Landmarks
            try:
                # pour utiliser detection du corps
                landmarks = results.pose_landmarks.landmark
    
                #detection de la main
                landmarks2 = results.right_hand_landmarks.landmark

                """
                position des différents membres dans la liste
                """
    
                shoulder = [landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER].x,
                            landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER].y]
    
                elbow = [landmarks[mp_holistic.PoseLandmark.RIGHT_ELBOW].x,
                         landmarks[mp_holistic.PoseLandmark.RIGHT_ELBOW].y]
    
                wrist = [landmarks[mp_holistic.PoseLandmark.RIGHT_WRIST].x,
                         landmarks[mp_holistic.PoseLandmark.RIGHT_WRIST].y]
    
                hip = [landmarks[mp_holistic.PoseLandmark.RIGHT_HIP].x,
                         landmarks[mp_holistic.PoseLandmark.RIGHT_HIP].y]
    
                shoulderB = [landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER].y,
                            landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER].z]
    
                elbowB = [landmarks[mp_holistic.PoseLandmark.RIGHT_ELBOW].y,
                         landmarks[mp_holistic.PoseLandmark.RIGHT_ELBOW].z]
    
                wristB = [landmarks[mp_holistic.PoseLandmark.RIGHT_WRIST].y,
                         landmarks[mp_holistic.PoseLandmark.RIGHT_WRIST.value].z]
    
                hipB = [landmarks[mp_holistic.PoseLandmark.RIGHT_HIP.value].y,
                         landmarks[mp_holistic.PoseLandmark.RIGHT_HIP.value].z]
    
                shoulderC = [landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER.value].x,
                            landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER.value].y]
    
                index = [landmarks2[mp_holistic.HandLandmark.INDEX_FINGER_TIP.value].x,
                            landmarks2[mp_holistic.HandLandmark.INDEX_FINGER_TIP.value].y]
    
                index2 = [landmarks2[mp_holistic.HandLandmark.INDEX_FINGER_MCP.value].x,
                         landmarks2[mp_holistic.HandLandmark.INDEX_FINGER_MCP.value].y]
    
                majeur = [landmarks2[mp_holistic.HandLandmark.MIDDLE_FINGER_TIP.value].x,
                         landmarks2[mp_holistic.HandLandmark.MIDDLE_FINGER_TIP.value].y]
    
                majeur2 = [landmarks2[mp_holistic.HandLandmark.MIDDLE_FINGER_MCP.value].x,
                          landmarks2[mp_holistic.HandLandmark.MIDDLE_FINGER_MCP.value].y]
    
                annulaire = [landmarks2[mp_holistic.HandLandmark.RING_FINGER_TIP.value].x,
                         landmarks2[mp_holistic.HandLandmark.RING_FINGER_TIP.value].y]
    
                annulaire2 = [landmarks2[mp_holistic.HandLandmark.RING_FINGER_MCP.value].x,
                             landmarks2[mp_holistic.HandLandmark.RING_FINGER_MCP.value].y]
    
                auriculaire = [landmarks2[mp_holistic.HandLandmark.PINKY_TIP.value].x,
                         landmarks2[mp_holistic.HandLandmark.PINKY_TIP.value].y]
    
                auriculaire2 = [landmarks2[mp_holistic.HandLandmark.PINKY_MCP.value].x,
                               landmarks2[mp_holistic.HandLandmark.PINKY_MCP.value].y]
    
                poignet = [landmarks2[mp_holistic.HandLandmark.WRIST.value].x,
                                landmarks2[mp_holistic.HandLandmark.WRIST.value].y]
    
                pouce = [landmarks2[mp_holistic.HandLandmark.THUMB_TIP.value].x,
                                landmarks2[mp_holistic.HandLandmark.THUMB_TIP.value].y]

                """
                calcul des différents angles et la position de la main
                """
    
                angleA = calculate_angle(shoulder, elbow, wrist)
                angleB = calculate_angle(hip, shoulder, elbow)
                angleC = calculate_angle(hipB, shoulderB, elbowB)
                angleD = calculate_angle(shoulderC, shoulder, elbow)

                if (pres_t-last_t >=0):
                    pos_main = main(index, index2, majeur, majeur2, annulaire, annulaire2, auriculaire, auriculaire2, poignet, pouce)
                    last_t = pres_t
                pres_t = time.time()
                
                    
                
                """
                permet d'afficher l'angle du coude sur le coude
                """

                cv2.putText(image, str(angleA),
                            tuple(np.multiply(elbow,[640, 480]).astype(int)),
                                  cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )
    
                """cv2.putText(image, str(angleB),
                            tuple(np.multiply(shoulder, [640, 480]).astype(int)),
                                  cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )"""

                """
                permet d'afficher l'anlge de l'épaule droite sur l'épaule droite
                """
                cv2.putText(image, str(angleD),
                            tuple(np.multiply(shoulder, [640, 480]).astype(int)),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )
    
                ret = "rien"

                """
                # curl counter logic
                if angle > 160 and angleB < 10:
                    stage = "down"
                if angle < 30 and stage == "down" and angleB < 20:
                    stage = "up"
                    counter += 1
                    #ret = write_read("1")

                #Shoulder counter logic
                if angleC < 50:
                    stageC = "down"
                if 80 < angleC < 100 and stageC == "down" and angleD < 170 and stageD != "up":
                    stageC = "up"
                    counterC += 1
                    #ret = write_read("3")
    
                # shoulder counter logic
                if angleB > 150:
                    stageB = "down"
                if angleB < 30 and stageB == "down" and angleC < 150 and stageC != "up":
                    stageB = "up"
                    counterB += 1
                    #ret = write_read("2")
    
                #Shoulder counter logic
                if angleD < 110:
                    stageD = "down"
                if angleD > 170 and stageD == "down" and stageC != "up":
                    stageD = "up"
                    counterD += 1
                    #ret = write_read("4")
                if ret != "rien":
                    print(ret)
                """

                if pos_main == 1:
                    stage_main = "down"
                if pos_main == 0:
                    stage_main = "up"
    
            except:
                pass

            

            cv2.putText(image, stage_main, (450, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2, cv2.LINE_AA)
           
            """cv2.rectangle(image, (0, 0), (112, 72), (245, 117, 16), -1)
    
            cv2.putText(image,'REPS', (15, 12),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)
    
            cv2.putText(image, str(counter), (10, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)
                        
            a mettre en commentaire{
            cv2.putText(image,'STAGE', (65, 12),                                    
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)  
    
            cv2.putText(image, stage, (60, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)           
            }
    
            cv2.rectangle(image, (112, 0), (224, 72), (30, 240, 16), -1)
    
            cv2.putText(image,'REPS', (120, 12),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)
    
            cv2.putText(image, str(counterB), (120, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)
            a mettre en commentaire{
            cv2.putText(image,'STAGE', (310, 12),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)
    
            cv2.putText(image, stageB, (290, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)
            }
    
            cv2.rectangle(image, (224, 0), (336, 72), (30, 16, 240), -1)
    
            cv2.putText(image, 'REPS', (240, 12),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)
    
            cv2.putText(image, str(counterC), (240, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)
            a mettre en commentaire{
            cv2.putText(image,'STAGE', (530, 12),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)
    
            cv2.putText(image, stageC, (530, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)
            }
    
            cv2.rectangle(image, (336, 0), (448, 72), (100, 100, 100), -1)
    
            cv2.putText(image,'REPS', (350, 12),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1, cv2.LINE_AA)
    
            cv2.putText(image, str(counterD), (350, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2, cv2.LINE_AA)
    
            cv2.putText(image,'STAGE', (450, 12),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1, cv2.LINE_AA)
            """
 
                        
    
    
            mp_drawing.draw_landmarks(image, results.pose_landmarks,mp_holistic.POSE_CONNECTIONS,
                                      mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=2,circle_radius=2),
                                        mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2, circle_radius=2)
                                      )
    
            mp_drawing.draw_landmarks(image, results.right_hand_landmarks,mp_holistic.HAND_CONNECTIONS,
                                      mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=2,circle_radius=2),
                                        mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2, circle_radius=2)
                                      )


            cv2.imshow('Mediapipe Feed', image)
    
            """
            permet de quitter le programme
            """
            if cv2.waitKey(10) & 0xFF == ord('q'):
                break
    
        """
        ferme la caméra et détruite la fenêtre de la caméra
        """

        cap.release()
        cv2.destroyAllWindows()
             
        #arduino.write('home')
        
finally:
  message.stop() 
  
  """
  dataAngle.graph(data1=dataAngle.val_angles["moteur_2"], data2=dataAngle.filt_RII["moteur_2"], leg1="Angles calculés", leg2="Filtre RII", xlabel = "Temps (s)", ylabel = "Angle (deg)", title=f"Angle du coude (moteur3)\nFe={fs}Hz, Ordre=2")
  dataAngle.graph(data1=dataAngle.data2, data2=dataAngle.filt2, data3=dataAngle.filt_IIR2, leg1="Angles calculés", leg2="Moyenne mobile", leg3='Angles filtrés', xlabel = "Temps (s)", ylabel = "Angle (deg)", title=f"Angle de l'épaule (moteur2)\nFe={fs}Hz, Ordre=10")
  dataAngle.graph(data1=dataAngle.data3, data2=dataAngle.filt3, data3=dataAngle.filt_IIR3, leg1="Angles calculés", leg2="Moyenne mobile", leg3='Angles filtrés', xlabel = "Temps (s)", ylabel = "Angle (deg)", title=f"Angle du coude (moteur3)\nFe={fs}Hz, Ordre=")
  
  
  plt.psd(dataAngle.val_angles["moteur_2"],Fs=fs,NFFT=len(dataAngle.val_angles["moteur_2"]))
  plt.psd(dataAngle.filt_RII["moteur_2"],Fs=fs,NFFT=len(dataAngle.filt_RII["moteur_2"]))
  plt.legend(["Angles calculés","Filtre RII"])
  plt.title("Densité spectrale des angles du biceps")
  plt.show()
  
  plt.psd(dataAngle.data2,Fs=fs,NFFT=len(dataAngle.data2))
  plt.psd(dataAngle.filt2,Fs=fs,NFFT=len(dataAngle.filt2))
  plt.show()
  plt.psd(dataAngle.data3,Fs=fs,NFFT=len(dataAngle.data3))
  plt.psd(dataAngle.filt3,Fs=fs,NFFT=len(dataAngle.filt3))
  plt.show()
  
  
  w, h = signal.freqz(dataAngle.b_RII,dataAngle.a_RII)
  fig, ax1 = plt.subplots()
  ax1.set_title('Réponse fréquentielle du filtre RII')
  f = w/(2*np.pi)*fs
  ax1.plot(f, 20 * np.log10(abs(h)))
  ax1.set_ylabel('Amplitude [dB]')
  ax1.set_xlabel('Frequency [rad/sample]')
  plt.show()
  """
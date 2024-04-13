import time
import mediapipe as mp
import numpy as np
import cv2
import serial
import time
import queue
import threading
from multiprocessing import Process, Queue, Semaphore

"""
J'ai essayer de mettre de la strucure dans le code final (Holistic_TempsReel_Angle_CommunicationSerie). 
Je voulais faire différent process et avoir de la communication entre eux. 
Ainsi, on aurait pu avoir un temps reel qui calcul des angles et un code basé sur l'analyse de mouvement précis. 
Avec le thread de clavier, on choisit avec le terminal quel code se lance et on peux arrêter le programme.
Les process que je voulais faire etaient le code temps reel, le code qui compte les mouvememts et la communication port série
"""

#region Globales
#Commandes universelles
EXIT_COMMAND = "Q"
TEMPS_REEL = "T"
DETECT_MVT = "C"
NOMBRES_SEMAPHORES = 1

testSem = Semaphore(1) #sémaphore pour affichage de print
testSemInt = 1 #int pour savoir accès
#endregion
"""
arduin0 = serial.Serial(port='COM5', baudrate=115200, timeout=.1)

def write_read(x):
    data =""
    if arduino.is_open:
        arduino.write(bytes(x, 'utf-8'))
        time.sleep(0.05)
        data = arduino.readline()
    print(data)
"""

def communication():
    print(f"a' IN : <{angleA}, {angleB}, {angleD}, {pos_main}>")
    #write_read(f"<{angleA}, {angleB}, {angleD}, {pos_main}>")

def calculate_angle(a, b, c):
    a = np.array(a)
    b = np.array(b)
    c = np.array(c)

    radians = np.arctan2(c[1] - b[1], c[0] - b[0]) - np.arctan2(a[1] - b[1], a[0] - b[0])
    angle = np.abs(radians * 180.0 / np.pi)

    if angle > 180.0:
        angle = 360 - angle

    return int(angle)

def hand_stage(a, b, c, d, e, f, g, h, i, j):
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

    if i[1] < a[1] < b[1] and i[1] < c[1] < d[1] and i[1] < e[1] < f[1] and i[1] < g[1] < h[1] and b[0] < j[0] < h[0]:
        stage_m = 1

    elif b[1] < a[1] < i[1] and d[1] < c[1] < i[1] and f[1] < e[1] < i[1] and h[1] < g[1] < i[1] and h[0] < j[0] < b[0]:
        stage_m = 1

    else:
        stage_m = 0

    return stage_m

def cameraTR(TRQ, angleA, angleB, angleC, angleD, pos_main):
    mp_drawing = mp.solutions.drawing_utils
    mp_holistic = mp.solutions.holistic
    # VIDEO FEED
    cap = cv2.VideoCapture(0)
    # initiate holistic model
    # curl counter
    counter = 0
    stage = None

    # shoulder counter
    counterB = 0
    stageB = None

    # shoulder counter
    counterC = 0
    stageC = None

    # shoulder counter
    counterD = 0
    stageD = None

    stage_main = None

    with (mp_holistic.Holistic(min_detection_confidence=0.5, min_tracking_confidence=0.5) as holistic):
        while cap.isOpened():
            commande = ""
            #commande = TRQ.get()  # enregistre la commande du clavier

            ret, frame = cap.read()
            # recolor image to rgb
            image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            image.flags.writeable = False
            # make detection
            results = holistic.process(image)
            # recolor back to RGB
            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

            # Extract Landmarks
            try:
                # pour utiliser detection du corps
                landmarks = results.pose_landmarks.landmark

                # detection de la main
                landmarks2 = results.right_hand_landmarks.landmark

                shoulder = [landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER].x,
                            landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER].y]

                elbow = [landmarks[mp_holistic.PoseLandmark.RIGHT_ELBOW].x,
                         landmarks[mp_holistic.PoseLandmark.RIGHT_ELBOW].y]

                wrist = [landmarks[mp_holistic.PoseLandmark.RIGHT_WRIST].x,
                         landmarks[mp_holistic.PoseLandmark.RIGHT_WRIST].y]

                hip = [landmarks[mp_holistic.PoseLandmark.RIGHT_HIP].x,
                       landmarks[mp_holistic.PoseLandmark.LEFT_HIP].y]

                shoulderB = [landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER].y,
                             landmarks[mp_holistic.PoseLandmark.RIGHT_SHOULDER].z]

                elbowB = [landmarks[mp_holistic.PoseLandmark.RIGHT_ELBOW].y,
                          landmarks[mp_holistic.PoseLandmark.RIGHT_ELBOW].z]

                wristB = [landmarks[mp_holistic.PoseLandmark.RIGHT_WRIST].y,
                          landmarks[mp_holistic.PoseLandmark.RIGHT_WRIST.value].z]

                hipB = [landmarks[mp_holistic.PoseLandmark.RIGHT_HIP.value].y,
                        landmarks[mp_holistic.PoseLandmark.LEFT_HIP.value].z]

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

                angleA = calculate_angle(shoulder, elbow, wrist)
                angleB = calculate_angle(hip, shoulder, elbow)
                angleC = calculate_angle(hipB, shoulderB, elbowB)
                angleD = calculate_angle(shoulderC, shoulder, elbow)
                pos_main = hand_stage(index, index2, majeur, majeur2, annulaire, annulaire2, auriculaire, auriculaire2,
                                poignet, pouce)

                cv2.putText(image, str(angleA),
                            tuple(np.multiply(elbow, [640, 480]).astype(int)),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )

                cv2.putText(image, str(angleB),
                            tuple(np.multiply(hip, [640, 480]).astype(int)),
                                  cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )

                cv2.putText(image, str(angleC),
                            tuple(np.multiply(shoulder, [640, 480]).astype(int)),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )

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
            cv2.putText(image, stage_main, (450, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2, cv2.LINE_AA)

            mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_holistic.POSE_CONNECTIONS,
                                      mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=2, circle_radius=2),
                                      mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2, circle_radius=2)
                                      )

            mp_drawing.draw_landmarks(image, results.right_hand_landmarks, mp_holistic.HAND_CONNECTIONS,
                                      mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=2, circle_radius=2),
                                      mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2, circle_radius=2)
                                      )

            cv2.imshow('Mediapipe Feed', image)

            if commande == DETECT_MVT or commande == EXIT_COMMAND:
                cap.release()
                cv2.destroyAllWindows()
                break

    cap.release()
    cv2.destroyAllWindows()

#region Lecture Clavier
def read_kbd_input(inputQueue):
    if testSemInt == 1: #sémaphore d'affichage disponible ?
        testSem.acquire()
        print('Commande à executer : ')
        testSem.release()
    while (True):
        input_str = input()
        inputQueue.put(input_str)
#endregion

def main():
    angleA = -900
    angleB = -900
    angleC = -900
    angleD = -900
    pos_main = -900
    FirstTimeTR = False

    inputQueue = queue.Queue()
    tempsReelQueue = Queue()
    controlQueue = Queue()
    inputThread = threading.Thread(target=read_kbd_input, args=(inputQueue,), daemon=True)
    ap = Process(target=cameraTR, args=(tempsReelQueue, angleA, angleB, angleC, angleD, pos_main))

    inputThread.start()

    while (True):
        if (inputQueue.qsize() > 0):  # caractère entré avec le clavier?
            input_str = inputQueue.get()  # enregistre la commande du clavier
            # print("input_str = {}".format(input_str))

            tempsReelQueue.put(input_str)  # envoie la commande à la maintenance
            controlQueue.put(input_str)  # envoie la commande à l'avion'

            if (input_str == TEMPS_REEL and not FirstTimeTR):
                ap.start()
                FirstTimeTR = True

            elif (input_str == EXIT_COMMAND):
                print("Exiting serial terminal.")
                break

        time.sleep(0.01)


    ap.join()
    print("End.")

if (__name__ == '__main__'):
    main()
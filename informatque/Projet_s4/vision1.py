#import time

import mediapipe as mp
import numpy as np
import cv2
import serial

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
arduino = serial.Serial(port='COM5',   baudrate=115200, timeout=.1)

# VIDEO FEED
cap = cv2.VideoCapture(0)
#initiate holistic model

# curl counter
counter = 0
stage = None

# shoulder counter
counterB = 0
stageB = None

#shoulder counter
counterC = 0
stageC = None

#shoulder counter
counterD = 0
stageD = None

with mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose:
    while cap.isOpened():
        ret, frame = cap.read()
        #recolor image to rgb
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        image.flags.writeable = False
        #make detection
        results = pose.process(image)
        #recolor back to RGB
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        #Extract Landmarks
        try:
            landmarks = results.pose_landmarks.landmark

            shoulder = [landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].x,
                        landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].y]

            elbow = [landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].x,
                     landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].y]

            wrist = [landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].x,
                     landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].y]

            hip = [landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x,
                     landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y]

            shoulderB = [landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].y,
                        landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].z]

            elbowB = [landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].y,
                     landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].z]

            wristB = [landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].y,
                     landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].z]

            hipB = [landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y,
                     landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z]

            shoulderC = [landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].x,
                        landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].y]

            angle = calculate_angle(shoulder, elbow, wrist)
            angleB = calculate_angle(hip, shoulder, elbow)
            angleC = calculate_angle(hipB, shoulderB, elbowB)
            angleD = calculate_angle(shoulderC, shoulder, elbow)

            cv2.putText(image, str(angle),
                        tuple(np.multiply(elbow,[640, 480]).astype(int)),
                              cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )

            """cv2.putText(image, str(angleB),
                        tuple(np.multiply(shoulder, [640, 480]).astype(int)),
                              cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )"""
            cv2.putText(image, str(angleC),
                        tuple(np.multiply(shoulder, [640, 480]).astype(int)),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA, )


            # curl counter logic
            if angle > 160:
                stage = "down"
            if angle < 30 and stage == "down":
                stage = "up"
                counter += 1
                arduino.write("curl;")

            # shoulder counter logic
            if angleB > 150:
                stageB = "down"
            if angleB < 30 and stageB == "down" and angleC < 150:
                stageB = "up"
                counterB += 1
                arduino.write("question;")

            #Shoulder counter logic
            if angleC < 50:
                stageC = "down"
            if 80 < angleC < 100 and stageC == "down" and angleD < 170:
                stageC = "up"
                counterC += 1
                arduino.write("jab;")

            #Shoulder counter logic
            if angleD < 110:
                stageD = "down"
            if angleD > 170 and stageD == "down":
                stageD = "up"
                counterD += 1
                arduino.write("corde;")




        except:
            pass

        cv2.rectangle(image, (0, 0), (112, 72), (245, 117, 16), -1)

        cv2.putText(image,'REPS', (15, 12),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)

        cv2.putText(image, str(counter), (10, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)

        """cv2.putText(image,'STAGE', (65, 12),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)

        cv2.putText(image, stage, (60, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)"""

        cv2.rectangle(image, (112, 0), (224, 72), (30, 240, 16), -1)

        cv2.putText(image,'REPS', (120, 12),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)

        cv2.putText(image, str(counterB), (120, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)

        """cv2.putText(image,'STAGE', (310, 12),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)

        cv2.putText(image, stageB, (290, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)"""


        cv2.rectangle(image, (224, 0), (336, 72), (30, 16, 240), -1)

        cv2.putText(image, 'REPS', (240, 12),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)

        cv2.putText(image, str(counterC), (240, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)

        """cv2.putText(image,'STAGE', (530, 12),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1,cv2.LINE_AA)

        cv2.putText(image, stageC, (530, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2,cv2.LINE_AA)"""

        cv2.rectangle(image, (336, 0), (448, 72), (100, 100, 100), -1)

        cv2.putText(image,'REPS', (350, 12),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1, cv2.LINE_AA)

        cv2.putText(image, str(counterD), (350, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2, cv2.LINE_AA)

        cv2.putText(image,'STAGE', (450, 12),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1, cv2.LINE_AA)

        cv2.putText(image, stageC, (450, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2, cv2.LINE_AA)


        mp_drawing.draw_landmarks(image, results.pose_landmarks,mp_pose.POSE_CONNECTIONS,
                                  mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=2,circle_radius=2),
                                    mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2, circle_radius=2)
                                  )


        def calculate_angle(a, b, c):
            a = np.array(a)
            b = np.array(b)
            c = np.array(c)

            radians = np.arctan2(c[1]-b[1], c[0]-b[0]) - np.arctan2(a[1]-b[1], a[0]-b[0])
            angle = np.abs(radians*180.0/np.pi)

            if angle >180.0:
                angle = 360-angle

            return angle


        cv2.imshow('Mediapipe Feed', image)



        if cv2.waitKey(10) & 0xFF == ord('q'):
            break


    cap.release()
    cv2.destroyAllWindows()

    #arduino.write('home')



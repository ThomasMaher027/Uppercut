
import mediapipe as mp
import cv2

mp_drawing = mp.solutions.drawing_utils
mp_holistic = mp.solutions.holistic


cap = cv2.VideoCapture(0)
#initiate holistic model

with mp_holistic.Holistic(min_detection_confidence=0.5, min_tracking_confidence=0.5) as holistic:
    while cap.isOpened():
        ret, frame = cap.read()
        #recolor feed
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        #make detection
        results = holistic.process(image)
        print(results.face_landmarks)

        #recolor image back to BGR for rendering
        image = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)

        #draw face landmarks
        mp_drawing.draw_landmarks(image, results.face_landmarks, mp_holistic.FACEMESH_TESSELATION,
                                  mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2,circle_radius=1),
                                  mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2,circle_radius=2))

        #draw left hand mark
        mp_drawing.draw_landmarks(image, results.left_hand_landmarks, mp_holistic.HAND_CONNECTIONS,
                                  mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=2,circle_radius=1),
                                  mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=2,circle_radius=2))

        # draw right hand mark
        mp_drawing.draw_landmarks(image, results.right_hand_landmarks, mp_holistic.HAND_CONNECTIONS,
                                  mp_drawing.DrawingSpec(color=(0, 0, 255), thickness=2,circle_radius=1),
                                  mp_drawing.DrawingSpec(color=(0, 0, 255), thickness=2,circle_radius=2))

        # pose detection
        mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_holistic.POSE_CONNECTIONS,
                                  mp_drawing.DrawingSpec(color=(90, 90, 90), thickness=2,circle_radius=1),
                                  mp_drawing.DrawingSpec(color=(90, 90, 90), thickness=2,circle_radius=2))


        cv2.imshow('Raw webcam Feed', image)

        if cv2.waitKey(10) & 0xFF == ord('q'):
            break


cap.release()
cv2.destroyAllWindows()
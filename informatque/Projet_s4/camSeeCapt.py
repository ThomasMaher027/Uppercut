import mediapipe as mp
import cv2

mp_drawing = mp.solutions.drawing_utils
mp_holistic = mp.solutions.holistic
mp_drawing.DrawingSpec(color=(0, 0, 255), thickness=2, circle_radius=2)

#mp.solutions.holistic.FACEMESH_TESSELATION

cap = cv2.VideoCapture(0)

#initiate holistic model
with mp_holistic.Holistic(min_detection_confidence=0.5, min_tracking_confidence=0.5) as holistic:
    while cap.isOpened():
        ret, frame = cap.read()

        #recolor feed
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        #make detections
        results = holistic.process(image)
        #print(results.face_landmarks)

        # face_landmarks, pose_landmarks, left_hand_landmarks, right_hand_landmarks

        #recolor image back to bgr for rendering
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        #draw face landmarks
        mp_drawing.draw_landmarks(image, results.face_landmarks, mp_holistic.FACEMESH_TESSELATION)  #FACEMESH_CONTOURS)

        # left hand
        mp_drawing.draw_landmarks(image, results.left_hand_landmarks, mp_holistic.HAND_CONNECTIONS,
                                  mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2, circle_radius=2),
                                  mp_drawing.DrawingSpec(color=(0, 0, 255), thickness=2, circle_radius=2))  # FACEMESH_CONTOURS)

        # right hand
        mp_drawing.draw_landmarks(image, results.right_hand_landmarks, mp_holistic.HAND_CONNECTIONS,
                                  mp_drawing.DrawingSpec(color=(0, 0, 255), thickness=2, circle_radius=2),
                                  mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=2, circle_radius=2))  # FACEMESH_CONTOURS)

        # pose detections
        mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_holistic.POSE_CONNECTIONS,)  # FACEMESH_CONTOURS)

        # feed
        cv2.imshow('Holistic Model Detections', image)

        # how to close program
        if cv2.waitKey(10) & 0xFF == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()
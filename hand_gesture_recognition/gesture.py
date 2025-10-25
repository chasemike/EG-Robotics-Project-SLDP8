import cv2
import numpy as np
import mediapipe as mp
from mediapipe.framework.formats import landmark_pb2

# Path to your model
model_path = "hand_gesture_recognition/hand_landmarker.task"

# Mediapipe setup
BaseOptions = mp.tasks.BaseOptions
HandLandmarker = mp.tasks.vision.HandLandmarker
HandLandmarkerOptions = mp.tasks.vision.HandLandmarkerOptions
VisionRunningMode = mp.tasks.vision.RunningMode

options = HandLandmarkerOptions(
	base_options=BaseOptions(model_asset_path=model_path),
	running_mode=VisionRunningMode.IMAGE,
	num_hands=2,
)
hand_commands = ["Palm", "Finger Up", "Finger Down", "None"]
detector = HandLandmarker.create_from_options(options)

# Drawing utilities
MARGIN = 10
FONT_SIZE = 1
FONT_THICKNESS = 1
HANDEDNESS_TEXT_COLOR = (88, 205, 54)


def draw_landmarks_on_image(rgb_image, detection_result):
	hand_landmarks_list = detection_result.hand_landmarks
	handedness_list = detection_result.handedness
	annotated_image = np.copy(rgb_image)

	for idx, hand_landmarks in enumerate(hand_landmarks_list):
		handedness = handedness_list[idx]
		hand_landmarks_proto = landmark_pb2.NormalizedLandmarkList()
		hand_landmarks_proto.landmark.extend(
			[
				landmark_pb2.NormalizedLandmark(x=lmk.x, y=lmk.y, z=lmk.z)
				for lmk in hand_landmarks
			]
		)
		mp.solutions.drawing_utils.draw_landmarks(
			annotated_image,
			hand_landmarks_proto,
			mp.solutions.hands.HAND_CONNECTIONS,
			mp.solutions.drawing_styles.get_default_hand_landmarks_style(),
			mp.solutions.drawing_styles.get_default_hand_connections_style(),
		)
		h, w, _ = annotated_image.shape
		x_coords = [lmk.x for lmk in hand_landmarks]
		y_coords = [lmk.y for lmk in hand_landmarks]
		text_x = int(min(x_coords) * w)
		text_y = int(min(y_coords) * h) - MARGIN
		cv2.putText(
			annotated_image,
			handedness[0].category_name,
			(text_x, text_y),
			cv2.FONT_HERSHEY_DUPLEX,
			FONT_SIZE,
			HANDEDNESS_TEXT_COLOR,
			FONT_THICKNESS,
			cv2.LINE_AA,
		)
	return annotated_image

def determine_command(landmark):
  # * The Y-axis starts from the top of the screen (although not shwon negative)
	# * Hence the > is flipped to <
  if (result.hand_landmarks[0][4].y < result.hand_landmarks[0][1].y and 
      result.hand_landmarks[0][8].y < result.hand_landmarks[0][5].y and
      result.hand_landmarks[0][12].y < result.hand_landmarks[0][9].y and
      result.hand_landmarks[0][16].y < result.hand_landmarks[0][13].y and
      result.hand_landmarks[0][20].y < result.hand_landmarks[0][17].y):
    # print("Palm!!!")
    return hand_commands[0]
  elif (result.hand_landmarks[0][8].y < result.hand_landmarks[0][5].y):
    # print(f"Pointing UP:   8y={result.hand_landmarks[0][8].y:.2f} | 5y={result.hand_landmarks[0][5].y:.2f}")
    return hand_commands[1]
  else:
  #  print(f"Pointing DOWN: 8y={result.hand_landmarks[0][8].y:.2f} | 5y={result.hand_landmarks[0][5].y:.2f}")
   return hand_commands[2]
  return hand_commands[-1]
# OpenCV video loop
cap = cv2.VideoCapture(0)
current_command = hand_commands[-1]
while True:
  ret, frame = cap.read()
  if not ret:
    break
  
  frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
  mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=frame_rgb)
  result = detector.detect(mp_image)

  annotated = draw_landmarks_on_image(frame_rgb, result)
  annotated_bgr = cv2.cvtColor(annotated, cv2.COLOR_RGB2BGR)
  cv2.imshow("Hand Landmarks", annotated_bgr)
  
  if (result.hand_landmarks):
    current_command = determine_command(result.hand_landmarks)
  else:
    current_command = "None"

  print(current_command)
  
  if cv2.waitKey(1) & 0xFF == ord("x"):
    break
  
cap.release()
cv2.destroyAllWindows()

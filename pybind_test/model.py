import cv2
from ultralytics import YOLO
import time
import math
import json
from bindings import PriorityQueue

#hazards is a list containing hazardous objects
hazards = [
    "remote",
    "cell phone",
    "bicycle",
    "car",
    "motorbike",
    "aeroplane",
    "bus",
    "train",
    "truck",
    "boat",
    "bird",
    "dog",
    "horse",
    "sheep",
    "frisbee",
    "sports ball",
    "skateboard",
    "scissors"
]

def compute_distance(x1, y1, x2, y2):
    return math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)

def compute_speed(dist, time1, time2):
    if time2 - time1 == 0:
        return 0
    return dist / (time2 - time1)

#greater score -> closer the object is to the screen
def distance_score_from_bbox(y1, y2, frame_height):
    bbox_h = max(0, y2 - y1)
    if bbox_h <= 0:
        return 0.0
    return bbox_h / frame_height 

def generate_danger(object_name, dist_score_int, speed):
    for hazard in hazards:
        if object_name == hazard:
            return dist_score_int + speed
    return 0


model = YOLO("yolo11n.pt")
cap = cv2.VideoCapture(0)

def main():
    pq = PriorityQueue()

    best_scores = {}
    
    obj_hist = {}

    while True:
        dist_score_int = 0
        sza = pq.size()
        if (sza > 1):
            top_value = pq.top()
            
            if (top_value > 400):
                pq.pop()
                print("watch out! there is an object, move out of harms way!")

        ret, frame = cap.read()
        if not ret:
            break

        frame_h, frame_w = frame.shape[:2] 

        #verbose=False for no text output
        results = model.track(frame, persist=True, verbose=False)[0]

        for b in results.boxes:

            if b.id is None:
                continue
            obj_id = int(b.id[0])
            object_name = results.names[int(b.cls[0])]

            seconds = time.perf_counter()
            x_center, y_center, w, h = b.xywh[0]
            x1, y1, x2, y2 = map(int, b.xyxy[0])

            if obj_id not in obj_hist:
                obj_hist[obj_id] = []

            obj_hist[obj_id].append((seconds, float(x_center), float(y_center)))
            speed = 0

            if len(obj_hist[obj_id]) > 1:
                dist_score = distance_score_from_bbox(y1, y2, frame_h)
                dist_score_int = int(dist_score * 300)
                
                time1, x1center, y1center = obj_hist[obj_id][-2]
                time2, x2center, y2center = obj_hist[obj_id][-1]

                dist = compute_distance(x1center, y1center, x2center, y2center)
                speed = compute_speed(dist, time1, time2)
                
                danger_score = generate_danger(object_name, dist_score_int, speed)
                
                if object_name in hazards:
                    if object_name not in best_scores or danger_score > best_scores[object_name]:
                        best_scores[object_name] = danger_score
                        pq.push(danger_score, object_name)

            label = f"{results.names[int(b.cls[0])]} ID:{obj_id} Speed:{int(speed)}px/s Distance:{dist_score_int}"
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0,255,0), 2)
            cv2.putText(frame, label,  (x1, y1-5),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0,255,0), 2)

        cv2.imshow("YOLO", frame)
        if cv2.waitKey(1) == ord('q'):
            break
        
if __name__ == "__main__":
    main()
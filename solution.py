from PIL import Image
from ultralytics import YOLO
from os.path import exists
import ultralytics
import torch
import eco2ai

tracker = eco2ai.Tracker(
	project_name="Cyclone_detection",
	experiment_description="train YOLO",
	file_name="emission.csv"
	)

name = input("Provide a name of file for recognition with .jpg extension (or TRAIN for training the model): ")

# Just for baseline model we will use yolov8n.pt. TODO: replace it with better model on production
model = YOLO('yolov8n' if not exists('runs/detect/yolov8n_custom/weights/best.pt') else 'runs/detect/yolov8n_custom/weights/best.pt')

# Use GPU for computing
model.to('cuda')

if name == "TRAIN":
	# Output info about current setup
	ultralytics.checks()
	tracker.start()
	# For baseline model just a few epochs. TODO: train better for production
	results = model.train(data='datasets/custom/data.yaml',
			imgsz=640,
			batch=8,
			epochs=20,
			name='yolov8n_custom'
			)
	tracker.stop()
else:
	# Process image
	results = model(name)
	# Save image in human verifiable form
	Image.fromarray(results[0].plot(pil=True)).save("out.jpg")
	#results[0].save_txt("out.txt") We will use format as specified. Not yolo's format.
	with open("out.txt", "w") as out_file:
		for i in range(len(results[0].boxes.cls)):
			c = int(results[0].boxes.cls[i]) + 1
			x = int(results[0].boxes.xywh[i][0])
			y = int(results[0].boxes.xywh[i][1])
			w = int(results[0].boxes.xywh[i][2])
			h = int(results[0].boxes.xywh[i][3])
			line = f'Class_{c}, {x}, {y}, {w}, {h}\n'
			out_file.write(line)

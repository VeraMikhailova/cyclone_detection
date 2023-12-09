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
device = 'cuda' if torch.cuda.is_available() else "cpu"


# Just for baseline model we will use yolov8n.pt. TODO: replace it with better model on production
model = YOLO('yolov8n' if not exists('runs/detect/yolov8n_custom/weights/best.pt') else 'runs/detect/yolov8n_custom/weights/best.pt')

# Use GPU for computing
model.to(device)


# Output info about current setup
ultralytics.checks()
tracker.start()
# For baseline model just a few epochs. TODO: train better for production
results = model.train(data='datasets/custom/data.yaml',
		imgsz=640,
		batch=8,
		epochs=5,
		name='yolov8n_custom'
			)
tracker.stop()


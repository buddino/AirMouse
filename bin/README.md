How to use binaries
=======
 
* **AirMouse**: the main program, it uses a trained pipeline to recognize hand poses and gestures and control your pointer
* **StaticGestureTrainer**: records the dataset for the hand poses and outputs a dataset file
* **ClickGestureTrainer**: records the dataset for the click gesture and outputs a dataset file
* **MergeTrainigSet**: merge different dataset for static hand poses (samples from different people) and outputs a trained pipeline. Useful if you want to train the static poses recognizer with samples from different people.

AirMouse
--------
AirMouse is the main program which takes as argument a configuration file.
###Configuration
```
pose-pipeline = "pipelines/TrainedPipeline4";
leftclick-pipeline = "pipelines/LeftClickPipeline";
rightclick-pipeline = "pipelines/RightClickPipeline";
base-speedfactor = 8.0;
shift-threshold = 0.05;
circle-progress = 1.2;		#radians
sampling-period = 10;		#millisecond
scroll-debounce = 200; 		#milliseconds
click-debounce = 1000;		#milliseconds
```


StaticGestureTrainer
--------

ClickGestureTrainer
--------

MergeTrainingSet
--------
Merge different dataset for static hand poses and outputs a trained pipeline. Useful if you want to train the static poses recognizer with samples from different people. If you have only one dataset file do the same with only that file in order to produce a trained pipeline.

###Usage:
1. Put all training sets into a folder (for example "test")
2. Launch the program passing the folder name as argument (./*MergeTrainingSet test*)
3. It will merge all the training sets into one that will be partitioned into train (80%) e test (20%), train with the given samples and output  the GRT pipeline as "TrainedPipeline". This file can be passed to *AirMouse*.


Heading
=======
 
* AirMouse: the main program, it uses a trained pipeline to recognize hand poses and gestures and control your pointer
* StaticGestureTrainer: records the dataset for the hand poses and outputs a dataset file
* ClickGestureTrainer: records the dataset for the click gesture and outputs a dataset file
* MergeTrainigSet: merge different dataset for static hand poses (samples from different people) and outputs a trained pipeline. Useful if you want to train the static poses recognizer with samples from different people.

MergeTrainingSet
----------------
Put all training sets into a folder (for example "test")
Launch the program passing the folder name as argument
It will merge all the training sets into one that will be partitioned into train (80) e test (20).
The program will output the GRT pipeline as "TrainedPipeline"


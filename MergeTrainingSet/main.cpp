#include <iostream>
#include <GRT/GRT.h>

using namespace std;

int main(int argc, char* argv[]) {
    GRT::ClassificationData trainSet;
    GRT::ClassificationData tempTrainset;
    GRT::GestureRecognitionPipeline pipeline;

    DIR *dir;
    struct dirent *ent;
    if (argc == 1){
        perror ("You should pass training data folder!");
        return EXIT_FAILURE;
    }
    string folder = argv[1];
    if ((dir = opendir (argv[1])) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(!strcmp(ent->d_name,".")) continue;
            if(!strcmp(ent->d_name,"..")) continue;
            string path = folder + "/" + ent->d_name;
            tempTrainset.load(path);
            printf("Loaded %d samples from %s\n", tempTrainset.getNumSamples(), ent->d_name);
            if ( trainSet.getNumDimensions() == 0 ){
                trainSet.setNumDimensions(tempTrainset.getNumDimensions());
            }
            trainSet.merge(tempTrainset);
            printf("Trainset has now %d sample\n", trainSet.getNumSamples());
            tempTrainset.clear();
        }
        closedir (dir);

        //Train pipeline
        trainSet.printStats();

        GRT::SVM svm;
        svm.enableNullRejection(true);
        svm.setKernelType(GRT::RBF);
        svm.enableAutoGamma(true);
        svm.enableCrossValidationTraining(true);
        svm.setKFoldCrossValidationValue(5);
        pipeline.setClassifier(svm);
        pipeline.train(trainSet);
s        printf("CrossValidation accuracy: %.2f\n",pipeline.getCrossValidationAccuracy());
        pipeline.save("TrainedPipeline");
        return 0;


    } else {
        /* could not open directory */
        perror ("Cannot open directory");
        return EXIT_FAILURE;
    }
}
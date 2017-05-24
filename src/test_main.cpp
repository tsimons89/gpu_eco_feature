#include <opencv2/opencv.hpp>
#include <opencv2/core/cuda.hpp>
#include "Eco_population.hpp"
#include "Data_set.hpp"
#include "Eco_train_options.hpp"
#include "Adaboost_model.hpp"
#include "probability.hpp"
#include <ctime>

using namespace std;
using namespace cv;

int main(int argc, char * argv[]){
	Eco_train_options options;
	options.parse_options(argc,argv);
	init_rand();
	Adaboost_model trainer;
	trainer.train();
	trainer.write();
}

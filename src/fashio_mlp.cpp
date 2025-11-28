#include <fstream>

#include <gtest/gtest.h>
#include <Eigen/Dense>

#include <mlp_classifier.h>

#include <helpers.h>

using namespace std;
using namespace mnist;

const size_t input_dim = 784;
const size_t hidden_dim = 128;
const size_t output_dim = 10;


int main(int argc, char* argv[])
{
  try{

    if (argc != 4)
    {
      std::cerr << "Usage: " << argv[0] << " <test file> <w1 file> <w2 file>";
      return 1;
    }

    auto w1 = read_mat_from_file(input_dim, hidden_dim, string(argv[2]).c_str());
    auto w2 = read_mat_from_file(hidden_dim, output_dim, string(argv[3]).c_str());

    auto clf = MlpClassifier{w1.transpose(), w2.transpose()};

    auto features = MlpClassifier::features_t{};


    std::ifstream test_data{string(argv[1])};
    if (!test_data.is_open())
    	throw std::runtime_error("unable to open test model file test.csv");


//    test_data.set_delim(',');


    int counter = 0;;
    int success = 0;
    for (;;) {
//        proba_true.clear();
//        for (size_t i = 0; i < output_dim; ++i) {
//            float val;
//            test_data >> val;
//            proba_true.push_back(val);
//        }
	int val;
	test_data >> val;
	test_data.ignore(1);
        if (!read_features_csv(test_data, features)) {
            break;
        }
        auto proba_pred = clf.predict_proba(features);
	double max = 0;
	int pred_class = 0;
        for (size_t i = 0; i < output_dim; ++i) {
	    if (proba_pred[i] > max)
            {
		max = proba_pred[i];
		pred_class = i;
	    }
        }
	counter++;
	if (val == pred_class)
	    success++;
     }
    cout << (double)success / counter << endl;
   }
   catch (const std::exception& ex)
  {
    std::cerr << "Exception: " << ex.what() << "\n";
  }
}

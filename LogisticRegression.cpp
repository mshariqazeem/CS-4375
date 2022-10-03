#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <chrono>    

using namespace std;
using namespace std::chrono;

/* This function peforms sigmoid algorithm on each value of the given vector. */
vector<double> sigmoid(vector<double> z)
{
	// go through each value of the vector
	for(int i = 0; i < z.size(); i++)
	{
		z[i] = 1 / (1 + exp(-1 * z[i]));
	}

	return z;
}

/* This function multiplies a matrix with a vector. */
vector<double> multiplyMatrix(vector<vector<double>> matrixOne, vector<double> matrixTwo)
{
	int rowOne = matrixOne.size();		// number of rows of the matrix
	int colOne = matrixOne[0].size();	// number of columns of the matrix

	vector<double> matrixThree(rowOne);	// resultant matrix

	for (int i = 0; i < rowOne; i++)
	{
		for (int k = 0; k < colOne; k++)
		{
			matrixThree[i] += matrixOne[i][k] * matrixTwo[k];
		}
	}

	return matrixThree;
}

/* This function takes in a matrix and returns its transpose. */
vector<vector<double>> transposeMatrix(vector<vector<double>> matrix)
{
	vector<vector<double>> matrixTransposed(matrix[0].size(), vector<double>(matrix.size()));

	for (int i = 0; i < matrix[i].size(); i++)
	{
		for (int j = 0; j < matrix.size(); j++)
		{
			// switch the rows and columns
			matrixTransposed[i][j] = matrix[j][i];
		}
	}

	return matrixTransposed;
}

/* This function takes in a weight vector, labels vector, and a data matrix.
	It uses those parameters to calculate the actual weights of the data. */
vector<double> calculateWeights(vector<double> weights, vector<vector<double>> dataMatrix, vector<int> labels)
{
	double learningRate = 0.001;
	vector<double> error(labels.size());

	for (int i = 0; i < 500000; i++)
	{
		// calculate the log likelihood
		vector<double> logOdds = multiplyMatrix(dataMatrix, weights);
		
		// get the probabilities
		vector<double> probVector = sigmoid(logOdds);

		// compute the error
		for (int j = 0; j < labels.size(); j++)
		{
			error[j] = labels[j] - probVector[j];
		}

		// calculate the gradient
		vector<vector<double>> transpose = transposeMatrix(dataMatrix);
		vector<double> gradient = multiplyMatrix(transpose, error);

		// update the weights
		for (int k = 0; k < weights.size(); k++)
		{
			weights[k] = weights[k] + (learningRate * gradient[k]);
		}
	}

	return weights;
}

/* This function converts predicted values to actual probabilities. */
vector<double> calculateProbabilities(vector<double> predicted)
{
	vector<double> probabilities(predicted.size());

	for (int i = 0; i < predicted.size(); i++)
	{
		probabilities[i] = exp(predicted[i]) / (1 + exp(predicted[i]));
	}

	return probabilities;
}

/* This function uses the probabilities vector to make predictions. */
vector<int> makePredictions(vector<double> probabilities)
{
	vector<int> predictions(probabilities.size());

	for (int i = 0; i < probabilities.size(); i++)
	{
		if(probabilities[i] > 0.5)
		{
			predictions[i] = 1;
		}
		else
		{
			predictions[i] = 0;
		}
	}

	return predictions;
}

/* This function calculates the accuracy of the algorithm. */
double calculateAccuracy(vector<int> labels, vector<int> predictions)
{
	int numRight = 0;

	for(int i = 0; i < labels.size(); i++)
	{
		if(predictions[i] == labels[i])
		{
			numRight++;
		}
	}

	double accuracy = (double)numRight/labels.size();
	return accuracy;
}

/* This function calculates the sensitivity of the algorithm. */
double calculateSensitivity(vector<int> labels, vector<int> predictions)
{
	int truePositives = 0;
	int falseNegatives = 0;

	for (int i = 0; i < labels.size(); i++)
	{
		if((predictions[i] == 1) && (labels[i] == 1))
		{
			truePositives++;
		}
		else if((predictions[i] != 1) && (labels[i] == 1))
		{
			falseNegatives++;
		}
	}

	double sensitivity = (double)truePositives / ((double)truePositives + (double)falseNegatives);
	return sensitivity;
}

/* This function calculates the specificity of the algorithm. */
double calculateSpecificity(vector<int> labels, vector<int> predictions)
{
	int trueNegatives = 0;
	int falsePositives = 0;

	for (int i = 0; i < labels.size(); i++)
	{
		if ((predictions[i] == 0) && (labels[i] == 0))
		{
			trueNegatives++;
		}
		else if ((predictions[i] != 0) && (labels[i] == 0))
		{
			falsePositives++;
		}
	}

	double specificity = ((double)trueNegatives) / ((double)trueNegatives + (double)falsePositives);
	return specificity;
}

int main()
{
	string line;
	ifstream inFS;
	string index_in, pclass_in, survived_in, sex_in, age_in;
	int numObservations = 0;

	const int MAX_LEN = 1500;
	vector<int> index(MAX_LEN);
	vector<int> pclass(MAX_LEN);
	vector<int> survived(MAX_LEN);
	vector<int> sex(MAX_LEN);
	vector<int> age(MAX_LEN);

	// open the file
	inFS.open("titanic_project.csv");
	if (!inFS.is_open())
	{
		cout << "Could not open file titanic_project.csv." << endl;
		return 1;
	}

	// ignore the first line
	getline(inFS, line);

	// read the data and save it in vectors
	while (inFS.good())
	{
		getline(inFS, index_in, ',');
		getline(inFS, pclass_in, ',');
		getline(inFS, survived_in, ',');
		getline(inFS, sex_in, ',');
		getline(inFS, age_in, '\n');

		index_in.erase(remove(index_in.begin(),index_in.end(),'"'),index_in.end());

		index.at(numObservations) = stoi(index_in);
		pclass.at(numObservations) = stoi(pclass_in);
		survived.at(numObservations) = stoi(survived_in);
		sex.at(numObservations) = stoi(sex_in);
		age.at(numObservations) = stoi(age_in);

		numObservations++;
	}

	// resize the vectors
	index.resize(numObservations);
	pclass.resize(numObservations);
	survived.resize(numObservations);
	sex.resize(numObservations);
	age.resize(numObservations);

	// close the file
	inFS.close();

	cout << "Data has been read from the file." << endl;

	// divide the data set into train and test
	vector<vector<int>> train(800, vector<int>(2));
	vector<vector<int>> test(numObservations - 800, vector<int>(2));

	for (int i = 0; i < numObservations; i++)
	{
		// the first 800 values are for training
		if (i < 800)
		{
			train[i][0] = sex[i];
			train[i][1] = survived[i];
		}
		// the rest of the values are for testing
		else
		{
			test[i-800][0] = sex[i];
			test[i-800][1] = survived[i];
		}
	}

	cout << "Data is divided into train and test." << endl << endl;

	// variables needed for training
	vector<double> weights = {1, 1};
	vector<vector<double>> dataMatrix(train.size(), vector<double>(2));
	vector<int> labels(train.size());

	// set up the training variables
	for (int i = 0; i < train.size(); i++)
	{
		dataMatrix[i][0] = 1;
		dataMatrix[i][1] = train[i][0];
		labels[i] = train[i][1];
	}

	cout << "Training the algorithm ..." << endl;

	// strat the clock
	auto start = high_resolution_clock::now();

	// train the algorithm
	weights = calculateWeights(weights, dataMatrix, labels);
	
	// stop the clock
	auto stop = high_resolution_clock::now();

	// get the time it took to train
	auto duration = duration_cast<minutes>(stop - start);

	cout << "Training Completed." << endl;

	// print the time it took to train
	cout << "Time taken to train the algorithm: "
		<< duration.count() << " minutes" << endl << endl;

	// print the calculated values
	cout << "Calculated Weights:" << endl
		<< "Intercept: " << weights[0] << endl
		<< "Coefficient: " << weights[1] << endl << endl;

	// variable needed for testing
	vector<vector<double>> testMatrix(test.size(), vector<double>(2));
	vector<int> testLabels(test.size());

	// set up the testing variables
	for (int i = 0; i < test.size(); i++)
	{
		testMatrix[i][0] = 1;
		testMatrix[i][1] = test[i][0];
		testLabels[i] = test[i][1];
	}

	// use the weights to get log odds on the test data
	vector<double> predicted = multiplyMatrix(testMatrix, weights);

	// use the log odds to get the actual probabailities
	vector<double> probabilities = calculateProbabilities(predicted);

	// use the probabilities to make predictions on the test data
	vector<int> predictions = makePredictions(probabilities);

	// use the predictions and actual values to calculate test metrics
	double accuracy = calculateAccuracy(testLabels, predictions);
	double sensitivity = calculateSensitivity(testLabels, predictions);
	double specificity = calculateSpecificity(testLabels, predictions);

	// Print the test metrics.
	cout << "Test Metrics:" << endl;
	cout << "Accuracy: " << accuracy << endl;
	cout << "Sensitivity: " << sensitivity << endl;
	cout << "Specificity: " << specificity << endl;

	return 0;
}
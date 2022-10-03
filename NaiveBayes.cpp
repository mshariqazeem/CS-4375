#define _USE_MATH_DEFINES
#include <cmath>
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

vector<vector<double>> lh_pclass;
vector<vector<double>> lh_sex;
vector<double> apriori;
vector<double> age_mean;
vector<double> age_var;

double calc_age_lh(double v, double mean_v, double var_v)
{
	return 1 / sqrt(2 * M_PI * var_v) * exp(-(pow(v - mean_v, 2)) / (2 * var_v));
}

vector<double> calc_raw_prob(int pclass, int sex, double age)
{
	double num_s = lh_pclass[1][pclass - 1] * lh_sex[1][sex] * apriori[1] * calc_age_lh(age, age_mean[1], age_var[1]);
	double num_p = lh_pclass[0][pclass - 1] * lh_sex[0][sex] * apriori[0] * calc_age_lh(age, age_mean[0], age_var[0]);
	double denominator = (lh_pclass[1][pclass - 1] * lh_sex[1][sex] * calc_age_lh(age, age_mean[1], age_var[1]) * apriori[1])
						+ (lh_pclass[0][pclass - 1] * lh_sex[0][sex] * calc_age_lh(age, age_mean[0], age_var[0]) * apriori[0]);
	double prob_survived = num_s / denominator;
	double prob_perished = num_p / denominator;

	vector<double> raw = {prob_perished, prob_survived};

	return raw;
}

/* This function calculates the accuracy of the algorithm. */
double calculateAccuracy(vector<int> labels, vector<int> predictions)
{
	int numRight = 0;

	for (int i = 0; i < labels.size(); i++)
	{
		if (predictions[i] == labels[i])
		{
			numRight++;
		}
	}

	double accuracy = (double)numRight / labels.size();
	return accuracy;
}

/* This function calculates the sensitivity of the algorithm. */
double calculateSensitivity(vector<int> labels, vector<int> predictions)
{
	int truePositives = 0;
	int falseNegatives = 0;

	for (int i = 0; i < labels.size(); i++)
	{
		if ((predictions[i] == 1) && (labels[i] == 1))
		{
			truePositives++;
		}
		else if ((predictions[i] != 1) && (labels[i] == 1))
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

		index_in.erase(remove(index_in.begin(), index_in.end(), '"'), index_in.end());

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
	vector<vector<int>> train(800, vector<int>(4));
	vector<vector<int>> test(numObservations - 800, vector<int>(4));

	for (int i = 0; i < numObservations; i++)
	{
		// the first 800 values are for training
		if (i < 800)
		{
			train[i][0] = pclass[i];
			train[i][1] = sex[i];
			train[i][2] = age[i];
			train[i][3] = survived[i];
		}
		// the rest of the values are for testing
		else
		{
			test[i - 800][0] = pclass[i];
			test[i - 800][1] = sex[i];
			test[i - 800][2] = age[i];
			test[i - 800][3] = survived[i];
		}
	}

	cout << "Data is divided into train and test." << endl << endl;

	cout << "Training the algorithm ..." << endl;

	// strat the clock
	auto start = high_resolution_clock::now();

	int countZero = 0, countOne = 0;

	for (int i = 0; i < train.size(); i++)
	{
		if(train[i][3] == 0)
		{
			countZero++;
		}
		else
		{
			countOne++;
		}
	}

	double aprioriZero = (double)countZero / train.size();
	double aprioriOne = (double)countOne / train.size();
	apriori = {aprioriZero, aprioriOne};

	vector<int> count_survived = {countZero, countOne};

	double classOneDead = 0, classTwoDead = 0, classThreeDead = 0, classOneSurvived = 0, classTwoSurvived = 0, classThreeSurvived = 0;

	for (int i = 0; i < train.size(); i++)
	{
		if (train[i][0] == 1 && train[i][3] == 0)
		{
			classOneDead++;
		}
		else if (train[i][0] == 2 && train[i][3] == 0)
		{
			classTwoDead++;
		}
		else if (train[i][0] == 3 && train[i][3] == 0)
		{
			classThreeDead++;
		}
		else if (train[i][0] == 1 && train[i][3] == 1)
		{
			classOneSurvived++;
		}
		else if (train[i][0] == 2 && train[i][3] == 1)
		{
			classTwoSurvived++;
		}
		else if (train[i][0] == 3 && train[i][3] == 1)
		{
			classThreeSurvived++;
		}
	}
	
	lh_pclass = {{classOneDead / countZero, classTwoDead / countZero, classThreeDead / countZero}, 
				{classOneSurvived / countOne, classTwoSurvived / countOne, classThreeSurvived / countOne}};

	double femaleDead = 0, femaleSurvived = 0, maleDead = 0, maleSurvived = 0;

	for (int i = 0; i < train.size(); i++)
	{
		if (train[i][1] == 0 && train[i][3] == 0)
		{
			femaleDead++;
		}
		else if (train[i][1] == 0 && train[i][3] == 1)
		{
			femaleSurvived++;
		}
		else if (train[i][1] == 1 && train[i][3] == 0)
		{
			maleDead++;
		}
		else if (train[i][1] == 1 && train[i][3] == 1)
		{
			maleSurvived++;
		}
	}

	lh_sex = {{femaleDead / countZero, maleDead / countZero}, 
			{femaleSurvived / countOne, maleSurvived / countOne}};

	double ageDead = 0, ageSurvived = 0;

	for(int i = 0; i < train.size(); i++)
	{
		if(train[i][3] == 0)
		{
			ageDead += train[i][2];
		}
		else
		{
			ageSurvived += train[i][2];
		}
	}

	age_mean = {ageDead / countZero, ageSurvived / countOne};

	double varDead = 0, varSurvived = 0;

	for (int i = 0; i < train.size(); i++)
	{
		if (train[i][3] == 0)
		{
			varDead += pow(train[i][2] - age_mean[0], 2);
		}
		else
		{
			varSurvived += pow(train[i][2] - age_mean[1], 2);
		}
	}

	age_var = {varDead / ((double)countZero - 1), varSurvived / ((double)countOne - 1)};

	// stop the clock
	auto stop = high_resolution_clock::now();

	// get the time it took to train
	auto duration = duration_cast<microseconds>(stop - start);

	cout << "Training Completed." << endl;

	// print the time it took to train
	cout << "Time taken to train the algorithm: "
		<< duration.count() << " microseconds" << endl << endl;
	
	cout << "Information learned from the data:" << endl;
	cout << "pclass:" << endl;
	cout << "   Class 1  Class 2  Class3" << endl;
	cout << 0 << ": " << lh_pclass[0][0] << " " << lh_pclass[0][1] << " " << lh_pclass[0][2] << endl
		<< 1 << ": " << lh_pclass[1][0] << " " << lh_pclass[1][1] << " " << lh_pclass[1][2] << endl << endl;

	cout << "sex:" << endl;
	cout << "    Female    Male" << endl;
	cout << 0 << ": " << lh_sex[0][0] << " " << lh_sex[0][1] << endl
		<< 1 << ": " << lh_sex[1][0] << " " << lh_sex[1][1] << endl << endl;

	cout << "age:" << endl;
	cout << "    Mean     Std" << endl;
	cout << 0 << ": " << age_mean[0] << " " << sqrt(age_var[0]) << endl
		<< 1 << ": " << age_mean[1] << " " << sqrt(age_var[1]) << endl << endl;

	vector<int> testLabels(test.size());

	for (int i = 0; i < test.size(); i++)
	{
		testLabels[i] = test[i][3];
	}

	vector<double> raw;
	vector<int> predictions(test.size());

	for(int i = 0; i < test.size(); i++)
	{
		raw = calc_raw_prob(test[i][0], test[i][1], test[i][2]);
		if(raw[0] > raw[1])
		{
			predictions[i] = 0;
		}
		else if(raw[1] > raw[0])
		{
			predictions[i] = 1;
		}
	}

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
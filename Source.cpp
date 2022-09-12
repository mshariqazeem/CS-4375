#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

/*This function calculates the sum of all the values in the given vector.*/
double calculateSum(vector<double> v)
{
	double sum = 0.0;
	for (int i = 0; i < v.size(); i++)
	{
		sum = sum + v[i];
	}
	return sum;
}

/*This function calculates the mean of all the values in the given vector.*/
double calculateMean(vector<double> v)
{
	double sum = calculateSum(v);
	int size = v.size();
	double mean = (double)(sum / size);
	return mean;
}

/*This function calculates the median of all the values in the given vector.*/
double calculateMedian(vector<double> v)
{
	double median;
	int size = v.size();
	sort(v.begin(), v.end());	// sort the vector

	// if the vector has an even number of values, average the two middle values
	if (size % 2 == 0)
	{
		double firstMiddle = v[size / 2];
		double secondMiddle = v[(size / 2) + 1];
		median = (firstMiddle + secondMiddle) / 2;
	}
	// if the vector has an odd number of values, select the middle value
	else
	{
		median = v[size / 2];
	}
	return median;
}

/*This function calculates the range between the max and min values of the given vecctor.*/
double calculateRange(vector<double> v)
{
	double max = -1000000;
	double min = 1000000;
	double range;

	for (int i = 0; i < v.size(); i++)
	{
		// find the max value
		if (v[i] > max)
		{
			max = v[i];
		}

		// find the min value
		if (v[i] < min)
		{
			min = v[i];
		}
	}

	range = max - min;
	return range;
}

/*This function calculates the covariance between two sets of values.*/
double calculateCovariance(vector<double> x, vector<double> y)
{
	double meanX = calculateMean(x);	// mean of the first vector
	double meanY = calculateMean(y);	// mean of the second vector
	double num = 0.0;

	// calculate the numerator for the covariance formula
	for (int i = 0; i < x.size(); i++)
	{
		num += (x[i] - meanX) * (y[i] - meanY);
	}

	double covariance = num / (x.size() - 1);
	return covariance;
}

/*This function calculates the correlation between two sets of values.*/
double calculateCorrelation(vector<double> x, vector<double> y)
{
	// calculate sigma of vector x
	double varianceX = calculateCovariance(x, x);
	double sigmaX = sqrt(varianceX);

	// calculate sigma of vector y
	double varianceY = calculateCovariance(y, y);
	double sigmaY = sqrt(varianceY);

	// get the covariance of vectors x and y
	double covariance = calculateCovariance(x, y);

	// calculate the correlation between vector x and y
	double correlation = covariance / (sigmaX * sigmaY);
	return correlation;
}

int main()
{
	string line;
	ifstream inFS;
	string rm_in, medv_in;
	int numObservations = 0;
	const int MAX_LEN = 1000;
	vector<double> rm(MAX_LEN);
	vector<double> medv(MAX_LEN);

	// open the file
	inFS.open("Boston.csv");
	if (!inFS.is_open())
	{
		cout << "Could not open file Boston.csv." << endl;
		return 1;
	}

	cout << "The file is opened." << endl;

	// ignore the first line
	getline(inFS, line);

	// read the data and save it in vectors
	while (inFS.good())
	{
		getline(inFS, rm_in, ',');
		getline(inFS, medv_in, '\n');

		rm.at(numObservations) = stof(rm_in);
		medv.at(numObservations) = stof(medv_in);

		numObservations++;
	}

	cout << "The data has been read from the file." << endl;

	// resize the vectors
	rm.resize(numObservations);
	medv.resize(numObservations);

	// close the file
	inFS.close();

	cout << "The file is closed." << endl << endl;

	// print the calculations
	cout << "Statistics calculated from that data can be found below." << endl << endl;

	cout << "Sum of rm: " << calculateSum(rm) << endl;
	cout << "Sum of medv: " << calculateSum(medv) << endl << endl;

	cout << "Mean of rm: " << calculateMean(rm) << endl;
	cout << "Mean of medv: " << calculateMean(medv) << endl << endl;

	cout << "Median of rm: " << calculateMedian(rm) << endl;
	cout << "Median of medv: " << calculateMedian(medv) << endl << endl;

	cout << "Range of rm: " << calculateRange(rm) << endl;
	cout << "Range of medv: " << calculateRange(medv) << endl << endl;

	cout << "Covariance between rm and medv: " << calculateCovariance(rm, medv) << endl;
	cout << "Correlation between rm and medv: " << calculateCorrelation(rm, medv) << endl;
}
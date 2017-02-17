#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

class Neuron;

typedef std::vector<Neuron> Layer;

struct Connection
{
	double weight;
	double deltaweight;
};

class Net
{
public:
	Net(const std::vector<unsigned> &topology);
	void feedForward(const std::vector<double> &inputVals);
	void backProp(const std::vector<double> &targetVals);
	void getResults(std::vector<double> &resultVals) const;
	double getRecentAverageError(void) const { return m_recentAverageError; };
	void setWeights(std::vector<double> &newWeights);
	void getWeights(std::vector<double> &newWeights);

private:
	std::vector<Layer> m_layers; // m_layers[layerNum][neuronNum]
	double m_error;
	double m_recentAverageError;
	double m_recentAverageSmoothingFactor;
};

class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myIndex);
	Neuron(unsigned numOutputs, unsigned myIndex, unsigned numNeuronsInCol);
	void setOutputVal(double val) { m_outputVal = val; }
	double getOutputVal(void) const { return m_outputVal; }
	void feedForward(const Layer &prevLayer);
	void calcOutputGradients(double targetVal);
	void calcHiddenGradients(const Layer &nextLayer);
	void updateInputWeights(Layer &prevLayer);
	std::vector<Connection> m_outputWeights;
	void Neuron::getWeight(std::vector<double> &newWeights, const Layer &prevLayer);
	void Neuron::setWeight(std::vector<double> &newWeights, Layer &prevLayer, unsigned &count);
	unsigned m_myIndex;
	double m_outputVal;

private:
	static double eta; // [0.0 - 1.0] overall net training weight
	static double alpha; //[0.0 - n] momentum multiplier
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);
	static double randomWeight(void) { return rand() / double(RAND_MAX); }
	double sumDOW(const Layer &nextLayer) const;

	double m_gradient;
};



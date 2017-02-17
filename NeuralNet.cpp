#include "stdafx.h"
#include "NeuralNet.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>

using namespace std;



Net::Net(const vector<unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
		m_layers.push_back(Layer());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		//We have made a new layer, now fill it with neurons
		// and add abias neuron to the layer
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum) {
			m_layers.back().push_back(Neuron(numOutputs, neuronNum));
			cout << "Made a Neuron!" << endl;
		}

		//force bias value
		m_layers.back().back().setOutputVal(1.0);
	}
}

void Net::feedForward(const vector<double> &inputVals)
{
	assert(inputVals.size() == (m_layers[0].size() - 1));
	//Assign (latch) input vals to input neurons
	for (unsigned i = 0; i < inputVals.size(); ++i) {
		m_layers[0][i].setOutputVal(inputVals[i]);
	}

	//Forward prop
	for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum) {
		Layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n) {
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

void Net::getResults(vector<double> &resultVals) const
{
	resultVals.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; ++n) {
		resultVals.push_back(m_layers.back()[n].getOutputVal());
	}
}

// ---------------------------------- NEURON ----------------------------
double Neuron::eta = 0.15; // overall net learning rate
double Neuron::alpha = 0.5; // momentum multiplier


void Neuron::updateInputWeights(Layer &prevLayer)
{
	//The weights to be updated are in the connection layer
	//in the neurons in the previous layer
	for (unsigned n = 0; n < prevLayer.size(); ++n) {
		Neuron &neuron = prevLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaweight;

		double newDeltaWeight =
			//Individual input magnified by the gradient and train weight
			eta
			* neuron.getOutputVal()
			* m_gradient
			//Also add in the momentum
			+ alpha
			* oldDeltaWeight;

		neuron.m_outputWeights[m_myIndex].deltaweight = newDeltaWeight;
		neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
	}
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
	double sum = 0.0;

	//Sum our contributions of the errors at the nodes that we feed
	for (unsigned n = 0; n < nextLayer.size() - 1; ++n) {
		sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
	}

	return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
	double dow = sumDOW(nextLayer);
	m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::calcOutputGradients(double targetVal)
{
	double delta = targetVal - m_outputVal;
	m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

double Neuron::transferFunction(double x)
{
	//use tanh (output range = -1.0 to 1.0)
	return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
	//tanh derivative
	return 1 - x*x;
}

void Neuron::feedForward(const Layer &prevLayer)
{
	double sum = 0.0;

	//Sum through the previous layers output (now inputs)
	//Including the bias node 

	for (unsigned n = 0; n < prevLayer.size(); ++n) {
		sum += prevLayer[n].m_outputVal * prevLayer[n].m_outputWeights[m_myIndex].weight;
	}

	m_outputVal = Neuron::transferFunction(sum);
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned c = 0; c < numOutputs; ++c) {
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight = randomWeight();
	}

	m_myIndex = myIndex;
}
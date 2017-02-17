
#include "MyForm.h"
#include <math.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <random>
#include "NeuralNet.h"

using namespace CarAI;
using namespace System::Windows::Forms;
using namespace System::Drawing;

//Define Functions
void rotateCoord(double*, double*, double*, double*, double* angle);
double checkCollisionDistance(double, double, double, double, double, double, double, double, double*, double*, int);
void updateCarPhysics(void);
bool endGameDetection(void);
void calculatePoints(int);
void updateVision(void);
unsigned numChildrenPerGen = 3000;
unsigned numChildrenPerGenFirstGen = 4000;
unsigned numChildToKeepDuringBreed = 100;
bool convRiskHigh = false;
unsigned numGenerations = 500;
unsigned maxCycles = 4000;
bool isSimMode = false;
std::vector<unsigned> topology = { (unsigned)searchParam[0],8,2 };
std::vector<double> carPathDraw;
std::default_random_engine randomEng;
std::normal_distribution<double> dist(5.0, .5);




[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Create the main window and run it
	Application::Run(gcnew MyForm());

	//run loop

	return 0;
}

double randNormDist() {
	double number = dist(randomEng);
	if (number < 0) number = 0;
	if (number > 10) number = 10;

	//map to [-1,1]
	number = (number - 5) / 5;

	return number;

}

double randNormDistWide() {
	double number = dist(randomEng);
	if (number < 2.5) number = 2.5;
	if (number > 7.5) number = 7.5;

	//map to [0,10]
	number = (number - 2.5) * 2;

	//map to [-1,1]
	number = (number - 5) / 5;

	return number;

}

//================================== Evolution Classes and Function ==================
class Child
{
public:
	//DNA
	Child();
	double getPoints() { return m_points; }
	double getDistance() { return m_distance; }
	double getTime(void) { return m_time; }
	void setPoints(double points) { m_points = points; }
	void setDistance(double distance) { m_distance = distance; }
	void setTime(double time) { m_time = time; };
	void getDNA(std::vector<double> &DNA) { DNA = m_DNA; }
	void setDNA(std::vector<double> const DNA) { m_DNA = DNA; }
	double m_points;
	std::vector<double> m_DNA;

private:

	double m_distance;
	double m_time;


};

Child::Child(void) {

}

class Generation
{
public:
	Generation();
	void addChild(Child child);
	void getChild(unsigned index, Child &child);
	unsigned getNumChildren(void) { return m_children.size(); }
	void eraseChild(unsigned index) { m_children.erase(m_children.begin() + index); }
	void breedChildren(double removePointLimit, unsigned maxChildrenDeleted);
	void randomBreedChildren(unsigned remainingChildren, unsigned maxChildren);

private:
	std::vector<Child> m_children;
	void mutateDNA(std::vector<double> &DNA);
	void mutateDNA(Child &child);
	double mutationProbLimit = 1;
	double mutationScale = .5;
	Child breedChild(std::vector<double> &c1DNA, std::vector<double> &c2DNA);
	Child zipBreedChild(std::vector<double> &c1DNA, std::vector<double> &c2DNA);
	Child randomPairBreedChild(std::vector<double> &c1DNA, std::vector<double> &c2DNA);

};

std::vector<Child> bestChildren;

Generation::Generation(void) {

}

void Generation::addChild(Child child) {
	m_children.push_back(child);
}

void Generation::getChild(unsigned index, Child &child) {
	child = m_children[index];
}

void Generation::breedChildren(double removePointLimit, unsigned maxChildrenDeleted) {
	std::srand(std::time(0));
	//randomize vector
	random_shuffle(m_children.begin(), m_children.end());

	//remove bad children
	//cycle through children and remove if points are less than removePointLimit
	unsigned numChildren = m_children.size();
	std::vector<unsigned> indexToDelete;
	for (unsigned i = 0; i < numChildren; i++) {
		if (m_children[i].getPoints() < removePointLimit) {
			indexToDelete.push_back(i);
		}
	}

	//perform deletion
	for (unsigned i = 0; i < indexToDelete.size(); i++) {
		//only delete the limit
		if (i < maxChildrenDeleted) {
			unsigned currentIndex = indexToDelete[i] - i;
			m_children.erase(m_children.begin() + currentIndex);
		}
	}

	//create new children
	numChildren = m_children.size();
	for (unsigned i = 0; i < numChildren; i++) {
		//get DNA for first breeding pairs
		std::vector<double> DNA1, DNA2;

		if (i < numChildren - 1) {
			m_children[i].getDNA(DNA1);
			m_children[i+1].getDNA(DNA2);
		}
		else {
			m_children[i].getDNA(DNA1);
			m_children[0].getDNA(DNA2);
		}

		m_children.push_back(breedChild(DNA1, DNA2));

		//get DNA for second breeding pairs
		if (i < numChildren - 1) {
			m_children[i].getDNA(DNA2);
			m_children[i + 1].getDNA(DNA1);
		}
		else {
			m_children[i].getDNA(DNA2);
			m_children[0].getDNA(DNA1);
		}

		m_children.push_back(breedChild(DNA1, DNA2));
	}
	//remove first children
	while (m_children.size() > numChildrenPerGen) {
		m_children.erase(m_children.begin());
	}

}

void Generation::randomBreedChildren(unsigned remainingChildren, unsigned maxChildren) {
	//sort list
	std::sort(m_children.begin(), m_children.end(), [](const Child& lhs, const Child& rhs) { return lhs.m_points < rhs.m_points; });

	//check for risk of convergence
	unsigned sizeMulti = 1;
	if (convRiskHigh) sizeMulti = 4;

	//add best child to list
	bestChildren.push_back(m_children[m_children.size() - 1]);

	//remove bad children
	while (m_children.size() > sizeMulti*remainingChildren) {
		m_children.erase(m_children.begin());
	}

	//create new children
	unsigned initialSize = m_children.size();
	unsigned indexC1 = -1;
	unsigned indexC2 = -1;
	while (m_children.size() < (maxChildren + sizeMulti*remainingChildren)) {
		//select 2 children

		indexC1++;
		if (indexC1 > initialSize) indexC1 = 0;

		//generate random partner
		if (true) {
			indexC2 = (unsigned)Math::Floor(initialSize*rand() / double(RAND_MAX));
			while (indexC2 == indexC1) indexC2 = (unsigned)Math::Floor(initialSize*rand() / double(RAND_MAX));
		}
		else {
			//asexual breeding with mutation
			indexC2 = indexC1;
		}

		//safety check
		if (indexC1 == initialSize) indexC1 = initialSize - 1;
		if (indexC2 == initialSize) indexC2 = initialSize - 1;

		//get DNA of children
		std::vector<double> DNA1, DNA2;
		m_children[indexC1].getDNA(DNA1);
		m_children[indexC2].getDNA(DNA2);

		m_children.push_back(zipBreedChild(DNA1, DNA2));
	}

	//delete parents
	while (m_children.size() > maxChildren) {
		m_children.erase(m_children.begin());
	}

}

Child Generation::randomPairBreedChild(std::vector<double> &c1DNA, std::vector<double> &c2DNA) {
	//new Child
	Child c3 = Child();
	std::vector<double> c3DNA;

	bool isC1 = true;
	for (int i = 0; i < c1DNA.size(); i++) {
		unsigned randNum = (unsigned)Math::Floor(rand() / double(RAND_MAX));
		if (randNum>0.5) {
			c3DNA.push_back(c1DNA[i]);
			isC1 = false;
		}
		else {
			c3DNA.push_back(c2DNA[i]);
			isC1 = true;
		}
	}

	//mutate
	mutateDNA(c3DNA);
	c3.setDNA(c3DNA);
	return c3;
}

Child Generation::zipBreedChild(std::vector<double> &c1DNA, std::vector<double> &c2DNA) {
	//new Child
	Child c3 = Child();
	std::vector<double> c3DNA;

	bool isC1 = true;
	for (int i = 0; i < c1DNA.size(); i++) {
		if (isC1) {
			c3DNA.push_back(c1DNA[i]);
			isC1 = false;
		}
		else {
			c3DNA.push_back(c2DNA[i]);
			isC1 = true;
		}
	}

	//mutate
	mutateDNA(c3DNA);
	c3.setDNA(c3DNA);
	return c3;
}

Child Generation::breedChild(std::vector<double> &c1DNA, std::vector<double> &c2DNA) {
	//recombine DNA
	Child c3 = Child();
	std::vector<double> c3DNA;

	//split DNA
	unsigned DNALength = c1DNA.size();
	unsigned splitLength = Math::Floor((double)DNALength / 2);

	//add DNA
	for (unsigned x = 0; x < splitLength; x++) {
		c3DNA.push_back(c1DNA[x]);
	}
	for (unsigned x = splitLength; x < DNALength; x++) {
		c3DNA.push_back(c2DNA[x - splitLength]);
	}

	//mutate
	mutateDNA(c3DNA);
	c3.setDNA(c3DNA);
	return c3;
}

void Generation::mutateDNA(std::vector<double> &DNA) {
	//calculate probability of mutation
	//if mutation probability is above limit
	//do this for all DNA pairs
	//mutliply my probabliy scale
	double probScale = 1;
	if (convRiskHigh) probScale = 0.9;

	//reset the random generator if high rish

	for (unsigned i = 0; i < DNA.size(); i++) {
		double probability = rand() / double(RAND_MAX);

		//check mutation rate
		if (probability > convRiskHigh*mutationProbLimit) {
			double mutation;
			if (convRiskHigh) {
				mutation = randNormDistWide();
			}
			else {
				mutation = randNormDist();
			}


			DNA[i] += mutation;

			//check if within bounds
			if (DNA[i] > 1) DNA[i] = 1;
			if (DNA[i] < 0) DNA[i] = 0;
		}
		

	}
	

}

void Generation::mutateDNA(Child &child) {
	//calculate probability of mutation
	//if mutation probability is above limit
	//do this for all DNA pairs
	double mutationMulti = 1;
	double mutationProbMulti = 1;
	if (convRiskHigh) {
		mutationMulti = 2;
		mutationProbMulti = 0.7;
	}


	for (unsigned i = 0; i < child.m_DNA.size(); i++) {
		double probability = rand() / double(RAND_MAX);

		//check mutation rate
		if (probability > mutationProbMulti*mutationProbLimit) {
			double mutation = mutationMulti*mutationScale*((rand() / double(RAND_MAX)) - 0.5 / 0.5);

			child.m_DNA[i] += mutation;

			//check if within bounds
			if (child.m_DNA[i] > 1) child.m_DNA[i] = 1;
			if (child.m_DNA[i] < 0) child.m_DNA[i] = 0;
		}


	}

}

//============================================= Control Functions ==========================
void CarAI::simulateDNA(PictureBox^ pb1) {
	isSimMode = true;

	//create new neural net
	::Net m_Net(topology);
	std::vector<double> inputVals, resultVals, DNA; //resultVals[0] = angle resultsVals[1] = speed

	//Create trail vector
	carPathDraw.clear();
	carPathDraw.push_back(carPos[0]);
	carPathDraw.push_back(carPos[1]);
	double distBetweenPoints = 5;

	//Turn drawing on
	isDrawingOn = true;

	m_Net.setWeights(CarAI::sim4DNA);
	CarAI::resetInitVariables();

	//simulate
	for (unsigned i = 0; i < maxCycles; i++) {
		//calculate control values
		inputVals.clear();
		for (int i = 0; i < searchParam[0]; i++) {
			double val = (searchParam[2] - searchResults[i]) / searchParam[2];
			if (val < 0) val = 1;
			inputVals.push_back(val);
		}
		m_Net.feedForward(inputVals);
		m_Net.getResults(resultVals);

		double linearVelMulti = ((resultVals[1] - 0.5) / 0.5);
		double angularVelMulti = ((resultVals[0] - 0.5) / 0.5);

		CarAI::carInputVel[0] = carPhysicsLimits[1] * linearVelMulti;
		CarAI::carInputVel[1] = carPhysicsLimits[2] * angularVelMulti;



		//update physics
		updateCarPhysics();

		//add point to path
		if (Math::Abs(carPos[0] - carPathDraw[carPathDraw.size() - 2]) > distBetweenPoints) {
			carPathDraw.push_back(carPos[0]);
			carPathDraw.push_back(carPos[1]);
		}


		//display if valid
		if (isDrawingOn) {
			pb1->Refresh();
		}
		else {
			updateVision();
		}

		//check for end
		if (endGameDetection()) {
			break;
		}
	}

	isSimMode = false;

}

void simulateDNAfromChild(PictureBox^ pb1, Child &c) {
	isSimMode = true;

	//create new neural net
	::Net m_Net(topology);
	std::vector<double> inputVals, resultVals, DNA; //resultVals[0] = angle resultsVals[1] = speed

													//Create trail vector
	carPathDraw.clear();
	carPathDraw.push_back(carPos[0]);
	carPathDraw.push_back(carPos[1]);
	double distBetweenPoints = 5;

	//Turn drawing on
	isDrawingOn = true;

	m_Net.setWeights(c.m_DNA);
	CarAI::resetInitVariables();

	//simulate
	for (unsigned i = 0; i < maxCycles; i++) {
		//calculate control values
		inputVals.clear();
		for (int i = 0; i < searchParam[0]; i++) {
			double val = (searchParam[2] - searchResults[i]) / searchParam[2];
			if (val < 0) val = 1;
			inputVals.push_back(val);
		}
		m_Net.feedForward(inputVals);
		m_Net.getResults(resultVals);

		double linearVelMulti = ((resultVals[1] - 0.5) / 0.5);
		double angularVelMulti = ((resultVals[0] - 0.5) / 0.5);

		CarAI::carInputVel[0] = carPhysicsLimits[1] * linearVelMulti;
		CarAI::carInputVel[1] = carPhysicsLimits[2] * angularVelMulti;



		//update physics
		updateCarPhysics();

		//add point to path
		if (Math::Abs(carPos[0] - carPathDraw[carPathDraw.size() - 2]) > distBetweenPoints) {
			carPathDraw.push_back(carPos[0]);
			carPathDraw.push_back(carPos[1]);
		}


		//display if valid
		if (isDrawingOn) {
			pb1->Refresh();
		}
		else {
			updateVision();
		}

		//check for end
		if (endGameDetection()) {
			break;
		}
	}

	isSimMode = false;
}

void animateResults(Child &child, PictureBox^ pb1);

void CarAI::mainLoop(PictureBox^ pb1, TextBox^ genText, TextBox^ childText, TextBox^ lastGenMaxScore) {
	//reset random generator
	std::srand(std::time(0));

	//initialize variables and structures
	std::vector<double> points;
	std::vector<double> maxGenPoints;
	std::vector<double> avgGenPoints;
	Generation gen = ::Generation();
	bool isFirstRun = true;
	unsigned lastTBUpdateInterval = 0;

	//redefine function

	for (int genNum = 0; genNum < numGenerations; genNum++) {
		points.erase(points.begin(), points.end());

		//update gen textbox output
		genText->Text = genNum.ToString();
		genText->Update();
		lastTBUpdateInterval = 0;

		unsigned numChildren= numChildrenPerGen;
		unsigned simCount = 0;

		for (unsigned childNum = 0; childNum < numChildren; childNum++) {

			//update textbox outputs
			if (childNum * 100 / numChildren > lastTBUpdateInterval) {
				lastTBUpdateInterval += 10;
				childText->Text = lastTBUpdateInterval.ToString();
				childText->Update();
			}
	
			//reset
			CarAI::resetInitVariables();
			Child child = ::Child();
			if (!isFirstRun) {
				gen.getChild(childNum, child);
			}

			//create new neural net
			::Net m_Net(topology);
			std::vector<double> inputVals, resultVals, DNA; //resultVals[0] = angle resultsVals[1] = speed
			if (isFirstRun) {
				m_Net.getWeights(DNA);
				child.setDNA(DNA);
			}
			else {
				child.getDNA(DNA);
				m_Net.setWeights(DNA);
			}


			//simulate
			for (unsigned i = 0; i < maxCycles; i++) {
				//calculate control values
				inputVals.clear();
				for (int i = 0; i < searchParam[0]; i++) {
					double val = (searchParam[2]-searchResults[i]) / searchParam[2];
					if (val < 0) val = 1;
					inputVals.push_back(val);
				}
				m_Net.feedForward(inputVals);
				m_Net.getResults(resultVals);

				double linearVelMulti = ((resultVals[1] - 0.5) / 0.5);
				double angularVelMulti = ((resultVals[0] - 0.5) / 0.5);

				CarAI::carInputVel[0] = carPhysicsLimits[1] * linearVelMulti;
				CarAI::carInputVel[1] = carPhysicsLimits[2] * angularVelMulti;

				//update physics
				updateCarPhysics();

				//test
				if (carPos[0] > 700) {
					//isDrawingOn = true;
				}
				else {
					isDrawingOn = false;
				}

				//display if valid
				if (isDrawingOn) {
					pb1->Refresh();
				}
				else {
					updateVision();
				}

				//update count
				simCount = i;

				//check for end
				if (endGameDetection()) {
					break;
				}
			}

			calculatePoints(simCount);

			//set points
			//LETS TRY THE COST FUNCTION OF DISTANCE ONLY
			child.setPoints(simPoints[1]);
			//child.setPoints(simPoints[1] * simPoints[1] / simPoints[0]);
			child.setDistance(simPoints[1]);
			child.setTime(simPoints[0]);
			gen.addChild(child);

			points.push_back(simPoints[1]);
			//points.push_back(simPoints[1] * simPoints[1] / simPoints[0]);


		}
		if (!isFirstRun) {
			for (unsigned q = 0; q < numChildren; q++) {
				gen.eraseChild(0);
			}
		}


		//sort
		unsigned maxDeleteChildren = Math::Floor((double)points.size() / 2);
		std::sort(points.begin(), points.end());

		//update analytics 
		maxGenPoints.push_back(points[points.size() - 1]);
		double avg = 0;
		for (unsigned q = 0; q < points.size(); q++) {
			avg += points[q];
		}
		avgGenPoints.push_back(avg / points.size());
		
		//update textbox display
		lastGenMaxScore->Text = maxGenPoints[maxGenPoints.size() - 1].ToString();
		lastGenMaxScore->Update();

		//analyse convergence risk
		if (maxGenPoints.size() >= 2) {
			if (Math::Abs(maxGenPoints[maxGenPoints.size() - 1] - maxGenPoints[maxGenPoints.size() - 2]) < 1) {
				convRiskHigh = true;
			}
			else {
				convRiskHigh = false;
			}
		}

		//breed
		unsigned pointLimit = points[maxDeleteChildren];
		gen.randomBreedChildren(numChildToKeepDuringBreed, numChildren);
		//gen.breedChildren(pointLimit, maxDeleteChildren);

		

		isFirstRun = false;

	}

	avgGenPoints.push_back(0);
	//display best results
	//show the best children
	for (Child c : bestChildren) {
		simulateDNAfromChild(pb1, c);
	}

	bestChildren.push_back(Child());
}

void CarAI::mainLoopHandler(PictureBox^ pb1) {
	//CarAI::carInputVel[0] = 50;
	//CarAI::carInputVel[1] = -0.018;

	//run iterations
	unsigned numberOfChildren = 20;
	std::vector<double> points;
	std::vector<double> newWeights;
	std::vector<unsigned> topology = { 5,3,2 };

	for (unsigned child = 0; child < numberOfChildren; child++) {
		
		//reset initial conditions
		carPos[0] = 20;
		carPos[1] = 325;
		carPos[2] = 0;
		carPos[3] = 0;
		carInputVel[0] = 0;
		carInputVel[1] = 0;



		//Setup Neural Network
		::Net m_Net(topology);
		if (newWeights.size() > 0) m_Net.setWeights(newWeights);
		std::vector<double> inputVals, resultVals; //resultVals[0] = angle resultsVals[1] = speed

		for (int i = 0; i < 2000; i++) {
			//calculate control values
			inputVals.clear();
			for (int i = 0; i < searchParam[0]; i++) {
				double val = searchResults[i] / searchParam[2];
				if (val < 0) val = 1;
				inputVals.push_back(val);
			}
			m_Net.feedForward(inputVals);
			m_Net.getResults(resultVals);

			double linearVelMulti = ((resultVals[1] - 0.5) / 0.5);
			double angularVelMulti = ((resultVals[0] - 0.5) / 0.5);

			CarAI::carInputVel[0] = carPhysicsLimits[1] * linearVelMulti;
			CarAI::carInputVel[1] = carPhysicsLimits[2] * angularVelMulti;

			//update physics
			updateCarPhysics();

			//display if valid
			if (isDrawingOn) {
				pb1->Refresh();
			}
			else {
				updateVision();
			}

			//check for end
			if (endGameDetection()) {
				calculatePoints(i);
				break;
			}

		}

		points.push_back(simPoints[1] * simPoints[1] / simPoints[0]);
		if((child>4)) m_Net.getWeights(newWeights);
		if (child == numberOfChildren-1) {
			//do nothing
			std::sort(points.begin(), points.end());

			points[0] = 0;

		}
	}
}

void CarAI::resetInitVariables(void) {
	//reset initial conditions
	carPos[0] = 20;
	carPos[1] = 320;
	carPos[2] = 0;
	carPos[3] = 0;
	carInputVel[0] = 0;
	carInputVel[1] = 0;


	for (int i = 0; i < (sizeof(searchResults) / sizeof(searchResults[0])); i++) {
		searchResults[i] = -1;
	}
}

void animateResults(Child &child, PictureBox^ pb1) {
	//create new neural net
	::Net m_Net(topology);
	std::vector<double> inputVals, resultVals, DNA; //resultVals[0] = angle resultsVals[1] = speed
	child.getDNA(DNA);
	m_Net.setWeights(DNA);

	//simulate
	for (unsigned i = 0; i < maxCycles; i++) {
		//calculate control values
		inputVals.clear();
		for (int i = 0; i < 5; i++) {
			double val = searchResults[i] / searchParam[2];
			if (val < 0) val = 1;
			inputVals.push_back(val);
		}
		m_Net.feedForward(inputVals);
		m_Net.getResults(resultVals);

		double linearVelMulti = ((resultVals[1] - 0.5) / 0.5);
		double angularVelMulti = ((resultVals[0] - 0.5) / 0.5);

		CarAI::carInputVel[0] = carPhysicsLimits[1] * linearVelMulti;
		CarAI::carInputVel[1] = carPhysicsLimits[2] * angularVelMulti;

		//update physics
		updateCarPhysics();

		//display
		pb1->Refresh();

		//check for end
		if (endGameDetection()) {
			calculatePoints(i);
			return;
		}
	}

}

//============================================= Draw Functions ==========================
void CarAI::draw(PictureBox^ pb1) {
	pb1->BackColor = Color::Black;
}

void CarAI::drawMap(PictureBox^ pb1, System::Windows::Forms::PaintEventArgs^ e) {
	//Paint background
	pb1->BackColor = Color::Black;
	
	//Get graphics link
	Graphics^ g = e->Graphics;

	//Clear graphcis
	g->Clear(Color::Black);

	//Draw road
	Pen^ redPen = gcnew Pen(Color::Red);
	redPen->Width = 4;
	//get number of lines in road
	unsigned numberOfRoadLines = sizeof(map)/sizeof(map[0]) ;

	for (int i = 0; i < numberOfRoadLines; i++) {
		g->DrawLine(redPen, CarAI::map[i][0], CarAI::map[i][1], CarAI::map[i][2], CarAI::map[i][3]);
	}
	
	//Draw car
	Pen^ bluePen = gcnew Pen(Color::Blue);
	bluePen->Width = 4;

	double carCor[4][2] = { { CarAI::carPos[0] - CarAI::car[0] / 2 ,CarAI::carPos[1] - CarAI::car[1] / 2 },
	{ CarAI::carPos[0] - CarAI::car[0] / 2 ,CarAI::carPos[1] + CarAI::car[1] / 2 },
	{ CarAI::carPos[0] + CarAI::car[0] / 2 ,CarAI::carPos[1] - CarAI::car[1] / 2 },
	{ CarAI::carPos[0] + CarAI::car[0] / 2 ,CarAI::carPos[1] + CarAI::car[1] / 2 } };

	double cx = CarAI::carPos[0];
	double cy = CarAI::carPos[1];

	for (int i = 0; i < 4; i++) {
		rotateCoord(&carCor[i][0], &carCor[i][1], &cx, &cy, &CarAI::carPos[2]);
	}
	
	g->DrawLine(bluePen, (int)carCor[0][0], (int)carCor[0][1], (int)carCor[1][0], (int)carCor[1][1]);
	g->DrawLine(bluePen, (int)carCor[1][0], (int)carCor[1][1], (int)carCor[3][0], (int)carCor[3][1]);
	g->DrawLine(bluePen, (int)carCor[2][0], (int)carCor[2][1], (int)carCor[3][0], (int)carCor[3][1]);
	g->DrawLine(bluePen, (int)carCor[2][0], (int)carCor[2][1], (int)carCor[0][0], (int)carCor[0][1]);


	// Draw Search Lines
	Pen^ whitePen = gcnew Pen(Color::White);
	whitePen->Width = 1;
	Pen^ greenPen = gcnew Pen(Color::Green);
	whitePen->Width = 1;
	Pen^ rectPen = gcnew Pen(Color::White);
	rectPen->Width = 4;

	for (int i = 0; i < searchParam[0]; i++) {
		double sx = cx + searchParam[2];
		double sy = cy;
		double ix = -1;
		double iy = -1;
		double angle = CarAI::carPos[2] - (int)Math::Floor(searchParam[0] / 2)*searchParam[1] + i*searchParam[1];
		rotateCoord(&sx, &sy, &cx, &cy, &angle);

		//check if collision
		bool coll = false;
		for (int x = 0; x < numberOfRoadLines; x++) {
			double collision = checkCollisionDistance( CarAI::map[x][0], CarAI::map[x][1], CarAI::map[x][2], CarAI::map[x][3], cx, cy, sx, sy, &ix, &iy,i);
			if (collision > -1) {
				coll = true;
				break;
			}
		}
		if (coll) {
			g->DrawLine(whitePen, (int)cx, (int)cy, (int)sx, (int)sy);
			g->DrawRectangle(rectPen, (int)ix - 2, (int)iy - 2, 4, 4);
			
		}else {
			g->DrawLine(greenPen, (int)cx, (int)cy, (int)sx, (int)sy);
		}
	}

	//draw trail is in sim mode
	if (isSimMode) {
		Pen^ rect2Pen = gcnew Pen(Color::Orange);
		rect2Pen->Width = 1;
		
		for (int i = 0; i < carPathDraw.size()-1; i++) {
			g->DrawRectangle(rect2Pen, (int)carPathDraw[i], (int)carPathDraw[i+1] ,1,1);

			i++;
		}
	}


}

void updateVision(void) {
	double cx = CarAI::carPos[0];
	double cy = CarAI::carPos[1];

	for (int i = 0; i < searchParam[0]; i++) {
		double sx = cx + searchParam[2];
		double sy = cy;
		double ix = -1;
		double iy = -1;
		double angle = CarAI::carPos[2] - (int)Math::Floor(searchParam[0] / 2)*searchParam[1] + i*searchParam[1];
		rotateCoord(&sx, &sy, &cx, &cy, &angle);

		//check if collision
		bool coll = false;
		for (int x = 0; x < 8; x++) {
			double collision = checkCollisionDistance(CarAI::map[x][0], CarAI::map[x][1], CarAI::map[x][2], CarAI::map[x][3], cx, cy, sx, sy, &ix, &iy, i);
			if (collision > -1) {
				coll = true;
				break;
			}
		}

	}
}

void rotateCoord(double* x, double* y, double* cx, double* cy, double* angle) {
	double ox = *x;
	double oy = *y;

	*x = (ox - *cx)*cos(*angle) - (oy - *cy)*sin(*angle) + *cx;
	*y = (ox - *cx)*sin(*angle) + (oy - *cy)*cos(*angle) + *cy;

}

double checkCollisionDistance(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double* ix, double* iy, int lineNumber) {
	//convert to vector form
	double vx1 = x1;
	double vy1 = y1;
	double vx1dot = x2 - x1;
	double vy1dot = y2 - y1;

	double vx2 = x3;
	double vy2 = y3;
	double vx2dot = x4 - x3;
	double vy2dot = y4 - y3;

	double snum = vx1dot*(vy2 - vy1) - vy1dot*(vx2 - vx1);
	double sden =  (-vx1dot*vy2dot + vx2dot*vy1dot);
	if (sden == 0) {
		CarAI::searchResults[lineNumber] = searchParam[2];
		return -1;
	}
	double s = snum / sden;

	double tnum = vx2dot*(vy1 - vy2) - vy2dot*(vx1 - vx2);
	double tden = (vy2dot*vx1dot - vy1dot*vx2dot);
	if (tden == 0) {
		CarAI::searchResults[lineNumber] = searchParam[2];
		return -1;
	}
	double t = tnum / tden;



	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		CarAI::searchResults[lineNumber] = searchParam[2] * s;
		*ix = vx2 + s*vx2dot;
		*iy = vy2 + s*vy2dot;
		return 1;
	}
	else {
		return -1;
	}

}


//=========================================== Physics Functions ==========================
void updateCarPhysics(void)
{
	double refreshRate = CarAI::simRate;

	double linearVelocity = CarAI::carInputVel[0]/ refreshRate;
	double angularVelocity = CarAI::carInputVel[1]/refreshRate;

	//Check velocities are within max limits
	double linearAccelLimit = CarAI::carPhysicsLimits[0]/refreshRate;
	double linearVelLimit = CarAI::carPhysicsLimits[1]/ refreshRate;
	double angularVelLimit = CarAI::carPhysicsLimits[2] / refreshRate;

	if (linearVelocity > linearVelLimit) linearVelocity = linearVelLimit;
	if (linearVelocity < -linearVelLimit) linearVelocity = -linearVelLimit;

	if (angularVelocity > angularVelLimit) angularVelocity = angularVelLimit;
	if (angularVelocity < -angularVelLimit) angularVelocity = -angularVelLimit;

	//Check linear acceleration is within bounds and update velocity data
	double currentVelocity = CarAI::carPos[3];
	if ((linearVelocity - currentVelocity) > linearAccelLimit) {
		CarAI::carPos[3] += linearAccelLimit;
	}
	else if ((linearVelocity - currentVelocity) < -linearAccelLimit) {
		CarAI::carPos[3] -= linearAccelLimit;
	}
	else {
		CarAI::carPos[3] = linearVelocity;
	}

	//Update angular data;
	CarAI::carPos[2] += angularVelocity;

	//Update positions;
	CarAI::carPos[0] += CarAI::carPos[3] * cos(CarAI::carPos[2]);
	CarAI::carPos[1] += CarAI::carPos[3] * sin(CarAI::carPos[2]);

}

bool endGameDetection(void) {
	//first do a quick search of the searchLines
	double firstCheckDistance = Math::Max(car[0], car[1])/2;
	bool collision = false;

	for (int i = 0; i < CarAI::searchParam[0]; i++) {
		if (CarAI::searchResults[i] > -1) {
			//is valid
			if (CarAI::searchResults[i] < firstCheckDistance) {
				collision = true;
				break;
			}
		}
	}

	//Check if the car is driving backwards
	if (carPos[0] < 0) collision = true;

		//if no possible collision then end check
	return collision;	
}

//=========================================== Game Functions ==============================
void calculatePoints(int gameCycles) {
	//The cost function is a product of distance travelled and time
	//C = distance^2/time
	if (CarAI::carPos[0] > 0) {
		simPoints[1] = CarAI::carPos[0];
	}
	else {
		simPoints[1] = 0;
	}

	simPoints[0] = gameCycles/simRate;
	
}


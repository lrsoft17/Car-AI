#pragma once
#include "NeuralNet.h"

namespace CarAI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	//User defined functions and variables
	void draw(PictureBox^);
	void mainLoopHandler(PictureBox^);
	void mainLoop(PictureBox^, TextBox^, TextBox^, TextBox^);
	void resetInitVariables(void);
	void simulateDNA(PictureBox^);

	std::vector<double> simDNA = { 0.00701078485257368,0.0171867139267672,0.443439930816672,0.0824234984650521,0.246134635796939,0,0.305950904735522,0.299785091747992,0.424525224448552,0.300657988112004,0.0418108218190262,0.616837878541069,0.0696769602311348,0.0260812981659181,0,0,0.367544792775941,0.0641184299135602,0.169793272947191,0.227647439241924,0.677018398840457,0.594751551726623,0.42914866308865,0,0.00129652432826876,0.237993512283673,0.130464767633622,0,0.162876696238823,0.666008586352776,0.111123324793259,0.531387048382774,0.121685745813016,0.109340730332647,0,0,0.0420020744862132,0.104673403625033,0.449623666979421,0.116391644537705,0.378254265833323,0.334402449675006,0.18346725440169,0.327836111562564,0.346583592124354,0,0.355138770383317,0,0.258095706778462,0.286266360999109,0.405643717340572,0.39738074719922,0.110506212061386,0.0660356550672466,0,0.313769560403712,0.211456123549032,0.07324367514631,0.0302056025223363,0.164737998165529,0.484661622273304,0.0693758180337539,0.165292086417078,0,0,0.843303387809276,0.00700947620524328,0.324030442419502,0.0884837649644516,0.790499478235525,0,0.0080552174436578,0,0.194435597175889,0.719018192249808,0.84646812142168,0.0602105610433496,0,0.266763458217521,0.105334954462458,0,0.233644609226254
	}; //7 inputs

	std::vector<double> sim4DNA = { 0.74257310527465,0.327999483888457,0.441183229379282,0.0124430477494746,0.290072534285585,0.162243263764901,0.107904298373502,0.196219846209488,0.46546585019061,0.0276709057711488,0.423179410181241,0,0.258686083211405,0.644606726334421,0.555519950767267,0.451728339938709,0.876011290009024,0.918902771581794,0.704205526763795,1,0.698707251727122,0.90233668699681,0.909606682196535,0.150003094504538,0.0273304434628398,0.40986124913677,0.283765137162139,0.107777930675997,0.12703556896625,0.427809241669584,0.226270903892953,0.329179095520067,0.318398290867304,0.463001317343137,0.244709883504794,0,0.091210848035573,0.137569752211792,0.520550608130806,0.363060780841214,0.281022612363777,0.21233999824677,0,0,0.613803782775528,0.418593402318265,0.363103474816919,0.0123363363021352,0.960696828967804,0.603540307288648,0.414711212439052,0,0.767904868356987,0.483428107758284,0.168146928322043,0.0386009741310514,0.883955403617133,0.508112651521457,0.801184952359222,0,0.290920471907245,0.0209311256222983,0,0.0185450565686769,0,0,0,0.256107768033944,0.158842290152834,0,1,0,0.0291719505451482,0.176657103701943,0.0482421449229129,0,0,0.503928321953131,0.0269404358558144,0.203059589330073,0.000570739140023063,0.324879005353203
	}; //7 inputs

	std::vector<double> sim3DNA = { 0.301786791509135,0.541331379982062,0.137213083189192,0.366258742498171,0.00611674548719971,0.0823937171887777,0,0.345941685651629,0.465309979834897,0.302077140227611,0.190622632412632,0.411465425362318,0,0,0.362712211390828,0.592427686251763,0.526730696493892,0.251789130876834,0.246481887065359,0.284493076264765,0.022437244575865,0.59926268336296,0.202977007007107,0.195394898638315,0.108515148092246,0.571831578780817,0,0.198988998269518,0,0.250118600348758,0.393726834012488,0.0426652395683007,0.206062393766875,0.234823350955461,0.0753640317020568,0.0415518998495119,0,0.0211421999773213,0,0,0.473869083792652,0.220545200290707,0.325098392810826,0.182182674032063,0.190859666181935,0.161371533481526,0.188416390209637,0.181479145835281,0,0,0.598725499016423,0.280335208586553,0.421568792105082,0.0593802729334686,0,0.264285507986644,0.437686135347625,0.0929215446790911,0.281731727983407,0.257480485883705,0,0.364919037379569,0.0400191657271492,0.465300925690007,0.0807248930926386,0.170447714505146,0.0477032581589067,0.105497244950761,0.0658478026536493,0.168844917646467,0.144455262635646,0.300453497452446,0.0821446506156967,0.148494323377382,0.146332953880667,0.663619222510062,0.299597289448813,0.0104850335403455,0.275391451620029,0,0.0519918868818942,0.104802270334817,0.35857501361342,0.0631628617659937,0.320240958152231,0.309259204925861,0.0217378033132401,0.200859038167994,0.147185666861423,0.00163825880042001,0.0190456817635513,0.0854613965604357,0.387542932609331,0.663127048019697,0,0.283277615638168,0.223291178537101,0,0.0178159108360695,0.698854733346645,0.134559501448016,0.33103758582118,0,0.38008896705589,0,0.497568766821459,0.117424024065967,0.0187034596856623,0,0.05887563788638,0,0.0817174640511597,0.0814109942202554,0.168233608219954
	}; //11 inputs

	std::vector<double> sim2DNA = { 0.273463306676774,0,0.0137158685060999,0.451408735836856,0.018096917633289,0.0812539514781539,0.220770528415783,0.698157580777296,0.114198320276925,0,0.601184457858538,0.246867259978657,0.326546653296259,0.141372548740792,0.163629945044599,0.549830444994352,0.855590098972508,0,0.549213315718521,0.380091460356888,0.00563827830631542,0.155276452952776,0,0.476031466195669,0.777893603331462,0.0273192069137377,0.882587416411776,0.577579084321904,0.748725265293019,0.44912326848831,0.360456201160636,0.853281720809869,0.533626042491282,0.92996185232846,0,0.668889797028311,0,0,0.483078597895714,0.121062124680238,0.0295130741076792,1,0.0809061844738691,0.383968309940539,0.0989904160700778,0,0.150793070687701,0.258286420324558,0,0.19295137314869,0.970353547370102,0.0528987709447655,0.0817679957554078,0.200599461664223,0.643727864100082,0.341528200299787,0.326849857048694,0.746341066055027,0,0.231229238878705,0.0292398753350431,0.0103832330976418,0.193938676655302,0,0,0.206005773046346,0,0.0159370715549846,0,0.291448786288715,0.103055207199988,0.130176477519252,0.0345860084202891,0.200344985774977,0.443366129719485,0.461385744844346,0.0357547901104712,0.863360264995056,0.0410449874286209,0.368478093197899,0.0522468680306868,0.976258472793907
	}; //7 inputs

	void drawMap(PictureBox^, System::Windows::Forms::PaintEventArgs^);
	
	int map[10][4] = { { 0,275,250,275 },
	{ 250,275,500,100 },
	{ 500,100,750,275 },
	{ 750,275,750,450 },
	{750,450,1000,450},
	{ 0,375,250,375 },
	{ 250,375,500,200 },
	{ 500,200,650,320 },
	{ 650,320,650,550 },
	{650,550,1000,550} };

	int map1[4][4] = { { 0,275,250,275 },
	{ 250,275,1000,300 },
	{ 0,375,250,375 },
	{ 250,375,1000,350 } };

	int map4[6][4] = { { 0,300,250,300 },
		{ 250,300,500,100 },
		{ 500,100,1000,130},
		{ 0,350,250,350 },
		{ 250,350,500,200 },
		{500,200,1000,170} };


	int map2[8][4] = {{ 0,275,250,275 },
		{250,275,500,100 },
		{ 500,100,750,275 },
		{ 750,275,1000,100},
		{ 0,375,250,375 },
		{ 250,375,500,200 },
		{ 500,200,750,375 },
		{ 750,375,1000,200 } };

	//Car Simulation Data Structures
	double simRate = 40; //refresh hertz
	int car[2] = { 30,16 }; //width, height
	double carPos[4] = { 20,325,0,0}; //center x, center y, angle, velocity
	double carInputVel[2] = { 0,0 }; //desired linear velocity, angular velocity
	double carPhysicsLimits[3] = { 0.1,30,0.5}; //max linear acceleration, max linear veloctiy, max angular velocity
	double searchParam[3] = { 7,.5,150 }; //number of lines, angle between (radians), length
	double searchResults[7] = { -1,-1,-1,-1,-1,-1,-1 }; //results of line search
	bool isDrawingOn = false;

	//Genetic Evolution Data Structures;
	double simPoints[2] = { 0,0 }; //time (Cycles / simRate), Distance




	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::PictureBox^  pb1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  genText;
	private: System::Windows::Forms::TextBox^  childText;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  lastGenMaxScore;
	private: System::Windows::Forms::Button^  DNATest;
	public:


	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pb1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->genText = (gcnew System::Windows::Forms::TextBox());
			this->childText = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lastGenMaxScore = (gcnew System::Windows::Forms::TextBox());
			this->DNATest = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pb1))->BeginInit();
			this->SuspendLayout();
			// 
			// pb1
			// 
			this->pb1->Location = System::Drawing::Point(12, 12);
			this->pb1->Name = L"pb1";
			this->pb1->Size = System::Drawing::Size(1060, 560);
			this->pb1->TabIndex = 0;
			this->pb1->TabStop = false;
			this->pb1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::onPaint);
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(12, 578);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(433, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Start Simulation (disabled)";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// genText
			// 
			this->genText->Location = System::Drawing::Point(117, 611);
			this->genText->Name = L"genText";
			this->genText->ReadOnly = true;
			this->genText->Size = System::Drawing::Size(100, 20);
			this->genText->TabIndex = 2;
			// 
			// childText
			// 
			this->childText->Location = System::Drawing::Point(117, 637);
			this->childText->Name = L"childText";
			this->childText->ReadOnly = true;
			this->childText->Size = System::Drawing::Size(100, 20);
			this->childText->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 614);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(99, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Generation Number";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 640);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(87, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Child Number (%)";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(235, 614);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(104, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Last Gen Max Score";
			// 
			// lastGenMaxScore
			// 
			this->lastGenMaxScore->Location = System::Drawing::Point(345, 611);
			this->lastGenMaxScore->Name = L"lastGenMaxScore";
			this->lastGenMaxScore->ReadOnly = true;
			this->lastGenMaxScore->Size = System::Drawing::Size(100, 20);
			this->lastGenMaxScore->TabIndex = 7;
			// 
			// DNATest
			// 
			this->DNATest->Location = System::Drawing::Point(585, 578);
			this->DNATest->Name = L"DNATest";
			this->DNATest->Size = System::Drawing::Size(433, 23);
			this->DNATest->TabIndex = 8;
			this->DNATest->Text = L"Test Programmed DNA";
			this->DNATest->UseVisualStyleBackColor = true;
			this->DNATest->Click += gcnew System::EventHandler(this, &MyForm::DNATest_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1084, 661);
			this->Controls->Add(this->DNATest);
			this->Controls->Add(this->lastGenMaxScore);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->childText);
			this->Controls->Add(this->genText);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pb1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pb1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		mainLoop(pb1, genText, childText, lastGenMaxScore);
	} //end btn_Click

	private: System::Void onPaint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		drawMap(pb1, e);
	}//end onPaint
	private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void DNATest_Click(System::Object^  sender, System::EventArgs^  e) {
		simulateDNA(pb1);
	}
};
}

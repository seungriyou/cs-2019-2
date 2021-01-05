#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <string>
#include <map>
#include <utility>
#include <Windows.h>

#define INPUT_SIZE 2	// input의 개수 = 2
#define LEARNING_RATE 0.2f	// 학습률
#define TOLERANCE 0.05f // 모든 (|t - o| <= TOLERANCE)일 때 학습 종료

using namespace std;

// AND, OR, XOR gate 학습 데이터
vector<vector<float>> train_gate_x = {
	{0,0},{0,1},{1,0},{1,1}
};
vector<float> train_gate_AND_y = { 0,0,0,1 };
vector<float> train_gate_OR_y = { 0,1,1,1 };
vector<float> train_gate_XOR_y = { 0,1,1,0 };

// 도우넛 모양 데이터
vector<vector<float>> train_DOUGHNUT_x = {
	{0,0}, {0,1}, {1,0}, {1,1}, {0.5,1}, {1, 0.5}, {0, 0.5}, {0.5, 0}, {0.5, 0.5} 
};
vector<float> train_DOUGHNUT_y = { 0,0,0,0,0,0,0,0,1 };

// Sigmoid 함수
float Sigmoid(float x)
{
	return 1 / (1 + exp(-x));
}

// Sigmoid 함수의 도함수
float Sigmoid_D(float x)
{
	float y = Sigmoid(x);
	return y * (1 - y);
}

class Neuron
{
private:
	vector<float> input_this; // 해당 Neuron의 input
	vector<float> weight_this; // 해당 Neuron의 weight
	vector<float> weight_next; // Back Propagation 과정에서 필요한 변형된 next layer의 weight
	float bias;
	float net;
	float delta_this; // 해당 Neuron의 delta

	// Initial Value(초기값) 설정
	void setValue()
	{
		bias = -1;	// bias의 초기값은 -1로 설정
		
		random_device rd;
		mt19937 random(rd());	// 난수 생성
		uniform_real_distribution<float> distr(-1.0, 1.0);	// 생성되는 난수는 -1.0 이상 1.0 미만의 실수

		for (size_t i = 0; i < weight_this.size(); i++)
		{
			weight_this[i] = distr(random);	// weight vector에 난수 저장
		}
	}

public:
	// 생성자
	Neuron(size_t input_size)
	{
		weight_this.resize(input_size); // weight.size() == INPUT_SIZE
		input_this.resize(input_size);
		setValue();
	}

	// Sigmoid 함수의 연산 결과인 output을 반환하는 함수
	float Forward(const vector<float> & input)	
	{
		float wx = 0.0;

		for (size_t i = 0; i < weight_this.size(); i++)
		{
			wx += weight_this[i] * input[i];	// wx의 합
		}

		input_this = input;
		net = bias + wx;
		return Sigmoid(net); // = output
	}

	// Back Propagation 과정에서의 연산을 나타낸 함수
	void Backward(float c, float error, const vector<float> & input_this) 
	{
		delta_this = Sigmoid_D(net) * error; // delta의 연산

		for (size_t i = 0; i < input_this.size(); i++)
		{
			weight_this[i] += c * delta_this * input_this[i]; // weight 값 조정
		}
		bias += c * delta_this * 1; // bias 값 조정
	}

	// vector w를 인수로 받아 weight_next에 복사하는 함수
	void setWeight_next(vector<float> w)
	{
		weight_next.clear();
		for (size_t i = 0; i < w.size(); i++)
		{
			weight_next.push_back(w[i]);
		}
	}

	vector<float> & getWeight_next()
	{
		return weight_next;
	}

	int getInputSize()
	{
		return weight_this.size();
	}

	float getNet()
	{
		return net;
	}

	vector<float> & getInput()
	{
		return input_this;
	}

	float getDelta()
	{
		return delta_this;
	}

	vector<float> & getWeight_this()
	{
		return weight_this;
	}

	float & getBias()
	{
		return bias;
	}
};

class Network
{
private:
	vector<vector<Neuron>> layers; // 모든 layer를 모은 vector
	vector<float> output_this; // 한 layer의 output을 모은 vector
	vector<float> delta_next; // 한 layer의 다음 layer의 delta를 모은 vector
	vector<size_t> setting_all; // 생성자의 인수로 받은 vector
	size_t layer_num; // layer 개수
	float error_o, error_h; // error_o = (train_y) - (Forward 후 output)

public:
	// 생성자, 인수로 주어진 setting vector에 따라 신경망을 구성함
	Network(const vector<size_t> & setting) 
	{
		layer_num = setting.size() - 1; 
		for (size_t i = 0; i < setting.size(); i++)
		{
			setting_all.push_back(setting[i]);
		}

		for (size_t i = 1; i <= layer_num; i++)
		{
			vector<Neuron> layer;
			for (size_t j = 0; j < setting_all[i]; j++)
			{
				layer.push_back(Neuron(setting_all[i - 1]));
			}
			layers.push_back(layer);
		}
	}

	// input layer ~ output layer 까지 한 번 연산하는 함수
	vector<float> Forward(const vector<float> & input_this)
	{
		vector<float> input_tmp;
		for (size_t i = 0; i < input_this.size(); i++)
		{
			input_tmp.push_back(input_this[i]);
		}

		for (size_t i = 1; i <= layer_num; i++) /// layer 개수만큼 반복
		{
			output_this.clear();
			for (size_t j = 0; j < setting_all[i]; j++) /// setting_all[i] = this layer의 neuron 개수
			{
				output_this.push_back(layers[i - 1][j].Forward(input_tmp)); /// layers[i - 1] = this layer
			}

			input_tmp.clear();
			for (size_t j = 0; j < output_this.size(); j++)
			{
				input_tmp.push_back(output_this[j]);
			}
		}

		// weight_next를 설정함
		for (size_t i = layer_num - 1; i >= 1; i--) /// 2번째 layer부터 (i=2)
		{
			// next layer의 각 neuron 마다의 weight를 weight_next에 임시 저장
			vector<vector<float>> weight_next;
			weight_next.clear();
			for (size_t j = 0; j < setting_all[i + 1]; j++) /// setting_all[i + 1] = next layer의 neuron 개수
			{
				weight_next.push_back(layers[i][j].getWeight_this()); /// layers[i] = next layer
			}

			// weight_next를 transpose하여 this layer의 각 neuron의 weight_next로 설정
			for (size_t k = 0; k < setting_all[i]; k++)	/// setting_all[i] = this layer의 neuron 개수
			{
				vector<float> weight_next_trans;
				for (size_t j = 0; j < setting_all[i + 1]; j++) /// setting_all[i + 1] = next layer의 neuron 개수
				{
					weight_next_trans.push_back(weight_next[j][k]);
				}
				layers[i - 1][k].setWeight_next(weight_next_trans); /// layers[i - 1] = this layer
			}
		}

		return output_this;
	}

	// 전체 input 데이터에 대해서 한 번 Back Propagation 연산하는 함수
	void Backward(float c, const vector<vector<float>> & train_x, const vector<float> & train_y)
	{
		// 전체 input 데이터에 대해 진행
		for (size_t n = 0; n < train_x.size(); n++)
		{
			// output layer의 연산
			float o = Forward(train_x[n])[0]; // Forward 연산
			error_o = train_y[n] - o; // output layer의 error
			layers[layer_num - 1][0].Backward(c, error_o, layers[layer_num - 1][0].getInput()); 
			/// layers[layer_num - 1] = this layer = output layer

			// hidden layer & input layer의 연산
			for (size_t i = layer_num - 1; i >= 1; i--) /// layers[layer_num - 1] = this layer = output layer
			{
				delta_next.clear();
				for (size_t j = 0; j < layers[i].size(); j++) /// layers[i] = next layer
				{
					delta_next.push_back(layers[i][j].getDelta()); /// layers[i] = next layer
				}

				for (size_t j = 0; j < layers[i - 1].size(); j++) /// layers[i - 1] = this layer
				{
					error_h = 0.0f; // hidden layer & input layer의 error
					for (size_t k = 0; k < layers[i].size(); k++) /// layers[i] = next layer
					{
						error_h += delta_next[k] * layers[i - 1][j].getWeight_next()[k]; /// layers[i - 1] = this layer
					}
					layers[i - 1][j].Backward(c, error_h, layers[i - 1][j].getInput()); /// layers[i - 1] = this layer
				}
			}
		}
	}

	vector<float> & getOutput_this()
	{
		return output_this;
	}

	size_t getLayer_num()
	{
		return layer_num;
	}

	vector <size_t> getSetting_all()
	{
		return setting_all;
	}

	vector<vector<Neuron>> getLayers()
	{
		return layers;
	}
};

// Console 창에 결과를 출력하는 함수
// : Epoch 수, input & output 표, Total Error 값, 각 layer의 neuron 별 weight & bias 표
void printResult(Network net, vector<vector<float>> & train_x, vector<float> & train_y, int epoch, float error_total)
{
	size_t input_size = train_x[0].size();
	size_t train_size = train_x.size();
	size_t layer_num = net.getLayer_num();
	vector<size_t> setting_all = net.getSetting_all();
	vector<vector<float>> input_output_layer; // output layer의 input을 저장함

	cout << " [Epoch " << epoch << "]" << "\n\n"; // 현재 Epoch 수 출력

	// input & output 표
	cout << " ";
	for (size_t i = 0; i < input_size; i++)
	{
		cout << "  X" << i + 1 << " |";
	}
	cout << "   OUTPUT   " << endl;
	cout << " ";
	for (size_t i = 0; i < input_size; i++)
	{
		cout << "-----|";
	}
	cout << "------------" << endl;
	for (size_t i = 0; i < train_size; i++)
	{
		cout << " ";
		for (size_t j = 0; j < input_size; j++)
		{
			cout.width(4);
			cout << train_x[i][j] << " |";
		}
		cout << " ";
		cout.width(9);
		cout << net.Forward(train_x[i])[0] << endl;
		input_output_layer.push_back((net.getLayers()[layer_num - 1][0]).getInput()); // output layer의 input을 저장함
	}

	// Total Error 값 출력
	cout << "\n => Total Error = " << error_total << "\n\n";

	// 각 layer의 neuron 별 weight & bias 표
	for (size_t i = 0; i < layer_num; i++)
	{
		cout << "   (Layer #" << i + 1 << ")" << endl;
		cout << "               ";
		for (size_t j = 0; j < setting_all[i]; j++)
		{
			cout << "|     W" << j + 1 << "     ";
		}
		cout << "|    Bias    ";
		cout << "\n    -----------";
		for (size_t j = 0; j < setting_all[i] + 1; j++)
		{
			cout << "|------------";
		}
		cout << endl;

		for (size_t j = 0; j < setting_all[i + 1]; j++)
		{
			cout << "     Neuron #" << j + 1 << " |";
			for (size_t k = 0; k < setting_all[i]; k++)
			{
				cout.width(11);
				cout << net.getLayers()[i][j].getWeight_this()[k] << " |";
			}
			cout.width(11);
			cout << net.getLayers()[i][j].getBias() << endl;
		}
		
		// output layer의 경우, train_x에 대한 각각의 변형된 input 조합 또한 출력 (그래프 그리기 위함)
		if (i == layer_num - 1)
		{
			cout << "\n               |";
			for (size_t j = 0; j < setting_all[i]; j++)
			{
				cout<< "     X" << j+1 << "     |";
			}
			cout << "\n               |";
			for (size_t j = 0; j < setting_all[i]; j++)
			{
				cout << "------------|";
			}
			cout << "\n";
			for (size_t j = 0; j < train_size; j++)
			{
				cout << "               |";
				for (size_t k = 0; k < setting_all[i]; k++)
				{
					cout.width(11);
					cout << input_output_layer[j][k] << " |";
				}
				cout << "\n";
			}
		}
		cout << endl;
	}
	cout << "----------------------------------------------------------------" << endl;
}

// 모든 (|t - o| <= TOLERANCE)일 때까지 학습을 반복하고, 지정된 Epoch마다 Console 창에 결과를 출력하는 함수
void trainResult(Network net, vector<vector<float>> & train_x, vector<float> & train_y, string dataName)
{
	size_t input_size = train_x[0].size(); // == INPUT_SIZE
	size_t train_size = train_x.size(); // == train_y.size()
	int epoch = 0;
	vector<float> error; // |t - o| 값을 저장하는 vector
	float error_total;
	bool STOP;

	size_t layer_num = net.getLayer_num(); // layer 개수
	vector<size_t> setting_all = net.getSetting_all(); // Network 생성자의 인수로 입력 받은 setting vector

	cout << "\n----------------------------------------------------------------\n";
	cout << "=========================== " << dataName << " ===========================\n";
	cout << "----------------------------------------------------------------\n";

	do 
	{
		STOP = true;
		error_total = 0.0f;
		error.clear();
		net.Backward(LEARNING_RATE, train_x, train_y); // 학습 1회 진행
		
		for (size_t i = 0; i < train_size; i++)
		{
			error.push_back(abs(train_y[i] - net.Forward(train_x[i])[0])); // |t - o| 값을 error vector에 저장
			STOP = STOP && (error[i] <= TOLERANCE); 
			// 모든 error 값(|train_y[i] - net.Forward(train_x[i])[0]|)이 TOLERANCE 보다 작거나 같아야 학습 종료
			error_total += 0.5f * pow(error[i], 2); // MSE 값의 합, MSE = 0.5 * error[i] ^ 2
		}

		// Console 창 출력
		if (epoch <= 1000)
		{
			if (epoch == 0 || epoch == 5 || epoch == 10 || epoch == 20 || epoch == 35 || epoch == 50 ||
				epoch == 100 || (epoch % 200) == 0)
			{
				printResult(net, train_x, train_y, epoch, error_total);
				cout << endl;
			}
		}
		else if ((epoch < 10000) && (epoch % 2000) == 0)
		{
			printResult(net, train_x, train_y, epoch, error_total);
			cout << endl;
		}
		else if ((epoch % 10000) == 0)
		{
			printResult(net, train_x, train_y, epoch, error_total);
			cout << endl;
		}

		epoch++; // epoch 1 증가
	} while (!STOP);

	// 학습 완료 후 Console 창 출력
	printResult(net, train_x, train_y, epoch, error_total);
	cout << "*** 학습이 완료되었습니다. ***\n\n";
}

int main(void)
{
	size_t data_num; // 선택된 데이터의 번호
	size_t layer_num; // 설정된 layer의 개수
	vector<size_t> setting_user; // 설정된 setting vector

	// 사용자로부터 학습을 희망하는 데이터의 종류, layer의 개수, 각 layer의 neuron 개수를 입력 받음
	cout << "\n > 학습을 희망하는 학습 데이터의 종류를 선택하세요. (input의 크기는 " << INPUT_SIZE << "입니다.)" << endl;
	cout << "   (1) AND gate\n" << "   (2) OR gate\n" << "   (3) XOR gate\n" << "   (4) Doughnut\n" << "   => ";
	cin >> data_num;
	cout << "\n > Layer의 개수를 설정하세요.\n   => ";
	cin >> layer_num;
	setting_user.push_back(INPUT_SIZE);
	cout << "\n ********* 권장하는 각 Layer 별 Neuron 개수 *********" << endl;
	cout << "  - Layer 3개인 경우: {3, 4, 1} 또는 {4, 2, 1}" << endl;
	cout << "  - Layer 4개인 경우: {3, 4, 2, 1}" << endl;
	cout << " ****************************************************" << endl;
	for (size_t i = 1; i < layer_num; i++)
	{
		size_t tmp;
		cout << "\n > " << i << " 번째 Layer의 Neuron 개수를 설정하세요.\n   => ";
		cin >> tmp;
		setting_user.push_back(tmp);
	}
	cout << "\n > " << layer_num << " 번째 Layer(Output Layer)의 Neuron 개수는 1로 설정합니다.\n\n"; // output이 1개여야 하기 때문임
	setting_user.push_back(1);
	Sleep(2000); // delay 부여
	
	// 사용자가 선택한 학습 데이터의 종류에 따른 학습 결과 출력
	if (data_num == 1)
	{
		Network net_AND(setting_user);
		trainResult(net_AND, train_gate_x, train_gate_AND_y, "AND gate");
	}
	else if (data_num == 2)
	{
		Network net_OR(setting_user);
		trainResult(net_OR, train_gate_x, train_gate_OR_y, "OR gate ");
	}
	else if (data_num == 3)
	{
		Network net_XOR(setting_user);
		trainResult(net_XOR, train_gate_x, train_gate_XOR_y, "XOR gate");
	}
	else if (data_num == 4)
	{
		Network net_DOUGHNUT(setting_user);
		trainResult(net_DOUGHNUT, train_DOUGHNUT_x, train_DOUGHNUT_y, "DOUGHNUT");
	}
	
	return 0;
}
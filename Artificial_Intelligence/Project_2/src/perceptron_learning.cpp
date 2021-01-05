#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <string>
#include <map>

#define INPUT_SIZE 2		// input의 개수 = 2
#define EPOCH_MAX 100000	// 학습 횟수
#define LEARNING_RATE 0.1	// 학습률

using namespace std;

// 학습 데이터 : { {input 조합}, target } 형식
// AND gate 학습 데이터 설정
vector<pair<vector<double>, double>> target_AND = {
	{ { 0, 0 }, 0 },
{ { 0, 1 }, 0 },
{ { 1, 0 }, 0 },
{ { 1, 1 }, 1 }
};
// OR gate 학습 데이터 설정
vector<pair<vector<double>, double>> target_OR = {
	{ { 0, 0 }, 0 },
{ { 0, 1 }, 1 },
{ { 1, 0 }, 1 },
{ { 1, 1 }, 1 }
};
// XOR gate 학습 데이터 설정
vector<pair<vector<double>, double>> target_XOR = {
	{ { 0, 0 }, 0 },
{ { 0, 1 }, 1 },
{ { 1, 0 }, 1 },
{ { 1, 1 }, 0 }
};

// Sigmoid 함수
double Sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

class Neuron
{
private:
	vector<double> weight;
	double bias;

	// Initial Value(초기값) 설정
	void setValue()
	{
		bias = -1;	// bias의 초기값은 편의상 -1로 고정
		
		random_device rd;
		mt19937 random(rd());	// 난수 생성
		uniform_real_distribution<double> distr(-1.0, 1.0);	// 생성되는 난수는 -1.0 이상 1.0 미만의 실수

		for (size_t i = 0; i < weight.size(); i++)
		{
			weight[i] = distr(random);	// weight vector에 난수 저장
		}
		
		 //weight[0] = -0.577339;	// 보고서 작성 시 임시로 설정한 값
		 //weight[1] = 0.744271;	// 보고서 작성 시 임시로 설정한 값
	}

public:
	// 생성자
	Neuron(int input_size)
	{
		weight.resize(input_size); // weight.size() == INPUT_SIZE
		setValue();
	}

	// Sigmoid 함수의 연산 결과인 output을 반환하는 함수
	double Calculate(const vector<double> & input)
	{
		double output, wx = 0.0;

		for (size_t i = 0; i < weight.size(); i++)
		{
			wx += weight[i] * input[i];	// wx의 합
		}
		
		output = Sigmoid(bias + wx);
		return output;
	}

	// output과 target을 비교하여 학습시키는 함수
	// c는 학습률(LEARNING_RATE), target은 학습 데이터에서 input 조합으로 설정한 값의 vector
	void Learn(double c, const vector<pair<vector<double>, double>> & target)
	{
		int input_size = target[0].first.size();

		for (size_t i = 0; i < target.size(); i++)
		{
			double o = Calculate(target[i].first); // 실제 계산된 output
			double t = target[i].second; // 학습 데이터에 설정된 target

			for (int j = 0; j < input_size; j++)
			{
				weight[j] += c * (t - o) * target[i].first[j];	// weight 값의 조정
			}

			bias += c * (t - o) * 1;	// bias 값의 조정
		}
	}

	// private 멤버인 weight의 getter
	vector<double> & getWeight()
	{
		return weight;
	}

	// private 멤버인 bias의 getter
	double getBias()
	{
		return bias;
	}
};

// Neuron을 학습시키고 그 결과를 출력하는 함수
void showResult(Neuron neuron, const vector<pair<vector<double>, double>> & target, string gateName)
{
	int i, j, k;
	double gradient, intercept; // 기울기, y 절편
	double error, total_error = 0.0; // error = 0.5 * pow((t - o), 2.0), total_error = error의 합

	cout << "\n----------------------------------------------------------------\n";
	cout << "=========================== " << gateName << " gate ===========================\n";
	cout << "----------------------------------------------------------------\n";

	// Initial Value 출력
	cout << "[Initial Value] ";
	for (i = 0; i < INPUT_SIZE; i++)
	{
		cout << "W" << i + 1 << " = " << neuron.getWeight()[i] << ", ";
	}
	cout << "bias = " << neuron.getBias() << '\n';

	// (INPUT_SIZE == 2)일 때, 직선의 기울기 및 y 절편 출력
	gradient = -(neuron.getWeight()[0] / neuron.getWeight()[1]);
	intercept = -(neuron.getBias() / neuron.getWeight()[1]);
	cout << "  => gradient = " << gradient << ", intercept = " << intercept << '\n';

	// 결과 출력
	for (j = 0; j < pow(2.0, INPUT_SIZE); j++)
	{
		for (k = 0; k < INPUT_SIZE - 1; k++)
		{
			cout << target[j].first[k] << " " << gateName << " ";
		}
		cout << target[j].first[k] << " => " << neuron.Calculate(target[j].first);
		error = 0.5 * pow((target[j].second - neuron.Calculate(target[j].first)), 2.0);
		total_error += error;
		cout << "        // error = " << error << '\n';
	}
	cout << "\t\t\t // total error = " << total_error << '\n';
	cout << "----------------------------------------------------------------\n";

	// 학습
	for (i = 1; i <= EPOCH_MAX; i++)
	{
		error = 0.0;
		total_error = 0.0;
		neuron.Learn(LEARNING_RATE, target);

		if (i == 5 || i == 10 || i == 20 || i == 35 || i == 50 || i == 100 || i == 500 || i == 1000 || i == 5000 || i == 10000 || (i % 50000) == 0)
		{
			cout << "[Epoch " << i << "] ";
			for (j = 0; j < INPUT_SIZE; j++)
			{
				cout << "W" << j + 1 << " = " << neuron.getWeight()[j] << ", ";
			}
			cout << "bias = " << neuron.getBias() << '\n';

			// (INPUT_SIZE == 2)일 때, 직선의 기울기 및 y 절편 출력
			gradient = -(neuron.getWeight()[0] / neuron.getWeight()[1]);
			intercept = -(neuron.getBias() / neuron.getWeight()[1]);
			cout << "  => gradient = " << gradient << ", intercept = " << intercept << '\n';

			// 결과 출력
			for (j = 0; j < pow(2.0, INPUT_SIZE); j++)
			{
				for (k = 0; k < INPUT_SIZE - 1; k++)
				{
					cout << target[j].first[k] << " " << gateName << " ";
				}
				cout << target[j].first[k] << " => " << neuron.Calculate(target[j].first);
				error = 0.5*pow((target[j].second - neuron.Calculate(target[j].first)), 2.0);
				cout << "        // error = " << error << '\n';
				total_error += error;
			}
			cout << "\t\t\t // total error = " << total_error << '\n';
			cout << "----------------------------------------------------------------\n";
		}
	}
}

int main(void)
{
	//AND gate
	Neuron neuron_AND(INPUT_SIZE);
	showResult(neuron_AND, target_AND, "AND");

	//OR gate
	Neuron neuron_OR(INPUT_SIZE);
	showResult(neuron_OR, target_OR, "OR");

	//XOR gate
	Neuron neuron_XOR(INPUT_SIZE);
	showResult(neuron_XOR, target_XOR, "XOR");

	return 0;
}
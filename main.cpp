#include <iostream>
#include <cstring>
#include <string>
#include <stack>
using namespace std;


string add(const string& num1, const string& num2);
string subtract(const string& num1, const string& num2);
string multiply(const string& num1, const string& num2);
string divide(const string& num1, const string& num2);
string mod(const string& num1, const string& num2);
string power(const string& base, const string& exp_str);
string evaluateExpression(const string& expression);
int precedence(char op);
string applyOp(const string& a, const string& b, char op);
bool isNumber(const string& token);


string add(const string& num1, const string& num2) {
	int i = num1.size() - 1;
	int j = num2.size() - 1;
	int carry = 0;
	string result;
	
	while (i >= 0 || j >= 0 || carry) {
		int digit1 = i >= 0 ? num1[i--] - '0' : 0;
		int digit2 = j >= 0 ? num2[j--] - '0' : 0;
		int sum = digit1 + digit2 + carry;
		carry = sum / 10;
		result = string(1, '0' + (sum % 10)) + result;
	}
	
	return result;
}


string subtract(const string& num1, const string& num2) {
	int i = num1.size() - 1;
	int j = num2.size() - 1;
	int borrow = 0;
	string result;
	
	while (i >= 0) {
		int digit1 = num1[i--] - '0' - borrow;
		int digit2 = j >= 0 ? num2[j--] - '0' : 0;
		
		if (digit1 < digit2) {
			digit1 += 10;
			borrow = 1;
		} else {
			borrow = 0;
		}
		
		result = string(1, '0' + (digit1 - digit2)) + result;
	}
	
	
	size_t pos = result.find_first_not_of('0');
	if (pos != string::npos) {
		return result.substr(pos);
	}
	return "0";
}


string multiply(const string& num1, const string& num2) {
	if (num1 == "0" || num2 == "0") return "0";
	
	int n1 = num1.size();
	int n2 = num2.size();
	int* result = new int[n1 + n2]();
	
	for (int i = n1 - 1; i >= 0; i--) {
		for (int j = n2 - 1; j >= 0; j--) {
			int mul = (num1[i] - '0') * (num2[j] - '0');
			int sum = mul + result[i + j + 1];
			
			result[i + j + 1] = sum % 10;
			result[i + j] += sum / 10;
		}
	}
	
	string product;
	bool leadingZero = true;
	for (int i = 0; i < n1 + n2; i++) {
		if (leadingZero && result[i] == 0) continue;
		leadingZero = false;
		product += static_cast<char>(result[i] + '0');
	}
	
	delete[] result;
	return product;
}


string divide(const string& num1, const string& num2) {
	if (num2 == "0") return "0"; 
	
	string dividend = num1;
	string divisor = num2;
	string quotient = "0";
	
	while (dividend.size() > divisor.size() || 
		  (dividend.size() == divisor.size() && dividend >= divisor)) {
		string temp = divisor;
		string multiple = "1";
		
		
		while (dividend.size() > temp.size() + 1 || 
			  (dividend.size() == temp.size() + 1 && dividend > temp + "0")) {
			temp = multiply(temp, "10");
			multiple = multiply(multiple, "10");
		}
		
		while (dividend.size() > temp.size() || 
			  (dividend.size() == temp.size() && dividend >= temp)) {
			dividend = subtract(dividend, temp);
			quotient = add(quotient, multiple);
		}
	}
	
	return quotient;
}


string mod(const string& num1, const string& num2) {
	if (num2 == "0") return "0"; 
	
	string quotient = divide(num1, num2);
	string product = multiply(quotient, num2);
	return subtract(num1, product);
}


string power(const string& base, const string& exp_str) {
	if (exp_str == "0") return "1";
	if (exp_str == "1") return base;
	
	string result = base;
	string exp_minus_one = exp_str;
	
	
	int i = exp_minus_one.size() - 1;
	while (i >= 0 && exp_minus_one[i] == '0') {
		exp_minus_one[i] = '9';
		i--;
	}
	if (i >= 0) {
		exp_minus_one[i]--;
	}
	
	
	int j = 0;
	while (j < (int)exp_minus_one.size() - 1 && exp_minus_one[j] == '0') j++;
	exp_minus_one = exp_minus_one.substr(j);
	
	
	while (exp_minus_one != "0") {
		result = multiply(result, base);
		
		
		i = exp_minus_one.size() - 1;
		while (i >= 0 && exp_minus_one[i] == '0') {
			exp_minus_one[i] = '9';
			i--;
		}
		if (i >= 0) {
			exp_minus_one[i]--;
		}
		
		
		j = 0;
		while (j < (int)exp_minus_one.size() - 1 && exp_minus_one[j] == '0') j++;
		exp_minus_one = exp_minus_one.substr(j);
	}
	
	return result;
}


int precedence(char op) {
	if (op == '^') return 4;
	if (op == '*' || op == '/') return 3;
	if (op == '%') return 2;
	if (op == '+' || op == '-') return 1;
	return 0;
}


string applyOp(const string& a, const string& b, char op) {
	switch (op) {
		case '+': return add(a, b);
		case '-': return subtract(a, b);
		case '*': return multiply(a, b);
		case '/': return divide(a, b);
		case '%': return mod(a, b);
		case '^': return power(a, b);
		default: return "0";
	}
}

bool isNumber(const string& token) {
	for (size_t i = 0; i < token.length(); i++) {
		if (!isdigit(token[i])) return false;
	}
	return true;
}

string evaluateExpression(const string& expression) {
	stack<string> values;
	stack<char> ops;
	
	for (size_t i = 0; i < expression.length(); i++) {
		if (expression[i] == ' ') continue;
		
		if (isdigit(expression[i])) {
			string num;
			while (i < expression.length() && isdigit(expression[i])) {
				num += expression[i++];
			}
			i--;
			values.push(num);
		}
		else if (expression[i] == '(') {
			ops.push(expression[i]);
		}
		else if (expression[i] == ')') {
			while (!ops.empty() && ops.top() != '(') {
				string val2 = values.top(); values.pop();
				string val1 = values.top(); values.pop();
				char op = ops.top(); ops.pop();
				values.push(applyOp(val1, val2, op));
			}
			if (!ops.empty()) ops.pop();
		}
		else {
			while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
				string val2 = values.top(); values.pop();
				string val1 = values.top(); values.pop();
				char op = ops.top(); ops.pop();
				values.push(applyOp(val1, val2, op));
			}
			ops.push(expression[i]);
		}
	}
	
	while (!ops.empty()) {
		string val2 = values.top(); values.pop();
		string val1 = values.top(); values.pop();
		char op = ops.top(); ops.pop();
		values.push(applyOp(val1, val2, op));
	}
	
	return values.top();
}

int main() {
	string expression;
	cout << "Welcome to High Precision Calculator (for non-negative integers)!\nSupported operators: (), ^ (power), */, % (modulo), +-.\nInput expression: ";
	getline(cin, expression);
	
	string result = evaluateExpression(expression);
	cout << "Result: " << result << "\nPress enter to exit.";
	cin.ignore();
	
	return 0;
}

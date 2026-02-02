//
// Created by Mohamed Bakr on 2/1/2026.
//

#include <cstdint>
#include <iostream>
#include <stack>
#include <fstream>
#include <cmath>
#include <string>

/**
 * @brief   expression evaluation implementation with Stack \n
 * infix    5 / 5 + 2 - 1 / 9 \n
 * postfix  5 5 / 2 + 1 9 / -
 */
int precedence(char operat) {
    if (operat == '+' || operat == '-') return 1;
    if (operat == '*' || operat == '/') return 2;
    if (operat == '^') return 3;
    return 0;
}

bool isRightAssociative(char operat) {
    return operat == '^';
}

std::string getPostfixFromInfix(const std::string &infix) {
    std::stack<char> infixStack;
    std::string postfix;
    for (size_t i = 0; i < infix.size(); ++i) {
        char ch = infix[i];
        if (std::isdigit(ch)) {
            while (i < infix.size() && std::isdigit(infix[i])) {
                postfix.push_back(infix[i++]);
            }
            postfix.push_back(' ');
            --i;
        } else if (ch == '(') {
            infixStack.push(ch);
        } else if (ch == ')') {
            while (!infixStack.empty() && infixStack.top() != '(') {
                postfix += infixStack.top();
                postfix.push_back(' ');
                infixStack.pop();
            }
            if (!infixStack.empty()) infixStack.pop();
        } else {
            while (!infixStack.empty() &&
                   infixStack.top() != '(' &&
                   (precedence(infixStack.top()) > precedence(ch) ||
                    (precedence(infixStack.top()) == precedence(ch) && !isRightAssociative(ch)))) {
                postfix += infixStack.top();
                postfix += ' ';
                infixStack.pop();
            }

            infixStack.push(ch);
        }
    }
    while (!infixStack.empty()) {
        postfix += infixStack.top();
        postfix.push_back(' ');
        infixStack.pop();
    }
    return postfix;
}
/// get the value of the postfix

double applyOperation (double x, double y, char operat) {
    return (operat == '+') ? x + y :
           (operat == '-') ? x - y :
           (operat == '*') ? x * y :
           (operat == '/') ? x / y :
           (operat == '^') ? std::pow(x, y) : 0.0;
}


double getValueFromGeneratedPostfix(const std::string &infix, std::string (*toPostfix)(const std::string &)) {
    std::string postfix = toPostfix(infix);
    std::stack<double> calculatedPostfixStack;
    std::string token;
    for (size_t i = 0; i < postfix.size(); ++i) {
        char ch = postfix[i];

        if (std::isdigit(ch)) {
            token.clear();
            while (i < postfix.size() && std::isdigit(postfix[i])) {
                token += postfix[i++];
            }
            calculatedPostfixStack.push(std::stod(token));
        }
        else if (ch == ' ') {
            continue;
        }
        else {
            double y = calculatedPostfixStack.top();
            calculatedPostfixStack.pop();
            double x = calculatedPostfixStack.top();
            calculatedPostfixStack.pop();

            calculatedPostfixStack.push(applyOperation(x, y, ch));
        }
    }
    return calculatedPostfixStack.top();
}

/// =====================================================================================================


/**
 * @brief   Checks if an expression is balanced
 *
 * @param   expression The input expression string
 * @return  true if the expression is balanced
 * @return  false if the expression is not balanced
 */

// Balanced Brackets using Stack
bool arePair(char open, char close) {
    if ((open == '(' && close == ')') || (open == '{' && close == '}') || (open == '[' && close == ']'))
        return true;
    return false;
}

bool isBalanced(const std::string &exp) {
    std::stack<char> stk;
    for (auto ch: exp) {
        if (ch == '(' || ch == '{' || ch == '[') {
            stk.push(ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (stk.empty() || !arePair(stk.top(), ch)) {
                return false;
            }
            stk.pop();
        }
    }
    return stk.empty();
}

void test_isBalanced_Function(int tc) {
    std::string expression;
    while (tc--) {
        std::cin >> expression;
        if (isBalanced(expression)) {
            std::cout << "balanced Expression!" << std::endl;
        } else {
            std::cout << "not balanced Expression!" << std::endl;
        }
    }
}

int_fast32_t main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    std::string expression;
    std::getline(std::cin, expression);
    std::cout << getPostfixFromInfix(expression) << std::endl;

    std::cout << getValueFromGeneratedPostfix(expression, getPostfixFromInfix) << std::endl;
    return 0;
}

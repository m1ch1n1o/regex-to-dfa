#include <bits/stdc++.h>

using namespace std;

string input, regexStr;
int num;

class transition;

typedef struct node {
    int id = 0;
    bool isEndNode = 0;
    vector<transition *> transitions;

    node(int id, bool isEndNode) {
        this->id = id;
        this->isEndNode = isEndNode;
    }
} node;

typedef struct transition {
    node *to;
    char symbol;

    transition(node *to, char symbol) {
        this->to = to;
        this->symbol = symbol;
    }

} transition;

typedef struct nfa {
    node *startNode;
    set<node *> endNodes;
} nfa;

bool isSymbol(char ch) {
    if ((ch <= 'z' && ch >= 'a') || (ch <= '9' && ch >= '0')) {
        return 1;
    }
    return 0;
}

string addConcatSymbol() {
    string res = "";
    res += input[0];
    for (int i = 1; i < input.size(); ++i) {
        if ((isSymbol(input[i - 1]) || input[i - 1] == ')' || input[i - 1] == '*')
            && (isSymbol(input[i]) || input[i] == '(')) {

            res += '&';
        }
        res += input[i];
    }
    return res;
}

nfa Star(nfa nf) {
    vector<transition *> transitionStartNodes = nf.startNode->transitions;
    set<node *> endNodes = nf.endNodes;

    for (node *endNode: endNodes) {
        for (int j = 0; j < transitionStartNodes.size(); ++j)
            endNode->transitions.push_back(
                    new transition(transitionStartNodes[j]->to, transitionStartNodes[j]->symbol));
    }

    nf.startNode->isEndNode = 1;
    nf.endNodes.insert(nf.startNode);

    return nf;
}

nfa Concat(nfa nfaFirst, nfa nfaSecond) {
    set<node *> firstEndNodes = nfaFirst.endNodes;
    vector<transition *> secondStartTransitions = nfaSecond.startNode->transitions;

    for (node *firstEnd: firstEndNodes) {
        for (int i = 0; i < secondStartTransitions.size(); ++i) {
            if (!nfaSecond.startNode->isEndNode) firstEnd->isEndNode = false;
            firstEnd->transitions.push_back(
                    new transition(secondStartTransitions[i]->to, secondStartTransitions[i]->symbol));
        }
    }

    if (!nfaSecond.startNode->isEndNode) nfaFirst.endNodes.clear();

    for (node *secondStartNode: nfaSecond.endNodes)
        nfaFirst.endNodes.insert(secondStartNode);

    return nfaFirst;
}

nfa Union(nfa nfaFirst, nfa nfaSecond) {
    vector<transition *> secondStartTransitions = nfaSecond.startNode->transitions;

    for (transition *secondStart: secondStartTransitions) {
        nfaFirst.startNode->transitions.push_back(
                new transition(secondStart->to, secondStart->symbol));
    }

    if (nfaSecond.startNode->isEndNode == 1) {
        nfaFirst.startNode->isEndNode = 1;
        nfaFirst.endNodes.insert(nfaFirst.startNode);
    }

    for (node *secondEnd: nfaSecond.endNodes) {
        nfaFirst.endNodes.insert(secondEnd);
    }

    return nfaFirst;
}

int priority(char ch) {
    switch (ch) {
        case '(':
            return 0;
        case '|':
            return 1;
        case '&':
            return 2;
        case '*':
            return 3;
        default:
            // Handle any other characters here, if needed
            return -1; // Or any other default value you prefer
    }
}

string regexToPostFix(string infix) {
    int i = 0;
    string postfix = "";
    stack<int> s;

    while (i < infix.size()) {
        if (isSymbol(infix[i])) {
            postfix += infix[i];
            i++;
        } else if (infix[i] == '(') {
            s.push(infix[i]);
            i++;
        } else if (infix[i] == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            s.pop();
            i++;
        } else {
            while (!s.empty() && priority(infix[i]) <= priority(s.top())) {
                postfix += s.top();
                s.pop();
            }
            s.push(infix[i]);
            i++;
        }
    }
    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }

    return postfix;
}

nfa makeNFA(char ch) {
    node *s = new node(num++, 0);
    node *e = new node(num++, 1);
    num--;

    nfa answ;
    s->transitions.push_back(new transition(e, ch));
    answ.startNode = s;
    answ.endNodes.insert(e);
    return answ;
}

nfa regexToNfa(string basicString) {
    stack<nfa> s;
    for (char ch: basicString) {
        nfa temp, temp1, temp2;
        switch (ch) {
            case '*':
                temp = s.top();
                s.pop();
                s.push(Star(temp));
                break;
            case '|':
                temp2 = s.top();
                s.pop();
                temp1 = s.top();
                s.pop();

                s.push(Union(temp1, temp2));
                break;
            case '&':
                temp2 = s.top();
                s.pop();
                temp1 = s.top();
                s.pop();

                s.push(Concat(temp1, temp2));
                break;
            default:
                temp = makeNFA(ch);
                s.push(temp);
                break;
        }
    }
    return s.top();
}

int n, a, t;
set<int> accepts;
map<int, map<int, map<char, vector<int>>>> moves;


map<int, node *> nodeMap;

void nfaOutput(nfa nfa1) {
    node *start = nfa1.startNode;

    queue<node *> q;
    q.push(start);
    set<int> beenHere;

    while (!q.empty()) {
        node *tmp = q.front();
        q.pop();

        nodeMap.insert({tmp->id, tmp});

        vector<transition *> transitions = tmp->transitions;
        for (auto tr: transitions) {
            if (beenHere.find(tr->to->id) == beenHere.end()) {
                beenHere.insert(tmp->id);
                q.push(tr->to);
            }
        }
    }

    int transitionNum = 0;
    for (auto m: nodeMap) {
        vector<transition *> trans = m.second->transitions;

        set<pair<char, int>> pairSt;
        for (auto tr: trans) {
            pairSt.insert(make_pair(tr->symbol, tr->to->id));
        }

        transitionNum += pairSt.size();
    }

    set<int> st;
    for (auto nd: nfa1.endNodes) {
        int sz = st.size();
        st.insert(nd->id);
        if (sz == st.size()) continue;
    }

    cout << nodeMap.size() << " " << st.size() << " " << transitionNum << endl;

    for (auto nu: st) {
        cout << nu << " ";
    }
    cout << endl;


    for (auto m: nodeMap) {
        vector<transition *> trans = m.second->transitions;

        set<pair<char, int>> pairSt;
        for (transition *tr: trans) {
            pairSt.insert(make_pair(tr->symbol, tr->to->id));
        }

        cout << pairSt.size() << " ";
        for (auto pr: pairSt) {
            cout << pr.first << " ";
            cout << pr.second << " ";
        }
        cout << endl;
    }
}


int main() {
    num = 0;
    cin >> input;
    regexStr = regexToPostFix(addConcatSymbol());
    nfa doneNFA = regexToNfa(regexStr);

    nfaOutput(doneNFA);
    return 0;
}
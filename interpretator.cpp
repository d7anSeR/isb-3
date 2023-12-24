
#include <map>
#include <iostream>
#include <math.h>
#include <string>
#include <cstring>
#include <stack>

using namespace std;
typedef string STR;
std::map<unsigned, std::string> TokenType
{
	{1, "DIGIT_INT"}, {2, "DIGIT_DOUBLE"} , {3, "WORD"}, {4, "SIGN"}
};
class Parser;
class Lexer;
class Tel {
	STR text = "";
	STR result = "";
	STR token_type;
	STR value;
	bool fun_flag = false;
	Tel* parent = NULL;
	Tel* pre = NULL;
	Tel* next = NULL;
	Tel* first = NULL;
	Tel* last = NULL;
	friend class Lexer;
	friend class Parser;
public:
	Tel() : text(""), result(""), parent(NULL), pre(NULL), next(NULL), last(NULL), first(NULL) {}
	Tel* Add(Tel* elem) {
		elem->Delete();
		if (first == NULL) {
			first = elem;
			last = first;
			first->pre = NULL;
			first->next = NULL;
			first->parent = this;
			return first;
		}
		last->next = elem;
		last->next->pre = last;
		last->next->parent = this;
		last->next->next = NULL;
		last = last->next;
		return last;
	}
	Tel* Delete() { // Óäàëÿåò ýëåìåíò èç ñïèñêà 

		if (parent != 0) // Åñëè óäàëÿåìûé ýëåìåíò ÿâëÿåòüñÿ ïîñëåäíèì â ñïèñêå 
			if (parent->last == this) parent->last = pre;

		if (parent != 0) // Åñëè óäàëÿåìûé ýëåìåíò ÿâëÿåòüñÿ Ïåðâûì â ñïèñêå 
			if (parent->first == this) parent->first = next;

		if (pre != 0) pre->next = next;// Îòñîåäåíÿåì îò ïðåäûäûùåãî 
		if (next != 0) next->pre = pre;// Îòñîåäåíÿåì îò ñëåäóþùåãî 

		next = 0;
		pre = 0;
		parent = 0;

		return this;
	}
	Tel* Add(STR istr, STR type) {
		Tel* result = NULL;
		if (istr != "") {
			result = new Tel();
			result->text = istr;
			result->value = istr;
			result->token_type = type;
			Add(result);
		}
		return result;
	}
	void Brackets(char s1, char s2) {
		Tel* next_elem = first;
		Tel* result = NULL;
		Tel* mel = NULL;
		while (next_elem != NULL) {
			mel = next_elem->next;
			if (next_elem->text[0] == s1) {
				if (result != NULL)
					result = result->Add(next_elem);
				else
					result = next_elem;
			}
			else if (next_elem->text[0] == s2) {
				next_elem->Delete();
				result = result->parent;
				if (result == this)
					result = NULL;
			}
			else if (result != NULL)
				result->Add(next_elem);
			next_elem = mel;
		}
	}
	void Print(Tel* el, string elem) {
		Tel* iel = el;
		while (iel != NULL) {
			std::cout << elem << iel->text << " - " << iel->token_type << endl;
			if (iel->first != NULL)
				Print(iel->first, elem + "    ");
			iel = iel->next;
		}
	}

};
class Parser {
private:
	Tel* cur;
public:
	friend class Tel;
	Parser(Tel* cur) : cur(cur) {}
	void Operation(Tel* node) {
		if (node->first != NULL && node->first->first != NULL) {
			Operation(node->first);
		}
		if (node->first != NULL && node->first->next != NULL && node->first->next->first != NULL) {
			Operation(node->first->next);
		}
		if (node->first != NULL) {
			if (node->first->next != NULL)
			{
				if (node->text == "^")
				{
					if (node->first->token_type == TokenType[3] && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						Tel* result_second = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL && result_second != NULL) {
							if (result_first->token_type == TokenType[2] || result_second->token_type == TokenType[2]) {
								node->text = to_string(pow(stod(result_first->value), stod(result_second->value)));
								node->value = node->text;
								node->token_type = TokenType[2];
							}
							else if (result_first->token_type == TokenType[1] && result_second->token_type == TokenType[1]) {
								node->text = to_string(pow(stoi(result_first->value), stoi(result_second->value)));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}

					}
					else if ((node->first->token_type == TokenType[1] || node->first->token_type == TokenType[2]) && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL) {
							if (result_first->token_type == TokenType[2] || node->first->token_type == TokenType[2]) {
								node->text = to_string(pow(stod(node->first->text), stod(result_first->value)));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (result_first->token_type == TokenType[1] && node->first->token_type == TokenType[1]) {
								node->text = to_string(pow(stoi(node->first->text), stoi(result_first->value)));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}
					else if ((node->first->next->token_type == TokenType[1] || node->first->next->token_type == TokenType[2]) && node->first->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						if (result_first != NULL) {
							if (node->first->next->token_type == TokenType[2] || result_first->token_type == TokenType[2]) {
								node->text = to_string(pow(stod(result_first->value), stod(node->first->next->text)));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (node->first->next->token_type == TokenType[1] && result_first->token_type == TokenType[1]) {
								node->text = to_string(pow(stoi(result_first->value), stoi(node->first->next->text)));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}

				}
				else if (node->text == "%")
				{
					if (node->first->token_type == TokenType[3] && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						Tel* result_second = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL && result_second != NULL) {
							node->text = to_string(stoi(result_first->value) % stoi(result_second->value));
							node->token_type = TokenType[1];
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}

					}
					else if ((node->first->token_type == TokenType[1] || node->first->token_type == TokenType[2]) && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL) {
							node->text = to_string(stoi(node->first->text) % stoi(result_first->value));
							node->token_type = TokenType[1];
							node->value = node->text;
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}
					else if ((node->first->next->token_type == TokenType[1] || node->first->next->token_type == TokenType[2]) && node->first->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						if (result_first != NULL) {
							node->text = to_string(stoi(result_first->value) % stoi(node->first->next->text));
							node->token_type = TokenType[1];
							node->value = node->text;
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL) {
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}

					else if (node->first->token_type == TokenType[2] && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL) {
							node->text = to_string(stoi(node->first->text) % stoi(result_first->value));
							node->token_type = TokenType[2];
							node->value = node->text;
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}
					else if (node->first->token_type == TokenType[3] && node->first->next->token_type == TokenType[2]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						if (result_first != NULL) {
							node->text = to_string(stoi(result_first->value) % stoi(node->first->next->text));
							node->token_type = TokenType[2];
							node->value = node->text;
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}

				}
				else if (node->text == "*")
				{
					if (node->first->token_type == TokenType[3] && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						Tel* result_second = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL && result_second != NULL) {
							if (result_first->token_type == TokenType[2] || result_second->token_type == TokenType[2]) {
								node->text = to_string(stod(result_first->value) * stod(result_second->value));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (result_first->token_type == TokenType[1] && result_second->token_type == TokenType[1]) {
								node->text = to_string(stoi(result_first->value) * stoi(result_second->value));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}

					}
					else if ((node->first->token_type == TokenType[1] || node->first->token_type == TokenType[2]) && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL) {
							if (result_first->token_type == TokenType[2] || node->first->token_type == TokenType[2]) {
								node->text = to_string(stod(node->first->text) * stod(result_first->value));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (result_first->token_type == TokenType[1] && node->first->token_type == TokenType[1]) {
								node->text = to_string(stoi(node->first->text) * stoi(result_first->value));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}

					}
					else if ((node->first->next->token_type == TokenType[1] || node->first->next->token_type == TokenType[2]) && node->first->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						if (result_first != NULL) {
							if (node->first->next->token_type == TokenType[2] || result_first->token_type == TokenType[2]) {
								node->text = to_string(stod(result_first->value) * stod(node->first->next->text));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (node->first->next->token_type == TokenType[1] && result_first->token_type == TokenType[1]) {
								node->text = to_string(stoi(result_first->value) * stoi(node->first->next->text));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}

				}
				else if (node->text == "/")
				{
					if (node->first->token_type == TokenType[3] && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						Tel* result_second = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL && result_second != NULL && stod(result_second->value) != 0) {
							node->text = to_string(stod(result_first->value) / stod(result_second->value));
							node->token_type = TokenType[2];
							node->value = node->text;
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}

					}
					else if ((node->first->token_type == TokenType[1] || node->first->token_type == TokenType[2]) && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL && stod(result_first->value) != 0) {
							node->text = to_string(stod(node->first->text) / stod(result_first->value));
							node->token_type = TokenType[2];
							node->value = node->text;
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}
					else if ((node->first->next->token_type == TokenType[1] || node->first->next->token_type == TokenType[2]) && node->first->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						if (result_first != NULL && stod(node->first->next->text) != 0) {
							node->text = to_string(stod(result_first->value) / stod(node->first->next->text));
							node->token_type = TokenType[2];
							node->value = node->text;
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}
				}
				else if (node->text == "+")
				{
					if (node->first->token_type == TokenType[3] && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						Tel* result_second = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL && result_second != NULL) {
							if (result_first->token_type == TokenType[2] || result_second->token_type == TokenType[2]) {
								node->text = to_string(stod(result_first->value) + stod(result_second->value));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (result_first->token_type == TokenType[1] && result_second->token_type == TokenType[1]) {
								node->text = to_string(stoi(result_first->value) + stoi(result_second->value));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}

					}
					else if ((node->first->token_type == TokenType[1] || node->first->token_type == TokenType[2]) && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL) {
							if (result_first->token_type == TokenType[2] || node->first->token_type == TokenType[2]) {
								node->text = to_string(stod(node->first->text) + stod(result_first->value));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (result_first->token_type == TokenType[1] && node->first->token_type == TokenType[1]) {
								node->text = to_string(stoi(node->first->text) + stoi(result_first->value));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}
					else if ((node->first->next->token_type == TokenType[1] || node->first->next->token_type == TokenType[2]) && node->first->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						if (result_first != NULL) {
							if (node->first->next->token_type == TokenType[2] || result_first->token_type == TokenType[2]) {
								node->text = to_string(stod(result_first->value) + stod(node->first->next->text));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (node->first->next->token_type == TokenType[1] && result_first->token_type == TokenType[1]) {
								node->text = to_string(stoi(result_first->value) + stoi(node->first->next->text));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}


				}
				else if (node->text == "-")
				{
					if (node->first->token_type == TokenType[3] && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						Tel* result_second = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL && result_second != NULL) {
							if (result_first->token_type == TokenType[2] || result_second->token_type == TokenType[2]) {
								node->text = to_string(stod(result_first->value) - stod(result_second->value));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (result_first->token_type == TokenType[1] && result_second->token_type == TokenType[1]) {
								node->text = to_string(stoi(result_first->value) - stoi(result_second->value));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}

					}
					else if ((node->first->token_type == TokenType[1] || node->first->token_type == TokenType[2]) && node->first->next->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->next->text);
						if (result_first != NULL) {
							if (result_first->token_type == TokenType[2] || node->first->token_type == TokenType[2]) {
								node->text = to_string(stod(node->first->text) - stod(result_first->value));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (result_first->token_type == TokenType[1] && node->first->token_type == TokenType[1]) {
								node->text = to_string(stoi(node->first->text) - stoi(result_first->value));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}
					else if ((node->first->next->token_type == TokenType[1] || node->first->next->token_type == TokenType[2]) && node->first->token_type == TokenType[3]) {
						Tel* tmp_node = cur;
						Tel* result_first = FindVal(tmp_node, node->first->text);
						if (result_first != NULL) {
							if (node->first->next->token_type == TokenType[2] || result_first->token_type == TokenType[2]) {
								node->text = to_string(stod(result_first->value) - stod(node->first->next->text));
								node->token_type = TokenType[2];
								node->value = node->text;
							}
							else if (node->first->next->token_type == TokenType[1] && result_first->token_type == TokenType[1]) {
								node->text = to_string(stoi(result_first->value) - stoi(node->first->next->text));
								node->token_type = TokenType[1];
								node->value = node->text;
							}
							node->first->next->Delete();
							node->first->Delete();
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
							{
								node = node->parent;
								if (node->pre != NULL)
									node = node->pre;
								else if (node->parent != NULL)
									node = node->parent;
							}
						}
					}


				}

				if (node->first != NULL) {
					if (node->first->next != NULL) {
						if (node->text != "==" && node->text != "!=" && node->text != "<=" && node->text != ">=" && node->text != ">" && node->text != "<") {
							if (node->first->token_type == TokenType[3] && node->first->next->token_type == TokenType[3]) {
								Tel* tmp_node = cur;
								Tel* result_first = FindVal(tmp_node, node->first->text);
								Tel* result_second = FindVal(tmp_node, node->first->next->text);
								if (result_first != NULL && result_second != NULL) {
									if (result_first->token_type == TokenType[1] && result_second->token_type == TokenType[1]) {
										if (node->text == "^")
											node->text = to_string(pow(stoi(result_first->value), stoi(result_second->value)));
										if (node->text == "%")
											node->text = to_string(stoi(result_first->value) % stoi(result_second->value));
										if (node->text == "*")
											node->text = to_string(stoi(result_first->value) * stoi(result_second->value));
										if (node->text == "/" && stod(result_second->value) != 0)
											node->text = to_string(stod(result_first->value) / stod(result_second->value));
										if (node->text == "+")
											node->text = to_string(stoi(result_first->value) + stoi(result_second->value));
										if (node->text == "-")
											node->text = to_string(stoi(result_first->value) - stoi(result_second->value));
										if (node->text.find('.')) node->token_type = TokenType[2];
										else node->token_type = TokenType[1];
										node->value = node->text;
										node->first->next->Delete();
										node->first->Delete();
									}
									else if ((result_first->token_type == TokenType[2] && result_second->token_type == TokenType[2]) ||
										(result_first->token_type == TokenType[2] && result_second->token_type == TokenType[1]) ||
										(result_first->token_type == TokenType[1] && result_second->token_type == TokenType[2])
										) {
										if (node->text == "^")
											node->text = to_string(pow(stod(result_first->value), stod(result_second->value)));
										if (node->text == "%")
											node->text = to_string(stoi(result_first->value) % stoi(result_second->value));
										if (node->text == "*")
											node->text = to_string(stod(result_first->value) * stod(result_second->value));
										if (node->text == "/" && stod(result_second->value) != 0)
											node->text = to_string(stod(result_first->value) / stod(result_second->value));
										if (node->text == "+")
											node->text = to_string(stod(result_first->value) + stod(result_second->value));
										if (node->text == "-")
											node->text = to_string(stod(result_first->value) - stod(result_second->value));
										node->value = node->text;
										node->token_type = TokenType[2];
										node->first->next->Delete();
										node->first->Delete();
									}
									if (node->pre != NULL)
										node = node->pre;
									else if (node->parent != NULL)
									{
										node = node->parent;
										if (node->pre != NULL)
											node = node->pre;
										else if (node->parent != NULL)
											node = node->parent;
									}
								}
							}

							else if (node->first->token_type == TokenType[1] && node->first->next->token_type == TokenType[1])
							{
								if (node->text == "^")
									node->text = to_string(pow(stoi(node->first->text), stoi(node->first->next->text)));
								if (node->text == "%")
									node->text = to_string(stoi(node->first->text) % stoi(node->first->next->text));
								if (node->text == "*")
									node->text = to_string(stoi(node->first->text) * stoi(node->first->next->text));
								if (node->text == "/" && stod(node->first->next->text) != 0)
									node->text = to_string(stod(node->first->text) / stod(node->first->next->text));
								if (node->text == "+")
									node->text = to_string(stoi(node->first->text) + stoi(node->first->next->text));
								if (node->text == "-")
									node->text = to_string(stoi(node->first->text) - stoi(node->first->next->text));
								if (node->text.find('.')) node->token_type = TokenType[2];
								else node->token_type = TokenType[1];
								node->value = node->text;
								node->first->next->Delete();
								node->first->Delete();
							}
							else if ((node->first->token_type == TokenType[2] && node->first->next->token_type == TokenType[2]) ||
								(node->first->token_type == TokenType[2] && node->first->next->token_type == TokenType[1]) ||
								(node->first->token_type == TokenType[1] && node->first->next->token_type == TokenType[2])
								) {
								if (node->text == "^")
									node->text = to_string(pow(stod(node->first->text), stod(node->first->next->text)));
								if (node->text == "%")
									node->text = to_string(stoi(node->first->text) % stoi(node->first->next->text));
								if (node->text == "*")
									node->text = to_string(stod(node->first->text) * stod(node->first->next->text));
								if (node->text == "/" && stod(node->first->next->text) != 0)
									node->text = to_string(stod(node->first->text) / stod(node->first->next->text));
								if (node->text == "+")
									node->text = to_string(stod(node->first->text) + stod(node->first->next->text));
								if (node->text == "-")
									node->text = to_string(stod(node->first->text) - stod(node->first->next->text));
								node->token_type = TokenType[2];
								node->value = node->text;
								node->first->next->Delete();
								node->first->Delete();
							}
							else if (node->first != NULL) {
								if (node->first->text == "(") {
									Runs(node);
									/*if (node->first != NULL) {
										node->token_type = node->first->token_type;
										node->result = node->first->result;
									}*/
								}
							}
						}
						
					}
				}
			}
			else if (node->pre != NULL) {
				if (node->pre->text == "print" && node->text == "(" && node->first != NULL && node->first->token_type == TokenType[3] && node->first->next == NULL) {
					Tel* tmp_node = cur;
					Tel* result = FindVal(tmp_node, node->first->text);
					if (result != NULL && result->token_type != TokenType[4]) {
						node->first->text = result->value;
						node->first->token_type = result->token_type;
						node->first->value = node->first->text;
						result->Delete();
						if (node->pre != NULL)
							node = node->pre;
						else if (node->parent != NULL)
						{
							node = node->parent;
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
								node = node->parent;
						}
					}
				}
				else if (node->pre->text != "print" && node->text == "(" && node->first != NULL && node->first->next == NULL) {
					if (node->first->first == NULL) {
						node->text = node->first->text;
						node->value = node->first->value;
						node->token_type = node->first->token_type;
						if (node->pre != NULL) {
							node = node->pre;
						}
						else if (node->parent != NULL)
						{
							node = node->parent;
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
								node = node->parent;
							if (node->first != NULL)
								node->first->Delete();
						}
					}
				}
			}
			else if (node->pre == NULL) {
				if (node->text == "(" && node->first != NULL && node->first->next == NULL) {
					if (node->first->first == NULL) {
						node->text = node->first->text;
						node->value = node->first->value;
						node->token_type = node->first->token_type;
						/*node->first->Delete();*/
						if (node->pre != NULL)
							node = node->pre;
						else if (node->parent != NULL)
						{
							node = node->parent;
							if (node->pre != NULL)
								node = node->pre;
							else if (node->parent != NULL)
								node = node->parent;
						}
					}
				}
				/*else
					Runs(node);*/
			}
		}


	}
	Tel* FindVal(Tel* node, std::string ichar) {
		Tel* result = NULL;
		while (node != NULL) {
			if (node->text == ichar) {
				result = node;
				break;
			}
			else {
				if (node->first != NULL) {
					result = FindVal(node->first, ichar);
					if (result != NULL) {
						break;
					}
				}

			}
			node = node->next;
		}

		return result;
	}

	//Tel* FindVal(Tel* node, std::string ichar) {
	//	while (node != NULL) {
	//		if (node->first != NULL) {
	//			if (node->text == ichar)
	//				return node;
	//			FindVal(node->first, ichar);
	//		}
	//		node = node->next;
	//	}
	//}
	void Print(Tel* node) {
		Tel* iel = node;
		if (iel->next->text == "(") {
			Runs(iel->next);
			Tel* elem = NULL;
			if (iel->next != NULL) elem = iel->next->first;
			while (elem != NULL) {
				cout << elem->text;
				elem = elem->next;
			}
			
			cout << endl;
		}

	}
	void Entrance(Tel* node) {
		Tel* elem = node;
		if (elem != NULL) {
			std::string result = "";
			if (elem->next != NULL) {
				for (int i = 0; i < (elem->text).length(); i++)
					result += tolower(elem->text[i]);
				if (result == "main" && elem->next->text == "{")
				{
					MainFunc(elem->next);
				}
			}
			if (elem->first != NULL)
				Entrance(elem->first);
		}
		if (node->next != NULL)
			Entrance(node->next);
	}
	void MainFunc(Tel* node) {
		Tel* elem = node;
		std::string tmp_result = "";
		for (int i = 0; i < (elem->text).length(); i++)
			tmp_result += tolower(elem->text[i]);
		elem->text = tmp_result;
		if (elem->first != NULL) {
			if (elem->first->first != NULL) {
				MainFunc(elem->first);
			}
			else if (elem->first->next != NULL) {
				if (elem->first->next->first != NULL) {
					MainFunc(elem->first->next);
				}
			}
			std::string result = "";
			for (int i = 0; i < (elem->text).length(); i++)
				result += tolower(elem->text[i]);
			elem->text = result;
			if (result == "=") {
				if (elem->first->token_type == TokenType[3] && (elem->first->next->token_type == TokenType[1] ||
					elem->first->next->token_type == TokenType[2])) {
					elem->value = elem->first->next->text;
					elem->text = elem->first->text;
					elem->token_type = elem->first->next->token_type;
					elem->first->next->Delete();
					elem->first->Delete();
				}
				else if (elem->first->token_type == TokenType[3] && elem->first->next->token_type == TokenType[4]) {
					Runs(elem);
					elem->value = elem->first->next->text;
					elem->text = elem->first->text;
					elem->token_type = elem->first->next->token_type;
					elem->first->next->Delete();
					elem->first->Delete();
				}
			}
			if (result == "^" || result == "%" || result == "*" || result == "/" || result == "+" || result == "-") {
				Runs(elem);
			}
			if (result == "(") {
				if (elem->pre != NULL) {
					if (elem->pre->text == "print" && elem->pre->fun_flag == false) {
						elem->pre->fun_flag = true;
						Print(elem->pre);
						if (elem->next != NULL)
							MainFunc(elem->next);
					}
					/*else if (elem->pre->text == "print" && elem->pre->fun_flag == true) {
						if(elem->next != NULL) 
							MainFunc(elem->next);
						else if(elem->pre->pre != NULL)
							MainFunc(elem->pre->pre);
					}
					else
						MainFunc(elem->pre);*/
				}

			}
		}
		if (tmp_result == "if") {
			if (elem->next->first->first->token_type == TokenType[3] && elem->next->first->first->next->token_type == TokenType[3]) {
				Tel* tmp_node = cur;
				Tel* result_first = FindVal(tmp_node, elem->next->first->first->text);
				Tel* result_second = FindVal(tmp_node, elem->next->first->first->next->text);
				if (result_first != NULL && result_second != NULL) {
					if (result_first->token_type == TokenType[1] && result_second->token_type == TokenType[1]) {
						if (elem->next->first->text == "<") {
							if (stoi(result_first->value) < stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">") {
							if (stoi(result_first->value) > stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
						}
						else if (elem->next->first->text == "<=") {
							if (stoi(result_first->value) <= stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">=") {
							if (stoi(result_first->value) >= stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "!=") {
							if (stoi(result_first->value) != stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "==") {
							if (stoi(result_first->value) == stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
					}

					if ((result_first->token_type == TokenType[2] && result_second->token_type == TokenType[2]) ||
						(result_first->token_type == TokenType[2] && result_second->token_type == TokenType[1]) ||
						(result_first->token_type == TokenType[1] && result_second->token_type == TokenType[2])) {
						if (elem->next->first->text == "<") {
							if (stod(result_first->value) < stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">") {
							if (stod(result_first->value) > stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "<=") {
							if (stod(result_first->value) <= stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">=") {
							if (stod(result_first->value) >= stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "!=") {
							if (stod(result_first->value) != stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "==") {
							if (stod(result_first->value) == stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
					}
				}

			}
			if ((elem->next->first->first->token_type == TokenType[1] || elem->next->first->first->token_type == TokenType[2]) && elem->next->first->first->next->token_type == TokenType[3]) {
				Tel* tmp_node = cur;
				Tel* result_first = elem->next->first->first;
				Tel* result_second = FindVal(tmp_node, elem->next->first->first->next->text);
				if (result_first != NULL && result_second != NULL) {
					if (result_first->token_type == TokenType[1] && result_second->token_type == TokenType[1]) {
						if (elem->next->first->text == "<") {
							if (stoi(result_first->value) < stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">") {
							if (stoi(result_first->value) > stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "<=") {
							if (stoi(result_first->value) <= stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">=") {
							if (stoi(result_first->value) >= stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "!=") {
							if (stoi(result_first->value) != stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "==") {
							if (stoi(result_first->value) == stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
					}

					if ((result_first->token_type == TokenType[2] && result_second->token_type == TokenType[2]) ||
						(result_first->token_type == TokenType[2] && result_second->token_type == TokenType[1]) ||
						(result_first->token_type == TokenType[1] && result_second->token_type == TokenType[2])) {
						if (elem->next->first->text == "<") {
							if (stod(result_first->value) < stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">") {
							if (stod(result_first->value) > stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "<=") {
							if (stod(result_first->value) <= stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">=") {
							if (stod(result_first->value) >= stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "!=") {
							if (stod(result_first->value) != stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "==") {
							if (stod(result_first->value) == stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
					}
				}

			}
			if (elem->next->first->first->token_type == TokenType[3] && (elem->next->first->first->next->token_type == TokenType[1] || elem->next->first->first->next->token_type == TokenType[2])) {
				Tel* tmp_node = cur;
				Tel* result_first = FindVal(tmp_node, elem->next->first->first->text);
				Tel* result_second = elem->next->first->first->next;
				if (result_first != NULL && result_second != NULL) {
					if (result_first->token_type == TokenType[1] && result_second->token_type == TokenType[1]) {
						if (elem->next->first->text == "<") {
							if (stoi(result_first->value) < stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">") {
							if (stoi(result_first->value) > stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "<=") {
							if (stoi(result_first->value) <= stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">=") {
							if (stoi(result_first->value) >= stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "!=") {
							if (stoi(result_first->value) != stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "==") {
							if (stoi(result_first->value) == stoi(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
					}

					if ((result_first->token_type == TokenType[2] && result_second->token_type == TokenType[2]) ||
						(result_first->token_type == TokenType[2] && result_second->token_type == TokenType[1]) ||
						(result_first->token_type == TokenType[1] && result_second->token_type == TokenType[2])) {
						if (elem->next->first->text == "<") {
							if (stod(result_first->value) < stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">") {
							if (stod(result_first->value) > stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "<=") {
							if (stod(result_first->value) <= stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == ">=") {
							if (stod(result_first->value) >= stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "!=") {
							if (stod(result_first->value) != stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
						else if (elem->next->first->text == "==") {
							if (stod(result_first->value) == stod(result_second->value)) {
								MainFunc(elem->next->next);
							}
							else
								MainFunc(elem->next->next->next);
						}
					}
				}

			}
		}
		else if (elem->next != NULL) {
			if (elem->next->text == "print" && elem->next->fun_flag == false) {
				elem->next->fun_flag = true;
				Print(elem->next);
				if (elem->next->next->next != NULL)
					MainFunc(elem->next->next->next);
			}
			else if (elem->next->text != "(")
				MainFunc(elem->next);
			else if (elem->next->next != NULL)
				MainFunc(elem->next->next);
			else
				MainFunc(elem->next);
		}

	}
	void Runs(Tel* node) {
		Tel* elem = node;
		while (elem != NULL) {
			Operation(elem);
			elem = elem->next;
		}
	}
};
class Lexer {
private:
	int curr_pos = 0;
	string curr_str = "";
public:
	friend class Parser;
	Lexer(int curr_pos = 0, string curr_str = "") : curr_pos(curr_pos), curr_str(curr_str) {}
	bool CheckLetter(char ichar) {
		if ((ichar >= 'a' && ichar <= 'z') || (ichar >= 'A' && ichar <= 'Z') || ichar == '_')
			return true;
		return false;
	}
	bool CheckDigit(char ichar) {
		if ((ichar >= '0' && ichar <= '9'))
			return true;
		return false;
	}
	bool CheckDigitLetter(char ichar) {
		return (CheckLetter(ichar) || CheckDigit(ichar));
	}
	string ReadWord() {
		string result = "";
		if (CheckLetter(curr_str[curr_pos])) {
			while (curr_pos < curr_str.length() && CheckDigitLetter(curr_str[curr_pos])) {
				result += curr_str[curr_pos];
				curr_pos++;
			}
		}
		return result;
	}
	string ReadDigit() {
		string result = "";
		if (CheckDigit(curr_str[curr_pos])) {
			while (curr_pos < curr_str.length() &&
				(CheckDigitLetter(curr_str[curr_pos]) ||
					(curr_str[curr_pos] == '.' && curr_pos != curr_str.length())
					)) {
				result += curr_str[curr_pos];
				curr_pos++;
			}
		}
		return result;
	}
	string ReadSign() {
		string result = "";
		if (curr_pos < curr_str.length()) {
			string temp_sign = "";
			temp_sign += curr_str[curr_pos];
			if (
				(temp_sign == "(") ||
				(temp_sign == ")") ||
				(temp_sign == "<") && (curr_str[curr_pos + 1] != '=') ||
				(temp_sign == ">") && (curr_str[curr_pos + 1] != '=') ||
				(temp_sign == "=") && (curr_str[curr_pos + 1] != '=') ||
				(temp_sign == "!") && (curr_str[curr_pos + 1] != '=') ||
				(temp_sign == "{") ||
				(temp_sign == "}") ||
				(temp_sign == "[") ||
				(temp_sign == "]") ||
				(temp_sign == "+") && (curr_str[curr_pos + 1] != '=') ||
				(temp_sign == "-") && (curr_str[curr_pos + 1] != '=') ||
				(temp_sign == "^") ||
				(temp_sign == "/") && (curr_str[curr_pos + 1] != '=') ||
				(temp_sign == "*") && (curr_str[curr_pos + 1] != '=') ||
				(temp_sign == "%") ||
				(temp_sign == ";") ||
				(temp_sign == ",")
				) {
				result = temp_sign;
				curr_pos += 1;
			}
			else if ((curr_pos + 1 < curr_str.length()) && (
				(temp_sign == "<") && (curr_str[curr_pos + 1] == '=') ||
				(temp_sign == ">") && (curr_str[curr_pos + 1] == '=') ||
				(temp_sign == "=") && (curr_str[curr_pos + 1] == '=') ||
				(temp_sign == "!") && (curr_str[curr_pos + 1] == '=') ||
				(temp_sign == "+") && (curr_str[curr_pos + 1] == '=') ||
				(temp_sign == "-") && (curr_str[curr_pos + 1] == '=') ||
				(temp_sign == "/") && (curr_str[curr_pos + 1] == '=') ||
				(temp_sign == "*") && (curr_str[curr_pos + 1] == '=') ||
				(temp_sign == ":") && (curr_str[curr_pos + 1] == '=')
				)) {
				temp_sign += curr_str[curr_pos + 1];
				result = temp_sign;
				curr_pos += 2;
			}
		}
		return result;
	}
	string ReadTabs() {
		string result = "";
		while ((curr_str[curr_pos] == ' ' || curr_str[curr_pos] == '\n' || curr_str[curr_pos] == '\t')
			&& curr_pos < curr_str.length()) {
			result += curr_str[curr_pos];
			curr_pos++;
		}
		return result;
	}

	Tel* ReadWordTel(const string& istr) {
		curr_str = istr;
		int len = istr.length();
		Tel* result = new Tel();
		while (curr_pos < len) {
			result->Add(ReadWord(), TokenType[3]);
			std::string digit = ReadDigit();
			if (digit.find(".") == true)
				result->Add(digit, TokenType[2]);
			else if (digit != "")
				result->Add(digit, TokenType[1]);
			ReadTabs();
			result->Add(ReadSign(), TokenType[4]);
		}
		return result;
	}

	void BlockInFunc(Tel* iel) {
		Tel* next_elem_first = iel->first;
		while (next_elem_first != NULL) {
			Tel* next_elem_second = next_elem_first->first;
			while (next_elem_second != NULL) {
				if (next_elem_second->token_type == TokenType[1])
				{
					if ((next_elem_second->next != NULL) && (next_elem_second->next->text[0] == '{'))
					{
						next_elem_second->fun_flag = true;
						next_elem_second->Add(next_elem_second->next);
					}
				}
				next_elem_second = next_elem_second->first;
			}
			next_elem_first = next_elem_first->next;

		}
	}
	void ParamInFunc(Tel* iel) {
		Tel* next_elem_first = iel->first;
		while (next_elem_first != NULL) {
			Tel* next_elem_second = next_elem_first->first;
			while (next_elem_second != NULL) {
				if (next_elem_second->token_type == TokenType[1])
					if ((next_elem_second->next != NULL) && (next_elem_second->next->text[0] == '('))
						next_elem_second->Add(next_elem_second->next);
				next_elem_second = next_elem_second->first;
			}
			next_elem_first = next_elem_first->next;

		}
	}
	void SignInFirst(Tel* iel) {
		Tel* elem = iel->first;
		while (elem != NULL) {
			if (elem->next != NULL && elem->pre != NULL && ((elem->text == "^") || (elem->text == "%") || (elem->text == "*") || (elem->text == "/"))) {
				elem->Add(elem->pre);
				elem->Add(elem->next);
			}
			if (elem->first != NULL) SignInFirst(elem);
			elem = elem->next;
		}
	}
	void SignInSecond(Tel* iel) {
		Tel* elem = iel->first;
		while (elem != NULL) {
			if (elem->next != NULL && elem->pre != NULL && ((elem->text == "+") || (elem->text == "-"))) {
				elem->Add(elem->pre);
				elem->Add(elem->next);
			}
			if (elem->first != NULL) SignInSecond(elem);
			elem = elem->next;
		}
	}
	void SignComparison(Tel* iel) {
		Tel* elem = iel->first;
		while (elem != NULL) {
			if (elem->next != NULL && elem->pre != NULL && ((elem->text == "<") || (elem->text == ">") || (elem->text == "<=") || (elem->text == ">=") || (elem->text == "==") || (elem->text == "!="))) {
				elem->Add(elem->pre);
				elem->Add(elem->next);
			}
			if (elem->first != NULL) SignComparison(elem);
			elem = elem->next;
		}
	}
	void AssignmentIn(Tel* iel) {
		Tel* elem = iel->first;
		while (elem != NULL) {
			if (elem->next != NULL && elem->pre != NULL && (elem->text == "=")) {
				elem->Add(elem->pre);
				elem->Add(elem->next);
			}
			if (elem->first != NULL) AssignmentIn(elem);
			elem = elem->next;
		}
	}
};

int main() {
	STR prog = "  MAIN{ a = 6 b = 3 print((1+ b)^2) if(a > b) {print(1) print(2)} if(a != 6){print(0) print(9)} print(11)}";
	Lexer lexer;
	Tel* pro = lexer.ReadWordTel(prog);
	pro->Brackets('[', ']');
	pro->Brackets('(', ')');
	pro->Brackets('{', '}');
	lexer.ParamInFunc(pro);
	lexer.BlockInFunc(pro);
	lexer.SignInFirst(pro);
	lexer.SignInSecond(pro);
	lexer.AssignmentIn(pro);
	lexer.SignComparison(pro);
	pro->Print(pro, " ");
	/*lexer.Print(pro, " ");*/
	Parser parser(pro);
	parser.Entrance(pro);
	pro->Print(pro, " ");
}
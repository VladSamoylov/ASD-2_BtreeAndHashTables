#include <iostream>
#include <vector>
#include "tinyxml2.h"
#include "unzip.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_RED "\033[31m"

using namespace std;

template <typename T, typename Y>
class SingleLinkedList;

template <typename T, typename Y>
class Node {
private:
	T key;
	Y data;
	Node<T, Y>* ptr_next;
	friend class SingleLinkedList<T, Y>;
public:
	Node(T key, Y data) : key(key), data(data), ptr_next(nullptr) {}
	Node(const Node<T, Y>& other) { this->key = other.key; this->data = other.data; this->ptr_next = nullptr; }
};

template <typename T, typename Y>
class SingleLinkedList {
private:
	Node<T, Y>* root_ptr;
	Node<T, Y>* current;
public:
	SingleLinkedList() : root_ptr(nullptr), current(nullptr) {}
	SingleLinkedList& operator=(const SingleLinkedList<T, Y>&);
	void PushBack(const T&, const Y&);
	int FindElem(const T&);
	void Increment(const string&);
	bool IsElement(const T&);
	void Show();
	int Size();
	void DeleteNode(const T&);
	void DeleteAllNodes();
	bool isEmty() { if (!this->root_ptr) return true; else return false; }
	pair<T, Y> GetDataByIndex(const Y&);
	~SingleLinkedList();
};

template <typename T, typename Y>
SingleLinkedList<T, Y>::~SingleLinkedList() {

	this->current = this->root_ptr;
	while (this->current) {
		Node<T, Y>* next = this->current->ptr_next;
		delete this->current;
		this->current = next;
		//cout << "Delete Node" << endl;
	}
	this->root_ptr = nullptr;
}

template <typename T, typename Y>
void SingleLinkedList<T, Y>::PushBack(const T& key, const Y& data) {

	if (!root_ptr) {
		this->root_ptr = new Node<T, Y>(key, data);
	}
	else {
		this->current = new Node<T, Y>(key, data);
		this->current->ptr_next = this->root_ptr;
		this->root_ptr = this->current;
	}
}

template <typename T, typename Y>
SingleLinkedList<T, Y>& SingleLinkedList<T, Y>::operator=(const SingleLinkedList<T, Y>& other) {
	
	if (this != &other) {		
		if (other.root_ptr == nullptr) {
			this->root_ptr = nullptr;
			this->current = nullptr;
		}
		else {
			this->root_ptr = new Node<T, Y>(other.root_ptr->key, other.root_ptr->data);
			this->current = this->root_ptr;
			Node<T, Y>* tmp_ptr = other.root_ptr->ptr_next;
			while (tmp_ptr) {
				this->current->ptr_next = new Node<T, Y>(tmp_ptr->key, tmp_ptr->data);
				this->current = this->current->ptr_next;
				tmp_ptr = tmp_ptr->ptr_next;
			}
		}
	}
	return *this;
}

template <typename T, typename Y>
int SingleLinkedList<T, Y>::FindElem(const T& key) {

	this->current = nullptr;
	this->current = this->root_ptr;
	while (this->current) {
		if (this->current->key == key) return this->current->data;
		else {
			this->current = current->ptr_next;
		}
	}
	throw "Error: <Don't find element by current key>";
}

template <typename T, typename Y>
void SingleLinkedList<T, Y>::DeleteNode(const T& key) {

	Node<T, Y>* tmp_ptr = nullptr;
	bool del = false;
	this->current = nullptr;
	this->current = this->root_ptr;
	while (this->current) {
		if (this->current->key == key) {
			if (this->current == this->root_ptr && this->current->ptr_next != nullptr) {
				this->root_ptr = this->current->ptr_next;
				delete this->current;
				del = true;
				this->current = nullptr;
			}
			else if (this->current == this->root_ptr && this->current->ptr_next == nullptr) {
				delete this->current;
				del = true;
				this->root_ptr = nullptr;
				this->current = nullptr;
			}
			else if (this->current->ptr_next == nullptr) {
				tmp_ptr = this->root_ptr;
				do {
					tmp_ptr = tmp_ptr->ptr_next;
				} while (tmp_ptr->ptr_next == this->current);
				tmp_ptr->ptr_next = nullptr;
				delete this->current;
				del = true;
				this->current = nullptr;
			}
			else {
				tmp_ptr = this->root_ptr;
				do {
					tmp_ptr = tmp_ptr->ptr_next;
				} while (tmp_ptr->ptr_next == this->current);
				tmp_ptr->ptr_next = this->current->ptr_next;
				delete this->current;
				del = true;
				this->current = nullptr;
			}
		}
		else {
			this->current = current->ptr_next;
		}
	}
	if (!del) throw "Error: <Don't find element by current key>";
}

template <typename T, typename Y>
void SingleLinkedList<T, Y>::DeleteAllNodes() {
	
	this->current = this->root_ptr;
	while (this->current) {
		Node<T, Y>* next = this->current->ptr_next;
		delete this->current;
		this->current = next;
		//cout << "Delete Node" << endl;
	}
	this->root_ptr = nullptr;
}

template <typename T, typename Y>
bool SingleLinkedList<T, Y>::IsElement(const T& key) {

	this->current = nullptr;
	this->current = this->root_ptr;
	while (this->current) {
		if (this->current->key == key) return true;
		else {
			this->current = current->ptr_next;
		}
	}
	return false;
}

template <>
void SingleLinkedList<string, int>::Increment(const string& key) {

	this->current = nullptr;
	this->current = this->root_ptr;
	while (this->current) {
		if (this->current->key == key) { this->current->data++; break; }
		else {
			this->current = current->ptr_next;
		}
	}
}

template <typename T, typename Y>
int SingleLinkedList<T, Y>::Size() {

	this->current = nullptr;
	this->current = this->root_ptr;
	int count = 0;
	while (this->current) {
		count++;
		this->current = this->current->ptr_next;
	}
	return count;
}

template <typename T, typename Y>
pair<T, Y> SingleLinkedList<T, Y>::GetDataByIndex(const Y& index) {

	try {
		if (this->Size() <= index) throw "Error <Out of range>";
		this->current = nullptr;
		this->current = this->root_ptr;
		for (int i = 1; i <= index; i++) {
			this->current = this->current->ptr_next;
		}
	}
	catch (const char* err) {
		cerr << err << endl;
	}	
	return make_pair(this->current->key, this->current->data);
}

template <typename T, typename Y>
void SingleLinkedList<T, Y>::Show() {

	this->current = nullptr;
	this->current = this->root_ptr;
	while (this->current) {
		cout << "--" << this->current->key << " " << this->current->data << " ID: " << this->current << endl;
		this->current = this->current->ptr_next;
	}
}

template <typename T, typename Y>
class HashTable {
private:
	bool isDivMethod;
	bool isChainsCollision;
	int HashTableSize;
	vector<SingleLinkedList<T, Y>> data;
	int ElemCount;
	friend class Epub;
public:
	HashTable();
	int HashFunction(const T&);
	void Add(const T&, const Y&);
	void AddAmountEpub(const string&, const int&);
	void Delete(const T&);
	void Clear();
	void FindElem(const T&);
	void ShowStructure();
	vector<pair<T, Y>> GetAllElements();
	~HashTable() { this->Clear(); }
};

template <typename T, typename Y>
HashTable<T, Y>::HashTable() {

	this->isDivMethod = true;
	this->isChainsCollision = true;
	this->HashTableSize = 5;
	this->data.resize(this->HashTableSize);
	this->ElemCount = 0;
}

template <typename T, typename Y>
int HashTable<T, Y>::HashFunction(const T& key) {

	return static_cast<int>(key) % this->HashTableSize;
}

template <>
int HashTable<string, int>::HashFunction(const string& key) {

	/*int keyId = 0;
	for (char c : key) {
		keyId += static_cast<int>(c);
	}
	return keyId % this->HashTableSize;*/
	unsigned long hash = 5381;
	for (char c : key) {
		hash = ((hash << 5) + hash) + static_cast<int>(c);
	}
	return hash % this->HashTableSize;
}

template <typename T, typename Y>
void HashTable<T, Y>::Add(const T& key, const Y& data) {

	if (!this->data[this->HashFunction(key)].IsElement(key)) {
		if (this->ElemCount == this->HashTableSize) {
			int Old_HashTableSize = this->HashTableSize;
			this->HashTableSize = this->HashTableSize * 2;
			vector<SingleLinkedList<T, Y>> tmp;
			tmp.resize(this->HashTableSize);
			pair<T, Y> tmp_data;
			for (int i = 0; i < Old_HashTableSize; i++) {
				if (!this->data[i].isEmty()) {
					for (int j = 0; j < this->data[i].Size(); j++) {
						tmp_data = this->data[i].GetDataByIndex(j);
						tmp[this->HashFunction(tmp_data.first)].PushBack(tmp_data.first, tmp_data.second);
					}
				}
			}
			this->data.resize(this->HashTableSize);
			this->data = tmp;
			this->data[this->HashFunction(key)].PushBack(key, data);
			this->ElemCount++;			
		}
		else {
			this->data[this->HashFunction(key)].PushBack(key, data);
			this->ElemCount++;
		}
	}
	else cout << "Element (" << ANSI_COLOR_GREEN << key << ANSI_COLOR_RESET << ") is present" << endl;	
}

template <>
void HashTable<string, int>::AddAmountEpub(const string& key, const int& data) {

	if (!this->data[this->HashFunction(key)].IsElement(key)) {
		if (this->ElemCount == this->HashTableSize) {
			int Old_HashTableSize = this->HashTableSize;
			this->HashTableSize = this->HashTableSize * 2;
			vector<SingleLinkedList<string, int>> tmp;
			tmp.resize(this->HashTableSize);
			pair<string, int> tmp_data;
			for (int i = 0; i < Old_HashTableSize; i++) {
				if (!this->data[i].isEmty()) {
					for (int j = 0; j < this->data[i].Size(); j++) {
						tmp_data = this->data[i].GetDataByIndex(j);
						tmp[this->HashFunction(tmp_data.first)].PushBack(tmp_data.first, tmp_data.second);
					}
				}
			}
			this->data.resize(this->HashTableSize);
			this->data = tmp;
			this->data[this->HashFunction(key)].PushBack(key, data);
			this->ElemCount++;
		}
		else {
			this->data[this->HashFunction(key)].PushBack(key, data);
			this->ElemCount++;
		}
	}
	else this->data[this->HashFunction(key)].Increment(key);
}

template <typename T, typename Y>
void HashTable<T, Y>::ShowStructure() {
	
	cout << "_____________________" << endl;
	for (int i = 0; i < this->HashTableSize; i++) {
		cout << "Bucket #" << ANSI_COLOR_RED << i << ANSI_COLOR_RESET << ':' << endl;
		data[i].Show();
	}	
	cout << "_____________________" << endl;
}

template <typename T, typename Y>
vector<pair<T, Y>> HashTable<T, Y>::GetAllElements() {

	vector<pair<T, Y>> elements;
	pair<T, Y> pair;
	for (int i = 0; i < this->HashTableSize; i++) {
		for (int j = 0; j < this->data[i].Size(); j++) {
			pair = this->data[i].GetDataByIndex(j);
			elements.emplace_back(pair.first, pair.second);
		}
	}
	return elements;
}

template <typename T, typename Y>
void HashTable<T, Y>::FindElem(const T& key) {

	cout << "Find element by key (" ANSI_COLOR_GREEN << key << ANSI_COLOR_RESET << ") : ";
	try {
		cout << ANSI_COLOR_BLUE << this->data[this->HashFunction(key)].FindElem(key) << ANSI_COLOR_RESET << " Bucket: " << this->HashFunction(key) << endl;
	}
	catch (const char* err) {
		cout << ANSI_COLOR_RESET << err << endl;
	}
}

template <typename T, typename Y>
void HashTable<T, Y>::Delete(const T& key) {

	cout << "Delete element by key (" ANSI_COLOR_GREEN << key << ANSI_COLOR_RESET << ") : ";
	try {
		this->data[this->HashFunction(key)].DeleteNode(key);
		cout << "completed...\n";
	}
	catch (const char* err) {
		cout << err << endl;
	}
}

template <typename T, typename Y>
void HashTable<T, Y>::Clear() {

	for (int i = 0; i < this->data.size(); i++) {
		this->data[i].DeleteAllNodes();
	}
	this->data.clear();
	this->ElemCount = 0;
	this->HashTableSize = 5;
	this->data.resize(this->HashTableSize);
}

class Epub : public HashTable<string, int> {
private:
	void ParseXHTML(const string&);
	void GetTextFromElement(tinyxml2::XMLElement*);
public:
	void ExtractEpub(const string&);
	void PrintTop50(const string&);
};

void Epub::ExtractEpub(const string& filename) {

	unzFile epub = unzOpen(filename.c_str());
	if (!epub) {
		throw "Error: <Failed to open EPUB file";
	}
	if (unzGoToFirstFile(epub) == UNZ_OK) {
		do {
			char filename_inzip[256];
			unz_file_info file_info;
			if (unzGetCurrentFileInfo(epub, &file_info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0) == UNZ_OK) {
				cout << "File EPUB: " << filename_inzip << endl;

				if (strstr(filename_inzip, ".html") != nullptr || strstr(filename_inzip, ".xhtml") != nullptr) {
					unzOpenCurrentFile(epub);
					std::vector<char> buffer(file_info.uncompressed_size);
					unzReadCurrentFile(epub, buffer.data(), buffer.size());
					unzCloseCurrentFile(epub);
					string content(buffer.begin(), buffer.end());
					this->ParseXHTML(content);
				}
			}
		} while (unzGoToNextFile(epub) == UNZ_OK);
	}
	unzClose(epub);
}

void Epub::ParseXHTML(const string& content) {
	using namespace tinyxml2;

	XMLDocument doc;
	XMLError result = doc.Parse(content.c_str());
	if (result != XML_SUCCESS) {
		throw "Error: <Parse XHTML>";
	}
	XMLElement* body = doc.FirstChildElement("html")->FirstChildElement("body");
	if (body != nullptr) {
		this->GetTextFromElement(body);
	}
}

void Epub::GetTextFromElement(tinyxml2::XMLElement* element) {
	using namespace tinyxml2;

	if (element == nullptr) return;
	const char* text = element->GetText();
	if (text) {
		string word, wordClear = "";
		istringstream stream(text);
		while (stream >> word) {
			//cout << word << endl;
			for (char c : word) {
				if (isalpha(static_cast<unsigned char>(c))) {
					wordClear += c;
				}
			}
			//cout << worldClear << endl;
			if (wordClear != "") {
				transform(wordClear.begin(), wordClear.end(), wordClear.begin(), ::tolower);
				this->AddAmountEpub(wordClear, 1);
			}
			wordClear = "";
		}
	}
	for (XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
		this->GetTextFromElement(child);
	}
}

void Epub::PrintTop50(const string& bookname) {

	vector<pair<string, int>> words = this->GetAllElements();
	sort(words.begin(), words.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
		return a.second > b.second;
	});
	cout << "\n--Top 50 words of " << bookname << ": " << endl;
	for (int i = 0; i < 50; i++) {
		cout << setw(6) << left << i + 1 << setw(10) << words[i].first << " : " << ANSI_COLOR_GREEN << words[i].second << ANSI_COLOR_RESET << " times" << endl;
	}
}

class BTreeNode {
private:
	vector<int> keys;
	vector<BTreeNode*> childs;
	bool isLeaf;
	int t;
	friend class BTree;
public:

};

class BTree {
private:
	BTreeNode* root;
	int t;
public:
	BTree();

};

int main() {

	HashTable<string, int> a;
	a.Add("Samon", 10);
	a.Add("Damon", 18);
	a.Add("man", 22);
	a.Add("pan", 14);
	a.Add("Samon", 10);
	a.Add("Deyli", 1);
	a.Add("debra", 33);
	a.Add("aron", 98);
	a.ShowStructure();
	a.FindElem("man");
	a.Delete("man");
	a.ShowStructure();
	a.FindElem("nap");

	Epub e;
	e.ExtractEpub("The Godfather - Mario Puzo.epub");
	//e.ShowStructure();
	e.PrintTop50("The Godfather - Mario Puzo.epub");
	e.Clear();
	e.ExtractEpub("Grisha 02 - Siege and Storm - Leigh Bardugo.epub");
	e.PrintTop50("Grisha 02 - Siege and Storm - Leigh Bardugo.epub");
	
	return 0;
}
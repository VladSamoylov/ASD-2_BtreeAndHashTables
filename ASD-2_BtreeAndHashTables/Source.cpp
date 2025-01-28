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
#define ANSI_COLOR_PURPLE "\033[35m"
#define ANSI_COLOR_LIGHTBLUE "\033[94m"

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

/**
 * @brief Деструктор класу зв'язного списку
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 */
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

/**
 * @brief Додавання нового елемента у зв'язний список
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param key Ключ 
 * @param data Дані
 */
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

/**
 * @brief Перевантаження оператора присвоєння для зв'язного списку
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param other Зв'язний список для присвоєння
 * @return Повернення поточного зв'язного списку
 */
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

/**
 * @brief Метод, який шукає за ключем шукає елемент зв'язного списку
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param key Ключ 
 * @return Дані, які містяться у зв'язному списку за ключем
 */
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

/**
 * @brief Метод, який видаляє ноду зв'язного списку за ключем
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param key Ключ
 */
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

/**
 * @brief Метод, який видаляє всі ноди зв'язного списку 
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 */
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

/**
 * @brief Метод, який перевіряє присутність елемента за ключем у зв'язному списку
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param key Ключ
 * @return True - елемент присутній у зв'язному списку, інакше - false
 */
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

/**
 * @brief Метод, який шукає вузол за ключем і інкремірує дані за цим ключем
 * @param key Ключ
 */
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

/**
 * @brief Метод, який повертає розмірність зв'язного списку
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @return Повертає розмірність зв'язного списку
 */
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

/**
 * @brief Метод, який повертає дані за індексом зв'язного списку
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param index Індекс
 * @return Дані, які містяться за індексом
 */
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

/**
 * @brief Метод, який відображує зв'язний список
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 */
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

/**
 * @brief Конструктор за замовчуванням геш-таблиці
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 */
template <typename T, typename Y>
HashTable<T, Y>::HashTable() {

	this->isDivMethod = true;
	this->isChainsCollision = true;
	this->HashTableSize = 5;
	this->data.resize(this->HashTableSize);
	this->ElemCount = 0;
}

/**
 * @brief Метод, який реалізує геш-функцію на основі ділення
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param key Вхідні дані для геш-функції
 * @return Повертає хеш-код
 */
template <typename T, typename Y>
int HashTable<T, Y>::HashFunction(const T& key) {

	return static_cast<int>(key) % this->HashTableSize;
}

/**
 * @brief Метод, який реалізує геш-функцію для ключа типу string
 * @param key Вхідні дані для геш-функції
 * @return Повертає хеш-код
 */
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

/**
 * @brief Метод, який додає новий елемент до хеш-таблиці
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param key Ключ
 * @param data Дані
 */
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

/**
 * @brief Метод, який додає новий елемент до хеш-таблиці із типом ключа string та значення int
 * @param key Ключ
 * @param data Дані
 */
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

/**
 * @brief Метод, який візуалізує внутрішню структуру хеш-таблиці, включно з усіма "кошиками" (buckets) і вмістом зв'язаних списків, що зберігаються в кожному з них
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 */
template <typename T, typename Y>
void HashTable<T, Y>::ShowStructure() {
	
	cout << "_____________________" << endl;
	for (int i = 0; i < this->HashTableSize; i++) {
		cout << "Bucket #" << ANSI_COLOR_RED << i << ANSI_COLOR_RESET << ':' << endl;
		data[i].Show();
	}	
	cout << "_____________________" << endl;
}

/**
 * @brief Метод, який дозволяє отримати всі елементи хеш-таблиці у вигляді вектора пар key-value
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @return Вектор пар key-value
 */
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

/**
 * @brief Метод, який шукає елемент у хеш-таблиці за ключем 
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param key Ключ
 */
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

/**
 * @brief Метод, який видаляє елемент у хеш-таблиці за ключем 
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 * @param key Ключ
 */
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

/**
 * @brief Метод, який очищує хеш-таблицю
 * @tparam T Заповнювач типу T
 * @tparam Y Заповнювач типу Y
 */
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

/**
 * @brief Метод, який розпаковує книгу формату EPUB і працює з його  HTML або XHTML файлами
 * @param filename Шлях до книги формату EPUB
 */
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

/**
 * @brief Метод, який парсить HTML або XHTML і отримує уривки тексту
 * @param content Файли формату HTML або XHTML
 */
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

/**
 * @brief Метод, який отримує з уривків текста слова і додає їх до геш-таблиці
 * @param element Уривок тексту
 */
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

/**
 * @brief Метод, який виводить топ 50 найбільш вживаних слів у вибраній книзі
 * @param bookname Назва вибраной книги
 */
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
	int currentKeyCount;
	friend class BTree;
public:
	BTreeNode(const int& t, const bool& isLeaf) : t(t), isLeaf(isLeaf) {
		this->keys.resize(2 * t - 1);
		this->currentKeyCount = 0;
		this->childs.resize(2 * t, nullptr);
	}
	BTreeNode* NodeSearch(const int&);
	void SplitChild(const int&, BTreeNode*);
	void InsertNonFull(const int&);
	void ShowNodes();
	void RemoveByKey(const int&);
	int SearchKey(const int&);
	void RemoveFromLeaf(const int&);
	void RemoveFromNonLeaf(const int&);
	int GetPred(const int&);
	int GetNext(const int&);
	void MergeNodes(const int&);
	void FillNode(int&);
	void BorrowFromPrev(const int&);
	void BorrowFromNext(const int&);
	void PrintBtree(BTreeNode*, const int&, const string&);
};

/**
 * @brief Метод, який відповідає за рекурсивне відображення всіх ключів дерева B-Tree у впорядкованому вигляді
 */
void BTreeNode::ShowNodes() {

	int i;
	for (i = 0; i < this->currentKeyCount; i++) {
		if (!isLeaf) this->childs[i]->ShowNodes();
		cout << " " << this->keys[i];
	}
	if (!isLeaf) this->childs[i]->ShowNodes();
}

/**
 * @brief Метод, який відображує B-Tree у вигляди Батько-нащадок
 * @param node Вузол для відображення
 * @param lvl Розмір табуляції
 * @param str Рядок для виведення 
 */
void BTreeNode::PrintBtree(BTreeNode* node, const int& lvl, const string& str) {

	if (node == nullptr) return;
	string tab = string(lvl * 4, ' ');
	cout << tab << str << "[";
	for (int i = 0; i < node->currentKeyCount; i++) {
		cout << node->keys[i];
		if (i < node->currentKeyCount - 1) cout << ", ";
	}
	cout << "]" << endl;
	if (!node->isLeaf) {
		for (int i = 0; i <= node->currentKeyCount; i++) {
			this->PrintBtree(node->childs[i], lvl + 1, "Child " + to_string(i) + ": ");
		}
	}
}

/**
 * @brief Метод, який виконує пошук певного ключа в вузлі B-дерева
 * @param key Ключ
 * @return Результат пошуку
 */
BTreeNode* BTreeNode::NodeSearch(const int& key) {

	int i = 0;
	while (i < this->currentKeyCount && key > this->keys[i]) i++;
	if (i < this->currentKeyCount && key == this->keys[i]) return this;
	if (this->isLeaf) return nullptr;

	return this->childs[i]->NodeSearch(key);
}

/**
 * @brief Метод, який виконує розбиття дочірнього вузла в B-дереві
 * @param index Індекс у батьківському вузлі, на який буде вставлений новий дочірній вузол після того, як поточний дочірній вузол (child) буде розбитий
 * @param child Вказівник на дочірній вузол, який буде розбитий.
 */
void BTreeNode::SplitChild(const int& index, BTreeNode* child) {

	BTreeNode* newNode = new BTreeNode(child->t, child->isLeaf);
	int t = child->t;
	newNode->currentKeyCount = t - 1;

	for (int i = 0; i < t - 1; i++) {
		newNode->keys[i] = child->keys[i + t];
	}

	if (!child->isLeaf) {
		for (int i = 0; i < t; i++) {
			newNode->childs[i] = child->childs[i + t];
		}
	}

	child->currentKeyCount = t - 1;
	child->keys.resize(2 * t - 1);
	for (int i = this->currentKeyCount; i >= index + 1; i--) {
		this->childs[i + 1] = this->childs[i];
	}
	this->childs[index + 1] = newNode;

	for (int i = this->currentKeyCount - 1; i >= index; i--) {
		this->keys[i + 1] = this->keys[i];
	}
	this->keys[index] = child->keys[t - 1];
	this->currentKeyCount++;
}

/**
 * @brief Метод, який вставляє ключ в B-дерево, яке не переповнене,
 * @param key Ключ
 */
void BTreeNode::InsertNonFull(const int& key) {

	int i = this->currentKeyCount - 1;

	if (this->isLeaf) {
		while (i >= 0 && key < this->keys[i]) {
			this->keys[i + 1] = this->keys[i];
			i--;
		}
		this->keys[i + 1] = key;
		this->currentKeyCount++;
	}
	else {
		while (i >= 0 && key < this->keys[i]) i--;
		
		if (this->childs[i + 1]->currentKeyCount == 2 * this->t - 1) {
			this->SplitChild(i + 1, this->childs[i + 1]);
			if (key > this->keys[i + 1]) i++;
		}
		this->childs[i + 1]->InsertNonFull(key);
	}
}

/**
 * @brief Метод, який відповідає за видалення ключа з B-дерева
 * @param key Ключ
 */
void BTreeNode::RemoveByKey(const int& key) {

	int index = this->SearchKey(key);

	if (index < this->currentKeyCount && keys[index] == key) {
		if (this->isLeaf) this->RemoveFromLeaf(index);
		else this->RemoveFromNonLeaf(index);
	}
	else {
		if (this->isLeaf) return;
	}

	if (this->childs[index]) {
		bool isLastChild = (index == this->currentKeyCount);

		if (this->childs[index]->currentKeyCount < this->t) {
			this->FillNode(index);
		}

		if (isLastChild && index > this->currentKeyCount) this->childs[index - 1]->RemoveByKey(key);
		else this->childs[index]->RemoveByKey(key);
	}
}

/**
 * @brief Метод, який відповідає за видалення ключа з листового вузла в B-дереві
 * @param index Індекс, який вказує на позицію ключа, який потрібно видалити
 */
void BTreeNode::RemoveFromLeaf(const int& index) {

	for (int i = index + 1; i < this->currentKeyCount; i++) {
		this->keys[i - 1] = this->keys[i];
	}
	this->currentKeyCount--;
}

/**
 * @brief Метод, який відповідає за видалення ключа з внутрінього вузла в B-дереві
 * @param index Індекс, який вказує на позицію ключа, який потрібно видалити
 */
void BTreeNode::RemoveFromNonLeaf(const int& index) {

	int key = this->keys[index];

	if (this->childs[index]->currentKeyCount >= t) {
		int pred = this->GetPred(index);
		this->keys[index] = pred;
		this->childs[index]->RemoveByKey(pred);
	}
	else if (this->childs[index + 1]->currentKeyCount >= t) {
		int next = this->GetNext(index);
		this->keys[index] = next;
		this->childs[index + 1]->RemoveByKey(next);
	}
	else {
		this->MergeNodes(index);
		this->childs[index]->RemoveByKey(key);
	}
}

/**
 * @brief Метод, який  використовуються для отримання найбільшого ключа з лівого  піддерев вузла в B-дереві
 * @param index Індекс в масиві дочірніх вузлів поточного вузла
 * @return Найбільший ключ в лівому піддереві дочірнього вузла на позиції index
 */
int BTreeNode::GetPred(const int& index) {

	BTreeNode* current = this->childs[index];
	while (!current->isLeaf) {
		current = current->childs[current->currentKeyCount];
	}

	return current->keys[current->currentKeyCount - 1];
}

/**
 * @brief Метод, який використовуються для отримання найменшого ключа з правого піддерев вузла в B-дереві
 * @param index Індекс в масиві дочірніх вузлів поточного вузла
 * @return Найменьший ключ в правому піддереві дочірнього вузла на позиції index
 */
int BTreeNode::GetNext(const int& index) {

	BTreeNode* current = this->childs[index + 1];
	while (!current->isLeaf) {
		current = current->childs[0];
	}
	return current->keys[0];
}

/**
 * @brief Метод, який використовується для об'єднання двох дочірніх вузлів B-дерева в один
 * @param index Індекс в масиві дочірніх вузлів поточного вузла
 */
void BTreeNode::MergeNodes(const int& index) {

	BTreeNode* child = this->childs[index];
	BTreeNode* sibling = this->childs[index + 1];
	child->keys[t - 1] = this->keys[index];

	for (int i = 0; i < sibling->currentKeyCount; i++) {
		child->keys[t + i] = sibling->keys[i];
	}
	if (!child->isLeaf) {
		for (int i = 0; i <= sibling->currentKeyCount; i++) {
			child->childs[i + t] = sibling->childs[i];
		}
	}
	for (int i = index + 1; i < this->currentKeyCount; i++) {
		this->keys[i - 1] = this->keys[i];
	}
	for (int i = index + 2; i <= this->currentKeyCount; i++) {
		this->childs[i - 1] = this->childs[i];
	}

	child->currentKeyCount += sibling->currentKeyCount + 1;
	this->currentKeyCount--;

	delete sibling;
}

/**
 * @brief Метод, який відповідає за коригування структури дерева після того, як кількість ключів у дочірньому вузлі стала меншою за мінімально допустиму кількість
 * @param index Індекс дочірнього вузла, у якому кількість ключів стала меншою за мінімально допустиму кількість
 */
void BTreeNode::FillNode(int& index) {

	if (index != 0 && this->childs[index - 1]->currentKeyCount >= this->t) this->BorrowFromPrev(index);
	else if (index != this->currentKeyCount && this->childs[index + 1]->currentKeyCount >= this->t) this->BorrowFromNext(index);
	else {
		if (index != this->currentKeyCount) this->MergeNodes(index);
		else this->MergeNodes(index - 1);
	}
}

/**
 * @brief Метод, який використовуються для того, щоб поточний вузол (дочірній вузол) позичив ключ із попереднього сусіднього вузла
 * @param index Індекс дочірнього вузла, у якому кількість ключів стала меншою за мінімально допустиму кількість
 */
void BTreeNode::BorrowFromPrev(const int& index) {

	BTreeNode* child = this->childs[index];
	BTreeNode* sibling = this->childs[index - 1];

	for (int i = child->currentKeyCount - 1; i >= 0; i--) {
		child->keys[i + 1] = child->keys[i];
	}
	if (!child->isLeaf) {
		for (int i = child->currentKeyCount; i >= 0; i--) {
			child->childs[i + 1] = child->childs[i];
		}
	}
	child->keys[0] = this->keys[index - 1];

	if (!child->isLeaf) {
		child->childs[0] = sibling->childs[sibling->currentKeyCount];
	}

	this->keys[index - 1] = sibling->keys[sibling->currentKeyCount - 1];
	child->currentKeyCount++;
	sibling->currentKeyCount--;
}

/**
 * @brief Метод, який використовуються для того, щоб поточний вузол (дочірній вузол) позичив ключ із наступного сусіднього вузла
 * @param index Індекс дочірнього вузла, у якому кількість ключів стала меншою за мінімально допустиму кількість 
 */
void BTreeNode::BorrowFromNext(const int& index) {

	BTreeNode* child = this->childs[index];
	BTreeNode* sibling = this->childs[index + 1];

	child->keys[child->currentKeyCount] = this->keys[index];

	if (!child->isLeaf) {
		child->childs[child->currentKeyCount + 1] = sibling->childs[0];
	}
	this->keys[index] = sibling->keys[0];

	for (int i = 1; i < sibling->currentKeyCount; i++) {
		sibling->keys[i - 1] = sibling->keys[i];
	}
	if (!sibling->isLeaf) {
		for (int i = 1; i <= sibling->currentKeyCount; i++) {
			sibling->childs[i - 1] = sibling->childs[i];
		}
	}

	child->currentKeyCount++;
	sibling->currentKeyCount--;
}

/**
 * @brief Метод, який використовується для пошуку індексу в масиві ключів поточного вузла, де може знаходитись заданий ключ
 * @param key Ключ
 * @return Індекс в поточному вузлі
 */
int BTreeNode::SearchKey(const int& key) {

	int index = 0;
	while (index < this->currentKeyCount && this->keys[index] < key) {
		index++;
	}

	return index;
}

class BTree {
private:
	BTreeNode* root;
	int t;
public:
	BTree(const int& t) : t(t), root(nullptr) {}
	void BTreeSearch(const int&);
	void BTreeInsert(const int&);
	void BTreeShowNodes();
	void BTreeDeleteByKey(const int&);
	void BTreePrint();
};

/**
 * @brief Метод, який виконує пошук певного ключа в B-дереві
 * @param key Ключ
 */
void BTree::BTreeSearch(const int& key) {
	
	if (this->root->NodeSearch(key)) cout << "The key : " << ANSI_COLOR_GREEN
		<< key << ANSI_COLOR_RESET << " is found in the Btree" << endl;
	else  cout << "The key : " << ANSI_COLOR_RED
		<< key << ANSI_COLOR_RESET << " isn`t found in the Btree" << endl;
}

/**
 * @brief Метод, який виводить ключи В-дерева
 */
void BTree::BTreeShowNodes() {
	
	if (this->root) this->root->ShowNodes();
	else cout << ANSI_COLOR_RED << "BTree is empty" << ANSI_COLOR_RESET << endl;
}

/**
 * @brief Метод, який будує В-дерево
 */
void BTree::BTreePrint() {
	this->root->PrintBtree(this->root, 0, "");
}

/**
 * @brief Метод вставки ключа в В-дерево
 * @param key Ключ для вставки в В-дерево
 */
void BTree::BTreeInsert(const int& key) {
	
	if (!this->root) {
		this->root = new BTreeNode(this->t, true);
		root->keys[0] = key;
		this->root->currentKeyCount = 1;
	}
	else {
		if (this->root->currentKeyCount == 2 * this->t - 1) {
			BTreeNode* newRoot = new BTreeNode(this->t, false);
			newRoot->childs[0] = root;
			newRoot->SplitChild(0, this->root);

			int i = 0;
			if (newRoot->keys[0] < key) i++;
			newRoot->childs[i]->InsertNonFull(key);
			this->root = newRoot;
		}
		else this->root->InsertNonFull(key);
	}
}

/**
 * @brief Метод який видаляє ключ з В-дерева
 * @param key Ключ для видалення з В-дерева
 */
void BTree::BTreeDeleteByKey(const int& key) {
	
	this->root->RemoveByKey(key);

	if (this->root->currentKeyCount == 0) {
		if (!this->root->isLeaf) {
			BTreeNode* oldRoot = this->root;
			this->root = this->root->childs[0];
			delete oldRoot;
		}
	}
}

int main() {

	HashTable<string, int> a;
	Epub e;
	BTree t(3);

	int task = 99, ltask = 99, vtask = 99, dtask = 99;
	int key;
	string sKey;

	while (task != 0) {
		switch (task) {
		case 1:
			while (ltask != 0) {
				switch (ltask) {
				case 1:
					try {
						cout << "Enter key : ";
						cin >> sKey;
						a.Add(sKey, 0);
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 2:
					try {
						cout << "Enter key : ";
						cin >> sKey;
						a.FindElem(sKey);
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 3:
					try {
						cout << "Enter key : ";
						cin >> sKey;
						a.Delete(sKey);
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 4:
					try {
						a.ShowStructure();
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 5:
					try {
						a.Clear();
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				}
				cout << "\n________Work with Hash Table________" << endl;
				cout << setw(35) << left << "Add element to hash table" << " / Enter - 1" << endl;
				cout << setw(35) << left << "Find element" << " / Enter - 2" << endl;
				cout << setw(35) << left << "Delete element" << " / Enter - 3" << endl;
				cout << setw(35) << left << "Show Structure" << " / Enter - 4" << endl;
				cout << setw(35) << left << "Clear hash table" << " / Enter - 5" << endl;
				cout << setw(35) << left << "Back " << " / Enter - 0" << endl;
				if (ltask != 0) {
					try {
						cout << "\nChoose the Task: ";
						cin >> ltask;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							throw "Error: <Incorrect Input Data>";
						}
					}
					catch (const char* err) {
						cerr << endl << err << endl;
						ltask = 99;
					}
				}
			}
			break;
		case 2:
			while (vtask != 0) {
				switch (vtask) {
				case 1:
					try {
						cout << "Enter key: ";
						cin >> key;
						t.BTreeInsert(key);
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 2:
					try {
						cout << "Enter key: ";
						cin >> key;
						t.BTreeDeleteByKey(key);
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 3:
					t.BTreePrint();
					break;
				case 4:
					t.BTreeShowNodes();
					break;
				case 5:
					try {
						cout << "Enter key: ";
						cin >> key;
						t.BTreeSearch(key);
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				}
				cout << "\n________Work with BTree________" << endl;
				cout << setw(45) << left << "Insert key into Btree" << " / Enter - 1" << endl;
				cout << setw(45) << left << "Delete key from Btree" << " / Enter - 2" << endl;
				cout << setw(45) << left << "Show Btree" << " / Enter - 3" << endl;
				cout << setw(45) << left << "Show keys" << " / Enter - 4" << endl;
				cout << setw(45) << left << "Find by key" << " / Enter - 5" << endl;
				cout << setw(45) << left << "Back " << " / Enter - 0" << endl;
				if (vtask != 0) {
					try {
						cout << "\nChoose the Task: ";
						cin >> vtask;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							throw "Error: <Incorrect Input Data>";
						}
					}
					catch (const char* err) {
						cerr << endl << err << endl;
						vtask = 99;
					}
				}
			}
			break;
		case 3:
			while (dtask != 0) {
				switch (dtask) {
				case 1:
					try {
						cout << "Choose the book : " << ANSI_COLOR_GREEN
							<< "The Godfather - Mario Puzo.epub" << ANSI_COLOR_RESET << endl;
						e.ExtractEpub("The Godfather - Mario Puzo.epub");
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 2:
					try {
						e.ShowStructure();
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 3:
					try {
						e.PrintTop50("The Godfather - Mario Puzo.epub");
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 0:
					e.Clear();
					break;
				}
				cout << "\n________Work with Lab Task var 17________" << endl;
				cout << setw(45) << left << "Load book and create Hash Table" << " / Enter - 1" << endl;
				cout << setw(45) << left << "Show structures of Hash Table" << " / Enter - 2" << endl;
				cout << setw(45) << left << "Print top 50 words" << " / Enter - 3" << endl;
				cout << setw(45) << left << "Back " << " / Enter - 0" << endl;
				if (dtask != 0) {
					try {
						cout << "\nChoose the Task: ";
						cin >> dtask;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							throw "Error: <Incorrect Input Data>";
						}
					}
					catch (const char* err) {
						cerr << endl << err << endl;
						dtask = 99;
					}
				}
			}
			break;
		case 0:
			break;
		}
		cout << "\n________________Menu of action________________" << endl;
		cout << setw(35) << left << "Work with Hash Table " << " / Enter - 1" << endl;
		cout << setw(35) << left << "Work with BTree " << " / Enter - 2" << endl;
		cout << setw(35) << left << "Work with Lab Task var 17  " << " / Enter - 3" << endl;
		cout << setw(35) << left << "Exit " << " / Enter - 0" << endl;

		if (task != 0) {
			ltask = 99;
			vtask = 99;
			try {
				cout << "\nChoose the Task: ";
				cin >> task;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					throw "Error: <Incorrect Input Data>";
				}
			}
			catch (const char* err) {
				cerr << endl << err << endl;
				task = 99;
			}
		}
	}

	return 0;
}
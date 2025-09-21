using namespace std;
#define ll long long
#define fl(i,n) for(ll i=0; i<n; i++)
#include <iostream>
#include <vector>
#include <sstream>

struct Node{
public:
    string i;
    Node* next=nullptr;
    Node(string x){this->i=x;}
};

class Stack{
private:
    Node* ptr; // top of stack
    public:
    Stack(){ptr=nullptr;};
    void push(string i);
    bool empty();
    string pop();
    string top();
};


class binaryMinHeap{
    int last; // points to end of heap
    int size; // size of array of heap
    string* a; //array implementation
    bool isDataTime; 
    
    public:
    binaryMinHeap(bool isDataTotalCount);
    void reSize();
    int& index(string file);
    ll getKey(string file);
    void addNode(string file);
    string HeapifyGetMin();
    void modifyKey(string file);
    void read (int num);
};

struct TreeNode{
    int version_id;
    string content;
    string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;
    TreeNode* parent;
    vector<TreeNode* > children;
    public: TreeNode();
};

struct intMap{
    int size=1000;
    vector<vector<pair<int,TreeNode*>>> v; //separate chaining
    intMap(){v.resize(size);};
    int h(int x);
    void insert(int k,TreeNode* val);
    bool count(int k);
    TreeNode*& operator[](int k);
};

class File{
    TreeNode* root;
    intMap version_map;
    TreeNode* active_version;
    public:
    // update these 3 variables accordingly
    int total_versions;
    time_t modified_time;
    int indexInHeap;
    string FILENAME;
    File(string FILENAME);
    string read();
    void insert(string new_content);
    void update(string new_content);
    void addChild(string content);
    void snapShot(string snap_message);
    void snapShot();
    void rollBack(int id);
    void history();
};
struct strMap{
    int size=1000;
    vector<vector<pair<string,File*>>> v;
    strMap(){v.resize(size);};
    int h(string s);
    void insert(string k,File* val);
    bool count(string k);
    File*& operator[](string k);
};

strMap systemFiles;
binaryMinHeap RECENT_FILES(false);
binaryMinHeap BIGGEST_TREES(true);
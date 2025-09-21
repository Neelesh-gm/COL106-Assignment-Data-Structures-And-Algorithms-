#include "headers.hpp"

TreeNode::TreeNode(){
    content="This file is the Root. It doesn't contain any content";
    version_id=0;
    // change msg maybe
    message="";
    created_timestamp = time(NULL);
    snapshot_timestamp = time(NULL);
    this->parent=nullptr;
    
}

File::File(string FILENAME){
    TreeNode* temp = new TreeNode();
    root=temp;
    active_version=temp;
    version_map[0]=temp;
    total_versions=1;
    modified_time=time(NULL);
    this->FILENAME=FILENAME;
    indexInHeap=-1;
}

string File::read(){ return active_version->content; }

void File::insert(string new_content){
    if(active_version->snapshot_timestamp){
        cout << "The current version is a snapshot. Can not \
append/update content. Will create new version.\n";
        if(active_version->message.size()==0){
            cout << "Enter Snapshot message: " << endl;
            string new_message;
            getline(cin, new_message);
            snapShot(new_message);
        }else if(active_version!=root) snapShot();
        addChild(new_content);
    }else active_version->content+=('\n' + new_content);
    modified_time=time(NULL);
    RECENT_FILES.modifyKey(FILENAME);
}

void File::update(string new_content){
    if(active_version->snapshot_timestamp){
        cout << "The current version is a snapshot. Can not \
        append/update content. Will create new version. ";
        if(active_version->message.size()==0){
            cout << "Enter Snapshot message: " << endl;
            string new_message;
            getline(cin, new_message);
            snapShot(new_message);
        }else snapShot();
        addChild(new_content);
    }else active_version->content = new_content;
    modified_time=time(NULL);
    RECENT_FILES.modifyKey(FILENAME);
}

void File::addChild(string content){
    TreeNode* curr= new TreeNode();
    curr->version_id = total_versions++;
    version_map[curr->version_id]=curr;
    curr->content = content;
    curr->message = "";
    curr->created_timestamp = time(NULL);
    curr->snapshot_timestamp=0;
    curr->parent=active_version;
    active_version->children.push_back(curr);
    active_version = curr;
    BIGGEST_TREES.modifyKey(FILENAME);
}

void File::snapShot(string snap_message){
    active_version->snapshot_timestamp = time(NULL);
    active_version->message = snap_message;
}
void File::snapShot(){
    active_version->snapshot_timestamp = time(NULL);
}

void File::rollBack(int id){
    if(id==-1) id=active_version->parent->version_id;
    if(! version_map.count(id)){
        cout << "INVALID ID.";
        return;
    }
    active_version = version_map[id];
}

void File::history(){
    TreeNode* temp=active_version;
    cout << "Version ID \t Created \t Snapshotted \t Message \n";
    if(active_version->snapshot_timestamp){
        cout << active_version->version_id << "\t"
        << active_version->created_timestamp << "\t" <<
        active_version->snapshot_timestamp << "\t" << 
        active_version->message << "\n";
    }
    temp=temp->parent;
    while(temp!=nullptr){
        cout << temp->version_id << "\t" << temp->created_timestamp 
        << "\t" << temp->snapshot_timestamp << "\t" << 
        temp->message << "\n";
        temp=temp->parent;
    }
}

int intMap::h(int x){return x%size;}

void intMap::insert(int k,TreeNode* val){
    int idx=h(k);
    for(auto &p:v[idx]){
        if(p.first==k){p.second=val;return;}
    }
    v[idx].push_back({k,val});
}

bool intMap::count(int k){
    int idx=h(k);
    for(auto &p:v[idx]) if(p.first==k) return 1;
    return 0;
}

TreeNode*& intMap::operator[](int k){
    int idx=h(k);
    for(auto &p:v[idx]) if(p.first==k) return p.second;
    v[idx].push_back({k,nullptr});
    return v[idx].back().second;
}

int strMap::h(string s){
    long long sum=0;
    for(char c:s) sum+=c;
    return sum%size;
}

void strMap::insert(string k,File* val){
    int idx=h(k);
    for(auto &p:v[idx]){
        if(p.first==k){p.second=val;return;}
    }
    v[idx].push_back({k,val});
}

bool strMap::count(string k){
    int idx=h(k);
    for(auto &p:v[idx]) if(p.first==k) return 1;
    return 0;
}

File*& strMap::operator[](string k){
    int idx=h(k);
    for(auto &p:v[idx]) if(p.first==k) return p.second;
    v[idx].push_back({k,nullptr});
    return v[idx].back().second;
}


void Stack::push(string i){
    Node* temp=new Node(i);
    temp->next=ptr;
    ptr=temp;
}

bool Stack::empty(){return (ptr==nullptr);}

string Stack::pop(){
    if(empty()) return "";
    string temp=ptr->i;
    ptr=ptr->next;
    return temp;
}

string Stack::top(){return ptr->i;}

binaryMinHeap::binaryMinHeap(bool isDataTotalCount){
    // tree can hold 16 files initially
    isDataTime= (!isDataTotalCount);
    a= new string[15];
    last=0;
    size=15;
}

void binaryMinHeap::reSize(){
    string* temp = new string[2*size - 1];
    fl(i,last) temp[i]=a[i];
    delete[] a;
    a=temp;
    size=(2*size-1);
}

int& binaryMinHeap::index(string file){
    return (systemFiles[file]->indexInHeap);
}

ll binaryMinHeap::getKey(string file){
    if(isDataTime) return (static_cast<long long>(
        systemFiles[file]->modified_time));
    else return (static_cast<long long>(systemFiles[file]->total_versions));
}

void binaryMinHeap::addNode(string file){
    if(last==size) reSize();
    int i=last++;
    while(i > 0 && getKey(file) < getKey(a[(i-1)/2])){
        a[i]=a[(i-1)/2];
        index(a[i])=i;
        i=((i-1)/2);
    }
    a[i]=file;
    index(file)=i;
}

string binaryMinHeap::HeapifyGetMin(){
    if(last==0){cout << "HEAP IS EMPTY. CREATE FILES TO CONTINUE"; return "";}
    string currMin=a[0];
    string root=a[--last];
    ll key=getKey(root);
    int i=0;
    while(i<last){
        int l=2*i+1, r=2*i+2;
        if(r<last) l = (getKey(a[l])<getKey(a[r]) ? l:r);
        else if(l>=last) break;
        if(key>getKey(a[l])){
            a[i]=a[l];
            index(a[i])=i;
            i=l;
        }else break;
    }
    a[i]=root;
    index(root)=i;
    return currMin;
}

void binaryMinHeap::read(int num){
    Stack ans;
    if(last==0){
        cout << "HEAP IS EMPTY";
        return;
    }
    if(num>last) num=last;
    while(num--) ans.push(HeapifyGetMin());
    while(!ans.empty()){
        cout << ans.top() << " ";
        addNode(ans.pop());
    }
}

void binaryMinHeap::modifyKey(string file) {
    if (last==0) return;
    int i=index(file);
    ll key=getKey(file);

    while (i>0 && key < getKey(a[(i-1)/2])) {
        a[i]=a[(i-1)/2];
        index(a[i])=i;
        i=(i-1)/2;
    }
    a[i]=file;
    index(file) = i;

    while (true){
        int l=2*i+1;
        int r=2*i+2;
        int smallest=i;
        if (l < last && getKey(a[l]) < getKey(a[smallest])) smallest=l;
        if (r < last && getKey(a[r]) < getKey(a[smallest])) smallest=r;
        if (smallest!=i){
            swap(a[i], a[smallest]);
            index(a[i])=i;
            index(a[smallest])=smallest;
            i=smallest;
        }else break;
    }
}

int main(){
    cout << "Valid Commands: \n \
    CREATE <filename> \n \
    READ <filename> \n \
    INSERT <filename> <content> \n \
    UPDATE <filename> <content> \n \
    SNAPSHOT <filename> <message> \n \
    ROLLBACK <filename> [versionID] \n \
    HISTORY <filename> \n";
    cout << "Press Ctrl + C to break out from the terminal anytime\n";
    while(true){
        cout << endl << "Enter Operation to perform: " << endl;
        string input;
        getline(cin, input);
        istringstream ss(input);
        string cmd, filename;
        ss >> cmd >> filename;
        if(cmd=="RECENT_FILES" || cmd=="BIGGEST_TREES"){
            istringstream sma(input);
            int num; ss >> input >> num;
            if(cmd=="RECENT_FILES") RECENT_FILES.read(num);
            else BIGGEST_TREES.read(num);
        }else if(cmd=="CREATE"){
            systemFiles[filename] = new File(filename);
            BIGGEST_TREES.addNode(filename);
            RECENT_FILES.addNode(filename);
        }else if(!systemFiles.count(filename)) cout << "INVALID FILENAME";
        else if(cmd=="READ"){
            cout << (systemFiles[filename]->read());
        }else if(cmd=="INSERT"){
            string tert; ss >> tert;
            systemFiles[filename]->insert(input.substr(input.find(tert)));
        }else if(cmd=="UPDATE"){
            string tert; ss >> tert;
            systemFiles[filename]->update(input.substr(input.find(tert)));
        }else if(cmd=="SNAPSHOT"){
            string tert; ss >> tert;
            systemFiles[filename]->snapShot(input.substr(input.find(tert)));
        }else if(cmd=="ROLLBACK"){
            string tert; ss >> tert;
            int id=-1;
            if(tert.size()!=0) id=stoi(tert);
            systemFiles[filename]->rollBack(id);
        }else if(cmd=="HISTORY"){
            systemFiles[filename]->history();
        }else cout << "INVALID COMMAND";
    }
}
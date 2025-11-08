#include<bits/stdc++.h>
using namespace std;

class node{
    public:
    node*left;
    node*right;
    char data;
    int freq;
    node(char data,int freq){
        this->data=data;
        this->freq=freq;
        this->left=NULL;
        this->right=NULL;
    }

};

class comp{
    public:
    bool operator()(node* &a,node* &b){
        return a->freq>b->freq;
    }   
};

class haffman{
    node*root=NULL;
    unordered_map<char,string>haffman_codes;
    
    public:
    void built_tree(string text){
        unordered_map<char,int>mp;
        for(auto ch:text){
            mp[ch]++;
        }
        priority_queue<node*,vector<node*>,comp>pq;
        for(auto it:mp){
            pq.push(new node(it.first,it.second));
        }

        while(pq.size()>1){
            node*l=pq.top();
            pq.pop();
            node*r=pq.top();
            pq.pop();
            int sum=l->freq;
            if(r!=NULL){
                sum+=r->freq;
            }
            node*r_node=new node('\0',sum);
            r_node->left=l;
            r_node->right=r;
            pq.push(r_node);
        }
        root=pq.top();
        generatecode(root,"");
    }


    void generatecode(node*root,string code){
        if(root==NULL)return;
        if(root->left==NULL&&root->right==NULL){
            haffman_codes[root->data]=code;
            return;
        }
        generatecode(root->left,code+"0");
        generatecode(root->right,code+"1");
    }


    string encode(string text){
        string ans;
        for(char ch:text){
            ans+=haffman_codes[ch];
        }
        return ans;
    }


    string decode(string text){
        string ans;
        node*temp=root;
        for(char bit:text){
            if(bit=='0'){
                temp=temp->left;
            }
            else{
                temp=temp->right;
            }

            if(temp->left==NULL&&temp->right==NULL){
                ans+=temp->data;
                temp=root;
            }
        }
        return ans;

    }

    void printcodes(){
        for(auto  it :haffman_codes){
            if(it.first==' '){
                cout<<"SPACE    : "<<it.second<<endl;
            }
            else{
                cout<<it.first<<" : "<<it.second<<endl;
            }
        }
    }


};


int main(){

    string input;
    cout<<"ENTER string : ";
    getline(cin,input);
    haffman h;
    h.built_tree(input);

    cout<<"Table : "<<endl;
    h.printcodes();
    cout<<"\n\n";
    
    cout<<"Enter text to encode : ";
    string enc;
    getline(cin,enc);
    string encode_str=h.encode(enc);
    cout<<"Encoded string : "<<encode_str<<endl;
    cout<<"\n\n";

    string dec;
    cout<<"Enter string to decode : ";
    getline(cin,dec);
    string decode_str=h.decode(dec);
    cout<<"Decoded string : "<<decode_str<<endl;
    
    return 0;
}
作者：是萌萌的前辈呀
链接：https://www.nowcoder.com/discuss/212003?type=0&order=0&pos=44&page=2
来源：牛客网

1.赋值操作符（2种写法）
CMyString CMyString::operator=(const CMyString &str)
{
  if(this==&str)
  return *this;
  delete []m_pData;
  m_pData=NULL;
  m_pData=new char[strlen(str.m_pData)+1];
  strcpy(m_pData,str.m_pData);
  return *this;
}
 
CMyString& CMyString::operator=(const CMyString &str)
{
  if(this!=&str)
  {  CMyString strTemp(str);  Char* pTemp=strTemp.m_pData;  strTemp.m_pData=m_pData;  m_pData=pTemp;
  }
  return *this;
}
 
2.智能指针
shared_ptr实现：
template<typename T>
class SmartPtr
{
  public:
    SmartPtr(T *p=0):pointee(p),count(new size_t(t)) {}
    SmartPtr(const SmartPtr& rhs):pointee(rhs.pointee),count(rhs.count)
    {++*count;}
    ~SmartPtr() {decr_count();}
    SmartPtr& operator=(const SmartPtr& rhs)
    {
      ++*rhs.count;
      decr_count();
      pointee=rhs.count;
      return *this;
    }
    T* operator->() {return pointee;}
    const T* operator->() const {return pointee;}
    T& operator*() {return *pointee;}
    const T& operator*() {return *pointee;}
    size_t get_refCount() {return *count;}
  private:
    T* pointee;
    size_t *count;
    void decr_count()
    {
      if(--*count==0)
      {
        delete pointee;
        delete count;
      }
    }
};
 
auto_ptr实现：
template<class T>  
class auto_ptr
{  
public:  
    explicit auto_ptr(T *p = 0): pointee(p) {}  
    auto_ptr(auto_ptr<T>& rhs): pointee(rhs.release()) {}  
    ~auto_ptr() { delete pointee; }  
    auto_ptr<T>& operator=(auto_ptr<T>& rhs)  
    {  
        if (this != &rhs) reset(rhs.release());  
        return *this;  
    }  
    T& operator*() const { return *pointee; }  
    T* operator->() const { return pointee; }  
    T* get() const { return pointee; }  
    T* release()  
    {  
        T *oldPointee = pointee;  
        pointee = 0;  
        return oldPointee;  
    }  
    void reset(T *p = 0)  
    {  
        if(pointee != p)
        {  
          delete pointee;  
          pointee = p;  
        }  
    }  
private:  
    T *pointee;  
};  
 
3.strcpy/memmove/memcpy
void *memmove(void* dis,const void* src,size_t n)
{
  assert(dst!=NULL&&src!=NULL);
  char* p=(char*) dst;
  const char* s=(const char*) src;
  if(p>=s&&p<=s+n)
  {
    p=p+n-1;
    s=s+n-1;
    while(n--)
      *p--=*s--;
  }
  else
  {
    while(n--)
      *p++=*s++;
  }
  return dst;
}
 
void* memcpy(void* dst,const void* src,size_t n); //不处理内存折叠
 
4.strcat
char* strcat(char* dst, const char* src)
{
    char* ret = dst;
    while(*dst != '\0')
        ++dst;
    while((*dst++ = *src++) != '\0');
    return ret;
}
 
5.strcmp
int strcmp(const char* str1, const char* str2)
{
    while(*str1 == *str2 && *str1 != '\0')
    {
        ++str1;
        ++str2;
    }
    return *str1 - *str2;
}
 
6.单例模式
class singleton
{
  public:
    static singleton* getInstance();
  private:
    singleton() {}
    static singleton* instance;
};
singleton* singleton::instance=new singleton;
singleton* singleton::getInstance()
{
  return instance;
}
 
7.堆排序
void heapAdjust(int R[],int l,int h)
{  int i=l;  int j=2*i;  int temp=R[i];  while(j<=h)  {  if(j<h&&R[j]<R[j+1])
        j++;  if(temp<R[j])  {  R[i]=R[j];  i=j;  j=2*i;
      }
      else
       break;
  }
  R[i]=temp;
}
 
void heapSort(int R[],int n)
{  for(int i=n/2-1;i>=0;i--)  heapAdjust(R,i,n-1);   //初始建堆  for(int j=n-1;j>=1;j--)  {  swap(R[1],R[j]);  heapAdjust(R,0,j-1)
  }
}
 
8.快排
int partition(int R[],int l,int r)
{  int tmp=R[l];  int i,j;  i=l,j=r;  while(i<j)  {  While(R[j]>tmp&&j>i) j--;  R[i]=R[j];  While(R[i]<tmp&&i<j) i++;  R[j]=R[i];  }  R[i]=tmp;  return i;
}
递归：
void quickSort(int R[],int l,int r)
{
  if(l<r)
  {
    int mid=partition(R,l,r);
    quickSort(R,l,mid-1);
    quickSort(R,mid+1,r);
  }
}
 
非递归：
void quickSort(int R[],int l,int r)
{  stack<int> st;  if(l<r)  {  int mid=partition(R,l,r);  if(mid-1>l)  {  st.push(l);  st.push(mid-1);
    }
    if(mid+1<r)
    {  st.push(mid+1);  st.push(r);
    }
    while(!st.empty())
    {  int end=st.top();st.pop();  int start=st.top();st.pop();  int kmid=partition(R,start,end);  if(kmid-1>start)  {  st.push(start);  st.push(kmid-1);
      }
      if(kmid+1<end)
      {  s.push(kmid+1);  s.push(end);
      }
    }
  }
}
 
9.Top K
void findk(int R[],int l,int h,int k)
{
  int p=partition(R,l,h);
  if(p==k-1)
  {
    for(int i=0;i<k;i++)
      cout<<R[i]<<" ";
  }
  else if(p>k-1)
    findk(R,l,p-1,k);
  else
    findk(R,p+1,h,k);
}
 
10.二路归并
void merge(vector<int>&nums,int l1,int r1,int l2,int r2)
{  int i=l1;  int j=l2;  int n=(r1-l1+1)+(r2-l2+1);  vector<int> temp(n);  int k=0;  while(i<=r1&&j<=r2)  {  if(nums[i]<=nums[j])  temp[k++]=nums[i++];  else temp[k++]=nums[j++];
}
while(i<=r1)
 temp[k++]=nums[i++];
while(j<=r2)
 temp[k++]=nums[j++];
for(int i=0;i<n;i++)  nums[l1+i]=temp[i];
}
void mergeSort(vector<int>&nums,int start,int end)
{  if(start<end)  {  int mid=(start+end)>>1;  mergeSort(nums,start,mid);  mergeSort(nums,mid+1,end);  merge(nums,start,mid,mid+1,end);
  }
}
 
11.冒泡
void BubbleSort(int R[],int n)
{  bool flag;  for(int i=0;i<n-1;i++)  {  flag=false;  for(int j=0;j<n-i-1;j++)  {  if(R[j]>R[j+1])  {  swap(R[j],R[j+1]);  flag=true;
      }
    }
    if(!flag)
     return;
  }
}
 
12.直接选择
void SelectSort(int R[],int n)
{  int k;  for(int i=0;i<n;i++)  {  k=i;  for(int j=i+1;j<n;j++)  {  if(R[k]>R[j])  k=j;
    }
  }
  swap(R[i],R[k]);
}
 
13.有序链表合并
非递归：
Node* mergeList(Node* p1,Node* p2)
{
  Node* p=new Node(0);
  Node* ret=p;
  while(p1&&p2)
  {
    if(p1->val<=p2->val)
    {
      p->next=p1;
      p1=p1->next;
    }
    else
    {
      p->next=p2;
      p2=p2->next;
    }
    p=p->next;
  }
  if(p1)
    p->next=p1;
  if(p2)
    p->next=p2;
  return ret->next;
}
 
递归：
Node* mergeList(Node* p1,Node* p2)
{
  if(p1==NULL)
    return p2;
  else if(p2==NULL)
    return p1;
  Node* pMergeHead=NULL;
  if(p1->val<p2->val)
  {
    pMergeHead=p1;
    pMergeHead->next=mergeList(p1->next,p2);
  }
  else
  {
    pMergeHead=p2;
    pMergeHead->next=mergeList(p1,p2->next);
  }
  return pMergeHead;
}
 
14.二叉树先序遍历（非递归版）
方法1：
vector<int> preorder(TreeNode *root)
{   if(!root)
            return vector<int>();
        stack<TreeNode*> st;
        st.push(root);
        vector<int> ret;
        while(st.size())
        {
            TreeNode* p=st.top();
            ret.push_back(p->val);
            st.pop();
            if(p->right)
                st.push(p->right);
            if(p->left)
                st.push(p->left);
        }
        return ret;
}
 
方法2：
void preorder(TreeNode *root)   
{
    stack<TreeNode*> s;
    TreeNode *p=root;
    while(p!=NULL||!s.empty())
    {
        while(p!=NULL)
        {
            cout<<p->data<<" ";
            s.push(p);
            p=p->left;
        }
        if(!s.empty())
        {
            p=s.top();
            s.pop();
            p=p->right;
        }
    }
}
 
15.二叉树中序遍历（非递归）
vector<int> inorder(TreeNode* root)
{  if (!root)  return vector<int>();  vector<int> ans;  stack<TreeNode*> st;  TreeNode* cur = root;  while (cur || !st.empty())  {  while (cur)  {  st.push(cur);  cur = cur->left;  }  if (!st.empty())  {  cur = st.top();  ans.push_back(cur->val);  st.pop();  cur = cur->right;  }  }  return ans;
}
 
16.二叉树后序（非递归）
vector<int> postorder(TreeNode *root)
{
  if(!root)
      return vector<int>();
  vector<int> ret;   
  stack<TreeNode*> st;
  st.push(root);
  while(!st.empty())
  {
      TreeNode* tmp=st.top();
      st.pop();
      ret.push_back(tmp->val);
      if(tmp->left)
          st.push(tmp->left);
      if(tmp->right)
          st.push(tmp->right);
  }
  reverse(ret.begin(),ret.end());
  return ret;    
}
 
17.二叉树层次遍历
vector<vector<int>> printLevel(TreeNode* root)
{
  if(!root)
    return vector<vector<int>>();
  vector<vector<int>> ans;
  queue<TreeNode*> que;
  que.push(root);
  while(!que.empty())
  {
    vector<int> tmp;
    int qsize=que.size();
    for(int i=0;i<qsize;i++)
    {
      TreeNode* p=que.front();
      tmp.push_back(p->val);
      que.pop();
      if(p->left)
        que.push_back(p->left);
      if(p->right)
        que.push_back(p->right);
    }
    ans.push_back(tmp);
  }
  return ans;
}
 
18.判读是否为同一棵树
bool isEqual(TreeNode* p1,TreeNode* p2)
{
  if(p1==NULL&&p2==NULL)
    return true;
  if(p1==NULL||p2==NULL)
    return false;
  return (p1->val==p2->val&&isEqual(p1->left,p2->left)
          &&isEqual(p1->right,p2->right));
}
 
19.平衡二叉树的判断
方法1：
int getDepth(TreeNode* root)
{
  if(!root)
    return 0;
  return max(getDepth(root->left),getDepth(root->right))+1;
}
bool isBlancedTree(TreeNode* root)
{
  if(!root)
    return true;
  int left=getDepth(root->left);
  int right=getDepth(root->right);
  if(abs(left-right)>1)
    return false;
  return isBlancedTree(root->left)&&isBlancedTree(root->right);
}
 
方法2：
bool isBlancedTree(TreeNode* root,int &depth)
{
  if(!root)
  {
    depth=0;
    return true;
  }
  int left=0,right=0;
  if(isBlancedTree(root->left,left)&&isBlancedTree(root->right,right))
  {
    if(abs(left-right)<=1)
    {
      depth=1+max(left,right);
      return true;
    }
  }
  return false;
}
 
20.树的子结构判断
bool isSubTree(TreeNode* root1,TreeNode* root2)
{
  if(!root2)
    return true;
  if(!root1)
    return false;
  if(root1->val!=root2->val)
    return false;
  return isSubTree(root1->left,root2->left) && isSubTree(root1->right,root2->right);
}
bool hasSubTree(TreeNode* root1,TreeNode* root2)
{
  if(!root1 || !root2)
    return false;
  bool flag=false;
  if(root1->val==root2->val)
    flag=isSubTree(root1,root2);
  if(flag)
    return flag;
  return hasSubTree(root1->left,root2) || hasSubTree(root1->right,root2);
}
 
21.左旋字符串
abcdefg->cdefgab  2位
void reverseStr(string &str,int l,int h)
{
  for(int i=l,j=h;i<=l+(h-l)/2;++i,j--)
    swap(str[i],str[j]);
}
string leftRotateString(string str,int n)
{
  int len=str.size();
  if(len<=0)
    return "";
  if(n==0)
    return str;
  reverse(str,0,n-1);
  reverse(str,n,len-1);
  reverse(str,0,len-1);
}
 
22.字符串逆序（递归）
void reverse(string &s,int l,int r)
{
  if(l>=r)
    return;
  swap(s[l],s[r]);
  reverse(s,l+1,r-1);
}
 
字符串逆序打印
void reversePrint(string s,int i)
{
  if(s[i]!='\0')
  {
    reversePrint(s,i+1);
    cout<<s[i];
  }
}
 
23.二叉树中和为某一值的路径
vector<vector<int>> ans;
void findPathStack(TreeNode* root,int expectNumber,int sum,vector<int> vec)
{
  sum+=root->val;
  vec.push_back(root->val);
  if(!root->left&&!root->right)
  {
    if(sum==expectNumber)
      ans.push_back(vec);
  }
  if(root->left)
    findPathStack(root->left,expectNumber,sum,vec);
  if(root->right)
    findPathStack(root->right,expectNumber,sum,vec);
}
vector<vector<int>> findPath(TreeNode* root,int expectNumber)
{
  if(!root)
    return ans;
  vector<int> vec;
  findPathStack(root,expectNumber,0,vec);
  return ans;
}
 
24.二叉搜索树与双向链表
void convertNode(TreeNode* root,TreeNode*& pre)
{
  if(!root)
    return;
  if(root->left)
    convertNode(root->left);
  root->left=pre;
  if(pre)
    pre->right=root;
  if(root->right)
    convertNode(root->right);
}
TreeNode* convert(TreeNode* root)
{
  TreeNode* pre=NULL;
  convert(root,pre);
  while(pre&&pre->left)
    pre=pre->left;
  return pre;
}
 
25.KMP   O(m+n)
         0 1 2 3 4 5 6 7 8
str:     A B A B A B A B B
substr:  A B A B A B B
next:   -1 0 0 1 2 3 4
 
i=6处不匹配，str、substr的0-5处相同，此时 j=next[j]=4意思指：0-5之间str的2-5与substr的0-3匹配，下一步只需将j=4以后元素与i依次比较
 
int kmp(string str,string substr,int next[])
{
  int i=0,j=0;
  while(i<str.size()&&j<substr.size())
  {
    if(str[i]==substr[j])
    {
      ++i;
      ++j;
    }
    else
    {
      j=next[j];
      if(j==-1)
      {
        j=0;
        ++i;
      }
    }
  }
  if(j==substr.size())
    return i-substr.size();
}
 
void getNext(string subStr,int next[])
{
  int i=0,j=-1;
  next[0]=-1;
  while(i<subStr.size())
  {
    if(j==-1||subStr[i]==subStr[j])
    {
      ++i;
      ++j;
      next[i]=j;
    }
    else
      j=next[j];
  }
}
 
26.字符串的全排列
void permutation(string s,int l,int h)
{
  if(l==h)
  {
    cout<<s<<endl;
    return;
  }
  for(int i=l;i<=h;i++)
  {
    swap(s[i],s[l]);
    permutation(s,l+1,h);
    swap(s,l+1,h);
  }
}
 
用dfs:
void perm_dfs(string s,int i,vector<char> &path,vector<int> vis)
{
  if(i==s.size())
  {
    for(auto c:path)
      cout<<c<<" ";
    cout<<endl;
    return;
  }
  for(int k=0;k<s.size();k++)
  {
    if(vis[k]==0)
    {
      path.push_back(s[k]);
      vis[k]=1;
      perm_dfs(s,i+1,path,vis);
      vis[k]=0;
      path.pop_back();
    }
  }
}
 
27.组合
void combination(char* str,int num,vector<char> &ret)
{
  if(num==0)
  {
    for(auto c:ret)
      cout<<c;
    cout<<endl;
    return;
  }
  if(*str=='\0')
    return;
  ret.push_back(*str);
  combination(str+1,num-1,ret);  //把当前字符放到组合中，从剩下的n-1个字符中选取m-1个字符
  ret.pop_back();
  combination(str+1,num,ret); //不放入该字符，从剩下的n个字符中选取m-1个字符
}
 
void combination(char* str)
{
  if(str==NULL)
    return;
  int len=strlen(str);
  vector<char> ret;
  for(int i=1;i<=len;i++)
    combination(str,i,ret);
}
 
28.二叉树的镜像
递归版：
void Mirror(TreeNode* root)
{
  if(!root)
    return;
  if(!root->left&&!root->right)
    return;
  TreeNode* ptemp=root->left;
  root->left=root->right;
  root->right=ptemp;
  if(root->left)
    Mirror(root->left);
  if(root->right)
    Mirror(root->right);
}
 
非递归版:
void Mirror(TreeNode* root)
{
  if(root==NULL)
    return;
  stack<TreeNode*> st;
  st.push(root);
  while(st.size())
  {
    TreeNode* tree=st.top();
    st.pop();
    if(tree->left||tree->right)
    {
      TreeNode* ptmep=tree->left;
      tree->left=tree->right;
      tree->right=ptemp;
    }
    if(tree->left)
      st.push(tree->left);
    if(tree->right)
      st.push(tree->right);
  }
}
 
29.字典树
(大致框架)
struct TrieNode
{
  TrieNode* next[26];
  bool exist;
};
TrieNode* createTrieNode()
{
  TrieNode* node=new TrieNode();
  memeset(node->next,0,sizeof(node->next));
  node->exist=false;
  return node;
}
 
void TrieInsert(TrieNode* root,string word)
{
  TrieNode* p=root;
  for(auto c:word)
  {
    int id=c-'a';
    if(p->next[id]==NULL)
      p->next[id]=createTrieNode();
    p=p->next[id];
  }
  p=p->exist=true;
}
 
int TrieSearch(TrieNode* root,string word)
{
  int ans=0;
  TrieNode* p=root;
  for(auto c:word)
  {
    int id=c-'a';
    p=p->next[id];
    if(p==NULL)
      break;
    if(p->exist)
        ans++;
  }
  return ans;
}
 
31.并查集
关于连通图的例子示意，有多个城市，有些城市之间有道路连接，问至少还需要几条路使得任意城市之间都可以到达
 
int find(int x)
{
  int r=x;
  while(r!=pre[r])
    r=pre[r];
  int i=x,j;
  while(pre[i]!=r)   //路径压缩
  {
    j=pre[i];
    pre[i]=r;
    i=j;
  }  
  return r;
}
 
void mix(int x,int y)
{
  int fx=find(x);
  int fy=find(y);
  if(fx!=fy)
    pre[fy]=fx;
}
 
int main()
{
  int n,m,a,b,i,j,ans;   //n为城市个数，m为路的条数
  while(scanf("%d%d",&n,&m)&&n)
  {
    for(i=1;i<=N;i++)
      pre[i]=i;
    for(i=1;i<=M;i++)
    {
      scanf("%d%d",&a,&b);
      mix(a,b);
    }
    memset(t,0,sizeof(t));
    for(i=1;i<N;i++)
      t[find(i)]=1;
    for(ans=0,i=1;i<=N;i++)
      if(t[i])
        ans++;
    printf("%d\n",ans);    
  }
}
 
32.sqrt(n)
牛顿迭代法
double sqrt(double a)
{
  double x=a,y=0;
  while(fabs(x-y)>0.00001)
  {
    y=x;
    x=0.5*(x+a/x);
  }
  return x;
}
 
二分法：
int mySqrt(int x)
{
  if(x<=1)
      return x;
  long long h=x/2;
  long long l=2;
  long long mid=(h+l)>>1;
  while(l<mid)
  {
      if(mid*mid>x)
          h=mid;
      else if(mid*mid<x)
          l=mid;
      else
          return mid;
      mid=(h+l)>>1;
 
  }
  return mid;
}
 
33.洗牌算法
void shuffle(int *a,int n)
{
  for(int i=n-1;i>0;i--)
  {
    srand((unsigned)time(NULL));
    swap(a[i],a[rand()%(i+1)]);
  }
}
 
34.单链表快排
ListNode* getpartions(ListNode* pbeg,ListNode* pend)
{
  int k=pbeg->val;
  ListNode* p=pbeg;
  ListNode* q=p->next;
  while(q!=pend)
  {
    if(q->val<k)
    {
      p=p->next;
      swap(p->val,q->val);
    }
    q=q->next;
  }
  swap(p->val,pbeg->val);
  return p;
}
void quickSort(ListNode* pbeg,ListNode* pend)
{
  if(pbeg!=pend)
  {
    ListNode* partitions=getpartions(pbeg,pend);
    quickSort(pbeg,partition);
    quickSort(partition->next,pend);
  }
}
 
quickSort(head,NULL);
 
35.反转链表
ListNode* reverseList(ListNode* pHead)
{
  if(!pHead||!pHead->next)
    return pHead;
  ListNode* p=pHead->next,*pre=pHead;
  while(p)
  {
    pre->next=p->next;
    p->next=pHead;
    pHead=p;
    p=p->next;
  }
  return pHead;
}
 
递归版
ListNode* reverseList(ListNode* pHead)
{
  if(!pHead||!pHead->next)
    return pHead;
  ListNode* pReverseNode=reverseList(pHead->next);
  pHead->next->next=pHead;
  pHead->next=NULL;
  return pReverseNode;
}
 
36.数据流中的中位数
将数据平均分配到最小堆、最大堆中
维持大顶堆中的数<=小顶堆中的数，且两者的个数相等或差1
 
priorty_queue<int,vector<int>,less<int>> maxHeap;
priorty_queue<int,vector<int>,greater<int>> minHeap;
int cnt=0;
void insert(int num)
{
  if(cnt%2==0)
  {
    if(maxHeap.size()>0&&num<maxHeap.top())
    {
      maxHeap.push(num);
      num=maxHeap.top();
      maxHeap.pop();
    }
    minHeap.push(num);
  }
  else
  {
    if(minHeap.size()>0&&minHeap.top()<num)
    {
      minHeap.push(num);
      num=minHeap.top();
      minHeap.pop();
    }
    maxHeap.push(num);
  }
  cnt++;
}
 
int getMedian()
{
  if(cnt%2==0)
    return (minHeap.top()+maxHeap.top())/2;
  return minHeap.top();
}
 
37.和为s的连续正数序列
vector<vector<int>> findContinuousSeq(int sum)
{
  vector<vector<int>> ret;
  int l=1,h=2;
  while(l<h)
  {
    int cur=(h+l)*(h-l+1)/2;
    if(cur<sum)
      h++;
    if(cur==sum)
    {
      vector<int> tmp;
      for(int i=l;i<=h;i++)
        tmp.push_back(i);
      ret.push_back(tmp);
      l++;
    }
    if(cur>sum)
      l++;
  }
  return ret;
}
 
38.正则表达式匹配
例：aaa与a.a和ab*ac*a匹配，与aa.a及ab*a不匹配
bool match(char* str,char* pattern)
{
  if(pattern[0]==0&&str[0]==0)
    return true;
  if(pattern[0]!=0&&pattern[1]=='*')
  {
    if(match(str,pattern+2))
      return true;
  }
  if((pattern[0]=='.'&&str[0])||str[0]==pattern[0])
  {
    if(match(str+1,pattern+1))
      return true;
    if(pattern[1]=='*'&&match(str+1,pattern))
      return  true;
  }
  return false;
}
 
39.链表中环的入口节点
ListNode *meetingNode(ListNode* head)
{
  if(!head)
    return NULL;
  ListNode* pSlow=head->next;
  if(!pSlow)
    return NULL;
  ListNode* pFast=pSlow->next;
  while(pFast&&pSlow)
  {
    if(pFast==pSlow)
        return pFast;
    pSlow=pSlow->next;
    pFast=pFast->next;
    if(pFast)
      pFast=pFast->next;
  }
  return NULL;
}
 
ListNode* entryNodeOfLoop(ListNode* head)
{
  ListNode* meetingNode=meetingNode(head);
  if(!meetingNode)
    return NULL;
  int circle=1;
  ListNode* p1=meetingNode;
  while(p1->next!=meetingNode)
  {
    p1=p1->next;
    circle++;    
  }
  p1=head;
  for(int i=0;i<circle;i++)
    p1=p1->next;
  ListNode* p2=head;
  while(p1!=p2)
  {
    p1=p1->next;
    p2=p2->next;
  }
  return p1;  
}
 
40.和为s的两个数字
递增排序的数组
vector<int> twoSum(vector<int> arr,int sum)
{
  vector<int> result;
  int length=arr.size();
  int start=0;
  int end=length-1;
  while(start<end)
  {
    if(arr[start]+arr[end]==sum)
    {
      result.push_back(arr[start]);
      result.push_back(arr[end]);
    }
    else if(arr[start]+arr[end]<sum)
      start++;
    else
      end--;
  }
  return result;
}
 
41.N皇后
bool isvalid(int R[],int i,int j)
{
  for(int k=0;k<i;k++)
  {
    if(j==R[k]||abs(k-i)==abs(R[k]-j))
      return false;
  }
  return true;
}
int process(int R[],int i,int n)
{
  if(i==n)
    return 1;
  int res=0;
  for(int j=0;j<n;j++)
  {
    if(isValid(R,i,j))
    {
      R[i]=j;
      res+=process(R,i+1,n);
    }
  }
  return res;
}
 
42.序列化和反序列化二叉树
char* serialize(TreeNode* root)
{
  if(!root)
    return "#";
  string s=to_string(root->left);
  s+=",";
  char* left=serialize(root->left);
  char* right=serialize(root->right);
  char* ans=new char[strlen(left)+strlen(right)+s.size()];
  strcpy(ans,s.c_str());
  strcat(ans,left);
  strcat(ans,right);
  return ans;
}
 
char* rstr;
TreeNode* Deserialize(char* str)
{
  rstr=str;
  if(*rstr=='#')
  {
    rstr++;
    return NULL;
  }
  int val=0;
  while(*rstr!=',')
  {
    val=val*10+*rstr-'0';
    rstr++;
  }
  rstr++;
  TreeNode* root=new TreeNode(val);
  root->left=Deserialize(rstr);
  root->right=Deserialize(rstr);
  return root;
}
 
43.未排序正数数组中累加和为给定值的最长子数组长度
arr:1,2,1,1,1  k=3
返回:3
int getMaxLength(int arr[],int n,int k)
{
  if(n<=0||k<=0)
    return 0;
  int l=0,r=0;
  int sum=arr[0];
  int len=0;
  while(r<n)
  {
    if(sum==k)
    {
      len=max(len,r-l+1);
      sum-=arr[l++];
    }
    else if(sum<k)
    {
      r++;
      if(r==n)
        break;
      sum+=arr[r];
    }
    else
      sum-=arr[l++];
  }
}
 
44.未排序数组中累加和为给定值的最长子数组系列问题
a.无序数组arr,元素可正、可负、可0，给定一个整数k
int maxLength(int arr[],int n,int k)
{
  if(n==0)
    return 0;
  unordered_map<int,int> map;
  map[0]=-1;
  int len=0;
  int sum=0;
  for(int i=0;i<n;i++)
  {
    sum+=arr[i];
    if(map.find(sum-k)!=map.end())
      len=max(len,i-map[sum-k]);
    if(map.find(sum)==map.end())
      map.put(sum,i);
  }
  return len;
}
 
b.求arr所有子数组中正数与负数个数相等的最长子数组长度
方法：整数全部变成1，负数全部变成-1,0不变，然后求累加和为0的最长子数组长度即可
 
c.求arr中所有的子数组中0和1个数相等的最长子数组长度
方法：0变成-1,1不变，求累加和为0的最长子数组长度即可
 
45.动态规划相关
背包问题系列
最长递增子序列
最长公共子序列
最长公共子串

# C/C++ Questions
> C/C++面试题

---

## Q：面向对象的特征

-  抽象

（1）数据抽象（类的属性）

（2）过程抽象（类的方法）

- 封装

（1）封装过程与数据

（2）访问只能通过特定的界面，如私有变量，用get/set方法访问

- 继承

（1）一个新类可以从现有类中派生（鼓励类重用）

（2）<font color=red>为实现多态性做准备</font>

- 多态

（1）方法的重载（静态多态）

（2）函数模板（静态多态）

编译时由编译器决定类型，```  template<class T> ```

（3）动态绑定

（4）方法的重写


## Q：C/C++ 内存区

- **C++：五个区**

（1）堆

（2）栈

（3）自由存储区

（4）全局/静态存储区（for 全局/静态变量）

（5）常量存储区（存放常量）

- **C：四个区**

（1）堆

（2）栈

（3）全局/静态存储区

（4）常量存储区

Notice：常量存储区：不占内存，与代码段在一个区

Notice：静态存储区：在内存空间中

**const 变量在C和C++中存储位置不一样**

（1）在C中：栈

（2）在C++中：常量存储区

## Q：const

- **默认状态下，const对象只在文件内有效**

如果想在多个文件之间共享const对象，必须在变量的定义和声明之前加extern关键字。

- **const 和类型的前后顺序可变**

```
const int i = 0;
int const j = 1;
```

- **对const引用时，引用的类型必须与其所引用的对象一致**

```
const int ci = 1024;
const int &r = ci;
```

- **初始化常量类型的引用时，可以用表达式或变量**

此时，不允许通过常量类型的引用去改变其绑定的值。

```
int i = 24;
const int &r = i;
const int &r2 = r * 2;
```

```
double dval = 3.14;
const int &r = dval;

// 实际上编译器的做法是：
const int temp = dval;
const int &r = temp;
```

- **要存放常量对象的地址，只能用指向常量的指针**

```
const double pi = 3.14;
const double *r = &pi;
```

同时类似的，指向常量的指针允许指向非常量对象。

- **summary**

指向常量的指针，或常量类型的引用都不会改变与他们相关（绑定）的对象的值。

Notice：指针是对象，但引用不是。

- **常量指针(const pointer)**

```
int a = 0;
int *const pa = &a; // 指向变量a的常量指针
const double b = 3.14;
const double *const pb = &b; // 指向双精度常量b的常量指针
```

- **顶层const和底层const（top-level const and low-level const）**

对于指针：顶层const是指指针本身是否是常量，底层const是指指针指向的对象是否是常量。

对于变量：只有顶层const，比如 const int ci = 0;

白话：顶层const更加关注自身，顶级重要！

他们的作用：（参考：https://blog.csdn.net/qq_19528953/article/details/50922303 ）

为啥非要区分顶层const和底层const呢，根据C++primer的解释，区分后有两个作用。

1. 执行对象拷贝时有限制，常量的底层const不能赋值给非常量的底层const。也就是说，你只要能正确区分顶层const和底层const，你就能避免这样的赋值错误。下面举一个例子：

```
int num_c = 3;
const int *p_c = &num_c;  //p_c为底层const的指针
//int *p_d = p_c;  //错误，不能将底层const指针赋值给非底层const指针
const int *p_d = p_c; //正确，可以将底层const指针复制给底层const指针
```

2. 使用命名的强制类型转换函数const_cast时，需要能够分辨底层const和顶层const，因为const_cast只能改变运算对象的底层const。下面举一个例子：

```
int num_e = 4;
const int *p_e = &num_e;
//*p_e = 5;  //错误，不能改变底层const指针指向的内容
int *p_f = const_cast<int *>(p_e);  //正确，const_cast可以改变运算对象的底层const。但是使用时一定要知道num_e不是const的类型。
*p_f = 5;  //正确，非顶层const指针可以改变指向的内容
cout << num_e;  //输出5
```

- **constexpr和常量表达式**

const expression 是指值不会改变，且在编译过程中就能得到计算结果的表达式。

C++11新规定：允许将变量声明为constexpr类型，以便编译器检查变量的值是否是一个常量表达式。 多用constexpr有好处。

```
constexpr int mf = 20; // 20是常量表达式
constexpr int limit = mf + 1; // mf + 1 是常量表达式
constexpr int sz = size(); // 当 size 为 constexpr 函constexpr数时，这行正确
```

- **指针和constexpr**

1. constexpr指针的初始值必须是nullptr，0或者某个存储于固定地址中的对象（一般是定义于函数体外的对象）。
2. constexpr只对指针本身有效，与指针所指向的对象无关。

```
const int *p = nullptr; // p 是一个指向常数类型的指针。
constexpr int *q = nullptr; // q 是一个指向整数的常量指针，constexpr把指针对象设置为顶层const。

（类似 int *const q = nullptr）

```

- **constexpr 函数**

指能用于常量表达式的函数。要求：函数的返回值以及所有形参都得是字面值类型，并有且只有一条 return 语句。编译时，编译器会用相应的结果值替换对 constexpr 函数的调用。为了能在编译过程中随时展开，constexpr 函数被隐式地指定为内联函数。

例外：我们其实会允许 constexpr 函数的返回值并非一个常量。但这不应被用。

```
constexpr int new_sz(){ return 42; }
constexpr size_t scale(size_t cnt){ return new_sz() * cnt }
int arr[scale(2)]; // 正确
int i = 2;
int a1 = scale(i); // 返回值是一个非常量表达式（不确定该行是否有错）
int a2[scale(i)]; // 错误，无法用非常量表达式初始化一个数组
```



- **类型别名与const**

1. C++11中可以用 using 代替 typedef

```
using SI = Sales_item;
SI item;
// 等价于 typedef Sales_item SI;

using int_array = int[4];
typedef int int_array[4]; // 等价的 typedef

```

2. typedef 与 *

```
typedef char *pstring; // pstring 是 char* 指针
const pstring cstr = 0; // cstr 是 char* 常量指针
char *const dstr = 0; // 该行与上行有一样的效果

const char *estr = 0; // const char* 的指针，指向常量字符
```



## Q：auto 类型说明符（C++11）

- **auto 类型在初始化时，会忽略掉顶层const**
```
const int ci = 0;
auto b = ci; // b 为 int 类型（省略了顶层const）
const auto cb = ci; // cb 为 const int 类型
auto c = &ci; // c 为指向整形常量的指针（保留了底层const）
```

但是！auto在碰到引用时，初始值中的顶层const属性仍然保留。

```
auto &g = ci; // g 是一个整形常量引用
auto &h = 42; // 报错，不能为非常量引用绑定到字面值（常量表达式）
const auto &j = 42; // 正确
```

一条语句定义多个变量：

```
// & 和 * 从属于某个声明符，auto 指的是基本数据类型
auto k = ci, &l = i; // k 是整形，l 是整形引用。
```

- **用 auto 简化 for 循环：**

```
vector<int> v = {0, 1, 2, 3, 4, 5};

for(auto &r : v){
  r *= 2;
}

// 与上述等价的传统写法
for(vector<int>::iterator beg = v.begin(), end = v.end(); beg != end; ++beg){
  vector<int>::iterator &r = *beg;
  r *= 2;
}

// 半简化
for(auto beg = v.begin(), end = v.end(); beg != end; ++beg){
  auto &r = *beg;
  r *= 2;
}
```


## Q：左值与右值

左值：指那些求值结果作为对象或函数的表达式。

右值：（一般而言）指那些结果是值的表达式。

summary：当一个对象被用作右值的时候，用的是对象的值（内容）；当对象被用作左值的时候，用的是对象的身份（在内存中的位置）

1. 赋值运算符左侧为左值，得到的结果也仍然是一个左值。

2. 取地址符作用于一个左值对象，返回一个指向该运算对象的指针（为右值）。

3. 解引用运算符*，下标运算符[]的求值结果都是左值。

4. 对于 decltype，如果表达式的结果是左值，decltype得到一个引用类型。


## Q：decltype（C++11）

从表达式推断出要定义的变量的类型，但不想用该表达式的值。

decltype（declared type）：C++11引入的第二种类型说明符，能返回操作数的数据类型。

```
decltype(f()) sum = x; // sum 的类型就是函数 f 的返回类型
const int ci = 0, &cj = ci;
decltype(ci) x = 0; // x: const int
decltype(cj) y = x; // y: const int&
decltype(cj) z; // 错误，z是一个引用，必须初始化

// decltype 的结果可以是引用类型
int i = 42, *p = &i, &r = i;
decltype(r + 0) b; // 正确，加法结果是左值，int，所以 b 也是 int 类型
decltype(*p) c; // 错误，*p 返回左值，所以 c 是 int& 类型，必须初始化
decltype((i)) d; // 错误，(i) 作为表达式，是左值，所以 d 是 int& 类型，必须初始化
decltype(i) e; // 正确，i 在这作为右值，所以 d 是 int 类型
decltype(r) f = i; // 正确，f 为 int& 类型，只有在 decltype 中，引用不是作为所指对象的同义词
```

## Q：无符号，有符号数混合运算

有符号数会先转换为无符号数，再进行相应的算术运算。

string::size_type 为无符号数，因此如果在一条表达式中已经有了 size() 函数，就不要再用 int 了，这样可以避免混用 int 和 unsigned int 可能带来的问题，比如：

s.size() < n // n 为负值 int，比如 -1，在表达式中 n 会被自动转换为一个巨大的无符号整数。


## Q：函数返回值

- **main 函数返回值**

int main 在没有写 return 时，编译器将隐式地插入一条 return 0;

除了 0 之外：

```
#include <cstdlib>

int main(){
  if(some_failure)
    return EXIT_FAILURE; // 定义在 cstdlib 头文件中
  else
    return EXIT_SUCCESS;
}
```

- **返回指针的函数**

普通方式：
```
// type (*function(parameter_list))[dimension]
int (*func(int i))[10]; // func 外面的括号必须有，否则将返回指针的数组
// 理解：
// func(int i) 没有疑问
// (*func(int i)) 表示我们可以对函数的返回结果执行解引用操作
// (*func(int i))[10] 表示解引用返回值后，将得到一个大小为 10 的数组
// int (*func(int i))[10] 表示解引用完的数组是 int 类型
```

别名方式：
```
typedef int arrT[10]; // 声明 arrT 是一个含有 10 个整数的数组
using arrT = int[10]; // 与上行等价，C++11

arrT *func(int i); // func 返回一个指向 10 个整数的数组的指针
```

More advanced 方式：（骚气地运用 auto）
```
// 尾置返回类型(trailing return type)，对于复杂函数很有用!
auto func(int i) -> int (*)[10];
```

奇怪的方式：（运用 decltype）
```
int odd[] = {1,2,3,4};
decltype(odd) *arrPtr(int i){
  return &odd;
}
```


## Q：列表初始化 (C++11)

```
vector<string> articles = {"a", "an", "the"}; // 正确的列表初始化
vector<string> v1("a"); // 错误，不能用字符串字面量构建 vector 对象
vector<int> ivec(10, -1); // 初始化 10 个 int 类型的 -1
vector<string> svec(10, "hi"); // 初始化 10 个 string 类型的 hi

long double ld = 3.1415926;
int a{ld}, b = {ld}; // 错误：列表初始化不执行类型转换，存在丢失信息的风险
int c(ld), d = ld; // 正确，默认赋值会执行类型转换
```

- **列表也可作为赋值语句的右侧运算对象：**

```
vector<int> vi; // 初始化为空
vi = {0, 1, 2, 3, 4, 5};
```

- **列表也可以作为函数的返回值**

```
vector<string> process(){
  if(...){
    return {}; // 空 vector 对象
  }
  else if(...){
    return {"function", "okay"}; //  返回列表初始化的 vector 对象
  }
  ...
}
```

- **列表初始化容器**

```
// 隐含指定了容器的大小
list<string> authors = {"Milton", "Shakespeare", "Austen"};
vector<const char*> articles = {"a", "an", "the"};
// 还能用迭代器进行初始化
forward_list<string> words(articles.begin(), articles.end());

```


## Q：迭代器

```
vector<int>::iterator it1;
string::iterator it2;

vector<int>::const_iterator it3; // it3 只能读取元素，不能修改元素
string::const_iterator it4; // it4 同理
```

C++11 引入 cbegin() 和 cend()，用来获取 const_iterator：

```
auto it = v.cbegin(); // it 为 vector<int>::const_iterator
// 还有另一种：逆序迭代器
auto rit = v.crbegin();
auto rit2 = v.crend();
```

Notice：任何一种可能改变 vector 对象容器的操作，比如添加元素 push_back，都会使该 vector 对象的迭代器失效。不仅仅是 vector，这条规则对任何容器都适用。

## Q：begin 和 end 函数（C++11引入的标准库函数）

```
int ia[] = {0, 1, 2, 3, 4, 5};
int *beg = begin(ia); // 指向首元素的指针
int *last = end(ia); // 指向尾元素的下一位置的指针，但该指针不能执行解引用或递增操作
```

## Q：C++11 遍历数组方式

```
int ia[3][4];
// p 指向含有 4 个整数的数组，等价于 int (*p)[4] = ia;
for(auto p = ia; p != ia + 3; ++p){
  // q 指向 4 个整数数组的首元素，也就是 q 指向一个整数
  for(auto q = *p; q != *p + 4; ++q){
    cout << *q << ' ';
  }
  cout << endl;
}

```

## Q：initializer_list 形参（C++11）

如果函数的实参数量未知，但是全部实参的类型都相同，可以用 initializer_list。

```
initializer_list<T> lst{a, b, c, ...}; // lst 中的元素是对应初始值的副本，列表中的元素是 const 类型

lst2 = lst; // 因此拷贝或赋值是浅拷贝，共享元素

lst.size();
lst.begin();
lst.end();
```

- **one more thing**

省略符形参是为了便于C++程序访问某些特殊的C代码而设置的，用了varargs的C标准库功能。

通常省略符形参不用于其他目的。且总在形参列表的最后一个位置。

```
void foo(parm_list, ...); // 对于前面定义的形参的实参会执行正常的类型检查，省略符形参对应的实参无须类型检查

void foo(...);
```

## Q：C++11 运算符

1. 取余

取余 % 操作的运算对象必须是整数类型。

m 总是与 (m/n)*n+m%n 相同。

C++11中，取余运算中，如果 m%n 不等于 0，则它的符号和 m 相同。

2. 除法

C++11 新标准中规定：商无论正负都是向 0 取整（即：直接切除小数部分）。

除法运算中，如果两个运算对象的符号相同则为正，否则为负。

3. sizeof 运算符

重点：sizeof 并不实际计算其运算对象的值，因此无效指针，解引用无效指针都没事。sizeof 不需要计算也能知道他们的类型。

sizeof (X)，计算类型/对象的 size。

sizeof expr，计算表达式/对象的 size，返回的是结果类型的大小。

```
class_a data, *p;
sizeof(class_a);
sizeof data;
sizeof p;
sizeof *p; // p 所指类型的空间大小

sizeof class_a::revenue; // C++11新规定：可以用作用域运算符来获取类成员
```

1. 对引用类型执行 sizeof，得到被引用对象所占空间的大小。

2. 对指针执行 sizeof，得到指针本身所占空间的大小。

3. 对数组执行 sizeof，得到整个数组所占空间的大小。

```
// 获取数组 ia 的元素数量
constexpr size_t sz = sizeof(ia) / sizeof(*ia);

// sizeof 返回的是一个常量表达式，可以用来初始化数组的维度
int arr2[sz];
```

4. 对 string/vector 对象执行 sizeof，只会返回该类型固定部分的大小，不会计算对象中的元素占用了多少空间。


## Q：内联函数

将函数指定为内联函数（inline），即在函数前面加上 inline 修饰符，通常就是将它在每个调用点上“内联地”展开，即把函数内容替换到调用处，消除了调用开销。

当然，内联符号只是向编译器发出一个请求，编译器可以选择忽略这个请求。

用法：内联机制用于优化规模较小、流程直接、频繁调用的函数。

PS：很多编译器都不支持内联递归函数！

## Q：=default（C++11）

对于类的构造函数，我们可以用如下的方式将其定义为默认构造函数：

```
struct Sales_data{
  Sales_data() = default; // 定义在类内则是内联的，在类外则是非内联的
  Sales_data( ... ){
    ...
  }
  ...
};
```

## Q：class 和 struct 的区别

使用class和struct定义类的唯一区别就是默认的访问权限。

1. 在 struct 中，定义在第一个访问权限说明符之前的成员是 public 的。
2. 在 class 中，定义在第一个访问权限说明符之前的成员是 private 的。

因此，当希望定义的类的所有成员都是 public 时，可以用 struct。

## Q：友元函数

使得类可以允许其他类或者函数访问它的非共有（private）成员。

```
class Sales_data{
  // 为 Sales_data 的非成员函数做友元声明，友元声明只能在类的内部，最好是开头/结尾
  friend std::istream &read(std::istream&, Sales_data&);
  firend std::ostream &print(std::ostream&, const Sales_data&);

private:
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};
// 在类外声明该友元函数，有些编译器并未强制，但最好加
std::istream &read(std::istream&, Sales_data&);
std::ostream &print(std::ostream&, const Sales_data&);
```

## Q：构造函数深入讨论

- **学会用构造函数初始值列表：一种更高效的初始化方式**

下面是一种构造函数的写法，但是效率并不是最好。因为如果没有在构造函数初始值列表中显式地初始化成员，则该成员将在构造函数之前执行默认初始化，然后才会在函数内被赋值。
```
Sales_data::Sales_data(const string &s, unsigned cnt, double price){
  bookNo = s;
  units_sold = cnt;
  revenue = cnt * price;
}
```

因此，有的时候我们必须用构造函数初始值列表来初始化成员。比如：成员是 const 或者引用的时候，只有一次机会进行初始化，还有类似的，如果成员的是类，且该类类型没有定义默认构造函数时。
```
class ConstRef{
public:
  ConstRef(int ii);
private:
  int i;
  const int ci;
  int &ri;
}
// 必须如下初始化
ConstRef::ConstRef(int ii): i(ii), ci(ii), ri(i) {}
```

Notice：构造函数初始值列表中的初始值的前后顺序不会影响实际的初始化顺序。实际顺序是它们在类定义中的顺序。

Notice：如果一个构造函数的所有参数都提供了默认实参，则它实际上也定义了默认构造函数。


- **委托构造函数**

```
class Sales_data{
public:
  // 非委托
  Sales_data(std::string s, unsigned cnt, double price):
        bookNo(s), units_sold(cnt), revenue(cnt * price) {}
  // 其余构造函数都是委托
  Sales_data(): Sales_data("", 0, 0) {}
  Sales_data(std::string s): Sales_data(s, 0, 0) {}
  Sales_data(std::istream &is): Sales_data() {read(is, *this); }
}
```

- **转换构造函数（隐式的类类型转换）**

通过一个实参调用的构造函数，定义一条从构造函数的参数类型到类类型的隐式转换规则。（这种转变只支持一步）

```
string null_book = "9-99999-99";
item.combine(null_book); // 用一个string实参自动创建了一个Sales_data的临时对象
Sales_data item2 = null_book; // 拷贝初始化时也可以隐式转换
```


Notice：可以抑制隐式转换，在构造函数前声明为 explicit，只对一个实参的构造函数有效，而且只能在类内中用。需要多个实参的构造函数默认不能用于执行隐式转换。
```
explicit Sales_data(std::istream&); // 显式构造函数
```
且，explicit构造函数只能用于直接初始化，不能拷贝初始化。

- **constexpr 构造函数**

一般的构造函数不能是 const 的，但是字面值常量类的构造函数可以是 constexpr 函数。

字面值常量类：

1. 所有数据成员都必须是字面值类型

2. 类必须含有一个 constexpr 构造函数

3. 使用默认的析构函数

4. 类内初始值是常量表达式

## Q：文件流 fstream

```
// file1 都被截断，即清空内容，但不删除文件
ofstream out("file1"); // 隐含以 out 模式打开并截断
ofstream out2("file1", ofstream::out); // 隐含截断
ofstream out3("file1", ofstream::out | ofstream::trunc);
// 为了保留文件内容，我们必须显式指定 app 模式
ofstream app("file2", ofstream::app); // 隐含输出模式
ofstream app2("file2", ofstream::out | ofstream::app);

// 保留被 ofstream 打开的文件中已有的数据的唯一方法是显式指定 app 或 in 模式
```


## Q：静态成员的类内初始化

```
class Account{
public:
  ...

private:
  static constexpr int period = 30; // 此时，静态成员必须是字面值常量类型的 constexpr

}
```

## Q：顺序容器

1. string 和 vector 将元素保存在连续的内存空间中，所以访问很快，在中间位置插入/删除很慢。

2. list 和 forward_list 的设计目的是在任何位置的插入和删除都很快，代价是不支持随机访问，只能遍历。且额外内存开销也比较大

3. deque 支持快速随机访问，在中间位置插入/删除开销大，但在两端很快。

4. forward_list 的设计目的是与最好的手写单向链表达到相当的性能。

5. array

```
// 初始化需要类型和元素数量
array<int, 42> a1;
array<int, 10> a2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
array<int, 10> a3 = {42}; // a3[0] 为 42，其余元素为 0
// 可以直接拷贝
array<int, 10> a4 = a2; // 要求元素类型和大小均一样
```

- **向顺序容器添加元素**

除了array之外，所有标准库容器都提供灵活的内存管理（动态）。

```
//
// forward_list 有自己专门的 insert, emplace
//
c.push_back(t);
c.emplace_back(arg);
c.push_front(t);
c.emplace_front(arg);
c.insert(p, t); // 在迭代器 p 所指向的元素之前插入元素，返回新添加元素的迭代器
c.emplace(p, arg);
//
c.insert(p, n, t); // 插入 n 个 t 元素，返回新添加的第一个元素的迭代器
c.insert(p, b, e); // 将迭代器 b 和 e 指定范围内的元素插入，返回第一个新元素迭代器
c.insert(p, il); // 将列表插入 ...
```

Notice：向一个vector,string,deque中插入元素会使得所有指向容器的迭代器，引用和指针失效。

- **C++11 引入了三个新成员：emplace_front, emplace, emplace_back**

```
// 调用 emplace_back 时，会在容器管理的内存空间中直接创建对象
c.emplace_back("978-0590353402", 25， 15.99);
// 而调用 push_back 则会创建一个局部临时对象，并将其压入容器中
c.push_back(Sales_data("978-0590353402", 25， 15.99));
```

- **访问容器元素**

访问成员函数返回的是引用（包括 front, back, [], at）。
```
c.front();
c.back();
c[n];
c.at(n);
```

- **删除容器元素**

```
// forward_list 有自己专门的 erase
//
c.pop_back();
c.pop_front();
c.erase(p); // 删除迭代器 p 所指定的元素
c.erase(b, e); // 删除迭代器 b 到 e 范围内的元素，返回最后被删的后面的迭代器，如果 e 本身就是尾，则返回尾迭代器
c.clear(); // 删除 c 中所有元素
```

- **vector的扩容**

因为 vector 元素内存连续，因此如果在原位置扩容失败，会分配新的内存空间来保存已有元素和新元素，将已有元素从旧位置移动过来。

vector 扩容时会分配一些预留空间，一般的策略是倍增。

```
// shrink_to_fit 只适用于 vector, string, deque
// capacity, reverse 只适用于 vector, string
c.shrink_to_fit(); // 将 capacity 减少到 size()，但也并不保证
c.capacity(); // 不重新分配内存的话，c 能保存多少个元素
c.reverse(n); // 分配至少能容纳 n 个元素的内存空间，reverse 不会减小 capacity
```

- **string的额外操作**

```
string s(cp, n); // s 拷贝了 cp 数组的前 n 个字符
string s(s2, pos2); // s 拷贝了 s2 从下标 pos2 开始到结尾的字符
string s(s2, pos2, len2); // s 拷贝了 s2 从下标 pos2 开始长度为 len2 的字符，最多拷贝到结尾
//
s.substr(pos, n); // 返回一个 string，包含 s 从下标 pos 开始的长度为 n 的字符的拷贝。
s.substr(pos); // n 的默认值为 0，所以拷贝到结尾
//
s.erase(s.size() - 5, 5); // 删除 s 最后 5 个字符
//
const char *cp = "Stately, plump Buck";
s.assign(cp, 7); // s == "Stately"
s.insert(s.size(), cp + 7); // 插入从 cp + 7 开始到结尾的字符
s.insert(0, s2, 0, s2.size()); // 在 s[0] 之前插入 s2[0] 开始的 s2.size() 个字符
//
s.append("haha"); // 在结尾插入
s.replace(11, 3, "5th"); // 从下标 11 开始，删除 3 个元素，并插入 5th
```

string 的搜索函数
```
string name("AnnaBelle");
// find 函数是最简单的搜索，找到第一个匹配位置的下标
auto pos1 = name.find("Anna"); // pos1 == 0
//
string numbers("0123456789"), name("r2d2");
auto pos = name.find_first_of(numbers); // 查找与给定字符串中任何一个字符匹配的位置
// 类似的还有
auto pos1 = name.find_first_not_of(numbers);
// 家族：
s.find_first_of(args);
s.find_last_of(args);
s.find_first_not_of(args);
s.find_last_not_of(args);
s.rfind(); // 逆向搜索

// args 可以指定开始位置
s.find(s, pos); // 从 pos 开始找
s.find(s, pos, n); // 从 pos 开始找前 n 个字符

```
string 的 compare 函数

```
// 类似 C 中的 strcmp
s.compare(pos1, n1, s2, pos2, n2);
```

## Q：Lambda 表达式

表示一个可调用的代码单元，可以看成一个未命名的内联函数。

格式：[capture list] (parameter_list) -> return type {function body}

可以省略为：[capture list] {function body}

Notice1：lambda 不接受默认参数

Notice2：捕获列表只用于局部非 static 变量，lambda 可以直接使用局部 static 变量和它所在函数之外声明的名字（如 cout）。

```
// 此处忽略了返回类型，lambda 将根据返回值推断出返回类型，如果没有 return，则为 void
auto f = [] { return 42; };
```

```
// 按长度排序，长度相同的单词维持字典序
stable_sort(words.begin(), words.end(),
            [](const string &a, const string &b)
              { return a.size() < b.size(); });
```

```
// 捕获局部变量
[sz](const string &a)
    { return a.size() >= sz; }
//
// 可以用 bind 来重写
bool check_size(const string &s, string::size_type sz){
  return s.size() >= sz;
}
auto wc = find_if(words.begin(), words.end(),
                  bind(check_size, _1, sz));
```

```
// 自定子 find_if，找到第一个 size() >= sz 的元素的迭代器
auto wc = find_if(words.begin(), words.end(),
                [sz](const string &a)
                    { return a.size() >= sz; });
```

Notice：当以引用方式捕获一个变量时，必须保证在 lambda 执行时变量是存在的。

- 隐式捕获（C++是全世界最棒的语言！！），不能同时使用隐式值捕获和隐式引用捕获
```
// os 显式引用捕获，c 隐式值捕获
for_each(words.begin(), words.end(),
        [=, &os](const string &s) { os << s << c; });
```

- 可变 Lambda

```
auto f = [v1]() mutable { return ++v1; }; // 可以改变局部变量 v1 的值
auto f = [&v1] { return ++v1; } // 用引用也可以改变
```

- 当lambda函数体内包含不止一个return语句时，编译器就不能推断其返回类型

```
// 此时必须指定返回类型
auto f = [](int i) -> int {if(i < 0) return -i; else return i; };
```


## Q：bind 函数（C++11）

可以绑定函数，实现嵌套

```
auto g = bind(f, a, b, _2, c, _1);

// 调用 g
g(x, y);
// 映射为
f(a, b, y, c, x);
```

bind 使用引用，必须使用标准库 ref 函数来返回一个对象，包含给定的引用，类似的还有 cref（针对 const）

ref 定义在头文件 functional 中
```
for_each(words.begin(), words.end(),
          bind(print, ref(os), _1, ' '));
```


## Q：新标准库的性能

## Q：free 底层实现



## Q：构造函数/析构函数可以是虚函数吗

## Q：引用的底层实现


## Q：指针和引用的区别

参考：https://www.cnblogs.com/heyonggang/p/3250194.html

指针和引用的区别？

(1)引用在创建时必须初始化，指针可以不初始化，引用不可以为NULL指针可以。
(2)不存在指向空值的引用，但是存在指向空值的指针。
(3)引用初始化后不能被改变，指针可以改变所指的对象.

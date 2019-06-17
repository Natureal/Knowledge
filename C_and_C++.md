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

他们的作用：（参考：https://blog.csdn.net/qq_19528953/article/details/50922303）

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
constexpr int sz = size(); // 当 size 为 constexpr 函数时，这行正确
```

- **指针和constexpr**

1. constexpr指针的初始值必须是nullptr，0或者某个存储于固定地址中的对象（一般是定义于函数体外的对象）。
2. constexpr只对指针本身有效，与指针所指向的对象无关。

```
const int *p = nullptr; // p 是一个指向常数类型的指针。
constexpr int *q = nullptr; // q 是一个指向整数的常量指针，constexpr把指针对象设置为顶层const。

```

- **类型别名与const**

1. C++11中可以用 using 代替 typedef

```
using SI = Sales_item;
SI item;
// 等价于 typedef Sales_item SI;
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


## Q：C++11 列表初始化

```
vector<string> articles = {"a", "an", "the"}; // 正确的列表初始化
vector<string> v1("a"); // 错误，不能用字符串字面量构建 vector 对象
vector<int> ivec(10, -1); // 初始化 10 个 int 类型的 -1
vector<string> svec(10, "hi"); // 初始化 10 个 string 类型的 hi

long double ld = 3.1415926;
int a{ld}, b = {ld}; // 错误：列表初始化不执行类型转换，存在丢失信息的风险
int c(ld), d = ld; // 正确，默认赋值会执行类型转换
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


## Q：free 底层实现

## Q：引用的底层实现

## Q：指针和引用的区别

参考：https://www.cnblogs.com/heyonggang/p/3250194.html

指针和引用的区别？

(1)引用在创建时必须初始化，指针可以不初始化，引用不可以为NULL指针可以。
(2)不存在指向空值的引用，但是存在指向空值的指针。
(3)引用初始化后不能被改变，指针可以改变所指的对象.

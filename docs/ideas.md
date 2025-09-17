### Compiler 思路与实现
#### Part 1：任务
Compiler semantic check 部分可端到端的抽象为阅读parser部分产生的AST，进行必要的检查，填充scope的内容，最终返回包含一切IR阶段需要信息的Scope与AST。  
具体实现如下内容：  
1. Types相关：  
   1. symbols collection of self-define types
   2. members collection of self-define types
   3. implement the following info of the type from its impl blocks:
       - signature of methods   
       - signature of associated   functions   
       - const values  
       - traits implemented  
2. Trait相关：需要impl的对象（const, function），用于之后对应impl块的检查。
3. Const相关：
   1. collect symbols of const.
   2. solve type and value of const, which could be used many times.
4. Func相关：collect signatures，之后用于检查。
5. Semantic杂项：
   1. type checking
   2. mutability checking
   3. context checking of return, break, continue and exit().
#### Part 2: 实现思路
在采用后序遍历整棵AST为一趟的前提下，对于以上任务而言，我将存在前向声明（后向依赖）的任务剖分到不同的趟中，即如果任务A需要用到任务B在AST后序下更靠后的位置产生的某些信息，就将两个任务分成两趟完成，B在前一趟产生的内容就可以被A运用。  

总共分为以下四趟：
1. Type and const collection: 收集type, const对应symbol（包括impl中const）,同时将
对应AST节点与之关联，为第二pass做准备。
2. Type and const solution: 处理所有type的成员类型，求取所有const的类型与value, 实现方式为遍历AST，访问尚未被solve的type与const symbol，并通过递归的方式按需处理type与const.
3. Funcion and Trait collection: 收集所有function，以及type的impl块中的method, associated func，同时收集trait的名称以及对应的restriction.
4. Semantic checking: 处理所有余下的问题：type match, mutability checking, context checking 以及 trait and impl match.
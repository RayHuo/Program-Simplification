## 简介
main函中有三个宏，其分别对应这`theoryFiles`文件夹中三个文档的理论实现和测试。
-MAXU   : 实现`/theroyFiles/MaxU.pdf`中第三部分描述的计算一个符合条件的最大的U。
-GRST   : 实现`/theroyFiles/Simplifying2014.pdf`中给出的计算NLP和DLP的greatest reliable set的算法。
-GRS_WS : 实现`/theroyFiles/ijcai15_simplifying.pdf`中第6部分给出的计算NLP和DLP的greatest strong/weak reliable set的算法。

注意：`/theroyFiles/Simplifying2014.pdf`中用于DLP的algorithm 2中计算E的定义有误，E中的元素应该是`head(r) \ {p | ~p \in L}`，而不是`head(r)`。前者有效地保证了当前的计算过程在执行uclp的递归时的新X的size必定比当前X的要下，这样便保证了不会死循环。

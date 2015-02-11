## 简介
main函中有三个宏，其分别对应这`theoryFiles`文件夹中三个文档的理论实现和测试。
- MAXU&nbsp;&nbsp;&nbsp;:&nbsp;实现`/theroyFiles/MaxU.pdf`中第三部分描述的计算一个符合条件的最大的U。
- GRST&nbsp;&nbsp;&nbsp;:&nbsp;实现`/theroyFiles/Simplifying2014.pdf`中给出的计算NLP和DLP的greatest reliable set的算法。
- GRS_WS&nbsp;:&nbsp;实现`/theroyFiles/ijcai15_simplifying.pdf`中第6部分给出的计算NLP和DLP的greatest strong/weak reliable set的算法。

注意：
>`/theroyFiles/Simplifying2014.pdf`中用于DLP的algorithm 2中计算E的定义有误，E中的元素应该是`head(r) \ {p | ~p \in L}`，而不是`head(r)`。前者有效地保证了当前的计算过程在执行uclp的递归时的新X的size必定比当前X的要下，这样便保证了不会死循环。

## 当前版本信息
当前程序的版本和IO中存放的结果是没有执行`lookahead`和舍弃了计算DLP的GWRS和GSRS中algorithm 1里的`for each p \in C, C \subseteq RS^w_P,L(X \cup {p})`的。

在PSRunning中的有两个编译版本，其中：
- programSimplificationL&nbsp;&nbsp;:&nbsp;仅加上了`lookahead`。
- programSimplificationLF&nbsp;:&nbsp;加上了`lookahead`和DLP中algorithm 1里的`for each p \in C, C \subseteq RS^w_P,L(X \cup {p})`。

上诉两个新编译版本在运行起来会很慢，暂时没有结果。另外注意：执行这两个版本应该在两个独立的文件夹中，否则后者会覆盖前者的结果，应该输出都在`IO/outputs/`下对应的文件夹中。

## 程序化简部分
<ol>
<li>将那些consequence作为约束，加入到P，得到P1;</li>
<li>通过GWRS化简P，将剩下的consequence作为约束加入到化简后的程序中，得到P2;</li>
<li>然后使用clasp/claspD求解程序P1和P2，比较两者的速度，我们的目标是P2更快些。</li>
</ol>
其中一些细节：
- `将那些consequence作约，加入到P`的含义是：如果是事实 fact，则加入 :- not fact. 如果是 否定式如 -p，则加入 :- p.。
由于我们目前的实验只处理正的结果，所以，就是将 fact 和其他正的 consequence p， 以  :- not p. 的形式加入原始程序。
- `剩下的 consequence `指：我们已经计算出一个 consequence 的集合 C，基于 C 计算 GWRS S，如果程序没有出错，应该 S \subseteq C，所以我们用 S 化简 P 得到程序 P'，再将 C- S (即，剩下的 consequence) 作为约束加入 P'，得到程序 P2.‍

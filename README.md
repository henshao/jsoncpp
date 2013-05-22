#缘由

JSON+UTF-8是一种非常好的组合，我们在后台服务器比较多地使用这种组合。[JsonCpp](http://jsoncpp.sourceforge.net/)是一个非常优秀的开源项目，代码非常简洁，使用方式也很直观。Google Chrome使用了JsonCpp。 

我们在后台的服务器上使用JsonCpp生成json格式时，发现它的效率是有问题的。format 40KB的数据，平均耗时10ms。为了提高JsonCpp的writer性能，我们想了很多方法，做了一些优化，效果很明显。

本来是想将这些优化合并到trunk上，但是提交的patch很久都没有得到回复。我们注意到JsonCpp已经属于公共领域（LICENSE文件中有注明），于是我们想在github上fork一个项目出来，让更多的人受益。

#主要的改进点

1. SConstruct中将platform设置为'linux-gcc'，并且设置'-O2'优化参数。这是最简单有效的优化方法，可惜很多人都不知道这点。
2. 增加Value::setValue接口，提高存入字符串类型的效率。
3. 优化JsonCpp对StaticString类型的处理，对StaticString类型不做深拷贝。
4. 优化valueToQuotedString函数，提高处理字符串类型的效率。
5. 优化FastWriter对objectValue类型的处理。

#benchmark

我在src/benchmark目录下放置了一个测试程序，由于不是很懂scons，不知道怎么增加一个target，可以实现输入这条'scons benchmark'命令就可以运行一次。如果有熟悉scons的朋友可以帮我们搞一下。

使用JsonCpp官方的0.6.0rc2版本作对比，结果如下。数据是在一台装备Intel(R) Xeon(R) CPU           E5420，Linux内核版本是2.6.18的服务器上得到的。在高并发的服务上运行，实际的性能差距还会更大。

```
官方原版本：     10,781ms
官方-02优化版本： 6,950ms
本版本：         3,210ms
```

#值得注意的几点

1. 尽量使用引用（可以参考benchmark例子的代码），否则JSON的层数越多，构造Value的开销越大。
2. 可以设计两个outfit，'outfmt=json'调用FastWriter生成紧凑的JSON字符串；'outfmt=json2'调用StyledWriter生成美观对齐的JSON字符串以供调试。

#以后的计划

我们会不定期合并官方trunk上的新功能。
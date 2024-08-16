# 第三次大作业

本次作业实现一个端对端加密的聊天函数，首先完成其需要的密码学函数，然后补全端对端加密的功能。

1. 请你把第二次大作业的 aes-128, aes_ctr-128 和 sha256 移植到该项目中（如果你不移植能做后面的任务也可以）
2. （15分）阅读HMAC的文档，实现基于 sha256 的 PRF-HMAC-SHA-256。要求达到 100 Mbps 效率
3. （25分）阅读AES-GCM的文档，实现 AES-GCM-128。要求达到 50 Mbps 效率
4. （20分）阅读HKDF文档，实现基于 HMAC-SHA256 的 HKDF。要求达到 100 Mbps 效率
5. （20分）阅读 Double ratchet 的文档，并补全 doubleratchet.cpp 中的 on_receiving_message 函数，以及 messenger.cpp 中的 receive_message 函数。 
6. （20分）简答问题
    1. What is Forward security? How does Double ratchet achieve Forward security?
    2. What is Break-in Recovery? How does Double ratchet achieve Break-in Recovery?
    3. What if they never update their DH keys at all? Please explain the security consequences of
    this change with regards to Forward Secrecy and Break-in Recovery.
    4. Consider the following conversation between Alice and Bob, protected via the Double Ratchet
    Algorithm according to the spec:
    A: Hey Bob , can you send me the locker combo?
    A: I need to get my laptop
    B: Sure , it’s 1 2 3 4!
    A: Great , thanks! I used it and deleted the previous message .
    B: Did it work?
    What is the length of the longest sending chain used by Alice? By Bob? Please explain.
    5. Unfortunately, in the situation above, Mallory has been monitoring their communications
    and finally managed to compromise Alice’s phone and steal all her keys just before she sent
    her third message. Mallory will be unable to determine the locker combination. State and
    describe the relevant security property and justify why double ratchet provides this property.

一部分测例已经给出，部分测例隐藏。你的 aes-gcm, hkdf, hmac 代码需要支持 <= 8Mbit (8\*1024\*1024 bit) 的输入。我们实现了测试效率的代码，会自动输出效率。

部分代码已给出，运行方法：
可以用 `make all` 命令来编译所有
也可以用 `make messenger-test`, `make messenger`, `make aes-gcm`, `make hmac`, `make hkdf` 命令来分别编译

`bin/messenger` 是一个交互式的消息测试，你可以在每一轮指定消息的发送者 （用 `'a'`, `'b'`）和消息，并且观察消息解密是否正确。

运行 `bin/messenger-test` 可以跑一次 messenger，会让 alice 和 bob 互相发随机消息。正确运行的输出应当如下，
```
Check res is 1, Bob received message is Hello, bob.
Test #0, Bob sent Name a random thing: 316185601, Alice received Name a random thing: 316185601, check res is 1.
Test #1, Alice sent Name a random thing: 66078249, Bob received Name a random thing: 66078249, check res is 1.
Test #2, Alice sent Name a random thing: 803092385, Bob received Name a random thing: 803092385, check res is 1.
Test #3, Bob sent Name a random thing: 1926406879, Alice received Name a random thing: 1926406879, check res is 1.
Test #4, Bob sent Name a random thing: 772865205, Alice received Name a random thing: 772865205, check res is 1.
Test #5, Bob sent Name a random thing: 1385504475, Alice received Name a random thing: 1385504475, check res is 1.
Test #6, Bob sent Name a random thing: 2086581062, Alice received Name a random thing: 2086581062, check res is 1.
Test #7, Bob sent Name a random thing: 1667716779, Alice received Name a random thing: 1667716779, check res is 1.
Test #8, Bob sent Name a random thing: 1539409249, Alice received Name a random thing: 1539409249, check res is 1.
Test #9, Bob sent Name a random thing: 380044886, Alice received Name a random thing: 380044886, check res is 1.
Test #10, Alice sent Name a random thing: 789968397, Bob received Name a random thing: 789968397, check res is 1.
Test #11, Alice sent Name a random thing: 1079604554, Bob received Name a random thing: 1079604554, check res is 1.
Test #12, Alice sent Name a random thing: 649835874, Bob received Name a random thing: 649835874, check res is 1.
Test #13, Alice sent Name a random thing: 1957603423, Bob received Name a random thing: 1957603423, check res is 1.
Test #14, Bob sent Name a random thing: 392658855, Alice received Name a random thing: 392658855, check res is 1.
Test #15, Alice sent Name a random thing: 913904395, Bob received Name a random thing: 913904395, check res is 1.
Test #16, Bob sent Name a random thing: 768055815, Alice received Name a random thing: 768055815, check res is 1.
Test #17, Alice sent Name a random thing: 1095184612, Bob received Name a random thing: 1095184612, check res is 1.
Test #18, Alice sent Name a random thing: 86331876, Bob received Name a random thing: 86331876, check res is 1.
Test #19, Alice sent Name a random thing: 1076388363, Bob received Name a random thing: 1076388363, check res is 1.
```
其他程序的运行方法：
`bin/aes-gcm <input-file> <output-file>`
如 

`# bin/aes-gcm data/aes_gcm_1.txt output/aes_gcm_1.txt`

`# bin/hmac data/hmac_1.txt output/hmac_1.txt`

`# bin/hkdf data/hkdf_1.txt output/hkdf_1.txt`

可以用 `test.sh` 批量运行，`test.sh` 会批量执行 `data/` 里面的所有测例，并且输出到 `output/`
Windows 系统可以在终端用 `> bash test-win.sh`。`test-win.sh` 和 `test.sh` 的区别是可执行文件后面有 `.exe` 后缀。

你可以修改的文件：

- src/aes.cpp
- src/aes-ctr.h
- src/aes-gcm.cpp
- src/double-ratchet.cpp 中的 on_receiving_message 函数
- src/hkdf.cpp
- src/hmac.cpp
- src/messenger.cpp 中的 receive_message 函数
- src/sha256.cpp
- include/aes.h
- include/aes-ctr.h
- include/sha256.h

提交时，打包你的代码，`output/` 文件夹，和报告（报告里只需要回答简答题）。

## CHANGELOG
2023.5.23: 将 `src/apps/aes-gcm-app.cpp` 第 80 行的输出 tag 长度由 24 改为 16。更新了 `data/aes_gcm_6-10.txt` 中，删掉了 tag 长度多于 16 byte 的部分。

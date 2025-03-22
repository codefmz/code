# 面试题

1. 常用那些指令

   拉仓库

   git clone 

   git remote add

   

   拉分支

   git fetch

   

   切分支

   git branch 

   git checkout 

   

   推分支

   git push 

   

   提交

   git add 

   git commit

   git commit --amend

   

   暂存

   git stash save

   

   修改文件到某个特定的分支

   git checkout -- file

   git checkout commitID -- file

   

   拉取别的分支修改

   git cherry pick 

   

   修改分支节点

   git rebase 分支

   git rebase -i

   

   ---

   

## git rebase 与 git merge的区别



# GIT

- 分布式版本控制工具 

- 多人开发

---

## git 工作区域

### 版本库

- .git 目录，存放了所有的版本数据

### 暂存区

- 存放在.git\index，索引

### 工作区

- 日常工作的代码文件或者文档所在的文件夹

---

## 文件状态

1. committed：
2. modified：
3. staged : 把已经修改的文件放在下次提交保存的清单里面

---

## 常用命令

### 初始化

| **作用**                       | 命令                                     |
| ------------------------------ | ---------------------------------------- |
| 初始化本地库（创建一个新的库） | git init                                 |
| 从远端拷贝库                   | git clone  <RemteURL>                    |
| 创建创库别名                   | git remote add <AnShortName> <RemoteURL> |

> 区别：

### 文件修改后提交推送

| 作用                             | 命令                                            |
| -------------------------------- | ----------------------------------------------- |
| 新增文件到暂存区                 | git add <file>                                  |
| 从暂存区中移出                   | git restore --staged <file>                     |
| 从暂存区中删除（文件会直接删除） | git rm                                          |
| 移动                             | git mv                                          |
| 提交                             | git commit                                      |
| 合并上一个提交                   | git commit --amend                              |
| 推送到远程仓库                   | git push <远程主机名> <本地分支名>:<远程分支名> |

> git commit  输入之后会出现vim 编辑提交信息
>
> :question: 已经提交了多个版本，怎么合并到一起

git push -force : 强制提交



### 分支管理

| 作用                     | **命令**C                                       |
| ------------------------ | ----------------------------------------------- |
| 创建分支                 | git branch -b                                   |
| 新建分支并切换           | git checkout -b                                 |
| 删除分支                 | git branch -d                                   |
| 查看分支                 | git branch                                      |
| 查看分支以及它所在的版本 | git branch -v                                   |
| 更新分支                 | git pull <远程主机名> <远程分支名>:<本地分支名> |
| 合并分支                 | git merge                                       |
| 查看修改                 | git diff                                        |
| 合并其他分支的东西       | git cherry-pick <commit id>                     |
| 分支改名                 | git branch -m                                   |
| 以另一个分支作为基线     | git rebase -i                                   |
|                          |                                                 |

> git cherry-pick -m 1: cherry pick一个merge

#### git diff

1. 比较当前分支与远程分支的修改

   ```bash
   git diff HEAD..C/master
   ```

2. 比较工作文件与暂存区的差异

   ```bash
   git diff 
   ```

3. 暂存区与最新提交

   ```bash
   git diff --staged
   ```



### 日志

| 作用         | **命令**       |
| ------------ | -------------- |
| 详细日志     | git log        |
| 最近一条     | git log -1     |
| 简略         | git reflog     |
| 对文件的操作 | git log --stae |
| 详细操作     | git log -p     |

---

## 配置ssh

1. 新生成key

```bash
ssh-keygen -t ed25519 -C "邮箱名字"  # ed25519 是一种数字签名算法 如果不支持，可以使用   也可用rsa替代
```

2. 两次确认

   - 第一次输入问这个key要存放的位置， 不输入为默认位置
   - 第二次输入设置密语，不输入为空
   - 一般直接跳过

3. 复制ssh key

   - 通过日志到key文件中复制 *.pub中的内容

4. 到`gitHub`中配置复制的key

   - SSH and GPG Keys

---

## 其他操作

### git 提交了不该提交的东西

```ABAP
git rm -r --cached 文件

git add 文件

git commit
```

---

### git rebase 操作

![git-rebase.png](https://waynerv.com/posts/git-rebase-intro/git-rebase-visual.png)

| 操作                                    | 命令                                               |
| --------------------------------------- | -------------------------------------------------- |
| git  rebase -i HEAD ~N                  | 交互重写历史（将pick 修改为 squash 合并多个commit) |
| git rebase master                       | 合并master的修改，以master最新的修改作为基线       |
| git checkout master && git merge cstudy | 切换并合并                                         |





## ignore 文件

### vscode - c

```json
# Created by https://www.toptal.com/developers/gitignore/api/c,visualstudiocode
# Edit at https://www.toptal.com/developers/gitignore?templates=c,visualstudiocode

### C ###
# Prerequisites
*.d

# Object files
*.o
*.ko
*.obj
*.elf

# Linker output
*.ilk
*.map
*.exp

# Precompiled Headers
*.gch
*.pch

# Libraries
*.lib
*.a
*.la
*.lo

# Shared objects (inc. Windows DLLs)
*.dll
*.so
*.so.*
*.dylib

# Executables
*.exe
*.out
*.app
*.i*86
*.x86_64
*.hex

# Debug files
*.dSYM/
*.su
*.idb
*.pdb

# Kernel Module Compile Results
*.mod*
*.cmd
.tmp_versions/
modules.order
Module.symvers
Mkfile.old
dkms.conf

### VisualStudioCode ###
.vscode/*
!.vscode/settings.json
!.vscode/tasks.json
!.vscode/launch.json
!.vscode/extensions.json
!.vscode/*.code-snippets

# Local History for Visual Studio Code
.history/

# Built Visual Studio Code Extensions
*.vsix

### VisualStudioCode Patch ###
# Ignore all local history of files
.history
.ionide

# End of https://www.toptal.com/developers/gitignore/api/c,visualstudiocode
```



## 更新`.gitignore`文件

```
rm -r --cached .
rm add .
git commit -m "update" .
```

> 先删除缓存，然后再提交


# GitHUb





# GItee





# GItLab

>  自建代码托管平台, 开源

gitlab是安装在Linux上的，需要配置虚拟机，4G内存，50G磁盘

vmware 更改虚拟机设置，修该硬件配置，扩展磁盘（需要保证没有快照）

关闭虚拟节的防火墙

> systemctl stop firewalld ：关闭防火墙
>
> systemctl status firewalld ：查看防火墙状态

修改主机ip地址(可选)

> vim /etc/sysconfig/network-scripts/ifcfg-ens33 ： 修改 IPADDR为：192.168.6.200 （现在我的是：192.168.200.130）

yum 安装包 

安装脚本

```bash
sudo rpm -ivh /opt/module/gitlab-ce-13.10.2-ce.0.el7.x86_64.rpm
sudo yum install -y curl policycoreutils-python openssh-server cronie
sudo lokkit -s http -s ssh
sudo yum install -y postfix
sudo service postfix start
sudo chkconfig postfix on
curl https://packages.gitlab.com/install/repositories/gitlab/gitlab-ce/script.rpm.sh | sudo bash
sudo EXTERNAL_URL="http://gitlab.example.com" yum -y install gitlab-ce
```

初始化gitlab服务

```bash
gitlab-ctl reconfigure
```

启动gitlab服务

```bash
gitlab-ctl start
```

使用端口号登录，或者做好映射后使用主机名登录

登录成功后，需要修改密码，默认提供root用户

如果没有提示修改密码，在服务器上设置root的密码

```bash
gitlab-rails console -e production  //进入gitlab 控制台
user = User.where(id:1).first  //将用户设置为root
user.password = 'fmwqqyx@+199'  //修改用户密码
user.password_confirmation='fmwqqyx@+199'  //确定密码
user.save   //保存设置
exit //退出
```

http://fmw/gitlab-instance-788e6b47/git_test.git

解决Clion 中git 过滤文件

---

git 本地版本落后于远程库的版本

1. git push -u 远程库名字 分支
2. git pull 获取远程库名字
3. 创建新分支然后提交


# 文件加密系统 - 服务端
author: wkyo

##通信规范

内容字节长度(unsigned short) + 传递的内容(转化为字节)

命令(unsigned short)

- login: 0 + sizeof(user) + user + sizeof(password) + password, 
	answer: 0 or 1 + sizeof(msg) + msg
- logout: 1 + sizeof(user) + user + sizeof(password) + password
- pullpwd: 10 + filemd5(128bits/16bytes), answer: filepwd(128bits/16bytes)
- pushpwd: 11 + filemd5(128bits/16bytes) + filepwd(128bits), answer: 0 or 1



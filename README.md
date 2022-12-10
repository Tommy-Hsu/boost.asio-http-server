# boost.asio-http-server
## 觀念問題 1 - 實作 http_server 的順序

    讀取 request - 剖悉 request - fork child process - setenv - 處理 socket (dup) - 結束

## 觀念問題 2 - 同步函數與非同步函數作用之差異，以及非同步函數會如何處理內部 call-back function。

    同步函數需要等待(blocking-waiting)回傳值，非同步函數可以在等待回傳時，先處理其他函式，但是必定會完整執行完 call-back function

## demo 題目
![S__44335107](https://user-images.githubusercontent.com/65523042/206854873-22be343f-ea78-4f69-baa9-1a7d402a56d6.jpg)

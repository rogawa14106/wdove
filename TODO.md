# TODO

## フォルダ構成 ○

`src`等の名前のディレクトリ以下にとりあえずヘッダ/ソースを置く  
機能別にサブディレクトリを構成して、ヘッダ/ソースをセットにしてまとめる

## HTTPリクエストができるようになる○

泥臭く、リテラルでGETリクエストのヘッダを作って阿部ひろしサイトを取得  
→完了。Httpヘッダの最小条件もわかった。

## 名前解決関数切り出し○

#incldue <netdb.h>
gethostbyname(const \*char)
→完了

## 引数からURLやメソッドを受け取って、パースするようにする。○

済：URLパース用関数作成中。(url.c)
可変長の構造体のメモリ割当をどうするのが普通なのかイマイチわからん。
->calloc,mallocでいいのか。

以下2パターン以外では、正常にパースすることに成功。

```
# ./build/$(PROG) "http://abehiroshi.la.coocan.jp/top.htm?id=10&name=rogawa"
# ./build/$(PROG) "http://abehiroshi.la.coocan.jp?id=10&name=rogawa"
```

->mallocをcallocに変えたらうまく行った。このあたりもっと調べないと今後たくさんバグ踏みそう。  
このあたりのエラーが出た。

- malloc() invalid size
- address of stack memory associated with local variable returned

## GETリクエスト周りの実装を整える ← ここから

クエリパラメータどこにつくか確認。pathのとこでいいのかな。

## CLIでHTTPメソッド受け取れるようにする。

Curlみたいに、-X <method> みたいなんでいい。指定なかったらGET。

## ほかメソッドも実装。一般化

POST  
json送る

## HTTPS対応

## Cookie対応

### 開発メモ

- 使う型には、wd\_~~\_tをつけるようにする。

```c
    // こんな感じ
    typedef struct {
        char* scheme;
    } wd_url_t;
```

- このフラグ書き方いい。フラグを0,1,2,4,8とかで書くより読みやすい。

```c
  #define H1_PARSE_OPT_NONE (0)
  #define H1_PARSE_OPT_STRICT (1 << 0)
```

### CURLメモ

- CURLUcode
  curl/include/curl/urlapi.h
  エラーコード

- CURLU
  urlの型

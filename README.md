# MQTTクライアントを作りながら学ぶソケットプログラミング入門 サポートページ

本リポジトリは技術書典18で頒布する技術同人誌「MQTTクライアントを作りながら学ぶソケットプログラミング入門」のサポートページです。

![表紙・裏表紙]()

## ディレクトリ構成

### echo

「2章 ソケットプログラミング」で作成する、Rubyで書かれたechoサーバーのプログラムです。

- echo/lib/server.rb
  - echoサーバーの処理が記載されています
- echo/lib/client.rb
  - クライアントの処理が記載されています

### mqtt/ruby

「4章 クライアント開発(Ruby編)」で作成する、Rubyで書かれたMQTTクライアントのプログラムです。

- mqtt/ruby/lib/tcp.rb
  - TCP接続・切断処理がメソッドとして定義されています
- mqtt/ruby/lib/mqtt.rb
  - MQTTの各種パケットを送受信する処理がメソッドとして定義されています
- mqtt/ruby/lib/mqtt_publish/main.rb
  - パブリッシャーのクライアントプログラムです
- mqtt/ruby/lib/mqtt_publish/main.rb
  - サブスクライバーのクライアントプログラムです

### mqtt/c

「5章 クライアント開発(C言語編)」で作成する、C言語で書かれたMQTTクライアントのプログラムです。

- mqtt/c/include
  - ヘッダファイルを格納しているディレクトリです
- mqtt/c/src/tcp.c
  - TCP接続・切断処理が関数として定義されています
- mqtt/c/src/mqtt.c
  - MQTTの各種パケットを送受信する処理が関数として定義されています
- mqtt/c/src/mqtt_publish/main.c
  - パブリッシャーのクライアントプログラムです
- mqtt/c/src/mqtt_subscribe/main.c
  - サブスクライバーのクライアントプログラムです

## 正誤表

書籍の誤植が見つかった場合、こちらに追記予定です。

## 指摘・質問がある場合

[本リポジトリのIssues](https://github.com/yuuu/mqtt-socket-programming-book-site/issues)に書き込みをお願いいたします。

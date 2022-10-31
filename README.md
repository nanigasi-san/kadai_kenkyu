# kadai_kenkyu
C言語を用いた画像のコピーや輪郭抽出の実装

## 輪郭抽出のアルゴリズム
モノクロ画像において、ピクセル同士を比較して、周辺との輝度の差が一定以上ある場合に輪郭を構成するピクセルと判定する。

## まとめ
1. range=10くらいがちょうどよい
2. 画質を上げると複雑な部分(羽など)が綺麗に取れる, 256to512LENNA ひかく
3. シンプルな塗りつぶされた矩形などについてもやってみました(MS Paint)
# 圧縮
mkdir img
cp *.img img
zip -r img.zip img

# 送る
scp ../sl.wasm osc-sl:~/repo/sl-wasm
scp img.zip osc-sl:~/repo/sl-wasm/wasm/sent-img.zip

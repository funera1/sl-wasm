# 圧縮
mkdir img
mv *.img img
zip -r img.zip img

# 送る
scp img.zip osc-raspi:~/repo/sl-wasm/wasm/sent-img.zip

# 圧縮
rm -rf img
rm img.zip
mkdir img
cp *.img img
zip -r img.zip img

# 疎通確認
# SSH config file path
SSH_CONFIG_FILE="$HOME/.ssh/config"
TARGET_HOST="osc-sl"

# Check if the SSH config file exists
if [ ! -f "$SSH_CONFIG_FILE" ]; then
  echo "SSH config file not found: $SSH_CONFIG_FILE"
  exit 1
fi

# Function to extract IP address for the specific host from SSH config file
extract_ip_for_host() {
  awk -v host="$TARGET_HOST" '
    $1 == "Host" && $2 == host { in_host = 1; next }
    in_host && $1 == "HostName" { print $2; exit }
    $1 == "Host" { in_host = 0 }
  ' "$SSH_CONFIG_FILE"
}

# Extract the IP address
IP_ADDRESS=$(extract_ip_for_host)

# Check if the IP address was found
if [ -z "$IP_ADDRESS" ]; then
  echo "IP address for host $TARGET_HOST not found in SSH config file."
  exit 1
fi

# Ping the IP address and display the result
echo "Pinging IP address: $IP_ADDRESS"
ping -c 1 -w 10 "$IP_ADDRESS"
if [ $? -eq 0 ]; then
  echo "IP address $IP_ADDRESS is reachable."
else
  echo "IP address $IP_ADDRESS is not reachable."
  exit 1
fi

# 送る
echo "Sending file..."
scp ./*table* osc-sl:~/repo/sl-wasm/wasm
scp ../sl.wasm osc-sl:~/repo/sl-wasm
scp img.zip osc-sl:~/repo/sl-wasm/wasm/sent-img.zip

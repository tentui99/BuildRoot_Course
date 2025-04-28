if [ $# -ne 2 ]; then
    echo "Error, Please provide exactly two arguments"
    exit 1
fi

writefile=$1
writestr=$2

writedir=$(dirname "$writefile")

if [ -d "$writedir" ]; then
    echo "Directory already exists: $writedir"
else
    mkdir -p "$writedir"
    if [ $? -ne 0 ]; then
        echo "Failed to create directory: $writedir"
        exit 1
    fi
fi

echo "$writestr" > "$writefile"

if [ $? -ne 0 ]; then
    echo "Could not write to file: $writefile"
    exit 1
fi

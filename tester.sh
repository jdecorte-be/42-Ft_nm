# !/bin/bash

DIR="./bin"

NM_OUTPUT="/tmp/nm_output.txt"
FT_NM_OUTPUT="/tmp/ft_nm_output.txt"

touch "$NM_OUTPUT" "$FT_NM_OUTPUT"

for file in "$DIR"/*; do
    if [[ -x "$file" ]]; then
        echo "Testing $file..."

        nm "$file" > "$NM_OUTPUT" 2>&1
        ./ft_nm "$file" > "$FT_NM_OUTPUT" 2>&1

        diff "$NM_OUTPUT" "$FT_NM_OUTPUT" > /dev/null

        if [[ $? -ne 0 ]]; then
            echo "Difference detected for $file"
            diff "$NM_OUTPUT" "$FT_NM_OUTPUT"
        else
            echo "$file is OK"
        fi
    fi
done

rm "$NM_OUTPUT" "$FT_NM_OUTPUT"

#!/bin/bash

# Check if the correct number of arguments is provided.
if [ "$#" -ne 1 ]; then
    echo "❌ Usage: $0 <directory>"
    exit 1
fi

DIR="$1"

NM_OUTPUT="/tmp/nm_output.txt"
FT_NM_OUTPUT="/tmp/ft_nm_output.txt"

# Function to perform cleanup on exit.
cleanup() {
    rm -f "$NM_OUTPUT" "$FT_NM_OUTPUT" valgrind_output.txt
}

# Trap any exit signal to perform cleanup.
trap cleanup EXIT

# Check if the directory exists.
if [[ ! -d "$DIR" ]]; then
    echo "❌ Directory $DIR does not exist!"
    exit 1
fi

# Iterate over all files in the directory.
for file in "$DIR"/*; do
    if [[ -x "$file" ]]; then
        echo "🔍 Testing $file..."

        # Run nm and ft_nm and redirect their outputs to files.
        nm "$file" > "$NM_OUTPUT" 2>&1
        NM_EXIT_STATUS=$?

        # Run ft_nm with valgrind and check for errors.
        valgrind --error-exitcode=1 --log-file=valgrind_output.txt ./ft_nm "$file" > "$FT_NM_OUTPUT" 2>&1
        FT_NM_VALGRIND_EXIT_STATUS=$?
        FT_NM_EXIT_STATUS=$?

        # If valgrind reports an error, output its log.
        if [[ $FT_NM_VALGRIND_EXIT_STATUS -ne 0 ]]; then
            echo "❌ Valgrind reported errors for $file"
            cat valgrind_output.txt
        fi

        # Skip diff if both commands produce an error.
        if [[ $NM_EXIT_STATUS -ne 0 && $FT_NM_EXIT_STATUS -ne 0 ]]; then
            echo "✅⚠️  Both commands produced an error for $file"
        # Compare the outputs of nm and ft_nm.
        elif diff -q "$NM_OUTPUT" "$FT_NM_OUTPUT" > /dev/null; then
            echo "✅ $file is OK"
        else
            echo "❌ Difference detected for $file"
            diff "$NM_OUTPUT" "$FT_NM_OUTPUT"
        fi
    fi
done